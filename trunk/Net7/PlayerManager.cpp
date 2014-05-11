// PlayerManager.cpp

#include "Net7.h"
#include "PlayerManager.h"
#include "SectorManager.h"
#include "Opcodes.h"
#include "PacketStructures.h"
#include "Connection.h"
#include <float.h>
#include "UDPconnection.h"
#include "MemoryHandler.h"
#include "ObjectClass.h"
#include "PlayerClass.h"
#include "ServerManager.h"
#include "StaticData.h"

// This helper function is referenced by _beginthread to launch the TCP thread.
#ifdef WIN32
void __cdecl RunMovementThreadAPI(void *arg)
{
    PlayerManager::MParam * param = (PlayerManager::MParam *) arg;
    PlayerManager * mgr = param->ClassAdd;
    //PlayerManager::Player *player = param->player;
    delete param;
    mgr->RunMovementThread();
    _endthread();
}
#else // Linux
void * RunMovementThreadAPI(void *arg)
{
    PlayerManager::MParam * param = (PlayerManager::MParam *) arg;
    PlayerManager * mgr = param->ClassAdd;
    //PlayerManager::Player *player = param->player;
    delete param;
    mgr->RunMovementThread();
    return NULL;
}
#endif

PlayerManager::PlayerManager()
{
	m_NextGroup = 0;
	m_GroupList = NULL;
	m_Movement_thread_running = false;
	m_Save_thread_running = false;

	memset(m_GlobalPlayerList, 0, sizeof(m_GlobalPlayerList));

   	// movement thread
    if (!m_Movement_thread_running)
    {
        MParam *param = new MParam;
        LogMessage("Creating movement thread\n");
        m_Movement_thread_running = true;
        
        if (param)
        {
            //param->ClassAdd = this;
            
#ifdef WIN32
            //_beginthread(&RunMovementThreadAPI, 0, param);
#else
            //pthread_create(&player->pThread, NULL, &RunMovementThreadAPI, (void *) param);
#endif
        }
    }
}

PlayerManager::~PlayerManager()
{
	//delete groups
	Group * g = m_GroupList;
	while (g)
	{
		Group * temp = g->next;
		delete g;
		g = temp;
	}
}

//PM
bool PlayerManager::SetupPlayer(Player *player, long IPaddr)
{
    player->ResetPlayer();
    player->SetLastAccessTime(GetNet7TickCount());
	m_PlayerLookup[player->CharacterID()] = player->GameID();

    //add this to the global player list
	SetIndex(player, m_GlobalPlayerList);

    player->SetMVASIndex(player->GameID());

    LogMessage("Confirmed MVAS connection for [%x]: MVAS node index %x\n", player->GameID(), player->MVASIndex());
    return true;    
}

void PlayerManager::DropPlayerFromSector(Player *p)
{
    //first remove the player from all the sector lists
    p->RemoveFromAllSectorRangeLists();
    //now set inactive until player revived at login
    p->SetActive(false);
    if (p->PlayerIndex()->GetSectorNum() > 0)
    {
        p->SaveData(true);
		p->UpdateDatabase();
		p->SaveAmmoLevels();
    }
    LogMessage("Player %s Removed from sector %s.\n", p->Name(), p->PlayerIndex()->GetSectorName());
}

void PlayerManager::DropPlayerFromGalaxy(Player *p)
{
    //first drop the player from the sector - this should remove the player ship from everyone's visibility
    DropPlayerFromSector(p);
	p->SavePosition();
    p->SetRemove();
	m_GlobMemMgr->ReleasePlayerNode(p);
	p->SetActive(false);
	LogMessage("Player %s Removed from Galaxy.\n", p->Name());
	UnSetIndex(p, m_GlobalPlayerList);
}

