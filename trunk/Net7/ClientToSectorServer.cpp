// ClientToSectorServer.cpp

/***********************************************
 *   ///////////////////////////////////////   *
 *   //  CLIENT TO SECTOR SERVER OPCODES  //   *
 *   ///////////////////////////////////////   *
 ***********************************************/

#include "Net7.h"
#include "Connection.h"
#include "ServerManager.h"
#include "Opcodes.h"
#include "PlayerClass.h"

void Connection::ProcessProxyClientOpcode(short opcode, short bytes)
{
	switch (opcode)
	{
    case ENB_OPCODE_3002_TCP_LOGIN_VALIDATE:
        ValidateLoginLink();
        break;

    case ENB_OPCODE_3004_PLAYER_SHIP_SENT:
        CommenceNavSend(); //shut down connection and cancel out TCP link
        break;

    case ENB_OPCODE_3003_TCP_LOGIN_CLOSE:
        LogMessage("--> Shutdown login link\n");
        ShutdownLoginLink();
        break;

    case ENB_OPCODE_3006_PLAYER_LOGIN_FAILED:
        LogMessage("LOGIN FAILURE! Resend handoff\n");
        HandleLoginFailed();
        break;

	case ENB_OPCODE_3008_STARBASE_LOGIN_COMPLETE:
		HandleStarbaseLoginComplete();
		break;

	case ENB_OPCODE_2010_SET_GLOBAL_LOGIN_LINK:
		SetConnectionToLoginLink();
		break;

	case ENB_OPCODE_2011_SET_PROXY_SECTOR_LINK:
		//Receiving this opcode is irrelevant as we're already a proxy link.
		//It's a safety feature
		//LogMessage("Re-setting link to Proxy sector.\n"); 
		break;

	default :
        ProxyClientOpcode(opcode, bytes);
		break;
	}
}

void Connection::ValidateLoginLink()
{
    long player_id = *((long*) m_RecvBuffer);
    Player *p = g_ServerMgr->m_PlayerMgr.GetPlayer(player_id);

    m_AvatarID = player_id;

    if (p)
    {
        LogMessage("Validating login link for player '%s'\n", p->Name());
		SetLoginHandoff(false);
        p->SetTCPLoginLink(this);
		m_LastOwner = p->Name();
    }
}

void Connection::ShutdownLoginLink()
{
    long player_id = *((long*) m_RecvBuffer);
    Player *p = g_ServerMgr->m_PlayerMgr.GetPlayer(player_id);

    if (p && m_AvatarID == player_id)
    {
        LogMessage("* Terminating Login link for '%s'\n", p->Name());
        p->SetTCPLoginLink(0);
        m_AvatarID = 0;
		if (m_TcpThreadRunning)
		{
#ifdef WIN32
			ResumeThread(m_RecvThreadHandle);
#else
// None at this time 
#endif
		}
		m_TcpThreadRunning = false;
    }
}

void Connection::ShutdownTCPLink(long game_id)
{
    if (m_AvatarID == game_id)
    {
		Player *p = g_ServerMgr->m_PlayerMgr.GetPlayer(game_id);
        LogMessage("Terminating TCP link for '%s'\n", p->Name());
		//first clear any remaining messages
		if (m_TcpThreadRunning)
		{
#ifdef WIN32
			ResumeThread(m_RecvThreadHandle);
#else
// None at this time
#endif
		}
		Sleep(100);
        p->SetTCPLoginLink(0);
        m_AvatarID = 0;
        m_TcpThreadRunning = false;
    }
}

void Connection::ProxyClientOpcode(short opcode, short bytes)
{
    Player *p;

    p = g_ServerMgr->m_PlayerMgr.GetPlayer(m_AvatarID);

    if (p)
    {
        p->AddMessage(opcode, bytes, m_RecvBuffer);
    }
	else
	{
		LogMessage("Player ID %x appears to have been released\n");
	}
}

void Connection::HandleStarbaseLoginComplete()
{
    long player_id = *((long*) m_RecvBuffer);
    Player *p = g_ServerMgr->m_PlayerMgr.GetPlayer(player_id);

    if (p && m_AvatarID == player_id)
    {
		if (p->PlayerIndex()->GetSectorNum() > 9999)
		{
			LogMessage("Starbase Login complete for '%s' Retaining TCP Link\n", p->Name());
		}
		else
		{
			LogMessage("Sector Login complete for '%s' Retaining TCP Link\n", p->Name());
			p->FinishLogin();
		}
        p->SetNavCommence();
    }
}

void Connection::CommenceNavSend()
{
    long player_id = *((long*) m_RecvBuffer);
    Player *p = g_ServerMgr->m_PlayerMgr.GetPlayer(player_id);

    if (p && m_AvatarID == player_id)
    {
        LogMessage("Terminating Login link for '%s'\n", p->Name());
		PulseConnectionOutput();
        p->SetTCPLoginLink(0);
        p->SetNavCommence();
        m_AvatarID = 0;
        m_TcpThreadRunning = false;
		p->FinishLogin();
    }
}

void Connection::SetConnectionToLoginLink()
{
	//now set this connection link to be a global login link
	m_ServerType = CONNECTION_TYPE_GLOBAL_PROXY_TO_SERVER;
	LogMessage("--> Changed TCP link to login link.\n");
}

void Connection::HandleLoginFailed()
{
    long player_id = *((long*) m_RecvBuffer);
    Player *p = g_ServerMgr->m_PlayerMgr.GetPlayer(player_id);

    if (p)
    {
        long sector_id = p->PlayerIndex()->GetSectorNum();
        if (sector_id > 0)
        {   
            LogMessage("Handle login fail for %s\n", p->Name());
            //p->SetActive(false);
            //g_PlayerMgr->DropPlayerFromSector(p);
            g_ServerMgr->GetSectorManager(sector_id)->SectorServerHandoff(p, sector_id);
            //p->SendOpcode(ENB_OPCODE_3006_PLAYER_LOGIN_FAILED_CONFIRM, 0, 0);
            //LogMessage("Login fail confirm - resend handoff for %s.\n", p->Name());
            p->LoginFailed();
        }
    }
}

#if 0
#include "Net7.h"
#include "Connection.h"
#include "ServerManager.h"
#include "Opcodes.h"
#include "PacketStructures.h"
#include "ObjectClass.h"
#include "ObjectManager.h"

void Connection::SendAdvancedPositionalUpdate(long object_id, PositionInformation * position_info)
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

	//LogMessage("Sending AdvancedPositionalUpdate packet\n");
	int length = 2 + 4 * index;
	SendResponse(ENB_OPCODE_003E_ADVANCED_POSITIONAL_UPDATE, (unsigned char *) &packet, length);
}

void Connection::SendRelationship(long ObjectID, long Reaction, bool IsAttacking)
{
	Relationship response;
	response.ObjectID = ntohl(ObjectID);
	response.Reaction = Reaction;
	response.IsAttacking = IsAttacking ? 1 : 0;

	//LogMessage("Sending Relationship packet\n");
	SendResponse(ENB_OPCODE_0089_RELATIONSHIP, (unsigned char *) &response, sizeof(response));
}

void Connection::SendNameDecal(int game_id, char *shipname, float h, float s, float v)
{
	NameDecal name_decal;

	memset(&name_decal, 0, sizeof(name_decal));
	name_decal.GameID = game_id;
	name_decal.RGB[0] = h;
	name_decal.RGB[1] = s;
	name_decal.RGB[2] = v;
	strncpy(name_decal.Name, shipname, sizeof(name_decal.Name) - 1);

	//LogMessage("Sending NameDecal packet\n");
	SendResponse(ENB_OPCODE_00B2_NAME_DECAL, (unsigned char *) &name_decal, sizeof(name_decal));
}

void Connection::SendDecal(int game_id, int decal_id, int decal_count)
{
	if (decal_count > MAX_DECALS)
	{
		decal_count = MAX_DECALS;
	}

	Decal decal;
	decal.GameID = game_id;
	decal.DecalCount = decal_count;
	for (int i = 0; i < decal_count; i++)
	{
		decal.Item[i].Index = i + 1;
		decal.Item[i].HSV[0] = 1.0;
		decal.Item[i].HSV[1] = 1.0;
		decal.Item[i].HSV[2] = 1.0;
		decal.Item[i].opacity = 1.0;
	}

	size_t size = ((char *) &decal.Item[decal_count]) - ((char *) &decal);

	//LogMessage("Sending Decal packet\n");
	SendResponse(ENB_OPCODE_0010_DECAL, (unsigned char *) &decal, size);
}

void Connection::SendCreate(int game_id, float scale, short asset, int type, float h, float s, float v)
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
	SendResponse(ENB_OPCODE_0004_CREATE, (unsigned char *) &create, sizeof(create));
}

void Connection::SendStart(long start_id)
{
	//LogMessage("Sending Start packet\n");
	SendResponse(ENB_OPCODE_0005_START, (unsigned char *) &start_id, sizeof(start_id));
}

void Connection::SendDataFileToClient(char *filename, long avatar_id)
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
			LogMessage("Sending data file '%s'\n", filename);
			fseek(f,0,SEEK_SET);
			unsigned char * buffer = new unsigned char[length];
			fread(buffer, 1, length, f);
			if (avatar_id)
			{
				*((long *) &buffer[4]) = avatar_id;
			}
			m_CryptOut.RC4(buffer, length);
			Send(buffer, length);
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

void Connection::SendClientType(long client_type)
{
	//LogMessage("Sending ClientType packet\n");
	SendResponse(ENB_OPCODE_003C_CLIENT_TYPE, (unsigned char *) &client_type, sizeof(client_type));
}

void Connection::SendGalaxyMap(char *system, char *sector, char *station)
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
	galaxy_map.PlayerID = m_AvatarID;

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
	SendResponse(ENB_OPCODE_0097_GALAXY_MAP, (unsigned char * ) &galaxy_map, size + 8);
}

void Connection::SendStarbaseSet(char action, char exit_mode, long sector_id)
{
	StarbaseSet starbase_set;
	memset(&starbase_set, 0, sizeof(starbase_set));
	starbase_set.StarbaseID = sector_id;
	starbase_set.Action = action;
	starbase_set.ExitMode = exit_mode;

	SendResponse(ENB_OPCODE_004F_STARBASE_SET, (unsigned char *) &starbase_set, sizeof(starbase_set));
}

void Connection::SendConstantPositionalUpdate(long game_id, float x, float y, float z, float *orientation)
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

	SendResponse(ENB_OPCODE_0040_CONSTANT_POSITIONAL_UPDATE, (unsigned char *) &update, sizeof(update));
}

void Connection::SetManufactureID(long mfg_id)
{
	SendResponse(ENB_OPCODE_007F_MANUFACTURE_SET_MANUFACTURE_ID, (unsigned char *) &mfg_id, sizeof(mfg_id));
}

void Connection::SendActivateRenderState(long game_id, unsigned long render_state_id)
{
	ActivateRenderState state;

	state.GameID = game_id;
	state.RenderStateID = render_state_id;

	SendResponse(ENB_OPCODE_0030_ACTIVATE_RENDER_STATE, (unsigned char *) &state, sizeof(state));
}

void Connection::SendActivateNextRenderState(long game_id, unsigned long render_state_id)
{
	ActivateRenderState state;

	state.GameID = game_id;
	state.RenderStateID = render_state_id;

	SendResponse(ENB_OPCODE_0031_ACTIVATE_NEXT_RENDER_STATE, (unsigned char *) &state, sizeof(state));
}

void Connection::SendSimplePositionalUpdate(long object_id, PositionInformation * position_info)
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

	//LogMessage("Sending SimplePositionalUpdate packet\n");
	SendResponse(ENB_OPCODE_0008_SIMPLE_POSITIONAL_UDPATE, (unsigned char *) &update, sizeof(update));
}

void Connection::SendPlanetPositionalUpdate(long object_id, PositionInformation * position_info)
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

	//LogMessage("Sending PlanetPositionalUpdate packet\n");
	SendResponse(ENB_OPCODE_003F_PLANET_POSITIONAL_UPDATE, (unsigned char *) &update, sizeof(update));
}

void Connection::SendResourceName(long resourceID, char *resource_name)
{
	unsigned char aux_data[64]; 
	memset(aux_data, 0, 64);
	short length = strlen(resource_name);
	*((long *) aux_data) = resourceID;
	*((short *) &aux_data[4]) = length + 4;
	*((short *) &aux_data[6]) = 0x1201;
	*((short *) &aux_data[8]) = length;

	strncpy((char*)&aux_data[10], resource_name, length);

	SendResponse(ENB_OPCODE_001B_AUX_DATA, aux_data, length+10);	
}

void Connection::SendAuxNameSignature(Object *obj)
{
    char *name = "d";
    if (obj->IsNav())
    {
        name = obj->Name();
    }

    int length = strlen(name) + 15;
    char *packet = (char*)_alloca(length);
    *((long *) packet) = obj->GameID();
    *((short *) &packet[4]) = strlen(name) + 9;
    packet[6] = 0x01;
    packet[7] = 0x72;
    *((short *) &packet[8]) = strlen(name);
    strncpy(&packet[10], name, strlen(name));
    int i = 10 + strlen(name);
    packet[i++] = obj->IsNav();
    *((float *) &packet[i]) = obj->Signature();

    SendResponse(ENB_OPCODE_001B_AUX_DATA, (unsigned char *) packet, length);    
}

void Connection::SendSimpleAuxName(Object *obj)
{
    int length = strlen(obj->Name()) + 10;
    char *packet = (char*)_alloca(length);
    *((long *) packet) = obj->GameID();
    *((short *) &packet[4]) = strlen(obj->Name()) + 4;
    packet[6] = 0x01;
    packet[7] = 0x12;
    *((short *) &packet[8]) = strlen(obj->Name());
    strncpy(&packet[10], obj->Name(), strlen(obj->Name()));

	SendResponse(ENB_OPCODE_001B_AUX_DATA, (unsigned char *) packet, length);    
}

void Connection::SendNavigation(int game_id, float signature, char visited, int nav_type, char is_huge)
{
	Navigation navigation;
	navigation.GameID = game_id;
	navigation.Signature = signature;
	navigation.PlayerHasVisited = visited;
	navigation.NavType = nav_type;
	navigation.IsHuge = is_huge;

	//LogMessage("Sending Navigation packet\n");
	SendResponse(ENB_OPCODE_0099_NAVIGATION, (unsigned char *) &navigation, sizeof(navigation));
}

void Connection::SendClientSetTime(long TimeSent)
{
	ClientSetTime data;
	data.ClientSent = TimeSent;
	data.ServerReceived = GetNet7TickCount();
	data.ServerSent = data.ServerReceived;

	SendResponse(ENB_OPCODE_0034_CLIENT_SET_TIME, (unsigned char *) &data, sizeof(data)); 
																						
	//LogMessage("Sending SetClientTime packet 0x%08x\n",TimeSent);
}

void Connection::SendMessageString(char *msg, char color, bool log)
{
	char buffer[512];
	memset(buffer, 0, sizeof(buffer));
	short length = strlen(msg) + 1;
	*((short *) &buffer[0]) = length;
	buffer[2] = color;
	strcpy(&buffer[3], msg);

	SendResponse(ENB_OPCODE_001D_MESSAGE_STRING, (unsigned char *) buffer, length + 3);
}

//#if 0

#include "Net7.h"
#include "Connection.h"
#include "ServerManager.h"
#include "Opcodes.h"
#include "PacketStructures.h"
#include "SectorData.h"
#include "VectorMath.h"
#include "UDPConnection.h"
#include "StationLoader.h"
#include "ObjectClass.h"
#include "ObjectManager.h"
#include <string>
#include "MOBclass.h"
#include "PacketMethods.h"

static int _httoi(const char *value); //convert hex string to int

int SkillPointsLevel;

void Connection::ProcessSectorServerOpcode(short opcode, short bytes)
{
	if (!m_ServerMgr.IsSectorServerReady(m_TcpPort))
	{
		// Handle MASTER SERVER to SECTOR SERVER packets
		HandleWaitingForMasterServer(opcode, bytes);
	}
	else
	{
		// Handle CLIENT to SECTOR SERVER packets
		HandleClientOpcode(opcode, bytes);
	}
}

void Connection::HandleWaitingForMasterServer(short opcode, short bytes)
{
	bool success = false;
	// Until the Sector Server gets an incoming connection from the
	// Master Server, we will ignore connections from any clients.
	switch (opcode)
	{
	case ENB_OPCODE_7801_SECTOR_ASSIGNMENT :
		if (bytes == 4)
		{
			m_SectorID = *((long *) m_RecvBuffer);
			//LogMessage("Received Sector Assignment - SectorID=%d\n", m_SectorID);

			if (m_ServerMgr.SetupSectorServer(m_SectorID))
			{
				// This is no longer a Client to Sector Server connection
				// This is now a Master Server to Sector Server connection
				m_ServerType = CONNECTION_TYPE_MASTER_SERVER_TO_SECTOR_SERVER;
				m_ServerMgr.SetSectorServerReady(m_TcpPort, true);
				LogMessage("Port:%d Sector:%d `%s`\n",
					m_TcpPort, m_SectorID, g_ServerMgr->GetSectorName(m_SectorID));
				success = true;
			}
			else
			{
				LogMessage("ERROR: m_ServerMgr.SetupSectorServer failed\n");
			}
		}
		else
		{
			LogMessage("ERROR: opcode 0x7801 is not four bytes!\n");
		}
		break;

	default :
		LogMessage("HandleWaitingForMasterServer -- UNRECOGNIZED OPCODE 0x%04x\n", opcode);
		break;
	}

	if (!success)
	{
		// TODO: Kill this connection
	}
}

void Connection::HandleClientOpcode(short opcode, short bytes)
{
	//LogMessage("HandleClientOpcode -- OPCODE 0x%04x\n", opcode);
	//DumpBuffer(m_RecvBuffer, bytes);

	switch (opcode)
	{
		case ENB_OPCODE_0002_LOGIN:
			HandleLogin();
			break;

		case ENB_OPCODE_0006_START_ACK :
			HandleStartAck();
			break;

		case ENB_OPCODE_0012_TURN:
			HandleTurn();
			break;

		case ENB_OPCODE_0013_TILT:
			HandleTilt();
			break;

		case ENB_OPCODE_0014_MOVE :
			HandleMove();
			break;

		case ENB_OPCODE_0017_REQUEST_TARGET :
			HandleRequestTarget();
			break;

		case ENB_OPCODE_0018_REQUEST_TARGETS_TARGET :
			HandleRequestTargetsTarget();
			break;

		case ENB_OPCODE_001A_DEBUG :
			HandleDebug();
			break;

		case ENB_OPCODE_0027_INVENTORY_MOVE:
			HandleInventoryMove();
			break;

		case ENB_OPCODE_0029_ITEM_STATE:
			HandleItemState();
			break;

		case ENB_OPCODE_002C_ACTION :
		case ENB_OPCODE_002D_ACTION2 :
			HandleAction();
			break;

		case ENB_OPCODE_002E_OPTION :
			HandleOption();
			break;

		case ENB_OPCODE_0033_CLIENT_CHAT :
			HandleClientChat();
			break;

		case ENB_OPCODE_0044_REQUEST_TIME :
			HandleRequestTime();
			break;

		case ENB_OPCODE_004E_STARBASE_REQUEST :
			HandleStarbaseRequest();
			break;

		case ENB_OPCODE_0051_SKILL_STRING_RQ:
			HandleSkillStringRequest();
			break;

		case ENB_OPCODE_0055_SELECT_TALK_TREE :
			HandleSelectTalkTree();
			break;

		case ENB_OPCODE_0057_SKILL_UP:
			HandleSkillAction();
			break;

		case ENB_OPCODE_0058_SKILL_ABILITY:
			HandleSkillAbility();
			break;

		case ENB_OPCODE_005A_VERB_REQUEST:
			HandleVerbRequest();
			break;

		case ENB_OPCODE_005D_EQUIP_USE :
			HandleEquipUse();
			break;

		case ENB_OPCODE_005E_AVATAR_EMOTE:
			HandleChatStream();
			break;

		case ENB_OPCODE_0079_MANUFACTURE_ITEM_CATAGORY:
			HandleManufactureTerminal();
			break;

		case ENB_OPCODE_007A_MANUFACTURE_ITEM_CATAGORY :
			HandleManufactureCategorySelection();
			break;

		case ENB_OPCODE_007B_MANUFACTURE_ITEM_ID :
			HandleManufactureSetItem();
			break;

		case ENB_OPCODE_007C_REFINERY_ITEM_ID :
			HandleRefineSetItem();
			break;

		case ENB_OPCODE_007E_MANUFACTURE_ACTION:
			HandleManufactureAction();
			break;

        case ENB_OPCODE_0080_MANUFACTURE_TECH_LEVEL_FILTER:
            HandleManufactureLevelFilter();
            break;
            
		case ENB_OPCODE_0086_MISSION_FORFEIT:
			HandleMissionForfeit();
			break;

		case ENB_OPCODE_0087_MISSION_DISMISSAL:
			HandleMissionDismissal();
			break;

		case ENB_OPCODE_0088_PETITION_STUCK:
			HandlePetitionStuck();
			break;

        case ENB_OPCODE_008D_INCAPACITANCE_REQUEST:
            HandleIncapacitanceRequest();
            break;

		case ENB_OPCODE_0098_GALAXY_MAP_REQUEST :
			HandleGalaxyMapRequest();
			break;

		case ENB_OPCODE_009B_WARP:
			HandleWarp();
			break;

		case ENB_OPCODE_009D_STARBASE_AVATAR_CHANGE :
			HandleStarbaseAvatarChange();
			break;

		case ENB_OPCODE_009F_STARBASE_ROOM_CHANGE :
			HandleStarbaseRoomChange();
			break;

		case ENB_OPCODE_00A1_TRIGGER_EMOTE :
			HandleTriggerEmote();
			break;

		case ENB_OPCODE_00A3_CLIENT_CHAT_REQUEST :
			HandleClientChatRequest();
			break;

		case ENB_OPCODE_00B9_LOGOFF_REQUEST :
			HandleLogoffRequest();
			break;

		case ENB_OPCODE_00BC_CTA_REQUEST :
			HandleCTARequest();
			break;

		case ENB_OPCODE_00C0_CONFIRMED_ACTION_RESPONSE:
			HandleActionResponse();
			break;

	default :
		LogMessage("HandleClientOpcode -- UNRECOGNIZED OPCODE 0x%04x\n", opcode);
		DumpBuffer(m_RecvBuffer, bytes);
		break;
	}
}

