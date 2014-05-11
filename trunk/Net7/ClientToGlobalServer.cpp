// ClientToGlobalServer.cpp

/***********************************************
 *   ///////////////////////////////////////   *
 *   //  CLIENT TO GLOBAL SERVER OPCODES  //   *
 *   ///////////////////////////////////////   *
 ***********************************************/

#include "Net7.h"
#include "Connection.h"
#include "Opcodes.h"
#include "ServerManager.h"
#include "PacketStructures.h"
#include "PlayerClass.h"
#include "PacketMethods.h"

void Connection::GlobalError(int Error)
{
	static char _ErrorMsg[][200] = 
	{
		{"Error: You have been temporarily banned."},
		{"Sorry, that name has already been taken. Please try again."},
		{"Sorry, your name can only contain the letters a-z. No spaces or other special characters are allowed.  Please try again."},
		{"Sorry, this name is too short. It must contain at least 3 characters. Please try again."},
		{"Sorry, this name needs enough vowels (a,e,i,o,u & y) to be pronouncable. Please try again."},
		{"Sorry, there are too many repeating characters in this name. Please try again."},
		{"Sorry, this name contains a reserved or illegal word. Please try again."},
		{"Error: Ticket Validation Failed."},
		{"Error: Authentication Server (AUTHD) is unavailable.  Try again in a few minutes."},
		{"Error: You have not compleated registration."},
		{"Sorry, that ship name is not allowed. Please try again"},
		{"Sorry, Net7 experienced an internal error. Please submit a bug report"},
		{"Sorry, The Stress test is now closed."}
	};

    //TODO: Recode this to use new packet builder methods
	char Buffer[1024];
	char *p = Buffer;

	*((int *) p) = strlen(_ErrorMsg[Error]); p+= 4;
	*((int *) p) = ntohl(Error+7); p+= 4;

	memcpy(p, _ErrorMsg[Error], strlen(_ErrorMsg[Error]));
	p+=strlen(_ErrorMsg[Error]);

	SendResponse(ENB_OPCODE_0075_GLOBAL_ERROR, (unsigned char *) &Buffer, (p - Buffer));

	LogDebug("GlobalError: %s\n", _ErrorMsg[Error]);
}


void Connection::ProcessGlobalServerOpcode(short opcode, short bytes)
{
    switch (opcode)
    {
    case ENB_OPCODE_0000_VERSION_REQUEST :
        HandleVersionRequest();
        break;

    case ENB_OPCODE_0035_MASTER_JOIN :
        // TODO: figure out why the client sometimes sends a Master Join
        // packet to the Global Server.  The Master Join packet should only
        // go to the Master Server.  Perhaps we should dump this oddball to
        // a data file so we can study it.
        //LogMessage("GlobalServer received MasterJoin packet from client\n");
        break;

    case ENB_OPCODE_006D_GLOBAL_CONNECT :
        HandleGlobalConnect();
        break;

    case ENB_OPCODE_006E_GLOBAL_TICKET_REQUEST :
        HandleGlobalTicketRequest();
        break;

    case ENB_OPCODE_0071_GLOBAL_DELETE_CHARACTER :
        HandleDeleteCharacter();
        break;

    case ENB_OPCODE_0072_GLOBAL_CREATE_CHARACTER :
        HandleCreateCharacter();
        break;

    default:
        LogMessage("ProcessGlobalServerOpcode -- UNRECOGNIZED OPCODE 0x%04x\n", opcode);
        break;
    }
}

void Connection::HandleVersionRequest()
{
    VersionRequest * request = (VersionRequest *) m_RecvBuffer;

    long major = ntohl(request->Major);
    long minor = ntohl(request->Minor);

    if (major == 42 && minor == 0)
    {
        // Version is good
        SendVersionResponse(0);
    }
    else if (major < 42)
    {
        // The client version is old, need to apply the patch
        SendVersionResponse(1);
    }
    else
    {
        // The client version is newer than this server.
        SendVersionResponse(2);
    }
}

void Connection::SendVersionResponse(long status)
{
    SendResponse(ENB_OPCODE_0001_VERSION_RESPONSE, (unsigned char *) &status, sizeof(status));
}