//This method is developer only for debugging.
//any players using it should expect grief!
//PM
void PlayerManager::SendPlayerWithoutConnection(long player_id)
{
	Player * p = 0;
	Player * player_to_send = 0;

    p = GetPlayer(player_id);

    //first create a new player entry.
    player_to_send = m_GlobMemMgr->GetPlayerNode();

	if (!p || !player_to_send)
    {
		LogMessage("*** Error *** NULL Pointer in SendPlayerWithoutConnection AvatarID: %d", player_id | 0x00FFFFFF);
		return;
	}

    long sector_id = p->PlayerIndex()->GetSectorNum();

    player_to_send->ResetPlayer();
    player_to_send->SetGroupID(-1);
    player_to_send->PlayerIndex()->SetSectorNum(sector_id);
    player_to_send->AddPlayerToRangeList(player_to_send); //add self to range list
    player_to_send->SetDebugPlayer();
    player_to_send->SetActive(true);

    //load some data
    player_to_send->SetCharacterID(p->CharacterID());
    memcpy(player_to_send->Database(), p->Database(), sizeof(CharacterDatabase));
    memcpy(player_to_send->Database()->avatar.avatar_first_name, "Dropdead Fred\0", 14);
    player_to_send->ReadSavedData();
    player_to_send->SetCharacterID(0);

    player_to_send->ShipIndex()->SetName("Dropdead Fred");
    player_to_send->ShipIndex()->SetGameID(player_to_send->GameID());
    player_to_send->SetName("Dropdead Fred");
    strcpy(player_to_send->Database()->ship_data.ship_name, "Summer Holiday");

    //add this to the global player list
    m_Mutex.Lock();
	SetIndex(player_to_send, m_GlobalPlayerList);
    SectorManager *SectorMgr = g_ServerMgr->GetSectorManager(sector_id);
    SectorMgr->AddPlayerToSectorList(player_to_send);
    player_to_send->SetSectorManager(SectorMgr);
    player_to_send->SetObjectManager(SectorMgr->GetObjectManager());

    m_Mutex.Unlock();

	if (player_to_send && p)
	{
        p->SetMyDebugPlayer(player_to_send);
		player_to_send->SetPosition(p->Position());
        player_to_send->SetOrientation(p->Orientation());
		player_to_send->SetVelocity(0.0f);
	}

    if (p->PlayerIndex()->GetSectorNum() > 9999)
    {
        //add to starbase
        player_to_send->SetActionFlag(65);
        player_to_send->SetOrient(0);
        p->DebugPlayerDock(true);
        //player_to_send->SendStarbaseAvatarList();
        //player_to_send->SetPosition(p->Position());
    }
}

// Finally this is OK because all the player info is protected
Player * PlayerManager::GetPlayer(long GameID, bool sector_login)
{
	long lookup_id = m_PlayerLookup[GameID & 0x00FFFFFF];
    return (m_GlobMemMgr->GetPlayerA(lookup_id, sector_login));
}

Player * PlayerManager::GetPlayerFromIndex(long index)
{
	return (m_GlobMemMgr->GetPlayerA(index, false));
}

Player * PlayerManager::GetPlayerFromCharacterID(long CharacterID)
{
	Player * p = (0);
    
	while (GetNextPlayerOnList(p, m_GlobalPlayerList))    
	{
        if ( p->CharacterID() == CharacterID )
        {
			return p;
        }
    }

	return 0;
}

Player * PlayerManager::GetPlayer(char * Name)
{
    Player * p = (0);
    Player * retval = (0);

	while (GetNextPlayerOnList(p, m_GlobalPlayerList))
    {
        if (_stricmp(p->Name(), Name) == 0)
        {
            retval = (Player*)p;
            break;
        }
    }

    return retval;
}

long PlayerManager::GetGameIDFromName(char * Name)
{
    Player * p = GetPlayer(Name);

    if (p)
    {
        return p->GameID();
    }
    else
    {
        return -1;
    }
}