void Connection::HandleLogin()
{
	Login * login = (Login *) m_RecvBuffer;

	m_MasterJoin = login->join_data;

	//LogMessage("Sector server received Login packet\n");

	m_AvatarID = ntohl(m_MasterJoin.avatar_id_lsb);
	m_SectorID = ntohl(m_MasterJoin.ToSectorID);
	m_FromSectorID = ntohl(m_MasterJoin.FromSectorID);
	
	// Obtain the sector manager for the specified sector
	m_SectorMgr = m_ServerMgr.GetSectorManager(m_SectorID);
    m_Player = m_ServerMgr.m_PlayerMgr.GetPlayer(m_AvatarID, true);

	if (!m_SectorMgr)
	{
		LogMessage("HandleLogin: Missing SectorManager for sector %d\n", m_SectorID);
	}
	else if (!m_Player)
	{
		LogMessage("Unable to find player for avatar ID 0x%08x\n",m_AvatarID);
	}
    else
	{
        //set up the player with a pointer to its sector manager and object manager
        m_ObjectMgr = m_SectorMgr->GetObjectManager();
        m_Player->SetObjectManager(m_ObjectMgr);
        m_Player->SetSectorManager(m_SectorMgr);

		// Add the player to the list of players we are tracking
        // Calls the player's SectorLogin() and FirstLogin() methods
		//m_ServerMgr.m_PlayerMgr.AddPlayer(m_AvatarID, this);

		// Set the client time
		SendClientSetTime(login->TimeSent);
		LogDebug("Sending server time\n");
		
		// Send the sector content to the client - NB AddPlayer must occur before SectorLogin
		m_SectorMgr->HandleSectorLogin(m_Player);
		
		m_StarbaseTargetID = 0;

		m_ActionResponseReceived = false;
        m_Player->SaveData();
	}
}

void Connection::HandleSkillAction()
{
    SkillAction * Skill = (SkillAction *)  m_RecvBuffer;
    //m_Player->HandleSkillAction(Skill);
}

void Connection::HandleSkillAbility()
{
    SkillUse * Ability = (SkillUse *) m_RecvBuffer;
	//m_Player->HandleSkillAbility(Ability);
}

//------MANUFACTURING

// Request Catagorys
void Connection::HandleManufactureTerminal() 
{
	ManufactureData * Packet = (ManufactureData *) m_RecvBuffer;
    //m_Player->HandleManufactureTerminal(ntohl(Packet->Data));
}

// Called when a catigory is selected
void Connection::HandleManufactureCategorySelection()
{
	ManufactureData * Packet = (ManufactureData *) m_RecvBuffer;
    //m_Player->HandleManufactureCategorySelection(ntohl(Packet->Data));
}

// Called when a formula is selected
void Connection::HandleManufactureSetItem()
{
	ManufactureData * Packet = (ManufactureData *) m_RecvBuffer;
    //m_Player->HandleManufactureSetItem(ntohl(Packet->Data));
}

void Connection::HandleManufactureLevelFilter()
{
    ManufactureTechLevelFilter * Filter = (ManufactureTechLevelFilter *) m_RecvBuffer;
    //m_Player->HandleManufactureLevelFilter(Filter->Enable, ntohl(Filter->BitField));
}

// --- Handels cration/dismantel of items
// Does it 'Handel' Bach as well? Sorry, couldn't resist!
void Connection::HandleManufactureAction()
{
	ManufactureData * Packet = (ManufactureData *) m_RecvBuffer;
    //m_Player->HandleManufactureAction(ntohl(Packet->Data));
}

void Connection::HandleRefineSetItem()
{
	ManufactureData * Packet = (ManufactureData *) m_RecvBuffer;
    //m_Player->HandleRefineSetItem(Packet->Data);
}

//-------------------

void Connection::RemovePlayer() //this just signals graphic removal of player from sector
{
    m_Player->Remove();
}

void Connection::SendStarbaseSet(char action, char exit_mode)
{
	StarbaseSet starbase_set;
	memset(&starbase_set, 0, sizeof(starbase_set));
	starbase_set.StarbaseID = m_SectorID;
	starbase_set.Action = action;
	starbase_set.ExitMode = exit_mode;

	//LogMessage("Sending StarbaseSet packet\n");
	SendResponse(ENB_OPCODE_004F_STARBASE_SET, (unsigned char *) &starbase_set, sizeof(starbase_set));
}

long Connection::TryLoungeFile(long sector_id)
{
    long return_sector = sector_id;
	char old_path[MAX_PATH];
    char lounge_npc[MAX_PATH];

    sprintf(lounge_npc, "LoungeNPC_%d.dat", sector_id);

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

void Connection::SendDataFileToClient(char *filename, long avatar_id)
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
			LogMessage("Sending data file '%s'\n", filename);
			fseek(f,0,SEEK_SET);
			unsigned char * buffer = new unsigned char[length];
			fread(buffer, 1, length, f);
			if (avatar_id)
			{
				*((long *) &buffer[4]) = avatar_id;
			}
			m_CryptOut.RC4(buffer, length);
			Send(buffer, length);
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

void Connection::SendConfirmedActionOffer()
{
	unsigned char action_data[] = 
	{
		0x00, 0x00, 0x00, 0x01,
		0x00, 0x00, 0x00, 0x65,
		0x07, 0x00, 0x4d, 0x65, 
		0x73, 0x73, 0x61, 0x67, 0x65
	};

	SendResponse(ENB_OPCODE_00BE_CONFIRMED_ACTION_OFFER, action_data, sizeof(action_data));	
	SendClientSound("push_mission_alert_sound", 2, 0);
}

void Connection::HandleActionResponse()
{
	long player_id = ntohl(*((long *) &m_RecvBuffer[0]));
	if (player_id == m_AvatarID)
	{
		m_ActionResponseReceived = true;
		ProcessConfirmedActionOffer();
	}
}

void Connection::ProcessConfirmedActionOffer()
{
	int length;
	char buffer[256];
	memset(&buffer, 0, 256);

	if (m_ActionResponseReceived == false)
	{
		SendConfirmedActionOffer();
		//m_SectorMgr->AddTimedCall(m_Player, B_FORCE_LOGOUT, 20000,0);
	}
	else
	{
		//send the message
		length = sprintf(buffer,"Hi %s, there is a new version of sockettest out now (v0.95), You need to download and use it to get correct movement.\
			you will be logged out in 20 seconds from receipt of this message.\nCheers, TB.", m_Player->Name());
		SendResponse(ENB_OPCODE_0054_TALK_TREE, (unsigned char *) buffer, length+3);
		
		{
			AvatarDescription avatar;
			memset(&avatar, 0, sizeof(avatar));
			memcpy(&avatar.avatar_data, &m_Player->Database()->avatar, sizeof(avatar.avatar_data));
			avatar.AvatarID = m_Player->GameID();
			avatar.unknown3 = 1.0;
			avatar.unknown4 = 1.0;
			SendResponse(ENB_OPCODE_0061_AVATAR_DESCRIPTION, (unsigned char *) &avatar, sizeof(avatar));
		}
	}
}

void Connection::SendClientSetTime(long TimeSent)
{
	ClientSetTime data;
	data.ClientSent = TimeSent;
	data.ServerReceived = GetNet7TickCount();
	data.ServerSent = data.ServerReceived;

	SendResponse(ENB_OPCODE_0034_CLIENT_SET_TIME, (unsigned char *) &data, sizeof(data)); 
																						
	//LogMessage("Sending SetClientTime packet 0x%08x\n",TimeSent);
}

void Connection::SendObjectToObjectLinkedEffect(Object *target, Object *source, short effect1, short effect2, float speedup)
{
	unsigned char link_data[128];
	memset(link_data,0,128);
    int index = 0;

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

    SendResponse(ENB_OPCODE_000E_OBJECT_TO_OBJECT_LINKED_EFFECT, link_data, index);
}

void Connection::SendObjectEffect(ObjectEffect *obj_effect)
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

    SendResponse(ENB_OPCODE_0009_OBJECT_EFFECT, effect, index);
}

void Connection::SendClientType(long client_type)
{
	//LogMessage("Sending ClientType packet\n");
	SendResponse(ENB_OPCODE_003C_CLIENT_TYPE, (unsigned char *) &client_type, sizeof(client_type));
}

void Connection::SendStart(long start_id)
{
	//LogMessage("Sending Start packet\n");
	SendResponse(ENB_OPCODE_0005_START, (unsigned char *) &start_id, sizeof(start_id));
}

void Connection::SendSetBBox(float xmin, float ymin, float xmax, float ymax)
{
	SetBBox bbox;
	bbox.XMin = xmin;
	bbox.YMin = ymin;
	bbox.XMax = xmax;
	bbox.YMax = ymax;

	//LogMessage("Sending SetBBox packet\n");
	SendResponse(ENB_OPCODE_002B_SET_BBOX, (unsigned char *) &bbox, sizeof(bbox));
}

void Connection::SendSetZBand(float min, float max)
{
	SetZBand zband;
	zband.Min = min;
	zband.Max = min;

	//LogMessage("Sending SetZBand packet\n");
	SendResponse(ENB_OPCODE_002A_SET_ZBAND, (unsigned char *) &zband, sizeof(zband));
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

void Connection::SendNavigation(int game_id, float signature, char visited, int nav_type, char is_huge)
{
	Navigation navigation;
	navigation.GameID = game_id;
	navigation.Signature = signature;
	navigation.PlayerHasVisited = visited;
	navigation.NavType = nav_type;
	navigation.IsHuge = is_huge;

	//LogMessage("Sending Navigation packet\n");
	SendResponse(ENB_OPCODE_0099_NAVIGATION, (unsigned char *) &navigation, sizeof(navigation));
}

void Connection::SendCreateAttachment(int parent, int child, int slot)
{
	CreateAttachment attachment;
	attachment.Parent_ID = ntohl(parent);
	attachment.Child_ID = ntohl(child);
	attachment.Slot = ntohl(slot);

	//LogMessage("Sending CreateAttachment packet\n");
	SendResponse(ENB_OPCODE_004A_CREATE_ATTACHMENT, (unsigned char *) &attachment, sizeof(attachment));
}

void Connection::SendDecal(int game_id, int decal_id, int decal_count)
{
	if (decal_count > MAX_DECALS)
	{
		decal_count = MAX_DECALS;
	}

	Decal decal;
	decal.GameID = game_id;
	decal.DecalCount = decal_count;
	for (int i = 0; i < decal_count; i++)
	{
		decal.Item[i].Index = i + 1;
		decal.Item[i].HSV[0] = 1.0;
		decal.Item[i].HSV[1] = 1.0;
		decal.Item[i].HSV[2] = 1.0;
		decal.Item[i].opacity = 1.0;
	}

	size_t size = ((char *) &decal.Item[decal_count]) - ((char *) &decal);

	//LogMessage("Sending Decal packet\n");
	SendResponse(ENB_OPCODE_0010_DECAL, (unsigned char *) &decal, size);
}

void Connection::SendNameDecal(int game_id, char *shipname, float h, float s, float v)
{
	NameDecal name_decal;

	memset(&name_decal, 0, sizeof(name_decal));
	name_decal.GameID = game_id;
	name_decal.RGB[0] = h;
	name_decal.RGB[1] = s;
	name_decal.RGB[2] = v;
	strncpy(name_decal.Name, shipname, sizeof(name_decal.Name) - 1);

	//LogMessage("Sending NameDecal packet\n");
	SendResponse(ENB_OPCODE_00B2_NAME_DECAL, (unsigned char *) &name_decal, sizeof(name_decal));
}

void Connection::SendConstantPositionalUpdate(long game_id, float x, float y, float z, float *orientation)
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

	//LogMessage("Sending ConstantPositionalUpdate packet\n");
	SendResponse(ENB_OPCODE_0040_CONSTANT_POSITIONAL_UPDATE, (unsigned char *) &update, sizeof(update));
}

void Connection::SendFormationPositionalUpdate(long leader_id, long target_id, float x, float y, float z)
{
	FormationPositionalUpdate update;
	memset(&update, 0, sizeof(update));

	update.LeaderID = leader_id;
	update.TargetID = target_id;
	update.Position[0] = x;
	update.Position[1] = y;
	update.Position[2] = z;

	//LogMessage("Sending FormationPositionalUpdate packet\n");
	SendResponse(ENB_OPCODE_0041_FORMATION_POSITIONAL_UPDATE, (unsigned char *) &update, sizeof(update));
}

void Connection::SendSimplePositionalUpdate(long object_id, PositionInformation * position_info)
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

	//LogMessage("Sending SimplePositionalUpdate packet\n");
	SendResponse(ENB_OPCODE_0008_SIMPLE_POSITIONAL_UDPATE, (unsigned char *) &update, sizeof(update));
}

void Connection::SendPlanetPositionalUpdate(long object_id, PositionInformation * position_info)
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

	//LogMessage("Sending PlanetPositionalUpdate packet\n");
	SendResponse(ENB_OPCODE_003F_PLANET_POSITIONAL_UPDATE, (unsigned char *) &update, sizeof(update));
}

void Connection::SendComponentPositionalUpdate(long object_id, PositionInformation * position_info, long timestamp)
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

	//LogMessage("Sending ComponentPositionalUpdate packet\n");
	SendResponse(ENB_OPCODE_0046_COMPONENT_POSITIONAL_UPDATE, (unsigned char *) &update, sizeof(update));
}

void Connection::SendAdvancedPositionalUpdate(long object_id, PositionInformation * position_info)
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

	//LogMessage("Sending AdvancedPositionalUpdate packet\n");
	int length = 2 + 4 * index;
	SendResponse(ENB_OPCODE_003E_ADVANCED_POSITIONAL_UPDATE, (unsigned char *) &packet, length);
}

void Connection::SendObjectToObjectEffect(ObjectToObjectEffect *obj_effect)
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

    SendResponse(ENB_OPCODE_000B_OBJECT_TO_OBJECT_EFFECT, effect, index);
}

void Connection::SendPointEffect(int effect_id, float x, float y, float z, short duration,
									short effect_desc_id, float scale, float h, float s, float v )
{
}

void Connection::SendActivateRenderState(long game_id, unsigned long render_state_id)
{
	ActivateRenderState state;

	state.GameID = game_id;
	state.RenderStateID = render_state_id;

	//LogMessage("Sending ActivateRenderState packet\n");
	SendResponse(ENB_OPCODE_0030_ACTIVATE_RENDER_STATE, (unsigned char *) &state, sizeof(state));
}


//TODO: Find out more about packet data structure - this is just a guess.
void Connection::SendInitRenderState(long game_id, unsigned long render_state_id)
{
	InitRenderState state;

	state.GameID = game_id;
	state.RenderStateID = render_state_id;

	SendResponse(ENB_OPCODE_002F_INIT_RENDER_STATE, (unsigned char *) &state, sizeof(state));
}

void Connection::SendActivateNextRenderState(long game_id, unsigned long render_state_id)
{
	ActivateRenderState state;

	state.GameID = game_id;
	state.RenderStateID = render_state_id;

	//LogMessage("Sending ActivateRenderState packet\n");
	SendResponse(ENB_OPCODE_0031_ACTIVATE_NEXT_RENDER_STATE, (unsigned char *) &state, sizeof(state));
}

void Connection::SendDeactivateRenderState(long game_id)
{
	/*ActivateRenderState state;

	state.GameID = game_id;
	state.RenderStateID = render_state_id;*/

	//LogMessage("Sending ActivateRenderState packet\n");
	SendResponse(ENB_OPCODE_0032_DEACTIVATE_RENDER_STATE, (unsigned char *) &game_id, sizeof(game_id));
}

void Connection::SendCreate(int game_id, float scale, short asset, int type, float h, float s, float v)
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
	SendResponse(ENB_OPCODE_0004_CREATE, (unsigned char *) &create, sizeof(create));
}

void Connection::HandleSkillStringRequest()
{
	ClientSkillsRequest * request = (ClientSkillsRequest *) m_RecvBuffer;

    //Loot item
    //not sure if this packet is used for anything other than looting
    
    //check we're targetting a HUSK
    Object *obj = m_ObjectMgr->GetObjectFromID(m_Player->ShipIndex()->GetTargetGameID());

    //TODO: set and check loot timer.
    if (obj && obj->ObjectType() == OT_HUSK)
    {
        //Now check no-one else is looting this object
        char *player = obj->CheckResourceLock();
        if (player)
        {
            m_Player->SendVaMessage("%s currently being looted by %s", obj->Name(), player);
        }
        else
        {
            SendClientSound("coin.wav",0, 0);
            long id = ntohl(obj->GameID());
            SendResponse(ENB_OPCODE_008C_LOOT_HULK_PERMISSION, (unsigned char *)&id, sizeof(long));
            m_ProspectWindow = true;
        }
    }
}

void Connection::HandleStartAck()
{
	LogDebug("Received StartAck packet\n");

	// Added to do things on sector change at top of 'Player' class
    // don't put them here!!

    //send the login camera just after we finally log in
    m_Player->SendLoginCamera();
}

void Connection::HandleRequestTime()
{
	LogDebug("Received RequestTime packet\n");

	// Set the client time
	// This is extremely high priority
	SendClientSetTime(*((long *) m_RecvBuffer));
	//don't set the connection as active until we get the time RQ
	SetActive(true);
}

void Connection::HandleClientChatRequest()
{
	ClientChatRequest * request = (ClientChatRequest *) m_RecvBuffer;

	//LogMessage("Received ClientChatRequest packet\n");
	//char Nick[30], Message[200];
	char * string1 = "";
	char * string2 = "";
	char * string3 = "";

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
		m_ServerMgr.m_PlayerMgr.ChatSendPrivate(m_AvatarID, string1, string3);
	}

	if (request->unknown1 == 0)		// Channel Message
	{
		//LogMessage("ClientChatRequest: SendToChannel: %s Message: %s\n", string2, string3);
		m_ServerMgr.m_PlayerMgr.ChatSendChannel(m_AvatarID, string2, string3);
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

void Connection::HandleTurn()
{
	struct PacketTurn {
		long GameID;
		float Intensity;
	} ATTRIB_PACKED;

	PacketTurn * Turning = (PacketTurn *)  m_RecvBuffer;

    if (!m_Player->WarpDrive())
    {
        m_Player->AbortProspecting(true,false);
        m_Player->Turn(Turning->Intensity);
    }
}

void Connection::HandleTilt()
{
	struct PacketTurn {
		long GameID;
		float Intensity;
	} ATTRIB_PACKED;

	PacketTurn * Turning = (PacketTurn *)  m_RecvBuffer;

    if (!m_Player->WarpDrive())
    {
        m_Player->AbortProspecting(true,false);
        m_Player->Tilt(Turning->Intensity);
    }
}

void Connection::HandleMove()
{
	MovePacket * Movement = (MovePacket *) m_RecvBuffer;

    if (!m_Player->WarpDrive())
    {
        m_Player->AbortProspecting(true,false);
        
        //LogMessage("Received Move packet Type: %d\n", Movement->type);
        if (Movement->type == 4)
        {
            m_Player->SendContrails(false);
        }
        else
        {
            m_Player->SendContrails(true);
        }
        
        m_Player->Move(Movement->type);
    }
}

void Connection::HandleWarp()
{
	WarpPacket * warp = (WarpPacket *) m_RecvBuffer;

    if (m_Player->WarpDrive())
    {
        m_Player->TerminateWarp(true);
    }
    else
    {
        m_Player->SendContrails(false);
        LogDebug("Warp Navs: %ld, GameID=%d (%s)\n", warp->Navs, (warp->GameID & 0x00FFFFFF), m_Player->Name());
        m_Player->SetupWarpNavs(warp->Navs, warp->TargetID);
        m_Player->PrepareForWarp();
    }
}

void Connection::Contrails(long player_id, bool contrails)
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

	SendResponse(ENB_OPCODE_001B_AUX_DATA, aux_data, sizeof(aux_data));
}

void Connection::SendResourceName(long resourceID, char *resource_name)
{
	unsigned char aux_data[64]; 
	memset(aux_data, 0, 64);
	short length = strlen(resource_name);
	*((long *) aux_data) = resourceID;
	*((short *) &aux_data[4]) = length + 4;
	*((short *) &aux_data[6]) = 0x1201;
	*((short *) &aux_data[8]) = length;

	strncpy((char*)&aux_data[10], resource_name, length);

	SendResponse(ENB_OPCODE_001B_AUX_DATA, aux_data, length+10);	
}

//this is the same way that the live server sent husk content, but it is horrendously inefficient
//I wonder if there's a better way we can send Husk content
//We can re-do this now with Net7Proxy
void Connection::SendHuskContent(Object *husk)
{
    bool contents_already_sent;

    if (husk)
    {
        contents_already_sent = husk->GetIndex(m_Player->ResourceSendList());
        
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

            unsigned char aux_data[512];
            int index = 0;
            int x;

            short length = strlen(husk->Name());
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
                    m_Player->SendItemBase(slot->item->ItemTemplateID());
                }
                else
                {
                    AddBuffer(aux_data, filler, 7, index);
                }
            }

            for (x = 0; x < 32; x++)
            {
                AddBuffer(aux_data, filler, 7, index);
            }

            //set length
            *((short *) &aux_data[4]) = index-8;
            
            SendResponse(ENB_OPCODE_001B_AUX_DATA, aux_data, index);
            husk->SetIndex(m_Player->ResourceSendList());
        }
    }
}



void Connection::SendHuskName(Object *husk)
{
	unsigned char aux_data[64]; 
	memset(aux_data, 0, 64);
    int index = 0;
    
	short length = strlen(husk->Name());
    AddData(aux_data, husk->GameID(), index);
    AddData(aux_data, (short)(length+10), index);
    AddData(aux_data, (long) (0x03E01601), index);
    AddData(aux_data, (short)(length), index);
    AddDataS(aux_data, husk->Name(), index);
    AddData(aux_data, (long)0x05050505, index);

	SendResponse(ENB_OPCODE_001B_AUX_DATA, aux_data, index);	
}

void Connection::SendMobName(Object *mob)
{
    int length = strlen(mob->Name());
    if (length > 24) length = 24;

	unsigned char aux_data[] = 
	{
		0x00, 0x00, 0x00, 0x00, //game id
		0x3B, 0x00, //AUX length
		0x01, 
		0x16, 0x30, 0x00, 0x40, 0x00, 0x00, 0x0C, 0x80, 0x03, 
		0x00, 0xBE, 0x40, 0xF8, 0xC0, 0x07, //always the same

		0x18, 0x00, //[22]

		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //[24]
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

		0x00, 0x00, 0xDA, 0x42, 0x00, 0x00, 0xDA, 0x42, //[48] (Hull strength)
		0x00, 0x00, 0x00, 0x00, 0x05, 0x05, 0x05, 0x05, //[56] (MOB level)
		0x05, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  
	};

	*((long *) aux_data) = mob->GameID();
	*((short *) &aux_data[22]) = (short)length;
	
	strncpy((char*)(&aux_data[24]), mob->Name(), length);

	*((long *) &aux_data[56]) = mob->Level();

	if (length < 24) 
	{
		memcpy((&aux_data[24+length]), (&aux_data[48]), 25);
	}

	SendResponse(ENB_OPCODE_001B_AUX_DATA, aux_data, sizeof(aux_data)); 
}

