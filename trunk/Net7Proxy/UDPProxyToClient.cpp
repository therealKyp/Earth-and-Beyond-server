#include "Net7.h"
#include "UDPClient.h"
#include "opcodes.h"
#include "PacketStructures.h"
#include "PacketMethods.h"
#include "Connection.h"
#include "ServerManager.h"

void __cdecl ObjectRemoveThread(void *Param);
void __cdecl EffectRemoveThread(void *Param);
void __cdecl ShutdownThread(void *Param);

bool g_ShuttingDown = false;

//Client->Server opcode
//TODO: allow for changing IP
void UDPClient::ForwardClientOpcode(short opcode, short bytes, char *packet)
{
    if (m_PlayerID != 0)
    {
		//LogMessage("Client activity : 0x%x\n", opcode);
        if (m_ServerTCP)
        {
            m_ServerTCP->SendResponse(opcode, (unsigned char*)packet, bytes);
        }
        else
        {
            SendResponse(m_ClientPort, opcode, (unsigned char*)packet, bytes);

			if (m_PreferTCP || m_UsingTCP)//establish TCP connection if required
			{
				EstablishTCPConnection(GetClientIP());
			}
        }
    }
}

extern DWORD time_debug;

//Server->Client direct opcode - we don't see any of these any more.
void UDPClient::ProcessClientOpcode(char *msg, EnbUdpHeader *header)
{
    //this is a raw, direct opcode just for forwarding to the client
    //we need to strip out the TCP opcode
    short opcode = header->opcode;
    short bytes = header->size - sizeof(EnbUdpHeader);
    //long sequence_num = header->packet_sequence;

	//LogMessage("Incomming opcode from Net7 Server: 0x%04x\n", opcode);
    if (time_debug > 0)
    {
        //LogMessage("Incomming opcode from Net7 Server: 0x%04x\n", opcode);
        time_debug--;
    }

    if (!ConnectionActive())
    {
        //LogMessage("Processing opcode when connection inactive: 0x%04x\n",opcode);
    }

    if (g_ServerMgr->m_SectorConnection)
    {
        g_ServerMgr->m_SectorConnection->SendResponse(opcode, (unsigned char *) msg, bytes, header->packet_sequence);
    }

    IncommingOpcodePreProcessing(opcode, msg, bytes);
}

//examine incomming opcodes and react accordingly
void UDPClient::IncommingOpcodePreProcessing(short opcode, char *msg, short bytes, bool tcp)
{
    //LogMessage("<SERVER> --> opcode #%04x\n", opcode);
    switch (opcode)
    {
    case ENB_OPCODE_00BA_LOGOFF_CONFIRMATION:
        LogMessage("---> LogOff confirm\n");
        g_ServerMgr->m_UDPConnection->SetConnectionActive(false);
        g_ServerMgr->m_UDPClient->SetConnectionActive(false);
        g_ServerMgr->m_UDPClient->SetPlayerID(0);
        g_ServerMgr->m_UDPConnection->SetPlayerID(0);
        break;
        
    case ENB_OPCODE_003A_SERVER_HANDOFF:
        LogMessage("---> Server handoff\n");
        g_ServerMgr->m_UDPConnection->SetConnectionActive(false);
        g_ServerMgr->m_UDPClient->SetConnectionActive(false);
        m_Packets.clear();
        m_CurrentPacketNum = -1;
		g_ServerMgr->m_UDPConnection->RecordLastHandoff(msg, bytes);
        break;
        
    case ENB_OPCODE_0005_START:
        if (!tcp)
        {
            g_ServerMgr->m_UDPClient->SetLoginComplete(true);
            g_ServerMgr->m_UDPConnection->KillTCPConnection();
            LogMessage("Terminating TCP link (UDP Login)\n");
        }
        break;
        
    default:
        break;
    }
}

