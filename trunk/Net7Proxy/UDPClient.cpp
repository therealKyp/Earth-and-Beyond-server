#include "Net7.h"
#include "UDPClient.h"
#include "opcodes.h"
#include "PacketStructures.h"
#include "ServerManager.h"

void __cdecl LaunchUDPCRecvThread(void *arg)
{
    ((UDPClient *) arg)->RecvThread();
    _endthread();
}

void __cdecl LaunchUDPMVASThread(void *arg)
{
    ((UDPClient *) arg)->MVASThread();
    _endthread();
}

bool UDPClient::OpenFixedPort(short port, long ip_addr)
{
    bool success = true;
    ULONG uAddr = 0;
    char strLocal[MAX_PATH] = { 0 };

    m_Listen_Socket = socket(AF_INET, SOCK_DGRAM, 0);
    
    if(m_Listen_Socket == INVALID_SOCKET)
    {
        LogMessage("Invalid Socket %d. Program Aborted\n", GetLastError());
        success = false;
    }

	if (SOCKET_ERROR != gethostname(strLocal, MAX_PATH))
	{
        struct hostent* hp;
        hp = gethostbyname(strLocal);
        if (hp != NULL)	
        {
            strcpy(strLocal, hp->h_name);
            uAddr = *((ULONG *) hp->h_addr_list[0]);
        }
    }

    CreateFrom(uAddr,0);

    if ( SOCKET_ERROR == bind(m_Listen_Socket, (sockaddr *) &m_SockAddr, sizeof(m_SockAddr)))
    {
        LogMessage("Socket Bind error '%d'. Connection not established\n", GetLastError());
        closesocket( m_Listen_Socket );
        success = false;
    }

    CreateFrom(ip_addr, port);

	if (SOCKET_ERROR == connect( m_Listen_Socket, (sockaddr *) &m_SockAddr, sizeof(m_SockAddr)))
    {
        closesocket( m_Listen_Socket );
        success = false;
    }

    return success;
}

bool UDPClient::OpenMultiPort(short port, long ip_addr)
{
    bool success = true;

    m_Listen_Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	
    if(m_Listen_Socket == INVALID_SOCKET)
    {
        LogMessage("Invalid Socket %d. Program Aborted\n", GetLastError());
        success = false;
    }

    SetBroadcast(m_Listen_Socket);

    return success;
}

UDPClient::UDPClient(short port, short connection_type, long ip_addr)
{
    m_IPAddr = ip_addr;
    m_Port = port;
    m_ConnectionType = connection_type;
    m_logged_in = false;
    m_global_account_rcv = false;
    m_PlayerID = 0;
    memset(m_AccountName, 0, 128);
    m_ClientPort = 0;
    m_ClientIP = 0;
    m_ConnectionActive = false;
    m_AlternatePorts = false;
    m_ServerTCP = (0);
    m_CurrentPacketNum = 0;
    bool success = false;
    m_Resync = true;
	m_PreferTCP = false;
	m_UsingTCP = false;
    m_PacketTimeout = 0;
	m_PacketDropThisSession = 0;
    m_Packets.clear();

	memset(&m_Server_handoff, 0, sizeof(m_Server_handoff));
	m_PacketResendTimer = 0;

    //if we want a UDP port that can be used to send messages to different ports, it needs to be opened in a different way
    switch (connection_type)
    {
    case CLIENT_TYPE_FIXED_PORT:
        success = OpenFixedPort(port, ip_addr);
        break;

    case CLIENT_TYPE_MULTI_PORT:
        success = OpenMultiPort(port, ip_addr);
        break;

    default:
        LogMessage("Bad port type.\n");
        return;
        break;
    }

    if (!success)
    {
        LogMessage("Error while opening UDP port\n.");
        return;
    }
    
    _beginthread(&LaunchUDPCRecvThread, 0, this);

    if (connection_type == CLIENT_TYPE_FIXED_PORT)
    {
        _beginthread(&LaunchUDPMVASThread, 0, this);
    }
}

UDPClient::~UDPClient()
{

}

void UDPClient::CreateFrom(long ip_addr, short port)
{
	memset(&m_SockAddr, 0, sizeof(m_SockAddr));
	m_SockAddr.sin_addr.s_addr = ip_addr;
	m_SockAddr.sin_port = htons( port );
	m_SockAddr.sin_family = AF_INET;
}

void UDPClient::SetBroadcast(SOCKET socket) 
{
	// If this fails, we'll hear about it when we try to send.  
	int broadcastPermission = 1;
	setsockopt(socket, SOL_SOCKET, SO_BROADCAST, (char *) &broadcastPermission, sizeof(broadcastPermission));
} 

