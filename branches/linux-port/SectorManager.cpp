// SectorManager.cpp
//
//	Used by the Sector Server to manage a single sector
//

#include "Net7.h"
#include "SectorManager.h"
#include "Opcodes.h"
#include "PacketStructures.h"
#include "Connection.h"
#include "TcpListener.h"
#include "ServerManager.h"
#include "ItemBase.h"
#include "SectorData.h"
#include "MemoryHandler.h"
#include "ObjectManager.h"
#include "UDPConnection.h"
#include <float.h>

// There is one instance of this class for each sector

SectorManager::SectorManager(ServerManager *server_mgr)
{
    m_ServerMgr = server_mgr;
    m_SectorData = NULL;
    m_SectorConnection = NULL;
    m_SectorID = -1;
    m_TcpPort = -1;
	m_IsSectorServerReady = false;
	m_EffectID = 0;		// Set effect ID to start at 0
	m_ShutDownMark = 0;

	m_LongTermTimeNodes.clear();

	//need to start up a timer thread
	m_SectorThreadRunning = false;
	m_MemoryMgr = new MemoryHandler(BROADCAST_SLOTS);
	//m_MemoryMgr->InitializeMemory(BROADCAST_SLOTS); //NB slots can be expanded if the sector is busy
    memset(m_EventSlotHead, 0, sizeof(m_EventSlotHead));
    m_EventSlotIndex = 0;
	m_Greetings = (0);
	m_ObjectMgr = (0);

	memset(m_PlayerList, 0, sizeof(m_PlayerList));
	m_PlayerCount = 0;
}

SectorManager::~SectorManager()
{
	m_MemoryMgr->~MemoryHandler();
}

long SectorManager::GetSectorNextObjID()
{
	return (m_ObjectMgr->GetAvailableSectorID());
}

void SectorManager::SetBoundaries(int sector)
{
    if (!m_SectorData)
    {
        return;
    }

    m_xmax = m_SectorData->m_xmax + 10000.0f;
    m_xmin = m_SectorData->m_xmin - 10000.0f;
    m_ymax = m_SectorData->m_ymax + 10000.0f;
    m_ymin = m_SectorData->m_ymin - 10000.0f;

    m_xctr = (m_xmax + m_xmin)/2;
    m_yctr = (m_ymax + m_ymin)/2;

    LogMessage("Sector: %d. X: %.2f   Y: %.2f\n", sector, m_xmax-m_xmin, m_ymax-m_ymin);
}

long SectorManager::GetSectorID()
{
    return m_SectorID;
}

short SectorManager::GetTcpPort()
{
    return m_TcpPort;
}

bool SectorManager::IsSectorServerReady()
{
    return m_IsSectorServerReady;
}

void SectorManager::SetSectorServerReady(bool ready)
{
    m_IsSectorServerReady = ready;
}

// This must be called for each instance of the sector manager
void SectorManager::StartListener(short port)
{
    m_TcpPort = port;
    m_SectorConnection = new UDP_Connection(port, m_ServerMgr, CONNECTION_TYPE_SECTOR_SERVER_TO_PROXY);;
}

bool SectorManager::SetupSectorServer(long sector_id)
{
    bool success = true;
	m_SectorID = sector_id;

	if (sector_id < 9999)
	{
		m_SectorData = g_ServerMgr->m_SectorContent.GetSectorData(sector_id);
	}
	else
	{
		m_SectorData = g_ServerMgr->m_SectorContent.GetSectorData(sector_id/10);
	}

    if (!m_SectorData)
    {
        LogMessage("SetupSectorServer sector_id=%d failed\n", sector_id);
        success = false;
    }
    else
    {
		m_SectorName = m_SectorData->name;
		m_SystemName = m_SectorData->system_name;
        m_SystemID = m_SectorData->system_id;
        m_ObjectMgr = m_SectorData->obj_manager;
		m_ParentSectorName = m_SectorData->system_name;
        if (sector_id < 9999)
        {
            m_ObjectMgr->SetSectorManager(this); //now we have a link to the sector manager via object manager
            m_ObjectMgr->InitialiseResourceContent();
        }
		else
		{
			StationTemplate *station = g_ServerMgr->m_StationMgr.GetStation(sector_id);
			m_ParentSectorName = 0;
			if (station)
			{
				if (station->WelcomeMessage) m_Greetings = station->WelcomeMessage;
				SectorData *parent_sector = g_ServerMgr->m_SectorContent.GetSectorData(sector_id / 10);
				if (parent_sector)
				{
					m_ParentSectorName = parent_sector->name;
					m_SystemName = parent_sector->system_name;
				}
				m_SectorName = station->Name;
			}
			else
			{
				LogMessage("data for station id %d seems to be missing\n", sector_id);
			}
		}
    }

    // Timed calls for manufacturing/refining need to be mad from within stations
#ifdef WIN32
	_beginthread(&RunEventThreadAPI, 0, this);
#else
	pthread_create(&m_SectorManagerThreadHandle, NULL, RunEventThreadAPI, (void *) this);
#endif

	return (success);
}

