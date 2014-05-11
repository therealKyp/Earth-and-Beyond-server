// ClientToSectorServer.cpp

#include "Net7.h"
#include "Connection.h"
#include "Opcodes.h"
#include "ServerManager.h"
#include "PacketStructures.h"
#include "UDPClient.h"
#include "PacketMethods.h"

DWORD time_debug = 0;

void Connection::ProcessSectorServerOpcode(short opcode, short bytes)
{
	unsigned long tick = GetNet7TickCount();
	//LogMessage("<CLIENT> ---> 0x%04x\n", opcode);
    //this can be used to intercept messages from the client
    switch (opcode)
    {
        case ENB_OPCODE_0002_LOGIN:
            g_LoggedIn = true;
            g_ServerMgr->m_SectorConnection = this;
            LogMessage("Sending Login packet - connection active\n");
            g_ServerMgr->m_UDPConnection->SetConnectionActive(true);
            g_ServerMgr->m_UDPClient->SetConnectionActive(true);
            time_debug = 50;
            break;

		case ENB_OPCODE_0006_START_ACK :
			g_ServerMgr->m_UDPConnection->ForwardClientOpcode(opcode, bytes, (char*)m_RecvBuffer);
			if (g_ServerMgr->m_UDPClient->GetSectorID() > 9999 || g_ServerMgr->m_UDPConnection->PreferTCP())
			{
				long player_id = g_ServerMgr->m_UDPClient->PlayerID();
				g_ServerMgr->m_UDPConnection->ForwardClientOpcode(ENB_OPCODE_3008_STARBASE_LOGIN_COMPLETE, sizeof(player_id), (char *) &player_id);
				g_ServerMgr->m_UDPClient->SetLoginComplete(true);
			}
			else
			{
				long player_id = g_ServerMgr->m_UDPClient->PlayerID();
				g_ServerMgr->m_UDPConnection->ForwardClientOpcode(ENB_OPCODE_3004_PLAYER_SHIP_SENT, sizeof(player_id), (char *) &player_id);
				g_ServerMgr->m_UDPConnection->KillTCPConnection();
				g_ServerMgr->m_UDPClient->SetLoginComplete(true);
			}
			LogMessage("Sending StartAck to Server %d\n", *((long *) &m_RecvBuffer[0]) );
			return;
			break;

		case ENB_OPCODE_0014_MOVE :
			break;

		case ENB_OPCODE_0012_TURN :
			//only send one of these per 0.25 secs
			if (tick > (m_Turn_Sent + 250))
			{
				g_ServerMgr->m_UDPConnection->ForwardClientOpcode(opcode, bytes, (char*)m_RecvBuffer);
				m_Turn_Sent = tick;
			}
			return;
			break;

		case ENB_OPCODE_0013_TILT :
			//only send one of these per 0.25 secs
			if (tick > (m_Tilt_Sent + 250))
			{
				g_ServerMgr->m_UDPConnection->ForwardClientOpcode(opcode, bytes, (char*)m_RecvBuffer);
				m_Tilt_Sent = tick;
			}
			return;
			break;

        case ENB_OPCODE_00B9_LOGOFF_REQUEST:
            g_LoggedIn = true;
            //drop through
        case ENB_OPCODE_003A_SERVER_HANDOFF:
            //DumpBuffer(m_RecvBuffer, bytes);
            //LogMessage("Handoff\n", opcode);
            //g_ServerMgr->m_UDPConnection->SetConnectionActive(false);
            //g_ServerMgr->m_UDPClient->SetConnectionActive(false);
            break;

        case ENB_OPCODE_002C_ACTION:
            g_ServerMgr->m_UDPConnection->ForwardClientOpcode(opcode, bytes, (char*)m_RecvBuffer);
            ProcessAction();
            return;
            break;

		case ENB_OPCODE_009F_STARBASE_ROOM_CHANGE :
			HandleStarbaseRoomChange();
			break;

		case ENB_OPCODE_009B_WARP:
			HandleWarp();
			break;

        default:
            //LogMessage("Forwarding opcode to server 0x%04x\n", opcode);
            break;
    }

    //forward the opcode to the server via UDP
    g_ServerMgr->m_UDPConnection->ForwardClientOpcode(opcode, bytes, (char*)m_RecvBuffer);
}