void UDPClient::SendCachedGalaxyMap()
{
    if (g_ServerMgr->m_SectorConnection)
    {
        g_ServerMgr->m_SectorConnection->SendDataFileToClient("GalaxyMap.dat");
    }
}

void UDPClient::SendClientDataFile(char *msg, EnbUdpHeader *header)
{
    //this is a raw, direct opcode just for forwarding to the client
    //we need to strip out the TCP opcode
    m_Resync = true;

    short bytes = header->size - sizeof(EnbUdpHeader);

    short opcode = *((short*) &msg[2]);
    short length = *((short*) &msg[0]);

    if (g_ServerMgr->m_SectorConnection)
    {
        switch (opcode)
        {
        case ENB_OPCODE_0097_GALAXY_MAP:
            SendCachedGalaxyMap();
            break;

        default:
            g_ServerMgr->m_SectorConnection->SendResponse(opcode, (unsigned char *) msg + 4, length - 4, header->packet_sequence);
            break;
        }
    }
}

#define PACKET_BLANK ((char*)0)
#define PACKET_DONE  ((char*)-1)

void UDPClient::SendPacketSequence(char *msg, EnbUdpHeader *header)
{
    ReSend resend;
    if (header->packet_sequence == 0) //reset packet sequence
    {
        m_CurrentPacketNum = 0;
        m_Packets.clear();
		m_PacketDropThisSession = 0;
    }  

    //store packet in sequence
    if (m_Packets[header->packet_sequence] == 0) //should take care of echoes
    {
        char *packet = new char[header->size];
        memcpy(packet, msg, header->size);
        m_Packets[header->packet_sequence] = packet;
    }
    else
    {
        //LogDebug("incomming: %x. Already processed [%x]\n", header->packet_sequence, m_CurrentPacketNum);
    }

    if (header->packet_sequence > m_CurrentPacketNum) //packet too early, or previous packet dropped out
    {
		m_PacketDropThisSession++;
		if (m_PacketDropThisSession > 15 || (header->packet_sequence - m_CurrentPacketNum) > 10) // bad connection, switch client over to TCP
		{
			m_PreferTCP = true;
			g_ServerMgr->m_UDPConnection->SetPreferTCP();
		}
        //LogDebug("incomming: %x. Expected: %x\n", header->packet_sequence, m_CurrentPacketNum);
        if (m_PacketTimeout > 4)
        {
            //find next valid entry and restart
            while (m_Packets[m_CurrentPacketNum] == 0 || m_Packets[m_CurrentPacketNum] == PACKET_DONE)
            {
                m_CurrentPacketNum++;
            }
            //LogDebug(">> unable to recover from packet loss, attempt to resume from packet # %x\n", m_CurrentPacketNum);
			//switch to TCP if we have bad timeouts
			m_PreferTCP = true;
			g_ServerMgr->m_UDPConnection->SetPreferTCP();
        }
        else
        {
            //re-request packet range
            resend.packet_start = m_CurrentPacketNum;
            resend.packet_count = header->packet_sequence - m_CurrentPacketNum;

			//are we allowed to ask for more packets yet?
			unsigned long tick = GetNet7TickCount();
			if ((tick - m_PacketResendTimer) > 250)
			{
				//LogDebug(">> request packet range %x [%x] to be re-sent\n", resend.packet_start, resend.packet_count);
				g_ServerMgr->m_UDPConnection->ForwardClientOpcode(ENB_OPCODE_2017_RESEND_PACKET_SEQUENCE, sizeof(ReSend), (char*)&resend);
				m_PacketResendTimer = tick;
			}

			if (resend.packet_count > 2)
			{
				m_PreferTCP = true;
				g_ServerMgr->m_UDPConnection->SetPreferTCP();
			}

            m_PacketTimeout++;
            return;
        }
    }
    
    //now kick off packet if it's what we're expecting
    while(m_Packets[m_CurrentPacketNum] != 0 && m_Packets[m_CurrentPacketNum] != PACKET_DONE)
    {
        char *msg = m_Packets[m_CurrentPacketNum];
        //LogDebug("Processing packet %x\n", m_CurrentPacketNum);
        SendClientPacketSequence(msg);
        delete [] msg;
        
        m_PacketTimeout = 0;
        m_Packets[m_CurrentPacketNum] = (PACKET_DONE);
        m_CurrentPacketNum++;
    }
}