Object *SectorManager::GetObject(long object_id)
{
    return (m_ObjectMgr->GetObjectFromID(object_id));
}

Object *SectorManager::GetObject(char *object_name) //NB object_name must be in string manager
{
    return (m_ObjectMgr->GetObjectFromName(object_name));
}

ServerParameters *SectorManager::GetSectorParams()
{
    return (&m_SectorData->server_params);
}

void SectorManager::SendServerParameters(Player *player)
{
	ServerParameters parameters;
    memset(&parameters, 0, sizeof(parameters));

	parameters.ZBandMin = m_SectorData->server_params.ZBandMin;
	parameters.ZBandMax = m_SectorData->server_params.ZBandMax;
	parameters.XMin     = m_SectorData->server_params.XMin;
	parameters.YMin     = m_SectorData->server_params.YMin;
	parameters.XMax     = m_SectorData->server_params.XMax;
	parameters.YMax     = m_SectorData->server_params.YMax;

	parameters.FogNear  = m_SectorData->server_params.FogNear;
	parameters.FogFar   = m_SectorData->server_params.FogFar;
	parameters.DebrisMode = m_SectorData->server_params.DebrisMode;
	parameters.LightBackdrop = m_SectorData->server_params.LightBackdrop; //false;
	parameters.FogBackdrop = m_SectorData->server_params.FogBackdrop;
	parameters.SwapBackdrop = m_SectorData->server_params.SwapBackdrop;
	parameters.BackdropFogNear = m_SectorData->server_params.BackdropFogNear;// 0.0;
	parameters.BackdropFogFar = m_SectorData->server_params.BackdropFogFar;// 0.0;
	parameters.MaxTilt = m_SectorData->server_params.MaxTilt;
	parameters.AutoLevel = m_SectorData->server_params.AutoLevel;
	parameters.ImpulseRate = m_SectorData->server_params.ImpulseRate;
	parameters.DecayVelocity = 9.33033f;
	parameters.DecaySpin = 9.33033f;
	parameters.SectorNum = m_SectorID;
	parameters.BackdropBaseAsset = m_SectorData->server_params.BackdropBaseAsset;

	player->SendOpcode(ENB_OPCODE_0042_SERVER_PARAMETERS, (unsigned char *) &parameters, sizeof(parameters));
}

void SectorManager::HandleSectorLogin2(Player *player)
{
    if (m_SectorID > 9999)
    {
        StationLogin2(player);
    }
    else
    {
        SectorLogin2(player);
    }

    if (!player->ToBeRemoved())
    {
        AddPlayerToSectorList(player);
    }

	player->SetLoginStage(0);
}

bool SectorManager::HandleSectorLogin(Player *player)
{
    if (m_SectorID > 9999)
    {
        StationLogin(player);
    }
    else
    {
        SectorLogin(player);
    }

	player->SetLoginStage(2);

    return true;
}