//PM
void PlayerManager::ListPlayersAndLocations(Player *send_to)
{
	Player * p = (0);
	long player_count = 0;

	while (GetNextPlayerOnList(p, m_GlobalPlayerList))    
	{
		player_count++;
		Object *nearest_nav = (0);
		if (send_to->AdminLevel() >= 50)
		{
			nearest_nav = p->NearestNav();
		}

		if (nearest_nav && p->InSpace())
		{
			send_to->SendVaMessage("%s[%x] in %d Near to %s", p->Name(), (p->GameID() & 0x00FFFFFF), p->PlayerIndex()->GetSectorNum(),
				nearest_nav->Name());
		}
		else if (send_to->AdminLevel() >= 50)
		{
			send_to->SendVaMessage("%s[%x] in %d", p->Name(), (p->GameID() & 0x00FFFFFF), p->PlayerIndex()->GetSectorNum());
		}
		else
		{
			send_to->SendVaMessage("%s", p->Name());
		}
	}

	send_to->SendVaMessage("Total players = %d\n", player_count);
}

void PlayerManager::CheckForDuplicatePlayers(Player *player)
{
	Player * p = (0);
    
	while (GetNextPlayerOnList(p, m_GlobalPlayerList))    
	{
        if (player != p &&
            p->CharacterID() == player->CharacterID() )
        {
            //this player is already logged in. Remove the old player
            LogMessage("Killing old instance of player '%s' [0x%08x]\n", p->Name(), p->GameID());
            DropPlayerFromGalaxy(p);
			//now forcefully free the node
        }
    }
}

bool PlayerManager::CheckAccountInUse(char *username)
{
	Player * p = (0);
    
	while (GetNextPlayerOnList(p, m_GlobalPlayerList))    
	{
		if (strncmp(p->AccountUsername(), username, 50) == 0)
		{
			if (p->Active())
			{			
				//this player's account is already active 
				//p->ForceLogout();
				//check time of last received update, if older than 30 seconds, kill the account.
				if ((p->LastAccessTime() + 30000) < GetNet7TickCount())
				{
					LogMessage("Account user %s seems to have died. Remove so they can log in again\n", username);
					DropPlayerFromGalaxy(p);
				}
				else
				{
					LogMessage("Account user %s already active, don't allow second account\n", p->AccountUsername());
					return true;
				}
			}
			else
			{
				LogMessage("Account user %s has dead player on server, remove\n", username);
				DropPlayerFromGalaxy(p);
			}
		}
    }
	return false;
}

//Remove all players
void PlayerManager::TerminateAllPlayers()
{
	Player *p = 0;
	while (GetNextPlayerOnList(p, m_GlobalPlayerList))    
	{
		if (p)
		{
			DropPlayerFromGalaxy(p);
			long player_id = p->GameID();
			p->SendOpcode(ENB_OPCODE_100A_MVAS_TERMINATE_S_C, (unsigned char *) &player_id, sizeof(long)); //force terminate Client & Proxy.
			p->SendPacketCache();
		}
	}
}

//PM - start at startup
void PlayerManager::RunMovementThread()
{
	Player * p = (0);
	unsigned long current_tick = GetNet7TickCount();

	// Run through all players sequentially and update each in turn
	m_Mutex.Lock();

	while (GetNextPlayerOnList(p, m_GlobalPlayerList))    
	{
		//Handle player removal here 
		if (p)
		{
			if (p->GetLoginStage() == 1)
			{
				p->SetLoginStage(2);
				p->SendPacketCache();
				p->HandleLoginStage2();
			}
			else if (p->IsToBeRemoved() || (p->LastAccessTime() + 2*60000) < current_tick)
			{
				DropPlayerFromGalaxy(p);
			}
			else if (p->Active())
			{
				if (p->InSpace())
				{
					if (p->MovementID() % 5 == 0)
					{
						p->CalcNewPosition(current_tick); //update the positions

						if (!p->DebugPlayer() && (!IS_PLAYER(p->MVASIndex()) || p->WarpDrive() || p->Following() || p->PlayerUpdateSet()) ) 
						{
							p->SendLocationAndSpeed(true);
						}
						else if (IS_PLAYER(p->MVASIndex()))
						{
							p->SendLocationAndSpeed(false);
						}

						if (p->ObjectIsMoving())
						{
							p->UpdateVerbs();
						}

						if (p->MovementID() % 50)
						{
							Object *obj = p->NearestNav();
							p->SetNearestNav(obj);
						}

						p->CheckObjectRanges();
						p->UpdatePlayerVisibilityList();
						p->ResetAttacksThisTick();
					}
					p->IncrementMovementID(1);
				}
				p->SendPacketCache();
				p->CheckEventTimes(current_tick);
			}
			else if (p->DebugPlayer())
			{
				if (p->MovementID() % 5 == 0)
				{
					p->SendLocationAndSpeed(false);
					p->UpdatePlayerVisibilityList();
				}
				p->IncrementMovementID(1);
			}
			else
			{
				p->SendPacketCache();
			}
		}
	}

	m_Mutex.Unlock();
}