bool UDPClient::HandleCustomOpcode(short opcode, char *ptr, u8 *tcp_packet, short &tcp_index)
{
	bool handled = true;
	switch (opcode)
	{
	case ENB_OPCODE_2011_GALAXY_MAP_CACHE:
		SendCachedGalaxyMap();
		g_ServerMgr->m_UDPClient->SetReceivedGalaxyMap();
		break;

	case ENB_OPCODE_2012_START_PROSPECT:
		StartProspecting(ptr, tcp_packet, tcp_index);
		break;

	case ENB_OPCODE_2013_TRACTOR_ORE:
		TractorOre(ptr, tcp_packet, tcp_index);
		break;

	case ENB_OPCODE_2014_LOOT_ITEM:
		LootItem(ptr, tcp_packet, tcp_index);
		break;

	case ENB_OPCODE_100A_MVAS_TERMINATE_S_C:
		{
			long player_id = m_PlayerID;
			//shutdown the client if it's still running and then close Net7Proxy
			LogMessage("Shutdown1\n");
			g_ShuttingDown = true;
			g_ServerMgr->m_SectorConnection->QueueResponse(tcp_packet, tcp_index, ENB_OPCODE_0003_LOGOFF, (unsigned char*)&player_id, sizeof(player_id) );
			_beginthread( ShutdownThread, 0, (void *) (0) );
		}
		break;

	default:
		handled = false;
	}

	return handled;
}

void UDPClient::SendClientPacketSequence(char *msg)
{
    EnbUdpHeader *header = (EnbUdpHeader*) msg;
    short bytes = header->size - sizeof(EnbUdpHeader);
    long index = 0;
    short tcp_index = 0;
    unsigned char *tcp_packet = m_QueueBuffer;
    char *ptr = msg + sizeof(EnbUdpHeader);
    bool terminate = false;

    if (!g_ServerMgr->m_SectorConnection) return;

    while (index < bytes && !terminate && !g_ShuttingDown)
    {
        short length = *((short*) &ptr[0]);
        short opcode = *((short*) &ptr[2]);
        //LogMessage("--> opcode #%04x, length: %x\n", opcode, length-4);

        if (length > (bytes - index) || length < 0)
        {
            //bail out here, some kind of error
            //TODO: get this packet re-sent
            LogMessage("Error in packet format for packet %x\n", header->packet_sequence);
            break;
        }

		if (!HandleCustomOpcode(opcode, ptr+4, tcp_packet, tcp_index))
		{
			//LogMessage("<SERVER UDP> --> opcode #%04x\n", opcode);
            if (opcode > 0x0000 && opcode < 0x00FF)
            {
				IncommingOpcodePreProcessing(opcode, (char *) ptr + 4, length - 4);
                g_ServerMgr->m_SectorConnection->QueueResponse(tcp_packet, tcp_index, opcode, (unsigned char *) ptr + 4, length - 4);
            }
            else
            {
                LogMessage("Bad opcode through to Proxy: 0x%04x Length: 0x%x\n", opcode, length);
                //terminate here
                terminate = true;
            }
		}

        ptr += length;
        index += length;
    }

    if (tcp_index > 0)
    {
        g_ServerMgr->m_SectorConnection->SendQueuedPacket(tcp_packet, tcp_index);        
    }
}