unsigned long UDPClient::checksum(char *buffer, int size) 
{ 
   unsigned long cksum=0; 

   while(size > 1) { cksum+=*buffer++; size -= 2; }
   if(size) cksum += *(UCHAR*)buffer; 
 
   cksum = (cksum >> 16) + (cksum & 0xffff); 
   cksum += (cksum >>16); 
   return (~cksum); 
}

void UDPClient::RecvThread()
{
	int received;
	EnbUdpHeader *header;
    short s_null = 0;

	m_recv_thread_running = true;

	while (!g_ServerShutdown && m_recv_thread_running)
    {	
		received = UDP_RecvFromServer((char*)m_RecvBuffer, MAX_UDPC_BUFFER);

		if (received > 0)
		{
			header = (EnbUdpHeader*)m_RecvBuffer;
			
			unsigned short bytes = header->size - sizeof(EnbUdpHeader);
			short opcode = header->opcode;
            long player_id = header->player_id;
            char *msg = (char*)(m_RecvBuffer + sizeof(EnbUdpHeader));
			//unsigned long crc = checksum(msg, bytes);
			
			// Make sure we recived the correct # of byes 
			if (received == (int)(bytes + sizeof(EnbUdpHeader)))
			{
				switch (opcode)
				{
				case ENB_OPCODE_1001_MVAS_LOGIN_S_C:
                    m_logged_in = true;
					break;
										
				case ENB_OPCODE_1007_MVAS_TOGGLE_SEND_FREQ_S_C:
                    ToggleSendFrequency(msg);
					break;

                case ENB_OPCODE_1009_MVAS_BAD_LOGIN_S_C:
                    SignalWrongVersion(msg);
                    m_recv_thread_running = false;
                    break;

                case ENB_OPCODE_100A_MVAS_TERMINATE_S_C:
                    TerminateClient(msg);
                    break;

                case ENB_OPCODE_2001_ACCOUNTVALIDATE:
                    ValidateAccount(msg, header);
                    break;

                case ENB_OPCODE_2003_AVATARLIST:
                    ReceiveAvatarList(msg, header);
                    break;

                case ENB_OPCODE_2005_AVATARLOGIN_CONFIRM:
                    AvatarLoginConfirm(msg, header);
                    break;
					
                case ENB_OPCODE_2009_MASTER_HANDOFF_CONFIRM:
                    MasterLoginConfirm(msg, header);
                    break;

                case ENB_OPCODE_200C_CREATE_DELETE_AVATAR_CONFIRM:
                    CreateDeleteConfirm(msg, header);
                    break;

                case ENB_OPCODE_200E_GLOBAL_ERROR:
                    ProcessGlobalError(msg, header);
                    break;

                case ENB_OPCODE_2010_DATA_FILE:
                    SendClientDataFile(msg, header);
                    break;

                case ENB_OPCODE_2016_PACKET_SEQUENCE:
                    SendPacketSequence((char*)m_RecvBuffer, header);
                    break;

                case ENB_OPCODE_2011_GALAXY_MAP_CACHE:
                    SendCachedGalaxyMap();
                    break;

                case ENB_OPCODE_2012_START_PROSPECT:
                    StartProspecting(msg, 0, s_null);
                    break;

                case ENB_OPCODE_2013_TRACTOR_ORE:
                    TractorOre(msg, 0, s_null);
                    break;

                case ENB_OPCODE_2014_LOOT_ITEM:
                    LootItem(msg, 0, s_null);
                    break;

                case ENB_OPCODE_3006_PLAYER_LOGIN_FAILED_CONFIRM:
                    LoginFailAck();
                    break;

                default:
                    ProcessClientOpcode(msg, header);
                    break;
				}
			}
            else 
            {
                LogMessage("Message read failure... opcode = 0x%04x length = 0x%x (rcvd = %x)\n",opcode, bytes, received);
                LogMessage("Opcode 0x%04x: Length: 0x%04x\n", opcode, bytes);
                LogMessage("Resend opcode.\n");
                ReSend send;
                send.packet_count = 1;
                send.packet_start = header->packet_sequence;
                g_ServerMgr->m_UDPConnection->ForwardClientOpcode(ENB_OPCODE_2017_RESEND_PACKET_SEQUENCE, sizeof(ReSend), (char*)&send);
                //DumpBuffer((unsigned char*)msg, received - sizeof(EnbUdpHeader));
            }
		}
	}

	closesocket(m_Listen_Socket);

	_endthread();
}

int UDPClient::UDP_RecvFromServer(char *buffer, int size)
{
	int rtn;

	if ((rtn = recv(m_Listen_Socket, buffer, size, 0)) < 0) 
	{
		Sleep(200); //little pause in here if we get a -1
		//fprintf(stderr,"Received failed (recv()) %d\n",rtn);
	}
	
	return rtn;
}