///////////////////////////
////    Chat System    ////
///////////////////////////

void PlayerManager::GetPostFix(Player *p, char *FName, int length)
{
	char PostFix[10];

	if (!p) return;

	switch(p->AdminLevel())
	{
	case 20:
		sprintf(PostFix, "HLP");
		break;
	case 30:
		sprintf(PostFix, "BETA");
		break;
	case 50:
		sprintf(PostFix, "GM");
		break;
	case 60:
		sprintf(PostFix, "DGM");
		break;
	case 70:
		sprintf(PostFix, "HGM");
		break;
	case 80:
		sprintf(PostFix, "DEV");
		break;
	case 90:
		sprintf(PostFix, "SDEV");
		break;
	case 100:
		sprintf(PostFix, "ADMIN");
		break;

	default:
		sprintf(PostFix, "");
		break;
	}

	if (p->Hijackee())
	{
		_snprintf(FName, length, "%s", p->GetHijackeeName());
	}
	else if (PostFix[0])
	{
		_snprintf(FName, length, "%s [%s]", p->Name(), PostFix);
	}
	else
	{
		_snprintf(FName, length, "%s", p->Name());
	}
}

void PlayerManager::ChatSendEveryone(long GameID, char *message, bool copy_to_originator)
{
    if (message)
    {
        // Find sender's name
        Player * s = GetPlayer(GameID);
		Player * p = (0);

		char FName[40];

        if (!s)
        {
            return;
        }

		GetPostFix(s, FName, 40);

        if (s)
        {
			char Channel[] = "Broadcast";		// Channel
            
			while (GetNextPlayerOnList(p, m_GlobalPlayerList))   
			{
				if ((p->GameID() != GameID) || copy_to_originator) //only send to originating player if directed to.
				{
					p->SendClientChatEvent(0x03, 0x00, FName, FName, Channel, message);
				}
			}
        }
    }
}

//PM
void PlayerManager::GMMessage(char *message)
{
	Player * p = 0;

    if (message)
    {
        //char broadcast[2048];
		m_Mutex.Lock();
		char Channel[] = "Admin Broadcast";		// Channel
            
		while (GetNextPlayerOnList(p, m_GlobalPlayerList))   
		{
			if (p->AdminLevel() >= 50)
			{
				p->SendClientChatEvent(0x03, 0x00,"GM", "GM", Channel, message);
			}
		}
        m_Mutex.Unlock();
	}
}

long PlayerManager::GetChannelFromName(char *channel_name)
{
	long channel_id = 0;
	if (!channel_name) return 0;
	char *test_name;
	long len = strlen(channel_name);
	bool found = false;

	while (test_name = ChannelNames[channel_id])
	{
		if (channel_name[0] == test_name[0]) //faster than strcmp
		{
			if (channel_name[1] == test_name[1])
			{
				if (channel_name[2] == test_name[2])
				{
					if (len <= 7 || len > 7 && channel_name[7] == test_name[7])
					{
						found = true;
						break;
					}
				}
			}
		}
		channel_id++;
	}

	if (found)
	{
		return channel_id;
	}
	else
	{
		return 0;
	}
}