void Connection::SendSimpleAuxName(Object *obj)
{
    int length = strlen(obj->Name()) + 10;
    char *packet = (char*)_alloca(length);
    *((long *) packet) = obj->GameID();
    *((short *) &packet[4]) = strlen(obj->Name()) + 4;
    packet[6] = 0x01;
    packet[7] = 0x12;
    *((short *) &packet[8]) = strlen(obj->Name());
    strncpy(&packet[10], obj->Name(), strlen(obj->Name()));

	SendResponse(ENB_OPCODE_001B_AUX_DATA, (unsigned char *) packet, length);    
}

void Connection::SendAuxNameSignature(Object *obj)
{
    char *name = "d";
    if (obj->IsNav())
    {
        name = obj->Name();
    }

    int length = strlen(name) + 15;
    char *packet = (char*)_alloca(length);
    *((long *) packet) = obj->GameID();
    *((short *) &packet[4]) = strlen(name) + 9;
    packet[6] = 0x01;
    packet[7] = 0x72;
    *((short *) &packet[8]) = strlen(name);
    strncpy(&packet[10], name, strlen(name));
    int i = 10 + strlen(name);
    packet[i++] = obj->IsNav();
    *((float *) &packet[i]) = obj->Signature();

    SendResponse(ENB_OPCODE_001B_AUX_DATA, (unsigned char *) packet, length);    
}

void Connection::SendAuxNameResource(Object *obj)
{
    int length = strlen(obj->Name()) + 11;
    char *packet = (char*)_alloca(length);
    *((long *) packet) = obj->GameID();
    *((short *) &packet[4]) = strlen(obj->Name()) + 5;
    packet[6] = 0x01;
    packet[7] = 0x16;
	packet[8] = 0x04;
    *((short *) &packet[9]) = strlen(obj->Name());
    strncpy(&packet[11], obj->Name(), strlen(obj->Name()));

	SendResponse(ENB_OPCODE_001B_AUX_DATA, (unsigned char *) packet, length);
}

void Connection::UnSetTarget(long GameID)
{
	if (m_Player->ShipIndex()->GetTargetGameID() == GameID)
	{
		if (m_ProspectWindow)
		{
			m_ProspectWindow = false;
			OpenInterface(1,0);
		}
		//LogMessage("Sending SetTarget packet\n");
		SendSetTarget(0, -1);
		
		// Display nothing
		m_Player->ShipIndex()->SetTargetThreatLevel(-1);
		m_Player->ShipIndex()->SetTargetThreat("");
		m_Player->ShipIndex()->SetTargetGameID(-1);
		m_Player->SendAuxShip();
        m_Player->BlankVerbs();
	}
}

void Connection::ActivateProspectBeam(long player_id, long target_id, char *message, short effect_type, long effectUID, long timestamp, short effect_time)
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

int LevelXP[] = { 500,1000,1500,2000,3000,4000,5000,7000,9000,11000,13000,15000,20000,25000,
				  30000,35000,40000,47500,55000,62500,70000,77500,87500,97500,107500,117500,127500,
				  140000,152500,165000,177500,190000,205000,220000,235000,250000,265000,285000,305000,
				  325000,345000,365000,390000,415000,440000,475000,510000,545000,595000,645000,
				  // Added XP for levels over 50
				  645000,645000,645000,645000,645000,645000,645000,645000,645000,645000,645000,645000,
				  645000,645000,645000,645000,645000,645000,645000,645000,645000,645000,645000,645000,
				  645000,645000,645000,645000,645000,645000,645000,645000,645000,645000,645000,645000,
				  645000,645000,645000,645000,645000,645000,645000,645000,645000,645000,645000,645000,
				  645000,645000,645000,645000,645000,645000,645000,645000,645000,645000,645000,645000
				};


//XP methods (the "xxx experience earned: xxx" message must be sent by parent method)

void Connection::AddCombatXP(char * mob_name, long XP_Gain, bool SkipGroup)
{
	bool GroupXP = false;
	char msg_buffer[128];

#ifdef BETA_TESTING
    XP_Gain *= 2;
#endif

	if (!SkipGroup)
	{
		GroupXP = m_ServerMgr.m_PlayerMgr.GroupXP(m_Player, mob_name, XP_Gain, m_Player->GroupID(), 0);		// 0 = Combat 1 = Explore
	} 

	if (!GroupXP)
	{
		u32 myCombatLevel = m_Player->PlayerIndex()->RPGInfo.GetCombatLevel();
		u32 mySkillPointsEarned = 0;

		if (myCombatLevel > 50)
		{
			myCombatLevel = 50;
		}

        long XP = (long)(m_Player->PlayerIndex()->RPGInfo.GetCombatXP() * LevelXP[myCombatLevel]) + XP_Gain;

		while (XP >= LevelXP[myCombatLevel]) 
		{			
            XP -= LevelXP[myCombatLevel];
			if (myCombatLevel <= 19) 
			{
				mySkillPointsEarned++;
                myCombatLevel++;
			} 
			else if (myCombatLevel <= 39) 
			{
				mySkillPointsEarned += 2;
                myCombatLevel++;
			} 
			else if (myCombatLevel < 50) 
			{
				mySkillPointsEarned += 3;
                myCombatLevel++;
			} 
			else
			{
				mySkillPointsEarned++;
                //TODO: distribute XP to other bars
			}
		}

		float myCombatXP = (float)(XP)/(float)(LevelXP[myCombatLevel]);

		if (!SkipGroup)
		{
			sprintf(msg_buffer, "%s %d Combat experience earned", mob_name, XP_Gain); 
			SendPriorityMessageString(msg_buffer,"MessageLine",5000,4);
		}

		m_Player->PlayerIndex()->RPGInfo.SetCombatXP(myCombatXP);

		if (mySkillPointsEarned > 0)
		{
			SendClientSound("Player_Levels",0,1);

			m_Player->PlayerIndex()->RPGInfo.SetCombatLevel(myCombatLevel);
			m_Player->PlayerIndex()->RPGInfo.SetSkillPoints(m_Player->PlayerIndex()->RPGInfo.GetSkillPoints() + mySkillPointsEarned);
			m_Player->PlayerIndex()->RPGInfo.SetTotalSkillPoints(m_Player->PlayerIndex()->RPGInfo.GetTotalSkillPoints() + mySkillPointsEarned);

			SendPushMessage("LEVEL UP!","QuickLine",0,3);
			sprintf(msg_buffer, "Combat level is now %d!", myCombatLevel);
			SendPushMessage(msg_buffer, "MessageLine", 3000, 3);

			// Update the combat level display when targeting
			m_Player->ShipIndex()->SetCombatLevel(myCombatLevel);
            m_Player->ShipIndex()->SetRank(m_Player->GetRank());
			m_Player->SendAuxShip();

            m_Player->LevelUpForSkills();
            m_Player->UpdateSkills();
		}
	}
}

void Connection::AddExploreXP(char * raw_name, long XP_Gain, bool SkipGroup)
{
	bool GroupXP = false;
	char msg_buffer[128];

#ifdef BETA_TESTING
    XP_Gain *= 2;
#endif

	if (!SkipGroup)
	{
		GroupXP = m_ServerMgr.m_PlayerMgr.GroupXP(m_Player, raw_name, XP_Gain, m_Player->GroupID(), 1);		// 0 = Combat 1 = Explore
	}

	if (!GroupXP)
	{
		u32 myExploreLevel = m_Player->PlayerIndex()->RPGInfo.GetExploreLevel();
		u32 mySkillPointsEarned = 0;

		if (myExploreLevel > 50)
		{
			myExploreLevel = 50;
		}

        long XP = (long)(m_Player->PlayerIndex()->RPGInfo.GetExploreXP() * LevelXP[myExploreLevel]) + XP_Gain;

		while (XP >= LevelXP[myExploreLevel]) 
		{			
            XP -= LevelXP[myExploreLevel];
			if (myExploreLevel <= 19) 
			{
				mySkillPointsEarned++;
                myExploreLevel++;
			} 
			else if (myExploreLevel <= 39) 
			{
				mySkillPointsEarned += 2;
                myExploreLevel++;
			} 
			else if (myExploreLevel < 50) 
			{
				mySkillPointsEarned += 3;
                myExploreLevel++;
			} 
			else
			{
				mySkillPointsEarned++;
                //TODO: distribute XP to other bars
			}
		}

		float myExploreXP = (float)(XP)/(float)LevelXP[myExploreLevel];

		if (!SkipGroup)
		{
			sprintf(msg_buffer, "%s %d Explore experience earned", raw_name, XP_Gain);
			SendPriorityMessageString(msg_buffer,"MessageLine",5000,4);
		}

		m_Player->PlayerIndex()->RPGInfo.SetExploreXP(myExploreXP);

		if (mySkillPointsEarned > 0)
		{
			SendClientSound("Player_Levels",0,1);

			m_Player->PlayerIndex()->RPGInfo.SetExploreLevel(myExploreLevel);
			m_Player->PlayerIndex()->RPGInfo.SetSkillPoints(m_Player->PlayerIndex()->RPGInfo.GetSkillPoints() + mySkillPointsEarned);
			m_Player->PlayerIndex()->RPGInfo.SetTotalSkillPoints(m_Player->PlayerIndex()->RPGInfo.GetTotalSkillPoints() + mySkillPointsEarned);

			SendPushMessage("LEVEL UP!","QuickLine",0,3);
			sprintf(msg_buffer, "Explore level is now %d!", myExploreLevel);
			SendPushMessage(msg_buffer, "MessageLine", 3000, 3);

            m_Player->LevelUpForSkills();
            m_Player->UpdateSkills();
            m_Player->ShipIndex()->SetRank(m_Player->GetRank());
            m_Player->SendAuxShip();
		}
	}
}

void Connection::AddTradeXP(char * name, long XP_Gain, bool SkipGroup)
{
	char msg_buffer[128];

	u32 myTradeLevel = m_Player->PlayerIndex()->RPGInfo.GetTradeLevel();
	u32 mySkillPointsEarned = 0;

#ifdef BETA_TESTING
    XP_Gain *= 2;
#endif

	sprintf(msg_buffer, "%s %d Trade experience earned", name, XP_Gain); 				
    SendPriorityMessageString(msg_buffer,"MessageLine",5000,4);
    
    if (myTradeLevel > 50)
    {
        myTradeLevel = 50;
    }

    long XP = (long)(m_Player->PlayerIndex()->RPGInfo.GetTradeXP() * LevelXP[myTradeLevel]) + XP_Gain;
    
    while (XP >= LevelXP[myTradeLevel]) 
    {			
        XP -= LevelXP[myTradeLevel];
        if (myTradeLevel <= 19) 
        {
            mySkillPointsEarned++;
            myTradeLevel++;
        } 
        else if (myTradeLevel <= 39) 
        {
            mySkillPointsEarned += 2;
            myTradeLevel++;
        } 
        else if (myTradeLevel < 50) 
        {
            mySkillPointsEarned += 3;
            myTradeLevel++;
        } 
        else
        {
            mySkillPointsEarned++;
            //TODO: distribute XP to other bars
        }
    }
    
    float myTradeXP = (float)(XP)/(float)(LevelXP[myTradeLevel]);
    
	m_Player->PlayerIndex()->RPGInfo.SetTradeXP(myTradeXP);

	if (mySkillPointsEarned > 0)
	{
		SendClientSound("Player_Levels",0,1);

		m_Player->PlayerIndex()->RPGInfo.SetTradeLevel(myTradeLevel);
		m_Player->PlayerIndex()->RPGInfo.SetSkillPoints(m_Player->PlayerIndex()->RPGInfo.GetSkillPoints() + mySkillPointsEarned);
		m_Player->PlayerIndex()->RPGInfo.SetTotalSkillPoints(m_Player->PlayerIndex()->RPGInfo.GetTotalSkillPoints() + mySkillPointsEarned);

		SendPushMessage("LEVEL UP!","QuickLine",0,3);
		sprintf(msg_buffer, "Trade level is now %d!", myTradeLevel);
		SendPushMessage(msg_buffer, "MessageLine", 3000, 3);

        m_Player->LevelUpForSkills();
        m_Player->UpdateSkills();
        m_Player->ShipIndex()->SetRank(m_Player->GetRank());
        m_Player->SendAuxShip();
	}
}

void Connection::AddMOBDestroyExperience(short mob_level, char *mob_name)
{
	char msg_buffer[128];

	short myXPEarned = 250 + 100 * (mob_level / (m_Player->PlayerIndex()->RPGInfo.GetCombatLevel() + 1));

	LogMessage("Mob XP: %d MobLevel: %d (%s)\n", myXPEarned, mob_level, m_Player->Name());
	
	// Cap it at 1600
	if (myXPEarned > 1600) myXPEarned = 1600;
	if (myXPEarned <= 0) myXPEarned = 0;

	sprintf(msg_buffer, "Defeated %s:", mob_name);

	AddCombatXP(msg_buffer, myXPEarned);
	m_Player->SendAuxPlayer();
}

void Connection::SendPushMessage(char *msg1, char *type, long time, long priority)
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

	SendResponse(ENB_OPCODE_0022_PUSH_MESSAGE, pptr, index);
}

void Connection::SetResourceDrainLevel(Object *obj, long slot)
{
    unsigned long slot_index = ((0x10 << slot) | 0x02); //slot index calc
    long length = 28;
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

	SendResponse(ENB_OPCODE_001B_AUX_DATA, aux_data, length); //this seems to initiate resource collapse or drains the resource according to last float val
}

void Connection::RemoveObject(long object_id)
{
	UnSetTarget(object_id);
	SendResponse(ENB_OPCODE_0007_REMOVE, (unsigned char *) &object_id, sizeof(object_id)); //remove the raw resource
}

//This is where I put all the AUX prospecting stuff I don't really understand
//any help decoding any of this appreciated!

//TODO: REMOVE THIS!!
void Connection::SendProspectAUX(long value, int type)
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
			
			SendResponse(ENB_OPCODE_001B_AUX_DATA, aux_data, sizeof(aux_data));
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
			
			SendResponse(ENB_OPCODE_001B_AUX_DATA, aux_data, sizeof(aux_data));
		}
		break;

	case 2:
		{
			m_Player->ShipIndex()->SetTargetGameID(0);
			m_Player->SendAuxShip();
            m_Player->BlankVerbs();
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
			SendResponse(ENB_OPCODE_001B_AUX_DATA, aux_data, sizeof(aux_data));
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
			
			SendResponse(ENB_OPCODE_001B_AUX_DATA, aux_data, sizeof(aux_data));
		}
		break;
	}
}

void Connection::CreateTractorComponent(float *position, float decay, float tractor_speed, long player_id, long article_id, long effect_id, long timestamp)
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

void Connection::HandleInventoryMove()
{
    if (m_Player->ShipIndex()->GetIsIncapacitated())
    {
        m_Player->SendVaMessageC(17, "Cannot manipulate inventory while incapacitated");
        return;
    }

	InvMove * Inventory = (InvMove *) m_RecvBuffer;
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
	if (m_Player->ShipIndex()->GetIsIncapacitated())
    {
		SendMessageString("Unable to move inventory while incapacitated.", 11);
		return;
    }

	//you can only move certain items from certain places (cannot equip from vault, ect)
	switch(InvMo.FromInv)
	{
        // From Cargo Inventory
		case 1:
			Source = *m_Player->ShipIndex()->Inventory.CargoInv.Item[InvMo.FromSlot].GetData();
			
			if (InvMo.ToInv == 1)	//cargo to cargo
			{
				Destination = *m_Player->ShipIndex()->Inventory.CargoInv.Item[InvMo.ToSlot].GetData();

				m_Player->CheckStack(InvMo.Num, &Source, &Destination);

				m_Player->ShipIndex()->Inventory.CargoInv.Item[InvMo.ToSlot].SetData(&Source);
				m_Player->ShipIndex()->Inventory.CargoInv.Item[InvMo.FromSlot].SetData(&Destination);
				m_Player->SendAuxShip();
			}
			else if(InvMo.ToInv == 2)	//equip from cargo
			{
                if (!m_Player->m_Equip[InvMo.ToSlot].CanEquip(&Source))
                {
					return;
                }

                //If we are moving ammo, they can stack if they are the same itemid
                if (m_Player->ShipIndex()->Inventory.AmmoInv.Item[InvMo.ToSlot].GetItemTemplateID() == Source.ItemTemplateID)
                {
                    Destination = *m_Player->ShipIndex()->Inventory.AmmoInv.Item[InvMo.ToSlot].GetData();
                    m_Player->CheckStack(InvMo.Num, &Source, &Destination);
                    m_Player->m_Equip[InvMo.ToSlot].Equip(&Source);
                }
                else
                {
				    Destination = m_Player->m_Equip[InvMo.ToSlot].Equip(&Source);
                }

				m_Player->ShipIndex()->Inventory.CargoInv.Item[InvMo.FromSlot].SetData(&Destination);
                m_Player->SendAuxShip();
			}
			else if(InvMo.ToInv == 3)	//cargo to vault
			{
				if (InvMo.ToSlot == -1)
				{
					return;
				}

				Destination = *m_Player->PlayerIndex()->SecureInv.Item[InvMo.ToSlot].GetData();

				m_Player->CheckStack(InvMo.Num, &Source, &Destination);

				m_Player->PlayerIndex()->SecureInv.Item[InvMo.ToSlot].SetData(&Source);
				m_Player->ShipIndex()->Inventory.CargoInv.Item[InvMo.FromSlot].SetData(&Destination);

				m_Player->SendAuxPlayer();
				m_Player->SendAuxShip();
			}
			else if(InvMo.ToInv == 4)	//selling cargo
			{
				if (Source.ItemTemplateID < 0)		// Keeps down XP hacking
					break;

				ItemBase * myItem = g_ItemBaseMgr->GetItem(Source.ItemTemplateID);

				if (!myItem || myItem->TechLevel() > 9) //not allowed to sell items of level 10 or above to vendors (fixes grail water XP hack).
					break;

				if (Source.StackCount <= InvMo.Num)
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
				
				sprintf(msg_buffer, "You have gained %ld credits", Source.Price * InvMo.Num);

                if (XP_earned > 0)
                {
                    char sub_buffer[40];
                    sprintf(sub_buffer, " and %ld trade experience", XP_earned);
                    strcat(msg_buffer, sub_buffer);
                }

				SendMessageString(msg_buffer, 3);
				SendClientSound("coin.wav");

                if (XP_earned > 0)
                {
				    sprintf(msg_buffer, "Item(s) eligible for");
				    AddTradeXP(msg_buffer, XP_earned);
                }

				m_Player->PlayerIndex()->SetCredits(m_Player->PlayerIndex()->GetCredits() + Source.Price * InvMo.Num);
				m_Player->SendAuxPlayer();

				m_Player->ShipIndex()->Inventory.CargoInv.Item[InvMo.FromSlot].SetData(&Destination);
				m_Player->SendAuxShip();
			}            
			else if(InvMo.ToInv == 11)	//spaceing item
			{
				Destination = Source;
				Destination.StackCount -= InvMo.Num;

				if (Destination.StackCount <= 0)
				{
					Destination = g_ItemBaseMgr->EmptyItem;
				}

				m_Player->ShipIndex()->Inventory.CargoInv.Item[InvMo.FromSlot].SetData(&Destination);
				m_Player->SendAuxShip();
			}
			// Result box in Manufacturing
            else if(InvMo.ToInv == 12 && (m_Player->ManuIndex()->GetMode() == 2 || m_Player->ManuIndex()->GetMode() == 3))
			{ 
				Destination = *m_Player->ManuIndex()->Target.Item[0].GetData();

				m_Player->ShipIndex()->Inventory.CargoInv.Item[InvMo.FromSlot].SetData(&Destination);
                m_Player->ManuIndex()->Target.Item[0].SetData(&Source);
				m_Player->SendAuxShip();
                m_Player->SendAuxManu();
			}
            // Control Interface box in Manufacturing
			else if(InvMo.ToInv == 14 && m_Player->ManuIndex()->GetMode() != 4)
			{
				Destination = *m_Player->ManuIndex()->Override.Item[0].GetData();

				m_Player->ShipIndex()->Inventory.CargoInv.Item[InvMo.FromSlot].SetData(&Destination);
                m_Player->ManuIndex()->Override.Item[0].SetData(&Source);
				m_Player->SendAuxShip();
                m_Player->SendAuxManu();
			}
			else if(InvMo.ToInv == 16)	//cargo to trade
			{
				Destination = *m_Player->ShipIndex()->Inventory.TradeInv.Item[InvMo.ToSlot].GetData();

				if (m_TradeID == -1)
				{
					return;
				}

				Player * targetp;
				targetp = m_ServerMgr.m_PlayerMgr.GetPlayer(m_TradeID);
				if (targetp)
				{
                    targetp->SendItemBase(Source.ItemTemplateID);
					m_Player->ShipIndex()->Inventory.CargoInv.Item[InvMo.FromSlot].SetData(&Destination);
					m_Player->SendAuxShip();
					m_Player->ShipIndex()->Inventory.TradeInv.Item[InvMo.ToSlot].SetData(&Source);
					m_Player->SendAuxShip(targetp);
					targetp->Connection()->TradeAction(0,6);
				}
			}
			break;

        // From Equip Inventory
		case 2:
			Source = *m_Player->ShipIndex()->Inventory.EquipInv.EquipItem[InvMo.FromSlot].GetItemData();

			if (InvMo.ToInv == 1)	//unequip item
			{
				Destination = *m_Player->ShipIndex()->Inventory.CargoInv.Item[InvMo.ToSlot].GetData();
				
                if (!m_Player->m_Equip[InvMo.FromSlot].CanEquip(&Destination))
                {
					return;
                }

                Source = m_Player->m_Equip[InvMo.FromSlot].Equip(&Destination);

				m_Player->ShipIndex()->Inventory.CargoInv.Item[InvMo.ToSlot].SetData(&Source);
				m_Player->SendAuxShip();
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

				Destination = *m_Player->ShipIndex()->Inventory.EquipInv.EquipItem[InvMo.ToSlot].GetItemData();

                //Moving two devices, just exchange them
                if (InvMo.FromSlot > 8 || InvMo.ToSlot > 8)
                {
                    Source = m_Player->m_Equip[InvMo.FromSlot].Equip(&Destination);
                    m_Player->m_Equip[InvMo.ToSlot].Equip(&Source);
                }

                //Now we are exchanging two weapon slots, the problem is that they can have ammo

                //If either weapon has ammo
                if (m_Player->ShipIndex()->Inventory.AmmoInv.Item[InvMo.ToSlot].GetItemTemplateID() != -2 ||
                    m_Player->ShipIndex()->Inventory.AmmoInv.Item[InvMo.FromSlot].GetItemTemplateID() != -2)
                {
                    //if the weapons are the same, swap the ammo in them, otherwise do nothing
                    if (Source.ItemTemplateID == Destination.ItemTemplateID)
                    {
			            Source = *m_Player->ShipIndex()->Inventory.AmmoInv.Item[InvMo.FromSlot].GetData();
			            Destination = *m_Player->ShipIndex()->Inventory.AmmoInv.Item[InvMo.ToSlot].GetData();

                        m_Player->CheckStack(InvMo.Num, &Source, &Destination);

                        m_Player->m_Equip[InvMo.FromSlot].Equip(&Destination);
                        m_Player->m_Equip[InvMo.ToSlot].Equip(&Source);
                    }
                    else
                    {
                        return;
                    }

                }
                else
                {
                    //If neither have ammo, just swap them normally
                    Source = m_Player->m_Equip[InvMo.FromSlot].Equip(&Destination);
                    m_Player->m_Equip[InvMo.ToSlot].Equip(&Source);
                }

				m_Player->SendAuxShip();
			}
			break;

        // From Vault Inventory
		case 3:
			Source = *m_Player->PlayerIndex()->SecureInv.Item[InvMo.FromSlot].GetData();

			if (InvMo.ToInv == 1)	//move from vault to cargo
			{
				Destination = *m_Player->ShipIndex()->Inventory.CargoInv.Item[InvMo.ToSlot].GetData();
				
				m_Player->CheckStack(InvMo.Num, &Source, &Destination);

				m_Player->ShipIndex()->Inventory.CargoInv.Item[InvMo.ToSlot].SetData(&Source);
				m_Player->PlayerIndex()->SecureInv.Item[InvMo.FromSlot].SetData(&Destination);

				m_Player->SendAuxShip();
				m_Player->SendAuxPlayer();
			}
			else if(InvMo.ToInv == 3)	//vault to vault
			{
				Destination = *m_Player->PlayerIndex()->SecureInv.Item[InvMo.ToSlot].GetData();
				m_Player->CheckStack(InvMo.Num, &Source, &Destination);

				m_Player->PlayerIndex()->SecureInv.Item[InvMo.ToSlot].SetData(&Source);
				m_Player->PlayerIndex()->SecureInv.Item[InvMo.FromSlot].SetData(&Destination);
				m_Player->SendAuxPlayer();
			}
			else if(InvMo.ToInv == 11)	//destroy item
			{
				Destination = Source;
				Destination.StackCount -= InvMo.Num;

				if (Destination.StackCount <= 0)
				{
					Destination = g_ItemBaseMgr->EmptyItem;
				}

				m_Player->PlayerIndex()->SecureInv.Item[InvMo.FromSlot].SetData(&Destination);
				m_Player->SendAuxPlayer();
			}
			break;

		// From Vendor Inventory
		case 4:

			Source = *m_Player->PlayerIndex()->VendorInv.Item[InvMo.FromSlot].GetData();

			if (InvMo.ToInv == 1)	//buy item
			{
				u64 Cost = Source.Price * InvMo.Num;
				if (m_Player->PlayerIndex()->GetCredits() < Cost)
				{
					m_Player->SendVaMessage("Insufficient credits!");
					return;
				}

				m_Player->PlayerIndex()->SetCredits(m_Player->PlayerIndex()->GetCredits() - Cost);
							
				//take losses on resales to vendors
				Cost /= 4;

				Source.StackCount = InvMo.Num;	// Trade this many items

				m_Player->CargoAddItem(&Source);
				m_Player->SendAuxPlayer();
				m_Player->SendAuxShip();
			}
			break;

        // From Loot Window
		case 6:
            {
                Object *obj = m_ObjectMgr->GetObjectFromID(m_Player->ShipIndex()->GetTargetGameID());
                if (obj)
                {
                    switch (obj->ObjectType())
                    {
                    case OT_HUSK:
                        m_Player->LootItem(InvMo.FromSlot);
                        break;
                                               
                    default:
                        LogMessage("Attempt to loot %s [%d]\n", obj->Name(), obj->GameID());
                    }
                }
                else
                {
                    LogMessage("Attempt to mine invalid object :%d\n", m_Player->ShipIndex()->GetTargetGameID());
                }
            }
			break;
	
        // Manufacturing Target
        case 12:
            if (InvMo.ToInv == 1 && (m_Player->ManuIndex()->GetMode() == 2 || m_Player->ManuIndex()->GetMode() == 3))
            {
			    Source = *m_Player->ManuIndex()->Target.Item[0].GetData();

				if (InvMo.ToSlot == -1)
				{
                    m_Player->CargoAddItem(&Source);
                    m_Player->ManuIndex()->Target.Item[0].Empty();
				    m_Player->SendAuxShip();
                    m_Player->SendAuxManu();
				}
				else if (InvMo.ToInv == 1)
			    {
				    Destination = *m_Player->ShipIndex()->Inventory.CargoInv.Item[InvMo.ToSlot].GetData();

				    m_Player->ShipIndex()->Inventory.CargoInv.Item[InvMo.ToSlot].SetData(&Source);
                    m_Player->ManuIndex()->Target.Item[0].SetData(&Destination);
				    m_Player->SendAuxShip();
                    m_Player->SendAuxManu();
                }
            }
            break;

        // Manufacturing Override
        case 14:
            if (InvMo.ToInv == 1 && m_Player->ManuIndex()->GetMode() != 4)
            {
			    Source = *m_Player->ManuIndex()->Override.Item[0].GetData();

				if (InvMo.ToSlot == -1)
				{
                    m_Player->CargoAddItem(&Source);
                    m_Player->ManuIndex()->Override.Item[0].Empty();
				    m_Player->SendAuxShip();
                    m_Player->SendAuxManu();
				}
			    else if (InvMo.ToInv == 1)
			    {
				    Destination = *m_Player->ShipIndex()->Inventory.CargoInv.Item[InvMo.ToSlot].GetData();

				    m_Player->ShipIndex()->Inventory.CargoInv.Item[InvMo.ToSlot].SetData(&Source);
                    m_Player->ManuIndex()->Override.Item[0].SetData(&Destination);
				    m_Player->SendAuxShip();
                    m_Player->SendAuxManu();
                }
            }
            break;

        // From Trade Window
		case 16:

			Source = *m_Player->ShipIndex()->Inventory.TradeInv.Item[InvMo.FromSlot].GetData();

			if (InvMo.ToInv == 1 && InvMo.ToSlot > 0)	//back to inventory
			{
				Destination = *m_Player->ShipIndex()->Inventory.CargoInv.Item[InvMo.ToSlot].GetData();

				if (m_TradeID == -1)
				{
					return;
				}

				Player * targetp;
				targetp = m_ServerMgr.m_PlayerMgr.GetPlayer(m_TradeID);
				if (targetp)
				{
                    targetp->SendItemBase(Source.ItemTemplateID);
					m_Player->ShipIndex()->Inventory.CargoInv.Item[InvMo.ToSlot].SetData(&Source);
					m_Player->SendAuxShip();
					m_Player->ShipIndex()->Inventory.TradeInv.Item[InvMo.FromSlot].SetData(&Destination);
					m_Player->SendAuxShip(targetp);
					targetp->Connection()->TradeAction(0,6);
				}

			}
			break;

        // From Mining Window
		case 18:
            {
                Object *obj = m_ObjectMgr->GetObjectFromID(m_Player->ShipIndex()->GetTargetGameID());
                if (obj)
                {
                    switch (obj->ObjectType())
                    {
                    case OT_HUSK:
                        m_Player->LootItem(InvMo.FromSlot);
                        break;
                        
                    case OT_HULK:
                    case OT_RESOURCE:
                        m_Player->MineResource(InvMo.FromSlot);
                        break;
                        
                    default:
                        LogMessage("Attempt to loot/mine %s [%d]\n", obj->Name(), obj->GameID());
                    }
                }
                else
                {
                    LogMessage("Attempt to mine invalid object :%d\n", m_Player->ShipIndex()->GetTargetGameID());
                }
            }
			break;
			
		default:
			LogMessage("UNRECOGNISED INVENTORY MOVE -- GameID: %ld From: %ld Slot: %ld To: %ld Slot %ld Number: %ld\n", InvMo.GameID,InvMo.FromInv,
				InvMo.FromSlot, InvMo.ToInv, InvMo.ToSlot, InvMo.Num);

			m_Player->SendVaMessage("UNRECOGNISED INVENTORY MOVE!\nPlease submit a bug report\n");
			return;
		}

}