bool Connection::HandleCustomOpcode(short opcode, short bytes)
{
    short tcp_index = 0;
    unsigned char *tcp_packet = g_ServerMgr->m_UDPConnection->GetQueueBuffer();

	bool handled;
	handled = g_ServerMgr->m_UDPConnection->HandleCustomOpcode(opcode, (char *) m_RecvBuffer, tcp_packet, tcp_index);
    if (tcp_index > 0)
    {
		g_ServerMgr->m_SectorConnection->SendQueuedPacket(tcp_packet, tcp_index);        
    }

	return handled;
}

//static bool set_start = false;

void Connection::ProcessProxyServerOpcode(short opcode, short bytes)
{
	if (g_ServerMgr->m_SectorConnection)
	{
		//LogMessage("<SERVER TO CLIENT TCP> ----> %04x [%04x]\n", opcode, bytes);
		/*if (opcode == ENB_OPCODE_0005_START && set_start == false)
		{
			set_start = true;
			LogMessage("Simulate start dropout\n");
			return;
		}*/
		
		if (opcode < 1000) g_ServerMgr->m_SectorConnection->SendResponse(opcode, (unsigned char *) m_RecvBuffer, bytes);

		if (opcode == ENB_OPCODE_0005_START)
		{
			LogMessage("Received start packet. Start ID was %d\n", *((long *) &m_RecvBuffer[0]) );
			g_ServerMgr->m_UDPClient->SetStartReceived();
			g_ServerMgr->m_UDPClient->SetStartID(*((long *) &m_RecvBuffer[0]));
			//set_start = false;
		}
		if (!HandleCustomOpcode(opcode, bytes))
		{
			g_ServerMgr->m_UDPConnection->IncommingOpcodePreProcessing(opcode, (char *) m_RecvBuffer, bytes, true);
		}
	}
	else
	{
		LogMessage("Incomming TCP proxy opcode but no valid connection [%04x]\n",opcode);
	}
}

void Connection::SendDataFile(unsigned char *buffer, short length)
{
	if (buffer)
	{
        DumpBuffer(buffer, length);
        //m_CryptOut.RC4(buffer, length);
        Send(buffer, length);
    }
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
			//m_CryptOut.RC4(buffer, length);
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

void Connection::SendRemoveEffect(int target_id)
{
	SendResponse(ENB_OPCODE_000F_REMOVE_EFFECT, (unsigned char *) &target_id, sizeof(int));
}

void Connection::RemoveObject(long target_id)
{
	SendResponse(ENB_OPCODE_0007_REMOVE, (unsigned char *) &target_id, sizeof(int));
}

void Connection::SendStarbaseSet(long sector, char action, char exit_mode)
{
	StarbaseSet starbase_set;
	memset(&starbase_set, 0, sizeof(starbase_set));
	starbase_set.StarbaseID = sector;
	starbase_set.Action = action;
	starbase_set.ExitMode = exit_mode;

	SendResponse(ENB_OPCODE_004F_STARBASE_SET, (unsigned char *) &starbase_set, sizeof(starbase_set));
}

void Connection::SendCameraControl(long Message, long GameID)
{
	CameraControl data;
	data.Message = Message;
	data.GameID = GameID;

	SendResponse(ENB_OPCODE_0092_CAMERA_CONTROL, (unsigned char *) &data, sizeof(data));
}

void Connection::SendStart(long start_id)
{
	SendResponse(ENB_OPCODE_0005_START, (unsigned char *) &start_id, sizeof(start_id));
}

///////////////////////////////////////////////////////////
//
// The following methods are used for queueing packets to create 
// multiopcode packets. These are needed for mining, to make sure the
// beam doesn't shoot off to 0,0,0
//
///////////////////////////////////////////////////////////

void Connection::QueueCameraControl(u8 *packet, short &index, long message, long game_id)
{
	CameraControl data;
	data.Message = message;
	data.GameID = game_id;

	QueueResponse(packet, index, ENB_OPCODE_0092_CAMERA_CONTROL, (unsigned char *) &data, sizeof(data));
}

void Connection::QueueObjectCreate(u8 *packet, short &index, int game_id, float scale, short asset, int type, float h, float s, float v)
{
	Create  create;
	create.GameID = game_id;
	create.Scale = scale;
	create.BaseAsset = asset;
	create.Type = (char) type;
	create.HSV[0] = h;
	create.HSV[1] = s;
	create.HSV[2] = v;

	QueueResponse(packet, index, ENB_OPCODE_0004_CREATE, (unsigned char *) &create, sizeof(create));
}

void Connection::QueueEffect(u8* packet, short &index, int player_id, int target_id, char *message, short effect_type, long effect_id, long timestamp, long effect_time)
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
	Prospect.EffectDescID = effect_type;
	Prospect.Message = message;
	Prospect.EffectID = effect_id;
    Prospect.Duration = short(effect_time);
	Prospect.TimeStamp = timestamp;
	
	QueueObjectToObjectEffect(packet, index, &Prospect);         
}