void UDPClient::StartProspecting(char *msg, u8* tcp_packet, short &tcp_index)
{
    long player_id = *((long*) &msg[0]);
    long target_id = *((long*) &msg[4]);
    long effect_id = *((long*) &msg[8]);
    long prospect_tick = *((long*) &msg[12]);
    long effect_time = *((long*) &msg[16]);

    if (!g_ServerMgr->m_SectorConnection) return;

    if (tcp_packet == 0)
    {
        tcp_index = 0;
        tcp_packet = m_QueueBuffer;
    }

    if (player_id == m_PlayerID) //we are the originating player
    {
        g_ServerMgr->m_SectorConnection->QueueProspectAUX(tcp_packet, tcp_index, prospect_tick, 0);
        g_ServerMgr->m_SectorConnection->QueueMessageString(tcp_packet, tcp_index, "Prospect ability activated.", 0);
    }

    g_ServerMgr->m_SectorConnection->QueueBeamEffect(tcp_packet, tcp_index, player_id, target_id, 0, 0x00BF, effect_id, prospect_tick);

    //now kick off a thread to remove the effect - this ensures we never get hanging effects, even if the next packet drops
    EffectCancel *cancel_effect = new EffectCancel;
    cancel_effect->effect_id = effect_id;
    cancel_effect->time_delay = effect_time;

    _beginthread( EffectRemoveThread, 0, (void *) (cancel_effect) );
}

void UDPClient::TractorOre(char *ch_msg, u8* tcp_packet, short &tcp_index)
{
    char name[128];
    int index = 0;
    unsigned char *msg = (unsigned char *)ch_msg;
    long player_id = ExtractLong(msg, index);
    long article_id = ExtractLong(msg, index);
    long article_effect_id = ExtractLong(msg, index);
    short resource_basset = ExtractShort(msg, index);
    long prospect_tick = ExtractLong(msg, index);
    ExtractDataLS(msg, name, index);
    long tractor_time = ExtractLong(msg, index);
    float tractor_speed = ExtractFloat(msg, index);
    float obj_pos[3];
    obj_pos[0] = ExtractFloat(msg, index);
    obj_pos[1] = ExtractFloat(msg, index);
    obj_pos[2] = ExtractFloat(msg, index);

    if (!g_ServerMgr->m_SectorConnection) return;

    //now construct the large TCP packet to send in one go (this stops the old problem with the beam shooting off to 0,0,0)
    if (tcp_packet == 0)
    {
        tcp_index = 0;
        tcp_packet = m_QueueBuffer;
    }

    if (player_id == m_PlayerID) //we are the originating player
    {
        g_ServerMgr->m_SectorConnection->QueueProspectAUX(tcp_packet, tcp_index, 0, 1);
        g_ServerMgr->m_SectorConnection->QueueCameraControl(tcp_packet, tcp_index, 0x03000000, ntohl(article_id));
        g_ServerMgr->m_SectorConnection->QueueObjectCreate(tcp_packet, tcp_index, article_id, 1.0f, resource_basset, 4);
        g_ServerMgr->m_SectorConnection->QueueCameraControl(tcp_packet, tcp_index, 0x03000000, ntohl(article_id));
    }
    else
    {
        g_ServerMgr->m_SectorConnection->QueueObjectCreate(tcp_packet, tcp_index, article_id, 1.0f, resource_basset, 4);
    }

    g_ServerMgr->m_SectorConnection->QueueEffect(tcp_packet, tcp_index, player_id, article_id, "TRACTOR", 0x0002, article_effect_id, prospect_tick, tractor_time);
    g_ServerMgr->m_SectorConnection->QueueRelationship(tcp_packet, tcp_index, article_id, RELATIONSHIP_FRIENDLY, 0);
    g_ServerMgr->m_SectorConnection->QueueResourceName(tcp_packet, tcp_index, article_id, name);
    g_ServerMgr->m_SectorConnection->QueueTractorComponent(tcp_packet, tcp_index, obj_pos, tractor_speed, player_id, article_id, article_effect_id, prospect_tick);

    //now kick off a thread to remove the effect
    EffectCancel *cancel_effect = new EffectCancel;
    cancel_effect->effect_id = article_id;
    cancel_effect->time_delay = tractor_time;

    _beginthread( ObjectRemoveThread, 0, (void *) (cancel_effect) );
}

