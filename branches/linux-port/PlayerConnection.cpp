// PlayerConnection.cpp

#ifdef WIN32
#pragma warning(disable:4786)
#endif

#include "PlayerClass.h"
#include "ServerManager.h"
#include "ObjectManager.h"
#include "Opcodes.h"
#include "PacketMethods.h"
#include "StaticData.h"
#include "UDPConnection.h"
#include "MOBDatabase.h"
#include "WinCompatibility.h"

void Player::SetPlayerPortIP(short port, long ip_addr)
{
    m_Player_IPAddr = ip_addr;
    m_Player_Port = port;
}

#define RECEIVE_MESSAGE_MAX_LENGTH 256
char unknown_corpse[] = "Corpse\0";

/*void Player::RunMsgThread()
{
    int length;
    EnbTcpHeader *header;
    unsigned char msg[RECEIVE_MESSAGE_MAX_LENGTH]; //message will be under 256 or the 'AddMessage' method will not process it

    m_MsgThreadRunning = true;

    while (!g_ServerShutdown)
    {
        while (m_RecvQueue->CheckQueue(msg, &length, RECEIVE_MESSAGE_MAX_LENGTH)) //check if there are any messages in the queue, if there are write them into the 'msg' buffer
        {
            //process this message
            //format is opcode/length/message
            header = (EnbTcpHeader*)msg;
            unsigned char *data = (msg + sizeof(EnbTcpHeader));

            //process opcode accordingly
            HandleClientOpcode(header->opcode, header->size, data);
        }
        SuspendThread(m_MsgThreadHandle);
        Sleep(2);
    }
}*/

void Player::PulsePlayerInput()
{
	int length;
	EnbTcpHeader *header;
	unsigned char msg[RECEIVE_MESSAGE_MAX_LENGTH]; //message will be under 256 or the 'AddMessage' method will not process it

	while (m_RecvQueue->CheckQueue(msg, &length, RECEIVE_MESSAGE_MAX_LENGTH)) //check if there are any messages in the queue, if there are write them into the 'msg' buffer
	{
		//process this message
		//format is opcode/length/message
		header = (EnbTcpHeader*)msg;
		unsigned char *data = (msg + sizeof(EnbTcpHeader));

		//process opcode accordingly
		HandleClientOpcode(header->opcode, header->size, data);
	}
}

void Player::AddMessage(short opcode, short length, unsigned char *data)
{
    if (!m_MsgThreadRunning)
    {
        LogMessage("ERROR!: Message thread for %s not running!!\n", Name());
        return;
    }

    unsigned char pData[RECEIVE_MESSAGE_MAX_LENGTH];

    if (length + 4 > RECEIVE_MESSAGE_MAX_LENGTH)
    {
        LogMessage("Recv message overflow: length = %d\n", length);
        return;
    }

    *((short*) &pData[0]) = length;
    *((short*) &pData[2]) = opcode;
    memcpy(pData + sizeof(short)*2, data, length);

    m_RecvQueue->Add(pData, length+sizeof(EnbTcpHeader), GameID());
}

void Player::SendOpcode(short opcode, unsigned char *data, long length, bool issue)
{
	if (!this) return;

    if (m_LoginConnection && m_LoginConnection->GameID() == GameID())
    {
        m_LoginConnection->SendResponse(opcode, data, length);
    }
    else if (m_UDPConnection)
    {
		m_LoginConnection = 0;
        if (!issue)
        {
            CacheOpcode(opcode, data, (u16)length);
        }
        else
        {
            m_UDPConnection->SendOpcode(opcode, this, data, length, m_Player_IPAddr, m_Player_Port, m_PacketSequenceNum);
        }
    }
}

void Player::CommitCacheToQueue()
{
	m_SendQueue->Add(m_QueueCache, m_QueueCacheIndex, GameID());
	m_QueueCacheIndex = 0;
}

void Player::CacheOpcode(short opcode, unsigned char *data, u16 length)
{
	if ( m_QueueCacheIndex < 0 || m_QueueCacheIndex >= MAXIMUM_PACKET_CACHE) m_QueueCacheIndex = 0;

	if ( m_QueueCacheIndex > 0 && (m_QueueCacheIndex + length + 4) > PERIODIC_CACHE_SEND_SIZE )
	{
		//need to commit existing packet to buffer and start again
		CommitCacheToQueue();
	}

	u8 *ptr = m_QueueCache + m_QueueCacheIndex;

	//write header
    *((u16 *) &ptr[0]) = (u16) length + sizeof(short)*2;
	*((short *) &ptr[2]) = opcode;

	ptr += 4;

	//write data
	memcpy(ptr, data, length);

	m_QueueCacheIndex += length + sizeof(short)*2;
}

void Player::SendPacketCache()
{
	m_WeaponsPerTick = 0;

	//first process all our inputs
	PulsePlayerInput();

	if (m_LoginConnection && m_LoginConnection->GameID() == GameID())
	{
		m_LoginConnection->SetLoginHandoff(true);
		m_LoginConnection->PulseConnectionOutput(true); //empty the TCP queue
	}
	else
    {
		int size;
		//commit current cache to queue if any
		if (m_QueueCacheIndex > 0) CommitCacheToQueue();

		m_Mutex.Lock();

		//see if there's anything we can send
		if (m_SendQueue->CheckQueue(m_PacketCache, &size, MAXIMUM_PACKET_CACHE, m_PacketSequenceNum))
		{
#if 0
			//packet drop out simulation testing
			if (rand()%5 == 0)
			{
				LogMessage("Dropped packet %x\n", m_PacketSequenceNum);
				m_PacketSequenceNum++;
			}
			else
#endif
			{
				SendOpcode(ENB_OPCODE_2016_PACKET_SEQUENCE, m_PacketCache, size, true);
				m_PacketSequenceNum++;
			}
		}

		m_Mutex.Unlock();
    }
	m_OpcodeResends = 0;
}

void Player::ReSendOpcodes(unsigned char *data)
{
    long packet_num = *((short*) &data[0]);
    long opcode_count = *((short*) &data[4]);
	int size;

	if (m_OpcodeResends > 5)
	{
		return; //don't resend any more than 5 requests per tick
	}

    LogMessage("Opcode re-send start at #%x, count %x\n", packet_num, opcode_count);

	if (opcode_count > 2)
	{
		packet_num += opcode_count - 2;
		opcode_count = 2;
	}

	//see if any of these packets exist, send if they do, otherwise send a blank
	while (opcode_count > 0 && m_UDPConnection)
	{
		m_Mutex.Lock();
		if (m_SendQueue->RetreiveMessage(m_PacketCache, &size, MAXIMUM_PACKET_CACHE, packet_num, GameID()))
		{
			m_UDPConnection->SendOpcode(ENB_OPCODE_2016_PACKET_SEQUENCE, this, m_PacketCache, size, m_Player_IPAddr, m_Player_Port, packet_num);
		}
		else
		{
			m_UDPConnection->SendOpcode(ENB_OPCODE_2016_PACKET_SEQUENCE, this, 0, 0, m_Player_IPAddr, m_Player_Port, packet_num);
		}
		m_Mutex.Unlock();
		opcode_count--;
		packet_num++;
		m_OpcodeResends++;
	}
}

void Player::HandleClientOpcode(short opcode, short bytes, unsigned char *data)
{
    switch (opcode)
    {
		case ENB_OPCODE_0002_LOGIN:
			HandleLogin(data);
			break;

		case ENB_OPCODE_0006_START_ACK :
			HandleStartAck(data);
			break;

		case ENB_OPCODE_0012_TURN:
			HandleTurn(data);
			break;

		case ENB_OPCODE_0013_TILT:
			HandleTilt(data);
			break;

		case ENB_OPCODE_0014_MOVE :
			HandleMove(data);
			break;

		case ENB_OPCODE_0017_REQUEST_TARGET :
			HandleRequestTarget(data);
			break;

		case ENB_OPCODE_0018_REQUEST_TARGETS_TARGET :
			HandleRequestTargetsTarget(data);
			break;

		case ENB_OPCODE_001A_DEBUG :
			HandleDebug(data);
			break;

		case ENB_OPCODE_0027_INVENTORY_MOVE:
			HandleInventoryMove(data);
			break;

		case ENB_OPCODE_0029_ITEM_STATE:
			HandleItemState(data);
			break;

		case ENB_OPCODE_002C_ACTION :
		case ENB_OPCODE_002D_ACTION2 :
			HandleAction(data);
			break;

		case ENB_OPCODE_002E_OPTION :
			HandleOption(data);
			break;

		case ENB_OPCODE_0033_CLIENT_CHAT :
			HandleClientChat(data);
			break;

		case ENB_OPCODE_0044_REQUEST_TIME :
			HandleRequestTime(data);
			break;

		case ENB_OPCODE_004E_STARBASE_REQUEST :
			HandleStarbaseRequest(data);
			break;

		case ENB_OPCODE_0051_SKILL_STRING_RQ:
			HandleSkillStringRequest(data);
			break;

		case ENB_OPCODE_0055_SELECT_TALK_TREE :
			HandleSelectTalkTree(data);
			break;

		case ENB_OPCODE_0057_SKILL_UP:
			HandleSkillAction(data);
			break;

		case ENB_OPCODE_0058_SKILL_ABILITY:
			HandleSkillAbility(data);
			break;

		case ENB_OPCODE_005A_VERB_REQUEST:
			HandleVerbRequest(data);
			break;

		case ENB_OPCODE_005D_EQUIP_USE :
			HandleEquipUse(data);
			break;

		case ENB_OPCODE_005E_AVATAR_EMOTE:
			HandleChatStream(data);
			break;

		case ENB_OPCODE_0079_MANUFACTURE_ITEM_CATAGORY:
            HandleManufactureTerminal(data);
			break;

		case ENB_OPCODE_007A_MANUFACTURE_ITEM_CATAGORY :
            HandleManufactureCategorySelection(data);
			break;

		case ENB_OPCODE_007B_MANUFACTURE_ITEM_ID :
            HandleManufactureSetItem(data);
			break;

		case ENB_OPCODE_007C_REFINERY_ITEM_ID :
            HandleRefineSetItem(data);
			break;

		case ENB_OPCODE_007E_MANUFACTURE_ACTION:
            HandleManufactureAction(data);
			break;

        case ENB_OPCODE_0080_MANUFACTURE_TECH_LEVEL_FILTER:
            HandleManufactureLevelFilter(data);
            break;

		case ENB_OPCODE_0086_MISSION_FORFEIT:
			HandleMissionForfeit(data);
			break;

		case ENB_OPCODE_0087_MISSION_DISMISSAL:
			HandleMissionDismissal(data);
			break;

		case ENB_OPCODE_0088_PETITION_STUCK:
			HandlePetitionStuck(data);
			break;

        case ENB_OPCODE_008D_INCAPACITANCE_REQUEST:
            HandleIncapacitanceRequest(data);
            break;

		case ENB_OPCODE_0098_GALAXY_MAP_REQUEST :
			HandleGalaxyMapRequest();
			break;

		case ENB_OPCODE_009B_WARP:
			HandleWarp(data);
			break;

		case ENB_OPCODE_009D_STARBASE_AVATAR_CHANGE :
			HandleStarbaseAvatarChange(data);
			break;

		case ENB_OPCODE_009F_STARBASE_ROOM_CHANGE :
			HandleStarbaseRoomChange(data);
			break;

		case ENB_OPCODE_00A1_TRIGGER_EMOTE :
			HandleTriggerEmote(data);
			break;

		case ENB_OPCODE_00A3_CLIENT_CHAT_REQUEST :
			HandleClientChatRequest(data);
			break;

		case ENB_OPCODE_00B9_LOGOFF_REQUEST :
			HandleLogoffRequest(data);
			break;

		case ENB_OPCODE_00BC_CTA_REQUEST :
			HandleCTARequest(data);
			break;

		case ENB_OPCODE_00C0_CONFIRMED_ACTION_RESPONSE:
			//ProcessConfirmedActionOffer(data);
			break;

            //resend lost/dropped opcodes
        case ENB_OPCODE_2017_RESEND_PACKET_SEQUENCE:
            ReSendOpcodes(data);
            break;

    default:
        LogMessage("Bad opcode for player 0x%08x '%s': %x\n", GameID(), Name(), opcode);
        break;
    }

    SetLastAccessTime(GetNet7TickCount());
}

void Player::HandleLoginStage2()
{
	LogMessage("Login stage2 for %s\n", Name());
	m_SectorMgr->HandleSectorLogin2(this);
	SetActive(true);
}

void Player::HandleLogin(unsigned char *data)
{
	Login * login = (Login *) data;

	m_MasterJoin = login->join_data;

	m_SentStart = false;

	long sector_id = ntohl(m_MasterJoin.ToSectorID);
    PlayerIndex()->SetSectorNum(sector_id);
	m_FromSectorID = ntohl(m_MasterJoin.FromSectorID);

	// Obtain the sector manager for the specified sector
	m_SectorMgr = g_ServerMgr->GetSectorManager(sector_id);

	if (!m_SectorMgr)
	{
		LogMessage("HandleLogin: Missing SectorManager for sector %d\n", sector_id);
	}
    else
	{
        //set up the player with a pointer to its sector manager and object manager
        SetObjectManager(m_SectorMgr->GetObjectManager());

        SectorReset();
        SectorLogin();

		// Set the client time
		SendClientSetTime(login->TimeSent);

        // Send the sector content to the client - NB AddPlayer must occur before SectorLogin
        m_SectorMgr->HandleSectorLogin(this);

        SaveData();
	}
}

void Player::SendClientSetTime(long TimeSent)
{
	ClientSetTime data;
	data.ClientSent = TimeSent;
	data.ServerReceived = GetNet7TickCount();
	data.ServerSent = data.ServerReceived;

    SendOpcode(ENB_OPCODE_0034_CLIENT_SET_TIME, (unsigned char*) &data, sizeof(data));
}

void Player::SendStarbaseSet(char action, char exit_mode)
{
	StarbaseSet starbase_set;
	memset(&starbase_set, 0, sizeof(starbase_set));
	starbase_set.StarbaseID = PlayerIndex()->GetSectorNum();
	starbase_set.Action = action;
	starbase_set.ExitMode = exit_mode;

	SendOpcode(ENB_OPCODE_004F_STARBASE_SET, (unsigned char *) &starbase_set, sizeof(starbase_set));
}

long Player::TryLoungeFile(long sector_id)
{
    long return_sector = sector_id;
	char old_path[MAX_PATH];
    char lounge_npc[MAX_PATH];

    sprintf(lounge_npc, "LoungeNPC_%ld.dat", sector_id);

	GetCurrentDirectory(sizeof(old_path), old_path);
	SetCurrentDirectory(SERVER_DATABASE_PATH);
	FILE *f = fopen(lounge_npc, "rb");
	if (!f)
	{
        sector_id = sector_id / 1000;
        //no file exists for this station.
        switch(sector_id)
        {
        case 14: //alpha-c
            return_sector = 10601;
            break;
        case 15: //antares
            return_sector = 10521;
            break;
        case 17: //aragoth
            return_sector = 10201;
            break;
        case 19: //capella
            return_sector = 10521;
            break;
        case 22: //
            return_sector = 10651;
            break;
        case 35: //
            return_sector = 10301;
            break;
        case 40: //Tau ceti
            return_sector = 10601;
            break;
        case 41: //sirius
            return_sector = 10551;
            break;
        default:
            return_sector = 10601;
            break;
        }
        LogMessage("Used lounge NPC of %d\n", return_sector);
	}

	SetCurrentDirectory(old_path);
    return return_sector;
}

void Player::SendDataFileToClient(const char *filename, long avatar_id)
{
	char old_path[MAX_PATH];
    if (m_LoginConnection)
    {
        SendDataFileToClientTCP(filename, avatar_id);
        return;
    }

	GetCurrentDirectory(sizeof(old_path), old_path);
	SetCurrentDirectory(SERVER_DATABASE_PATH);
	FILE *f = fopen(filename, "rb");
	if (f)
	{
		fseek(f,0,SEEK_END);
		long length = ftell(f);
		if ((length > 0) && (length < UDP_BUFFER_SEND_SIZE + sizeof(EnbUdpHeader)))
		{
			fseek(f,0,SEEK_SET);
			char * buffer = (char*)&m_UDPSendBuffer[0]; //use actual send buffer
			fread(buffer + sizeof(EnbUdpHeader), 1, length, f);

            *((short*) &buffer[0]) = (short)length;
            *((short*) &buffer[2]) = ENB_OPCODE_2010_DATA_FILE;
            *((long *) &buffer[4]) = GameID();

			if (avatar_id)
			{
				*((long *) &buffer[12]) = avatar_id;
			}

            //DumpBuffer((unsigned char*)buffer, length);

			m_UDPConnection->UDP_Send(buffer, length, m_Player_IPAddr, m_Player_Port);
		}
		else
		{
			LogMessage("SendDataFileToClient: Invalid file length %d : '%s'\n", length, filename);
		}
		fclose(f);
	}
	else
	{
		LogMessage("SendDataFileToClient: Unable to open %s\n", filename);
	}

	SetCurrentDirectory(old_path);
}

void Player::SendDataFileToClientTCP(const char *filename, long avatar_id)
{
	char old_path[MAX_PATH];
	GetCurrentDirectory(sizeof(old_path), old_path);
	SetCurrentDirectory(SERVER_DATABASE_PATH);
	FILE *f = fopen(filename, "rb");
	if (f)
	{
		fseek(f,0,SEEK_END);
		long length = ftell(f);
		if ((length > 0) && (length < TCP_BUFFER_SIZE))
		{
			fseek(f,0,SEEK_SET);
			unsigned char * buffer = new unsigned char[length];
			fread(buffer, 1, length, f);
			if (avatar_id)
			{
				*((long *) &buffer[4]) = avatar_id;
			}
			m_LoginConnection->Send(buffer, length);
			delete [] buffer;
		}
		else
		{
			LogMessage("SendDataFileToClient: Invalid file length %d : '%s'\n", length, filename);
		}
		fclose(f);
	}
	else
	{
		LogMessage("SendDataFileToClient: Unable to open %s\n", filename);
	}

	SetCurrentDirectory(old_path);
}

void Player::SendConfirmedActionOffer()
{
	unsigned char action_data[] =
	{
		0x00, 0x00, 0x00, 0x01,
		0x00, 0x00, 0x00, 0x65,
		0x07, 0x00, 0x4d, 0x65,
		0x73, 0x73, 0x61, 0x67, 0x65
	};

	SendOpcode(ENB_OPCODE_00BE_CONFIRMED_ACTION_OFFER, action_data, sizeof(action_data));
	SendClientSound("push_mission_alert_sound", 2, 0);
}

void Player::HandleActionResponse(unsigned char *data)
{
	long player_id = ntohl(*((long *) &data[0]));
	if (player_id == GameID())
	{
		m_ActionResponseReceived = true;
		ProcessConfirmedActionOffer();
	}
}

void Player::ProcessConfirmedActionOffer()
{
	int length;
	char buffer[256];
	memset(&buffer, 0, 256);

	if (m_ActionResponseReceived == false)
	{
		SendConfirmedActionOffer();
	}
	else
	{
		//TODO: re-vamp push messages, change Proxy version to match expected
		length = _snprintf(buffer,256,"Hi %s, there is a new version of Net7Proxy out now (v0.95), You need to download and install it.\
			this version of Net7Proxy will expire within a week or so.\nCheers, TB.", Name());
		SendOpcode(ENB_OPCODE_0054_TALK_TREE, (unsigned char *) buffer, length+3);

		{
			AvatarDescription avatar;
			memset(&avatar, 0, sizeof(avatar));
			memcpy(&avatar.avatar_data, &Database()->avatar, sizeof(avatar.avatar_data));
			avatar.AvatarID = GameID();
			avatar.unknown3 = 1.0;
			avatar.unknown4 = 1.0;
			SendOpcode(ENB_OPCODE_0061_AVATAR_DESCRIPTION, (unsigned char *) &avatar, sizeof(avatar));
		}
	}
}

void Player::SendObjectToObjectLinkedEffect(Object *target, Object *source, short effect1, short effect2, float speedup)
{
	unsigned char link_data[128];
	memset(link_data,0,128);
    int index = 0;

    if (!m_SectorMgr)
    {
        return;
    }

    long effect_id = m_SectorMgr->GetSectorNextObjID();

	AddData(link_data, effect_id, index);
	AddData(link_data, GetNet7TickCount(), index);
	AddData(link_data, source->GameID(), index);
    AddData(link_data, (char)(0), index);       //Unknown spacer
	AddData(link_data, target->GameID(), index);
	AddData(link_data, effect1, index);         //DurationLinkedEffectDescID
    AddData(link_data, effect2, index);         //EffectDescID
    AddData(link_data, 0.0f, index);            //x offset from default target hit zone
    AddData(link_data, 0.0f, index);            //y offset //NB - we leave these at zero
    AddData(link_data, 0.0f, index);            //z offset //     because the client seems to do a good job
    AddData(link_data, (long)(0), index);       //unknown (doesn't appear to be used)
    AddData(link_data, (char)(1), index);       //outside target radius
    AddData(link_data, (float)(1.0f), index);   //scale
    AddData(link_data, (float)(0.0f), index);   //HSV[0]
    AddData(link_data, (float)(0.0f), index);   //HSV[1]
    AddData(link_data, (float)(0.0f), index);   //HSV[2]
    AddData(link_data, speedup, index);         //speedup

    SendOpcode(ENB_OPCODE_000E_OBJECT_TO_OBJECT_LINKED_EFFECT, link_data, index);
}

void Player::SendClientSound(const char *sound_name, long channel, char queue)
{
	unsigned char packet[128];
	memset(packet,0,128);
	int index = 0;

	long length = strlen(sound_name) + 1;

	AddData(packet, length, index);
	AddDataS(packet, sound_name, index);
	AddData(packet, char(0), index);
	AddData(packet, channel, index);
	AddData(packet, queue, index);

	SendOpcode(ENB_OPCODE_006A_CLIENT_SOUND, (unsigned char *) &packet[0], index);
}

void Player::SendObjectEffect(ObjectEffect *obj_effect)
{
	unsigned char effect[128];
	memset(effect,0,128);
    int index = 0;

    AddData(effect, obj_effect->Bitmask, index);
    AddData(effect, obj_effect->GameID, index);
    AddData(effect, obj_effect->EffectDescID, index);

	if (obj_effect->Bitmask & 0x01)
	{
        AddData(effect, obj_effect->EffectID, index);
	}
	if (obj_effect->Bitmask & 0x02)
	{
        if (obj_effect->TimeStamp == 0)
        {
            obj_effect->TimeStamp = GetNet7TickCount();
        }

        AddData(effect, obj_effect->TimeStamp, index);
	}
	if (obj_effect->Bitmask & 0x04)
	{
        AddData(effect, obj_effect->Duration, index);
	}
	if (obj_effect->Bitmask & 0x08)
	{
        AddData(effect, obj_effect->Scale, index);
	}
	if (obj_effect->Bitmask & 0x10)
	{
        AddData(effect, obj_effect->HSVShift[0], index);
	}
	if (obj_effect->Bitmask & 0x20)
	{
        AddData(effect, obj_effect->HSVShift[1], index);
	}
	if (obj_effect->Bitmask & 0x40)
	{
        AddData(effect, obj_effect->HSVShift[2], index);
	}

    SendOpcode(ENB_OPCODE_0009_OBJECT_EFFECT, effect, index);
}

void Player::PointEffect(float *position, short effect_id, float scale)
{
	 /*2C 00            Length = 44 bytes
 0A 00            Opcode 0x0A = Point_Effect
 BF 0C 3A 00 object_id
 24 6A 7E 25 time+200
 18 E4 CA C7 x
 B5 05 19 47 y
 F6 50 98 45 z
 00 00 duration
 F5 03 effect_id
 7A FB 01 43 scale
 00 00 00 00 H
 00 00 00 00 S
 00 00 00 00 V */

    if (!m_SectorMgr)
    {
        return;
    }

	unsigned char point_data[] =
	{
		0x00, 0x00, 0x00, 0x00, //0
		0x00, 0x00, 0x00, 0x00, //4
		0x00, 0x00, 0x00, 0x00, //8
		0x01, 0x00, 0x00, 0x00, //12
		0x59, 0x0B, 0x00, 0x00, //16
		0x64, 0x4C, //20
		0x20, 0x25, //22
		0x00, 0x00, 0x00, 0x00, //24
		0x00, 0x00, 0x00, 0x00, //28
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00
	};

	*((long *) &point_data[0]) = m_SectorMgr->GetSectorNextObjID();
	*((long *) &point_data[4]) = GetNet7TickCount() + 200;
	*((float *) &point_data[8]) = position[0];
	*((float *) &point_data[12]) = position[1];
	*((float *) &point_data[16]) = position[2];
	*((short *) &point_data[20]) = 0;
	*((short *) &point_data[22]) = effect_id;
	*((float *) &point_data[24]) = 129.982f * scale;

	SendOpcode(ENB_OPCODE_000A_POINT_EFFECT, point_data, sizeof(point_data));
}

void Player::SendClientType(long client_type)
{
	//LogMessage("Sending ClientType packet\n");
	SendOpcode(ENB_OPCODE_003C_CLIENT_TYPE, (unsigned char *) &client_type, sizeof(client_type));
}

void Player::SendStart(long start_id)
{
	m_SentStart = true;
	//LogMessage("Sending Start packet\n");
	SendOpcode(ENB_OPCODE_0005_START, (unsigned char *) &start_id, sizeof(start_id));
}

void Player::SendSetBBox(float xmin, float ymin, float xmax, float ymax)
{
	SetBBox bbox;
	bbox.XMin = xmin;
	bbox.YMin = ymin;
	bbox.XMax = xmax;
	bbox.YMax = ymax;

	//LogMessage("Sending SetBBox packet\n");
	SendOpcode(ENB_OPCODE_002B_SET_BBOX, (unsigned char *) &bbox, sizeof(bbox));
}

void Player::SendSetZBand(float min, float max)
{
	SetZBand zband;
	zband.Min = min;
	zband.Max = min;

	//LogMessage("Sending SetZBand packet\n");
	SendOpcode(ENB_OPCODE_002A_SET_ZBAND, (unsigned char *) &zband, sizeof(zband));
}

/*
--------------------

 12 00            Length = 18 bytes
 99 00            Opcode 0x99 = Navigation
 6E 01 00 00      GameID
 00 C8 2F 47      Sig
 00               visited
 02 00 00 00      Type
 00               Ishuge*/

void Player::SendNavigation(int game_id, float signature, char visited, int nav_type, char is_huge)
{
	Navigation navigation;
	navigation.GameID = game_id;
	navigation.Signature = signature;
	navigation.PlayerHasVisited = visited;
	navigation.NavType = nav_type;
	navigation.IsHuge = is_huge;

	SendOpcode(ENB_OPCODE_0099_NAVIGATION, (unsigned char *) &navigation, sizeof(navigation));
}

void Player::SendCreateAttachment(int parent, int child, int slot)
{
	CreateAttachment attachment;
	attachment.Parent_ID = ntohl(parent);
	attachment.Child_ID = ntohl(child);
	attachment.Slot = ntohl(slot);

	SendOpcode(ENB_OPCODE_004A_CREATE_ATTACHMENT, (unsigned char *) &attachment, sizeof(attachment));
}

void Player::SendDecal(int game_id, int decal_id, int decal_count)
{
	if (decal_count > MAX_DECALS)
	{
		decal_count = MAX_DECALS;
	}

	Decal decal;
	decal.GameID = game_id;
	decal.DecalCount = (short)decal_count;
	for (int i = 0; i < decal_count; i++)
	{
		decal.Item[i].Index = i + 1;
        decal.Item[i].decal_id = decal_id;
		decal.Item[i].HSV[0] = 1.0f;
		decal.Item[i].HSV[1] = 1.0f;
		decal.Item[i].HSV[2] = 1.0f;
		decal.Item[i].opacity = 1.0f;
	}

	size_t size = ((char *) &decal.Item[decal_count]) - ((char *) &decal);

	SendOpcode(ENB_OPCODE_0010_DECAL, (unsigned char *) &decal, size);
}

void Player::SendNameDecal(int game_id, char *shipname, float h, float s, float v)
{
	NameDecal name_decal;

	memset(&name_decal, 0, sizeof(name_decal));
	name_decal.GameID = game_id;
	name_decal.RGB[0] = h;
	name_decal.RGB[1] = s;
	name_decal.RGB[2] = v;
	strncpy(name_decal.Name, shipname, sizeof(name_decal.Name) - 1);

	SendOpcode(ENB_OPCODE_00B2_NAME_DECAL, (unsigned char *) &name_decal, sizeof(name_decal));
}

void Player::SendConstantPositionalUpdate(long game_id, float x, float y, float z, float *orientation)
{
	ConstantPositionalUpdate update;
	memset(&update, 0, sizeof(update));

	update.GameID = game_id;
	update.Position[0] = x;
	update.Position[1] = y;
	update.Position[2] = z;
	if (orientation)
	{
		update.Orientation[0] = orientation[0];
		update.Orientation[1] = orientation[1];
		update.Orientation[2] = orientation[2];
		update.Orientation[3] = orientation[3];
	}

	SendOpcode(ENB_OPCODE_0040_CONSTANT_POSITIONAL_UPDATE, (unsigned char *) &update, sizeof(update));
}

void Player::SendFormationPositionalUpdate(long leader_id, long target_id, float x, float y, float z)
{
	FormationPositionalUpdate update;
	memset(&update, 0, sizeof(update));

	update.LeaderID = leader_id;
	update.TargetID = target_id;
	update.Position[0] = x;
	update.Position[1] = y;
	update.Position[2] = z;

	Player *leader = g_PlayerMgr->GetPlayer(leader_id);

	//TODO: work out way of smoothing out warp drive.
	//		plan: send 2 of these formation updates after warp drive kicks in to get orientation locked
	//		      then just do normal warp updates
	//	          every time we change direction, repeat the above.

	SendOpcode(ENB_OPCODE_0041_FORMATION_POSITIONAL_UPDATE, (unsigned char *) &update, sizeof(update));
}