void Connection::QueueObjectToObjectEffect(u8* packet, short &p_index, ObjectToObjectEffect *obj_effect)
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

    QueueResponse(packet, p_index, ENB_OPCODE_000B_OBJECT_TO_OBJECT_EFFECT, effect, index);
}

void Connection::QueueRelationship(u8* packet, short &index, long ObjectID, long Reaction, bool IsAttacking)
{
	Relationship response;
	response.ObjectID = ntohl(ObjectID);
	response.Reaction = Reaction;
	response.IsAttacking = IsAttacking ? 1 : 0;

	//LogMessage("Sending Relationship packet\n");
	QueueResponse(packet, index, ENB_OPCODE_0089_RELATIONSHIP, (unsigned char *) &response, sizeof(response));
}

void Connection::QueueResourceName(u8* packet, short &index, long resourceID, char *resource_name)
{
	unsigned char aux_data[64]; 
	memset(aux_data, 0, 64);
	short length = strlen(resource_name);
	*((long *) aux_data) = resourceID;
	*((short *) &aux_data[4]) = length + 4;
	*((short *) &aux_data[6]) = 0x1201;
	*((short *) &aux_data[8]) = length;

	strncpy((char*)&aux_data[10], resource_name, length);

	QueueResponse(packet, index, ENB_OPCODE_001B_AUX_DATA, aux_data, length+10);	
}

void Connection::QueueTractorComponent(u8* packet, short &index, float *position, float tractor_speed, long player_id, long article_id, long effect_id, long timestamp)
{
	ComponentPositionalUpdate update;
	memset(&update, 0, sizeof(update));

	update.simple.GameID = article_id;
	if (timestamp)
	{
		update.simple.TimeStamp = timestamp;
	}
	else
	{
		update.simple.TimeStamp = GetNet7TickCount();
	}
	update.simple.Position[0] = position[0];
	update.simple.Position[1] = position[1];
	update.simple.Position[2] = position[2];
	update.simple.Orientation[0] = 1.0f;
	update.simple.Orientation[1] = 0.0f;
	update.simple.Orientation[2] = 0.0f;
	update.simple.Orientation[3] = 0.0f;
	update.ImpartedDecay = 9.8f;
	update.TractorSpeed = tractor_speed;
	update.TractorID = player_id;
	update.TractorEffectID = effect_id;

	QueueResponse(packet, index, ENB_OPCODE_0046_COMPONENT_POSITIONAL_UPDATE, (unsigned char *) &update, sizeof(update));
}

void Connection::QueueStarbaseSet(u8* packet, short &index, long sector, char action, char exit_mode)
{
	StarbaseSet starbase_set;
	memset(&starbase_set, 0, sizeof(starbase_set));
	starbase_set.StarbaseID = sector;
	starbase_set.Action = action;
	starbase_set.ExitMode = exit_mode;

	QueueResponse(packet, index, ENB_OPCODE_004F_STARBASE_SET, (unsigned char *) &starbase_set, sizeof(starbase_set));
}

void Connection::QueueContrails(u8* packet, short &index, long player_id, bool contrails)
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

	QueueResponse(packet, index, ENB_OPCODE_001B_AUX_DATA, aux_data, sizeof(aux_data));
}

void Connection::QueueStart(u8* packet, short &index, long start_id)
{
	QueueResponse(packet, index, ENB_OPCODE_0005_START, (unsigned char *) &start_id, sizeof(start_id));
}

void Connection::QueueObjectLinkedEffect(u8* packet, short &index, short bitmask, long avatar_id, long UID, long effectID, short effectDID, long effect_time, long timestamp)
{
	//send an effect linked to our ship
	ObjectToObjectEffect OBTOBE;

	OBTOBE.Bitmask = bitmask;
	OBTOBE.GameID = avatar_id;
	OBTOBE.TargetID = UID;
	OBTOBE.EffectDescID = effectDID;
	OBTOBE.Message = 0;
	OBTOBE.EffectID = effectID;
	OBTOBE.Duration = short(effect_time);
	OBTOBE.TimeStamp = timestamp;

    QueueObjectToObjectEffect(packet, index, &OBTOBE);
}