void UDPClient::SignalWrongVersion(char *msg)
{
    float version = (float)*((long *) &msg[0]);

    version = version*0.01f;
    char msg2[256];

	sprintf(msg2, "Incompatible version of Net7Proxy.\nLatest version is %.2f", version);
	::MessageBox(NULL, msg2, "Net7Proxy", MB_ICONERROR);

    fprintf(stderr,"\nIncompatible version of Net7Proxy.");
    fprintf(stderr,"\nYou need to upgrade to version %.2f",version);
    fprintf(stderr,"\nMake sure you are using the latest LanuchNet7,\nIt should auto-patch the Net7Proxy to the latest version.\n");
}

bool UDPClient::VerifyConnection()
{
	long count = 5;

	long version = VERSION_N;

    LogMessage("Sending login.\n");

	while (count > 0 && m_logged_in == false)
	{
		SendResponse(MVAS_LOGIN_PORT, ENB_OPCODE_1000_MVAS_REGISTER_C_S, (unsigned char *) &version, sizeof(version));
		Sleep(1000);
		count--;
	}

	if (m_logged_in)
	{
		LogMessage("login OK\n");
    }
	else
	{
        LogMessage("login failed - server not found.\n");
		return false;
	}
	   
	return true;
}

void UDPClient::SendResponse(short port, short opcode, unsigned char *data, size_t length)
{
	EnbUdpHeader * header = (EnbUdpHeader *) m_SendBuffer;
	header->size = (short) length + sizeof(EnbUdpHeader);
	header->opcode = opcode;
    header->player_id = m_PlayerID;
	if (length)
	{
		memcpy(m_SendBuffer + sizeof(EnbUdpHeader), data, length);
	}

	int bytes = length + sizeof(EnbUdpHeader);

    //LogMessage("Sending1 opcode 0x%04x\n", opcode);

    UDP_Send(port, (char*)&m_SendBuffer[0], bytes);
}

void UDPClient::SendResponse(long player_id, short port, short opcode, unsigned char *data, size_t length)
{
	EnbUdpHeader * header = (EnbUdpHeader *) m_SendBuffer;
	header->size = (short) length + sizeof(EnbUdpHeader);
	header->opcode = opcode;
    header->player_id = player_id;

	if (length)
	{
		memcpy(m_SendBuffer + sizeof(EnbUdpHeader), data, length);
	}

	int bytes = length + sizeof(EnbUdpHeader);

    //LogMessage("Sending2 opcode 0x%04x\n", opcode);

    UDP_Send(port, (char*)&m_SendBuffer[0], bytes);
}

void UDPClient::UDP_Send(short port, const char *buffer, int bufferLen) 
{
    int buffsend;
    
    switch (m_ConnectionType)
    {
    case CLIENT_TYPE_MULTI_PORT:
        {
            SOCKADDR_IN lSockAddr;
            memset(&lSockAddr,0, sizeof(lSockAddr));
            lSockAddr.sin_family = AF_INET;
            lSockAddr.sin_port = htons(port);
            lSockAddr.sin_addr.s_addr = m_IPAddr; //inet_addr(m_IPAddr);
            
            // Write out the whole buffer as a single message.
            if (buffsend = sendto(m_Listen_Socket, buffer, bufferLen, 0,
                (sockaddr *) &lSockAddr, sizeof(lSockAddr)) != bufferLen) 
            {
                fprintf(stderr,"\nSend failed %d %d", bufferLen, buffsend);
            }
        }
        break;
        
    case CLIENT_TYPE_FIXED_PORT:
        {
            buffsend = send(m_Listen_Socket, buffer, bufferLen, 0);
        }
        break;
    }
}

void UDPClient::WaitForResponse()
{
    int count = 20;
	while (count > 0 && m_global_account_rcv == false)
	{
        for (int i = 0; i < 4; i++)
        {
            if (m_global_account_rcv) break;
            Sleep(250);
        }
		count--;
	}
}

void UDPClient::WaitForResponse(short port, short opcode, unsigned char *data, size_t length)
{
    int count = 20;
	while (count > 0 && m_global_account_rcv == false)
	{
		SendResponse(port, opcode, data, length);
        for (int i = 0; i < 4; i++)
        {
            if (m_global_account_rcv) break;
            Sleep(250);
        }
		count--;
	}
}

void UDPClient::RecordLastHandoff(char *msg, short bytes)
{
	memset(&m_Server_handoff, 0, sizeof(m_Server_handoff));
	memcpy(&m_Server_handoff, msg, bytes);
}