//This gets invoked when a user logs into the account AFTER being validated
void Connection::HandleGlobalConnect()
{
    char *ticket = (char *) &m_RecvBuffer[4];

    //This is a pointer assign since GetUsernameFromTicket() uses the string manager
    m_AccountUsername = g_ServerMgr->m_AccountMgr->GetUsernameFromTicket(ticket);

    if (!m_AccountUsername)
    {
		LogDebug("HandleGlobalConnect() - Invalid Username\n");
		GlobalError(G_ERROR_NET7_INTERNAL);
        return;
    }

    long account_id = g_AccountMgr->GetAccountID(m_AccountUsername);
	long account_status = g_AccountMgr->GetAccountStatus(m_AccountUsername);

	//block people entering while stress server down
	if (account_status == 0)
	{
		GlobalError(G_ERROR_STRESS_TEST_CLOSED);
        return;
	}

    if (account_id == -1)
    {
		LogDebug("HandleGlobalConnect() - Invalid AccountID\n");
		GlobalError(G_ERROR_NET7_INTERNAL);
        return;
    }

	if (account_status == -1)		// Banned
	{
		LogDebug("HandleGlobalConnect() - Banned Account\n");
		GlobalError(G_ERROR_BANNED_ACCOUNT);
		return;
	}

	if (account_status == -2)		// Complete Registration
	{
		LogDebug("HandleGlobalConnect() - Inactive Account\n");
		GlobalError(G_ERROR_INACTIVE_ACCOUNT);
		return;
	}

    SendAvatarList(g_AccountMgr->GetAccountID(m_AccountUsername));
}

void Connection::HandleDeleteCharacter()
{
    long character_slot = ntohl(*(long *) m_RecvBuffer);
	long avatar_id = g_AccountMgr->GetAvatarID(m_AccountUsername, character_slot);

	LogMessage("Delete character %d\n", avatar_id);

    g_AccountMgr->DeleteCharacter(avatar_id);

    SendAvatarList(ACCOUNT_ID(avatar_id));
}

void Connection::HandleCreateCharacter()
{
    GlobalCreateCharacter *create= (GlobalCreateCharacter *) m_RecvBuffer;

    m_AccountUsername = g_StringMgr->GetStr(create->account_username);
    long ret_val = g_AccountMgr->CreateCharacter(create);

    if (ret_val != -1)
    {
        GlobalError(ret_val);
        return;

    }

    LogDebug("New character `%s` created\n", create->avatar.avatar_first_name);

    SendAvatarList(g_AccountMgr->GetAccountID(m_AccountUsername));
}

//This sends the client the avatar list after: Logging in, deleting character, creating character
void Connection::SendAvatarList(long account_id)
{
    GlobalAvatarList avatar_list;
    g_AccountMgr->BuildAvatarList(&avatar_list, account_id);

    SendResponse(ENB_OPCODE_0070_GLOBAL_AVATAR_LIST, (unsigned char *) &avatar_list, sizeof(GlobalAvatarList));
}

//Occurs when the player selects a charachter
void Connection::HandleGlobalTicketRequest()
{
    // The player selected a character
    long char_slot = ntohl(*(long *) m_RecvBuffer);
    long avatar_id = g_AccountMgr->GetAvatarID(m_AccountUsername, char_slot);

    if (avatar_id == -1)
    {
        LogMessage("GlobalTicketRequest() Error obtaining global character slot id\n");
        return;
    }

    LogDebug("Received GlobalTicketRequest packet -- User: `%s`, Slot: %d\n", m_AccountUsername, char_slot);

    //Now finally we can allocate a player slot, since the player is logging into the game
    Player *player = g_ServerMgr->m_GlobMemMgr->GetPlayerNode(m_AccountUsername);

    if (player)
    {
		player->SetCharacterID(avatar_id);
        player->SetCharacterSlot(char_slot);
		g_PlayerMgr->SetupPlayer(player, m_IPaddr);
        player->SetAccountUsername(m_AccountUsername);
		player->SetGameID(player->CharacterID() | PLAYER_TAG);

		player->SetPlayerPortIP(0, 0);

        LogMessage("Allocated %s a playerID of 0x%08x [%d]\n", m_AccountUsername, player->GameID(), player->CharacterID());

        //Read the charachter database
        g_AccountMgr->ReadDatabase(player->Database(), avatar_id);
        player->SetName(player->Database()->avatar.avatar_first_name);
		m_AvatarID = player->GameID();

        ProcessGlobalTicket(player);

		//LogMessage("PlayerID: %d[%s] status is: %c\n", slot_id, g_ServerMgr->m_AccountMgr->GetAccountUsername(slot_id), g_ServerMgr->m_AccountMgr->GetAccountStatus(slot_id));
    }
    else
    {
		LogMessage("** Critical Error: Can't get AvatarID [%d]! **\n", avatar_id);
        SendGlobalTicket(player->GameID(), 0, 1002, false); //galaxy full
    }
}