void Connection::QueueBeamEffect(u8* packet, short &index, long player_id, long target_id, char *message, short effect_type, long effectUID, long timestamp, short effect_time)
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
	
	QueueObjectToObjectEffect(packet, index, &Prospect);
}                

void Connection::QueueMessageString(u8* packet, short &index, char *msg, char color, bool log)
{
	char buffer[512];
	memset(buffer, 0, sizeof(buffer));
	short length = strlen(msg) + 1;
	*((short *) &buffer[0]) = length;
	buffer[2] = color;
	strcpy(&buffer[3], msg);

	QueueResponse(packet, index, ENB_OPCODE_001D_MESSAGE_STRING, (unsigned char *) buffer, length + 3);
}

//TODO: REMOVE THIS!! May interfere with cloaking
void Connection::QueueProspectAUX(u8* packet, short &index, long value, int type)
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
			
			QueueResponse(packet, index, ENB_OPCODE_001B_AUX_DATA, aux_data, sizeof(aux_data));
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
			
			QueueResponse(packet, index, ENB_OPCODE_001B_AUX_DATA, aux_data, sizeof(aux_data));
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
			QueueResponse(packet, index, ENB_OPCODE_001B_AUX_DATA, aux_data, sizeof(aux_data));
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
			
			QueueResponse(packet, index, ENB_OPCODE_001B_AUX_DATA, aux_data, sizeof(aux_data));
		}
		break;
	}
}

////////////////////////////////////////////////////////////////
//
// This sends the player_id to the remote server, via the temporary login link
// After this, the server knows who owns the new link, and can match the link to a Player object

void Connection::SendPlayerConnectionConfirm(long player_id)
{
    SendResponse(ENB_OPCODE_3002_TCP_LOGIN_VALIDATE, (unsigned char *) &player_id, sizeof(player_id));
}

void Connection::SendLoginLink(UDPClient *client)
{
	long blank = 0;
	SendResponse(ENB_OPCODE_2010_SET_GLOBAL_LOGIN_LINK, (unsigned char *) &blank, sizeof(blank));
	m_ServerType = CONNECTION_TYPE_GLOBAL_PROXY_TO_SERVER;
	m_UDPClient = client;
}

void Connection::SetToProxyLink()
{
	long blank = 0;
	LogMessage("Setting TCP link to proxy.\n");
	SendResponse(ENB_OPCODE_2011_SET_PROXY_SECTOR_LINK, (unsigned char *) &blank, sizeof(blank));
	m_ServerType = CONNECTION_TYPE_SECTOR_SERVER_TO_PROXY;
}

void Connection::ProcessAction()
{
	ActionPacket * myAction = (ActionPacket *) m_RecvBuffer;
	
    //LogDebug("Action - ID: %d, Action: %d, Target: %d, OptVar: %d\n",myAction->GameID, myAction->Action, myAction->Target, myAction->OptionalVar);
    //g_ServerMgr->m_UDPConnection->ForwardClientOpcode(opcode, bytes, (char*)m_RecvBuffer);

	switch (myAction->Action)
	{
        case 7:     //docking complete
            //LogMessage("Finished docking\n");
            break;

		case 8:		//land
            //LogMessage("Finished landing\n");
            break;

		case 18:	//gate button
            //LogMessage("Gate Request.\n");
            g_ServerMgr->m_UDPConnection->EstablishTCPConnection(g_ServerMgr->m_UDPConnection->GetClientIP());
            break;
		
		case 19:	//finish gate sequence
            //LogMessage("Finish Gate.\n");
			break;

   		case 28:	//dock
            //LogMessage("dock request\n");
            g_ServerMgr->m_UDPConnection->EstablishTCPConnection(g_ServerMgr->m_UDPConnection->GetClientIP());
            break;

		case 29:	//planet landing button
            //LogMessage("Pressed planet land\n");
            g_ServerMgr->m_UDPConnection->EstablishTCPConnection(g_ServerMgr->m_UDPConnection->GetClientIP());
            break;
    }
}

void Connection::HandleStarbaseRoomChange()
{
    StarbaseRoomChange *change = (StarbaseRoomChange *) m_RecvBuffer;

    if (change->NewRoom == -1)
    {
        //LogMessage("Leaving starbase?\n");
        g_ServerMgr->m_UDPConnection->EstablishTCPConnection(g_ServerMgr->m_UDPConnection->GetClientIP());
    }
}

void Connection::HandleWarp()
{
	//send client position update first, so we don't get rubber banding
	g_ServerMgr->m_UDPClient->SendPositionIfChanged();
}