//PM
void PlayerManager::ChatSendChannel(long GameID, char * Channel, char * Message)
{
    if (Message && Channel)
    {
        // Find sender's name
        Player * s = GetPlayer(GameID);
		Player * p = (0);

		char FName[40];

		GetPostFix(s, FName, 40);
		long channel_id = GetChannelFromName(Channel);

        if (s)
        {
			while (GetNextPlayerOnList(p, m_GlobalPlayerList))   
			{			
				if (p->Active() && p->IsSubscribed(channel_id))
				{
					p->SendClientChatEvent(0x03, 0x00, FName, FName, Channel, Message);
				}
			}
        }
    }
}

void PlayerManager::GlobalAdminMessage(char * Message)
{
	Player * p = 0;
    
    if (Message)
    {
        m_Mutex.Lock();
        
		while (GetNextPlayerOnList(p, m_GlobalPlayerList))   
		{
			p->SendMessageString(Message, 0x0A, false);
		}

        m_Mutex.Unlock();
    }
}

//PM
void PlayerManager::ChatSendPrivate(long GameID, char * Nick, char * Message)
{
    if (Nick && Message)
    {
        Player * p_list = (0);
        bool FoundPlayer = false;
		char Channel[] = "Private";

		if (Nick[0] == 0) return;

        Player * p = GetPlayer(GameID);

		char FName[40];

		GetPostFix(p, FName, 40);
        
        if (p)
        {
            while (GetNextPlayerOnList(p_list, m_GlobalPlayerList))  
			{
                if (!_stricmp(p_list->Name(), Nick))
                {
                    p_list->SendClientChatEvent(0x03, 0x00, FName, FName, Channel, Message);
                    FoundPlayer = true;
                    break;
                }
            }
            
            if (!FoundPlayer)
            {
                p->SendVaMessageC(17, "%s is not currently online", Nick);
            }
        }

        if (FoundPlayer)
        {
            LogChatMsg("tell> %s \"%s\"\n", Nick, Message);
        }
    }
}

//PM
char *PlayerManager::WhoHtml(size_t *response_length)
{
    // returns a list of who is online
    // first estimate how much memory we need
    int count = 0;
    size_t length = 512; // increase this if we add more formatting and decorations
    char *player = NULL;
    Player * p = (0);

    while (GetNextPlayerOnList(p, m_GlobalPlayerList))  
	{
        player = p->Name();
        count++;
        length += 128; // increase this if we display other player stuff in the table
    }
    
    char *data = (char*)_alloca(length);
    //char *data = new char[length];

	/*
    if (count == 0)
    {
        strcpy(data,
            "<html><head><title>Net-7: Who is Online</title></head><body>\r\n"
            "No players are currently online on Net-7\r\n"
            "</body></html>\r\n");
    }
    else if (count == 1)
    {
        sprintf(data,
            "<html><head><title>Net-7: Who is Online</title></head><body>\r\n"
            "The only player currently online on Net-7 is %s\r\n"
            "</body></html>\r\n", player);
    }
    else
    {
		*/
        sprintf(data,
			"<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n\r"
            "<online number=\"%d\">\r\n"
            "	<list>\r\n",
            count);
		struct in_addr in;
		p = (0);
        while (GetNextPlayerOnList(p, m_GlobalPlayerList))  
		{
			if (p)
			{
				in.S_un.S_addr = p->PlayerIPAddr();
			} 
            else 
            {
				in.S_un.S_addr = 0;
            }
            
            sprintf(data, "%s		<player name=\"%s\" ip=\"%s\" clevel=\"%d\" elevel=\"%d\" tlevel=\"%d\" race=\"%d\" profession=\"%d\"/>\r\n", data, p->Name(), 
                inet_ntoa(in),
                p->Database()->info.combat_level, p->Database()->info.explore_level, p->Database()->info.trade_level,
                p->Database()->ship_data.race, p->Database()->ship_data.profession);
            //strcat(data, "      <player name=\"");
            //strcat(data, p->Database()->avatar.avatar_first_name);
            //strcat(data, "</td></tr>\r\n");
            
        }
		strcat(data, "	</list>\r\n</online>\r\n");
        //strcat(data, "</table></body></html>\r\n");
    //}

    // Determine the actual length
    length = strlen(data);

    char header[256];
	sprintf(header,
		"HTTP/1.1 200 OK\r\n"
        "<META HTTP-EQUIV=\"Pragma\" CONTENT=\"no-cache\">\r\n"
		"Content-Type: text/html\r\n"
		"Server: AuthServer/2.5\r\n"
		"Content-Length: %d\r\n"
 		"\r\n",
		length);

    size_t header_length = strlen(header);
    *response_length = header_length + length;

    char * response = new char[header_length + length + 1]; // !heap alloc!

    if (response)
    {
	    strcpy(response, header);
        memcpy(response + header_length, data, length);
        response[header_length + length] = 0;
    }

    // done with data
    //delete [] data;

	return (response);
}