//TODO: Recode this
void Connection::SendGlobalTicket(long avatar_id, long sector_id, long level, bool issue)
{
    GlobalTicket ticket;
    memset(&ticket, 0, sizeof(ticket));
    unsigned char *ptr_ticket = (unsigned char *)&ticket;
    int index = 0;

    // 1000 = 'Error: AvatarSelectionTask-User not authorized'
    //        'Error: You have not been authorized to play this character.'
    // 1002 = 'Error: AvatarSelectionTask-Shard is full'
    //        'Error: This galaxy is full, please select another character.'

    if (issue)
    {
        AddDataFlip4(ptr_ticket, 0, index);
    }
    else
    {
        AddDataFlip4(ptr_ticket, level, index);
    }

    index = 20;

    AddDataFlip4(ptr_ticket, avatar_id, index);
    AddDataFlip4(ptr_ticket, sector_id, index);
    index = 32;
    AddData(ptr_ticket, level, index);
    index = 48;
    AddDataS(ptr_ticket, "MY_Avatar_Ticket", index);
    
    SendResponse(ENB_OPCODE_006F_GLOBAL_TICKET, (unsigned char *) &ticket, sizeof(ticket));
}

//TODO: Recode this
void Connection::ProcessGlobalTicket(Player *player)
{
    long sector_id = ntohl(player->Database()->info.sector_id);
   
    if (1)
    {
		LogDebug("trying to connect to %d\n",sector_id);
        SectorManager * mgr = g_ServerMgr->GetSectorManager(sector_id);
        if (!mgr)
        {
            // Oops, Sector Server is offline
            // Dump the player into the Net-7 SOL station for now
            sector_id = 10711;
            mgr = g_ServerMgr->GetSectorManager(sector_id);
            if (!mgr)
            {
                LogMessage("Fatal error: Sector Servers are unreachable\n");
                return;
            }
            player->Database()->info.sector_id = ntohl(sector_id);
        }

		//long avatar_id = player->CharacterID() | PLAYER_TAG ;

		SendGlobalTicket(player->GameID(), sector_id, g_AccountMgr->GetAccountStatus(player->AccountUsername()), true);

        //LogMessage("Sending GlobalTicket packet, SectorID = %d\n", m_SectorID);

    }
    else
    {
        LogDebug("Unable to load character database for AvatarID %d\n", player->GameID());
    }
}

void Connection::ProcessProxyGlobalOpcode(short opcode, short bytes)
{
	switch (opcode)
    {
	case ENB_OPCODE_2000_ACCOUNTDATA:
		LogMessage("Received account login attempt from Net7Proxy.\n");
		HandleAccountValid(bytes);
		break;

	case ENB_OPCODE_2002_TICKET:
		LogMessage("Processing TCP Ticket.\n");
		ProcessTicketInfo(bytes);
		break;

	case ENB_OPCODE_2011_SET_PROXY_SECTOR_LINK:
		SetConnectionToProxyLink();
		break;

    case ENB_OPCODE_2004_AVATARLOGIN:
        LogMessage("Received TCP AvatarLogin confirm (start player)\n");
        HandleGlobalTicketRequest();
        break;

    case ENB_OPCODE_200B_CREATE_AVATAR:
        LogMessage("Received TCP Create Avatar packet\n");
        HandleCreateCharacter();
        break;

    case ENB_OPCODE_200D_DELETE_AVATAR:
        LogMessage("Received TCP Delete Avatar packet\n");
		HandleDeleteCharacter();
        //HandleAvatarDeleteRequest(msg, hdr, source_addr, source_port);
        break;

	case ENB_OPCODE_2008_MASTER_HANDOFF:
		LogMessage("Recieved TCP master handoff\n");
		HandleMasterHandoff();
		break;

	default:
		LogMessage("Illegal opcode from Server to Global Account connection\n");
		break;
	}
}