void Connection::HandleItemState()
{
    ItemState * Data = (ItemState *) m_RecvBuffer;

    if (Data->Inventory == 2)
    {
        long ItemState = m_Player->ShipIndex()->Inventory.EquipInv.EquipItem[Data->ItemNum].GetItemState();

        if (Data->Enable == 1)
        {
            ItemState |= Data->BitMask;
        }
        else
        {
            ItemState &= ~Data->BitMask;
        }

        m_Player->ShipIndex()->Inventory.EquipInv.EquipItem[Data->ItemNum].SetItemState(ItemState);
        m_Player->SendAuxShip();
    }
    else
    {
		LogMessage("UNRECOGNISED ITEM STATE:\n");
        DumpBuffer(m_RecvBuffer, sizeof(ItemState));

		m_Player->SendVaMessage("UNRECOGNISED ITEM STATE!\nPlease submit a bug report\n");
    }
}

void Connection::HandleRequestTarget()
{
	RequestTarget * request = (RequestTarget *) m_RecvBuffer;
    Object *obj = m_ObjectMgr->GetObjectFromID(request->TargetID);

	//LogMessage("Received RequestTarget packet\n");

	if (m_ProspectWindow == true)
	{
		m_ProspectWindow = false;
		OpenInterface(1,0);
	}

	SendSetTarget(request->TargetID, -1);

	if (obj->ObjectType() == OT_MOB)
	{
		// Display just Combat Level
		m_Player->ShipIndex()->SetTargetThreatLevel(obj->Level());
	} else {
		if (obj->ObjectType() == OT_PLAYER) {
			// Display just Text
			m_Player->ShipIndex()->SetTargetThreatLevel(-1);
			m_Player->ShipIndex()->SetTargetThreat("Not Working Yet");
		} else {
			// Display nothing
			m_Player->ShipIndex()->SetTargetThreatLevel(-1);
			m_Player->ShipIndex()->SetTargetThreat("");
		}
	}

	m_Player->ShipIndex()->SetTargetThreatSound("success_1.wav");
	m_Player->ShipIndex()->SetTargetGameID(request->TargetID);
	m_Player->SendAuxShip();
    m_Player->BlankVerbs();

    if (obj)
    {
        obj->OnTargeted(m_Player);
    }
}

void Connection::HandleRequestTargetsTarget()
{
	RequestTarget * request = (RequestTarget *) m_RecvBuffer;
	Player *p = m_ServerMgr.m_PlayerMgr.GetPlayer(request->TargetID);
	
	if (p && p->Connection())
	{
		*((int *) &m_RecvBuffer[4]) = p->ShipIndex()->GetTargetGameID();
		HandleRequestTarget();
	}
	else
	{
		SendSetTarget(0, -1);
		m_Player->ShipIndex()->SetTargetGameID(-1);
		m_Player->SendAuxShip();
	}
}

bool Connection::CheckResourceLock(long object_id)
{
	char * player = 0; 
	Object *obj = m_ObjectMgr->GetObjectFromID(object_id);

	if (obj)
	{
		player = obj->CheckResourceLock();
	}

	if (player)
	{
		m_Player->SendVaMessage("%s is being mined by %s", obj->Name(), player);
		return false;
	}
	else
	{
		return true;
	}
}

long Connection::CurrentResourceTarget()
{
	if (m_ProspectWindow)
	{
		return (m_Player->ShipIndex()->GetTargetGameID());
	}
	else
	{
		return 0;
	}
}

void Connection::HandleVerbRequest()
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

	VerbRequest * pkt = (VerbRequest *) m_RecvBuffer;

	long subject_id = (long) ntohl(pkt->SubjectID);
	long object_id = (long) ntohl(pkt->ObjectID);

	//LogMessage("Received VerbRequest packet {SubjectID=%d, ObjectID=%d, Action=%d}\n",
	//    subject_id, object_id, pkt->Action);

	if (subject_id == m_AvatarID && pkt->Action == 1)
	{
        m_Player->UpdateVerbs(true);
	}
}

//This is just used to tweak HUD verbs for the /verb command
void Connection::HandleSendVerbRequest(char *param)
{
	short verbID = _httoi(param);

	//SendVerbUpdatePacket(m_Player->ShipIndex()->GetTargetGameID(), ATTRIBUTE_DIS_TOOFAR, 0, verbID, 0, 0, 0);	
}

void Connection::OpenInterface(long UIChange, long UIType)
{
	SetInterface set_interface;
	set_interface.UIChange = UIChange;
	set_interface.UIType = UIType;

	//LogMessage("Sending SetInterface packet\n");
	SendResponse(ENB_OPCODE_0066_OPEN_INTERFACE, (unsigned char *) &set_interface, sizeof(set_interface));
}

void Connection::CloseInterfaceIfTargetted(long target_id)
{
	if (m_Player->ShipIndex()->GetTargetGameID() == target_id)
	{
		m_ProspectWindow = false;
		OpenInterface(1,0);
	}
}

void Connection::CloseInterfaceIfOpen()
{
	if (m_ProspectWindow)
	{
		m_ProspectWindow = false;
		OpenInterface(1,0);
	}
}

void Connection::PointEffect(float *position, short effect_id, float scale)
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

	SendResponse(ENB_OPCODE_000A_POINT_EFFECT, point_data, sizeof(point_data));				
}

void Connection::SendAttackerUpdates(long mob_id, long update)
{
	unsigned char attacker_data[] =
	{
		0x00, 0x00, 0x00, 0x00,
		0x01,
		0x00, 0x00, 0x00, 0x00 //[5]
	};

	*((long *) &attacker_data[0]) = update;
	*((long *) &attacker_data[5]) = mob_id;

	SendResponse(ENB_OPCODE_008B_ATTACKER_UPDATES, attacker_data, sizeof(attacker_data));
}
void Connection::SendChangeBasset(ChangeBaseAsset *NewAsset)
{
	SendResponse(ENB_OPCODE_0026_CHANGE_BASE_ASSET, (unsigned char*) NewAsset, sizeof(ChangeBaseAsset));
}

void Connection::SendObjectLinkedEffect(short bitmask, long UID, long effectID, short effectDID, long effect_time)
{
	//send an effect linked to our ship
	ObjectToObjectEffect OBTOBE;

	OBTOBE.Bitmask = bitmask;
	OBTOBE.GameID = m_AvatarID;
	OBTOBE.TargetID = UID;
	OBTOBE.EffectDescID = effectDID;
	OBTOBE.Message = 0;
	OBTOBE.EffectID = effectID;
	OBTOBE.Duration = short(effect_time);
	OBTOBE.TimeStamp = GetNet7TickCount();

	SendObjectToObjectEffect(&OBTOBE);
}

void Connection::CheckObjectRanges()
{
    if (this && m_Player->ConnectionAvailable())
    {
        m_ObjectMgr->DisplayDynamicObjects(m_Player);
    }
}

void Connection::SendClientSound(char *sound_name, long channel, char queue)
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

	SendResponse(ENB_OPCODE_006A_CLIENT_SOUND, (unsigned char *) &packet[0], index);
}

void Connection::SendSetTarget(int game_id, int target_id)
{
	SetTarget set_target;
	set_target.GameID = game_id;
	set_target.TargetID = target_id;

	SendResponse(ENB_OPCODE_0019_SET_TARGET, (unsigned char *) &set_target, sizeof(set_target));
}

void Connection::SendRemoveEffect(int target_id)
{
	SendResponse(ENB_OPCODE_000F_REMOVE_EFFECT, (unsigned char *) &target_id, sizeof(int));
}

void Connection::TradeAction(long GameID, int Action)
{
	unsigned char buffer[5];

	*((long *) &buffer[0]) = GameID;
	*((char *) &buffer[4]) = (char) Action;

	SendResponse(ENB_OPCODE_001F_TRADE, buffer, 5);
}

void Connection::HandleAction()
{
	ActionPacket * myAction = (ActionPacket *) m_RecvBuffer;
	char message[128];
	Object *obj = m_ObjectMgr->GetObjectFromID(m_Player->ShipIndex()->GetTargetGameID());
	
    LogDebug("Action - ID: %d, Action: %d, Target: %d, OptVar: %d\n",myAction->GameID, myAction->Action, myAction->Target, myAction->OptionalVar);

	int x;

	switch (myAction->Action)
	{
		case 1:		//tractor
			if (m_Player->ShipIndex()->GetIsIncapacitated())
			{
				SendMessageString("Unable to tractor while incapacitated.", 11);
				return;
			}
			if (obj)
			{
				//shouldnt targeted ID be myAction->target??
                //you would thinkthat, but for some reason HULK action doesn't send the target ID...
                switch (obj->ObjectType())
                {
                case OT_FLOATING_ORE:
                    m_Player->LootItem(0);
                    break;

                case OT_HUSK:
                    if (m_ProspectWindow == false && CheckResourceLock(m_Player->ShipIndex()->GetTargetGameID()))
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
                    destination = m_SectorMgr->GetSectorID() * 10 + 1;
                    //LogMessage("TODO: Stations should have destinations in DASE: some sectors have more than 1 station.\n");
                }               
                m_SectorMgr->SectorServerHandoff(this, destination);
                break;
            }
		case 8:		//land
			if (m_Player->ShipIndex()->GetIsIncapacitated())
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

					m_Player->TerminateWarp();
					m_SectorMgr->SectorServerHandoff(this, obj->Destination());
					LogMessage("going to planet : %d\n", obj->Destination());
					return;
				}
				break;
			}
		case 9:		//fire all weapons
			{
				// if you are incapacited you cant equip items
				if (m_Player->ShipIndex()->GetIsIncapacitated())
				{
					SendMessageString("Unable to fire weapons while incapacitated.", 11);
					return;
				}
                m_Player->FireAllWeapons();
				break;
			}
		case 10:	//invite target to group
			{
				m_ServerMgr.m_PlayerMgr.GroupInvite(m_Player->GroupID(),m_AvatarID,myAction->Target);
				break;
			}
		case 11:	//accept group invitation
			{
				m_ServerMgr.m_PlayerMgr.AcceptGroupInvite(m_Player->GroupID(),m_AvatarID);
				break;
			}
		case 12:	//decline group invitation
			{
				m_ServerMgr.m_PlayerMgr.RejectGroupInvite(m_Player->GroupID(), m_AvatarID);
				break;
			}
		case 13:	//disban group
			{
				m_ServerMgr.m_PlayerMgr.DisbanGroup(m_Player->GroupID(), m_AvatarID);
				break;
			}
		case 14:	//leave group
			{
				m_ServerMgr.m_PlayerMgr.LeaveGroup(m_Player->GroupID(), m_AvatarID);
				break;
			}
		case 15:	//kick target from group
			{
				m_ServerMgr.m_PlayerMgr.KickFromGroup(m_Player->GroupID(), m_AvatarID, myAction->Target);
				break;
			}
		case 17:	//mine
			{
				if (m_Player->ShipIndex()->GetIsIncapacitated())
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
				if (m_Player->ShipIndex()->GetIsIncapacitated())
				{
					SendMessageString("Unable to gate while incapacitated.", 11);
					return;
				}
				if (obj != 0 && obj->ObjectType() == OT_STARGATE)
				{
                    m_Player->TerminateWarp();
					SendClientSound("1512_00_032Se.mp3",0,0);
					m_SectorMgr->GateActivate(this, myAction->Target);
				}
				break;
			}
		case 19:	//finish gate sequence
            {
                m_SectorMgr->SectorServerHandoff(this, m_StargateDestination);
            }
			break;

		//TODO: Remove confirm if trade changes
		case 20:	//trade
			{
				Player * targetp = m_ServerMgr.m_PlayerMgr.GetPlayer(myAction->Target);
				if (targetp)
				{
					if (targetp->Connection()->m_TradeID != -1)
					{
						m_Player->SendVaMessage("That player is already trading with someone!");
					}

					LogMessage("Clearing trade for players %x and %x\n",myAction->GameID,myAction->Target);

					for(x=0;x<6;x++) 
					{
						targetp->ShipIndex()->Inventory.TradeInv.Item[x].Empty();
						m_Player->ShipIndex()->Inventory.TradeInv.Item[x].Empty();
					}
					
					m_Player->SendAuxShip(targetp);
					targetp->SendAuxShip(m_Player);

					// TESTING
					if (!m_Player->Active() && myAction->Target != m_Player->GameID())
					{
						SendCreate(targetp->GameID(), 1, 0x4B06, CREATE_SHIP, 0, 0, 0);
						targetp->Connection()->SendCreate(m_Player->GameID(), 1, 0x4B06, CREATE_SHIP, 0, 0, 0);
					}
					// -------

					TradeAction(myAction->Target,0);						// Opens a trade window
					targetp->Connection()->TradeAction(myAction->GameID, 0);	// Open trade window for other player
					m_TradeID = myAction->Target;							// Set player tradeing with
					m_TradeConferm = 0;
					targetp->Connection()->m_TradeID = myAction->GameID;
					targetp->Connection()->m_TradeConferm = 0;
				}
			}
			break;
		case 21:	//confirm trade
			{
				Player * targetp;
				targetp = m_ServerMgr.m_PlayerMgr.GetPlayer(m_TradeID);
				m_TradeConferm = 1;
				TradeAction(0,3);

				if (targetp)
				{
					targetp->Connection()->TradeAction(0,5);
				}

				if (targetp && targetp->Connection()->m_TradeConferm == 1) 
				{
					LogMessage("Trade comfirned for players %x and %x\n",myAction->GameID,myAction->Target);

					//close windows and reset tradeIds
					TradeAction(m_TradeID,2);
					targetp->Connection()->TradeAction(myAction->GameID, 2);
					m_TradeID = -1;
					targetp->Connection()->m_TradeID = -1;

					//add and remove credits
					m_Player->PlayerIndex()->SetCredits(m_Player->PlayerIndex()->GetCredits() + targetp->ShipIndex()->GetTradeMoney() - m_Player->ShipIndex()->GetTradeMoney());
					m_Player->SendAuxPlayer();
					targetp->PlayerIndex()->SetCredits(targetp->PlayerIndex()->GetCredits() + m_Player->ShipIndex()->GetTradeMoney() - targetp->ShipIndex()->GetTradeMoney());
					targetp->SendAuxPlayer();

					//Add other player's items
					for(x=0;x<6;x++) 
					{
						m_Player->CargoAddItem(targetp->ShipIndex()->Inventory.TradeInv.Item[x].GetData());
					}
					m_Player->SendAuxShip();

					//add items to other player
					for(x=0;x<6;x++) 
					{
						targetp->CargoAddItem(m_Player->ShipIndex()->Inventory.TradeInv.Item[x].GetData());
					}
					targetp->SendAuxShip();

					//clear trade fr both players
					LogMessage("Clearing trade for players %x and %x\n",myAction->GameID,myAction->Target);
					for(x=0;x<6;x++) 
					{
						targetp->ShipIndex()->Inventory.TradeInv.Item[x].Empty();
						m_Player->ShipIndex()->Inventory.TradeInv.Item[x].Empty();
					}
					
					m_Player->SendAuxShip(targetp);
					targetp->SendAuxShip(m_Player);

					// Reset money
					targetp->ShipIndex()->SetTradeMoney(0);
					m_Player->ShipIndex()->SetTradeMoney(0);


					// TESTING
					if (!m_Player->Active() && myAction->Target != m_Player->GameID())		// if we are in a station use this
					{
						RemoveObject(targetp->GameID());
						targetp->Connection()->RemoveObject(m_Player->GameID());
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
				m_Player->SendVaMessage("ACTION 23. Target: %d",myAction->Target);
				break;				
			}
		case 24:	//trade money
			{
				// TODO: We need to log all player trades for the GMs
				Player * targetp;
				targetp = m_ServerMgr.m_PlayerMgr.GetPlayer(m_TradeID);
				if (targetp)
				{
					m_Player->ShipIndex()->SetTradeMoney(myAction->OptionalVar);
					m_Player->SendAuxShip(targetp);
					m_Player->SendAuxShip();
					m_Player->Connection()->TradeAction(m_TradeID,4);
					targetp->Connection()->TradeAction(m_TradeID,4);
				}
			}
			break;
		case 25:	//register
			if (obj)
			{
				//Starbase Registration
				if (obj->ObjectType() == OT_STATION && !m_RegistrationSent)
				{
                    SendClientSound("Reg_OK",0,0);
					sprintf(&message[0],"%s control: Registration Confirmed.", obj->Name());
					SendMessageString(message,5);

					m_Player->PlayerIndex()->SetRegistrationStarbase(obj->Name());
					m_Player->PlayerIndex()->SetRegistrationStarbaseSector(m_Player->PlayerIndex()->GetSectorName());
                    m_RegistrationSent = true;
				}
				break;
			}
		case 26:	// Jump Start
            if (!obj)
            {
                m_Player->SendVaMessage("Invalid JS target! SUBMIT BUG REPORT!");
                return;
            }

			if (obj->ObjectType() == OT_PLAYER)
			{
				m_Player->AbilityJumpStart(obj->GameID());
			}
			break;
		case 28:	//dock
			// if you are incapacited you cant equip items
			if (m_Player->ShipIndex()->GetIsIncapacitated())
			{
				SendMessageString("Unable to dock while incapacitated.", 11);
				return;
			}
			if (obj)
			{
				if (obj->ObjectType() == OT_STATION)
				{
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
                m_Player->SendEffect(obj->GameID(), 0, 10007, UID, GetNet7TickCount(), 3000);
                //by default when you scan something you cause a mission advance
                m_Player->MissionObjectScan(obj);
                break;
            }

		default:
			LogMessage("UNRECOGNIZED ACTION! ID: %d, Action: %d, Target: %d, OptVar: %d\n",myAction->GameID, myAction->Action, myAction->Target, myAction->OptionalVar);
			m_Player->SendVaMessage("UNRECOGNIZED ACTION! SUBMIT BUG REPORT!");
			m_Player->SendVaMessage("Action ID: %d, Action: %d, Target: %d, OptVar: %d",myAction->GameID, myAction->Action, myAction->Target, myAction->OptionalVar);
			break;
	}
}


void Connection::CancelTrade()
{
	Player * targetp;
	targetp = m_ServerMgr.m_PlayerMgr.GetPlayer(m_TradeID);
	if (targetp)
	{
		LogMessage("Trade cancelled for players %x and %x\n", m_Player->GameID(), m_TradeID);

		TradeAction(m_TradeID,1);									// Closes a trade window
		targetp->Connection()->TradeAction(m_Player->GameID(), 1);	// Closes trade window for other player
		m_TradeID = -1;												// No longer Tradeing
		targetp->Connection()->m_TradeID = -1;

		//return player's items
		for(int x=0;x<6;x++) 
		{
			m_Player->CargoAddItem(m_Player->ShipIndex()->Inventory.TradeInv.Item[x].GetData());
		}
		m_Player->SendAuxShip();

		//return other player's items
		for(x=0;x<6;x++) 
		{
			targetp->CargoAddItem(targetp->ShipIndex()->Inventory.TradeInv.Item[x].GetData());
		}
		targetp->SendAuxShip();

		//clear trade fr both players
		LogMessage("Clearing trade for players %x and %x\n", m_Player->GameID(), m_TradeID);
		for(x=0;x<6;x++) 
		{
			targetp->ShipIndex()->Inventory.TradeInv.Item[x].Empty();
			m_Player->ShipIndex()->Inventory.TradeInv.Item[x].Empty();
		}
					
		m_Player->SendAuxShip(targetp);
		targetp->SendAuxShip(m_Player);

		targetp->Connection()->m_TradeConferm = 0;
		m_TradeConferm = 0;

		// Reset money
		targetp->ShipIndex()->SetTradeMoney(0);
		m_Player->ShipIndex()->SetTradeMoney(0);

		// TESTING
		if (!m_Player->Active() && m_TradeID != m_Player->GameID())
		{
			RemoveObject(targetp->GameID());
			targetp->Connection()->RemoveObject(m_Player->GameID());
		}
		// ----
	}
}

void Connection::SendResourceLevel(long target_id)
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
		
		SendResponse(ENB_OPCODE_001B_AUX_DATA, aux_data, sizeof(aux_data));
	}
}