void UDPClient::LootItem(char *ch_msg, u8* tcp_packet, short &tcp_index)
{
    char name[128];
    int index = 0;
    unsigned char *msg = (unsigned char *)ch_msg;
    long player_id = ExtractLong(msg, index);
    long article_id = ExtractLong(msg, index);
    long article_effect_id = ExtractLong(msg, index);
    short resource_basset = ExtractShort(msg, index);
    long prospect_tick = ExtractLong(msg, index);
    ExtractDataLS(msg, name, index);
    long tractor_time = ExtractLong(msg, index);
    float tractor_speed = ExtractFloat(msg, index);
    float obj_pos[3];
    obj_pos[0] = ExtractFloat(msg, index);
    obj_pos[1] = ExtractFloat(msg, index);
    obj_pos[2] = ExtractFloat(msg, index);

    if (!g_ServerMgr->m_SectorConnection) return;

    //now construct the large TCP packet to send in one go (this stops the old problem with the beam shooting off to 0,0,0)

    if (tcp_packet == 0)
    {
        tcp_index = 0;
        tcp_packet = m_QueueBuffer;
    }

    g_ServerMgr->m_SectorConnection->QueueObjectCreate(tcp_packet, tcp_index, article_id, 1.0f, resource_basset, 4);
    g_ServerMgr->m_SectorConnection->QueueRelationship(tcp_packet, tcp_index, article_id, RELATIONSHIP_FRIENDLY, 0);
    g_ServerMgr->m_SectorConnection->QueueResourceName(tcp_packet, tcp_index, article_id, name);
    g_ServerMgr->m_SectorConnection->QueueObjectLinkedEffect(tcp_packet, tcp_index, 0x07, player_id, article_id, 10018, 2, tractor_time, prospect_tick);
    g_ServerMgr->m_SectorConnection->QueueTractorComponent(tcp_packet, tcp_index, obj_pos, tractor_speed, player_id, article_id, article_effect_id, prospect_tick);

    //now kick off a thread to remove the effect
    EffectCancel *cancel_effect = new EffectCancel;
    cancel_effect->effect_id = article_id;
    cancel_effect->time_delay = tractor_time;

    _beginthread( ObjectRemoveThread, 0, (void *) (cancel_effect) );
}

void UDPClient::SendCommsAlive()
{
    SendResponse(m_ClientPort, ENB_OPCODE_3005_PLAYER_COMMS_ALIVE, 0, 0);
}

void __cdecl ObjectRemoveThread(void *Param)
{
	EffectCancel* effect = reinterpret_cast<EffectCancel*>( Param );

    if (effect->time_delay < 200) effect->time_delay = 200;

    Sleep(effect->time_delay);

    if (g_ServerMgr->m_SectorConnection)
    {
        g_ServerMgr->m_SectorConnection->RemoveObject(effect->effect_id);
    }

    delete effect;
    _endthread(); //also kills the thread handle
}

void __cdecl EffectRemoveThread(void *Param)
{
	EffectCancel* effect = reinterpret_cast<EffectCancel*>( Param );

    if (effect->time_delay < 200) effect->time_delay = 200;

    Sleep(effect->time_delay);

    if (g_ServerMgr->m_SectorConnection)
    {
        g_ServerMgr->m_SectorConnection->SendRemoveEffect(effect->effect_id);
    }

    delete effect;
    _endthread(); //also kills the thread handle
}

void __cdecl ShutdownThread(void *Param)
{
    Sleep(1000);

    g_ServerShutdown = true;
	ShutdownClient();

    _endthread(); //also kills the thread handle
}