void Connection::HandleAccountValid(short bytes)
{
    char username[64];
    char password[64];
    int index = 0;
    char info[128] = "Valid=False\r\n";
    long length = 13;

    ExtractDataLS(m_RecvBuffer, username, index);
    ExtractDataLS(m_RecvBuffer, password, index);

    LogMessage("Account info received for: %s\n", username);

	if (username && password)
	{
        char * ticket = 0;

		unsigned char *ip = (unsigned char *)&m_IPaddr;
	    LogMessage("User '%s' logging in from IP: %u.%u.%u.%u\n", username, ip[0], ip[1], ip[2], ip[3]);

        // TODO: This is a good place to check for "banned" IP addresses.
        // TODO: we need to create a list of accounts which are logged in successfully, so Net7Proxy create/delete can't be abused

        if (ticket = g_AccountMgr->IssueTicket(username, password))
        {
		    length = sprintf(info, "Valid=TRUE\r\nTicket=%s\r\n", ticket);
            LogMessage("Valid ticket %s\n",ticket);
        }
	}

	SendResponse(ENB_OPCODE_2001_ACCOUNTVALID, (unsigned char *) &info, length);
}

void Connection::SetConnectionToProxyLink()
{
	//now set this connection link to be a sector proxy link
	m_ServerType = CONNECTION_TYPE_SECTOR_SERVER_TO_PROXY;
	LogMessage("--> Changed TCP link to sector proxy link.\n");
}

void Connection::ProcessTicketInfo(short bytes)
{
    //ok, we have a new ticket, lets validate the ticket and send the avatarlist
    char *ticket = (char *) &m_RecvBuffer;

    //This is a pointer assign since GetUsernameFromTicket() uses the string manager
    //char *account_name = g_ServerMgr->m_AccountMgr->GetUsernameFromTicket(ticket);
	char *account_name = strtok(ticket, "-");

    if (!account_name)
    {
		LogMessage("ProcessTicketInfo() - Invalid Username\n");
		GlobalError(G_ERROR_NET7_INTERNAL);
        return;
    }

	m_AccountUsername = g_ServerMgr->m_StringMgr->GetStr(account_name);

    long account_id = g_AccountMgr->GetAccountID(account_name);
	long account_status = g_AccountMgr->GetAccountStatus(account_name);

	//block people entering while stress server down
	/*if (account_status == 0)
	{
		SendGlobalError(G_ERROR_STRESS_TEST_CLOSED);
        return false;
	}*/

    if (account_id == -1)
    {
		LogMessage("ProcessTicketInfo() - Invalid AccountID\n");
		GlobalError(G_ERROR_NET7_INTERNAL);
        return;
    }

	if (account_status == -1)		// Banned
	{
		LogMessage("ProcessTicketInfo() - Banned Account\n");
		GlobalError(G_ERROR_BANNED_ACCOUNT);
		return;
	}

	if (account_status == -2)		// Complete Registration
	{
		LogMessage("ProcessTicketInfo() - Inactive Account\n");
		GlobalError(G_ERROR_INACTIVE_ACCOUNT);
		return;
	}

    SendAvatarList(g_AccountMgr->GetAccountID(account_name));

    return;
}

void Connection::HandleMasterHandoff()
{
	LogMessage("!!! Unimplemented opcode used - Implement me if TCP master login is what you want.\n");
}