/*
void Connection::SendResourceContentsAUXForHulk(Object *obj) 
{
    bool contents_already_sent;
    
    if (obj)
    {
        contents_already_sent = obj->GetIndex(m_Player->ResourceSendList());
        
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
            
            SendResponse(ENB_OPCODE_001B_AUX_DATA, aux_data, sizeof(aux_data));
            obj->SetIndex(m_Player->ResourceSendList());
        }
    }
}*/

//TODO: make this part of AUX handler.
void Connection::SendResourceContentsAUX(Object *obj) 
{
    bool contents_already_sent;

    if (obj)
    {
        contents_already_sent = obj->GetIndex(m_Player->ResourceSendList());
        
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
                    m_Player->SendItemBase(slot->item->ItemTemplateID());
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
            
            SendResponse(ENB_OPCODE_001B_AUX_DATA, packet, index);
            obj->SetIndex(m_Player->ResourceSendList());
        }
    }
}

void Connection::SendCameraControl(long Message, long GameID)
{
	CameraControl data;
	data.Message = Message;
	data.GameID = GameID;

	//LogMessage("Sending CameraControl packet\n");
	SendResponse(ENB_OPCODE_0092_CAMERA_CONTROL, (unsigned char *) &data, sizeof(data));
}

bool Connection::MatchOptWithParam (char *option, char *arg, char *&param, bool &msg_sent)
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
			m_Player->SendVaMessage("Missing arg for option %s", option);
			msg_sent = true;
		}
	}

	return false;
}

void Quat4fMul(float vector1[], float vector2[], float *vector3)
{
    float x1,y1,z1,w1,x2,y2,z2,w2;

	w1 = vector1[3];
	x1 = vector1[0];
    y1 = vector1[1];
    z1 = vector1[2];

	w2 = vector2[3];
    x2 = vector2[0];
    y2 = vector2[1];
    z2 = vector2[2];

    vector3[3] = (w1*w2 - x1*x2 - y1*y2 - z1*z2);
    vector3[0] = (w1*x2 + x1*w2 + y1*z2 - z1*y2);
    vector3[1] = (w1*y2 - x1*z2 + y1*w2 + z1*x2);
    vector3[2] = (w1*z2 + x1*y2 - y1*x2 + z1*w2);

}

//Object
void Quat4fMulInv(float vector1[], float vector2[], float *vector3)
{
    float x1,y1,z1,w1,x2,y2,z2,w2;

	w1 = vector1[3];
    x1 = vector1[0];
    y1 = vector1[1];
    z1 = vector1[2];
    
	w2 = vector2[3];
    x2 = - vector2[0];
    y2 = - vector2[1];
    z2 = - vector2[2];

    vector3[3] = (w1*w2 - x1*x2 - y1*y2 - z1*z2);
    vector3[0] = (w1*x2 + x1*w2 + y1*z2 - z1*y2);
    vector3[1] = (w1*y2 - x1*z2 + y1*w2 + z1*x2);
    vector3[2] = (w1*z2 + x1*y2 - y1*x2 + z1*w2);
}

void Connection::HandleEquipUse()
{
	EquipUse *myUse = (EquipUse *) m_RecvBuffer;

    m_Player->m_Equip[myUse->InvSlot].ManualActivate();
}

void Connection::SendClientDamage(long target_id, long source_id, float damage, float modifier, long type, long inflicted)
{
	int index = 0;
	unsigned char packet[32];

	AddData(packet, damage, index);
	AddData(packet, modifier, index);
	AddData(packet, type, index);
	AddData(packet, inflicted, index);
	AddData(packet, source_id, index);
	AddData(packet, target_id, index);

	SendResponse(ENB_OPCODE_0064_CLIENT_DAMAGE, packet, index);
}

void Connection::Dialog(char *Stringd, int Type)
{
	int Index = 0;
	unsigned char Data[75];

	*((short*) &Data[Index]) = strlen(Stringd) + 1;		// String Size
	Index+=2;
	*((long*) &Data[Index]) = Type;						// Type
	Index+=4;
	memcpy(&Data[Index], Stringd, strlen(Stringd) + 1);		// copy the string
	Index+=strlen(Stringd)+1;

	SendResponse(0x62, (unsigned char *) &Data, Index);	
}

bool Connection::HandleResetMaster()
{
	//simulate a connection dropout for test purposes
	ResetConnection();
	return true;
}