//PM
void PlayerManager::SetUDPConnection(UDP_Connection *connection)
{
	m_UDPConnection = connection;
}

//PM
void PlayerManager::SetGlobalMemoryHandler(GMemoryHandler *MemMgr)
{
    m_GlobMemMgr = MemMgr;
}

//Method to iterate along a player list
bool PlayerManager::GetNextPlayerOnList(Player *&p, u32 *player_list)
{
	//iterate along the player list
	u32 index = 0;
	u32 block_index = 0;
	u32 *entry = player_list;
	bool found = false;

	if (p)
	{
		index = (p->GetGameIndex()) + 1;
		block_index = index/32;
		entry = (u32*) (player_list + block_index);
	}

	while (index < MAX_ONLINE_PLAYERS)
	{
		//is this bit set?
		if (*entry & (1 << index%32))
		{
			p = GetPlayerFromIndex(index | PLAYER_TAG);
			if (p)
			{
				found = true;
				break;
			}
			else
			{
				*entry &= (0xFFFFFFFF ^ (1 << index%32)); //unset the bit, as it's invalid
			}
		}
		index++;

		if (*entry == 0)
		{
			//skip to start of next block
			block_index++;
			index = 32*block_index;
			entry = (u32*) (player_list + block_index);
		}
		else if (index%32 == 0)
		{
			block_index++;
			entry = (u32*) (player_list + block_index);
		}
	};

	return found;
}

//object index methods
void PlayerManager::SetIndex(Player *p, u32 *player_list)
{
	u32 index;
	if (p)
	{
		index = p->GetGameIndex();
	}
	else
	{
		return;
	}

	if (index >= 0 && index < MAX_ONLINE_PLAYERS)
	{
		u32 *entry = (u32*) (player_list + (index/(sizeof(u32)*8)));

		//now set the specific bit
		*entry |= (1 << index%32);
	}
}

void PlayerManager::UnSetIndex(Player *p, u32 *player_list)
{
	u32 index;
	if (p)
	{
		index = p->GetGameIndex();
	}
	else
	{
		return;
	}

	if (index >= 0 && index < MAX_ONLINE_PLAYERS)
	{
		u32 *entry = (u32*) (player_list + (index/(sizeof(u32)*8)));
		
		//now unset the specific bit
		*entry &= (0xFFFFFFFF ^ (1 << index%32));
	}
}

bool PlayerManager::GetIndex(Player *p, u32 *player_list)
{
	u32 index;
	if (p)
	{
		index = p->GetGameIndex();
	}
	else
	{
		return false;
	}

	if (index > MAX_ONLINE_PLAYERS) return false;

	u32 *entry = (u32*) (player_list + (index/(sizeof(u32)*8)));

	//now get the specific bit
	if (*entry & (1 << index%32))
	{
		return true;
	}
	else
	{
		return false;
	}
}

//PC Obsolete
void PlayerManager::SetSector(Player * player, long sector_id)
{
    m_Mutex.Lock();
    player->PlayerIndex()->SetSectorNum(sector_id);
    m_Mutex.Unlock();
}