void SectorManager::SectorLogin(Player *player)
{
    long GameID = player->GameID();

    player->SendGalaxyMap(m_SystemName, m_SectorName, "");
    player->SetManufactureID(0);

	player->SendClientType(m_SectorData->sector_type);

    player->PlayerIndex()->SetSectorName(m_SectorName);
    player->PlayerIndex()->SetSectorNum(m_SectorID);
    player->SendPlayerInfo();

    /*player->SendLoginShipData();

    int SpeedFactor = (m_SectorData->sector_type == ST_SPACE) ? 1 : 2;
    player->SendShipInfo(GameID, SpeedFactor);

    //player->SendDataFileToClient("ServerParameters.dat");
    SendServerParameters(player);

    // Send Navs
    if (m_SectorData)
    {
        m_ObjectMgr->SendAllNavs(player);
    }
    else
    {
        player->SendDataFileToClient("EarthSector.dat");
        player->SendDataFileToClient("EarthSectorObjects.dat");
    }

    player->SendVaMessage("We have entered %s Sector (%s System).", m_SectorName, m_SystemName);

    player->SendStart(player->CharacterID());*/
}

void SectorManager::SectorLogin2(Player *player)
{
    long GameID = player->GameID();

    player->SendLoginShipData();

    int SpeedFactor = (m_SectorData->sector_type == ST_SPACE) ? 1 : 2;
    player->SendShipInfo(GameID, SpeedFactor);

    //player->SendDataFileToClient("ServerParameters.dat");
    SendServerParameters(player);

    // Send Navs
    if (m_SectorData)
    {
        m_ObjectMgr->SendAllNavs(player);
    }
    else
    {
        player->SendDataFileToClient("EarthSector.dat");
        player->SendDataFileToClient("EarthSectorObjects.dat");
    }

    player->SendVaMessage("We have entered %s Sector (%s System).", m_SectorName, m_SystemName);

    player->SendStart(player->CharacterID());
}

void SectorManager::AddPlayerToSectorList(Player *player)
{
    PlayerList::iterator itrPList;
    bool found = false;

    m_Mutex.Lock();
	u32 player_num = player->GameID() & 0x0000FFFF;

	if (!GetIndex(player_num))
	{
		SetIndex(player_num);
		m_PlayerCount++;
	}

    m_Mutex.Unlock();
}

//only do this at the end of each sector processing cycle.
void SectorManager::RemovePlayerFromSectorList(Player *player)
{
    m_Mutex.Lock();
	u32 player_num = player->GameID() & 0x0000FFFF;

	//see if it's set
	if (GetIndex(player_num))
	{
		UnSetIndex(player_num);
		m_PlayerCount--;
	}

    m_Mutex.Unlock();
}

//object index methods
void SectorManager::SetIndex(u32 index)
{
	u32 *entry = (u32*) (m_PlayerList + (index/(sizeof(u32)*8)));

	//now set the specific bit
	*entry |= (1 << index%32);
}

void SectorManager::UnSetIndex(u32 index)
{
	u32 *entry = (u32*) (m_PlayerList + (index/(sizeof(u32)*8)));

	//now unset the specific bit
	*entry &= (0xFFFFFFFF ^ (1 << index%32));
}