void Connection::HandleClientChat()
{
	bool success = false;
	bool msg_sent = false;

	ClientChat * chat = (ClientChat *) m_RecvBuffer;
	char * types[] =
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
		m_ServerMgr.m_PlayerMgr.ChatSendEveryone(chat->GameID, chat->String);
	}

	if ((chat->Type == 1) && m_Player)
	{
		if (m_Player->GroupID() != -1)
			m_ServerMgr.m_PlayerMgr.GroupChat(m_Player->GroupID(),m_Player->Name(), chat->String);
		else
			m_Player->SendVaMessage("Error: You are not in a group!");
	}

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
	if (chat->String[0] == '/' && chat->String[1] == '/' && chat->String[2] != 0 && m_Player->AdminLevel() >= 50)
	{
		char *param;
		char *pch = (char*)_alloca(strlen(&chat->String[2]));//copy to stack to avoid heap fragment
		strcpy(pch, &chat->String[2]);
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
						m_Player->SendVaMessage("Syntax: //adduser <username> <password> <access>");
                        return;
                    }

					if (g_AccountMgr->AddUser(Username, Password, Access))
                    {
					    m_Player->SendVaMessage("Account %s / %s / %s Created", Username, Password, Access);
                    }
                    else
                    {
					    m_Player->SendVaMessage("Account creation failed!");
                    }

                    msg_sent = true;
				}
            }
			break;

			case 'r':
			{
				if (strcmp(pch, "rstations") == 0)
				{
					// Remove old Data
					delete [] m_ServerMgr.m_StationList;
					m_ServerMgr.m_StationList = NULL;
					char SectorID[10];

					StationLoader StationsReload;
					StationsReload.LoadStations();

					m_ServerMgr.m_StationList = StationsReload.GetStationList();

					sprintf(SectorID, "%d", m_SectorID);
					HandleWormholeRequest(SectorID);

					m_Player->SendVaMessage("Reloading Stations...");
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
						m_Player->SendVaMessage("Syntax: //setpassword <username> <password>");
                        return;
                    }

					g_AccountMgr->ChangePassword(Username, Password);
					m_Player->SendVaMessage("Account %s password has been set to %s", Username, Password);
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
					    m_Player->SendVaMessage("Player `%s` not found", param);
					    return;
                    }

					m_Player->SendVaMessage("Access level for `%s` is %d", param, target->AdminLevel());
					msg_sent = true;
					success = true;	
				}

				if (MatchOptWithParam("gmsetaccess", pch, param, msg_sent))
				{
					char *Username = strtok(param, " ");
					char *Access = strtok(NULL, " ");

					if (!Username || !Access)
                    {
						m_Player->SendVaMessage("Syntax: //gmsetaccess <playername> <password>");
                        return;
                    }

                    Player * target = g_PlayerMgr->GetPlayer(Username);
                    if (!target)
                    {
					    m_Player->SendVaMessage("Player `%s` not found", Username);
					    return;
                    }

                    long new_access = atoi(Access);

					// Can't promote over your level
					if (new_access > m_Player->AdminLevel())
						new_access = m_Player->AdminLevel();

					// Can't demote someone above your access
					if (g_AccountMgr->GetAccountStatus(target->AccountUsername()) > m_Player->AdminLevel())
					{
						m_Player->SendVaMessage("Can't change access for player above your access level");
						msg_sent = true;
					}
                    else
                    {
						g_AccountMgr->SetAccountStatus(target->AccountUsername(), new_access);
						m_Player->SendVaMessage("Player %s has access set to %d", Username, new_access);
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
						Player * TargetP = m_ServerMgr.m_PlayerMgr.GetPlayer(Username);

						if (!TargetP) {
							m_Player->SendVaMessage("Player %s is not online", Username);
							msg_sent = true;
							success = true;
						} else {
							TargetP->PlayerIndex()->RPGInfo.SetSkillPoints(SkillPoints);
							TargetP->LevelUpForSkills();
							TargetP->UpdateSkills();
							TargetP->SendAuxPlayer();
							TargetP->SendVaMessage("You have gotten %d SkillPoints", SkillPoints);
							m_Player->SendVaMessage("You have gave %s %d SkillPoints", Username, SkillPoints);
							success = true;
							msg_sent = true;
						}
					} else {
						m_Player->SendVaMessage("Syntax: //gmskillpoints <playername> <skillpoints>");
						success = false;
						msg_sent = true;
					}
				}

				if (MatchOptWithParam("gmenableskills", pch, param, msg_sent))
				{
					char *PlayerName = strtok(param, " ");

					if (!PlayerName) {
						m_Player->SendVaMessage("Syntax: //gmenableskills <playername>");
						msg_sent = true;
						success = false;
					} else {
						Player * TargetP = m_ServerMgr.m_PlayerMgr.GetPlayer(PlayerName);

						if (!TargetP) {
							m_Player->SendVaMessage("Player %s is not online", PlayerName);
							msg_sent = true;
							success = true;
						} else {
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
							m_Player->SendVaMessage("Player %s had skills enabled", PlayerName);
						}
					}
				}

				if (MatchOptWithParam("gmplayerlevel", pch, param, msg_sent))
				{
					char *Username = 0, *SLevel = 0;
					int Level = 0;
					
					Username = strtok(param, " ");
					if (Username) {
						SLevel = strtok(NULL, " ");
						if (SLevel)
							Level = atoi(SLevel);
					}

					if (SLevel && Level <= 50)
					{
						Player * TargetP = m_ServerMgr.m_PlayerMgr.GetPlayer(Username);

						if (!TargetP) {
							m_Player->SendVaMessage("Player %s is not online", Username);
							msg_sent = true;
							success = true;
						} else {
							TargetP->PlayerIndex()->RPGInfo.SetCombatLevel(Level);
							TargetP->PlayerIndex()->RPGInfo.SetTradeLevel(Level);
							TargetP->PlayerIndex()->RPGInfo.SetExploreLevel(Level);
							TargetP->LevelUpForSkills();
							TargetP->UpdateSkills();
							TargetP->SendAuxPlayer();
                    
							TargetP->SendVaMessage("Combat, Explore and Trade LVLs set to %d",Level);
							m_Player->SendVaMessage("Player %s leveled to %d", Username, Level);
							msg_sent = true;
							success = true;
						}
					} 
                    else 
                    {
						m_Player->SendVaMessage("Syntax: //gmplayerlevel <playername> <level 1-50>");
						msg_sent = true;
						success = false;
					}
				}
			}

		}


	}

	// This is for normal commands
	if ((chat->String[0] == '/') &&
		(chat->String[1] != 0) && (!msg_sent || !success))
	{
		char *param;
		char *pch = (char*)_alloca(strlen(&chat->String[1]));//copy to stack to avoid heap fragment
		strcpy(pch, &chat->String[1]);
		int retval = 0;
		switch(*pch)
		{
        case 'a':
            {
				if (strcmp(pch, "authlevel") == 0)
				{
                    m_Player->SendVaMessage("Authentication Level - Num: %d", m_Player->AdminLevel());
					msg_sent = true;
					success = true;
				}
                if (m_Player->AdminLevel() >= 80 && MatchOptWithParam("altweapon", pch, param, msg_sent)) //no error checking - server devs only
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

                    //m_Player->ChangeMountBoneName(weapon_id, bone_id);
					msg_sent = true;
					success = true;
                }
                if (m_Player->AdminLevel() >= 80 && MatchOptWithParam("altname", pch, param, msg_sent)) //no error checking - server devs only
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
                        m_Player->ChangeMountBoneName(weapon_id, cmd);
                        m_Player->SendVaMessage("Change weapon id #%d to %s", weapon_id, cmd);
                    }

                    
					msg_sent = true;
					success = true;
                }
            }
            break;

		case 'b':
			{
				//BUFF for future reference

				/*				
				if (MatchOptWithParam("buff", pch, param, msg_sent))
				{
					AuxShip::Buff myBuff;
					memset(&myBuff,0,sizeof(AuxShip::Buff));

					myBuff.Flags[0] = (char) 0xF2;
					myBuff.Flags[1] = (char) 0x01;
					sprintf(myBuff.BuffType,"Damage_Tactics");
					myBuff.BuffRemovalTime = GetNet7TickCount() + 100000;
					myBuff.IsPermanent = false;
					myBuff.ElementsFlags = 0x12;

					myBuff.Elements[0].Flags[0] = (char) 0xF2;
					myBuff.Elements[0].Flags[1] = (char) 0x01;
					myBuff.Elements[0].ExpirationTime = GetNet7TickCount() + 10000;
					myBuff.Elements[0].IsActive = true;
					sprintf(myBuff.Elements[0].SourceObject, "Lesser Psionic Shield");
					strcpy(myBuff.Elements[0].SourceEntity, m_AuxShip.m_Name);
					myBuff.Elements[0].Magnitude = atoi(param);

					m_Player->SendVaMessage("Sending buff 0!\n");
					m_AuxShip.SetBuff(this,&myBuff,0);
					msg_sent = true;
				}
				*/
				
			}
			break;
		case 'c':
			{
                if (MatchOptWithParam("ccamera", pch, param, msg_sent))
				{
                    Object *obj = m_ObjectMgr->GetObjectFromID(m_Player->ShipIndex()->GetTargetGameID());
                    if (obj)
                    {
                        m_Player->SendVaMessage("Camera control: %x, %x", ntohl(atoi(param)), ntohl(obj->GameID()) );
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
                    m_ServerMgr.m_AccountMgr->ChangePassword(m_Player->AccountUsername(), param);
                    m_Player->SendVaMessage("Your password has been changed to: `%s`", param);
                    msg_sent = true;
                }
                else if (MatchOptWithParam("createitem", pch, param, msg_sent))
                {
                    if (atoi(param) == 3052 && strcmp(m_Player->AccountUsername(), "Tienbau")) //temp lockout for testing
                    {
                        m_Player->SendVaMessage("Unable to create.");
                    }
                    else if (m_Player->AdminLevel() >= 30)      // Beta to Admin
                    {  
                        int FreeSlot = m_Player->GetCargoSlotFromItemID(0, -1);;
                        
                        if (FreeSlot == -1) 
                        {
                            m_Player->SendVaMessage("No free slots in inventory free up space");
                        } 
                        else 
                        {
                            _Item myItem = g_ItemBaseMgr->EmptyItem;

							char *ItemIDs = strtok(param, " ");
							char *NumberS;
							int Number;

							if (ItemIDs) 
                            {
								NumberS = strtok(NULL, " ");
								if (NumberS)
                                {
									Number = atoi(NumberS);
                                }
								else
                                {
									Number = 1;
                                }
							} 
                            else 
                            {
								Number = 1;
							}
                            
                            myItem.ItemTemplateID = atoi(ItemIDs);
                            myItem.StackCount = Number;
                            myItem.Price = 200;
                            myItem.Quality = 1;
                            myItem.Structure = 1;
                            
                            m_Player->CargoAddItem(&myItem);
                            m_Player->SendAuxShip();
                            m_Player->SendVaMessage("Item %d Created %d", atoi(ItemIDs), Number);
                        }
                        msg_sent = true;
                    } 
                    else 
                    {
                        m_Player->SendVaMessage("GM Only command.");
                    }
                } 
				else if (MatchOptWithParam("createcredits", pch, param, msg_sent))
				{
					if (m_Player->AdminLevel() >= 30)      // Beta to Admin
					{
						m_Player->PlayerIndex()->SetCredits(m_Player->PlayerIndex()->GetCredits() + _atoi64(param));
						m_Player->SendAuxPlayer();
					} 
					else 
					{
						m_Player->SendVaMessage("GM Only command.");
					}
					msg_sent = true;
				} 
				/*
				else if (MatchOptWithParam("createmission", pch, param, msg_sent))
				{
					if (m_Player->AdminLevel() >= 50)      // GM to Admin
					{
						m_ServerMgr.m_Missions.givePlayerMission(this, atoi(param));
						msg_sent = true;
					} 
					else 
					{
						m_Player->SendVaMessage("GM Only command.");
					}
				} 
				*/
				else if (MatchOptWithParam("createmob", pch, param, msg_sent))
				{
                    if (m_Player->AdminLevel() >= 50)      // GM -> admin
                    {  
                        success = HandleMobCreateRequest(param);
                        msg_sent = true;
                    }
                    else
                    {
                        m_Player->SendVaMessage("GM Only command.");
                    }
				}
                else if (MatchOptWithParam("create", pch, param, msg_sent))
				{
					success = HandleObjCreateRequest(param);
					msg_sent = true;
				}
                else if (0 == strcmp("checklock", pch))
                {
                    Object *obj = m_ObjectMgr->GetObjectFromID(m_Player->ShipIndex()->GetTargetGameID());
                    if (obj)
                    {
                        char *player = obj->CheckResourceLock();
                        if (player)
                        {
                            m_Player->SendVaMessage("Currently being mined/looted by %s", player);
                        }
                        else
                        {
                            m_Player->SendVaMessage("Not being looted.");
                        }
                    }
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
				&& m_Player->AdminLevel() >= 80)
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
                    Object *obj = m_ObjectMgr->AddNewObject(OT_DECO);
                    obj->SetPosition(m_Player->Position());
                    obj->SetBasset(basset);
                    obj->SetSignature(30000.0f);
                    obj->SetOrientation(0,0,0,0);
                    obj->SetScale(1.0f);
                    m_CurrentDecoObj = obj;
                    LogMessage("Deco created at:\n %.2f, %.2f %.2f Basset %d\n", obj->PosX(), obj->PosY(), obj->PosZ(), basset);
                }
                else
                {
                    LogMessage("/deco <basset>");
                }
                msg_sent = true;
                success = true;
            }
			break;
		case 'e' :
			{
				if (strcmp(pch, "enableskills") == 0)
				{
                    u32 Availability[4] = {4,0,0,1};
                    for (int i=0;i<64;i++)
                    {
                        if (m_Player->PlayerIndex()->RPGInfo.Skills.Skill[i].GetAvailability()[0] == 3)
                        {
                            m_Player->PlayerIndex()->RPGInfo.Skills.Skill[i].SetAvailability(Availability);
                        }
                        m_Player->SendAuxPlayer();
                    }
					msg_sent = true;
				}
				
				if (m_Player->AdminLevel() >= 30)      // Beta to Admin
				{	
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
                            WarpEffect.GameID = chat->GameID;
                            WarpEffect.Duration = Length;
                            WarpEffect.TimeStamp = GetNet7TickCount();
                            
                            m_Player->SendObjectEffectToRangeList(&WarpEffect);
                            
                            m_Player->SendVaMessage("Send Effect %d Disc: %d for %dms", WarpEffect.EffectID, EffectDID, Length);
                        }
                        else
                        {
                            m_Player->SendVaMessage("/effect <effect_desc_id> <length>");
                        }
                        
                        success = true;
                        msg_sent = true;
                    } 
					else if (MatchOptWithParam("effecto", pch, param, msg_sent))	
					{
                        char *cmd = strtok(param, " ");
                        int EffectDID = 0, Length, count = 0;
                        float scale = 0.0f, speedup = 0.0f;
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
                                scale = atof(cmd);
                                bitmask += 0x40;
                                cmd = strtok(NULL, " ");
                                if (cmd)
                                {
                                    speedup = atof(cmd);
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
                            OBTOBE.GameID = chat->GameID;
                            OBTOBE.TargetID = m_Player->ShipIndex()->GetTargetGameID();
                            OBTOBE.EffectDescID = EffectDID;// 0x00BF;
                            OBTOBE.Message = 0;
                            OBTOBE.EffectID = m_SectorMgr->GetSectorNextObjID();
                            OBTOBE.Duration = Length;
                            OBTOBE.TimeStamp = GetNet7TickCount();
                            OBTOBE.Scale = scale;
                            OBTOBE.Speedup = speedup;
                            
                            m_Player->SendObjectToObjectEffectToRangeList(&OBTOBE);
                            m_Player->SendVaMessage("Send Effect %d Disc: %d to Object", OBTOBE.EffectID, EffectDID);
                        }
                        else
                        {
                            m_Player->SendVaMessage("/effecto <effect_desc_id> <length>");
                        }
                        success = true;
                        msg_sent = true;
					} 
					else if (MatchOptWithParam("effects", pch, param, msg_sent)) 
					{
						int EffectID = atoi(strtok(param,  " "));
					
						SendResponse(ENB_OPCODE_000F_REMOVE_EFFECT, (unsigned char *)&EffectID, sizeof(int));

						m_Player->SendVaMessage("Stopping Effect %d", EffectID);
						success = true;
						msg_sent = true;
					}
				}
			}
			break;
		case 'f':
			if (m_Player->AdminLevel() >= 30)      // Beta to Admin
			{
				if (strcmp(pch, "flushinv") == 0)
				{
					// Flush invtory clean!
					for(int Slot=0;Slot<m_Player->ShipIndex()->Inventory.GetCargoSpace();Slot++)
					{
						m_Player->ShipIndex()->Inventory.CargoInv.Item[Slot].Empty();
					}

					m_Player->SendAuxShip();
					m_Player->SendVaMessage("Your inventory is now flushed!");

					success = true;
					msg_sent = true;
				}
			}

			if (m_Player->AdminLevel() >= 50)      // GM to Admin
			{
				if (strcmp(pch,"fetch") == 0) //no args
				{
					success = HandleFetchRequest();
					msg_sent = true;
				}

                if (MatchOptWithParam("find", pch, param, msg_sent))
			    {
                    Player * target = m_ServerMgr.m_PlayerMgr.GetPlayer(param);
                    if (target)
                    {
                        m_Player->SendVaMessage("Player found! Name: `%s` GameID: %x Account: `%s`",target->Name(), target->GameID(),target->AccountUsername());
                    }
                    else
                    {
                        m_Player->SendVaMessage("Player `%s` not found!", param);
                    }
                    msg_sent =  true;
			    }
			}
			
			if (strcmp(pch, "face") == 0)
			{
				success = HandleFaceRequest(m_Player->ShipIndex()->GetTargetGameID());
				msg_sent = true;
			}
			else if (strcmp(pch, "fgps") == 0)
			{
				ProcessConfirmedActionOffer();
				success = true;
				msg_sent = true;
			}
			break;

		case 'g':
			if (strcmp(pch, "goto") == 0)
			{
				success = HandleGotoRequest();
				msg_sent = true;
			}
			else if (strcmp(pch, "groupc") == 0)
			{
				m_Player->SendVaMessage("Group Count = %d", m_ServerMgr.m_PlayerMgr.GetMemberCount(m_Player->GroupID()));
				msg_sent = true;
			}            
			else if (strcmp(pch, "groupid") == 0)
			{
				m_Player->SendVaMessage("Group ID = %d", m_Player->GroupID());
				msg_sent = true;
			}            
			else if (strcmp(pch, "gameid") == 0)
			{
				m_Player->SendVaMessage("Game ID = %x", m_AvatarID);
				msg_sent = true;
			}            
			break;

        case 'h':
            if (strcmp(pch, "hijack") == 0 && m_Player->ShipIndex()->GetTargetGameID() > 0)
            {
                if (m_Player->AdminLevel() >= 50)      // GM to Admin
                {
                    success = HandleObjectHijack();
                    msg_sent = true;
                }
                else
                {
                    m_Player->SendVaMessage("You do not have hijack permission.\n" );
                }
            }
            if (strcmp(pch, "heading") == 0)
            {
                float *heading = m_Player->Heading();
                m_Player->SendVaMessage("Heading: %.5f %.5f %.5f", heading[0], heading[1], heading[2]);
                msg_sent = true;
                success = true;
            }
            if (MatchOptWithParam("ht", pch, param, msg_sent))
            {
                int head = atoi(strtok(param,  " "));
                int body = atoi(strtok(NULL, " "));
                int gender = atoi(strtok(NULL, " "));
                m_Player->SendVaMessage("Current Head: %d Body: %d G: %d", m_Player->Database()->avatar.head_type, m_Player->Database()->avatar.body_type, 
                    m_Player->Database()->avatar.gender);

                m_Player->Database()->avatar.head_type = head;
                m_Player->Database()->avatar.body_type = body;
                m_Player->Database()->avatar.gender = gender;
                success = true;
                msg_sent =  true;
            }
            
            break;

		case 'i':
			if (MatchOptWithParam("invite", pch, param, msg_sent))
			{
                long GameID = m_ServerMgr.m_PlayerMgr.GetGameIDFromName(param);
                if (GameID == -1)
                {
                    m_Player->SendVaMessage("Could not find player %s",param);
                    return;
                }

				if (GameID == m_AvatarID)
				{
					if (!(m_Player->AdminLevel() >= 50))      // GM to Admin
					{
					    m_Player->SendVaMessage("Cannot group with yourself!");
					    return;
					}
				}
				m_ServerMgr.m_PlayerMgr.GroupInvite(m_Player->GroupID(),m_AvatarID, GameID);
				msg_sent = true;
			}
			break;

		case 'k':
			if (m_Player->AdminLevel() >= 50)      // GM to Admin
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
				m_ServerMgr.m_PlayerMgr.LeaveGroup(m_Player->GroupID(),m_AvatarID);
				msg_sent = true;
			}
			else if (MatchOptWithParam("level", pch, param, msg_sent))
            {
                if (m_Player->AdminLevel() >= 30)      // Beta to Admin
                {
                    
                    if (atoi(param) < 0 || atoi(param) > 50)
                    {
                        m_Player->SendVaMessage("0 <= Level <= 50");
                        return;
                    }
                    
                    m_Player->PlayerIndex()->RPGInfo.SetCombatLevel(atoi(param));
                    m_Player->PlayerIndex()->RPGInfo.SetTradeLevel(atoi(param));
                    m_Player->PlayerIndex()->RPGInfo.SetExploreLevel(atoi(param));
					m_Player->PlayerIndex()->RPGInfo.SetSkillPoints(atoi(param) * 10);
                    m_Player->LevelUpForSkills();
                    m_Player->UpdateSkills();
                    m_Player->SendAuxPlayer();
                    
                    m_Player->SendVaMessage("Combat, Explore and Trade LVLs set to %d",atoi(param));
                    m_Player->SendVaMessage("Additionally, you now have %d skillpoints",atoi(param) * 10);
                    msg_sent = true;
                }
                else
                {
                    m_Player->SendVaMessage("/level not available");
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
			break;

		case 'o':
            if (strcmp(pch, "ori") == 0)
            {
                float *ori = m_Player->Orientation();
                m_Player->SendVaMessage("Orientation: %.5f %.5f %.5f %.5f", ori[0], ori[1], ori[2], ori[3]);
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
					m_Player->SendVaMessage("OpenInterface (%d,%d):", atoi(a), atoi(b));
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
					m_Player->SendVaMessage("/ore base ore energy,base ore time,base tractor speed");
					m_Player->SendVaMessage("Base Ore Energy: %d", m_BaseOreEnergy);
					m_Player->SendVaMessage("Base Ore Time: %.2f", m_BaseOreTime);
					m_Player->SendVaMessage("Tractor Speed base: %.2f", m_BaseOreTracSpeed);
				}
				else
				{
					m_Player->SendVaMessage("New base ore energy: %d", base_energy);
					m_Player->SendVaMessage("New base ore time: %.2f", base_time);
					m_Player->SendVaMessage("New tractor speed base: %.2f", base_speed);
					
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
				m_Player->SendVaMessage("ObjectID = 0x%08x",m_Player->ShipIndex()->GetTargetGameID());
				if (m_Player->ShipIndex()->GetTargetGameID() != -1)
                {
                    Object *obj = m_ObjectMgr->GetObjectFromID(m_Player->ShipIndex()->GetTargetGameID());
                    if (obj) m_Player->SendVaMessage("%s @ %.2f %.2f %.2f", obj->Name(), obj->PosX(), obj->PosY(), obj->PosZ());
                }
				msg_sent = true;					
				success = true;
			}
			break;

		case 'r':
			if (strcmp(pch, "reffect") == 0)
			{
				//m_SectorMgr->m_EffectManager.RemoveEffectsByPlayer(m_AvatarID);
				m_Player->SendVaMessage("Removed all effects on you!");
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
                for(int Slot=0;Slot<m_Player->ShipIndex()->Inventory.GetCargoSpace();Slot++)
                {
                    m_Player->ShipIndex()->Inventory.CargoInv.Item[Slot].Empty();
                }

                m_Player->SendVaMessage("Cargo slots = %d", m_Player->ShipIndex()->Inventory.GetCargoSpace());

                if (m_Player->ShipIndex()->Inventory.GetCargoSpace() < 20)
                {
                     m_Player->ShipIndex()->Inventory.SetCargoSpace(20);
                     for(int i=0;i<m_Player->ShipIndex()->Inventory.GetCargoSpace();i++)
                     {
                         m_Player->ShipIndex()->Inventory.CargoInv.Item[i].Empty();
                     }
                     m_Player->SendVaMessage("Restored cargo slots = %d", m_Player->ShipIndex()->Inventory.GetCargoSpace());
                }

                m_Player->SendAuxShip();

                success = true;
                msg_sent = true;
            }
			/*
			else if (strcmp(pch, "refreshmissionlist") == 0)
			{
				if (m_Player->AdminLevel() >= 50)      // GM to Admin
				{
					LogMessage("Refreshing mission list\n");
					m_ServerMgr.m_Missions.Initialize();
					success = true;
				}
			}
			*/
			else if (strcmp(pch, "resetmaster") == 0)
			{
				success = HandleResetMaster();
				msg_sent = true;
			}
            else if (strcmp(pch, "resetmounts") == 0)
            {
                m_Player->ResetWeaponMounts();
                success = true;
                msg_sent = true;
            }
			break;

		case 's' :
			if (MatchOptWithParam("scale", pch, param, msg_sent))
			{
				success = HandleScaleRequest(param);
				msg_sent = true;
			}

			if (MatchOptWithParam("skillpoints", pch, param, msg_sent))
			{
                if (m_Player->AdminLevel() >= 50)      // GM to Admin
                {
					m_Player->PlayerIndex()->RPGInfo.SetSkillPoints(atoi(param));
					m_Player->LevelUpForSkills();
					m_Player->UpdateSkills();
					m_Player->SendAuxPlayer();
                    m_Player->SendVaMessage("You now have %d skillpoints", atoi(param));
					success = true;
					msg_sent = true;
				}
			}

            if (MatchOptWithParam("scan", pch, param, msg_sent))
            {
                if (m_Player->AdminLevel() >= 50)      // GM to Admin
                {
                    long scan_range = atoi(param);
                    if (scan_range > 1000)
                    {
                        //m_Player->ShipIndex()->CurrentStats.SetScanRange(scan_range);
						m_Player->m_Stats.ResetStat(STAT_SCAN_RANGE);
						m_Player->m_Stats.SetStat(STAT_BASE_VALUE, STAT_SCAN_RANGE, scan_range);
						m_Player->ShipIndex()->CurrentStats.SetScanRange(scan_range);
                        m_Player->SendVaMessage("Scan range set to %d", scan_range);
                        success = true;
                        msg_sent = true;
                    }
                    else
                    {
                        m_Player->SendVaMessage("Set Scan Range: /scan <1000..>", scan_range);
                    }
                }
            }

			if (m_Player->AdminLevel() >= 50)      // GM to Admin
			{

				if (strcmp(pch, "shutdown") == 0)
				{
                    LogMessage(">>>> SHUTDOWN issued by %s [%s]\n", m_Player->Name(), m_Player->AccountUsername());
		            m_ServerMgr.m_PlayerMgr.GlobalAdminMessage("Server shutdown in 30 seconds!");
					_sleep(15000);
		            m_ServerMgr.m_PlayerMgr.GlobalAdminMessage("Server shutdown in 15 seconds!");
					_sleep(5000);
		            m_ServerMgr.m_PlayerMgr.GlobalAdminMessage("Server shutdown in 10 seconds!");
					_sleep(5000);
		            m_ServerMgr.m_PlayerMgr.GlobalAdminMessage("Server shutdown in 5 seconds!");
					_sleep(5000);
		            m_ServerMgr.m_PlayerMgr.GlobalAdminMessage("Server shutting down!");
					_sleep(5000);
                    
                    g_ServerShutdown = true;
					success = true;
					msg_sent = true;
				}
				else if (strcmp(pch, "sendp") == 0)
				{
					success = true;
					msg_sent = true;
					m_ServerMgr.m_PlayerMgr.SendPlayerWithoutConnection(m_AvatarID);
				}
                else if (strcmp(pch, "strings") == 0)
                {
                    char buffer[256];
                    g_StringMgr->Statistics(buffer);
                    m_Player->SendVaMessage(buffer);
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
                m_Player->SendVaMessage("Test Successful!");
                msg_sent =  true;
			}

			if (strcmpi(pch, "talktree") == 0)
			{
				char string[] = 
					    "That was one heck of an explotion! Are you allright over there?\0"
						"\0\3"
						"\0\0\0\0"
						"I need a tow\0"
						"\1\0\0\0"
						"Toggle distress beacon\0"
						"\2\0\0\0"
						"I'm OK\0";

				SendResponse(ENB_OPCODE_0054_TALK_TREE, (unsigned char *) string, sizeof(string));
			}
			if (MatchOptWithParam("testmsg", pch, param, msg_sent))
			{
                long time_delay = atoi(param);
                m_SectorMgr->AddTimedCall(m_Player, B_TEST_MESSAGE, time_delay, 0, time_delay);

                msg_sent = true;
                success = true;
            }

			if (MatchOptWithParam("trade", pch, param, msg_sent))
			{
                // Trade requires that both players recieved a create packet for eachother's ships
                Player * targetp = m_ServerMgr.m_PlayerMgr.GetPlayer(param);

                if (!targetp)
                {
                    m_Player->SendVaMessage("Could not find player %s",param);
                    return;
                }

				if (targetp->GameID() == m_AvatarID)
				{
					if (!(m_Player->AdminLevel() >= 50))      // GM to Admin
					{

					m_Player->SendVaMessage("Cannot trade with yourself!");
					return;
					}
				}

				if (targetp->Connection()->m_TradeID != -1)
				{
					m_Player->SendVaMessage("Target is already trading");
					return;
				}

				LogDebug("Clearing trade for players `%s` and `%s`\n",m_Player->Name(),targetp->Name());

				for(int x=0;x<6;x++) 
				{
					targetp->ShipIndex()->Inventory.TradeInv.Item[x].Empty();
					m_Player->ShipIndex()->Inventory.TradeInv.Item[x].Empty();
				}
				
				m_Player->SendAuxShip(targetp);
				targetp->SendAuxShip(m_Player);

				TradeAction(targetp->GameID(),0);					// Opens a trade window
				targetp->Connection()->TradeAction(m_AvatarID, 0);	// Open trade window for other player
				m_TradeID = targetp->GameID();						// Set player tradeing with
				m_TradeConferm = 0;
				targetp->Connection()->m_TradeID = m_AvatarID;
				targetp->Connection()->m_TradeConferm = 0;

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
					SendResponse(ENB_OPCODE_0065_UI_TRIGGER, (unsigned char *) &Data, Index);
				}
				msg_sent = true;
			}
            else if (MatchOptWithParam("upgrade", pch, param, msg_sent))
			{
                if (m_Player->AdminLevel() >= 30) //Beta +
                {
                    long upgrade = atoi(param);
                    m_Player->ShipUpgrade(upgrade);
                    msg_sent = true;
                    success = true;
                }
            }
            break;

		case 'v':
			if (MatchOptWithParam("verb", pch, param, msg_sent))
			{
				HandleSendVerbRequest(param);
				msg_sent = true;
			}
		case 'w':
			if (strcmp(pch, "who") == 0)
			{
				m_ServerMgr.m_PlayerMgr.ListPlayersAndLocations(m_Player);
				msg_sent = true;
				success = true;
			}			
            else if (MatchOptWithParam("warp", pch, param, msg_sent))
			{
				int limit = 10000;
				
				if (atoi(param) > limit || atoi(param) < 1000)
				{
					m_Player->SendVaMessage("Warp limits are between 1000 and %d!", limit);
				}
				else
				{
					m_Player->SendVaMessage("Setting warp to %d",atoi(param));
					m_Player->ShipIndex()->CurrentStats.SetWarpSpeed(atoi(param));
					m_Player->SendAuxShip();
				}
				msg_sent = true;
			}

			if (m_Player->AdminLevel() >= 30)      // Beta to Admin
			{
				if (MatchOptWithParam("wormhole", pch, param, msg_sent))
				{
					success = HandleWormholeRequest(param);
					msg_sent = true;
				}
			}
            if (strcmp(pch, "warpreset") == 0)
            {
                m_Player->SetWarp();
                m_Player->TerminateWarp();
                msg_sent = true;
                success = true;
            }

			break;

		}

		if (!success && !msg_sent)
		{
			m_Player->SendVaMessage("Illegal slash command: %s", pch);
		}
	}
}

void Connection::HandleLogoffRequest()
{
	//LogoffRequest * request = (LogoffRequest *) m_RecvBuffer;
	LogMessage("Received LogoffRequest for player %s\n",m_Player->Name());

	//remove player from the group
	m_ServerMgr.m_PlayerMgr.LeaveGroup(m_Player->GroupID(),m_AvatarID);

	SendLogoffConfirmation();

	m_SectorMgr->RemovePlayerFromSectorList(m_Player);

	//now kill the player node
	m_ServerMgr.m_PlayerMgr.UnallocatePlayer(m_Player);
}

void Connection::HandleCTARequest()
{
	CTARequest * myCTARequest = (CTARequest *) m_RecvBuffer;

	LogMessage("CTA Request:\n");
	DumpBuffer(m_RecvBuffer, sizeof(CTARequest));

	m_ServerMgr.m_PlayerMgr.GroupAction(myCTARequest->SourceID, myCTARequest->TargetID, myCTARequest->Action);

	//capture_3		packet# 21495
	unsigned char CTAResponse[] =
	{
		0x00, 0x00, 0x00, 0x00,		//GameID
		0x0F, 0x00, 0x00, 0x00,		//Unknown
		0x01						//Char
	};

	*((long*) &CTAResponse[0]) = myCTARequest->SourceID;

	SendResponse(ENB_OPCODE_00BD_CTA_RESPONSE, (unsigned char *) &CTAResponse, sizeof(CTAResponse));
}

void Connection::SendLogoffConfirmation()
{
	LogMessage("Sending LogoffConfirmation packet\n");
	SendResponse(ENB_OPCODE_00BA_LOGOFF_CONFIRMATION);
}

void Connection::HandleStarbaseAvatarChange()
{
	StarbaseAvatarChange *pkt = (StarbaseAvatarChange *) m_RecvBuffer;
	//LogMessage("Received StarbaseAvatarChange packet\n");
	/*LogMessage("{AvatarID=%d RoomType=%d Orient=%f Position=(%f,%f,%f) ActionFlag=%d}\n",
	    pkt->AvatarID, pkt->RoomType, pkt->Orient,
	    pkt->Position[0], pkt->Position[1], pkt->Position[2],
	    pkt->ActionFlag);*/

    m_Player->HandleStarbaseAvatarChange(m_SectorID, pkt);
}

void Connection::HandleStarbaseRoomChange()
{
	StarbaseRoomChange *pkt = (StarbaseRoomChange *) m_RecvBuffer;
	//LogMessage("Received StarbaseRoomChange packet\n");
    m_Player->HandleStarbaseRoomChange(m_SectorID, pkt);
}

void Connection::SendTalkTreeAction(long action)
{
	//LogMessage("Sending TalkTreeAction packet\n");
	SendResponse(ENB_OPCODE_0056_TALK_TREE_ACTION, (unsigned char *) &action, sizeof(long));
}

bool Connection::HandleRangeRequest()
{
	bool success = false;

    if (m_Player->ShipIndex()->GetTargetGameID() > 0)
    {
        Object *obj = m_ObjectMgr->GetObjectFromID(m_Player->ShipIndex()->GetTargetGameID());
        if (obj)
        {
            m_Player->SendVaMessage("Range to object = %f", obj->RangeFrom(m_Player->Position(), true)); //report absolute range
            success = true;
        }
    }
	else
	{
		m_Player->SendVaMessage("Unable to access selected object - could not find object ID");
	}

	return (success);
}