void Player::SendSimplePositionalUpdate(long object_id, PositionInformation * position_info)
{
	SimplePositionalUpdate update;
	memset(&update, 0, sizeof(update));

	update.GameID = object_id;
	update.TimeStamp = GetNet7TickCount();
	update.Position[0] = position_info->Position[0];
	update.Position[1] = position_info->Position[1];
	update.Position[2] = position_info->Position[2];
	update.Orientation[0] = position_info->Orientation[0];
	update.Orientation[1] = position_info->Orientation[1];
	update.Orientation[2] = position_info->Orientation[2];
	update.Orientation[3] = position_info->Orientation[3];
	update.Velocity[0] = position_info->Velocity[0];
	update.Velocity[1] = position_info->Velocity[1];
	update.Velocity[2] = position_info->Velocity[2];

	SendOpcode(ENB_OPCODE_0008_SIMPLE_POSITIONAL_UDPATE, (unsigned char *) &update, sizeof(update));
}

void Player::SendPlanetPositionalUpdate(long object_id, PositionInformation * position_info)
{
	PlanetPositionalUpdate update;
	memset(&update, 0, sizeof(update));

	update.GameID = object_id;
	update.TimeStamp = GetNet7TickCount();
	update.Position[0] = position_info->Position[0];
	update.Position[1] = position_info->Position[1];
	update.Position[2] = position_info->Position[2];
	update.OrbitID = position_info->OrbitID;
	update.OrbitDist = position_info->OrbitDist;
	update.OrbitAngle = position_info->OrbitAngle;
	update.OrbitRate = position_info->OrbitRate;
	update.RotateAngle = position_info->RotateAngle;
	update.RotateRate = position_info->RotateRate;
	update.TiltAngle = position_info->TiltAngle;

	SendOpcode(ENB_OPCODE_003F_PLANET_POSITIONAL_UPDATE, (unsigned char *) &update, sizeof(update));
}

void Player::SendComponentPositionalUpdate(long object_id, PositionInformation * position_info, long timestamp)
{
	ComponentPositionalUpdate update;
	memset(&update, 0, sizeof(update));

	update.simple.GameID = object_id;
	if (timestamp)
	{
		update.simple.TimeStamp = timestamp;
	}
	else
	{
		update.simple.TimeStamp = GetNet7TickCount();
	}
	update.simple.Position[0] = position_info->Position[0];
	update.simple.Position[1] = position_info->Position[1];
	update.simple.Position[2] = position_info->Position[2];
	update.simple.Orientation[0] = position_info->Orientation[0];
	update.simple.Orientation[1] = position_info->Orientation[1];
	update.simple.Orientation[2] = position_info->Orientation[2];
	update.simple.Orientation[3] = position_info->Orientation[3];
	update.simple.Velocity[0] = position_info->Velocity[0];
	update.simple.Velocity[1] = position_info->Velocity[1];
	update.simple.Velocity[2] = position_info->Velocity[2];
	update.ImpartedDecay = position_info->ImpartedDecay;
	update.TractorSpeed = position_info->TractorSpeed;
	update.TractorID = position_info->TractorID;
	update.TractorEffectID = position_info->TractorEffectID;

	SendOpcode(ENB_OPCODE_0046_COMPONENT_POSITIONAL_UPDATE, (unsigned char *) &update, sizeof(update));
}

void Player::SendAdvancedPositionalUpdate(long object_id, PositionInformation * position_info)
{
	char packet[sizeof(AdvancedPositionalUpdate)];
	memset(packet, 0, sizeof(packet));
	short *pBitmask = (short *) &packet[0];
	long *pLong = (long *) &packet[2];
	float *pFloat = (float *) &packet[2];
	short bitmask = position_info->Bitmask;
	int index = 0;

	// Package the data into the packet
	*pBitmask = bitmask;
	pLong[index++] = object_id;           // GameID
	pLong[index++] = GetNet7TickCount();      // TimeStamp
	pFloat[index++] = position_info->Position[0];
	pFloat[index++] = position_info->Position[1];
	pFloat[index++] = position_info->Position[2];
	pFloat[index++] = position_info->Orientation[0];
	pFloat[index++] = position_info->Orientation[1];
	pFloat[index++] = position_info->Orientation[2];
	pFloat[index++] = position_info->Orientation[3];
	pLong[index++] = position_info->MovementID;
	if (bitmask & 0x0001)
	{
		pFloat[index++] = position_info->CurrentSpeed;
	}
	if (bitmask & 0x0002)
	{
		pFloat[index++] = position_info->SetSpeed;
	}
	if (bitmask & 0x0004)
	{
		pFloat[index++] = position_info->Acceleration;
	}
	if (bitmask & 0x0008)
	{
		pFloat[index++] = position_info->RotY;
	}
	if (bitmask & 0x0010)
	{
		pFloat[index++] = position_info->DesiredY;
	}
	if (bitmask & 0x0020)
	{
		pFloat[index++] = position_info->RotZ;
	}
	if (bitmask & 0x0040)
	{
		pFloat[index++] = position_info->DesiredZ;
	}
	if (bitmask & 0x0080)
	{
		pFloat[index++] = position_info->ImpartedVelocity[0];
		pFloat[index++] = position_info->ImpartedVelocity[1];
		pFloat[index++] = position_info->ImpartedVelocity[2];
		pFloat[index++] = position_info->ImpartedSpin;
		pFloat[index++] = position_info->ImpartedRoll;
		pFloat[index++] = position_info->ImpartedPitch;
	}
	if (bitmask & 0x0100)
	{
		pLong[index++] = position_info->UpdatePeriod;
	}

	int length = 2 + 4 * index;
	SendOpcode(ENB_OPCODE_003E_ADVANCED_POSITIONAL_UPDATE, (unsigned char *) &packet, length);

	//is this to ourselves?
	if (this->GameID() == object_id)
	{
		_sleep(1);
	}
}

void Player::SendObjectToObjectEffect(ObjectToObjectEffect *obj_effect)
{
	unsigned char effect[128];
	memset(effect,0,128);
    int index = 0;

    AddData(effect, obj_effect->Bitmask, index);
    AddData(effect, obj_effect->GameID, index);
    AddData(effect, obj_effect->TargetID, index);
    AddData(effect, obj_effect->EffectDescID, index);

    if (obj_effect->Message)
    {
        AddDataS(effect, obj_effect->Message, index);
    }
    else
    {
        AddData(effect, (char)0, index);
    }

	if (obj_effect->Bitmask & 0x01)
	{
        AddData(effect, obj_effect->EffectID, index);
	}
	if (obj_effect->Bitmask & 0x02)
	{
        if (obj_effect->TimeStamp == 0)
        {
            obj_effect->TimeStamp = GetNet7TickCount();
        }

        AddData(effect, obj_effect->TimeStamp, index);
	}
	if (obj_effect->Bitmask & 0x04)
	{
        AddData(effect, obj_effect->Duration, index);
	}
	if (obj_effect->Bitmask & 0x08)
	{
        AddData(effect, obj_effect->TargetOffset[0], index);
        AddData(effect, obj_effect->TargetOffset[1], index);
        AddData(effect, obj_effect->TargetOffset[2], index);
	}
	if (obj_effect->Bitmask & 0x10)
	{
        AddData(effect, obj_effect->OutsideTargetRadius, index);
	}
	if (obj_effect->Bitmask & 0x20) //from here on isn't correct - packet struct in packetstructures.h is wrong... TODO: work out correct packet structure.
	{
        AddData(effect, obj_effect->unused, index);
	}
    if (obj_effect->Bitmask & 0x40)
    {
        AddData(effect, obj_effect->Scale, index);
    }
	if (obj_effect->Bitmask & 0x80)
	{
        AddData(effect, obj_effect->HSVShift[0], index);
        AddData(effect, obj_effect->HSVShift[1], index);
        AddData(effect, obj_effect->HSVShift[2], index);
	}
    if (obj_effect->Bitmask & 0x100)
    {
        AddData(effect, obj_effect->Speedup, index);
    }

    SendOpcode(ENB_OPCODE_000B_OBJECT_TO_OBJECT_EFFECT, effect, index);
}

void Player::SendActivateRenderState(long game_id, unsigned long render_state_id)
{
	ActivateRenderState state;

	state.GameID = game_id;
	state.RenderStateID = render_state_id;

	//LogMessage("Sending ActivateRenderState packet\n");
	SendOpcode(ENB_OPCODE_0030_ACTIVATE_RENDER_STATE, (unsigned char *) &state, sizeof(state));
}


//TODO: Find out more about packet data structure - this is just a guess.
void Player::SendInitRenderState(long game_id, unsigned long render_state_id)
{
	InitRenderState state;

	state.GameID = game_id;
	state.RenderStateID = render_state_id;

	SendOpcode(ENB_OPCODE_002F_INIT_RENDER_STATE, (unsigned char *) &state, sizeof(state));
}

void Player::SendActivateNextRenderState(long game_id, unsigned long render_state_id)
{
	ActivateRenderState state;

	state.GameID = game_id;
	state.RenderStateID = render_state_id;

	//LogMessage("Sending ActivateRenderState packet\n");
	SendOpcode(ENB_OPCODE_0031_ACTIVATE_NEXT_RENDER_STATE, (unsigned char *) &state, sizeof(state));
}

void Player::SendDeactivateRenderState(long game_id)
{
	/*ActivateRenderState state;

	state.GameID = game_id;
	state.RenderStateID = render_state_id;*/

	//LogMessage("Sending ActivateRenderState packet\n");
	SendOpcode(ENB_OPCODE_0032_DEACTIVATE_RENDER_STATE, (unsigned char *) &game_id, sizeof(game_id));
}

void Player::SendCreate(int game_id, float scale, short asset, int type, float h, float s, float v)
{
	Create  create;
	create.GameID = game_id;
	create.Scale = scale;
	create.BaseAsset = asset;
	create.Type = (char) type;
	create.HSV[0] = h;
	create.HSV[1] = s;
	create.HSV[2] = v;

	//LogMessage("Sending Create packet\n");
	SendOpcode(ENB_OPCODE_0004_CREATE, (unsigned char *) &create, sizeof(create));
}

void Player::HandleSkillStringRequest(unsigned char *data)
{
	ClientSkillsRequest * request = (ClientSkillsRequest *) data;

    //Loot item
    //not sure if this packet is used for anything other than looting

    //check we're targetting a HUSK
	ObjectManager *om = GetObjectManager();
	Object *obj = (0);
	if (om) obj = om->GetObjectFromID(ShipIndex()->GetTargetGameID());

    //TODO: set and check loot timer.
    if (obj && obj->ObjectType() == OT_HUSK)
    {
        //Now check no-one else is looting this object
        Player *player = obj->CheckResourceLock();
        if (player && player != this)
        {
            SendVaMessage("%s currently being looted by %s", obj->Name(), player->Name());
        }
        else
        {
            SendClientSound("coin.wav",0, 0);
            long id = ntohl(obj->GameID());
            SendOpcode(ENB_OPCODE_008C_LOOT_HULK_PERMISSION, (unsigned char *)&id, sizeof(long));
			//check if MOB loot is empty
			if (obj->ResourceRemains() == 0.0f)
			{
				obj->SendObjectDrain(-1);
			}
			else
			{
				m_ProspectWindow = true;
			}
        }
    }
}

void Player::HandleStartAck(unsigned char *data)
{
	LogDebug("Received StartAck packet\n");

	// Added to do things on sector change at top of 'Player' class
    // don't put them here!!

    //send the login camera just after we finally log in
    SendLoginCamera();
}

void Player::HandleRequestTime(unsigned char *data)
{
	LogDebug("Received RequestTime packet\n");

	// Set the client time
	SendClientSetTime(*((long *) data));
}

//TODO: recode this so it doesn't requre new/delete
//maybe allocate chat buffers, and re-size them if necessary
void Player::HandleClientChatRequest(unsigned char *data)
{
	ClientChatRequest * request = (ClientChatRequest *) data;

	//LogMessage("Received ClientChatRequest packet\n");
	//char Nick[30], Message[200];
	char * string1;
	char * string2;
	char * string3;

	short length1 = request->string_length;
	char *p = ((char *) &request->string_length);
	p += sizeof(short);
	if (length1)
	{
		string1 = new char[length1 + 1];
		if (string1)
		{
			memcpy(string1, p, length1);
			string1[length1] = 0;
			p += length1;
		}
	}
	short length2 = *((short *) p);
	p += sizeof(short);
	if (length2)
	{
		string2 = new char[length2 + 1];
		if (string2)
		{
			memcpy(string2, p, length2);
			string2[length2] = 0;
			p += length2;
		}
	}
	short length3 = *((short *) p);
	p += sizeof(short);
	if (length3)
	{
		string3 = new char[length3 + 1];
		if (string3)
		{
			memcpy(string3, p, length3);
			string3[length3] = 0;
			p += length3;
		}
	}
	long unknown2 = *((long *) p);
	//LogMessage("PlayerID=%d  arg1=%d  arg2=%d\n",
	//    request->PlayerID, request->unknown1, unknown2);
	//LogMessage("String1=\"%s\"\n", string1);
	//LogMessage("String2=\"%s\"\n", string2);
	//LogMessage("String3=\"%s\"\n", string3);

//  sprintf(Nick, "%s", string1);
	//sprintf(Message, "%s", string3);
	if (request->unknown1 == 1)
	{
		g_ServerMgr->m_PlayerMgr.ChatSendPrivate(GameID(), string1, string3);
	}

	if (request->unknown1 == 0)		// Channel Message
	{
		//LogMessage("ClientChatRequest: SendToChannel: %s Message: %s\n", string2, string3);
		if (string3[0] == '/')
		{
			HandleSlashCommands(string3);
		}
		else
		{
			g_ServerMgr->m_PlayerMgr.ChatSendChannel(GameID(), string2, string3);
		}
	}

	if (length1)
	{
		delete [] string1;
	}
	if (length2)
	{
		delete [] string2;
	}
	if (length3)
	{
		delete [] string3;
	}
}

void Player::HandleTurn(unsigned char *data)
{
	struct PacketTurn {
		long GameID;
		float Intensity;
	} ATTRIB_PACKED;

	PacketTurn * Turning = (PacketTurn *)  data;

    if (!WarpDrive())
    {
        AbortProspecting(true,false);
        Turn(Turning->Intensity);
    }
}

void Player::HandleTilt(unsigned char *data)
{
	struct PacketTurn {
		long GameID;
		float Intensity;
	} ATTRIB_PACKED;

	PacketTurn * Turning = (PacketTurn *)  data;

    if (!WarpDrive())
    {
        AbortProspecting(true,false);
        Tilt(Turning->Intensity);
    }
}

void Player::HandleMove(unsigned char *data)
{
	MovePacket * Movement = (MovePacket *) data;

    if (!WarpDrive())
    {
        AbortProspecting(true,false);

		// Break formation if we are in a group & formed
		if (GroupID() != -1)
		{
			// If we are the leader we can move the whole group
			if (g_ServerMgr->m_PlayerMgr.GetMemberID(this->GroupID(), 0) != this->GameID())
			{
				g_ServerMgr->m_PlayerMgr.LeaveFormation(GameID());
			}
		}

        if (Movement->type == 4)
        {
            SendContrailsRL(false);
			g_ServerMgr->m_PlayerMgr.FormationEngineOperation(this, false);
        }
        else
        {
            SendContrailsRL(true);
			g_ServerMgr->m_PlayerMgr.FormationEngineOperation(this, true);
        }

        Move(Movement->type);
    }
}

void Player::HandleWarp(unsigned char *data)
{
	WarpPacket * warp = (WarpPacket *) data;

    if (WarpDrive())
    {
		TerminateWarpGroup(true);
    }
    else
    {
		// Make sure we are both in formation & Group leader
		if (g_ServerMgr->m_PlayerMgr.CheckGroupFormation(this) && g_ServerMgr->m_PlayerMgr.GetMemberID(this->GroupID(), 0) == this->GameID())
		{
			// Start up warps if you are in a formation
			for(int x=0;x<6;x++)
			{
				int PlayerID = g_ServerMgr->m_PlayerMgr.GetMemberID(this->GroupID(), x);
				if (PlayerID > 0)
				{
					Player* pid = g_ServerMgr->m_PlayerMgr.GetPlayer(PlayerID);

					if (g_ServerMgr->m_PlayerMgr.CheckGroupFormation(pid))
					{
						pid->SendContrailsRL(false);
						pid->SetupWarpNavs(warp->Navs, warp->TargetID);
						pid->PrepareForWarp();
					}
				}
			}
		}
		else
		{
			// Break formation if we are in a group & formed
			if (GroupID() != -1)
			{
				g_ServerMgr->m_PlayerMgr.LeaveFormation(GameID());
			}
			SendContrailsRL(false);
			LogDebug("Warp Navs: %ld, GameID=%d (%s)\n", warp->Navs, (warp->GameID & 0x00FFFFFF), Name());
			SetupWarpNavs(warp->Navs, warp->TargetID);
			PrepareForWarp();
		}
    }
}

void Player::Contrails(long player_id, bool contrails)
{
	unsigned char aux_data[] =
	{
		0x00, 0x00, 0x00, 0x00,
		0x13, 0x00,
		0x01,
		0x02, 0x00, 0x00, 0x00, 0x00, 0x00,	0x04, 0x00,
		0x00, 0x00, 0x00, 0x00
	};

	if (contrails == true)
	{
		*((long*) &aux_data[15]) = 1;
	}

	*((long *) aux_data) = player_id;

	SendOpcode(ENB_OPCODE_001B_AUX_DATA, aux_data, sizeof(aux_data));
}

void Player::SendResourceName(long resourceID, char *resource_name)
{
	unsigned char aux_data[64];
	memset(aux_data, 0, 64);
	short length = strlen(resource_name);
	*((long *) aux_data) = resourceID;
	*((short *) &aux_data[4]) = length + 4;
	*((short *) &aux_data[6]) = 0x1201;
	*((short *) &aux_data[8]) = length;

	strncpy((char*)&aux_data[10], resource_name, length);

	SendOpcode(ENB_OPCODE_001B_AUX_DATA, aux_data, length+10);
}

//this is the same way that the live server sent husk content, but it is horrendously inefficient
//I wonder if there's a better way we can send Husk content
//we can re-do this with Net7Proxy so it's optimal
void Player::SendHuskContent(Object *husk)
{
    bool contents_already_sent;

    if (husk)
    {
        contents_already_sent = husk->GetIndex(ResourceSendList());

        if (!contents_already_sent)
        {
            unsigned char spacer[4] =
            {
                0x36, 0x00, 0xFF
            };
            unsigned char filler[8] =
            {
                0x16, 0x80, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF
            };
            unsigned char prologue1[] =
            {
                0xF6, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F
            };
            unsigned char prologue2[] =
            {
                0xF6, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F
            };

            unsigned char aux_data[1024];
            int index = 0;
            int x;

            long length = husk->NameLen();
            AddData(aux_data, husk->GameID(), index);
            AddData(aux_data, (short)0x0000, index);
            AddData(aux_data, (long) (0x0FEC1601), index);
            AddData(aux_data, (short)(length), index);
            AddDataS(aux_data, husk->Name(), index);
            AddData(aux_data, (long)0x05050505, index);

            ContentSlot *slot;

            AddBuffer(aux_data, prologue1, 6, index);

            for (x = 0; x < 20; x++)
            {
                AddBuffer(aux_data, filler, 7, index);
            }

            AddBuffer(aux_data, prologue2, 11, index);

            for (x = 0; x < MAX_ITEMS_PER_RESOURCE; x++)
            {
                slot = husk->GetContents(x);
                if (slot->stack > 0 && slot->item)
                {
                    AddBuffer(aux_data, spacer, 3, index);
                    AddData(aux_data, slot->item->ItemTemplateID(), index);
                    AddData(aux_data, long(slot->stack), index);
                    SendItemBase(slot->item->ItemTemplateID());
                }
                else
                {
                    AddBuffer(aux_data, filler, 7, index);
                }
            }

            for (x = 0; x < 40-MAX_ITEMS_PER_RESOURCE; x++)
            {
                AddBuffer(aux_data, filler, 7, index);
            }

            //set length
            *((short *) &aux_data[4]) = index-8;

            SendOpcode(ENB_OPCODE_001B_AUX_DATA, aux_data, index);
            husk->SetIndex(ResourceSendList());
        }
    }
}

void Player::SendHuskName(Object *husk)
{
	unsigned char aux_data[256];
	memset(aux_data, 0, 256);
    int index = 0;

	char *name = unknown_corpse;
	int length = 7;

	if (husk->Name())
	{
		name = husk->Name();
		length = husk->NameLen();
	}

    AddData(aux_data, husk->GameID(), index);
    AddData(aux_data, (short)(length+10), index);		// Lengh of Aux Packet
    AddData(aux_data, (long) (0x03E01601), index);
    AddData(aux_data, (short)(length), index);
    AddDataS(aux_data, name, index);
    AddData(aux_data, (long)0x05050505, index);

	if (index >= 256)
	{
		LogMessage("**ERROR**: HuskNameIndex overflow error, %s, index len = %d\n", name, index);
	}

	SendOpcode(ENB_OPCODE_001B_AUX_DATA, aux_data, index);
}