bool SectorManager::GetIndex(u32 index)
{
	u32 *entry = (u32*) (m_PlayerList + (index/(sizeof(u32)*8)));

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

u32* SectorManager::GetSectorPlayerList()
{
	return m_PlayerList;
}

void SectorManager::StationLogin(Player *player)
{
    long GameID = player->GameID();
    long ManuID = player->ManuID();

    //LogMessage("Station Login: %d %s\n", m_SectorID, m_SectorName);

    //player->SendGalaxyMap(m_SystemName, m_ParentSectorName, m_SectorName);

    // TODO: Send ClientChatEvent packet for the 'local' channel

    // This is the manufacturing lab
    player->ManuIndex()->SetGameID(ManuID);
    player->ManuIndex()->TerminalReset(0);

    player->SetManufactureID(ntohl(ManuID));
	player->SetInSpace(false);

    player->SendCreate(ManuID, 1.0, -1, CREATE_MANU_LAB);
    player->SendRelationship(ManuID, RELATIONSHIP_FRIENDLY, false);
    player->SendConstantPositionalUpdate(ManuID, 0, 0, 0);
    player->SendAuxManu(true);

	// Repair ship if needed
	if (player->ShipIndex()->GetMaxHullPoints() != player->ShipIndex()->GetHullPoints())
	{
		player->ShipIndex()->SetHullPoints(player->ShipIndex()->GetMaxHullPoints());
	}

    player->PlayerIndex()->SetSectorName(m_SectorName);
    player->PlayerIndex()->SetSectorNum(m_SectorID);
    player->SendPlayerInfo();

    player->SendLoginShipData();

    // TODO: send ClientChatEvent to enable the station channel

    // TODO: send ClientChatList to show which friends are online
}

void SectorManager::StationLogin2(Player *player)
{
    long GameID = player->GameID();

    player->SendShipInfo(GameID, 1);

    player->SendMessageString(m_Greetings);
    player->SendStarbaseSet(0, 0);

    char lounge_npc[MAX_PATH];
    long lounge_npc_id = player->TryLoungeFile(m_SectorID);
    sprintf(lounge_npc, "LoungeNPC_%ld.dat", lounge_npc_id);

    if (!player->SendLoungeNPC(m_SectorID))
    {	// if we cant find it in MySQL load from file
        LogMessage("Can't load from MySQL Loading from file (%d)\n", m_SectorID);
        player->SendDataFileToClient(lounge_npc);
    }

    player->SendStart(player->CharacterID());

    // TODO: send ClientChatEvent to enable the station channel

    // TODO: send ClientChatList to show which friends are online
}

void SectorManager::LaunchIntoSpace(Player *player)
{
	char launch_msg[256];
    player->SendStarbaseSet(1, 0);
    player->SetManufactureID(0);
	if (g_ServerMgr->m_StationMgr.GetStation(m_SectorID))
	{
		sprintf(launch_msg, "This is %s control... you are cleared for departure.", g_ServerMgr->m_StationMgr.GetStation(m_SectorID)->Name);
		player->SendMessageString(launch_msg);
	}

    // TODO: ClientChatEvent to cancel the station channel
    long to_sector_id = m_SectorID / 10;
	if (m_SectorID < 9999) to_sector_id = m_SectorID;
    m_ServerMgr->m_PlayerMgr.DropPlayerFromSector(player);
    player->PlayerIndex()->SetSectorNum(to_sector_id);
    player->SendServerHandoff(m_SectorID, to_sector_id, m_SectorName, "", m_ParentSectorName, m_SystemName);
}

void SectorManager::SectorServerHandoff(Player *player, int new_sector_id)
{
    char from_sector[128];
	char *to_sector = g_ServerMgr->GetSectorName(new_sector_id);
	char *to_system = g_ServerMgr->GetSystemName(new_sector_id);
    player->SetActive(false);
    sprintf(from_sector, "%s (%s System)", m_SectorName, m_SystemName);
    m_ServerMgr->m_PlayerMgr.DropPlayerFromSector(player);
    player->PlayerIndex()->SetSectorNum(new_sector_id);
    player->SendServerHandoff(m_SectorID, new_sector_id, from_sector, m_SystemName, to_sector, to_system);
}

void SectorManager::Dock(Player *player, long target)
{
    //cancel warp
    player->TerminateWarp(true);
	Object * obj = GetObject(target);
    //LogMessage("Player requested docking at %s\n", obj->Name());

	long destination = obj->Destination();
	if (destination < 900)
	{
        player->SendVaMessage("Station has no destination set - Please add station destination in DASE.");
		return;
	}
    if (!m_ServerMgr->GetSectorManager(destination))
    {
        player->SendVaMessage("Station is offline, no access permitted.");
        return;
    }

	//TODO: add field to Station DB for docking message, use that here
	char dock_message[128];

	_snprintf(dock_message,128,"%s CONTROL: Docking clearance granted.", obj->Name());

    player->SendMessageString(dock_message);

    player->SendCameraControl(0x05000000, ntohl(target));

	player->SetVelocity(100.0f);
	player->SendContrailsRL(true);

    player->LogDockCoords();
}

void SectorManager::GateActivate(Player *player, long target)
{
	Gate(player, target);
}

void SectorManager::Gate(Player *player, long target)
{
    // Initiate Gating
    //LogMessage("Player requested Gate\n");

    // determine the destination
    Object *gate = m_ObjectMgr->GetObjectFromID(target);
    player->SetStargateDestination(gate->Destination());

	if (gate->Destination() == 0)
	{
		player->SendVaMessage("Gate Malfunction. Destination coordinates unsafe.");
		return;
	}

    if (!m_ServerMgr->GetSectorManager(gate->Destination()))
    {
        LogMessage("SECTOR OFFLINE. Gate Inoperative.\n");
        player->SendVaMessage("Destination sector is offline. Gate inoperative.");
        return;
    }

    if (gate->Destination() < 900 || gate->Destination() > 5000)
    {
        LogMessage("Stargate '%s' has invalid destination: %d\n", gate->Name(), gate->Destination());
        player->SetStargateDestination(1060); //something's gone wrong, send player to earth sector
    }

	player->OpenStargate(target);
	player->SendClientSound("sfx_generic_gate_thrust 1",0,0);
    player->SendContrailsRL(true);
    player->MoveToward(gate, 20.0f);

	//LogMessage("GATE HANDOFF SEQUENCE COMPLETE\n");
    //add timer to gate cam operation
    AddTimedCall(player, B_CAMERA_CONTROL, 5800, 0, 0x04000000, ntohl(gate->GameID()) );
}

char * SectorManager::GetSystemName(long sector_id)
{
    char *server_name = "Unknown";
    SectorData *p = g_ServerMgr->m_SectorContent.GetSectorData(sector_id);
	if (p)
	{
		server_name = p->system_name;
	}
    return (server_name);
}

char *SectorManager::GetSectorName(long sector_id)
{
    char *sector_name = "Unknown";
    SectorData *p = g_ServerMgr->m_SectorContent.GetSectorData(sector_id);
	if (p)
	{
		sector_name = p->name;
	}

    return (sector_name);
}

long SectorManager::GetSectorIDFromName(char *sector_name)
{
    long sector_id = 0;
	//loop through all the sectors
    SectorData *p = g_ServerMgr->m_SectorContent.GetSectorData(sector_name);
	if (p)
	{
		sector_id = p->sector_id;
	}

    return (sector_id);
}

//given a pointer to an array of bits and an index into them, return true if the bit is set
bool SectorManager::GetBitEntry(long *bit_array, long index)
{
	long *entry = (long*) (bit_array + (index/(sizeof(long)*8)));

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

//given a pointer to an array of bits and an index into them, return true if the bit is set
void SectorManager::SetBitEntry(long *bit_array, long index)
{
	long *entry = (long*) (bit_array + (index/(sizeof(long)*8)));

	//now set the specific bit
	*entry |= (1 << index%32);
}

void SectorManager::UnsetBitEntry(long *bit_array, long index)
{
	long *entry = (long*) (bit_array + (index/(sizeof(long)*8)));

	//now unset the specific bit
	*entry &= (0xFFFFFFFF ^ (1 << index%32));
}

void SectorManager::RechargeReactor(Player *player)
{
    player->RechargeReactor();
}

void SectorManager::RechargeShield(Player *player)
{
    player->RechargeShield();
}

void SectorManager::ForceLogout(Player *player)
{
    player->ForceLogout();
}

void SectorManager::BuffTimeout(Player *player)
{
	// TODO: This needs to be reworked

	/*
	if (player->connection)
	{
		for(int x=0;x<16;x++)
		{
			// Remove expired buffs
			if (player->player->m_AuxShip.m_BuffArray.Buffs[x].BuffRemovalTime < (signed) GetNet7TickCount()
                && !player->player->m_AuxShip.m_BuffArray.Buffs[x].IsPermanent)
			{
				// Remove effect
				//player->player->SendResponse(ENB_OPCODE_000F_REMOVE_EFFECT, (unsigned char *)&player->player->m_AuxShip.m_BuffArray.Buffs[x].Elements[0].Magnitude, sizeof(int));
				// Send to sector to remove effect
				m_ServerMgr->m_PlayerMgr.SendToSector(player, ENB_OPCODE_000F_REMOVE_EFFECT,
                    (unsigned char *)&player->player->m_AuxShip.m_BuffArray.Buffs[x].Elements[0].Magnitude, sizeof(int));
				// Remove buff from memory
				memset(&player->player->m_AuxShip.m_BuffArray.Buffs[x],0,sizeof(player->player->m_AuxShip.m_BuffArray.Buffs[x]));
			}
		}
	}
	*/
}

#define SECTOR_THREAD_INTERVAL 100
void SectorManager::RunSectorEventThread()
{
	m_SectorThreadRunning = true;
	unsigned long current_tick;
	long sleep_time;
	unsigned long check_tick = m_SectorID;
    ObjectList stations;
    ObjectList::iterator itrOList;

	if (m_ObjectMgr == (0)) return;

    ObjectList * mob_list;
    long mob_index = 0;

	//TODO: don't need an object manager for stations, but we do need an event thread.

	if (m_SectorID < 9999)
	{
		mob_list = m_ObjectMgr->GetMOBList();
		m_ObjectMgr->SectorSetup(stations);
		m_ObjectMgr->SetSectorManager(this);
	}

	do
	{
		current_tick = GetNet7TickCount();

		if (m_SectorID < 9999) //only MOBs and object effects for space & planet sectors
		{
			ProcessMOBs(current_tick, check_tick, true);

			//send station broadcasts
			for (itrOList = stations.begin(); itrOList != stations.end(); ++itrOList)
			{
				(*itrOList)->SendSoundBroadcast(check_tick);
			}
		}

        if (m_PlayerCount > 0) //only need to process timeslots if there are players present (for now - interacting MOBs will require time slots)
        {
            //first make the event calls for this node
            CallSlotEvents(m_EventSlotIndex);
			CallLongTermEvents(m_EventSlotIndex);

            //increment the slot index
            m_EventSlotIndex++;
            if (m_EventSlotIndex == (TIMESLOT_DURATION*10)) m_EventSlotIndex = 0;
        }

		check_tick++;
		sleep_time = current_tick - GetNet7TickCount() + SECTOR_THREAD_INTERVAL;
        if (check_tick > 128) check_tick = 0;
		if (sleep_time < 0) sleep_time = 0;
		Sleep(sleep_time);

    } while (!g_ServerShutdown);

	m_SectorThreadRunning = false;
}

void SectorManager::ProcessMOBs(u32 current_tick, long check_tick, bool handle_attacks)
{
	u32 index = 0;

    //update MOBs
    if (check_tick % 5 == 0) //update every 500ms
    {
		ObjectList *olist = m_ObjectMgr->GetMOBList();
		u32 mob_size = olist->size();

		Object *obj;

		while (index < mob_size && mob_size == olist->size())
		{
			obj = (*olist)[index];

            switch (obj->ObjectType())
            {
            case OT_MOB:
                ((MOB*)obj)->UpdateMOB(current_tick, handle_attacks);
                break;

            case OT_MOBSPAWN:
                index += ((MOBSpawn*)obj)->UpdateSpawn(current_tick, handle_attacks);
                break;

            default:
                LogMessage("Unknown object in MOB list: '%s' type '%d'\n",obj->Name(), obj->ObjectType());
                break;
            }
			index++;
        }
    }
}

bool SectorManager::CallSlotEvents(long index)
{
    bool called_event = false;
	TimeNodeVec::iterator itrTNode;

    //traverse this node list and call all the nodes
	for (itrTNode = m_EventSlotHead[index].begin(); itrTNode < m_EventSlotHead[index].end(); ++itrTNode)
	{
		MakeTimedCall(*itrTNode);
		called_event = true;
	}

	m_EventSlotHead[index].clear();

    return called_event;
}

bool SectorManager::CallLongTermEvents(long index)
{
    bool called_event = false;
	TimeNodeVec::iterator itrTNode;

	//now check for long term time nodes, check every 5 seconds, and only pull one off at a time
	if ( (index % 50 == 0) && (m_LongTermTimeNodes.size() > 0) )
	{
		for (itrTNode = m_LongTermTimeNodes.begin(); itrTNode < m_LongTermTimeNodes.end(); ++itrTNode)
		{
			if ( (*itrTNode)->event_time > GetNet7TickCount() )
			{
				MakeTimedCall(*itrTNode);
				m_LongTermTimeNodes.erase(itrTNode);
				called_event = true;
				break;
			}
		}
	}

	return called_event;
}

void SectorManager::AddTimedCallPNode(TimeNode *node, unsigned long time_offset)
{
	SlotTimedCall(node, time_offset);
}

void SectorManager::AddTimedCallPNode(TimeNode *this_node, broadcast_function func, long time_offset, Object *obj, long i1, long i2, long i3, long i4, char *ch, float a)
{
	//store params in node
    m_Mutex.Lock();

	memset ((void*)this_node, 0, sizeof(TimeNode));

	this_node->func = func;
	this_node->obj = obj;
	this_node->i1 = i1;
	this_node->i2 = i2;
	this_node->i3 = i3;
	this_node->i4 = i4;
	if (ch != 0)
	{
        this_node->ch = g_StringMgr->GetStr(ch);
	}
	else
	{
		this_node->ch = 0;
	}
	this_node->a = a;
	m_Mutex.Unlock();

	SlotTimedCall(this_node, time_offset);
}

TimeNode * SectorManager::AddTimedCall(Player *player, broadcast_function func, long time_offset, Object *obj, long i1, long i2, long i3, long i4, char *ch, float a)
{
	//store params in node
	TimeNode * this_node = m_MemoryMgr->GetBroadcastNodeSlot();

	m_Mutex.Lock();

	memset ((void*)this_node, 0, sizeof(TimeNode));

	this_node->func = func;
	this_node->obj = obj;
	this_node->i1 = i1;
	this_node->i2 = i2;
	this_node->i3 = i3;
	this_node->i4 = i4;
	if (ch != 0)
	{
        this_node->ch = g_StringMgr->GetStr(ch);
	}
	else
	{
		this_node->ch = 0;
	}
	this_node->a = a;

	if (player == 0)
	{
		this_node->player_id = SECTOR_SERVER_CONNECTIONLESS_NODE;
	}
	else
	{
		this_node->player_id = player->GameID();
	}
	m_Mutex.Unlock();

	SlotTimedCall(this_node, time_offset);


	return (this_node);
}

long SectorManager::GetSlotIndex(long time_offset)
{
    long index;
    //work out required number of slots ahead we need
    long slots = ((time_offset+50) / 100);

    if (slots < 2) slots = 2; //Ensure we never crash into CallSlotEvents call

    m_Mutex.Lock();
    index = (m_EventSlotIndex + slots) % (TIMESLOT_DURATION * 10);
    m_Mutex.Unlock();

    return index;
}

void SectorManager::SlotTimedCall(TimeNode *this_node, unsigned long time_offset)
{
	if (m_EventSlotHead)
	{
		this_node->event_time = time_offset + GetNet7TickCount();

		if (time_offset < (TIMESLOT_DURATION * 1000)) //use the slotted system
		{
			//slot call into appropriate vector slot
			long index = GetSlotIndex(time_offset);
			this_node->EventIndex = (short)index;
			m_EventSlotHead[index].push_back(this_node);
		}
		else //use long term slot system
		{
			this_node->EventIndex = -1;
			m_LongTermTimeNodes.push_back(this_node);
		}
	}
	else
	{
		LogMessage("*** FATAL ERROR: TimeSlots not available in sector %d %s\n", this->m_SectorNumber, this->m_SectorName);
	}
}

void SectorManager::MakeTimedCall(TimeNode *this_node)
{
    TimeNode node;
    memcpy(&node, this_node, sizeof(TimeNode));
    RemoveTimedCall(this_node);

	int ShutdownTimes[] = { 15, 10, 5 };

	Player *p = g_PlayerMgr->GetPlayer(node.player_id);

    if ((node.player_id != NODE_NO_LONGER_NEEDED) &&
        ((node.player_id == SECTOR_SERVER_CONNECTIONLESS_NODE) || p) )
	{
		switch(node.func)
		{
		case B_WARP_BROADCAST:
            p->StartWarp();
			break;
		case B_DESTROY_RESOURCE:
			m_ObjectMgr->DestroyObject(node.obj, 0, node.i1);
			break;
		case B_WARP_RESET:
			p->FinalWarpReset();
			break;
		case B_WARP_TERMINATE:
            p->TerminateWarp();
			break;
		case B_MINE_RESOURCE://(time_to_drain, obj, stack_val, slot);
            p->PullOreFromResource(node.obj, node.i1, node.i2, node.i3, node.i4);
			break;
		case B_COLLECT_RESOURCE:
            p->TakeOreOnboard(node.obj, node.i3);
			break;
		case B_MOB_DAMAGE:
			node.obj->DamageMOB(node.i1, node.i2, node.a, node.i3);
			break;
        case B_SHIP_DAMAGE:
            p->ShipDamage(node.i1, node.a, (short)node.i2);
            break;
		case B_RECHARGE_REACTOR:
			RechargeReactor(p);
			break;
        case B_RECHARGE_SHIELD:
            RechargeShield(p);
            break;
		case B_FORCE_LOGOUT:
			ForceLogout(p);
			break;
		case B_BUFF_TIMEOUT:
			BuffTimeout(p);
			break;
        case B_CAMERA_CONTROL:
            p->SendCameraControl(node.i1, node.i2);
            break;
        case B_MANUFACTURE_ACTION:
            p->ManufactureTimedReturn(node.i1);
            break;
        case B_ITEM_INSTALL:
            p->m_Equip[node.i1].FinishInstall();
            break;
        case B_ITEM_COOLDOWN:
            p->m_Equip[node.i1].CoolDown();
            break;
        case B_TEST_MESSAGE:
            p->SendVaMessage("Test Message time: %d [%d]",node.event_time, node.i1);
            break;
		case B_ABILITY_REMOVE:
			p->AbilityRemove(node.i1);
			break;
		case B_PLAYER_BUFFS:
			p->m_Buffs.Update(node.i1);
			break;
		case B_SHOOT_AMMO:
			p->m_Equip[node.i1].ShootAmmo(node.i2);
			break;
		case B_SERVER_SHUTDOWN:
			char Msg[60];
			sprintf(Msg, "Server shutdown in %d seconds!", ShutdownTimes[m_ShutDownMark]);
			g_ServerMgr->m_PlayerMgr.GlobalAdminMessage(Msg);
			AddTimedCall(0, B_SERVER_SHUTDOWN, ShutdownTimes[m_ShutDownMark] * 1000, NULL);
			m_ShutDownMark++;
			if (m_ShutDownMark > 2)
			{
				// Shutdown the server
				g_ServerShutdown = true;
			}
		}
	}
}

void SectorManager::RemoveTimedCall(TimeNode *node, bool force)
{
    m_Mutex.Lock();
	if (force)
	{
		TimeNodeVec::iterator itrTNode;
		if (node->EventIndex == -1)
		{
			for (itrTNode = m_LongTermTimeNodes.begin(); itrTNode < m_LongTermTimeNodes.end(); ++itrTNode)
			{
				if (*itrTNode == node)
				{
					LogMessage("Long term TimeNode erased\n");
					m_LongTermTimeNodes.erase(itrTNode);
					break;
				}
			}
		}
		else
		{
			for (itrTNode = m_EventSlotHead[node->EventIndex].begin(); itrTNode < m_EventSlotHead[node->EventIndex].end(); ++itrTNode)
			{
				if (*itrTNode == node)
				{
					LogMessage("TimeNode erased\n");
					m_EventSlotHead[node->EventIndex].erase(itrTNode);
					break;
				}
			}
		}
	}

    memset(node, 0, sizeof(TimeNode)); //erase node contents.
    m_Mutex.Unlock();
}

// This helper function is referenced by _beginthread to launch the TCP thread.
#ifdef WIN32
void __cdecl RunEventThreadAPI(void *arg)
{
    SectorManager * mgr = (SectorManager *) arg;
    mgr->RunSectorEventThread();
    _endthread();
}
#else // Linux
void * RunEventThreadAPI(void *arg)
{
	SectorManager * mgr = (SectorManager *) arg;
    mgr->RunSectorEventThread();
    return NULL;
}
#endif