bool Connection::HandleWormholeRequest(char *sector)
{
	bool success = false;
    bool illegal_dest = false;

    if (m_Player->PlayerIndex()->GetSectorNum() > 9999)
    {
        //m_Player->SendVaMessage("Unable to wormhole out of starbase, disembark first.");
		SendStarbaseSet(1, 0);
		SetManufactureID(0);
    }

    if (m_SectorMgr)
	{
        m_Player->TerminateWarp();
		long sector_id = atoi(sector);
		/*
		if (sector_id > 9999)
		{
			sector_id /= 10;
		}
		*/

        switch (sector_id)
        {
        case 1493:
        case 1930:
        case 2290:
        case 2295:
        case 3595:
        case 4195:
        case 4595:
        case 4095:
        case 4093:
            illegal_dest = true;
            break;
        default:
            break;
        };      

		char *current_sector_name = m_ServerMgr.m_SectorServerMgr.GetSectorName(m_SectorID);
		char *sector_name = m_ServerMgr.m_SectorServerMgr.GetSectorName(sector_id);
		if (sector_name && !illegal_dest)
		{
			m_Player->SendVaMessage("Wormhole from %s to %s\n", current_sector_name, sector_name);
			m_Player->SendVaMessage("Wormhole out of sector %d", m_SectorID);

            m_Player->RemoveFromAllSectorRangeLists();

			//Hand off server
			m_SectorMgr->SectorServerHandoff(this, sector_id);

			//set to new server;
			m_SectorID = sector_id;
			success = true;
		}
		else
		{
			m_Player->SendVaMessage("Invalid or Illegal Sector ID %d", sector_id);
		}
	}

    m_Player->RemoveProspectNodes();

	return (success);
}

bool Connection::HandleKick(char *param)
{
	char Message[1024];

	char *name = strtok(param, " ");
	char *reason = 0;

	if (name)
		reason = strtok(NULL, "");
	else
		return 0;

	Player * target = m_ServerMgr.m_PlayerMgr.GetPlayer(name);

	if (!reason) 
    {
		reason = new char[50];
		sprintf(reason, "No Reason Given");
	}

	if (target && target->ConnectionAvailable()) 
	{
		sprintf(Message, "Player '%s' kicked by '%s': %s", target->Name(), m_Player->Name(), reason);
		LogMessage(" ** Kick: %s\n", Message);

		target->Connection()->m_Player->SendVaMessage("You have been kicked by %s: %s", m_Player->Name(), reason);
		m_ServerMgr.m_PlayerMgr.GMMessage(Message);
		_sleep(100);
		target->Connection()->ForceLogout();
		return 1;
	}

	return 0;    
}

void Connection::ForceLogout()
{
    int Count = 0;

    // Wait 30 seconds for the player to become active, while in space
    while (!m_Player->Active() && m_Player->PlayerIndex()->GetSectorNum() < 10000 && Count < 30)
    {
        _sleep(1000);
        Count++;
    }

    // Give them 5 seconds to read the kick message!
    _sleep(5000);

	SendResponse(ENB_OPCODE_0003_LOGOFF, (unsigned char*)&m_AvatarID, sizeof(m_AvatarID));
	_sleep(100);

	m_ServerMgr.m_PlayerMgr.LeaveGroup(m_Player->GroupID(),m_AvatarID);
    m_SectorMgr->RemovePlayerFromSectorList(m_Player);
	m_ServerMgr.m_PlayerMgr.UnallocatePlayer(m_Player);
	m_TcpThreadRunning = false;
}

bool Connection::HandleMoveRequest(char *param)
{
	bool success = false;
	float x, y, z;
	char *a = strtok(param, ",");
	char *b = strtok(NULL, ",");
	char *c = strtok(NULL, ",");

	x = y = z = 0.0f;

	if (a == NULL)
	{
		m_Player->SendVaMessage("/move: syntax: x,y,z");
		m_Player->SendVaMessage("/move: all parameters float type");
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
			y = m_Player->PosY();
		}

		if (c != NULL)
		{
			z = (float)atof(c);
		}
		else
		{
			z = m_Player->PosZ();
		}
	}

	if (!m_Player)
	{
        m_Player->SetPosition(x, y, z);

		m_Player->SendVaMessage("Setting position to : %d %d %d", x, y, z);
            
		//m_ServerMgr.m_PlayerMgr.SendLocation(m_Player, m_Player);
		//CheckTargetUpdate();
        m_Player->UpdateVerbs();
		CheckObjectRanges();
		success = true;
	}

	return (success);
}


bool Connection::HandleOrientationRequest(char *orientation)
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
		m_Player->SendVaMessage("/orientation: syntax: o1,o2,o3,o4");
		m_Player->SendVaMessage("/orientation: all parameters float type");
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

		m_Player->SendVaMessage("Targetting 0x%08x Player = 0x%08x (%08x)",m_Player->ShipIndex()->GetTargetGameID(), m_AvatarID, m_AvatarID);

        obj = m_ObjectMgr->GetObjectFromID(m_Player->ShipIndex()->GetTargetGameID());

        if (obj)
        {
            //New orientation info:
            obj->SetOrientation(o1, o2, o3, o4);

            m_Player->SendVaMessage("Object name: %s", obj->Name());

            obj->SendPosition(this);

			success = true;
		}
		else
		{
			m_Player->SendVaMessage("Unable to access selected object - could not find object ID");
		}
	}

	return (success);
}

bool Connection::HandleFaceRequest(long Target)
{
	bool success = false;

	Object *obj = m_ObjectMgr->GetObjectFromID(Target);

	if (obj)
	{
        m_Player->Face(obj);
        m_Player->SendLocationAndSpeed(true);
		success = true;
	}
	else
	{
		m_Player->SendVaMessage("Unable to access selected object - could not find object ID");
	}

	return (success);
}

bool Connection::HandleEulerOrientationRequest(char *orientation)
{
	bool success = false;
	float o1, o2, o3;
	char *a = strtok(orientation, ",");
	char *b = strtok(NULL, ",");
	char *c = strtok(NULL, ",");

	o1 = o2 = o3 = 0.0f;

	if (a == NULL)//allow incomplete args
	{
		m_Player->SendVaMessage("/oeuler: syntax: heading,alt,bank");
		m_Player->SendVaMessage("/oeuler: all parameters float type");
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

        obj = m_ObjectMgr->GetObjectFromID(m_Player->ShipIndex()->GetTargetGameID());

        if (!obj)
        {
            obj = m_CurrentDecoObj;
        }

        if (obj)
        {
            //New orientation info:
            obj->SetEulerOrientation(o1,o2,o3);

            m_Player->SendVaMessage("Object name: %s", obj->Name());
			m_Player->SendVaMessage("Setting Euler orientation to %.2f,%.2f,%.2f",o1*180.0f/PI,o2*180.0f/PI,o3*180.0f/PI);
            float *vector;
            vector = obj->Orientation();
			m_Player->SendVaMessage("Setting Quaternion orientation to %.4f,%.4f,%.4f,%.4f",vector[0],vector[1],vector[2],vector[3]);

            obj->SendPosition(this);
			
			success = true;
		}
		else
		{
			m_Player->SendVaMessage("Unable to access selected object - could not find object ID");
		}
	}

	return (success);
}

bool Connection::HandleMobCreateRequest(char *param)
{
	char *pch_parse, *name = 0;
	short level = 0;
    short mob_type = 0;
    short mob_count = 0;
    bool direct_basset = false;
    bool directional = false;
    float velocity = 100.0f;
    float turn = 0.2f;
    float xoffset = 300.0f;

    //are we trying to create a mob too close to a station?
	/*
    Object *obj = m_Player->NearestNav();

    if (obj)
    {
		// Allow turrets to be put near stations
        if (obj->ObjectType() == OT_STATION && (obj->RangeFrom(m_Player->Position()) < 5000.0f || mob_type == 14))
        {
            m_Player->SendVaMessage("You're trying to create a MOB too close to a station. Move to at least 5K away.");
            return false;
        }
    }
	*/

    while (MobData[mob_count].basset != 0)
    {
        mob_count++;
    };

	pch_parse = strtok(param, " ");

	if (!pch_parse)
	{
		return false;
	}

    if (*pch_parse == 'B')
    {
        //direct basset method
        pch_parse = strtok(NULL, " ");
        direct_basset = true;
    }
    else if (*pch_parse == 'D')
    {
        pch_parse = strtok(NULL, " ");
        directional = true;
        level = 30;
        mob_type = MOB_EnergyPhoenix;
        name = MobData[mob_type].name;
        if (pch_parse) 
        {
            xoffset = atof(pch_parse);
            pch_parse = strtok(NULL, " ");
            if (pch_parse) 
            {
                velocity = atof(pch_parse);
                pch_parse = strtok(NULL, " ");
                if (pch_parse) 
                {
                    turn = atof(pch_parse);
                }
            }
        }
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
	
	if ((level > 0 && level < 67) && ( (mob_type > -1 && mob_type < mob_count) || direct_basset == true) )
	{
        MOB *mob = (MOB*)m_ObjectMgr->AddNewObject(OT_MOB); //MOB creation

        if (!name)
        {
            name = MobData[mob_type].name;
        }

        if (mob)
        {
            if (direct_basset)
            {
                m_Player->SendVaMessage("Creating %d at level %d (%s)", mob_type, level, name);
                mob->SetMOBType(0);
                mob->SetBasset(mob_type);
            }
            else
            {
                m_Player->SendVaMessage("Creating %s at level %d (%s)", MobData[mob_type].name, level, name);
                mob->SetMOBType(mob_type);
            }
            mob->SetName(name);
            mob->SetPosition(m_Player->Position());
            mob->MovePosition(xoffset, 0.0f, -200.0f);
            mob->SetLevel(mob_type == 14? 66 : level);
            mob->SetActive(true);
            mob->SetRespawnTick(0);

			// For Turretts
			if (mob_type == 14) {
				velocity = 0;
				mob->SetHostileTo(OT_MOB);
				mob->Turn(0);
			} else {
				mob->SetHostileTo(OT_PLAYER);
				mob->Turn(turn);
			}
			mob->SetVelocity(velocity);
            mob->SetDefaultStats(turn, MobData[mob_type].behaviour, velocity, 50);
            mob->SetUpdateRate(50);
            mob->SetBehaviour(MobData[mob_type].behaviour);
            mob->AddBehaviourPosition(m_Player->Position());
            
            return true;
        }
        else
        {
            m_Player->SendVaMessage("Unable to create MOB at this time.");
            return false;
        }
	}
	else
	{
		m_Player->SendVaMessage("Create Mob: /createmob <MOB type>0..%d <level>1..66 <name>", (mob_count - 1));
        mob_count = 0;
        while (MobData[mob_count].basset != 0)
        {
            m_Player->SendVaMessage(" type %d: %s", mob_count, MobData[mob_count].name);
            mob_count++;
        };

		return false;
	}

    return true;

}

bool Connection::HandleObjCreateRequest(char *param)
{
	char *pch_level;
	short level = 0;
	pch_level = strchr(param, ' ');

	if (strcmp(param, "ON") == 0)
	{
		m_Player->SendVaMessage("Resource Creation ACTIVATED");
		m_Player->SendVaMessage("Create Resource: /create <type>0..12 <level>1..9");
		m_ServerMgr.m_AllowCreate = true;
		return true;
	}
	else if (strcmp(param, "XML") == 0)
	{
		m_Player->SendVaMessage("XML output on");
		m_ServerMgr.m_DumpXML = true;
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
			m_Player->SendVaMessage("Create Asteroid field: /create F <type>1..9 <level>1..8 <count>1.. <radius>1.. ");
			return false;
		}
		else if (m_Player->AdminLevel() >= 50)      // GM to Admin
		{
            Object *obj = m_ObjectMgr->AddNewObject(OT_FIELD); //complete creation of field
            obj->SetFieldRadius((float)radius);
            obj->SetLevel(level);
            obj->SetFieldCount(number);
            obj->SetFieldType(type);
            obj->SetPosition(m_Player->Position());
            obj->PopulateField(); // issue field
			m_Player->SendVaMessage("Asteroid field created. Level:%d Count:%d Radius:%d", level, number, radius);
            m_ObjectMgr->DisplayDynamicObjects(m_Player, true); //update display for all roids.
			return true;
		}
		else
		{
			m_Player->SendVaMessage("%s, you can't create an asteroid field ... yet.", m_Player->Name());
			return false;
		}
	}
	else if (strncmp(param, "M", 1) == 0)
	{
		Object *obj = m_ObjectMgr->GetObjectFromID(m_Player->ShipIndex()->GetTargetGameID());
		if (obj)
		{
			//p->Connection()->m_SectorMgr->GetMobPosition(mob, &position[0]);
			//mob->position_info.Position
			//m_SectorMgr->GetMobPosition(obj, &position[0]);
			


			/*position[0] = obj->position_info.Position[0];
			position[1] = obj->position_info.Position[1];
			position[2] = obj->position_info.Position[2];*/
			//m_SectorMgr->AddNewObject(this, 9, 7, &position[0]);
		}
	}
	else if (m_ServerMgr.m_AllowCreate == false)
	{
		m_Player->SendVaMessage("Create Resource: Unable to create resource");
		return false;
	}
	
	if (!pch_level)
	{
		m_Player->SendVaMessage("Create Resource: /create <type>0..12 <level>1..9");
		return false;
	}

	short object_type = (long)atoi(param);

	level = atoi(pch_level + 1);

	if ((level > 0 && level < 10) && (object_type > -1 && object_type < 13))
	{
        Object *obj = m_ObjectMgr->AddNewObject(OT_RESOURCE);
        obj->SetTypeAndName(object_type);
        obj->SetLevel(level);
        obj->SetPosition(m_Player->Position());
        obj->ResetResource(); //populate with items

		return true;
	}
	else
	{
		m_Player->SendVaMessage("Create Resource: /create <type>0..12 <level>1..9");
		return false;
	}
}

void Connection::ChangeSectorID(long SectorID)
{
	m_ServerMgr.m_PlayerMgr.SetSector(m_Player, SectorID);
}

bool Connection::HandleFetchRequest()
{
    bool success = false;
    Object *obj = m_ObjectMgr->GetObjectFromID(m_Player->ShipIndex()->GetTargetGameID());

    if (obj != NULL)
    {
        obj->SetPosition(m_Player->Position());
        m_Player->SendVaMessage("Object name: %s", obj->Name());
        obj->SendPosition(this); //TODO: send new position to everyone
	}
	else
	{
		m_Player->SendVaMessage("Unable to access selected object - could not find object ID");
	}

	return (success);
}

bool Connection::HandleObjectHijack()
{
    Object *obj = m_ObjectMgr->GetObjectFromID(m_Player->ShipIndex()->GetTargetGameID());

    if (obj)
    {
        //try a different approach with this.
        //lets try just locking the position of the hijacked object onto the player's ship
        //and not showing the ship to anyone else
        m_Player->SendVaMessage("You have been successfully assigned to control '%s'.", obj->Name());
        LogMessage("HIJACK occurred. Account name '%s'.\n", m_Player->AccountUsername());
        
        m_Player->HijackObject(obj);
        return true;
    }
    else
    {
        m_Player->SendVaMessage("Unable to hijack object with GameID %d.\n", m_Player->ShipIndex()->GetTargetGameID());
        return false;
    }
}

void Connection::HandleReleaseHijack()
{
    if (m_Player->Hijackee())
    {
        m_Player->SendVaMessage("You have been unassigned from control of '%s'.", m_Player->Hijackee()->Name());
        m_Player->Hijackee()->SetPlayerHijack(0);
        m_Player->SetHijackee(0);        
    }
}

bool Connection::HandleGotoRequest()
{
    bool success = false;
    Object *obj;
	bool admin = false;

	//only allow admins to jump to resources
	if (m_Player->AdminLevel() >= 50)      // GM to Admin
	{
		admin = true;
	}

	if (admin == false)
	{
		m_Player->SendVaMessage("/goto has been disabled");
		return (false);
	}

    obj = m_ObjectMgr->GetObjectFromID(m_Player->ShipIndex()->GetTargetGameID());

    if (obj != NULL)
    {
        m_Player->SetPosition(obj->Position());
        m_Player->SendLocationAndSpeed(true);
		m_Player->UpdateVerbs();
		CheckObjectRanges();
        m_Player->CheckNavs();
		success = true;
	}

	return (success);
}

void Connection::GateSequenceEnd()
{
	m_StargateID = 0;
	m_SectorMgr->GateJump(this);
}

//First sequence for Stargate open
void Connection::OpenStargate_1()
{
	Object *obj = m_ObjectMgr->GetObjectFromID(m_Player->ShipIndex()->GetTargetGameID());

	if (obj && obj->ObjectType() == OT_STARGATE)
	{
		SendActivateRenderState(m_Player->ShipIndex()->GetTargetGameID(), 2);
		obj->SetRenderState(2);
	}
}

void Connection::OpenStargate_2(long object_id)
{
    SendActivateNextRenderState(object_id, 1);
}

void Connection::CloseStargate(long object_id)
{
    SendActivateNextRenderState(object_id, 3);
}

bool Connection::HandleRenderStateRequest()
{
    bool success = false;
    Object *obj = m_ObjectMgr->GetObjectFromID(m_Player->ShipIndex()->GetTargetGameID());

    if (obj)
    {
        m_Player->SendVaMessage("Object '%s'", obj->Name());
        m_Player->SendVaMessage("Current Render State   = 0x%X", obj->RenderState());
        //m_Player->SendVaMessage("Allowable Render State = 0x%X", obj->allowable_render_states);
        success = true;
    }
    else
    {
        m_Player->SendVaMessage("Unable to access selected object - could not find object ID");
    }

	return (success);
}

bool Connection::HandleScaleRequest(char *param)
{
	bool success = false;
	if (m_SectorMgr && (m_Player->ShipIndex()->GetTargetGameID() > 0 || m_CurrentDecoObj))
	{
        float scale = (float) atof(param);
        Object *obj = m_ObjectMgr->GetObjectFromID(m_Player->ShipIndex()->GetTargetGameID());

        if (!obj) obj = m_CurrentDecoObj;

        if (obj)
        {
            if (scale == 0.0)
            {
                // Display current scale
                m_Player->SendVaMessage("Scale of %s is currently %.2f", obj->Name(), obj->Scale());
            }
            else
            {
                // Set new scale
                m_Player->SendVaMessage("Changing scale from %g to %g", obj->Scale(), scale);
                obj->SetScale(scale);
                long target_id = obj->GameID();
                // Remove the Object
                obj->Remove();
                // Recreate the Object
                obj->SendObject(m_Player);
            }
        }
    }

	return (success);
}

bool Connection::SendLoungeNPC(long StationID)
{
#ifdef USE_MYSQL_STATIONS
	struct StationLounge LoungeData;
	unsigned char bufferd[10000];
	unsigned char *buffer = bufferd;
	int Size, x;

	memset(buffer, 0, sizeof(buffer));
	memset(&LoungeData, 0, sizeof(LoungeData));

	StationTemplate * Stn = m_ServerMgr.m_StationList;

	while(Stn && Stn->StationID != StationID)
	{
		Stn = Stn->next;
	}

	if (!Stn)			// Station not found?
		return false;

	LoungeData.Station.StationType = Stn->Type;
	LoungeData.Station.RoomNumber = Stn->NumRooms;
	LoungeData.NumTerms = Stn->NumTerms;
	LoungeData.NumNPCs = Stn->NumNPCs;

	NPCTemplate * NPCs = Stn->NPCs;
	for(x=0;x<Stn->NumNPCs;x++)
	{
		LoungeData.NPC[x].BoothType = NPCs->Booth;
		LoungeData.NPC[x].Location = NPCs->Location;
		LoungeData.NPC[x].NPCID = NPCs->StarbaseID;
		LoungeData.NPC[x].RoomNumber = NPCs->Room;
		memcpy(&LoungeData.NPC[x].Avatar,&NPCs->Avatar,sizeof(AvatarData));
		NPCs = NPCs->next;
	}

	RoomTemplate * Rooms = Stn->Rooms;
	for(x=0;x<Stn->NumRooms;x++)
	{
		LoungeData.Rooms[x].RoomNumber = Rooms->Index;
		LoungeData.Rooms[x].RoomStyle = Rooms->Style;
		LoungeData.Rooms[x].FogFar = Rooms->FogFar;
		LoungeData.Rooms[x].FogNear = Rooms->FogNear;
		LoungeData.Rooms[x].FogRed = Rooms->FogRed;
		LoungeData.Rooms[x].FogGreen = Rooms->FogGreen;
		LoungeData.Rooms[x].FogBlue = Rooms->FogBlue;
		
		Rooms = Rooms->next;
	}

	TermTemplate * Terms = Stn->Terms;
	for(x=0;x<Stn->NumTerms;x++)
	{
		LoungeData.Terms[x].TermType = Terms->Type;
		LoungeData.Terms[x].Location = Terms->Location;
		LoungeData.Terms[x].RoomNumber = Terms->Room;
		Terms = Terms->next;
	}

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

	SendResponse(ENB_OPCODE_0052_LOUNGE_NPC, (unsigned char *) bufferd, Size);

	//LogMessage("Loaded Station from MySQL (%d)\n", m_SectorID);

	//DumpBuffer(bufferd, Size);

	return true;
#else
	return false;
#endif
}

//Not too sure on the data structure of this packet
bool Connection::HandleRenderStateInitRequest(char *param)
{
    bool success = false;
    Object *obj = m_ObjectMgr->GetObjectFromID(m_Player->ShipIndex()->GetTargetGameID());
    long render_state = atol(param);
    
    if (obj != NULL)
    {
        m_Player->SendVaMessage("Object '%s'", obj->Name());
        m_Player->SendVaMessage("Current Render State   = 0x%X", obj->RenderState());
        m_Player->SendVaMessage("Allowable Render State = 0x%X", 3);
        m_Player->SendVaMessage("New Render State       = 0x%X", render_state);
        SendInitRenderState(m_Player->ShipIndex()->GetTargetGameID(), render_state);
        obj->SetRenderState(render_state);
        success = true;
    }
    else
    {
        m_Player->SendVaMessage("Unable to access selected object - could not find object ID");
    }
    return (success);
}

bool Connection::HandleRenderStateActivateRequest(char *param)
{
    bool success = false;
    Object *obj;
    long render_state = atol(param);
    obj = m_ObjectMgr->GetObjectFromID(m_Player->ShipIndex()->GetTargetGameID());

    if (obj != NULL)
    {
        m_Player->SendVaMessage("Object '%s'", obj->Name());
        m_Player->SendVaMessage("Activate Render State: 2 = open gate half way");
        m_Player->SendVaMessage("Current Render State   = 0x%X", obj->RenderState());
        m_Player->SendVaMessage("Allowable Render State = 0x%X", 3);
        m_Player->SendVaMessage("New Render State       = 0x%X", render_state);
        SendActivateRenderState(m_Player->ShipIndex()->GetTargetGameID(), render_state);
        obj->SetRenderState(render_state);
        success = true;
    }
    else
    {
        m_Player->SendVaMessage("Unable to access selected object - could not find object ID");
    }
    return (success);
}