void Player::SendMobName(Object *mob)
{
    unsigned char packet[340];
    int index = 0;

    unsigned char epilogue[] =
    {
		0x05, 0x05, 0x05, 0x05,
		0x05, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

	unsigned char aux_data[] =
	{
		0x3B, 0x00, //AUX length [4]
		0x01,
		0x16, 0x30, 0x00, 0x40, 0x00, 0x00, 0x0C, 0x80, 0x03,
		0x00, 0xBE, 0x40, 0xF8, 0xC0, 0x07,
	};

    AddData  (packet, (long)mob->GameID(), index);
    AddBuffer(packet, aux_data, sizeof(aux_data), index);
    AddDataLS(packet, mob->Name(), index);
	AddData  (packet, (float)mob->GetHullLevel(), index);
    AddData  (packet, (float)mob->GetStartHullLevel(), index);
    AddData  (packet, (long)mob->Level(), index);
    AddBuffer(packet, epilogue, sizeof(epilogue), index);

    *((short *) &packet[4]) = index - 6;

	SendOpcode(ENB_OPCODE_001B_AUX_DATA, packet, index);
}

void Player::SendSimpleAuxName(Object *obj)
{
	int length = obj->NameLen();
    char *packet = (char*)_alloca(length+10);
    *((long *) packet) = obj->GameID();
    *((short *) &packet[4]) = length + 4;
    packet[6] = 0x01;
    packet[7] = 0x12;
    *((short *) &packet[8]) = length;
    strncpy(&packet[10], obj->Name(), length);

	SendOpcode(ENB_OPCODE_001B_AUX_DATA, (unsigned char *) packet, length+10);
}

void Player::SendAuxNameSignature(Object *obj)
{
	char *name;
	char nav = 0;
	if (obj->IsNav() || obj->ObjectType() == OT_FIELD)
    {
		if (obj->Name() != 0)
		{
			name = obj->Name();
		}
		nav = 1;
    }

    int length = strlen(name);
    char *packet = (char*)_alloca(length + 15);
    *((long *) packet) = obj->GameID();
    *((short *) &packet[4]) = length + 9;
    packet[6] = 0x01;
    packet[7] = 0x72;
    *((short *) &packet[8]) = length;
    strncpy(&packet[10], name, length);
    int i = 10 + length;
    packet[i++] = nav;
    *((float *) &packet[i]) = obj->Signature();

    SendOpcode(ENB_OPCODE_001B_AUX_DATA, (unsigned char *) packet, length+15);
}

void Player::SendAuxNameResource(Object *obj)
{
	char packet[256];
	char *name = unknown_corpse;
	int length = 7;

	if (!obj->Name()) Sleep(50); //tiny delay here seems to help if name is unknown.

	if (obj->Name())
	{
		name = obj->Name();
		length = obj->NameLen();
	}

    *((long *)  &packet[0]) = obj->GameID();
    *((short *) &packet[4]) = length + 5;
    packet[6] = 0x01;
    packet[7] = 0x16;
	packet[8] = 0x04;
    *((short *) &packet[9]) = length;
    strncpy(&packet[11], name, length);

	SendOpcode(ENB_OPCODE_001B_AUX_DATA, (unsigned char *) packet, length + 11);
}

void Player::UnSetTarget(long GameID)
{
	if (ShipIndex()->GetTargetGameID() == GameID)
	{
		if (m_ProspectWindow)
		{
			m_ProspectWindow = false;
			OpenInterface(1,0);
		}
		//LogMessage("Sending SetTarget packet\n");
		SendSetTarget(0, -1);

		ShipIndex()->SetTargetGameID(-1);
		SendAuxShip();
        BlankVerbs();
	}
}

void Player::ActivateProspectBeam(long player_id, long target_id, const char *message, short effect_type, long effectUID, long timestamp, short effect_time)
{
	ObjectToObjectEffect Prospect;

    if (effect_time == 0)
    {
        Prospect.Bitmask = 0x03;
    }
    else
    {
        Prospect.Bitmask = 0x07;
    }
	Prospect.GameID = player_id;
	Prospect.TargetID = target_id;
	Prospect.EffectDescID = effect_type;// 0x00BF;
	Prospect.Message = message;
	Prospect.EffectID = effectUID;
    Prospect.Duration = short(effect_time);
	Prospect.TimeStamp = timestamp;

	SendObjectToObjectEffect(&Prospect);
}

void Player::SendPushMessage(const char *msg1, const char *type, long time, long priority)
{
	unsigned char packet[128];
	memset(packet,0,128);
	unsigned char *pptr = &packet[0];
	int index = 0;

	AddDataS(pptr, msg1, index);
	AddData(pptr, char(0), index);
	AddDataS(pptr, type, index);
	AddData(pptr, char(0), index);
	AddData(pptr, time, index);
	AddData(pptr, priority, index);

	SendOpcode(ENB_OPCODE_0022_PUSH_MESSAGE, pptr, index);
}

void Player::SetResourceDrainLevel(Object *obj, long slot)
{
    unsigned long slot_index = ((0x10 << slot) | 0x02); //slot index calc
    u16 length = 28;
	//Control which resource gets removed and how much of the resource is left
	unsigned char aux_data[] =
	{
		0x00, 0x00, 0x00, 0x00,
		0x16, 0x00,
		0x01, // 6
		0x62, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x32, 0x00, //the second byte value determines which resource to remove ((0x10 << slot) | 0x02)
		0xFE, 0xFF, 0xFF, 0xFF,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00  // amount of colour left in resource - 0 is collapse
	};

	*((long *) aux_data) = obj->GameID();
	*((long *) &aux_data[8]) = slot_index;
	*((float *) &aux_data[24]) = obj->ResourceRemains();

	if (obj->GetStack(slot) > 0)
	{
        length = 24;
		*((char *) &aux_data[4]) = 0x12;  //new size
		*((char *) &aux_data[14]) = 0x22; //indicates partial removal
		*((long *) &aux_data[16]) = obj->GetStack(slot) ; //resource remaining in this slot
		*((float *) &aux_data[20]) = obj->ResourceRemains();
	}

	SendOpcode(ENB_OPCODE_001B_AUX_DATA, aux_data, length); //this seems to initiate resource collapse or drains the resource according to last float val
}

void Player::SetHuskDrainLevel(Object *obj, long slot)
{
	unsigned long slot_index = ((0x10 << slot) | 0x02); //slot index calc
	unsigned char packet[128];
	memset(packet,0,128);
	unsigned char *pptr = &packet[0];
	int index = 0;

	unsigned char aux_data[] =
	{
		0x01, 0x02, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x32, 0x00
	} ;

	AddData(pptr, obj->GameID(), index);
	AddData(pptr, short(0), index);
	AddBuffer(pptr, aux_data, 11, index);
	AddData(pptr, 0xFFFFFFFE, index);
	AddData(pptr, long(0), index);

	*((long *) &pptr[9]) = slot_index;
	*((short *) &pptr[4]) = (index - 6); //set info length

	SendOpcode(ENB_OPCODE_001B_AUX_DATA, pptr, index);
}

void Player::RemoveObject(long object_id)
{
	UnSetTarget(object_id);
	SendOpcode(ENB_OPCODE_0007_REMOVE, (unsigned char *) &object_id, sizeof(object_id)); //remove the raw resource
}

//This is where I put all the AUX prospecting stuff I don't really understand
//any help decoding any of this appreciated!

//TODO: REMOVE THIS!!
void Player::SendProspectAUX(long value, int type)
{
	switch (type)
	{
	case 0:
		{
			//non-parsemode AuxPlayer
			//sets prospect skill last activation time
			unsigned char aux_data[] =
			{
				0x00, 0x00, 0x00, 0x00,
				0x15, 0x00,
				0x00,
				0x01, 0x00, 0x00, 0x00,
				0x59, 0x0B, 0x00, 0x00, //always this for prospecting
				0x64, 0x4C, 0x20, 0x25, //timestamp...
				0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00
			};

			*((long *) &aux_data[15]) = value;

			SendOpcode(ENB_OPCODE_001B_AUX_DATA, aux_data, sizeof(aux_data));
		}
		break;

	case 1:
		{
			//second type of AUX prospecting requires to be sent
			//diables the users cloak and advancd cloak abilities
			unsigned char aux_data[] =
			{
				0x00, 0x00, 0x00, 0x00,
				0x1D, 0x00,
				0x00,
				0x02, 0x00, 0x00, 0x00,
				0x15, 0x0C, 0x00, 0x00,
				0x00, 0x01, 0x00, 0x00,
				0xF5, 0x0C, 0x00, 0x00,
				0x00, 0x01, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00
			}; //no idea what this does, but always the same for prospect. Maybe some effect?

			SendOpcode(ENB_OPCODE_001B_AUX_DATA, aux_data, sizeof(aux_data));
		}
		break;

	case 2:
		{
			ShipIndex()->SetTargetGameID(0);
			SendAuxShip();
            BlankVerbs();
		}
		break;

	case 3:
		{
			//AuxShip Packet (most likely "disables" weapons)
			unsigned char aux_data[] =
			{
				0x00, 0x00, 0x00, 0x00,
				0x1C, 0x00,
				0x01,
				0x02, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, //inventory
				0x02, 0x08,										//equip inventory
				0x22, 0x08, 0x00,								//items 1 and 7
				0x02, 0x00,	0x01,								//equipitem flags
				0x10, 0x20, 0x00, 0x00,							//itemstats
				0x02, 0x00, 0x01,								//equipitem flags
				0x10, 0x20, 0x00, 0x00							//itemstats
			};
			*((long *) aux_data) = value;
			SendOpcode(ENB_OPCODE_001B_AUX_DATA, aux_data, sizeof(aux_data));
		}
		break;

	case 4:
		{
			//re-enables the users cloak and advancd cloak abilities
			unsigned char aux_data[] =
			{
				0x00, 0x00, 0x00, 0x00,
				0x1D, 0x00,
				0x00,
				0x02, 0x00, 0x00, 0x00,
				0x15, 0x0C, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
				0xF5, 0x0C, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00
			}; //no idea what this does, but always the same for prospect. Maybe some effect?

			SendOpcode(ENB_OPCODE_001B_AUX_DATA, aux_data, sizeof(aux_data));
		}
		break;
	}
}

void Player::CreateTractorComponent(float *position, float decay, float tractor_speed, long player_id, long article_id, long effect_id, long timestamp)
{
	PositionInformation pos_info;

	memset(&pos_info, 0, sizeof(PositionInformation));

	pos_info.Position[0] = position[0];
	pos_info.Position[1] = position[1];
	pos_info.Position[2] = position[2];
	pos_info.Orientation[4] = 1.0f;
	pos_info.ImpartedDecay	= decay;
	pos_info.TractorSpeed	= tractor_speed;
	pos_info.TractorID		= player_id;//playerID
	pos_info.TractorEffectID= effect_id;

	SendComponentPositionalUpdate(
		article_id,
		&pos_info,
		timestamp);
}

void Player::HandleInventoryMove(unsigned char *data)
{
    if (ShipIndex()->GetIsIncapacitated())
    {
        SendVaMessageC(17, "Cannot manipulate inventory while incapacitated");
        return;
    }

	ObjectManager *om = GetObjectManager();
	InvMove * Inventory = (InvMove *) data;
	InvMove InvMo;

	_Item Source, Destination;

	InvMo.FromInv = ntohl(Inventory->FromInv);
	InvMo.FromSlot = ntohl(Inventory->FromSlot);
	InvMo.ToSlot = ntohl(Inventory->ToSlot);
	InvMo.ToInv = ntohl(Inventory->ToInv);
	InvMo.GameID = ntohl(Inventory->GameID);
	InvMo.Num = ntohl(Inventory->Num);

	LogDebug("Inventory Move - GameID: %ld From %ld Slot: %ld To: %ld Slot %ld Number: %ld\n", InvMo.GameID,InvMo.FromInv,
				InvMo.FromSlot, InvMo.ToInv, InvMo.ToSlot, InvMo.Num);

	// if you are incapacited you cant equip items
	if (ShipIndex()->GetIsIncapacitated())
    {
		SendMessageString("Unable to move inventory while incapacitated.", 11);
		return;
    }

	//you can only move certain items from certain places (cannot equip from vault, ect)
	switch(InvMo.FromInv)
	{
        // From Cargo Inventory
		case 1:
			if (InvMo.FromSlot >= 0 && InvMo.FromSlot < 100) //sanity check, there a nvMo.ToInv of 0x01000000 which breaks this, not sure what it is.
			{
				Source = *ShipIndex()->Inventory.CargoInv.Item[InvMo.FromSlot].GetData();
			}

			if (InvMo.ToInv == 1)	//cargo to cargo
			{
				Destination = *ShipIndex()->Inventory.CargoInv.Item[InvMo.ToSlot].GetData();

				CheckStack(InvMo.Num, &Source, &Destination);

				ShipIndex()->Inventory.CargoInv.Item[InvMo.ToSlot].SetData(&Source);
				ShipIndex()->Inventory.CargoInv.Item[InvMo.FromSlot].SetData(&Destination);
				SendAuxShip();
				SaveInventoryChange(InvMo.FromSlot);
				SaveInventoryChange(InvMo.ToSlot);
				//TODO: request to save inv slots
			}
			else if(InvMo.ToInv == 2)	//equip from cargo
			{
                if (!m_Equip[InvMo.ToSlot].CanEquip(&Source))
                {
					return;
                }

                //If we are moving ammo, they can stack if they are the same itemid
                if (ShipIndex()->Inventory.AmmoInv.Item[InvMo.ToSlot].GetItemTemplateID() == Source.ItemTemplateID)
                {
                    Destination = *ShipIndex()->Inventory.AmmoInv.Item[InvMo.ToSlot].GetData();
                    CheckStack(InvMo.Num, &Source, &Destination);
                    m_Equip[InvMo.ToSlot].Equip(&Source);
                }
                else
                {
				    Destination = m_Equip[InvMo.ToSlot].Equip(&Source);
                }

				ShipIndex()->Inventory.CargoInv.Item[InvMo.FromSlot].SetData(&Destination);
                SendAuxShip();
				SaveInventoryChange(InvMo.FromSlot);
			}
			else if(InvMo.ToInv == 3)	//cargo to vault
			{
				if (InvMo.ToSlot == -1)
				{
					return;
				}

				Destination = *PlayerIndex()->SecureInv.Item[InvMo.ToSlot].GetData();

				CheckStack(InvMo.Num, &Source, &Destination);

				PlayerIndex()->SecureInv.Item[InvMo.ToSlot].SetData(&Source);
				ShipIndex()->Inventory.CargoInv.Item[InvMo.FromSlot].SetData(&Destination);

				SendAuxPlayer();
				SendAuxShip();
				SaveInventoryChange(InvMo.FromSlot);
				SaveVaultChange(InvMo.ToSlot);
			}
			else if(InvMo.ToInv == 4)	//selling cargo
			{
				if (Source.ItemTemplateID < 0)		// Keeps down XP hacking
					break;

				ItemBase * myItem = g_ItemBaseMgr->GetItem(Source.ItemTemplateID);

				if (!myItem || myItem->TechLevel() > 9) //not allowed to sell items of level 10 or above to vendors (fixes grail water XP hack).
					break;

				if (Source.StackCount <= (u32)InvMo.Num)
				{
					Destination = g_ItemBaseMgr->EmptyItem;
				}
				else
				{
					Destination = Source;
					Destination.StackCount -= InvMo.Num;
                    Destination.TradeStack -= InvMo.Num;
				}

				char msg_buffer[128];

				// something VERY simple and yet efficient
                long XP_earned = 50 * myItem->TechLevel() * (Source.TradeStack < InvMo.Num ? Source.TradeStack : InvMo.Num);

				int sp_len = _snprintf(msg_buffer, 128, "You have gained %ld credits", Source.Price * InvMo.Num);

                if (XP_earned > 0)
                {
                    _snprintf(msg_buffer + sp_len, 128 - sp_len, " and %ld trade experience", XP_earned);
                }

				SendMessageString(msg_buffer, 3);
				SendClientSound("coin.wav");

                if (XP_earned > 0)
                {
				    sprintf(msg_buffer, "Item(s) eligible for");
				    AwardTradeXP(msg_buffer, XP_earned);
                }

				PlayerIndex()->SetCredits(PlayerIndex()->GetCredits() + Source.Price * InvMo.Num);
				SaveCreditLevel();
				SendAuxPlayer();
				SaveCreditLevel();

				ShipIndex()->Inventory.CargoInv.Item[InvMo.FromSlot].SetData(&Destination);
				SendAuxShip();
				SaveInventoryChange(InvMo.FromSlot);
			}
			else if(InvMo.ToInv == 11)	//spaceing item
			{
				Destination = Source;
				Destination.StackCount -= InvMo.Num;

				if (Destination.StackCount <= 0)
				{
					Destination = g_ItemBaseMgr->EmptyItem;
				}

				ShipIndex()->Inventory.CargoInv.Item[InvMo.FromSlot].SetData(&Destination);
				SendAuxShip();
				SaveInventoryChange(InvMo.FromSlot);
			}
			// Result box in Manufacturing
            else if(InvMo.ToInv == 12 && (ManuIndex()->GetMode() == 2 || ManuIndex()->GetMode() == 3))
			{
				Destination = *ManuIndex()->Target.Item[0].GetData();

				ShipIndex()->Inventory.CargoInv.Item[InvMo.FromSlot].SetData(&Destination);
                ManuIndex()->Target.Item[0].SetData(&Source);
				SendAuxShip();
                SendAuxManu();
				SaveInventoryChange(InvMo.FromSlot);
			}
            // Control Interface box in Manufacturing
			else if(InvMo.ToInv == 14 && ManuIndex()->GetMode() != 4)
			{
				Destination = *ManuIndex()->Override.Item[0].GetData();

				ShipIndex()->Inventory.CargoInv.Item[InvMo.FromSlot].SetData(&Destination);
                ManuIndex()->Override.Item[0].SetData(&Source);
				SendAuxShip();
                SendAuxManu();
				SaveInventoryChange(InvMo.FromSlot);
			}
			else if(InvMo.ToInv == 16)	//cargo to trade
			{
				Destination = *ShipIndex()->Inventory.TradeInv.Item[InvMo.ToSlot].GetData();

				if (m_TradeID == -1)
				{
					return;
				}

				Player * targetp;
				targetp = g_ServerMgr->m_PlayerMgr.GetPlayer(m_TradeID);
				if (targetp)
				{
                    targetp->SendItemBase(Source.ItemTemplateID);
					ShipIndex()->Inventory.CargoInv.Item[InvMo.FromSlot].SetData(&Destination);
					SendAuxShip();
					ShipIndex()->Inventory.TradeInv.Item[InvMo.ToSlot].SetData(&Source);
					SendAuxShip(targetp);
					targetp->TradeAction(0,6);

					SaveInventoryChange(InvMo.FromSlot);
				}
			}
			break;

        // From Equip Inventory
		case 2:
			Source = *ShipIndex()->Inventory.EquipInv.EquipItem[InvMo.FromSlot].GetItemData();

			if (InvMo.ToInv == 1)	//unequip item
			{
				Destination = *ShipIndex()->Inventory.CargoInv.Item[InvMo.ToSlot].GetData();

                if (!m_Equip[InvMo.FromSlot].CanEquip(&Destination))
                {
					return;
                }

                Source = m_Equip[InvMo.FromSlot].Equip(&Destination);

				ShipIndex()->Inventory.CargoInv.Item[InvMo.ToSlot].SetData(&Source);
				SendAuxShip();
				SaveInventoryChange(InvMo.ToSlot);
			}
			else if(InvMo.ToInv == 2)	//move equip item
			{
                //If one of these is Reactor, Engine or Shield, you cant move them
                if (InvMo.FromSlot < 3 || InvMo.ToSlot < 3)
                {
                    return;
                }

                //If trying to move a weapon to device, or a device to weapon, prevent it
                if ((InvMo.FromSlot < 9 && InvMo.ToSlot > 8) || (InvMo.FromSlot > 8 && InvMo.ToSlot < 9))
                {
                    return;
                }

				Destination = *ShipIndex()->Inventory.EquipInv.EquipItem[InvMo.ToSlot].GetItemData();

                //Moving two devices, just exchange them
                if (InvMo.FromSlot > 8 || InvMo.ToSlot > 8)
                {
                    Source = m_Equip[InvMo.FromSlot].Equip(&Destination);
                    m_Equip[InvMo.ToSlot].Equip(&Source);
                }
                //Now we are exchanging two weapon slots, the problem is that they can have ammo
                //If either weapon has ammo
                else if (ShipIndex()->Inventory.AmmoInv.Item[InvMo.ToSlot].GetItemTemplateID() != -2 ||
                    ShipIndex()->Inventory.AmmoInv.Item[InvMo.FromSlot].GetItemTemplateID() != -2)
                {
                    //if the weapons are the same, swap the ammo in them, otherwise do nothing
                    if (Source.ItemTemplateID == Destination.ItemTemplateID)
                    {
			            Source = *ShipIndex()->Inventory.AmmoInv.Item[InvMo.FromSlot].GetData();
			            Destination = *ShipIndex()->Inventory.AmmoInv.Item[InvMo.ToSlot].GetData();

                        CheckStack(InvMo.Num, &Source, &Destination);

                        m_Equip[InvMo.FromSlot].Equip(&Destination);
                        m_Equip[InvMo.ToSlot].Equip(&Source);
                    }
                    else
                    {
                        return;
                    }
                }
                else
                {
                    //If neither have ammo, just swap them normally
                    Source = m_Equip[InvMo.FromSlot].Equip(&Destination);
                    m_Equip[InvMo.ToSlot].Equip(&Source);
                }

				SendAuxShip();
			}
			break;

        // From Vault Inventory
		case 3:
			Source = *PlayerIndex()->SecureInv.Item[InvMo.FromSlot].GetData();

			if(InvMo.ToInv == 11 || InvMo.ToSlot == -1)	//destroy item
			{
				Destination = Source;
				Destination.StackCount -= InvMo.Num;

				if (Destination.StackCount <= 0)
				{
					Destination = g_ItemBaseMgr->EmptyItem;
				}

				PlayerIndex()->SecureInv.Item[InvMo.FromSlot].SetData(&Destination);
				SendAuxPlayer();
				SaveVaultChange(InvMo.FromSlot);
			}
			else if (InvMo.ToInv == 1)	//move from vault to cargo
			{
				Destination = *ShipIndex()->Inventory.CargoInv.Item[InvMo.ToSlot].GetData();

				CheckStack(InvMo.Num, &Source, &Destination);

				ShipIndex()->Inventory.CargoInv.Item[InvMo.ToSlot].SetData(&Source);
				PlayerIndex()->SecureInv.Item[InvMo.FromSlot].SetData(&Destination);

				SendAuxShip();
				SendAuxPlayer();
				SaveInventoryChange(InvMo.ToSlot);
				SaveVaultChange(InvMo.FromSlot);
			}
			else if(InvMo.ToInv == 3)	//vault to vault
			{
				Destination = *PlayerIndex()->SecureInv.Item[InvMo.ToSlot].GetData();
				CheckStack(InvMo.Num, &Source, &Destination);

				PlayerIndex()->SecureInv.Item[InvMo.ToSlot].SetData(&Source);
				PlayerIndex()->SecureInv.Item[InvMo.FromSlot].SetData(&Destination);
				SendAuxPlayer();
				SaveVaultChange(InvMo.ToSlot);
				SaveVaultChange(InvMo.FromSlot);
			}

			break;

		// From Vendor Inventory
		case 4:
			if (InvMo.FromSlot >= 0 && InvMo.FromSlot < 100) //sanity check, there a nvMo.ToInv of 0x06000000 which breaks this, not sure what it is.
			{
				Source = *PlayerIndex()->VendorInv.Item[InvMo.FromSlot].GetData();
			}

			if (InvMo.ToInv == 1)	//buy item
			{
				u64 Cost = Source.Price * InvMo.Num;
				if (PlayerIndex()->GetCredits() < Cost)
				{
					SendVaMessage("Insufficient credits!");
					return;
				}

				PlayerIndex()->SetCredits(PlayerIndex()->GetCredits() - Cost);
				SaveCreditLevel();

				//take losses on resales to vendors
				Cost /= 4;

				Source.StackCount = InvMo.Num;	// Trade this many items

				CargoAddItem(&Source);
				SendAuxPlayer();
				SendAuxShip();
			}
			break;

        // From Loot Window
		case 6:
            {
                Object *obj = 0;
				if (om) obj = om->GetObjectFromID(ShipIndex()->GetTargetGameID());
                if (obj)
                {
                    switch (obj->ObjectType())
                    {
                    case OT_HUSK:
                        LootItem(InvMo.FromSlot);
                        break;

                    default:
                        LogMessage("Attempt to loot %s [%d]\n", obj->Name(), obj->GameID());
                    }
                }
                else
                {
                    LogMessage("Attempt to mine invalid object :%d\n", ShipIndex()->GetTargetGameID());
                }
            }
			break;

        // Manufacturing Target
        case 12:
            if (InvMo.ToInv == 1 && (ManuIndex()->GetMode() == 2 || ManuIndex()->GetMode() == 3))
            {
			    Source = *ManuIndex()->Target.Item[0].GetData();

				if (InvMo.ToSlot == -1)
				{
                    CargoAddItem(&Source);
                    ManuIndex()->Target.Item[0].Empty();
				    SendAuxShip();
                    SendAuxManu();
				}
				else if (InvMo.ToInv == 1)
			    {
				    Destination = *ShipIndex()->Inventory.CargoInv.Item[InvMo.ToSlot].GetData();

				    ShipIndex()->Inventory.CargoInv.Item[InvMo.ToSlot].SetData(&Source);
                    ManuIndex()->Target.Item[0].SetData(&Destination);
					SaveInventoryChange(InvMo.ToSlot);
				    SendAuxShip();
                    SendAuxManu();
                }
            }
            break;

        // Manufacturing Override
        case 14:
            if (InvMo.ToInv == 1 && ManuIndex()->GetMode() != 4)
            {
			    Source = *ManuIndex()->Override.Item[0].GetData();

				if (InvMo.ToSlot == -1)
				{
                    CargoAddItem(&Source);
                    ManuIndex()->Override.Item[0].Empty();
				    SendAuxShip();
                    SendAuxManu();
				}
			    else if (InvMo.ToInv == 1)
			    {
				    Destination = *ShipIndex()->Inventory.CargoInv.Item[InvMo.ToSlot].GetData();

				    ShipIndex()->Inventory.CargoInv.Item[InvMo.ToSlot].SetData(&Source);
                    ManuIndex()->Override.Item[0].SetData(&Destination);
					SaveInventoryChange(InvMo.ToSlot);
				    SendAuxShip();
                    SendAuxManu();
                }
            }
            break;

        // From Trade Window
		case 16:

			Source = *ShipIndex()->Inventory.TradeInv.Item[InvMo.FromSlot].GetData();

			if (InvMo.ToInv == 1 && InvMo.ToSlot > 0)	//back to inventory
			{
				Destination = *ShipIndex()->Inventory.CargoInv.Item[InvMo.ToSlot].GetData();

				if (m_TradeID == -1)
				{
					return;
				}

				Player * targetp;
				targetp = g_ServerMgr->m_PlayerMgr.GetPlayer(m_TradeID);
				if (targetp)
				{
                    targetp->SendItemBase(Source.ItemTemplateID);
					ShipIndex()->Inventory.CargoInv.Item[InvMo.ToSlot].SetData(&Source);
					SaveInventoryChange(InvMo.ToSlot);
					SendAuxShip();
					ShipIndex()->Inventory.TradeInv.Item[InvMo.FromSlot].SetData(&Destination);
					SendAuxShip(targetp);
					targetp->TradeAction(0,6);
				}

			}
			break;

        // From Mining Window
		case 18:
            {
                Object *obj = 0;
				if (om) obj = om->GetObjectFromID(ShipIndex()->GetTargetGameID());
                if (obj)
                {
                    switch (obj->ObjectType())
                    {
                    case OT_HUSK:
                        LootItem(InvMo.FromSlot);
                        break;

                    case OT_HULK:
                    case OT_RESOURCE:
                        MineResource(InvMo.FromSlot);
                        break;

                    default:
                        LogMessage("Attempt to loot/mine %s [%d]\n", obj->Name(), obj->GameID());
                    }
                }
                else
                {
                    LogMessage("Attempt to mine invalid object :%d\n", ShipIndex()->GetTargetGameID());
                }
            }
			break;

		default:
			LogMessage("UNRECOGNISED INVENTORY MOVE -- GameID: %ld From: %ld Slot: %ld To: %ld Slot %ld Number: %ld\n", InvMo.GameID,InvMo.FromInv,
				InvMo.FromSlot, InvMo.ToInv, InvMo.ToSlot, InvMo.Num);

			SendVaMessage("UNRECOGNISED INVENTORY MOVE!\nPlease submit a bug report\n");
			return;
		}

}

void Player::HandleItemState(unsigned char *data)
{
    ItemState * Data = (ItemState *) data;

    if (Data->Inventory == 2)
    {
        long ItemState = ShipIndex()->Inventory.EquipInv.EquipItem[Data->ItemNum].GetItemState();

        if (Data->Enable == 1)
        {
            ItemState |= Data->BitMask;
        }
        else
        {
            ItemState &= ~Data->BitMask;
        }

        ShipIndex()->Inventory.EquipInv.EquipItem[Data->ItemNum].SetItemState(ItemState);
        SendAuxShip();
    }
    else
    {
		LogMessage("UNRECOGNISED ITEM STATE:\n");
        DumpBuffer(data, sizeof(ItemState));

		SendVaMessage("UNRECOGNISED ITEM STATE!\nPlease submit a bug report\n");
    }
}

void Player::HandleRequestTarget(unsigned char *data)
{
	RequestTarget * request = (RequestTarget *) data;
    ObjectManager *obj_manager = GetObjectManager();
	Object *obj = (0);

	if (obj_manager)
	{
		obj = obj_manager->GetObjectFromID(request->TargetID);
	}

	//LogMessage("Received RequestTarget packet\n");

	if (m_ProspectWindow == true)
	{
		m_ProspectWindow = false;
		OpenInterface(1,0);
	}

	SendSetTarget(request->TargetID, -1);

	ShipIndex()->SetTargetGameID(request->TargetID);


	// TB to David: Please tidy up the braces so they match the rest of the N7 source code.
	// and fix typos, "Impossible"

	if (obj && obj->ObjectType() == OT_MOB) {
		int Lvl = obj->Level();
		char Threat[40];
		sprintf(Threat, "Level %d", Lvl);
		ShipIndex()->SetTargetThreat(Threat);
		//ShipIndex()->SetTargetThreatLevel(Lvl);		// If Player Use this

	} else if (obj && obj->ObjectType() == OT_PLAYER) {

		Player *p = g_ServerMgr->m_PlayerMgr.GetPlayer(request->TargetID);

		if (p)
		{
			int LvlDif = p->CombatLevel() - CombatLevel();
			char Threat[40];

			if (LvlDif >= 8)
			{
				sprintf(Threat, "Impossible");
			} else if (LvlDif >= 5)
			{
				sprintf(Threat, "Very Hard");
			} else if (LvlDif >= 2)
			{
				sprintf(Threat, "Hard");
			} else if (LvlDif >= -1)
			{
				sprintf(Threat, "Even");
			} else if (LvlDif >= -4)
			{
				sprintf(Threat, "Easy");
			} else if (LvlDif >= -7)
			{
				sprintf(Threat, "Very Easy");
			} else {
				sprintf(Threat, "Pathetic");
			}

			ShipIndex()->SetTargetThreat(Threat);
		}
	} else {
		ShipIndex()->SetTargetThreatLevel(0);
		ShipIndex()->SetTargetThreat("");
	}
	SendAuxShip();
    BlankVerbs();

    if (obj)
    {
        obj->OnTargeted(this);
    }
}

void Player::HandleRequestTargetsTarget(unsigned char *data)
{
	RequestTarget * request = (RequestTarget *) data;
	Player *p = g_ServerMgr->m_PlayerMgr.GetPlayer(request->TargetID);

	if (p)
	{
		*((int *) &data[4]) = p->ShipIndex()->GetTargetGameID();
		HandleRequestTarget(data);
	}
	else
	{
		SendSetTarget(0, -1);
		ShipIndex()->SetTargetGameID(-1);
		SendAuxShip();
	}
}

bool Player::CheckResourceLock(long object_id)
{
	Player * player = 0;
	ObjectManager *om = GetObjectManager();
	Object *obj = (0);
	if (om) obj = om->GetObjectFromID(object_id);

	if (obj)
	{
		player = obj->CheckResourceLock();
	}

	if (player && player != this)
	{
		SendVaMessage("%s is being mined by %s", obj->Name(), player->Name());
		return false;
	}
	else
	{
		return true;
	}
}

long Player::CurrentResourceTarget()
{
	if (m_ProspectWindow)
	{
		return (ShipIndex()->GetTargetGameID());
	}
	else
	{
		return 0;
	}
}

void Player::HandleVerbRequest(unsigned char *data)
{
	//Here is a list of VerbID's for HUD icons:
	//01 - scan;	02 - land;		03 - loot
	//04 - group;	05 - message;	06 - trade
	//07 - tractor;	08 - Dock;		09 - Prospect
	//0a - gate;	0b - register	0c - jumpstart
	//0d - follow

	//And Attributes:
	//00 - Enabled	 (all others disabled)
	//01 - Player already in group
	//02 - Too far
	//03+ - Unavailable -- Disabled with no reason given

	VerbRequest * pkt = (VerbRequest *) data;

	long subject_id = (long) ntohl(pkt->SubjectID);
	long object_id = (long) ntohl(pkt->ObjectID);

	//LogMessage("Received VerbRequest packet {SubjectID=%d, ObjectID=%d, Action=%d}\n",
	//    subject_id, object_id, pkt->Action);

	if (subject_id == GameID() && pkt->Action == 1)
	{
        UpdateVerbs(true);
	}
}

void Player::OpenInterface(long UIChange, long UIType)
{
	SetInterface set_interface;
	set_interface.UIChange = UIChange;
	set_interface.UIType = UIType;

	//LogMessage("Sending SetInterface packet\n");
	SendOpcode(ENB_OPCODE_0066_OPEN_INTERFACE, (unsigned char *) &set_interface, sizeof(set_interface));
}

void Player::CloseInterfaceIfTargetted(long target_id)
{
	if (ShipIndex()->GetTargetGameID() == target_id)
	{
		m_ProspectWindow = false;
		OpenInterface(1,0);
	}
}

void Player::CloseInterfaceIfOpen()
{
	if (m_ProspectWindow)
	{
		m_ProspectWindow = false;
		OpenInterface(1,0);
	}
}

void Player::SendAttackerUpdates(long mob_id, long update)
{
	unsigned char attacker_data[] =
	{
		0x00, 0x00, 0x00, 0x00,
		0x01,
		0x00, 0x00, 0x00, 0x00 //[5]
	};

	*((long *) &attacker_data[0]) = update;
	*((long *) &attacker_data[5]) = mob_id;

	SendOpcode(ENB_OPCODE_008B_ATTACKER_UPDATES, attacker_data, sizeof(attacker_data));
}
void Player::SendChangeBasset(ChangeBaseAsset *NewAsset)
{
	SendOpcode(ENB_OPCODE_0026_CHANGE_BASE_ASSET, (unsigned char*) NewAsset, sizeof(ChangeBaseAsset));
}

void Player::SendObjectLinkedEffectRL(short bitmask, long UID, long effectID, short effectDID, long effect_time)
{
	//send an effect linked to our ship
	ObjectToObjectEffect OBTOBE;

	OBTOBE.Bitmask = bitmask;
	OBTOBE.GameID = GameID();
	OBTOBE.TargetID = UID;
	OBTOBE.EffectDescID = effectDID;
	OBTOBE.Message = 0;
	OBTOBE.EffectID = effectID;
	OBTOBE.Duration = short(effect_time);
	OBTOBE.TimeStamp = GetNet7TickCount();

	SendObjectToObjectEffectRL(&OBTOBE);
}

void Player::CheckObjectRanges()
{
	ObjectManager *om = GetObjectManager();
    if (!m_DebugPlayer && om)
    {
        om->DisplayDynamicObjects(this);
    }

	CheckArrivalTriggers();
}

void Player::SendSetTarget(int game_id, int target_id)
{
	SetTarget set_target;
	set_target.GameID = game_id;
	set_target.TargetID = target_id;

	SendOpcode(ENB_OPCODE_0019_SET_TARGET, (unsigned char *) &set_target, sizeof(set_target));
}

void Player::SendRemoveEffect(int target_id)
{
	SendOpcode(ENB_OPCODE_000F_REMOVE_EFFECT, (unsigned char *) &target_id, sizeof(int));
}

void Player::TradeAction(long GameID, int Action)
{
	unsigned char buffer[5];

	*((long *) &buffer[0]) = GameID;
	*((char *) &buffer[4]) = (char) Action;

	SendOpcode(ENB_OPCODE_001F_TRADE, buffer, 5);
}

void Player::SendConfirmation(char * msg, int PlayerID, int Ability)
{
	unsigned char buffer[2000];
	int index = 0;

	// Save this data so we can call back
	m_Confirmation = 2;
	m_Confirmation_PlayerID = PlayerID;
	m_Confirmation_Ability = Ability;

	*((short *) &buffer[index]) = strlen(msg) + 1; index += 2;
	*((char *) &buffer[index]) = (char) 0x01; index++;
	memcpy(&buffer[index], msg, strlen(msg) + 1); index += (strlen(msg) + 1);

	SendOpcode(ENB_OPCODE_001E_GROUP, buffer, index);
}

void Player::HandleAction(unsigned char *data)
{
	ActionPacket * myAction = (ActionPacket *) data;
	char message[128];
	ObjectManager *om = GetObjectManager();
	Object *obj = (0);
	if (om) obj = om->GetObjectFromID(ShipIndex()->GetTargetGameID());

    LogDebug("Action - ID: 0x%x, Action: %d, Target: %d, OptVar: %d\n",myAction->GameID, myAction->Action, myAction->Target, myAction->OptionalVar);

	int x;

	switch (myAction->Action)
	{
		case 1:		//tractor
			if (ShipIndex()->GetIsIncapacitated())
			{
				SendMessageString("Unable to tractor while incapacitated.", 11);
				return;
			}
			if (obj)
			{
				//shouldnt targeted ID be myAction->target??
                //you would think that, but for some reason HULK action doesn't send the target ID...
                switch (obj->ObjectType())
                {
                case OT_FLOATING_ORE:
                    LootItem(0);
                    break;

                case OT_HUSK:
                    if (m_ProspectWindow == false && CheckResourceLock(ShipIndex()->GetTargetGameID()))
                    {
                        SendResourceContentsAUX(obj);
                        m_ProspectWindow = true;
                        OpenInterface(0, 0);
						SendMessageString("\0", 17);
					}
					else
					{
						m_ProspectWindow = false;
						OpenInterface(1,0);
					}
                    break;

                default:
                    break;
                }
				break;
			}
        case 7:     //docking complete
            if (obj)
			{
                long destination = obj->Destination();
                if (destination == 0)
                {
					SendVaMessage("DOCKING FAULT: Station needs docking sector to be set in DASE. Inform content devs.");
					break;
                }
                m_SectorMgr->SectorServerHandoff(this, destination);
                break;
            }
		case 8:		//land
			if (ShipIndex()->GetIsIncapacitated())
			{
				SendMessageString("Unable to land while incapacitated.", 11);
				return;
			}
			if (obj)
			{
				if (obj->Destination() > 0)
				{
					// Cancel trade if docking
					if (m_TradeID != -1)
						CancelTrade();

					TerminateWarp();
					m_SectorMgr->SectorServerHandoff(this, obj->Destination());
					return;
				}
				break;
			}
		case 9:		//fire all weapons
			{
				// if you are incapacited you cant equip items
				if (ShipIndex()->GetIsIncapacitated())
				{
					SendMessageString("Unable to fire weapons while incapacitated.", 11);
					return;
				}
                FireAllWeapons();
				break;
			}
		case 10:	//invite target to group
			{
				g_ServerMgr->m_PlayerMgr.GroupInvite(GroupID(),GameID(),myAction->Target);
				break;
			}
		case 11:	//accept group invitation
			{
				// See if we have a conformation up insted of group
				if (m_Confirmation == 2)
				{
					Player * p = g_ServerMgr->m_PlayerMgr.GetPlayer(m_Confirmation_PlayerID);

					if (p && p->m_AbilityList[m_Confirmation_Ability])
					{
						// Call back and give a responce
						p->m_AbilityList[m_Confirmation_Ability]->Confirmation(true);
					}

					// reset data
					m_Confirmation = 0;
				}
				else
				{
					g_ServerMgr->m_PlayerMgr.AcceptGroupInvite(GroupID(),GameID());
					m_Confirmation = 0;
				}
				break;
			}
		case 12:	//decline group invitation
			{
				// See if we have a conformation up insted of group
				if (m_Confirmation == 2)
				{
					Player * p = g_ServerMgr->m_PlayerMgr.GetPlayer(m_Confirmation_PlayerID);

					if (p && p->m_AbilityList[m_Confirmation_Ability])
					{
						// Call back and give a responce
						p->m_AbilityList[m_Confirmation_Ability]->Confirmation(false);
					}

					// reset data
					m_Confirmation = 0;
				}
				else
				{
					m_Confirmation = 0;
					g_ServerMgr->m_PlayerMgr.RejectGroupInvite(GroupID(), GameID());
				}
				break;
			}
		case 13:	//disban group
			{
				g_ServerMgr->m_PlayerMgr.DisbanGroup(GroupID(), GameID());
				break;
			}
		case 14:	//leave group
			{
				g_ServerMgr->m_PlayerMgr.LeaveGroup(GroupID(), GameID());
				break;
			}
		case 15:	//kick target from group
			{
				g_ServerMgr->m_PlayerMgr.KickFromGroup(GroupID(), GameID(), myAction->Target);
				break;
			}
		case 17:	//mine
			{
				if (ShipIndex()->GetIsIncapacitated())
				{
					SendMessageString("Unable to mine while incapacitated.", 11);
					return;
				}
				//if window open close it
				if (m_ProspectWindow)
				{
					m_ProspectWindow = false;
					OpenInterface(1,0);
					break;
				}

				if (obj != 0 && (obj->ObjectType() == OT_RESOURCE || obj->ObjectType() == OT_HULK) &&
                    CheckResourceLock(myAction->Target))
				{
					m_ProspectWindow = true;
					OpenInterface(0,0);
					SendMessageString("\0", 17);
				}
				break;
			}
		case 18:	//gate button
			{
				if (ShipIndex()->GetIsIncapacitated())
				{
					SendMessageString("Unable to gate while incapacitated.", 11);
					return;
				}
				if (obj != 0 && obj->ObjectType() == OT_STARGATE)
				{
					if (this->GroupID() != -1)
					{
						g_ServerMgr->m_PlayerMgr.BreakFormation(GameID());
						g_ServerMgr->m_PlayerMgr.LeaveFormation(GameID());
					}
                    TerminateWarp();
					SendClientSound("1512_00_032Se.mp3",0,0);
					m_SectorMgr->GateActivate(this, myAction->Target);
				}
				break;
			}
		case 19:	//finish gate sequence
            {
                m_SectorMgr->SectorServerHandoff(this, StargateDestination());
            }
			break;

		//TODO: Remove confirm if trade changes
		case 20:	//trade
			{
				Player * targetp = g_ServerMgr->m_PlayerMgr.GetPlayer(myAction->Target);
				if (targetp)
				{
					if (targetp->m_TradeID != -1)
					{
						SendVaMessage("That player is already trading with someone!");
					}

					for(x=0;x<6;x++)
					{
						targetp->ShipIndex()->Inventory.TradeInv.Item[x].Empty();
						ShipIndex()->Inventory.TradeInv.Item[x].Empty();
					}

					SendAuxShip(targetp);
					targetp->SendAuxShip(this);

					// TESTING
					if (!Active() && myAction->Target != GameID())
					{
						SendCreate(targetp->GameID(), 1, 0x4B06, CREATE_SHIP, 0, 0, 0);
						targetp->SendCreate(GameID(), 1, 0x4B06, CREATE_SHIP, 0, 0, 0);
					}
					// -------

					TradeAction(myAction->Target,0);						// Opens a trade window
					targetp->TradeAction(myAction->GameID, 0);	// Open trade window for other player
					m_TradeID = myAction->Target;							// Set player tradeing with
					m_TradeConfirm = 0;
					targetp->m_TradeID = myAction->GameID;
					targetp->m_TradeConfirm = 0;
				}
			}
			break;
		case 21:	//confirm trade
			{
				Player * targetp;
				targetp = g_ServerMgr->m_PlayerMgr.GetPlayer(m_TradeID);
				m_TradeConfirm = 1;
				TradeAction(0,3);

				if (targetp)
				{
					targetp->TradeAction(0,5);
				}

				if (targetp && targetp->m_TradeConfirm == 1)
				{
					LogMessage("Trade comfirned for players %x and %x\n",myAction->GameID,myAction->Target);

					//close windows and reset tradeIds
					TradeAction(m_TradeID,2);
					targetp->TradeAction(myAction->GameID, 2);
					m_TradeID = -1;
					targetp->m_TradeID = -1;

					//add and remove credits
					PlayerIndex()->SetCredits(PlayerIndex()->GetCredits() + targetp->ShipIndex()->GetTradeMoney() - ShipIndex()->GetTradeMoney());
					SaveCreditLevel();
					SendAuxPlayer();
					targetp->PlayerIndex()->SetCredits(targetp->PlayerIndex()->GetCredits() + ShipIndex()->GetTradeMoney() - targetp->ShipIndex()->GetTradeMoney());
					targetp->SaveCreditLevel();
					targetp->SendAuxPlayer();

					//Add other player's items
					for(x=0;x<6;x++)
					{
						CargoAddItem(targetp->ShipIndex()->Inventory.TradeInv.Item[x].GetData());
					}
					SendAuxShip();

					//add items to other player
					for(x=0;x<6;x++)
					{
						targetp->CargoAddItem(ShipIndex()->Inventory.TradeInv.Item[x].GetData());
					}
					targetp->SendAuxShip();

					//clear trade fr both players
					LogMessage("Clearing trade for players %x and %x\n",myAction->GameID,myAction->Target);
					for(x=0;x<6;x++)
					{
						targetp->ShipIndex()->Inventory.TradeInv.Item[x].Empty();
						ShipIndex()->Inventory.TradeInv.Item[x].Empty();
					}

					SendAuxShip(targetp);
					targetp->SendAuxShip(this);

					// Reset money
					targetp->ShipIndex()->SetTradeMoney(0);
					ShipIndex()->SetTradeMoney(0);


					// TESTING
					if (!Active() && myAction->Target != GameID())		// if we are in a station use this
					{
						RemoveObject(targetp->GameID());
						targetp->RemoveObject(GameID());
					}
					// ----
				}
			}
			break;
		case 22:	//cancel trade
			{
				CancelTrade();
			}
			break;
		case 23:	//keep trading???
			{
				SendVaMessage("ACTION 23. Target: %d",myAction->Target);
				break;
			}
		case 24:	//trade money
			{
				// TODO: We need to log all player trades for the GMs
				Player * targetp;
				targetp = g_ServerMgr->m_PlayerMgr.GetPlayer(m_TradeID);
				if (targetp)
				{
					ShipIndex()->SetTradeMoney(myAction->OptionalVar);
					SendAuxShip(targetp);
					SendAuxShip();
					TradeAction(m_TradeID,4);
					targetp->TradeAction(m_TradeID,4);
				}
			}
			break;
		case 25:	//register
			if (obj)
			{
				//Starbase Registration
				if (obj->ObjectType() == OT_STATION && m_RegisteredSectorID != obj->Destination())
				{
                    SendClientSound("Reg_OK",0,0);
					_snprintf(message, 128, "%s control: Registration Confirmed.", obj->Name());
					SendMessageString(message,5);

					m_RegisteredSectorID = obj->Destination();

					SaveRegisteredStarbase();

					PlayerIndex()->SetRegistrationStarbase(obj->Name());
					PlayerIndex()->SetRegistrationStarbaseSector(PlayerIndex()->GetSectorName());
				}
				break;
			}
		case 26:	// Jump Start
            if (!obj)
            {
                SendVaMessage("Invalid JS target! SUBMIT BUG REPORT!");
                return;
            }

			if (obj->ObjectType() == OT_PLAYER)
			{
				// Make sure we can use the ability
				if (m_AbilityList[JUMPSTART]->CanUse(obj->GameID(), JUMPSTART, SKILL_JUMPSTART))
				{
					// Execute the ability
					m_AbilityList[JUMPSTART]->Use(obj->GameID());
				}
			}
			break;
		case 28:	//dock
			// if you are incapacited you cant equip items
			if (ShipIndex()->GetIsIncapacitated())
			{
				SendMessageString("Unable to dock while incapacitated.", 11);
				return;
			}
			if (obj)
			{
				if (obj->ObjectType() == OT_STATION)
				{
					if (this->GroupID() != -1)
					{
						g_ServerMgr->m_PlayerMgr.BreakFormation(GameID());
						g_ServerMgr->m_PlayerMgr.LeaveFormation(GameID());
					}
					if (m_TradeID != -1)
                    {
						CancelTrade();
                    }
					m_SectorMgr->Dock(this, obj->GameID());
				}
				break;
			}
		case 29:	//planet landing button
			{
				SendClientSound("1512_00_032Se.mp3",0,0);
				break;
			}
        case 30:    //scan object in space
            {
                long UID = m_SectorMgr->GetSectorNextObjID();
                SendEffectRL(obj->GameID(), 0, 10007, UID, GetNet7TickCount(), 3000);
                //by default when you scan something you cause a mission advance
                MissionObjectScan(obj);
                break;
            }

		default:
			LogMessage("UNRECOGNIZED ACTION! ID: %d, Action: %d, Target: %d, OptVar: %d\n",myAction->GameID, myAction->Action, myAction->Target, myAction->OptionalVar);
			SendVaMessage("UNRECOGNIZED ACTION! SUBMIT BUG REPORT!");
			SendVaMessage("Action ID: %d, Action: %d, Target: %d, OptVar: %d",myAction->GameID, myAction->Action, myAction->Target, myAction->OptionalVar);
			break;
	}
}


void Player::CancelTrade()
{
	Player * targetp;
	targetp = g_ServerMgr->m_PlayerMgr.GetPlayer(m_TradeID);
	int x;

	if (targetp)
	{
		TradeAction(m_TradeID,1);									// Closes a trade window
		targetp->TradeAction(GameID(), 1);	// Closes trade window for other player
		m_TradeID = -1;												// No longer Tradeing
		targetp->m_TradeID = -1;

		//return player's items
		for(x=0;x<6;x++)
		{
			CargoAddItem(ShipIndex()->Inventory.TradeInv.Item[x].GetData());
		}
		SendAuxShip();

		//return other player's items
		for(x=0;x<6;x++)
		{
			targetp->CargoAddItem(targetp->ShipIndex()->Inventory.TradeInv.Item[x].GetData());
		}
		targetp->SendAuxShip();

		//clear trade fr both players
		//LogMessage("Clearing trade for players %x and %x\n", GameID(), m_TradeID);
		for(x=0;x<6;x++)
		{
			targetp->ShipIndex()->Inventory.TradeInv.Item[x].Empty();
			ShipIndex()->Inventory.TradeInv.Item[x].Empty();
		}

		SendAuxShip(targetp);
		targetp->SendAuxShip(this);

		targetp->m_TradeConfirm = 0;
		m_TradeConfirm = 0;

		// Reset money
		targetp->ShipIndex()->SetTradeMoney(0);
		ShipIndex()->SetTradeMoney(0);

		// TESTING
		if (!Active() && m_TradeID != GameID())
		{
			RemoveObject(targetp->GameID());
			targetp->RemoveObject(GameID());
		}
		// ----
	}
}

void Player::SendResourceLevel(long target_id)
{
	Object *obj = m_ObjectMgr->GetObjectFromID(target_id);
	float resource_remains;

	if (obj)
	{
		resource_remains = obj->ResourceRemains();//m_SectorMgr->CalcResourceRemains(obj);
		unsigned char aux_data[] =
		{
			0x00, 0x00, 0x00, 0x00,
			0x0C, 0x00,
			0x01,
			0xC6, 0x02,
			0x05,
			0x00, 0x00, 0x00, 0x00,
			0x00, 0x00,	0x00, 0x00
		};

		*((long *) aux_data) = target_id;
		*((float *) &aux_data[10]) = resource_remains;
		*((long *) &aux_data[14]) = obj->Level();

		SendOpcode(ENB_OPCODE_001B_AUX_DATA, aux_data, sizeof(aux_data));
	}
}

/*
void Player::SendResourceContentsAUXForHulk(Object *obj)
{
    bool contents_already_sent;

    if (obj)
    {
        contents_already_sent = obj->GetIndex(ResourceSendList());

        if (!contents_already_sent)
        {
            unsigned char aux_data[] = //NB this only supports 4 ores in the resource inventory
            {
                0x00, 0x00, 0x00, 0x00,
                0x3A, 0x01,
                0x01,
                0xA6, 0x02, 0xF6, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F,
                0x36, 0x00, 0xFF,
                0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00,
                0x36, 0x00, 0xFF,
                0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00,
                0x36, 0x00, 0xFF,
                0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00,
                0x36, 0x00, 0xFF,
                0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00,
                0x16, 0x80, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0x16, 0x80, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF,
                0x16, 0x80, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0x16, 0x80, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF,
                0x16, 0x80, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0x16, 0x80, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF,
                0x16, 0x80, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0x16, 0x80, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF,
                0x16, 0x80, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0x16, 0x80, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF,
                0x16, 0x80, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0x16, 0x80, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF,
                0x16, 0x80, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0x16, 0x80, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF,
                0x16, 0x80, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0x16, 0x80, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF,
                0x16, 0x80, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0x16, 0x80, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF,
                0x16, 0x80, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0x16, 0x80, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF,
                0x16, 0x80, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0x16, 0x80, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF,
                0x16, 0x80, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0x16, 0x80, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF,
                0x16, 0x80, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0x16, 0x80, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF,
                0x16, 0x80, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0x16, 0x80, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF,
                0x16, 0x80, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0x16, 0x80, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF,
                0x16, 0x80, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0x16, 0x80, 0xFF, 0xFE, 0xFF,	0xFF, 0xFF,
                0x16, 0x80, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0x16, 0x80, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF,
                0x16, 0x80, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0x16, 0x80, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF,
                0x00, 0x00,	0x00, 0x00 //resource tech level
            };
            short entries = 0;
            int index = 0;
            ContentSlot *slot;
            bool client_already_has_itembase = false;
            for (int x = 0; x < MAX_ITEMS_PER_RESOURCE; x++)
            {
                slot = obj->GetContents(x);
                if (slot->stack > 0 && slot->item)
                {
                    index = 23 + x*11;

                    *((long *) &aux_data[index]) = slot->item->m_ItemTemplateID;
                    *((long *) &aux_data[index+4]) = slot->stack;
                    slot->item->SendItemBasePacket(this);
                    entries++;
                }
            }

            *((long *) aux_data) = obj->GameID();
            *((long *) &aux_data[316]) = obj->Level();

            SendOpcode(ENB_OPCODE_001B_AUX_DATA, aux_data, sizeof(aux_data));
            obj->SetIndex(ResourceSendList());
        }
    }
}*/

//TODO: make this part of AUX handler.
void Player::SendResourceContentsAUX(Object *obj)
{
    bool contents_already_sent;

    if (obj)
    {
        contents_already_sent = obj->GetIndex(ResourceSendList());

        if (!contents_already_sent)
        {
            unsigned char spacer[4] =
            {
                0x36, 0x00, 0xFF
            };
            unsigned char filler[8] =
            {
                0x16, 0x80, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF //6
            };
            unsigned char prologue[16] =
            {
                0x3A, 0x01,
                0x01,
                0xA6, 0x02, 0xF6, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F //15
            };

            unsigned char packet[340];

            int index = 0;
            ContentSlot *slot;

            AddData(packet, obj->GameID(), index);
            AddBuffer(packet, prologue, 16, index);

			int x;
            for (x = 0; x < MAX_ITEMS_PER_RESOURCE; x++)
            {
                slot = obj->GetContents(x);
                if (slot->stack > 0 && slot->item)
                {
                    AddBuffer(packet, spacer, 3, index);
                    AddData(packet, slot->item->ItemTemplateID(), index);
                    AddData(packet, long(slot->stack), index);
                    SendItemBase(slot->item->ItemTemplateID());
                }
                else
                {
                    AddBuffer(packet, filler, 7, index);
                }
            }

            for (x = 0; x < 32; x++)
            {
                AddBuffer(packet, filler, 7, index);
            }

            AddData(packet, (long)obj->Level(), index);

            //set length
            *((short *) &packet[4]) = index-10;

            SendOpcode(ENB_OPCODE_001B_AUX_DATA, packet, index);
            obj->SetIndex(ResourceSendList());
        }
    }
}

void Player::SendCameraControl(long Message, long GameID)
{
	CameraControl data;
	data.Message = Message;
	data.GameID = GameID;

	//LogMessage("Sending CameraControl packet\n");
	SendOpcode(ENB_OPCODE_0092_CAMERA_CONTROL, (unsigned char *) &data, sizeof(data));
}

bool Player::MatchOptWithParam (const char *option, char *arg, char *&param, bool &msg_sent)
{
	int len = strlen(option);

	if (strncmp(option, arg, len) == 0)
	{
		if (arg[len] == '=' || arg[len] == ' ')
		{
			param = arg + len + 1;
			return true;
		}
		else
		{
			SendVaMessage("Missing arg for option %s", option);
			msg_sent = true;
		}
	}

	return false;
}

void Player::HandleEquipUse(unsigned char *data)
{
	EquipUse *myUse = (EquipUse *) data;

    m_Equip[myUse->InvSlot].ManualActivate();
}

void Player::SendClientDamage(long target_id, long source_id, float damage, float modifier, long type, long inflicted)
{
	int index = 0;
	unsigned char packet[32];

	AddData(packet, damage, index);
	AddData(packet, modifier, index);
	AddData(packet, type, index);
	AddData(packet, inflicted, index);
	AddData(packet, source_id, index);
	AddData(packet, target_id, index);

	SendOpcode(ENB_OPCODE_0064_CLIENT_DAMAGE, packet, index);
}

//what's this?
void Player::Dialog(char *Stringd, int Type)
{
	int Index = 0;
	unsigned char Data[75];

	*((short*) &Data[Index]) = strlen(Stringd) + 1;		// String Size
	Index+=2;
	*((long*) &Data[Index]) = Type;						// Type
	Index+=4;
	memcpy(&Data[Index], Stringd, strlen(Stringd) + 1);		// copy the string
	Index+=strlen(Stringd)+1;

	SendOpcode(0x62, (unsigned char *) &Data, Index);
}

void Player::HandleClientChat(unsigned char *data)
{
	ClientChat * chat = (ClientChat *) data;
	const char * types[] =
	{
		"To Target",
		"To Group",
		"To Guild",
		"To Local Area",
		"To Entire Sector"
	};

	LogDebug("Received ClientChat packet -- GameID=%d  Type=%d (%s)\n",
	    chat->GameID, chat->Type, types[chat->Type]);

	char *p = chat->String;
	p += strlen(p) + 1;
	short length = *((short *) p);
	p += 2;

	// Send to everyone
	if ((chat->Type == 4) && (chat->String[0] != '/'))
	{
		g_ServerMgr->m_PlayerMgr.ChatSendEveryone(chat->GameID, chat->String);
	}

	if ((chat->Type == 1) && this)
	{
		if (GroupID() != -1)
			g_ServerMgr->m_PlayerMgr.GroupChat(GroupID(),Name(), chat->String);
		else
			SendVaMessage("Error: You are not in a group!");
	}

	if (chat->String[0] == '/')
	{
		HandleSlashCommands(chat->String);
	}
}


void Player::HandleSlashCommands(char *Msg)
{
	bool success = false;
	bool msg_sent = false;

	// TODO: Handle slash commands here
	//
	//      - Add slash command '/opcode' to allow user to enter hex opcodes to be echoed
	//          back to the client application for opcode testing.
	//
	//          -- interpret data as hex (-x) (default)
	//          -- interpret data as little endian integer data (-i) (-i2) (-i4)
	//          -- interpret data as big endian integer data (-I2) (-I4)
	//          -- interpret data as floating point data (-f)
	//
	//      - (Navigation)  /n int gameID, float signature, byte visited, int navtype, byte ishuge
	//      - (ConstPositionalUp) /c int gameID, float x, float y, float z
	//      - (Create Object)  /o int gameID, float scale, short asset, int type, float x, float y, float x
	//      - (Remove Object)  /e int gameID
	//

    //TODO: reformat this to remove C hacker style braces ('} else {') and other unreadable formatting
    //      the rest of the Net7 source code adheres to the new C++ style standard
    //      please change this to use Net7/C++ formatting style ASAP.

	// This is for GM/DEV/ADMIN commands!




	if (Msg[0] == '/' && Msg[1] == '/' && Msg[2] != 0 && AdminLevel() >= 50)
	{
		char *param;
		char *pch = (char*)_alloca(strlen(&Msg[2]) + 1);//copy to stack to avoid heap fragment
		strcpy(pch, &Msg[2]);
		int retval = 0;

		switch(*pch)
		{
			case 'a':
            {
                if (MatchOptWithParam("adduser", pch, param, msg_sent))
				{
					char *Username = strtok(param, " ");
					char *Password = strtok(NULL, " ");
					char *Access = strtok(NULL, " ");

                    if (!Username || !Password || !Access)
                    {
						SendVaMessage("Syntax: //adduser <username> <password> <access>");
                        return;
                    }

					if (g_AccountMgr->AddUser(Username, Password, Access))
                    {
					    SendVaMessage("Account %s / %s / %s Created", Username, Password, Access);
                    }
                    else
                    {
					    SendVaMessage("Account creation failed!");
                    }

                    msg_sent = true;
				}
            }
			break;

			case 'r':
			{
				if (strcmp(pch, "rstations") == 0)
				{
					// Read in stations again
					SendVaMessage("Reloading Stations...");
					g_ServerMgr->m_StationMgr.LoadStations();
					SendVaMessage("Station Reload Complete.");
					if (PlayerIndex()->GetSectorNum() > 9999)
					{
						SendVaMessage("You must launch into space and re-dock for changes.");
					}

					msg_sent = true;
					success = true;
				}
				else if (strcmp(pch, "rsectors") == 0)
				{
					// Read in sectors again
					long scan_range = ShipIndex()->CurrentStats.GetScanRange();
					ShipIndex()->CurrentStats.SetScanRange(10); //set ship almost blind
					CheckObjectRanges();

					SendVaMessage("Reloading Sectors and mobs...");
					g_ServerMgr->m_MOBList.LoadMOBContent();
					g_ServerMgr->m_SectorContent.LoadSectorContent();
					SendVaMessage("Sector Reload Complete.");
					if (PlayerIndex()->GetSectorNum() < 9999)
					{
						SendVaMessage("You must change sectors to see changes to navs.");
					}

					ShipIndex()->CurrentStats.SetScanRange(scan_range);
					ResetRangeLists();
					CheckObjectRanges();

					msg_sent = true;
					success = true;
				}
				else if (strcmp(pch, "ritems") == 0)
				{
					SendVaMessage("Reloading Items...");
					g_ServerMgr->m_ItemBaseMgr.Initialize();
					SendVaMessage("Items Reloaded.");
					msg_sent = true;
					success = true;
				}
				else if (strcmp(pch, "rmissions") == 0)
				{
					SendVaMessage("Reloading Missions...");
					g_ServerMgr->m_Missions.LoadMissionContent();
					SendVaMessage("Missions Reloaded.");
					msg_sent = true;
					success = true;
				}
			}
			break;

			case 's':
			{
				if (MatchOptWithParam("setpassword", pch, param, msg_sent))
				{
					char *Username = strtok(param, " ");
					char *Password = strtok(NULL, " ");

                    if (!Username || !Password)
                    {
						SendVaMessage("Syntax: //setpassword <username> <password>");
                        return;
                    }

					g_AccountMgr->ChangePassword(Username, Password);
					SendVaMessage("Account %s password has been set to %s", Username, Password);
                    msg_sent = true;
				}
			}
            break;

			case 'g':
			{
				if (MatchOptWithParam("gmgetaccess", pch, param, msg_sent))
				{
                    Player * target = g_PlayerMgr->GetPlayer(param);
                    if (!target)
                    {
					    SendVaMessage("Player `%s` not found", param);
					    return;
                    }

					SendVaMessage("Access level for `%s` is %d", param, target->AdminLevel());
					msg_sent = true;
					success = true;
				}

				if (MatchOptWithParam("gmsetaccess", pch, param, msg_sent))
				{
					char *Username = strtok(param, " ");
					char *Access = strtok(NULL, " ");

					if (!Username || !Access)
                    {
						SendVaMessage("Syntax: //gmsetaccess <playername> <password>");
                        return;
                    }

                    Player * target = g_PlayerMgr->GetPlayer(Username);
                    if (!target)
                    {
					    SendVaMessage("Player `%s` not found", Username);
					    return;
                    }

                    long new_access = atoi(Access);

					// Can't promote over your level
					if (new_access > AdminLevel())
						new_access = AdminLevel();

					// Can't demote someone above your access
					if (g_AccountMgr->GetAccountStatus(target->AccountUsername()) > AdminLevel())
					{
						SendVaMessage("Can't change access for player above your access level");
						msg_sent = true;
					}
                    else
                    {
						g_AccountMgr->SetAccountStatus(target->AccountUsername(), new_access);
						SendVaMessage("Player %s has access set to %d", Username, new_access);
						msg_sent = true;
						success = true;
					}
				}

				if (MatchOptWithParam("gmskillpoints", pch, param, msg_sent))
				{
					char *Username = 0, *SSkillPoints = 0;
					int SkillPoints;

					Username = strtok(param, " ");
					if (Username) {
						SSkillPoints = strtok(NULL, " ");
						if (SSkillPoints)
							SkillPoints = atoi(SSkillPoints);
					}

					if (SSkillPoints)
					{
						Player * TargetP = g_ServerMgr->m_PlayerMgr.GetPlayer(Username);

						if (!TargetP)
						{
							SendVaMessage("Player %s is not online", Username);
							msg_sent = true;
							success = true;
						}
						else
						{
							TargetP->PlayerIndex()->RPGInfo.SetSkillPoints(SkillPoints);
							TargetP->LevelUpForSkills();
							TargetP->UpdateSkills();
							TargetP->SendAuxPlayer();
							TargetP->SendVaMessage("You have gotten %d SkillPoints", SkillPoints);
							SendVaMessage("You have gave %s %d SkillPoints", Username, SkillPoints);
							success = true;
							msg_sent = true;
						}
					}
					else
					{
						SendVaMessage("Syntax: //gmskillpoints <playername> <skillpoints>");
						success = false;
						msg_sent = true;
					}
				}

				if (MatchOptWithParam("gmenableskills", pch, param, msg_sent))
				{
					char *PlayerName = strtok(param, " ");

					if (!PlayerName)
					{
						SendVaMessage("Syntax: //gmenableskills <playername>");
						msg_sent = true;
						success = false;
					}
					else
					{
						Player * TargetP = g_ServerMgr->m_PlayerMgr.GetPlayer(PlayerName);

						if (!TargetP)
						{
							SendVaMessage("Player %s is not online", PlayerName);
							msg_sent = true;
							success = true;
						}
						else
						{
							u32 Availability[4] = {4,0,0,1};
							for (int i=0;i<64;i++)
							{
								if (TargetP->PlayerIndex()->RPGInfo.Skills.Skill[i].GetAvailability()[0] == 3)
								{
									TargetP->PlayerIndex()->RPGInfo.Skills.Skill[i].SetAvailability(Availability);
								}
								TargetP->SendAuxPlayer();
							}
							msg_sent = true;
							success = true;
							TargetP->SendVaMessage("Your skills are now enabled");
							SendVaMessage("Player %s had skills enabled", PlayerName);
						}
					}
				}

				if (MatchOptWithParam("gmplayerlevel", pch, param, msg_sent))
				{
					char *Username = 0, *SLevel = 0;
					int Level = 0;

					Username = strtok(param, " ");
					if (Username)
					{
						SLevel = strtok(NULL, " ");
						if (SLevel)
							Level = atoi(SLevel);
					}

					if (SLevel && Level <= 50)
					{
						Player * TargetP = g_ServerMgr->m_PlayerMgr.GetPlayer(Username);

						if (!TargetP)
						{
							SendVaMessage("Player %s is not online", Username);
							msg_sent = true;
							success = true;
						}
						else if (TargetP->AdminLevel() <= 30)
						{
							SendVaMessage("Unable to use gmplayerlevel on BETA and below at the moment - reason: more low level testing needed.");
							msg_sent = true;
							success = true;
						}
						else
						{
							TargetP->PlayerIndex()->RPGInfo.SetCombatLevel(Level);
							TargetP->PlayerIndex()->RPGInfo.SetTradeLevel(Level);
							TargetP->PlayerIndex()->RPGInfo.SetExploreLevel(Level);
							TargetP->LevelUpForSkills();
							TargetP->UpdateSkills();
							TargetP->SendAuxPlayer();

							TargetP->SendVaMessage("Combat, Explore and Trade LVLs set to %d",Level);
							SendVaMessage("Player %s leveled to %d", Username, Level);
							msg_sent = true;
							success = true;
						}
					}
                    else
                    {
						SendVaMessage("Syntax: //gmplayerlevel <playername> <level 1-50>");
						msg_sent = true;
						success = false;
					}
				}
			}

		}
	}

	// This is for normal commands
	if ((Msg[0] == '/') &&
		(Msg[1] != 0) && (!msg_sent || !success))
	{
		char *param;
		char *pch = (char*)_alloca(strlen(&Msg[1]) + 1);//copy to stack to avoid heap fragment
		strcpy(pch, &Msg[1]);
		int retval = 0;
		switch(*pch)
		{
        case 'a':
            {
				if (strcmp(pch, "authlevel") == 0)
				{
                    SendVaMessage("Authentication Level - Num: %d", AdminLevel());
					msg_sent = true;
					success = true;
				}
                if (AdminLevel() >= 80 && MatchOptWithParam("altweapon", pch, param, msg_sent)) //no error checking - server devs only
                {
                    char *cmd = strtok(param, " ");
                    int weapon_id, bone_id;
                    if (cmd)
                    {
                        weapon_id = atoi(cmd);
                        cmd = strtok(NULL, " ");
                    }

                    if (cmd)
                    {
                        bone_id = atoi(cmd);
                    }

                    //ChangeMountBoneName(weapon_id, bone_id);
					msg_sent = true;
					success = true;
                }
                if (AdminLevel() >= 80 && MatchOptWithParam("altname", pch, param, msg_sent)) //no error checking - server devs only
                {
                    char *cmd = strtok(param, " ");
                    int weapon_id = 0;
                    if (cmd)
                    {
                        weapon_id = atoi(cmd);
                        cmd = strtok(NULL, " ");
                    }

                    if (cmd)
                    {
                        ChangeMountBoneName(weapon_id, cmd);
                        SendVaMessage("Change weapon id #%d to %s", weapon_id, cmd);
                    }


					msg_sent = true;
					success = true;
                }
            }
            break;

		case 'b':
			{
				if (MatchOptWithParam("basset", pch, param, msg_sent))
				{
					success = HandleBassetRequest(param);
					msg_sent = true;
				}
				//BUFF for future reference
				if (MatchOptWithParam("buff", pch, param, msg_sent))
				{
					Buff TestBuff;
					memset(&TestBuff, 0, sizeof(Buff));

					strcpy(TestBuff.BuffType, param);
					TestBuff.ExpireTime = GetNet7TickCount() + 100000;
					TestBuff.IsPermanent = false;

					m_Buffs.AddBuff(&TestBuff);
					SendVaMessage("Sending buff 0!\n");
					msg_sent = true;
				}
				else if (AdminLevel() == 90 && MatchOptWithParam("baseitemlist", pch, param, msg_sent)) //one use only, remove once table created
				{
					success = HandleBaseItemListCreate();
					msg_sent = true;
				}
			}
			break;
		case 'c':
			{
                if (MatchOptWithParam("ccamera", pch, param, msg_sent))
				{
                    Object *obj = m_ObjectMgr->GetObjectFromID(ShipIndex()->GetTargetGameID());
                    if (obj)
                    {
                        SendVaMessage("Camera control: %x, %x", ntohl(atoi(param)), ntohl(obj->GameID()) );
                        if (atoi(param) != 4) //this would cause a crash
                        {
                            SendCameraControl( ntohl(atoi(param)), ntohl(obj->GameID()) ) ;
                        }
                        success = true;
                        msg_sent = true;
                    }
				}
                else if (MatchOptWithParam("changepassword", pch, param, msg_sent))
				{
                    g_ServerMgr->m_AccountMgr->ChangePassword(AccountUsername(), param);
                    SendVaMessage("Your password has been changed to: `%s`", param);
                    msg_sent = true;
                }
                else if (MatchOptWithParam("createitem", pch, param, msg_sent))
                {
                    if (atoi(param) == 3052 && strcmp(AccountUsername(), "Tienbau")) //temp lockout for testing
                    {
                        SendVaMessage("Unable to create.");
                    }
                    else if (AdminLevel() >= 50)      // GM to Admin (Beta testers tend to spam this with random numbers)
                    {
                        int FreeSlot = GetCargoSlotFromItemID(0, -1);;

                        if (FreeSlot == -1)
                        {
                            SendVaMessage("No free slots in inventory free up space");
                        }
                        else
                        {
                            _Item myItem = g_ItemBaseMgr->EmptyItem;

							char *ItemIDs = strtok(param, " ");
							char *NumberS;
							char *QualityS;
							int Number;
							float Quality = 1;

							if (ItemIDs)
                            {
								NumberS = strtok(NULL, " ");
								if (NumberS)
                                {
									Number = atoi(NumberS);

									QualityS = strtok(NULL, " ");
									if (QualityS)
									{
										Quality = (float)atof(QualityS);
									}
                                }
								else
                                {
									Number = 1;
                                }

								if (Quality > 2.0f) Quality = 2.0f;

								myItem.ItemTemplateID = atoi(ItemIDs);
								myItem.StackCount = Number;
								myItem.Price = 200;
								myItem.Quality = Quality;
								myItem.Structure = 1;
								strcpy(myItem.BuilderName, this->Name());
								strcat(myItem.BuilderName, " GM/DEV");

								if (CargoAddItem(&myItem) != -2)
								{
									SendAuxShip();
									SendVaMessage("Item %d Created %d", atoi(ItemIDs), Number);
								}
								else
								{
									SendVaMessage("No item exists with id %d", myItem.ItemTemplateID);
								}

							}
                            else
                            {
								Number = 1;
								SendVaMessage("/createitem used incorrectly.");
							}
                        }
                        msg_sent = true;
                    }
                    else
                    {
                        SendVaMessage("GM Only command.");
                    }
                }
				else if (MatchOptWithParam("createcredits", pch, param, msg_sent))
				{
					if (AdminLevel() >= 30)      // Beta to Admin
					{
						PlayerIndex()->SetCredits(PlayerIndex()->GetCredits() + _atoi64(param));
						SaveCreditLevel();
						SendAuxPlayer();
					}
					else
					{
						SendVaMessage("GM Only command.");
					}
					msg_sent = true;
				}
				/*
				else if (MatchOptWithParam("createmission", pch, param, msg_sent))
				{
					if (AdminLevel() >= 50)      // GM to Admin
					{
						g_ServerMgr->m_Missions.givePlayerMission(this, atoi(param));
						msg_sent = true;
					}
					else
					{
						SendVaMessage("GM Only command.");
					}
				}
				*/
				else if (MatchOptWithParam("createmob", pch, param, msg_sent))
				{
                    if (AdminLevel() >= 50)      // GM -> admin
                    {
                        success = HandleMobCreateRequest(param);
                        msg_sent = true;
                    }
                    else
                    {
                        SendVaMessage("GM Only command.");
                    }
				}
                else if (MatchOptWithParam("create", pch, param, msg_sent))
				{
					success = HandleObjCreateRequest(param);
					msg_sent = true;
				}
                else if (0 == strcmp("checklock", pch))
                {
                    Object *obj = m_ObjectMgr->GetObjectFromID(ShipIndex()->GetTargetGameID());
                    if (obj)
                    {
                        Player *player = obj->CheckResourceLock();
                        if (player)
                        {
                            SendVaMessage("Currently being mined/looted by %s", player);
                        }
                        else
                        {
                            SendVaMessage("Not being looted.");
                        }
                    }
                    msg_sent = true;
                }
				else if (strcmp(pch, "commit") == 0 && AdminLevel() >= 80)
				{
					success = HandleCommitRequest(ShipIndex()->GetTargetGameID());
					msg_sent = true;
				}
			}
			break;
		case 'd' :
			if (MatchOptWithParam("dialog", pch, param, msg_sent))
			{
				char *a = strtok(param, ",");
				char *b = strtok(NULL, ",");
				if (b)
				{
					Dialog(a,atoi(b));
				}
				msg_sent = true;
			}
			else if (strcmp(pch, "debug") == 0
				&& AdminLevel() >= 80)
			{
				g_Debug = !g_Debug;
				LogMessage("Debug mode is %s\n", g_Debug ? "ON" : "OFF");
				msg_sent = true;
				success = true;
			}
            else if (MatchOptWithParam("deco", pch, param, msg_sent))
            {
                if (param)
                {
                    short basset = atoi(param);
					char msg_buffer[128];
                    Object *obj = m_ObjectMgr->AddNewObject(OT_DECO);
                    obj->SetPosition(Position());
                    obj->SetBasset(basset);
                    obj->SetSignature(30000.0f);
                    obj->SetOrientation(0,0,0,0);
                    obj->SetScale(1.0f);
                    m_CurrentDecoObj = obj;
					AssetData *asset = g_ServerMgr->AssetList()->GetAssetData(basset);

					if (asset && asset->m_Name)
					{
						_snprintf(msg_buffer, 127, "%d:%s", basset, asset->m_Name);
						SendPushMessage(msg_buffer, "MessageLine", 3000, 3);
					}

                    LogMessage("Deco created at:\n %.2f, %.2f %.2f Basset %d\n", obj->PosX(), obj->PosY(), obj->PosZ(), basset);
                }
                else
                {
                    LogMessage("/deco <basset>");
                }
                msg_sent = true;
                success = true;
            }
            else if (strcmp(pch, "dockp") == 0)
            {
                success = true;
                msg_sent = true;
                DebugPlayerDock(true);
                //g_ServerMgr->m_PlayerMgr.SendPlayerWithoutConnection(GameID());
            }
			break;
		case 'e' :
			{
				if (AdminLevel() >= 30)      // Beta to Admin
				{

					if (strcmp(pch, "enableskills") == 0)
					{
						u32 Availability[4] = {4,0,0,1};
						for (int i=0;i<64;i++)
						{
							if (PlayerIndex()->RPGInfo.Skills.Skill[i].GetAvailability()[0] == 3)
							{
								PlayerIndex()->RPGInfo.Skills.Skill[i].SetAvailability(Availability);
							}
							SendAuxPlayer();
						}
						msg_sent = true;
					}

                    if (MatchOptWithParam("effect", pch, param, msg_sent))
                    {
                        char *cmd = strtok(param, " ");
                        int EffectDID = 0, Length, count = 0;
                        if (cmd)
                        {
                            EffectDID = atoi(cmd);
                            cmd = strtok(NULL, " ");
                            count++;
                        }
                        if (cmd)
                        {
                            Length = atoi(cmd);
                        }
                        else
                        {
                            Length = 4000;
                        }

                        if (count == 1)
                        {
                            ObjectEffect WarpEffect;

                            WarpEffect.Bitmask = 0x07;
                            WarpEffect.EffectDescID = EffectDID;
                            WarpEffect.EffectID = m_SectorMgr->GetSectorNextObjID();;
                            WarpEffect.GameID = GameID();
                            WarpEffect.Duration = Length;
                            WarpEffect.TimeStamp = GetNet7TickCount();

							m_Effects.AddEffect(&WarpEffect);

                            //SendObjectEffectRL(&WarpEffect);

                            SendVaMessage("Send Effect %d Disc: %d for %dms", WarpEffect.EffectID, EffectDID, Length);
                        }
                        else
                        {
                            SendVaMessage("/effect <effect_desc_id> <length>");
                        }

                        success = true;
                        msg_sent = true;
                    }
					else if (MatchOptWithParam("effecto", pch, param, msg_sent))
					{
                        char *cmd = strtok(param, " ");
                        int EffectDID = 0, Length, count = 0;
                        float scale = 1.0f, speedup = 0.0f;
                        short bitmask = 0x07;
                        if (cmd)
                        {
                            EffectDID = atoi(cmd);
                            cmd = strtok(NULL, " ");
                            count++;
                        }
                        if (cmd)
                        {
                            Length = atoi(cmd);
                            cmd = strtok(NULL, " ");
                            if (cmd)
                            {
                                scale = (float)atof(cmd);
                                bitmask += 0x40;
                                cmd = strtok(NULL, " ");
                                if (cmd)
                                {
                                    speedup = (float)atof(cmd);
                                    bitmask += 0x100;
                                }
                            }
                        }
                        else
                        {
                            Length = 4000;
                        }

                        if (count == 1)
                        {
                            ObjectToObjectEffect OBTOBE;
                            memset(&OBTOBE, 0, sizeof(ObjectToObjectEffect));

                            OBTOBE.Bitmask = bitmask;
                            OBTOBE.GameID = GameID();
                            OBTOBE.TargetID = ShipIndex()->GetTargetGameID();
                            OBTOBE.EffectDescID = EffectDID;// 0x00BF;
                            OBTOBE.Message = 0;
                            OBTOBE.EffectID = m_SectorMgr->GetSectorNextObjID();
                            OBTOBE.Duration = Length;
                            OBTOBE.TimeStamp = GetNet7TickCount();
                            OBTOBE.Scale = scale;
                            OBTOBE.Speedup = speedup;

                            SendObjectToObjectEffectRL(&OBTOBE);
                            SendVaMessage("Send Effect %d Disc: %d to Object", OBTOBE.EffectID, EffectDID);
                        }
                        else
                        {
                            SendVaMessage("/effecto <effect_desc_id> <length>");
                        }
                        success = true;
                        msg_sent = true;
					}
					else if (MatchOptWithParam("effects", pch, param, msg_sent))
					{
						int EffectID = atoi(strtok(param,  " "));

						m_Effects.RemoveEffect(EffectID);

						SendVaMessage("Stopping Effect %d", EffectID);
						success = true;
						msg_sent = true;
					}
					else if (MatchOptWithParam("exposedecos", pch, param, msg_sent))
					{
						success = true;
						msg_sent = true;

						if (GetObjectManager())
						{
							bool selection = false;
							if (strcmp(param, "on") == 0) selection = true;

							if (selection)
							{
								if (!ObjectIsMoving())
								{
									SendVaMessage("Nearby decos should now be clickable.");
									m_ExposeDecos = true;
									if (m_SectorMgr->GetSectorType() == 0)
									{
										RemoveFromAllSectorRangeLists();
										m_SectorMgr->SectorServerHandoff(this, PlayerIndex()->GetSectorNum());
									}
									else
									{
										SendVaMessage("Unable to auto-wormhole you, please leave sector and return to expose decos.");
									}

									success = true;
									msg_sent = true;
								}
								else
								{
									SendVaMessage("You should be stationary when using /exposedecos.");
								}
							}
							else
							{
								m_ExposeDecos = false;
								SendVaMessage("Decos no longer exposed - gate out of system to return to normal");
							}
						}
					}
				}
			}
			break;
		case 'f':
			if (AdminLevel() >= 30)      // Beta to Admin
			{
				if (MatchOptWithParam("form", pch, param, msg_sent) && AdminLevel() >= 50)
				{
					SendVaMessage("Forming up target!");
					SendFormationPositionalUpdate(this->GameID(), ShipIndex()->GetTargetGameID(), 100, 100, 100);
					success = true;
					msg_sent = true;
				}
				if (strcmp(pch, "flushinv") == 0)
				{
					// Flush invtory clean!
					for(u32 Slot=0;Slot<ShipIndex()->Inventory.GetCargoSpace();Slot++)
					{
						ShipIndex()->Inventory.CargoInv.Item[Slot].Empty();
					}

					SendAuxShip();
					SendVaMessage("Your inventory is now flushed!");

					success = true;
					msg_sent = true;
				}
			}

			if (AdminLevel() >= 50)      // GM to Admin
			{
				if (strcmp(pch,"fetch") == 0) //no args
				{
					success = HandleFetchRequest();
					msg_sent = true;
				}

                if (MatchOptWithParam("find", pch, param, msg_sent))
			    {
                    Player * target = g_ServerMgr->m_PlayerMgr.GetPlayer(param);
                    if (target)
                    {
                        SendVaMessage("Player found! Name: `%s` GameID: %x Account: `%s`",target->Name(), target->GameID(),target->AccountUsername());
                    }
                    else
                    {
                        SendVaMessage("Player `%s` not found!", param);
                    }
                    msg_sent =  true;
			    }
			}

			if (strcmp(pch, "face") == 0)
			{
				success = HandleFaceRequest(ShipIndex()->GetTargetGameID());
				msg_sent = true;
			}
			else if (strcmp(pch, "faceme") == 0)
			{
				success = HandleFaceMeRequest(ShipIndex()->GetTargetGameID());
				msg_sent = true;
			}
			else if (strcmp(pch, "fgps") == 0)
			{
				ProcessConfirmedActionOffer();
				success = true;
				msg_sent = true;
			}

				/*TODO:
				SendVaMessage("/faddore <ore itemID from database> - adds ore choice to field");
				SendVaMessage("/fremoveore <ore itemID from database> - removes ore choice from field");*/

			if (AdminLevel() >= 80)
			{
				if (MatchOptWithParam("fradius", pch, param, msg_sent))
				{
					success = HandleChangeFieldRequest(param, 1);
					msg_sent = true;
				}
				else if (MatchOptWithParam("ftype", pch, param, msg_sent))
				{
					success = HandleChangeFieldRequest(param, 2);
					msg_sent = true;
				}
				else if (MatchOptWithParam("flevel", pch, param, msg_sent))
				{
					success = HandleChangeFieldRequest(param, 3);
					msg_sent = true;
				}
				else if (MatchOptWithParam("fcount", pch, param, msg_sent))
				{
					success = HandleChangeFieldRequest(param, 4);
					msg_sent = true;
				}
				else if (MatchOptWithParam("faddasteroidtpe", pch, param, msg_sent))
				{
					success = HandleChangeFieldRequest(param, 5);
					msg_sent = true;
				}
				else if (MatchOptWithParam("faddore", pch, param, msg_sent))
				{
					success = HandleChangeFieldRequest(param, 6);
					msg_sent = true;
				}
			}
			break;

		case 'g':
			if (MatchOptWithParam("gform", pch, param, msg_sent) && AdminLevel() >= 50)
			{
				SendVaMessage("Group Formation Set, Please accept!");
				this->PlayerIndex()->GroupInfo.SetFormationName(param);
				this->PlayerIndex()->GroupInfo.SetFormation(1);
				this->PlayerIndex()->GroupInfo.SetPosition(-1);
				this->SendAuxPlayer();
				success = true;
				msg_sent = true;
			}

			if (strcmp(pch, "goto") == 0)
			{
				success = HandleGotoRequest();
				msg_sent = true;
			}
			else if (strcmp(pch, "groupc") == 0)
			{
				SendVaMessage("Group Count = %d", g_ServerMgr->m_PlayerMgr.GetMemberCount(GroupID()));
				msg_sent = true;
			}
			else if (strcmp(pch, "groupid") == 0)
			{
				SendVaMessage("Group ID = %d", GroupID());
				msg_sent = true;
			}
			else if (strcmp(pch, "gameid") == 0)
			{
				SendVaMessage("Game ID = %x", GameID());
				msg_sent = true;
			}
			break;

        case 'h':
            if (strcmp(pch, "hijack") == 0 && ShipIndex()->GetTargetGameID() > 0)
            {
                if (AdminLevel() >= 50)      // GM to Admin
                {
                    success = HandleObjectHijack();
                    msg_sent = true;
                }
                else
                {
                    SendVaMessage("You do not have hijack permission.\n" );
                }
            }
            if (strcmp(pch, "heading") == 0)
            {
                float *heading = Heading();
                SendVaMessage("Heading: %.5f %.5f %.5f", heading[0], heading[1], heading[2]);
                msg_sent = true;
                success = true;
            }
            if (MatchOptWithParam("ht", pch, param, msg_sent))
            {
                int head = atoi(strtok(param,  " "));
                int body = atoi(strtok(NULL, " "));
                int gender = atoi(strtok(NULL, " "));
                SendVaMessage("Current Head: %d Body: %d G: %d", Database()->avatar.head_type, Database()->avatar.body_type,
                    Database()->avatar.gender);

                Database()->avatar.head_type = head;
                Database()->avatar.body_type = body;
                Database()->avatar.gender = gender;
                success = true;
                msg_sent =  true;
            }
			else if (strcmp(pch, "helpedit") == 0)
			{
				SendVaMessage("/hijack - take control of object.");
				SendVaMessage("/release - release control of object.");
				SendVaMessage("/exposedecos on - make all decos in sector clickable.");
				SendVaMessage("/exposedecos off - decos unclickable; back to normal.");
				SendVaMessage("/faceme - selected object faces player.");
				SendVaMessage("/panup <value> - pans the selected object up or down by the value (use negative for down).");
				SendVaMessage("/panx <value> - pans selected object in x axis (east/west on map).");
				SendVaMessage("/pany <value> - pans selected object in y axis (north/south on map).");
				SendVaMessage("/panz <value> - pans selected object in z axis (up/down) - same as panup.");
				SendVaMessage("/rotatex <value> - rotates selected object in x axis (roll).");
				SendVaMessage("/rotatey <value> - rotates selected object in y axis (pitch)");
				SendVaMessage("/rotatez <value> - rotates selected object in z axis (yaw)");
				SendVaMessage("/levelout - makes the selected object level - useful after positioning by /hijack.");
				SendVaMessage("/scale <value> - change scale of object to floating point <value> eg 1.2");
				SendVaMessage("/planetspin <value 1...1000> - gives a planet type a rotational spin. 1000 is fast, 1 is slow");
				SendVaMessage("/tilt <value 0...90> - gives a planet type a tilt, in degrees");
				SendVaMessage("/commit (DEV only) - commits changes to selected object to database.");
				SendVaMessage("//rsectors - undo all non-committed changes.");
                success = true;
                msg_sent =  true;
			}
			else if (strncmp(pch, "helpfield", 9) == 0)
			{
				SendVaMessage("/fradius xxxx - adjust field radius");
				SendVaMessage("/ftype <0-5> - change the field spread type");
				SendVaMessage("/flevel <1-8>");
				SendVaMessage("/fcount xxxx - adjust field asteroid count");
				SendVaMessage("/faddasteroidtype <roid basset> - add additional asteroid type");
				SendVaMessage("/faddore <ore itemID from database> - adds ore choice to field");
				SendVaMessage("/fremoveore <ore itemID from database> - removes ore choice from field");
                success = true;
                msg_sent =  true;
			}
            break;

		case 'i':
			if (MatchOptWithParam("invite", pch, param, msg_sent))
			{
                long game_id = g_ServerMgr->m_PlayerMgr.GetGameIDFromName(param);
                if (game_id == -1)
                {
                    SendVaMessage("Could not find player %s",param);
                    return;
                }

				if (game_id == GameID())
				{
					if (!(AdminLevel() >= 50))      // GM to Admin
					{
					    SendVaMessage("Cannot group with yourself!");
					    return;
					}
				}
				g_ServerMgr->m_PlayerMgr.GroupInvite(GroupID(),GameID(), game_id);
				msg_sent = true;
			}
			break;

		case 'k':
			if (AdminLevel() >= 50)      // GM to Admin
			{
				if (MatchOptWithParam("kick", pch, param, msg_sent))
				{
					success = HandleKick(param);
					msg_sent = true;
				}
			}

		case 'l':
			if (strcmp(pch, "leavegroup") == 0)
			{
				g_ServerMgr->m_PlayerMgr.LeaveGroup(GroupID(),GameID());
				msg_sent = true;
			}
			else if (strcmp(pch, "levelout") == 0)
			{
				success = HandleLevelOutRequest(ShipIndex()->GetTargetGameID());
				msg_sent = true;
			}
			else if (MatchOptWithParam("level", pch, param, msg_sent))
            {
                if (AdminLevel() >= 50)      // GM to Admin
                {
                    if (atoi(param) < 0 || atoi(param) > 50)
                    {
                        SendVaMessage("0 <= Level <= 50");
                        return;
                    }

                    PlayerIndex()->RPGInfo.SetCombatLevel(atoi(param));
                    PlayerIndex()->RPGInfo.SetTradeLevel(atoi(param));
                    PlayerIndex()->RPGInfo.SetExploreLevel(atoi(param));
					PlayerIndex()->RPGInfo.SetSkillPoints(atoi(param) * 10);
                    LevelUpForSkills();
                    UpdateSkills();
                    SendAuxPlayer();

                    SendVaMessage("Combat, Explore and Trade LVLs set to %d",atoi(param));
                    SendVaMessage("Additionally, you now have %d skillpoints",atoi(param) * 10);
                    msg_sent = true;
                }
                else
                {
					SendVaMessage("/level not available at [BETA] and below");
                    msg_sent = true;
                }
            }
            break;

		case 'm':
			if (MatchOptWithParam("move", pch, param, msg_sent))
			{
				success = HandleMoveRequest(param);
				msg_sent = true;
			}
			else if (MatchOptWithParam("mobaggro", pch, param, msg_sent) && AdminLevel() >= 30)
			{
				success = HandleAggroSetting(param);
				msg_sent = true;
			}

			break;

		case 'o':
            if (strcmp(pch, "ori") == 0)
            {
                float *ori = Orientation();
                SendVaMessage("Orientation: %.5f %.5f %.5f %.5f", ori[0], ori[1], ori[2], ori[3]);
                msg_sent = true;
                success = true;
            }

			if (MatchOptWithParam("orientation", pch, param, msg_sent))
			{
				success = HandleOrientationRequest(param);
				msg_sent = true;
			}
			else if (MatchOptWithParam("oeuler", pch, param, msg_sent))
			{
				success = HandleEulerOrientationRequest(param);
				msg_sent = true;
			}
			else if (MatchOptWithParam("openif", pch, param, msg_sent))
			{
				char *a = strtok(param, ",");
				char *b = strtok(NULL, ",");
				if (b)
				{
					OpenInterface(atoi(a), atoi(b));
					SendVaMessage("OpenInterface (%d,%d):", atoi(a), atoi(b));
				}
				msg_sent = true;
			}
			/*else if (MatchOptWithParam("ore", pch, param, msg_sent))
			{
				char *ch0 = strtok(param, ",");
				int base_energy = 0;
				char *ch = strtok(NULL, ",");
				char *ch2 = strtok(NULL, ",");
				float base_time = 0.0f;
				float base_speed = 0.0f;

				if (ch0 != 0)
				{
					base_energy = atoi(ch0);
				}

				if (ch != 0)
				{
					base_time = (float)atof(ch);
				}

				if (ch2 != 0)
				{
					base_speed = (float)atof(ch2);
				}

				if (base_energy < 1 || base_time == 0 || base_speed == 0)
				{
					SendVaMessage("/ore base ore energy,base ore time,base tractor speed");
					SendVaMessage("Base Ore Energy: %d", m_BaseOreEnergy);
					SendVaMessage("Base Ore Time: %.2f", m_BaseOreTime);
					SendVaMessage("Tractor Speed base: %.2f", m_BaseOreTracSpeed);
				}
				else
				{
					SendVaMessage("New base ore energy: %d", base_energy);
					SendVaMessage("New base ore time: %.2f", base_time);
					SendVaMessage("New tractor speed base: %.2f", base_speed);

					m_BaseOreEnergy = base_energy;
					m_BaseOreTime = base_time;
					m_BaseOreTracSpeed = base_speed;
				}
				msg_sent = true;
				success = true;
			}*/
			break;

		case 'p':
			if (strcmp(pch, "position") == 0)
			{
				SendVaMessage("ObjectID = 0x%08x",ShipIndex()->GetTargetGameID());
				if (ShipIndex()->GetTargetGameID() != -1)
                {
                    Object *obj = m_ObjectMgr->GetObjectFromID(ShipIndex()->GetTargetGameID());
                    if (obj) SendVaMessage("%s @ %.2f %.2f %.2f", obj->Name(), obj->PosX(), obj->PosY(), obj->PosZ());
                }
				msg_sent = true;
				success = true;
			}

			if (MatchOptWithParam("panup", pch, param, msg_sent))
			{
				success = HandlePanRequest(param, 3);
				msg_sent = true;
			}
			else if (MatchOptWithParam("panx", pch, param, msg_sent))
			{
				success = HandlePanRequest(param, 1);
				msg_sent = true;
			}
			else if (MatchOptWithParam("pany", pch, param, msg_sent))
			{
				success = HandlePanRequest(param, 2);
				msg_sent = true;
			}
			else if (MatchOptWithParam("panz", pch, param, msg_sent))
			{
				success = HandlePanRequest(param, 3);
				msg_sent = true;
			}
			else if (MatchOptWithParam("planetspin", pch, param, msg_sent))
			{
				success = HandleSpinRequest(param);
				msg_sent = true;
			}
			break;

		case 'r':
			if (strcmp(pch, "reffect") == 0)
			{
				//m_SectorMgr->m_EffectManager.RemoveEffectsByPlayer(GameID());
				SendVaMessage("Removed all effects on you!");
				msg_sent = true;
				success = true;
			}
			if (strcmp(pch, "rs") == 0)
			{
				success = HandleRenderStateRequest();
				msg_sent = true;
			}
            else if (strcmp(pch, "release") == 0)
            {
                //release hijack
                HandleReleaseHijack();
                msg_sent = true;
                success = true;
            }
			else if (MatchOptWithParam("rsi", pch, param, msg_sent))
			{
				success = HandleRenderStateInitRequest(param);
			}
			else if (MatchOptWithParam("rsa", pch, param, msg_sent))
			{
				success = HandleRenderStateActivateRequest(param);
			}
			else if (MatchOptWithParam("rsn", pch, param, msg_sent))
			{
				success = HandleRenderStateActivateNextRequest(param);
			}
			else if (strcmp(pch, "rsd") == 0)
			{
				success = HandleRenderStateDeactivate();
				msg_sent = true;
			}
			else if (strcmp(pch, "range") == 0)
			{
				success = HandleRangeRequest();
				msg_sent = true;
			}
            else if (strcmp(pch, "restoreinv") == 0)
            {
                // Flush invtory clean!
                for(u32 Slot=0;Slot<ShipIndex()->Inventory.GetCargoSpace();Slot++)
                {
                    ShipIndex()->Inventory.CargoInv.Item[Slot].Empty();
                }

                SendVaMessage("Cargo slots = %d", ShipIndex()->Inventory.GetCargoSpace());

                if (ShipIndex()->Inventory.GetCargoSpace() < 20)
                {
                     ShipIndex()->Inventory.SetCargoSpace(20);
                     for(u32 i=0;i<ShipIndex()->Inventory.GetCargoSpace();i++)
                     {
                         ShipIndex()->Inventory.CargoInv.Item[i].Empty();
                     }
                     SendVaMessage("Restored cargo slots = %d", ShipIndex()->Inventory.GetCargoSpace());
                }

                SendAuxShip();

                success = true;
                msg_sent = true;
            }
			else if (MatchOptWithParam("rotatex", pch, param, msg_sent))
			{
				success = HandleRotateRequest(param, 1);
				msg_sent = true;
			}
			else if (MatchOptWithParam("rotatey", pch, param, msg_sent))
			{
				success = HandleRotateRequest(param, 2);
				msg_sent = true;
			}
			else if (MatchOptWithParam("rotatez", pch, param, msg_sent))
			{
				success = HandleRotateRequest(param, 3);
				msg_sent = true;
			}
			/*
			else if (strcmp(pch, "refreshmissionlist") == 0)
			{
				if (AdminLevel() >= 50)      // GM to Admin
				{
					LogMessage("Refreshing mission list\n");
					g_ServerMgr->m_Missions.Initialize();
					success = true;
				}
			}
			*/
            else if (strcmp(pch, "resetmounts") == 0)
            {
				ResetWeaponMounts();
                success = true;
                msg_sent = true;
            }
			else if (strcmp(pch, "resetnavs") == 0)
			{
				if (AdminLevel() >= 50)      // GM to Admin
                {
					memset(m_NavsExplored, 0, sizeof(m_NavsExplored));
					memset(m_NavsExposed, 0, sizeof(m_NavsExposed));
					memset(m_FoundAllSectorNavs, 0, sizeof(m_FoundAllSectorNavs));
                    SendVaMessage("All navs have been marked unexplored and undiscovered");
					if (PlayerIndex()->GetSectorNum() <= 9999)
					{
						char thissector[5];
						_snprintf(thissector, 5, "%d\0", PlayerIndex()->GetSectorNum());
						HandleWormholeRequest(thissector);
					}

                    msg_sent = true;
                }
                else
                {
					SendVaMessage("/resetnavs not available at [BETA] and below");
                    msg_sent = true;
                }
			}
			break;

		case 's' :
			if (MatchOptWithParam("sounds", pch, param, msg_sent))
			{
				unsigned char Data[256];
				int Index = 0;
				*((int *) &Data[0]) = strlen(param);
				Index += 4;
				strcpy((char *) &Data[Index], param);
				Index += strlen(param);
				*((int *) &Data[Index]) = 1;
				Index += 4;
				*((char *) &Data[Index]) = 0;
				Index += 1;
				SendOpcode(ENB_OPCODE_006A_CLIENT_SOUND, Data, Index);

				success = true;
				msg_sent = true;
			}

			if (MatchOptWithParam("scale", pch, param, msg_sent))
			{
				success = HandleScaleRequest(param);
				msg_sent = true;
			}

			if (MatchOptWithParam("skillpoints", pch, param, msg_sent))
			{
                if (AdminLevel() >= 50)      // GM to Admin
                {
					PlayerIndex()->RPGInfo.SetSkillPoints(atoi(param));
					LevelUpForSkills();
					UpdateSkills();
					SendAuxPlayer();
                    SendVaMessage("You now have %d skillpoints", atoi(param));
					success = true;
					msg_sent = true;
				}
			}

            if (MatchOptWithParam("scan", pch, param, msg_sent))
            {
                if (AdminLevel() >= 30)      // Beta to Admin
                {
                    long scan_range = atoi(param);
                    long max_scan = AdminLevel() >= 80 ? 400000 : 80000;

                    if (scan_range >= 1000 && scan_range <= max_scan)
                    {
                        //ShipIndex()->CurrentStats.SetScanRange(scan_range);
						m_Stats.ResetStat(STAT_SCAN_RANGE);
						m_Stats.SetStat(STAT_BASE_VALUE, STAT_SCAN_RANGE, (float)scan_range);
						ShipIndex()->CurrentStats.SetScanRange(scan_range);
                        SendVaMessage("Scan range set to %d", scan_range);
                        success = true;
                        msg_sent = true;
                    }
                    else
                    {
                        SendVaMessage("Set Scan Range: /scan <1000..%d>", max_scan);
                    }
                }
            }

			if (AdminLevel() >= 80)      // Dev to Admin
			{
				if (strcmp(pch, "shutdown") == 0)
				{
                    LogMessage(">>>> SHUTDOWN issued by %s [%s]\n", Name(), AccountUsername());
		            g_ServerMgr->m_PlayerMgr.GlobalAdminMessage("Server shutdown in 30 seconds!");
					GetSectorManager()->AddTimedCall(0, B_SERVER_SHUTDOWN, 15000, NULL);
					success = true;
					msg_sent = true;
				}
				else if (strcmp(pch, "sendp") == 0)
				{
					success = true;
					msg_sent = true;
					g_ServerMgr->m_PlayerMgr.SendPlayerWithoutConnection(GameID());
				}
                else if (strcmp(pch, "strings") == 0)
                {
                    char buffer[256];
                    g_StringMgr->Statistics(buffer);
                    SendVaMessage(buffer);
					success = true;
					msg_sent = true;
                }
				else if (strcmp(pch, "stats") == 0)
				{
					Object *obj = m_ObjectMgr->GetObjectFromID(ShipIndex()->GetTargetGameID());
					if (obj && obj->ObjectType() == OT_FIELD)
					{
						Field *f = (Field*)obj;
						SendVaMessage("Stats for field: %s ID[%d]", f->Name(), f->GetDatabaseUID());
						SendVaMessage("Level: %d", f->Level());
						SendVaMessage("Radius: %.1f", f->FieldRadius());
						SendVaMessage("Spread Type: %d", f->GetFieldType());
						SendVaMessage("Roid Count: %d", f->FieldCount());
						success = true;
						msg_sent = true;
					}
				}
				else if (MatchOptWithParam("shieldbuff", pch, param, msg_sent))
				{
					if (param)
					{
						float BuffAmount = (float)atoi(param);

						this->SetDamageAbsorb(BuffAmount);
					}
					else
					{
						SendVaMessage("Absorb Left: %f", this->GetDamageAbsorb());
					}
					success = true;
					msg_sent = true;
				}
			}
			break;

        // /test command (do not remove)
		case 't':
			if (strcmp(pch, "test") == 0)
			//if (MatchOptWithParam("test", pch, param, msg_sent))
            {
                SendVaMessage("Test Successful!");
                msg_sent =  true;
			}

			if (_strcmpi(pch, "talktree") == 0)
			{
				char string[] =
					    "That was one heck of an explotion! Are you alright over there?\0"
						"\0\3"
						"\0\0\0\0"
						"I need a tow\0"
						"\1\0\0\0"
						"Toggle distress beacon\0"
						"\2\0\0\0"
						"I'm OK\0";

				SendOpcode(ENB_OPCODE_0054_TALK_TREE, (unsigned char *) string, sizeof(string));
			}

			if (MatchOptWithParam("testmsg", pch, param, msg_sent))
			{
                long time_delay = atoi(param);
                m_SectorMgr->AddTimedCall(this, B_TEST_MESSAGE, time_delay, 0, time_delay);

                msg_sent = true;
                success = true;
            }

			if (MatchOptWithParam("tilt", pch, param, msg_sent))
			{
				success = HandleTiltRequest(param);
				msg_sent = true;
			}

            if (MatchOptWithParam("terminate", pch, param, msg_sent))
            {
                Player *p = g_ServerMgr->m_PlayerMgr.GetPlayer(param);

                if (!p)
                {
                    p = g_ServerMgr->m_PlayerMgr.GetPlayer(atoi(param) + 0x40000000);
                }

                //send terminate to net7 proxy
                if (p && AdminLevel() >= 80)
                {
                    long player_id = p->GameID();
                    SendVaMessage("Terminate player %s [%d]", p->Name(), p->GameID());
                    p->SendOpcode(ENB_OPCODE_100A_MVAS_TERMINATE_S_C, (unsigned char *) &player_id, sizeof(long));
                }
            }

			if (MatchOptWithParam("trade", pch, param, msg_sent))
			{
                // Trade requires that both players recieved a create packet for eachother's ships
                Player * targetp = g_ServerMgr->m_PlayerMgr.GetPlayer(param);

                if (!targetp)
                {
                    SendVaMessage("Could not find player %s",param);
                    return;
                }

				if (targetp->GameID() == GameID())
				{
					if (!(AdminLevel() >= 50))      // GM to Admin
					{

					SendVaMessage("Cannot trade with yourself!");
					return;
					}
				}

				if (targetp->m_TradeID != -1)
				{
					SendVaMessage("Target is already trading");
					return;
				}

				LogDebug("Clearing trade for players `%s` and `%s`\n",Name(),targetp->Name());

				for(int x=0;x<6;x++)
				{
					targetp->ShipIndex()->Inventory.TradeInv.Item[x].Empty();
					ShipIndex()->Inventory.TradeInv.Item[x].Empty();
				}

				SendAuxShip(targetp);
				targetp->SendAuxShip(this);

				TradeAction(targetp->GameID(),0);					// Opens a trade window
				targetp->TradeAction(GameID(), 0);	// Open trade window for other player
				m_TradeID = targetp->GameID();						// Set player tradeing with
				m_TradeConfirm = 0;
				targetp->m_TradeID = GameID();
				targetp->m_TradeConfirm = 0;

                msg_sent = true;
			}
			break;

		case 'u' :
			if (MatchOptWithParam("uitrigger", pch, param, msg_sent))
			{
				char *a = strtok(param, ",");
				char *b = strtok(NULL, ",");
				if (b)
                {
					int Index = 0;
					unsigned char Data[75];

					*((long*) &Data[Index]) = atoi(a);
					Index+=4;
					*((long*) &Data[Index]) = atoi(b);
					Index+=4;
					SendOpcode(ENB_OPCODE_0065_UI_TRIGGER, (unsigned char *) &Data, Index);
				}
				msg_sent = true;
			}
            else if (MatchOptWithParam("upgrade", pch, param, msg_sent))
			{
                if (AdminLevel() >= 80) //Dev +
                {
                    long upgrade = atoi(param);
                    ShipUpgrade(upgrade);
                    msg_sent = true;
                    success = true;
                }
            }
            else if (strcmp(pch, "undockp") == 0)
            {
                success = true;
                msg_sent = true;
                DebugPlayerDock(false);
                //g_ServerMgr->m_PlayerMgr.SendPlayerWithoutConnection(GameID());
            }
            break;

		case 'w':
			if (strcmp(pch, "who") == 0)
			{
				g_ServerMgr->m_PlayerMgr.ListPlayersAndLocations(this);
				msg_sent = true;
				success = true;
			}
            else if (MatchOptWithParam("warp", pch, param, msg_sent))
			{
				int limit = 10000;

				if (atoi(param) > limit || atoi(param) < 1000)
				{
					SendVaMessage("Warp limits are between 1000 and %d!", limit);
				}
				else
				{
					SendVaMessage("Setting warp to %d",atoi(param));
					ShipIndex()->CurrentStats.SetWarpSpeed(atoi(param));
					SendAuxShip();
				}
				msg_sent = true;
			}

			if (MatchOptWithParam("wormhole", pch, param, msg_sent))
			{
				if (AdminLevel() >= 50)      // GM to Admin
				{
					success = HandleWormholeRequest(param);
				}
				else
				{
					SendVaMessage("/wormhole GM and above only");
					success = false;
				}
				msg_sent = true;
			}

            if (strcmp(pch, "warpreset") == 0)
            {
                SetWarp();
                TerminateWarp();
                msg_sent = true;
                success = true;
            }

			break;
		}

		if (!success && !msg_sent)
		{
			SendVaMessage("Illegal slash command: %s", pch);
		}
	}
}

void Player::HandleLogoffRequest(unsigned char *data)
{
	//LogoffRequest * request = (LogoffRequest *) data;
	LogMessage("Received LogoffRequest for player %s\n",Name());

	//remove player from the group
	g_ServerMgr->m_PlayerMgr.LeaveGroup(GroupID(),GameID());

	SendLogoffConfirmation();

	//disconnect player from TCP if still connected
	if (m_LoginConnection)
	{
		m_LoginConnection->ShutdownTCPLink(GameID());
	}

    g_ServerMgr->m_PlayerMgr.DropPlayerFromGalaxy(this);
}

void Player::HandleCTARequest(unsigned char *data)
{
	CTARequest * myCTARequest = (CTARequest *) data;

	LogMessage("CTA Request:\n");
	DumpBuffer(data, sizeof(CTARequest));

	g_ServerMgr->m_PlayerMgr.GroupAction(myCTARequest->SourceID, myCTARequest->TargetID, myCTARequest->Action);

	//capture_3		packet# 21495
	unsigned char CTAResponse[] =
	{
		0x00, 0x00, 0x00, 0x00,		//GameID
		0x0F, 0x00, 0x00, 0x00,		//Unknown
		0x01						//Char
	};

	*((long*) &CTAResponse[0]) = myCTARequest->SourceID;

	SendOpcode(ENB_OPCODE_00BD_CTA_RESPONSE, (unsigned char *) &CTAResponse, sizeof(CTAResponse));
}

void Player::SendLogoffConfirmation()
{
	LogMessage("Sending LogoffConfirmation packet\n");
	SendOpcode(ENB_OPCODE_00BA_LOGOFF_CONFIRMATION, 0, 0);
    SendPacketCache();
}

void Player::SendTalkTreeAction(long action)
{
	//LogMessage("Sending TalkTreeAction packet\n");
	SendOpcode(ENB_OPCODE_0056_TALK_TREE_ACTION, (unsigned char *) &action, sizeof(long));
}

bool Player::HandleRangeRequest()
{
	bool success = false;

    if (ShipIndex()->GetTargetGameID() > 0)
    {
        Object *obj = m_ObjectMgr->GetObjectFromID(ShipIndex()->GetTargetGameID());
        if (obj)
        {
            SendVaMessage("Range to object = %f", obj->RangeFrom(Position(), true)); //report absolute range
            success = true;
        }
    }
	else
	{
		SendVaMessage("Unable to access selected object - could not find object ID");
	}

	return (success);
}

bool Player::HandleWormholeRequest(char *sector)
{
	bool success = false;
    bool illegal_dest = false;

    if (PlayerIndex()->GetSectorNum() > 9999)
    {
        SendVaMessage("Unable to wormhole out of starbase, disembark first.");
    }
    else if (m_SectorMgr)
	{
        TerminateWarp();
		long sector_id = atoi(sector);

		if (sector_id > 9999)
		{
			sector_id /= 10;
		}

		if (m_SectorMgr->GetSectorType() != 0 && AdminLevel() != 90)
		{
			illegal_dest = true;
		}

		char *current_sector_name = g_ServerMgr->GetSectorName(PlayerIndex()->GetSectorNum());
		char *sector_name = g_ServerMgr->GetSectorName(sector_id);
		if (sector_name && !illegal_dest)
		{
			SendVaMessage("Wormhole from %s to %s\n", current_sector_name, sector_name);
			SendVaMessage("Wormhole out of sector %d", PlayerIndex()->GetSectorNum());

			RemoveFromAllSectorRangeLists();

			//Hand off server
			m_SectorMgr->SectorServerHandoff(this, sector_id);

			//set to new server;
			PlayerIndex()->SetSectorNum(sector_id);
			success = true;
		}
		else
		{
			SendVaMessage("Invalid or Illegal Sector ID %d", sector_id);
		}
	}

    RemoveProspectNodes();

	return (success);
}

bool Player::HandleKick(char *param)
{
	char Message[1024];

	char *name = strtok(param, " ");
	char *reason = 0;

	if (name)
		reason = strtok(NULL, "");
	else
		return 0;

	Player * target = g_ServerMgr->m_PlayerMgr.GetPlayer(name);

	if (!reason)
    {
		reason = new char[50];
		sprintf(reason, "No Reason Given");
	}

	if (target)
	{
		_snprintf(Message, 1024, "Player '%s' kicked by '%s': %s", target->Name(), Name(), reason);
		LogMessage(" ** Kick: %s\n", Message);

		target->SendVaMessage("You have been kicked by %s: %s", Name(), reason);
		g_ServerMgr->m_PlayerMgr.GMMessage(Message);
		Sleep(100);
		target->ForceLogout();
		return 1;
	}

	return 0;
}

//TODO: spawn a thread to do this, otherwise the GM is waiting for ages for the kick to complete
void Player::ForceLogout()
{
    int Count = 0;

    // Wait 30 seconds for the player to become active, while in space
	/*
    while (!Active() && PlayerIndex()->GetSectorNum() < 10000 && Count < 30)
    {
        Sleep(1000);
        Count++;
    }
	*/

    // Give them 5 seconds to read the kick message!
    Sleep(5000);

	long GameIDD = GameID();

	SendOpcode(ENB_OPCODE_0003_LOGOFF, (unsigned char*)&GameIDD, sizeof(GameIDD));
    SendPacketCache();
	Sleep(100);

	g_ServerMgr->m_PlayerMgr.LeaveGroup(GroupID(),GameID());
    g_ServerMgr->m_PlayerMgr.DropPlayerFromGalaxy(this);
    /*m_SectorMgr->RemovePlayerFromSectorList(this);
	g_ServerMgr->m_PlayerMgr.UnallocatePlayer(this);*/
}

bool Player::HandleBaseItemListCreate()
{
	char QueryString[256];
	//ok we run a query on the item database, find all the ores and populate the base_ore_list table
	sprintf(QueryString, "SELECT * FROM `item_base` WHERE `category` = '81'");
	sql_connection_c connection( "net7", g_MySQL_Host, g_MySQL_User, g_MySQL_Pass);
	sql_result_c result;
	sql_result_c *ItemList_result = &result;
	sql_query_c ItemList  (&connection);
	sql_query_c ItemUpdate(&connection);
	sql_row_c ItemList_Data;

	// Execute Query
	ItemList.execute( QueryString );
	ItemList.store(ItemList_result);

	if (!ItemList_result || !ItemList_result->n_rows())
		return false;

	for(unsigned int item=0;item < ItemList_result->n_rows(); item++)
	{
		//create this entry in the base_ore_list table
		sql_query BaseItemBuilder;
		ItemList_result->fetch_row(&ItemList_Data);
		long item_id = ItemList_Data["id"];
		const char *name = ItemList_Data["name"];

		// Loop though each sector
		long sector_id = 0;
		while ((sector_id = g_ServerMgr->m_SectorContent.GetNextSectorID(sector_id)))
		{
			if (sector_id < 9999)
			{
				BaseItemBuilder.Clear();
				BaseItemBuilder.SetTable("base_ore_list");
				BaseItemBuilder.AddData("item_id", item_id);
				BaseItemBuilder.AddData("name", name);
				BaseItemBuilder.AddData("sector_id", sector_id);

				ItemUpdate.run_query(BaseItemBuilder.CreateQuery());
			}
		}
	}

	SendVaMessage("Base item list completed.");

	return true;
}

bool Player::HandleAggroSetting(char *param)
{
	bool success = false;
	long aggro_level = atoi(param);
	char queryString[256];

	Object *obj = m_ObjectMgr->GetObjectFromID(ShipIndex()->GetTargetGameID());
	MOB *mob = (0);
	MOBData *mob_data = (0);

	if (obj && obj->ObjectType() == OT_MOB)
	{
		mob = (MOB*)obj;
		mob_data = g_ServerMgr->MOBList()->GetMOBData(mob->GetMOBType());
	}

	if (mob && mob_data && (aggro_level >= 0 && aggro_level <=10) && (aggro_level != mob_data->m_Agressiveness))
	{
		success = true;
		long scan_range = ShipIndex()->CurrentStats.GetScanRange();
		ShipIndex()->CurrentStats.SetScanRange(10); //set ship almost blind
		CheckObjectRanges();
		mob_data->m_Agressiveness = (u8)aggro_level;

		SendVaMessage("Setting mob aggro level of '%s' to %d", mob->Name(), aggro_level);

		//now save new aggro level in MOB DB
		SendVaMessage("Now commit setting to database.");
		sql_connection_c connection( "net7", g_MySQL_Host, g_MySQL_User, g_MySQL_Pass);
	    sql_query_c MobUpdate(&connection);

		sprintf(queryString, "UPDATE `mob_base` SET `aggressiveness` = '%ld' WHERE `mob_id` = '%d'", aggro_level, mob->GetMOBType());
	    MobUpdate.run_query(queryString);

		Sleep(200);
		ShipIndex()->CurrentStats.SetScanRange(scan_range);
		ResetRangeLists();
		CheckObjectRanges();
	}

	return (success);
}

bool Player::HandleMoveRequest(char *param)
{
	bool success = false;
	float x, y, z;
	char *a = strtok(param, ",");
	char *b = strtok(NULL, ",");
	char *c = strtok(NULL, ",");

	x = y = z = 0.0f;

	if (a == NULL)
	{
		SendVaMessage("/move: syntax: x,y,z");
		SendVaMessage("/move: all parameters float type");
	}
	else
	{
		x = (float)atof(a);

		if (b != NULL)
		{
			y = (float)atof(b);
		}
		else
		{
			y = PosY();
		}

		if (c != NULL)
		{
			z = (float)atof(c);
		}
		else
		{
			z = PosZ();
		}
	}

	if (!this)
	{
        SetPosition(x, y, z);

		SendVaMessage("Setting position to : %d %d %d", x, y, z);

		//g_ServerMgr->m_PlayerMgr.SendLocation(this, this);
		//CheckTargetUpdate();
        UpdateVerbs();
		CheckObjectRanges();
		success = true;
	}

	return (success);
}


bool Player::HandleOrientationRequest(char *orientation)
{
	bool success = false;
	float o1, o2, o3, o4;
	char *a = strtok(orientation, ",");
	char *b = strtok(NULL, ",");
	char *c = strtok(NULL, ",");
	char *d = strtok(NULL, ",");

	o1 = o2 = o3 = o4 = 0.0f;

	if (a == NULL)//allow incomplete args
	{
		SendVaMessage("/orientation: syntax: o1,o2,o3,o4");
		SendVaMessage("/orientation: all parameters float type");
	}
	else
	{
		o1 = (float)atof(a);

		if (b != NULL)
		{
			o2 = (float)atof(b);
		}
		if (c != NULL)
		{
			o3 = (float)atof(c);
		}
		if (d != NULL)
		{
			o4 = (float)atof(d);
		}


        Object *obj;

		SendVaMessage("Targetting 0x%08x Player = 0x%08x",ShipIndex()->GetTargetGameID(), GameID());

        obj = m_ObjectMgr->GetObjectFromID(ShipIndex()->GetTargetGameID());

        if (obj)
        {
            //New orientation info:
            obj->SetOrientation(o1, o2, o3, o4);

            SendVaMessage("Object name: %s", obj->Name());

            obj->SendPosition(this);

			success = true;
		}
		else
		{
			SendVaMessage("Unable to access selected object - could not find object ID");
		}
	}

	return (success);
}

bool Player::HandleFaceRequest(long Target)
{
	bool success = false;

	Object *obj = m_ObjectMgr->GetObjectFromID(Target);

	if (obj)
	{
        Face(obj);
        SendLocationAndSpeed(true);
		success = true;
	}
	else
	{
		SendVaMessage("Unable to access selected object - could not find object ID");
	}

	return (success);
}

bool Player::HandleFaceMeRequest(long Target)
{
	bool success = false;

	Object *obj = m_ObjectMgr->GetObjectFromID(Target);

	if (obj)
	{
		obj->CalcOrientation(Position(), obj->Position(), true);
		//leave object pointing in correct direction
		obj->SendLocationAndSpeed(true);
		success = true;
	}
	else
	{
		SendVaMessage("Unable to access selected object - could not find object ID");
	}

	return (success);
}

bool Player::HandleChangeFieldRequest(char *param, int option)
{
	bool success = false;
	bool change = false;
	bool update_done = false;
	char queryString[256];

	Object *obj = m_ObjectMgr->GetObjectFromID(ShipIndex()->GetTargetGameID());
	int value = atoi(param);

	if (obj && value != 0 && obj->ObjectType() == OT_FIELD)
	{
		sql_connection_c connection( "net7", g_MySQL_Host, g_MySQL_User, g_MySQL_Pass);
		sql_query_c FieldUpdate(&connection);
		Field *f = (Field*)obj;

		switch (option)
		{
		case 1: //radius
			if (value > 1999)
			{
				f->SetFieldRadius((float)value);
				//commit to DB & re-pop
				sprintf(queryString, "UPDATE `sector_objects_harvestable` SET `max_field_radius` = '%d' WHERE `resource_id` = '%ld'", value, f->GetDatabaseUID());
				change = true;
			}
			else
			{
				SendVaMessage("field radius must be 2000 or greater");
			}
			break;

		case 2: //spread type
			f->SetFieldType(value);
			//commit to DB & re-pop
			sprintf(queryString, "UPDATE `sector_objects_harvestable` SET `field` = '%d' WHERE `resource_id` = '%ld'", value, f->GetDatabaseUID());
			change = true;
			break;

		case 3: //level
			if (value > 0 && value < 9)
			{
				f->SetLevel(value);
				//commit to DB & re-pop
				sprintf(queryString, "UPDATE `sector_objects_harvestable` SET `level` = '%d' WHERE `resource_id` = '%ld'", value, f->GetDatabaseUID());
				change = true;
			}
			else
			{
				SendVaMessage("level must be between 1 and 8");
			}
			break;

		case 4: //count
			if (value != f->FieldCount())
			{
				SendVaMessage("New asteroid field count: cannot display new changes until server re-start");
				//commit to DB & re-pop
				sprintf(queryString, "UPDATE `sector_objects_harvestable` SET `res_count` = '%d' WHERE `resource_id` = '%ld'", value, f->GetDatabaseUID());
				change = true;
			}
			break;

		case 5: //add type
			//first check basset is valid
			break;

		case 6: //add item_id choice to this field
			//1. check item is valid
			if (value > 0 && value < 10000)
			{
				f->AddItemID(value, 0.0f);
				//need to make a new entry
				sql_result_c result;
				sprintf(queryString, "SELECT * FROM sector_objects_harvestable_oretypes WHERE resource_id = '%ld' AND additional_ore_item_id = '%d'", f->GetDatabaseUID(), value);
				FieldUpdate.execute(queryString);
				FieldUpdate.store(&result);

				if (result.n_rows() == 0)
				{
					sql_query ItemBuilder;
					ItemBuilder.Clear();
					ItemBuilder.SetTable("sector_objects_harvestable_oretypes");

					ItemBuilder.AddData("resource_id", f->GetDatabaseUID());
					ItemBuilder.AddData("additional_ore_item_id", value);
					ItemBuilder.AddData("frequency", 0);

					if (!FieldUpdate.run_query(ItemBuilder.CreateQuery()))
					{
						SendVaMessage("Error while comitting new item to DB.");
					}
				}
				change = true;
				update_done = true;
			}
			else
			{
				SendVaMessage("invalid item id: %d", value);
			}
			break;

		default: //
			break;
		}

		if (change == true)
		{
			if (!update_done) FieldUpdate.run_query(queryString);
			//now blank & re-pop field
			long scan_range = ShipIndex()->CurrentStats.GetScanRange();
			ShipIndex()->CurrentStats.SetScanRange(10); //set ship almost blind
			CheckObjectRanges();

			//now rebuild field
			f->PopulateField(false, true);

			ShipIndex()->CurrentStats.SetScanRange(scan_range);
			ResetRangeLists();
			CheckObjectRanges();
		}

		success = true;
	}
	else
	{
		SendVaMessage("No valid asteroid field selected or invalid value.");
	}

	return (success);
}

bool Player::HandlePanRequest(char *param, int axis)
{
	bool success = false;

	Object *obj = m_ObjectMgr->GetObjectFromID(ShipIndex()->GetTargetGameID());
	float value = (float)atof(param);

	if (obj && value != 0.0f)
	{
		switch (axis)
		{
		case 1:
			obj->MovePosition(value,0,0);
			break;

		case 2:
			obj->MovePosition(0,value,0);
			break;

		case 3:
			obj->MovePosition(0,0,value);
			break;

		default:
			break;
		}

		obj->SendLocationAndSpeed(true);
		success = true;
	}
	else
	{
		SendVaMessage("Unable to move selected object - move amount: %.2f", value);
	}

	return (success);
}

bool Player::HandleRotateRequest(char *param, int axis)
{
	bool success = false;

	Object *obj = m_ObjectMgr->GetObjectFromID(ShipIndex()->GetTargetGameID());
	float value = (float)atof(param);

	if (obj && value != 0.0f)
	{
		value = value * PI / 180.0f;

		float *ori = obj->Orientation();

		if (ori[0] == 0 && ori[1] == 0 && ori[2] == 0 && ori[3] == 0)
		{
			obj->SetOrientation(0.0f,0.0f,0.0f,1.0f);
			obj->SetHeading();
			obj->LevelOut();
			obj->LevelOrientation();
		}
		else
		{
			obj->SetHeading();
		}

		switch (axis)
		{
		case 1:
			obj->Rotate(value,0,0);
			break;

		case 2:
			obj->Rotate(0,value,0);
			break;

		case 3:
			obj->Rotate(0,0,value);
			break;

		default:
			break;
		}

		obj->SendLocationAndSpeed(true);
		success = true;

		if (obj->ObjectType() == OT_FIELD)
		{
			Field *f = (Field*)obj;
			//issue field update
			long scan_range = ShipIndex()->CurrentStats.GetScanRange();
			ShipIndex()->CurrentStats.SetScanRange(10); //set ship almost blind
			CheckObjectRanges();

			//now rebuild field
			f->PopulateField(false, true);

			ShipIndex()->CurrentStats.SetScanRange(scan_range);
			ResetRangeLists();
			CheckObjectRanges();
		}
	}
	else
	{
		SendVaMessage("Unable to move selected object - move amount: %.2f", value);
	}

	return (success);
}

bool Player::HandleLevelOutRequest(long Target)
{
	bool success = false;

	Object *obj = m_ObjectMgr->GetObjectFromID(Target);

	if (obj)
	{
		obj->SetHeading();
		obj->LevelOut();
		obj->LevelOrientation();
		//leave object square on the Z plane
		obj->SendLocationAndSpeed(true);
		success = true;
		SendVaMessage("%s leveled out", obj->Name());
	}
	else
	{
		SendVaMessage("Unable to access selected object - could not find object ID");
	}

	return (success);
}

bool Player::HandleCommitRequest(long Target)
{
	bool success = false;
	char queryString[256];

	Object *obj = m_ObjectMgr->GetObjectFromID(Target);

	if (obj)
	{
		float *ori = obj->Orientation();
		SendVaMessage("Attempting to commit %s to database.", obj->Name());
		sql_connection_c connection( "net7", g_MySQL_Host, g_MySQL_User, g_MySQL_Pass);
	    sql_query_c PositionUpdate(&connection);

		sprintf(queryString, "UPDATE `sector_objects` SET `position_x` = '%.2f' WHERE `sector_object_id` = '%ld'", obj->PosX(), obj->GetDatabaseUID());
	    PositionUpdate.run_query(queryString);
		sprintf(queryString, "UPDATE `sector_objects` SET `position_y` = '%.2f' WHERE `sector_object_id` = '%ld'", obj->PosY(), obj->GetDatabaseUID());
	    PositionUpdate.run_query(queryString);
		sprintf(queryString, "UPDATE `sector_objects` SET `position_z` = '%.2f' WHERE `sector_object_id` = '%ld'", obj->PosZ(), obj->GetDatabaseUID());
	    PositionUpdate.run_query(queryString);
		sprintf(queryString, "UPDATE `sector_objects` SET `orientation_u` = '%.6f' WHERE `sector_object_id` = '%ld'", ori[0], obj->GetDatabaseUID());
	    PositionUpdate.run_query(queryString);
		sprintf(queryString, "UPDATE `sector_objects` SET `orientation_v` = '%.6f' WHERE `sector_object_id` = '%ld'", ori[1], obj->GetDatabaseUID());
	    PositionUpdate.run_query(queryString);
		sprintf(queryString, "UPDATE `sector_objects` SET `orientation_w` = '%.6f' WHERE `sector_object_id` = '%ld'", ori[2], obj->GetDatabaseUID());
	    PositionUpdate.run_query(queryString);
		sprintf(queryString, "UPDATE `sector_objects` SET `orientation_z` = '%.6f' WHERE `sector_object_id` = '%ld'", ori[3], obj->GetDatabaseUID());
	    PositionUpdate.run_query(queryString);
		sprintf(queryString, "UPDATE `sector_objects` SET `scale` = '%.4f' WHERE `sector_object_id` = '%ld'", obj->Scale(), obj->GetDatabaseUID());
	    PositionUpdate.run_query(queryString);

		//store specialist information
		switch (obj->ObjectType())
		{
		case OT_PLANET:
			sprintf(queryString, "UPDATE `sector_objects_planets` SET `rotate_rate` = '%f' WHERE `planet_id` = '%ld'", obj->Spin(), obj->GetDatabaseUID());
			PositionUpdate.run_query(queryString);
			sprintf(queryString, "UPDATE `sector_objects_planets` SET `tilt_angle` = '%f' WHERE `planet_id` = '%ld'", obj->Tilt(), obj->GetDatabaseUID());
			PositionUpdate.run_query(queryString);
			break;
		default:
			break;
		} ;

		SendVaMessage("Object %s committed to Net7 Database.", obj->Name());
		LogMessage("%s just committed changes to object %s %d\n", AccountUsername(), obj->Name(), obj->GetDatabaseUID());
	}
	else
	{
		SendVaMessage("Unable to commit selected object - could not find object ID");
	}

	return (success);
}


bool Player::HandleEulerOrientationRequest(char *orientation)
{
	bool success = false;
	float o1, o2, o3;
	char *a = strtok(orientation, ",");
	char *b = strtok(NULL, ",");
	char *c = strtok(NULL, ",");

	o1 = o2 = o3 = 0.0f;

	if (a == NULL)//allow incomplete args
	{
		SendVaMessage("/oeuler: syntax: heading,alt,bank");
		SendVaMessage("/oeuler: all parameters float type");
	}
	else
	{
		o1 = (float)atof(a);

		if (b != NULL)
		{
			o2 = (float)atof(b);
		}
		if (c != NULL)
		{
			o3 = (float)atof(c);
		}

		if (o1 == 180.0f) o1 = o1 + 0.5f;
		if (o2 == 180.0f) o2 = o2 + 0.5f;
		if (o3 == 180.0f) o3 = o3 + 0.5f;

		o1 = o1 / 180.0f * PI;
		o2 = o2 / 180.0f * PI;
		o3 = o3 / 180.0f * PI;

        Object *obj;

        obj = m_ObjectMgr->GetObjectFromID(ShipIndex()->GetTargetGameID());

        if (!obj)
        {
            obj = m_CurrentDecoObj;
        }

        if (obj)
        {
            //New orientation info:
            obj->SetEulerOrientation(o1,o2,o3);

            SendVaMessage("Object name: %s", obj->Name());
			SendVaMessage("Setting Euler orientation to %.2f,%.2f,%.2f",o1*180.0f/PI,o2*180.0f/PI,o3*180.0f/PI);
            float *vector;
            vector = obj->Orientation();
			SendVaMessage("Setting Quaternion orientation to %.4f,%.4f,%.4f,%.4f",vector[0],vector[1],vector[2],vector[3]);

            obj->SendPosition(this);

			success = true;
		}
		else
		{
			SendVaMessage("Unable to access selected object - could not find object ID");
		}
	}

	return (success);
}

bool Player::HandleMobCreateRequest(char *param)
{
	char *pch_parse, *name = 0;
	short level = 0;
    long mob_type = 0;
    long mob_count = 0;
    bool direct_basset = false;
    bool directional = false;
    float velocity = 100.0f;
    float turn = 0.2f;
    float xoffset = 300.0f;

    //are we trying to create a mob too close to a station?
    Object *obj = NearestNav();

    if (obj)
    {
        if (obj->ObjectType() == OT_STATION && obj->RangeFrom(Position()) < 5000.0f)
        {
            SendVaMessage("You're trying to create a MOB too close to a station. Move to at least 5K away.");
            return false;
        }
    }

#ifdef SQL_ENABLE
    if (_stricmp(Name(), "Abramelin") != 0)
    {
        SendVaMessage("createmob has been suspended temporarily - will be replaced by createspawn very soon\n");
        return true;
    }
#endif

    mob_count = g_ServerMgr->MOBList()->GetMOBCount();

	pch_parse = strtok(param, " ");

	if (!pch_parse)
	{
		return false;
	}

    if (pch_parse && directional == false)
    {
        mob_type = (long)atoi(pch_parse);
        pch_parse = strtok(NULL, " ");
        if (pch_parse)
        {
            level = atoi(pch_parse);
            name = strtok(NULL, " ");
        }
    }

	if (mob_type > -1 && mob_type < mob_count)
	{
        MOB *mob = (MOB*)m_ObjectMgr->AddNewObject(OT_MOB); //MOB creation
        MOBData *mob_data = g_ServerMgr->MOBList()->GetMOBData(mob_type);

        name = mob_data->m_Name;

        if (mob && mob_data)
        {
            SendVaMessage("Creating %s at level %d", name, mob_data->m_Level);
            mob->SetMOBType((short)mob_type);

            mob->SetName(name);
            mob->SetPosition(Position());
            mob->MovePosition(xoffset, 0.0f, -200.0f);
            mob->SetActive(true);
            mob->SetRespawnTick(0);
            mob->SetHostileTo(OT_PLAYER);
            mob->SetVelocity(velocity);
            mob->Turn(turn);
            mob->SetDefaultStats(turn, CURIOUS, velocity, 50);
            mob->SetUpdateRate(50);
            mob->SetBehaviour(CURIOUS);
            mob->AddBehaviourPosition(Position());

            return true;
        }
        else
        {
            SendVaMessage("Unable to create MOB at this time.");
            return false;
        }
	}
	else
	{
		SendVaMessage("Create Mob: /createmob <MOB type>0..%d <level>1..66 <name>", (mob_count - 1));
		return false;
	}

    return true;

}

bool Player::HandleObjCreateRequest(char *param)
{
	char *pch_level;
	short level = 0;
	pch_level = strchr(param, ' ');

	if (strcmp(param, "ON") == 0)
	{
		SendVaMessage("Resource Creation ACTIVATED");
		SendVaMessage("Create Resource: /create <type>0..12 <level>1..9");
		g_ServerMgr->m_AllowCreate = true;
		return true;
	}
	else if (strcmp(param, "XML") == 0)
	{
		SendVaMessage("XML output on");
		g_ServerMgr->m_DumpXML = true;
		return true;
	}
	else if (strncmp(param, "F", 1) == 0)
	{
		char *pch_lev = 0;
		char *pch_number = 0;
		char *pch_radius = 0;
		int type = 0;
		int number = 0;
		int radius = 0;

		if (pch_level) pch_lev = strchr(pch_level + 1, ' ');
		if (pch_lev) pch_number = strchr(pch_lev + 1, ' ');
		if (pch_number) pch_radius = strchr(pch_number + 1, ' ');

		if (pch_lev && pch_level && pch_number && pch_radius)
		{
			type = atoi(pch_level + 1);
			level = atoi(pch_lev + 1);
			number = atoi(pch_number + 1);
			radius = atoi(pch_radius + 1);
		}

		if (radius < 3000.0f || number < 1 || level > 8 || type > 9 || level < 1 || type < 1)
		{
			SendVaMessage("Create Asteroid field: /create F <type>1..9 <level>1..8 <count>1.. <radius>1.. ");
			return false;
		}
		else if (AdminLevel() >= 80)      // GM to Admin
		{
            Object *obj = m_ObjectMgr->AddNewObject(OT_FIELD); //complete creation of field
            obj->SetFieldRadius((float)radius);
            obj->SetLevel(level);
            obj->SetFieldCount(number);
            obj->SetFieldType(type);
            obj->SetPosition(Position());
            obj->PopulateField(true); // issue field
			SendVaMessage("Asteroid field created. Level:%d Count:%d Radius:%d", level, number, radius);
            m_ObjectMgr->DisplayDynamicObjects(this, true); //update display for all roids.
			return true;
		}
		else
		{
			SendVaMessage("%s, you can't create an asteroid field ... yet.", Name());
			return false;
		}
	}
	else if (strncmp(param, "M", 1) == 0)
	{
		Object *obj = m_ObjectMgr->GetObjectFromID(ShipIndex()->GetTargetGameID());
		if (obj)
		{
			//p->m_SectorMgr->GetMobPosition(mob, &position[0]);
			//mob->position_info.Position
			//m_SectorMgr->GetMobPosition(obj, &position[0]);



			/*position[0] = obj->position_info.Position[0];
			position[1] = obj->position_info.Position[1];
			position[2] = obj->position_info.Position[2];*/
			//m_SectorMgr->AddNewObject(this, 9, 7, &position[0]);
		}
	}
	else if (g_ServerMgr->m_AllowCreate == false)
	{
		SendVaMessage("Create Resource: Unable to create resource");
		return false;
	}

	if (!pch_level)
	{
		SendVaMessage("Create Resource: /create <type>0..12 <level>1..9");
		return false;
	}

	long object_type = (long)atoi(param);

	level = atoi(pch_level + 1);

	if ((level > 0 && level < 10) && (object_type > -1 && object_type < 13))
	{
        Object *obj = m_ObjectMgr->AddNewObject(OT_RESOURCE);
        obj->SetTypeAndName((short)object_type + 0x71E);
        obj->SetLevel(level);
        obj->SetPosition(Position());
        obj->ResetResource(); //populate with items

		return true;
	}
	else
	{
		SendVaMessage("Create Resource: /create <type>0..12 <level>1..9");
		return false;
	}
}

void Player::ChangeSectorID(long SectorID)
{
    PlayerIndex()->SetSectorNum(SectorID);
}

bool Player::HandleFetchRequest()
{
    bool success = false;
    Object *obj = m_ObjectMgr->GetObjectFromID(ShipIndex()->GetTargetGameID());

	if (obj != NULL)
    {
		if (obj->ObjectType() != OT_PLAYER && obj->ObjectType() != OT_MOB)
		{
			obj->SetPosition(Position());
			SendVaMessage("Object name: %s", obj->Name());
			obj->SendPosition(this); //TODO: send new position to everyone
		}
		else
		{
			SendVaMessage("Unable to change position of Player or MOB");
		}
	}
	else
	{
		SendVaMessage("Unable to access selected object - could not find object ID");
	}

	return (success);
}

bool Player::HandleObjectHijack()
{
    Object *obj;
	ObjectManager *obj_manager = GetObjectManager();

	if (obj_manager)
	{
		obj = obj_manager->GetObjectFromID(ShipIndex()->GetTargetGameID());
	}

    if (obj && HijackObject(obj))
    {
        //try a different approach with this.
        //lets try just locking the position of the hijacked object onto the player's ship
        //and not showing the ship to anyone else
        SendVaMessage("You have been successfully assigned to control '%s'.", obj->Name());
        LogMessage("HIJACK occurred. Account name '%s'.\n", AccountUsername());
        return true;
    }
    else
    {
        SendVaMessage("Unable to hijack object with GameID %d.\n", ShipIndex()->GetTargetGameID());
        return false;
    }
}

void Player::HandleReleaseHijack()
{
    if (GetObjectManager() && Hijackee() > 0)
    {
		Object *obj = GetObjectManager()->GetObjectFromID(Hijackee());
		if (obj)
		{
			SendVaMessage("You have been unassigned from control of '%s'.", obj->Name());
			obj->SetVelocity(0);
			obj->SetPlayerHijack(0);
		}
		SetHijackee(0);
    }
}

bool Player::HandleGotoRequest()
{
    bool success = false;
    Object *obj;
	bool admin = false;

	//only allow admins to jump to resources
	if (AdminLevel() >= 50)      // GM to Admin
	{
		admin = true;
	}

	if (admin == false)
	{
		SendVaMessage("/goto has been disabled");
		return (false);
	}

    obj = m_ObjectMgr->GetObjectFromID(ShipIndex()->GetTargetGameID());

    if (obj != NULL)
    {
        SetPosition(obj->Position());
        SendLocationAndSpeed(true);
		UpdateVerbs();
		CheckObjectRanges();
        CheckNavs();
		success = true;
	}

	return (success);
}

void Player::OpenStargate(long object_id)
{
    SendActivateNextRenderState(object_id, 1);
}

void Player::CloseStargate(long object_id)
{
    SendActivateNextRenderState(object_id, 3);
}

bool Player::HandleRenderStateRequest()
{
    bool success = false;
    Object *obj = m_ObjectMgr->GetObjectFromID(ShipIndex()->GetTargetGameID());

    if (obj)
    {
        SendVaMessage("Object '%s'", obj->Name());
        SendVaMessage("Current Render State   = 0x%X", obj->RenderState());
        //SendVaMessage("Allowable Render State = 0x%X", obj->allowable_render_states);
        success = true;
    }
    else
    {
        SendVaMessage("Unable to access selected object - could not find object ID");
    }

	return (success);
}

bool Player::HandleScaleRequest(char *param)
{
	bool success = false;
	if (m_SectorMgr && (ShipIndex()->GetTargetGameID() > 0 || m_CurrentDecoObj))
	{
        float scale = (float) atof(param);
        Object *obj = m_ObjectMgr->GetObjectFromID(ShipIndex()->GetTargetGameID());

        if (!obj) obj = m_CurrentDecoObj;

        if (obj)
        {
            if (scale == 0.0)
            {
                // Display current scale
                SendVaMessage("Scale of %s is currently %.2f", obj->Name(), obj->Scale());
            }
            else
            {
                // Set new scale
                SendVaMessage("Changing scale from %g to %g", obj->Scale(), scale);
                obj->SetScale(scale);
                long target_id = obj->GameID();
				if (obj->ObjectType() == OT_RESOURCE)
				{
					// Remove the Object
					obj->Remove();
					SendPacketCache();
					Sleep(200);
					// Recreate the Object
					obj->SendObject(this);
				}
				else
				{
					RemoveObject(target_id);
					SendPacketCache();
					Sleep(200);
					obj->SendObject(this);
				}
            }
        }
    }

	return (success);
}

bool Player::HandleSpinRequest(char *param)
{
	bool success = false;
	if (m_SectorMgr && (ShipIndex()->GetTargetGameID() > 0))
	{
        float spin = (float) atof(param);
        Object *obj = m_ObjectMgr->GetObjectFromID(ShipIndex()->GetTargetGameID());

		if (obj && obj->ObjectType() == OT_PLANET && (spin > 0 && spin < 1001))
		{
			float old_spin = (obj->Spin() - 0.000001f)/0.00001f;
			SendVaMessage("Changing spin from %f to %f", old_spin, spin);
			// Set new spin
			spin = 0.000001f + spin*0.00001f;
			obj->SetSpin(spin);
			long target_id = obj->GameID();

			RemoveObject(target_id);
			SendPacketCache();
			Sleep(200);
			obj->SendObject(this);
		}
		else
		{
			SendVaMessage("/spin only works on 'Planet' type objects, and must be 1 to 1000");
		}
    }

	return (success);
}

bool Player::HandleTiltRequest(char *param)
{
	bool success = false;
	if (m_SectorMgr && (ShipIndex()->GetTargetGameID() > 0))
	{
        float tilt = (float) atof(param);
        Object *obj = m_ObjectMgr->GetObjectFromID(ShipIndex()->GetTargetGameID());

		if (obj && obj->ObjectType() == OT_PLANET && (tilt >= 0 && tilt <= 90))
		{
			// Set new tilt
			SendVaMessage("Changing tilt from %f to %f", (obj->Tilt()*180.0f/PI), tilt);
			tilt = tilt * PI / 180.0f;
			obj->SetTilt(tilt);
			long target_id = obj->GameID();

			RemoveObject(target_id);
			SendPacketCache();
			Sleep(200);
			obj->SendObject(this);
		}
		else
		{
			SendVaMessage("/tilt only works on 'Planet' type objects, and must be 0 to 90");
		}
    }

	return (success);
}

bool Player::HandleBassetRequest(char *param)
{
	bool success = false;
	if (m_CurrentDecoObj)
	{
		char msg_buffer[128];
		long basset = atoi(param);
		Object *obj = m_CurrentDecoObj;
		AssetData *asset = g_ServerMgr->AssetList()->GetAssetData(basset);

		if (asset && asset->m_Name)
		{
			_snprintf(msg_buffer,127, "%ld:%s", basset, asset->m_Name);
			SendPushMessage(msg_buffer, "MessageLine", 3000, 3);
		}

		// Set new basset
		SendVaMessage("Changing basset from %d to %d", obj->BaseAsset(), basset);

		obj->SetBasset((short)basset);
		// Remove the Object
		obj->Remove();
		//ensure remove and re-create aren't in same packet stream.
		SendPacketCache();
		Sleep(200);
		// Recreate the Object
		obj->SendObject(this);
		SendPacketCache();
		Sleep(200);
		obj->Remove();
		//ensure remove and re-create aren't in same packet stream.
		SendPacketCache();
		Sleep(200);
		// Recreate the Object
		obj->SendObject(this);
	}

	return (success);
}

bool Player::SendLoungeNPC(long StationID)
{
#ifdef USE_MYSQL_STATIONS
	struct StationLounge LoungeData;
	unsigned char bufferd[10000];
	unsigned char *buffer = bufferd;
	int Size, x;

	memset(buffer, 0, sizeof(buffer));
	memset(&LoungeData, 0, sizeof(LoungeData));

	StationTemplate * Stn = g_ServerMgr->m_StationMgr.GetStation(StationID);

	if (!Stn)			// Station not found?
    {
        LogMessage("Station %d not on SQL list\n", StationID);
		return false;
    }

	LoungeData.Station.StationType = Stn->Type;

	NPCTemplate * NPCs = NULL;
	x=0;
	for(NPCList::const_iterator npc = Stn->NPCs.begin(); npc < Stn->NPCs.end(); ++npc)
    {
		NPCs = g_ServerMgr->m_StationMgr.GetNPC(*npc);//npc->second;
		LoungeData.NPC[x].BoothType = NPCs->Booth;
		LoungeData.NPC[x].Location = NPCs->Location;
		LoungeData.NPC[x].NPCID = NPCs->StarbaseID;
		LoungeData.NPC[x].RoomNumber = NPCs->Room;
		memcpy(&LoungeData.NPC[x].Avatar,&NPCs->Avatar,sizeof(AvatarData));
		x++;
	}
	LoungeData.NumNPCs = x;

	RoomTemplate * Rooms;
	x=0;
    for(RoomList::const_iterator room = Stn->Rooms.begin(); room < Stn->Rooms.end(); ++room)
    {
		Rooms = g_ServerMgr->m_StationMgr.GetRoom(*room);

		LoungeData.Rooms[x].RoomNumber = Rooms->Index;
		LoungeData.Rooms[x].RoomStyle = Rooms->Style;
		LoungeData.Rooms[x].FogFar = Rooms->FogFar;
		LoungeData.Rooms[x].FogNear = Rooms->FogNear;
		x++;
	}
	LoungeData.Station.RoomNumber = x;

	TermTemplate * Terms;
	x=0;
	for(TermList::const_iterator term = Stn->Terms.begin(); term < Stn->Terms.end(); ++term)
    {
		Terms = g_ServerMgr->m_StationMgr.GetTerminal(*term);
		LoungeData.Terms[x].TermType = Terms->Type;
		LoungeData.Terms[x].Location = Terms->Location;
		LoungeData.Terms[x].RoomNumber = Terms->Room;
		x++;
	}
	LoungeData.NumTerms = x;

	// Build Packet

	memcpy(buffer,&LoungeData.Station,sizeof(LoungeData.Station));
	buffer+=sizeof(LoungeData.Station);

	for(x=0;x<LoungeData.Station.RoomNumber;x++)
	{
		memcpy(buffer,&LoungeData.Rooms[x],sizeof(LoungeData.Rooms[x]));
		buffer+=sizeof(LoungeData.Rooms[x]);
	}

	*((int *) buffer) = LoungeData.NumTerms; buffer+=4;

	for(x=0;x<LoungeData.NumTerms;x++)
	{
		memcpy(buffer,&LoungeData.Terms[x], sizeof(LoungeData.Terms[x]));
		buffer+=sizeof(LoungeData.Terms[x]);
	}

	*((int *) buffer) = LoungeData.NumNPCs; buffer+=4;

	for(x=0;x<LoungeData.NumNPCs;x++)
	{
		memcpy(buffer,&LoungeData.NPC[x], sizeof(LoungeData.NPC[x]));
		buffer+=sizeof(LoungeData.NPC[x]);
	}

	Size = buffer - &bufferd[0];

	SendOpcode(ENB_OPCODE_0052_LOUNGE_NPC, (unsigned char *) bufferd, Size);

	//DumpBuffer(bufferd, Size);

	return true;
#else
	return false;
#endif
}

//Not too sure on the data structure of this packet
bool Player::HandleRenderStateInitRequest(char *param)
{
    bool success = false;
    Object *obj = m_ObjectMgr->GetObjectFromID(ShipIndex()->GetTargetGameID());
    long render_state = atol(param);

    if (obj != NULL)
    {
        SendVaMessage("Object '%s'", obj->Name());
        SendVaMessage("Current Render State   = 0x%X", obj->RenderState());
        SendVaMessage("Allowable Render State = 0x%X", 3);
        SendVaMessage("New Render State       = 0x%X", render_state);
        SendInitRenderState(ShipIndex()->GetTargetGameID(), render_state);
        obj->SetRenderState((short)render_state);
        success = true;
    }
    else
    {
        SendVaMessage("Unable to access selected object - could not find object ID");
    }
    return (success);
}

bool Player::HandleRenderStateActivateRequest(char *param)
{
    bool success = false;
    Object *obj;
    long render_state = atol(param);
    obj = m_ObjectMgr->GetObjectFromID(ShipIndex()->GetTargetGameID());

    if (obj != NULL)
    {
        SendVaMessage("Object '%s'", obj->Name());
        SendVaMessage("Activate Render State: 2 = open gate half way");
        SendVaMessage("Current Render State   = 0x%X", obj->RenderState());
        SendVaMessage("Allowable Render State = 0x%X", 3);
        SendVaMessage("New Render State       = 0x%X", render_state);
        SendActivateRenderState(ShipIndex()->GetTargetGameID(), render_state);
        obj->SetRenderState((short)render_state);
        success = true;
    }
    else
    {
        SendVaMessage("Unable to access selected object - could not find object ID");
    }
    return (success);
}

bool Player::HandleRenderStateActivateNextRequest(char * param)
{
    bool success = false;
    Object *obj;
    long render_state = atol(param);
    obj = m_ObjectMgr->GetObjectFromID(ShipIndex()->GetTargetGameID());

    if (obj != NULL)
    {
        SendVaMessage("Object '%s'", obj->Name());
        SendVaMessage("Activate Next Render State: 1 = Gate opens fully 3 = gate closes");
        SendVaMessage("Current Render State   = 0x%X", obj->RenderState());
        SendVaMessage("Allowable Render State = 0x%X", 3);
        SendVaMessage("New Render State       = 0x%X", render_state);
        SendActivateNextRenderState(ShipIndex()->GetTargetGameID(), render_state);
        obj->SetRenderState((short)render_state);
        success = true;
    }
    else
    {
        SendVaMessage("Unable to access selected object - could not find object ID");
    }
    return (success);
}

bool Player::HandleRenderStateDeactivate()
{
    bool success = false;
    Object *obj;
    obj = m_ObjectMgr->GetObjectFromID(ShipIndex()->GetTargetGameID());

    if (obj != NULL)
    {
        SendVaMessage("Object '%s'", obj->Name());
        SendVaMessage("Deactivate Render State");
        SendVaMessage("Current Render State   = 0x%X", obj->RenderState());
        SendVaMessage("Allowable Render State = 0x%X", 3);
        SendVaMessage("New Render State       = 0x%X", 0);
        SendDeactivateRenderState(ShipIndex()->GetTargetGameID());
        obj->SetRenderState(0); //Not sure if I should be changing this here
        success = true;
    }
    else
    {
        SendVaMessage("Unable to access selected object - could not find object ID");
    }
    return (success);
}

void Player::SendStarbaseAvatarChange(Player *p)
{
    if (!p) return;

    StarbaseAvatarChange_S2C change;
    memset(&change, 0, sizeof(change));
    change.AvatarID = p->GameID();
    change.Orient = p->m_Orient;
    change.Position[0] = p->PosX();
    change.Position[1] = p->PosY();
    change.Position[2] = p->PosZ();
    change.ActionFlag = p->ActionFlag();
    change.Room = p->m_Room;

    SendOpcode(ENB_OPCODE_009E_STARBASE_AVATAR_CHANGE, (unsigned char *) &change, sizeof(change));
}

void Player::HandleStarbaseRequest(unsigned char *data)
{
	StarbaseRequest * pkt = (StarbaseRequest *) data;

    LogDebug("Starbase Request - PlayerID: %d Action: %d StarBaseID: %d\n",pkt->PlayerID, pkt->Action, pkt->StarbaseID);

	NPCTemplate * NPC = NULL;

	const char *professions[] =
    {
        " Warriors",
        "  Traders",
        "Explorers"
    };

	char string[] =
		"/happy1 Hello! Hello - step right up, Sir. Explorers are welcome here.  How can I help you?\0"
		"\0\2"
		"\0\0\0\0"
		"I would like to trade\0"
		"\1\0\0\0"
		"Nothing today\0";

    memcpy((string+43),professions[Profession()], 9);

	/*printf("Received StarbaseRequest packet, PlayerID=%x, StarbaseID=%d, Action=%d\n",
	    pkt->PlayerID, pkt->StarbaseID, pkt->Action);*/

	m_TradeWindow = false;

	switch(pkt->Action)
	{
	case 1: // Exiting the station action
		if (m_SectorMgr)
		{
			if (m_TradeID != -1)
            {
				CancelTrade();
            }
            LogMessage("Player '%s' Leaving station.\n", Name());
			// Launch into space!
			m_SectorMgr->LaunchIntoSpace(this);
		}
		break;
	case 4: // Talk to NPC
		m_StarbaseTargetID = pkt->StarbaseID;
			// Display the standard talk tree unless we have a mission here
        if (CheckMissions(0, 1, (short)m_StarbaseTargetID, TALK_NPC) ||
            CheckForNewMissions(0, 1, (short)m_StarbaseTargetID) )
        {
            return;
        }

        NPC = g_ServerMgr->m_StationMgr.GetNPC(m_StarbaseTargetID);

		// Save our current NPC to make it easier to find
		m_CurrentNPC = NPC;

#ifdef USE_MYSQL_STATIONS
		// Get talk tree for NPC
		// and save to m_CurrentTalkTree
		if (NPC && NPC->NPCInteraction.talk_tree.NumNodes > 0)
		{
			long length = GenerateTalkTree(&NPC->NPCInteraction.talk_tree, 1);

			if (length == 0)
			{
				return;
			}

			// Output first node
			SendOpcode(ENB_OPCODE_0054_TALK_TREE, (unsigned char *) m_TalkTreeBuffer, length);
		}
		else
#endif
		{
			SendOpcode(ENB_OPCODE_0054_TALK_TREE, (unsigned char *) string, sizeof(string));
			m_CurrentTalkTree = (0);
		}

        m_StarbaseTargetID = pkt->StarbaseID;

        break;
	case 6: // Activating Job Terminal
		{
            /*
			PacketBuffer buffer;

			g_ServerMgr->m_Jobs.GetJobList(this, &buffer);
			SendOpcode(ENB_OPCODE_0093_JOB_LIST, buffer.getBuffer(), buffer.size());
            */

			break;
		}
	case 7: // Clicking on a job on the job terminal to pull up a job description
		{	/*  JobDescription structure

				0x00, 0x00, 0x00, 0x00, // JobDescriptionID
				0x00,					// Is this job still available? (0x00 = no, 0x01 = yes)
				0xXX, ........... 0x00, // Variable length null terminated "Title" string
				0xXX, ........... 0x00, // Variable length null terminated "Description" string
			*/


            /*
			PacketBuffer buffer;

			g_ServerMgr->m_Jobs.GetJobDescription(pkt->StarbaseID, &buffer, &g_ServerMgr->m_TokenParser);
			SendOpcode(ENB_OPCODE_0094_JOB_DESCRIPTION, buffer.getBuffer(), buffer.size());

			_Item myItem;
			memset(&myItem,0,sizeof(_Item));

			int itemId = g_ServerMgr->m_Jobs.GetItem(pkt->StarbaseID);

			ItemBase * item = g_ItemBaseMgr->GetItem(itemId);

            if (item)
            {
			    myItem.ItemTemplateID = itemId;
			    myItem.Price = item->Cost();
			    myItem.Quality = 1.0f;
			    myItem.Structure = 1.0f;
			    myItem.StackCount = 1;

			    printf("itemID: %d\n", itemId);


		   	    ItemBase *myItemBase = g_ItemBaseMgr->GetItem(myItem.ItemTemplateID);
                SendItemBase(myItem.ItemTemplateID);

			    PlayerIndex()->RewardInv.Item[0].SetData(&myItem);
			    SendAuxPlayer();
            }
            */

		break;
		}

	case 8: // Accept job?
			LogMessage("Accepting Job\n");
			SendOpcode(ENB_OPCODE_0096_JOB_ACCEPT_REPLY);
			// Eventually we'll be able to accept jobs once the system is fixed.
			//g_ServerMgr->m_Missions.givePlayerMission(this, 1);
		break;
	case 9: // ??
		break;
	case 10: // Customize avatar - NOT WORKING YET
		SendOpcode(ENB_OPCODE_0083_RECUSTOMIZE_AVATAR_START);
		break;
	case 11: // Customize starship - NOT WORKING YET
		SendOpcode(ENB_OPCODE_0081_RECUSTOMIZE_SHIP_START);
	}
}


void Player::SetManufactureID(long mfg_id)
{
	//LogMessage("Sending SetManufactureID packet\n");
	SendOpcode(ENB_OPCODE_007F_MANUFACTURE_SET_MANUFACTURE_ID, (unsigned char *) &mfg_id, sizeof(mfg_id));
}

/*
 1C 00  "Friendship 7 Recreation Port"
 00 00  ""
 05 00  "Glenn"
 0A 00  "Beta Hydri"

 20 00  "Glenn Sector (Beta Hydri System)       from sector
 0A 00  "Beta Hydri"                            from system
 0E 00  "Swooping Eagle"                        to sector
 06 00  "Sirius"                                to system

*/

//TODO: Recode using new packet methods
void Player::SendServerHandoff(long from_sector_id, long to_sector_id, const char *from_sector, const char *from_system, const char *to_sector, const char *to_system)
{
    // Check for invalid destinations
	if (to_sector == 0 || to_sector_id == 0 || to_system == 0)
	{
		LogMessage("FATAL ERROR! Invalid server handoff destination! Returning to source\n");
		to_sector = from_sector;
		to_sector_id = from_sector_id;
		to_system = from_system;
	}

	ServerHandoff server_handoff;
	memset(&server_handoff, 0, sizeof(server_handoff));

	server_handoff.join = m_MasterJoin;
	server_handoff.join.ToSectorID = ntohl(to_sector_id);
	server_handoff.join.FromSectorID = ntohl(from_sector_id);

	// Populate "FROM SECTOR" string
	char *p = (char *) &server_handoff.variable_data;
	*((short *) p) = strlen(from_sector);
	p += 2;
	strncpy(p, from_sector, strlen(from_sector));
	p += strlen(from_sector);

	// Populate "FROM SYSTEM" string
	*((short *) p) = strlen(from_system);
	p += 2;
	strncpy(p, from_system, strlen(from_system));
	p += strlen(from_system);

	// Populate "TO SECTOR" string
	*((short *) p) = strlen(to_sector);
	p += 2;
	strncpy(p, to_sector, strlen(to_sector));
	p += strlen(to_sector);

	// Populate "TO SYSTEM" string
	*((short *) p) = strlen(to_system);
	p += 2;
	strncpy(p, to_system, strlen(to_system));
	p += strlen(to_system);

	size_t length = (p - (char *) &server_handoff);

	LogMessage("Sending ServerHandoff, ToSectorID = %d\n", to_sector_id);

	SendOpcode(ENB_OPCODE_003A_SERVER_HANDOFF, (unsigned char *) &server_handoff, length);

    SendPacketCache();

	ChangeSectorID(to_sector_id);
}

void Player::HandleTriggerEmote(unsigned char *data)
{
	TriggerEmote * emote = (TriggerEmote *) data;

	SendNotifyEmote(emote->GameID, emote->Emote);
}

//TODO: Recode using new packet methods
void Player::HandleChatStream(unsigned char *data)
{
	ChatStream * chat_stream = (ChatStream *) data;
	unsigned char *buffer;
	unsigned char *p;

	if (chat_stream->message[0] == 0x02)	// Emote
	{
		//LogMessage("Received AvatarTriggerEmote packet -- GameID=%d\n", chat_stream->GameID);

		buffer = new unsigned char[chat_stream->ChatSize + 7];		// 7 = (long GameID, short ChatSize, char type)
		memset(buffer, 0, chat_stream->ChatSize + 7);

		*((short *) &buffer[0]) = chat_stream->ChatSize;
		buffer[2] = 0x01;
		*((long *) &buffer[3]) = chat_stream->GameID;
		p = (unsigned char *)chat_stream;
		p+=7;
		memcpy(&buffer[7],p,chat_stream->ChatSize);

		//SendOpcode(ENB_OPCODE_005F_AVATAR_EMOTE_RESPONSE, buffer, chat_stream->ChatSize + 7);
        SendToSector(ENB_OPCODE_005F_AVATAR_EMOTE_RESPONSE, buffer,  chat_stream->ChatSize + 7);
		delete[] buffer;
	}
	else if (chat_stream->message[0] == 0x01)	// Chat in Stations
	{
		//Commented this section out so local messages aren't sent twice (Once as local, the other as broadcast)
		//short chatStringLength = *((short *)&chat_stream->message[2]);
		//char broadcast[2048];
		//sprintf(broadcast, "[Local] %s: %s", Name(), &chat_stream->message[4]);
		//SendMessageString(broadcast, 7);
		LogDebug("ChatStream code: %d\n", chat_stream->message[0]);
	}
	else
	{
		LogMessage("ChatStream code: %d\n", chat_stream->message[0]);
		LogMessage("Received Unknown ChatStream code\n");
	}
}

void Player::SendClientChatEvent(int Unknown, int Unknown2, const char * FName, const char * LName, const char * Channel, const char * Message)
{
	unsigned char Packet[1024];
	int Index = 0;

	AddData(Packet, Unknown, Index);		// Not sure what this is usualy its 0x03
	AddData(Packet, Unknown2, Index);		// Not sure what this is usualy its 0x00
	AddDataLS(Packet, FName, Index);		// First Name of Player
	AddDataLS(Packet, LName, Index);		// Last Name of Player
	AddData(Packet, (short) 0, Index);		// ? End Marker I think ?
	AddDataLS(Packet, Channel, Index);		// Channel it came from
	AddDataLS(Packet, Message, Index);		// Message
	AddData(Packet, 0, Index);				// ? End Marker I think ?
	AddData(Packet, (short) 0, Index);		// ? End Marker I think ?


	if (0/*log*/) //TODO: add chat logging global
	{
		// Log all chat messages to the chat log.
		LogChatMsg("%s->[%s]> \"%s\" (%s)\n", FName, Channel, Message, Name());
	}

	SendOpcode(ENB_OPCODE_00A5_CLIENT_CHAT_EVENT, (unsigned char *) Packet, Index);	// Send Packet
}

void Player::SendNotifyEmote(long game_id, long emote)
{
	NotifyEmote response;
	response.GameID = game_id;
	response.Emote = emote;

    SendToRangeList(ENB_OPCODE_00A2_NOTIFY_EMOTE, (unsigned char *) &response, sizeof(response));
}

void Player::HandleOption(unsigned char *data)
{
	OptionPacket * myOption = (OptionPacket *) data;

	LogMessage("Received Option packet -- GameID=%d  OptionType=%d  OptionVar=%d\n",
		myOption->GameID, myOption->OptionType, myOption->OptionVar);
	DumpBuffer(data,sizeof(OptionPacket));
}

// This is going to need re-coding in the same way as the Action Packet method
void Player::HandleSelectTalkTree(unsigned char *data)
{
	SelectTalkTree * packet = (SelectTalkTree *) data;

	/*LogMessage("Received SelectTalkTree packet -- PlayerID=%x  Selection=%d\n",
		(packet->PlayerID & 0x00FFFFFF), packet->Selection);*/

	if (m_ActionResponseReceived == true
		&& packet->Selection == 0)
	{
		SendTalkTreeAction(-32);
		SendTalkTreeAction(-32);
		m_ActionResponseReceived = false;
		return;
	}

	if (m_TradeWindow == true)
	{
		if (packet->Selection == 0)
		{
			SendTalkTreeAction(-32);
            m_TradeWindow = false;
			return;
		}
        m_TradeWindow = false;
	}

	if (m_BeaconRequest)
	{
		switch(packet->Selection)
		{
			case 0:    // Tow to Base
				SendTalkTreeAction(-32);
				SetDistressBeacon(false);
				TowToBase();
				m_BeaconRequest = false;
				break;
			case 1:	   //  Distress Beacon
				SendTalkTreeAction(-32);
				SetDistressBeacon(true);
				m_BeaconRequest = false;
				break;
			case 2:    // I'm OK
				SendTalkTreeAction(-32);
				SetDistressBeacon(false);
				m_BeaconRequest = false;
				break;
			case 230:  // Close
				SendTalkTreeAction(-32);
				m_BeaconRequest = false;
				break;
		}

		if (!m_BeaconRequest)
			return;
	}

	if (packet->Selection != 230 && CheckMissions(0, packet->Selection, (short)m_StarbaseTargetID, TALK_NPC))
    {
        m_TradeWindow = true;
        if (packet->Selection == 0)
        {
            SendTalkTreeAction(-32); //close display
        }
        return;
    }

	if (CheckTalkTree(packet->Selection))		// See if we need a talktree from the database
		return;



    // Trade with vendor
	if (packet->Selection == 0)
    {
		char string[] = "/happy2 Thanks for coming by my booth.  May good fortune be with you.\0\0\0";

#ifdef USE_MYSQL_STATIONS
		// Send Item List to Player
		NPCTradeItems();
#else

		// 78 Items
		long ItemTemp[128];
		long ItemPrice[128];
		int TotalItems = 0;

		int ItemType = 14; //Default to weapons if type is not set!!
        int ItemType2 = 0, ItemType3 = 0, ItemType4 = 0;

		//TODO: Remove hardcoding
		//just make this a bit more fun until we do.

		switch (m_StarbaseTargetID)
		{
			//Weapons
		case 1:
		case 145:
			ItemType = 14;  // Beams
            ItemType2 = 15; // Missiles
			ItemType3 = 16; // Projectiles
			ItemType4 = 10;	// Ammo
			break;
			// Shields
		case 46:
			ItemType = 2;
			break;
			//junk/components (I think)
		case 81:
		case 160:
		case 124:
		case 74:
			ItemType = 13;
            ItemType2 = 10; //also sell missile ammo
			break;
			// Reactors
		case 80:
			ItemType = 7;
			break;

			//Engines
		case 66:
			ItemType = 6;
			break;

			//If we haven't got a specific ID then choose one based on ID modulus
		default:
			{
				switch (m_StarbaseTargetID%5)
				{
				case 0:
					ItemType = 14;  // Beams
                    ItemType2 = 15; // Missiles
					ItemType3 = 16; // Projectiles
					ItemType4 = 10;	// Ammo
					break;
				case 1:
					ItemType = 2;
					break;
				case 2:
					ItemType = 13;
                    ItemType2 = 16;
					break;
				case 3:
					ItemType = 7;
					break;
				case 4:
					ItemType = 6;
					break;
				}
			}
		}


		ItemBase * item = 0;
		int x = 0;

		while (TotalItems < 50 && x < MAX_ITEMBASE_ITEMS)
		{
            if (item = g_ItemBaseMgr->GetItem(x))
            {
			    if (item->ItemType() == ItemType || (ItemType2 != 0 && item->ItemType() == ItemType2)
					|| (ItemType3 != 0 && item->ItemType() == ItemType3) || (ItemType4 != 0 && item->ItemType() == ItemType4))
			    {
                    SendItemBase(item->ItemTemplateID());
				    ItemTemp[TotalItems] = item->ItemTemplateID();
				    ItemPrice[TotalItems] = item->Cost() * 2;
				    TotalItems++;
			    }
            }

			x++;
		}

		for(x=0; x<TotalItems; x++)
		{
			PlayerIndex()->VendorInv.Item[x].SetItemTemplateID(ItemTemp[x]);
			PlayerIndex()->VendorInv.Item[x].SetStackCount(1);
			PlayerIndex()->VendorInv.Item[x].SetStructure(1.0f);
			PlayerIndex()->VendorInv.Item[x].SetQuality(1.0f);
			PlayerIndex()->VendorInv.Item[x].SetPrice(ItemPrice[x]);
		}
		for (x=TotalItems; x<128; x++)
		{
			PlayerIndex()->VendorInv.Item[x].Clear();
		}

        SetPrices();
		SendAuxPlayer();
        SendAuxShip();
#endif

		SendTalkTreeAction(5);
		SendTalkTreeAction(3);
		SendOpcode(ENB_OPCODE_0054_TALK_TREE, (unsigned char *) string, sizeof(string));
		SendTalkTreeAction(6);

		m_TradeWindow = true;
	}

	if (packet->Selection == 1 || packet->Selection == 230 )
	{
		SendTalkTreeAction(-32);

        ClearPrices();
		SendAuxPlayer();
        SendAuxShip();
	}
}

bool Player::CheckTalkTree(int Selection)
{
	// If we have a talk tree open use this
	if (!m_CurrentNPC) return false;

	// if Selection is 0 exit!
	if (Selection == 0)
	{
		SendTalkTreeAction(-32); //close display
		return true;
	}

	long length = GenerateTalkTree(&m_CurrentNPC->NPCInteraction.talk_tree, Selection);
	u16 flags = GetNodeFlags(&m_CurrentNPC->NPCInteraction.talk_tree, Selection);

	if (length == 0) return false;

	if (flags == NODE_TRADE)
	{
		// Send Items to Trade
		NPCTradeItems();
		// Send Exit Message
		SendTalkTreeAction(5);		// Open Trade Window
		SendTalkTreeAction(3);		// Send the "Trade" button
		SendOpcode(ENB_OPCODE_0054_TALK_TREE, (unsigned char *) m_TalkTreeBuffer, length);
		SendTalkTreeAction(6);		// Not sure?
	}
	else
	{
		// Send out Node
		SendOpcode(ENB_OPCODE_0054_TALK_TREE, (unsigned char *) m_TalkTreeBuffer, length);
	}

	return true;
}

bool Player::NPCTradeItems()
{
	int TotalItems = 0;

	if (m_CurrentNPC == NULL)
		return false;

	ItemBase * item = 0;
	VendorItemList *ItemL;

	TotalItems = 0;
    for(vecItemList::const_iterator v_item = m_CurrentNPC->Vendor.Items.begin(); v_item < m_CurrentNPC->Vendor.Items.end(); ++v_item)
    {
		ItemL = g_ServerMgr->m_StationMgr.GetVendorItem(*v_item);

		if (ItemL && (item = g_ItemBaseMgr->GetItem(ItemL->ItemID)) && ItemL->Quanity != 0 )
		{
			// Send ItemBase for every item if needed
			SendItemBase(item->ItemTemplateID());

			PlayerIndex()->VendorInv.Item[TotalItems].SetItemTemplateID(item->ItemTemplateID());
			PlayerIndex()->VendorInv.Item[TotalItems].SetStackCount(1);
			PlayerIndex()->VendorInv.Item[TotalItems].SetStructure(1.0f);
			PlayerIndex()->VendorInv.Item[TotalItems].SetQuality(1.0f);

			// if we have a set sell price then sell it for that price
			if (ItemL->SellPrice == 0)
			{
				PlayerIndex()->VendorInv.Item[TotalItems].SetPrice(item->Cost());
			}
			else
			{
				PlayerIndex()->VendorInv.Item[TotalItems].SetPrice(ItemL->SellPrice);
			}

			// Make sure we dont go over 128 Items
			TotalItems++;
		}
		if (TotalItems >= 128)
		{
			break;
		}
	}

	// Fill the rest of the inventory with no items
	for (int x=TotalItems; x<128; x++)
	{
		PlayerIndex()->VendorInv.Item[x].Clear();
	}

	SetPrices();
	SendAuxPlayer();
	SendAuxShip();

	m_TradeWindow = true;

	return true;
}

void Player::SendRelationship(long ObjectID, long Reaction, bool IsAttacking)
{
	Relationship response;
	response.ObjectID = ntohl(ObjectID);
	response.Reaction = Reaction;
	response.IsAttacking = IsAttacking ? 1 : 0;

	//LogMessage("Sending Relationship packet\n");
	SendOpcode(ENB_OPCODE_0089_RELATIONSHIP, (unsigned char *) &response, sizeof(response));
}

//TODO: Cache this with Net7Proxy
void Player::HandleGalaxyMapRequest()
{
	//LogMessage("Received GalaxyMap request packet\n");
    SendOpcode(ENB_OPCODE_2011_GALAXY_MAP_CACHE, 0, 0);
	//SendDataFileToClient("GalaxyMap.dat");
}

void Player::SendGalaxyMap(const char *system, const char *sector, const char *station)
{
	struct GalaxyMap
	{
		long    Type;
		long    Size;
		long    PlayerID;
		char    Variable[64];
		long    unknown;
	};

	GalaxyMap galaxy_map;
	long string_length;
	long size = 8;  // include PlayerID and unknown
	char *p = galaxy_map.Variable;

	galaxy_map.Type = 4;
	galaxy_map.PlayerID = GameID();

	// System
	strcpy(p, system);
	string_length = strlen(p) + 1;
	size += string_length;
	p += string_length;

	// Sector
	strcpy(p, sector);
	string_length = strlen(p) + 1;
	size += string_length;
	p += string_length;

	// Station
	strcpy(p, station);
	string_length = strlen(p) + 1;
	size += string_length;
	p += string_length;

	*((long *) p) = 375;    // unknown = 375
	galaxy_map.Size = size;

	//LogMessage("Sending GalaxyMap packet\n");
	SendOpcode(ENB_OPCODE_0097_GALAXY_MAP, (unsigned char * ) &galaxy_map, (u16)(size + 8));
}

void Player::HandleDebug(unsigned char *data)
{
	LogDebug("Received Debug packet\n");
}

void Player::SetStartingPosition()
{
    Object *nearest_nav = (0);
    SetOrientation(0.0f, 0.0f, 1.0f, 0.0f);

    if (PlayerIndex()->GetSectorNum() > 9999) //stations is simplest case
    {
        SetPosition(0.0f, 0.0f, 0.0f);
        return;
    }

	ObjectManager *om = this->GetObjectManager();

	if (m_FromSectorID > 9999)
	{
		// The player just left a station
        // see if we had any entry coords
        if (RestoreDockingCoords())
		{
			//ok let's just check we're within sector boundaries
			SectorManager *sector_manager = GetSectorManager();
			if (sector_manager)
			{
				ServerParameters *params = GetSectorManager()->GetSectorParams();
				float *pos = this->Position();
				if (om && (pos[0] > params->XMax ||
					pos[0] < params->XMin ||
					pos[1] > params->YMax ||
					pos[1] < params->YMin))
				{
					//find nearest nav point, and place player next to it.
					nearest_nav = om->NearestNav(Position());
					SetPosition(nearest_nav->Position());
				}
			}
		}
		else
        {
            Object *station = om->FindStation(0);
            if (station)
            {
                int offset = (int)station->Radius();
                SetPosition(station->Position());
                nearest_nav = NearestNav();
                if (nearest_nav)
                {
                    Face(nearest_nav);
                    SetHeading();
                }

                // now move the ship forward by the station radius
                ExtrapolatePosition(station->Radius(), 1.0f);
            }
        }
    }
    else
    {
        // The player just went through a gate
        Object *gate = om->FindGate(m_FromSectorID);
        if (!gate) gate = om->FindFirstNav();
        if (gate)
        {
            // Place the player's ship near the gate
            SetPosition(gate->Position());
            nearest_nav = NearestNav();
            if (nearest_nav)
            {
                Face(nearest_nav);
                SetHeading();
            }

            // Now move the ship outside of the gate
            ExtrapolatePosition(gate->Radius(), 1.0f);
        }
        else
        {
            SetPosition(0.0f, 0.0f, 0.0f);
        }
    }

	if (m_FromSectorID == 0 || (m_FromSectorID == PlayerIndex()->GetSectorNum()))		// We just logged in!
	{
		bool success = LoadPosition();

		if (!success || (PosX() == 0.0f && PosY() == 0.0f && PosZ() == 0.0f))
		{
			LogDebug("Cannot load postion file setting to nearest gate.\n");

            //find first nav
            Object *obj = om->FindFirstNav();

            if (!obj)
            {
                obj = om->GetObjectFromID(10000);
            }

            if (obj)
            {
                SetPosition(obj->Position());
                nearest_nav = NearestNav();
                if (nearest_nav)
                {
                    Face(nearest_nav);
                    SetHeading();
                }
                ExtrapolatePosition(obj->Radius() + 500.0f, 1.0f);
            }
		}
	}

    UpdateVerbs();
}

void Player::SendMessageString(const char *msg, char color, bool log)
{
	char buffer[512];
	memset(buffer, 0, sizeof(buffer));
	short length = strlen(msg) + 1;
	*((short *) &buffer[0]) = length;
	buffer[2] = color;
	strcpy(&buffer[3], msg);

	if (log)
	{
		// Log all chat messages to the chat log.
		LogChatMsg("%s> \"%s\"\n", Name(), msg);
	}

	SendOpcode(ENB_OPCODE_001D_MESSAGE_STRING, (unsigned char *) buffer, length + 3);
}

void Player::SendPriorityMessageString(const char *msg1, const char *msg2, long time, long priority)
{
	unsigned char buffer[512];
	int index = 0;
	memset(buffer, 0, sizeof(buffer));

	AddDataSN(buffer, msg1, index);
	AddDataSN(buffer, msg2, index);
	AddData(buffer, time, index);
	AddData(buffer, priority, index);

	SendOpcode(ENB_OPCODE_0020_PRIORITY_MESSAGE, &buffer[0], index);
}

void Player::HandleMissionDismissal(unsigned char *data)
{
    MissionDismissal *dismiss = (MissionDismissal *)data;
    long MissionID = ntohl(dismiss->MissionID);
	long PlayerID = ntohl(dismiss->PlayerID);

    MissionDismiss(MissionID);
}

void Player::HandleMissionForfeit(unsigned char *data) //TODO: change handling of forfeit. at the moment, we allow ppl to repeat the mission.
{
    MissionDismissal *dismiss = (MissionDismissal *)data;
    long MissionID = ntohl(dismiss->MissionID);
	long PlayerID = ntohl(dismiss->PlayerID);

    MissionDismiss(MissionID);
}

/*
struct PetitionStuck
{
	long GameID;            // Player ID
	long ProblemType;       // see below
	char Subject[];         // variable length null-terminated string
	char Complaint[];       // variable length null-terminated string
	char PlayerList[];      // variable length null-terminated string
};

Problem Type:
  1 = I have a question about game play
  2 = I am stuck, trapped, or blackholed
  3 = I am having a problem with another player

 23 00                              Length = 35 bytes
 88 00                              Opcode 0x88 = PetitionStuck
 EF 17 C0 07                        GameID = player
 03 00 00 00                        ProblemType
 73 75 62 6A 65 63 74 00            Subject = "Subject"
 64 65 74 61 69 6C 73 00            Complaint = "Details"
 50 6C 61 79 65 72 6E 61 6D 65 00   PlayerList = "Playername"
*/

void Player::HandlePetitionStuck(unsigned char *data)
{
	char *p = (char *) data;
	long GameID = *((long *) p);
	p += 4;
	long ProblemType = *((long *) p);
	p += 4;
	char *Subject = p;
	p += strlen(p) + 1;
	char *Complaint = p;
	p += strlen(p) + 1;
	char *PlayerList = p;
	LogMessage("Received PetitionStuck packet {PlayerID=%x ProblemType=%d}\n", GameID, ProblemType);
	LogMessage("Subject=\"%s\"\n", Subject);
	LogMessage("Complaint=\"%s\"\n", Complaint);
	LogMessage("PlayerList=\"%s\"\n", PlayerList);
}


void Player::HandleIncapacitanceRequest(unsigned char *data)
{
    long player = *data; //we know who this is... But maybe for UDP we wouldn't have

	char string[] =
		"That was one heck of an explosion! Are you alright over there?\0"
		"\0\3"
		"\0\0\0\0"
		"I need a tow\0"
		"\1\0\0\0"
		"Toggle distress beacon\0"
		"\2\0\0\0"
		"I'm OK\0";


	SendOpcode(ENB_OPCODE_0054_TALK_TREE, (unsigned char *) string, sizeof(string));

	AvatarDescription avatar;
	memset(&avatar, 0, sizeof(avatar));
	memcpy(&avatar.avatar_data, &Database()->avatar, sizeof(avatar.avatar_data));
	avatar.AvatarID = GameID();
	avatar.unknown3 = 1.0;
	avatar.unknown4 = 1.0;
	SendOpcode(ENB_OPCODE_0061_AVATAR_DESCRIPTION, (unsigned char *) &avatar, sizeof(avatar));

	m_BeaconRequest = true;
}

void Player::SendWarpIndex(int index)
{
	SendOpcode(ENB_OPCODE_009C_WARP_INDEX, (unsigned char*) &index, sizeof(index));
}

void Player::WaitForHandoffReceived()
{
	long counter = 0;
    while (m_UDPHandoffReceived == false && counter < 300)
    {
        Sleep(100);
		counter++;
    }

	if (counter >= 300)
	{
		LogMessage("UDP wait timed out for player %s\n", Name());
	}

    m_UDPHandoffReceived = false;
}

void Player::LoginFailed()
{
    SectorReset();
    m_LoginFailed = true;
    m_NavCommence = true;
}