bool Connection::HandleRenderStateActivateNextRequest(char * param)
{
    bool success = false;
    Object *obj;
    long render_state = atol(param);
    obj = m_ObjectMgr->GetObjectFromID(m_Player->ShipIndex()->GetTargetGameID());

    if (obj != NULL)
    {
        m_Player->SendVaMessage("Object '%s'", obj->Name());
        m_Player->SendVaMessage("Activate Next Render State: 1 = Gate opens fully 3 = gate closes");
        m_Player->SendVaMessage("Current Render State   = 0x%X", obj->RenderState());
        m_Player->SendVaMessage("Allowable Render State = 0x%X", 3);
        m_Player->SendVaMessage("New Render State       = 0x%X", render_state);
        SendActivateNextRenderState(m_Player->ShipIndex()->GetTargetGameID(), render_state);
        obj->SetRenderState(render_state);
        success = true;
    }
    else
    {
        m_Player->SendVaMessage("Unable to access selected object - could not find object ID");
    }
    return (success);
}

bool Connection::HandleRenderStateDeactivate()
{
    bool success = false;
    Object *obj;
    obj = m_ObjectMgr->GetObjectFromID(m_Player->ShipIndex()->GetTargetGameID());

    if (obj != NULL)
    {
        m_Player->SendVaMessage("Object '%s'", obj->Name());
        m_Player->SendVaMessage("Deactivate Render State");
        m_Player->SendVaMessage("Current Render State   = 0x%X", obj->RenderState());
        m_Player->SendVaMessage("Allowable Render State = 0x%X", 3);
        m_Player->SendVaMessage("New Render State       = 0x%X", 0);
        SendDeactivateRenderState(m_Player->ShipIndex()->GetTargetGameID());
        obj->SetRenderState(0); //Not sure if I should be changing this here
        success = true;
    }
    else
    {
        m_Player->SendVaMessage("Unable to access selected object - could not find object ID");
    }
    return (success);
}

void Connection::HandleStarbaseRequest()
{
	StarbaseRequest * pkt = (StarbaseRequest *) m_RecvBuffer;

    LogDebug("Starbase Request - PlayerID: %d Action: %d StarBaseID: %d\n",pkt->PlayerID, pkt->Action, pkt->StarbaseID);

    char *professions[] =
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

    memcpy((string+43),professions[m_Player->Profession()], 9);

	printf("Received StarbaseRequest packet, PlayerID=%x, StarbaseID=%d, Action=%d\n",
	pkt->PlayerID, pkt->StarbaseID, pkt->Action);
	
	m_TradeWindow = false;

	switch(pkt->Action)
	{
	case 1: // Exiting the station action
		if (pkt->StarbaseID == m_SectorID && m_SectorMgr)
		{
			if (m_TradeID != -1)
				CancelTrade();
			// Launch into space!
			m_SectorMgr->LaunchIntoSpace(this);
		}
		break; 
	case 4: // Talk to NPC
		m_StarbaseTargetID = pkt->StarbaseID;
			// Display the standard talk tree unless we have a mission here
        if (m_Player->CheckMissions(0, 1, 0, m_StarbaseTargetID) ||
            m_Player->CheckForNewMissions(0, 1, 0, m_StarbaseTargetID) )
        {
            return;
        }
        else
        {
            SendResponse(ENB_OPCODE_0054_TALK_TREE, (unsigned char *) string, sizeof(string));
            m_StarbaseTargetID = pkt->StarbaseID;
        }
        break;
	case 6: // Activating Job Terminal
		{
            /*
			PacketBuffer buffer;
			
			m_ServerMgr.m_Jobs.GetJobList(this, &buffer);
			SendResponse(ENB_OPCODE_0093_JOB_LIST, buffer.getBuffer(), buffer.size());
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

			m_ServerMgr.m_Jobs.GetJobDescription(pkt->StarbaseID, &buffer, &m_ServerMgr.m_TokenParser);
			SendResponse(ENB_OPCODE_0094_JOB_DESCRIPTION, buffer.getBuffer(), buffer.size());

			_Item myItem; 
			memset(&myItem,0,sizeof(_Item)); 

			int itemId = m_ServerMgr.m_Jobs.GetItem(pkt->StarbaseID);

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
                m_Player->SendItemBase(myItem.ItemTemplateID);

			    m_Player->PlayerIndex()->RewardInv.Item[0].SetData(&myItem);
			    m_Player->SendAuxPlayer();
            }
            */

		break;
		}

	case 8: // Accept job?
			LogMessage("Accepting Job\n");
			SendResponse(ENB_OPCODE_0096_JOB_ACCEPT_REPLY);
			// Eventually we'll be able to accept jobs once the system is fixed. 
			//m_ServerMgr.m_Missions.givePlayerMission(this, 1);			
		break;
	case 9: // ??
		break;
	case 10: // Customize avatar - NOT WORKING YET
		SendResponse(ENB_OPCODE_0083_RECUSTOMIZE_AVATAR_START);
		break;		
	case 11: // Customize starship - NOT WORKING YET
		SendResponse(ENB_OPCODE_0081_RECUSTOMIZE_SHIP_START);
	}
}


void Connection::SetManufactureID(long mfg_id)
{
	//LogMessage("Sending SetManufactureID packet\n");
	SendResponse(ENB_OPCODE_007F_MANUFACTURE_SET_MANUFACTURE_ID, (unsigned char *) &mfg_id, sizeof(mfg_id));
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
void Connection::SendServerHandoff(long from_sector_id, long to_sector_id, char *from_sector, char *from_system, char *to_sector, char *to_system)
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

	LogDebug("Sending ServerHandoff, ToSectorID = %d\n", to_sector_id);
	SendResponse(ENB_OPCODE_003A_SERVER_HANDOFF, (unsigned char *) &server_handoff, length);

	ChangeSectorID(to_sector_id);
}

void Connection::HandleTriggerEmote()
{
	TriggerEmote * emote = (TriggerEmote *) m_RecvBuffer;

	SendNotifyEmote(emote->GameID, emote->Emote);
}

//TODO: Recode using new packet methods
void Connection::HandleChatStream()
{
	ChatStream * chat_stream = (ChatStream *) m_RecvBuffer;
	unsigned char *buffer;
	unsigned char *p;
	
	if (chat_stream->message[0] == 0x02)	// Emote
	{
		LogMessage("Received AvatarTriggerEmote packet -- GameID=%d\n", chat_stream->GameID);

		buffer = new unsigned char[chat_stream->ChatSize + 7];		// 7 = (long GameID, short ChatSize, char type)
		memset(buffer, 0, chat_stream->ChatSize + 7);
		
		*((short *) &buffer[0]) = chat_stream->ChatSize;
		buffer[2] = 0x01;
		*((long *) &buffer[3]) = chat_stream->GameID;
		p = (unsigned char *)chat_stream;
		p+=7;
		memcpy(&buffer[7],p,chat_stream->ChatSize);

		//SendResponse(ENB_OPCODE_005F_AVATAR_EMOTE_RESPONSE, buffer, chat_stream->ChatSize + 7);
        m_Player->SendToSector(ENB_OPCODE_005F_AVATAR_EMOTE_RESPONSE, buffer,  chat_stream->ChatSize + 7);
		delete[] buffer;
	}
	else if (chat_stream->message[0] == 0x01)	// Chat in Stations
	{
		//Commented this section out so local messages aren't sent twice (Once as local, the other as broadcast)
		//short chatStringLength = *((short *)&chat_stream->message[2]);
		//char broadcast[2048];
		//sprintf(broadcast, "[Local] %s: %s", m_Player->Name(), &chat_stream->message[4]);
		//SendMessageString(broadcast, 7);
		LogDebug("ChatStream code: %d\n", chat_stream->message[0]);
	}
	else
	{
		LogMessage("ChatStream code: %d\n", chat_stream->message[0]);
		LogMessage("Received Unknown ChatStream code\n");
	}
}

void Connection::SendClientChatEvent(int Unknown, int Unknown2, char * FName, char * LName, char * Channel, char * Message)
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
		LogChatMsg("%s->[%s]> \"%s\" (%s)\n", FName, Channel, Message, m_Player->Name());
	}

	SendResponse(ENB_OPCODE_00A5_CLIENT_CHAT_EVENT, (unsigned char *) Packet, Index);	// Send Packet
}

void Connection::SendNotifyEmote(long game_id, long emote)
{
	NotifyEmote response;
	response.GameID = game_id;
	response.Emote = emote;

	//LogMessage("Sending NotifyEmote packet\n");
	//SendResponse(ENB_OPCODE_00A2_NOTIFY_EMOTE, (unsigned char *) &response, sizeof(response));
    m_Player->SendToRangeList(ENB_OPCODE_00A2_NOTIFY_EMOTE, (unsigned char *) &response, sizeof(response));
}

void Connection::HandleOption()
{
	OptionPacket * myOption = (OptionPacket *) m_RecvBuffer;

	LogMessage("Received Option packet -- GameID=%d  OptionType=%d  OptionVar=%d\n",
		myOption->GameID, myOption->OptionType, myOption->OptionVar);
	DumpBuffer(m_RecvBuffer,sizeof(OptionPacket));
}

// This is going to need re-coding in the same way as the Action Packet method
void Connection::HandleSelectTalkTree()
{
	SelectTalkTree * packet = (SelectTalkTree *) m_RecvBuffer;

	LogMessage("Received SelectTalkTree packet -- PlayerID=%x  Selection=%d\n",
		(packet->PlayerID & 0x00FFFFFF), packet->Selection);

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
				m_Player->SetDistressBeacon(false);
				m_Player->TowToBase();
				m_BeaconRequest = false;
				break;
			case 1:	   //  Distress Beacon
				SendTalkTreeAction(-32);
				m_Player->SetDistressBeacon(true);
				m_BeaconRequest = false;
				break;
			case 2:    // I'm OK
				SendTalkTreeAction(-32);
				m_Player->SetDistressBeacon(false);
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


    if (packet->Selection != 230 && m_Player->CheckMissions(0, packet->Selection, 0, m_StarbaseTargetID))
    {
        m_TradeWindow = true;
        if (packet->Selection == 0)
        {
            SendTalkTreeAction(-32); //close display
        }
        return;
    }

    // Trade with vendor
	if (packet->Selection == 0)
    {
		char string[] = "/happy2 Thanks for coming by my booth.  May good fortune be with you.\0\0\0";
		
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
                    m_Player->SendItemBase(item->ItemTemplateID());
				    ItemTemp[TotalItems] = item->ItemTemplateID();
				    ItemPrice[TotalItems] = item->Cost() * 2;
				    TotalItems++;
			    }
            }

			x++;
		}

		for(x=0; x<TotalItems; x++) 
		{
			m_Player->PlayerIndex()->VendorInv.Item[x].SetItemTemplateID(ItemTemp[x]);
			m_Player->PlayerIndex()->VendorInv.Item[x].SetStackCount(1);
			m_Player->PlayerIndex()->VendorInv.Item[x].SetStructure(1.0f);
			m_Player->PlayerIndex()->VendorInv.Item[x].SetQuality(1.0f);
			m_Player->PlayerIndex()->VendorInv.Item[x].SetPrice(ItemPrice[x]);
		}
		for (x=TotalItems; x<128; x++)
		{
			m_Player->PlayerIndex()->VendorInv.Item[x].Clear();
		}

        m_Player->SetPrices();
		m_Player->SendAuxPlayer();
        m_Player->SendAuxShip();

		SendTalkTreeAction(5);
		SendTalkTreeAction(3);
		SendResponse(ENB_OPCODE_0054_TALK_TREE, (unsigned char *) string, sizeof(string));
		SendTalkTreeAction(6);

		m_TradeWindow = true;
	}

	if (packet->Selection == 1 || packet->Selection == 230 ) 
	{
		SendTalkTreeAction(-32);

        m_Player->ClearPrices();
		m_Player->SendAuxPlayer();
        m_Player->SendAuxShip();
	}
}

void Connection::SendRelationship(long ObjectID, long Reaction, bool IsAttacking)
{
	Relationship response;
	response.ObjectID = ntohl(ObjectID);
	response.Reaction = Reaction;
	response.IsAttacking = IsAttacking ? 1 : 0;

	//LogMessage("Sending Relationship packet\n");
	SendResponse(ENB_OPCODE_0089_RELATIONSHIP, (unsigned char *) &response, sizeof(response));
}

void Connection::HandleGalaxyMapRequest()
{
	LogMessage("Received GalaxyMap request packet\n");
    SendResponse(ENB_OPCODE_2011_GALAXY_MAP, 0, 0);
	//SendDataFileToClient("GalaxyMap.dat");
}

void Connection::SendGalaxyMap(char *system, char *sector, char *station)
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
	galaxy_map.PlayerID = m_AvatarID;

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
	SendResponse(ENB_OPCODE_0097_GALAXY_MAP, (unsigned char * ) &galaxy_map, size + 8);
}

void Connection::HandleDebug()
{
	LogDebug("Received Debug packet\n");
}

void Connection::SendAdvancedPositionalUpdate(long object_id)
{
	// NOTE: THIS IS HARD-CODED FOR PLACING THE PLAYERS SHIP IN EARTH SECTOR
	// THIS TEMPORARY CODE WILL BE DELETED!

    m_Player->SetOrientation(0.0f, 0.0f, 1.0f, 0.0f);

	/*
	switch (m_SectorID)
	{
	case 1060 : // Earth - Earth Station
		position[0] =    4200.0;    // X
		position[1] =   14300.0;    // Y
		position[2] =       0.0;    // Z
		break;
	case 1052 : // Io - Nishino Research Facility
		position[0] =  -56400.0;    // X
		position[1] =   86800.0;    // Y
		position[2] =     750.0;    // Z
		break;
	case 1020 : // High Earth - Loki Station
		position[0] =  -68310.0;    // X
		position[1] =  -22180.0;    // Y
		position[2] =       0.0;    // Z
		break;
	case 1015 : // Luna - Luna Station
		position[0] =  -91620.0;    // X
		position[1] =  105450.0;    // Y
		position[2] =       0.0;    // Z
		break;
	case 1030 : // Mars Beta - Arx Prima
		position[0] =  -84770.0;    // X
		position[1] =     590.0;    // Y
		position[2] =    8000.0;    // Z
		break;
	default :   // arbitrary starting point
		position[0] =       0.0;    // X
		position[1] =    5000.0;    // Y
		position[2] =       0.0;    // Z
		break;
	}*/

	if (m_FromSectorID > 9999)
	{
		// The player just left a station
        // see if we had any entry coords
        if (!m_Player->RestoreDockingCoords())
        {
            Object *station = m_ObjectMgr->FindStation(0);
            Object *nearest_nav = m_Player->NearestNav();
            if (nearest_nav)
            {
                m_Player->Face(nearest_nav);
            }
            if (station)
            {
                int offset = 0;
                // Place the player's ship near the gate
                switch(m_SectorID)
                {
                case 1060 :
                    offset = - 10000;
                    break;
                default :
                    offset = station->Radius();
                    if (offset == 0)
                    {
                        offset = 4400;
                    }
                    break;
                }
                m_Player->SetPosition(station->PosX() + offset, station->PosY(), station->PosZ());
            }
        }
    }
    else 
    {
        if (m_SectorID < 9999)
        {
            // The player just went through a gate
            Object *gate = m_ObjectMgr->FindGate(m_FromSectorID);
            if (!gate) gate = m_ObjectMgr->FindFirstNav();
            if (gate)
            {
                // Place the player's ship near the gate
                // TODO: Move the player about 1K towards the nearest nav
                // TODO: Point the players ship (orientation) towards the nearest nav
                m_Player->SetPosition(gate->Position());
            }
            else
            {
                LogMessage("WARNING!! No gate or nav found in sector %s\n", m_SectorMgr->GetSectorName(m_SectorID));
                m_Player->SetPosition(0.0f, 0.0f, 0.0f);
            }
        }
        else
        {
            m_Player->SetPosition(0.0f, 0.0f, 0.0f);           
        }
    }

	if (m_FromSectorID == 0 || (m_FromSectorID == m_SectorID))		// We just logged in! 
	{
		bool success = m_Player->LoadPosition();

		if (!success || (m_Player->PosX() == 0.0f && m_Player->PosY() == 0.0f && m_Player->PosZ() == 0.0f))
		{
			LogDebug("Cannot load postion file setting to nearest gate.\n");
			
            //find first nav
            Object *obj = m_ObjectMgr->FindFirstNav();

            if (!obj)
            {
                obj = m_ObjectMgr->GetObjectFromID(10000);
            }

            if (obj)
            {
                m_Player->SetPosition(obj->Position());            
            }
		} 
	}

	//TODO: replace this with something more sensible
	//we can't send the advpos until we have received a time request.
	unsigned char update[] =
	{
		0x00, 0x00,                 // Bitmask
		0xEF, 0x17, 0xC0, 0x07,		// GameID
		0xE0, 0xEB, 0xEA, 0x25,     // Timestamp
		0x7C, 0x6A, 0x83, 0x45,     // X
		0x3A, 0xFE, 0x5F, 0x46,     // Y
		0xB0, 0x39, 0xDE, 0x41,     // Z
		0x00, 0x00, 0x00, 0x00,		//O1
		0x00, 0x00, 0x00, 0x00,		//O2
		0x5B, 0xD1, 0x7F, 0x3F,		//O3
		0x3A, 0x82, 0x1A, 0xBD,		//O4
		0x05, 0x00, 0x00, 0x00		// Movement ID
	};

	*((long *) &update[2]) = object_id;
	*((long *) &update[6]) = GetNet7TickCount();
	*((float *) &update[10]) = m_Player->PosX();
	*((float *) &update[14]) = m_Player->PosY();
	*((float *) &update[18]) = m_Player->PosZ();
    float *orientation = m_Player->Orientation();
    *((float *) &update[22]) = orientation[0];
	*((float *) &update[26]) = orientation[1];
	*((float *) &update[30]) = orientation[2];
	*((float *) &update[34]) = orientation[3];

	//LogMessage("Sending AdvancedPositionalUpdate packet\n");
	SendResponse(ENB_OPCODE_003E_ADVANCED_POSITIONAL_UPDATE, update, sizeof(update));
    m_Player->UpdateVerbs();
}

void Connection::SendMessageString(char *msg, char color, bool log)
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
		LogChatMsg("%s> \"%s\"\n", m_Player->Name(), msg);
	}

	SendResponse(ENB_OPCODE_001D_MESSAGE_STRING, (unsigned char *) buffer, length + 3);
}

void Connection::SendPriorityMessageString(char *msg1, char *msg2, long time, long priority)
{
	unsigned char buffer[512];
	int index = 0;
	memset(buffer, 0, sizeof(buffer));

	AddDataSN(buffer, msg1, index);
	AddDataSN(buffer, msg2, index);
	AddData(buffer, time, index);
	AddData(buffer, priority, index);

	SendResponse(ENB_OPCODE_0020_PRIORITY_MESSAGE, &buffer[0], index);
}

void Connection::HandleMissionDismissal()
{
    MissionDismissal *dismiss = (MissionDismissal *)m_RecvBuffer;
    long MissionID = ntohl(dismiss->MissionID);
	long PlayerID = ntohl(dismiss->PlayerID);

    m_Player->MissionDismiss(MissionID);
}

void Connection::HandleMissionForfeit() //TODO: change handling of forfeit. at the moment, we allow ppl to repeat the mission.
{
    MissionDismissal *dismiss = (MissionDismissal *)m_RecvBuffer;
    long MissionID = ntohl(dismiss->MissionID);
	long PlayerID = ntohl(dismiss->PlayerID);

    m_Player->MissionDismiss(MissionID);
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

void Connection::HandlePetitionStuck()
{
	char *p = (char *) m_RecvBuffer;
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


void Connection::HandleIncapacitanceRequest()
{
    long player = *m_RecvBuffer; //we know who this is... But maybe for UDP we wouldn't have

	char string[] = 
		"That was one heck of an explosion! Are you alright over there?\0"
		"\0\3"
		"\0\0\0\0"
		"I need a tow\0"
		"\1\0\0\0"
		"Toggle distress beacon\0"
		"\2\0\0\0"
		"I'm OK\0";


	SendResponse(ENB_OPCODE_0054_TALK_TREE, (unsigned char *) string, sizeof(string));

	AvatarDescription avatar;
	memset(&avatar, 0, sizeof(avatar));
	memcpy(&avatar.avatar_data, &m_Player->Database()->avatar, sizeof(avatar.avatar_data));
	avatar.AvatarID = m_Player->GameID();
	avatar.unknown3 = 1.0;
	avatar.unknown4 = 1.0;
	SendResponse(ENB_OPCODE_0061_AVATAR_DESCRIPTION, (unsigned char *) &avatar, sizeof(avatar));

	// Send Avatar ID to show up in talk tree
	m_Player->PlayerIndex()->SetPIPAvatarID(m_Player->CharacterID());
	m_Player->SendAuxPlayer();

	m_BeaconRequest = true;
}

void Connection::SendWarpIndex(int index)
{
	SendResponse(ENB_OPCODE_009C_WARP_INDEX, (unsigned char*) &index, sizeof(index));
}

void Connection::SetInSpace(bool in_space)
{
    m_Player->SetInSpace(in_space);
}

void Connection::SetActive(bool is_active)
{
    m_Player->SetActive(is_active);
	if (g_Debug && m_Player->AdminLevel() == 80)
	{
		m_Player->SendVaMessage("Connection Active: %s", is_active ? "true" : "false");
	}
}

// Utility functions


// given a string of hex digits convert them into an integer value
static int _httoi(const char *value)
{
	char *storage = (char*)alloca(strlen(value) + 1);
	struct CHexMap
	{
		char chr;
		int value;
	};
	const int HexMapL = 16;
	CHexMap HexMap[HexMapL] =
	{
		{'0', 0}, {'1', 1},
		{'2', 2}, {'3', 3},
		{'4', 4}, {'5', 5},
		{'6', 6}, {'7', 7},
		{'8', 8}, {'9', 9},
		{'A', 10}, {'B', 11},
		{'C', 12}, {'D', 13},
		{'E', 14}, {'F', 15}
	};
	strcpy(storage, value);
	char *p = storage;
	while (*p)
	{
		*p = toupper(*p);
		p++;
	}
	char *s = storage;
	int result = 0;
	if (*s == '0' && *(s + 1) == 'X') s += 2;
	bool firsttime = true;
	while (*s != '\0')
	{
		bool found = false;
		for (int i = 0; i < HexMapL; i++)
		{
			if (*s == HexMap[i].chr)
			{
				if (!firsttime) result <<= 4;
				result |= HexMap[i].value;
				found = true;
				break;
			}
		}
		if (!found) break;
		s++;
		firsttime = false;
	}
	return result;
}

#endif
