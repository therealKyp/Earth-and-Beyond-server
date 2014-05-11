#include "Net7.h"

#include "ServerManager.h"
#include "UDPConnection.h"
#include "PacketStructures.h"
#include "PlayerClass.h"
#include "Opcodes.h"

// This helper function is referenced by _beginthread to launch the TCP thread.
#ifdef WIN32
/*void __cdecl LaunchUDPSendThread(void *arg)
{
    ((UDP_Connection *) arg)->RunSendThread();
    _endthread();
}*/
void __cdecl LaunchUDPRecvThread(void *arg)
{
    ((UDP_Connection *) arg)->RunRecvThread();
    _endthread();
}
#else // Linux
/*void * LaunchUDPSendThread(void *arg)
{
    ((UDP_Connection *) arg)->RunSendThread();
    return NULL;
}*/
void * LaunchUDPRecvThread(void *arg)
{
    ((UDP_Connection *) arg)->RunRecvThread();
    return NULL;
}
#endif

//make a send and receive socket, and start a send and receive thread
UDP_Connection::UDP_Connection(unsigned short port, ServerManager *server_mgr, int server_type)
	: m_ServerType(server_type)
{
	//m_Send_Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	m_Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	m_Port = port;
    m_ServerMgr = server_mgr;
    m_Sector_Operational = true; //false
    m_SectorID = 0;
	m_LoginAttempts = 0;
	m_Socket_Reset_Required = false;

	if(m_Socket == INVALID_SOCKET)
	{
		LogMessage("Invalid Socket %d for UDP connection.\n", GetLastError());
		return;
	}

    UDP_SetBroadcast(m_Socket);

	if (!UDP_BindPort(m_Port, m_Socket))
	{
		m_Socket = INVALID_SOCKET;
		return;
	}

	m_ThreadRunning = false;

	// Launch the Receiver thread
#ifdef WIN32
	_beginthread(&LaunchUDPRecvThread, 0, this);
#else
    pthread_create(&m_Thread, NULL, &LaunchUDPRecvThread, (void *) this);
#endif
}

UDP_Connection::~UDP_Connection()
{
	LogMessage("UDP connection terminates\n");

	m_ThreadRunning = false;

}

ULONG GetLocalAddr()
{
    char localname[MAX_PATH];
    ULONG addr = 0;
#ifdef WIN32
	if (SOCKET_ERROR != gethostname(localname, MAX_PATH))
#else
	if (0 != gethostname(localname, MAX_PATH))
#endif
	{
        struct hostent* hp;
        hp = gethostbyname(localname);
        if (hp != NULL)
        {
            strcpy(localname, hp->h_name);
            addr = *((ULONG *) hp->h_addr_list[0]);
        }
    }

    return addr;
}

bool UDP_Connection::UDP_BindPort(short port, SOCKET socket)
{
	sockaddr_in localAddr;
    ULONG addr;

	memset(&localAddr, 0, sizeof(localAddr));
	localAddr.sin_family = AF_INET;

    addr = GetLocalAddr();

	localAddr.sin_addr.s_addr = addr;//INADDR_ANY; //addr
	localAddr.sin_port = htons(port);

	if (bind(socket, (sockaddr *) &localAddr, sizeof(sockaddr_in)) < 0)
	{
		LogMessage("Set of local port failed.\n");
		return false;
	}
	return true;
}

unsigned long UDP_Connection::checksum(char *buffer, int size)
{
   unsigned long cksum=0;

   while(size > 1) { cksum+=*buffer++; size -= 2; }
   if(size) cksum += *(UCHAR*)buffer;

   cksum = (cksum >> 16) + (cksum & 0xffff);
   cksum += (cksum >>16);
   return (~cksum);
}

void UDP_Connection::UDP_SetBroadcast(SOCKET socket)
{
	int broadcastPermission = 1;
	setsockopt(socket, SOL_SOCKET, SO_BROADCAST, (char *)&broadcastPermission, sizeof(broadcastPermission));
}

void UDP_Connection::RunRecvThread()
{
	long source_addr;
	unsigned short source_port;
	int received;
	EnbUdpHeader *header;
	bool fail = false;

    if (m_Port <= 0)
        LogMessage("UDP_Connection: RunRecvThread - Port not set\n");

    if (m_Socket == INVALID_SOCKET)
        LogMessage("UDP_Connection: RunRecvThread - Invalid Socket\n");

    if (m_ThreadRunning)
        LogMessage("UDP_Connection: RunRecvThread - Thread already running\n");

	m_ThreadRunning = true;

	//LogMessage("Receive thread running.\n");

	while (!g_ServerShutdown && m_ThreadRunning)
	{
		received = UDP_RecvS((char*)m_RecvBuffer, MAX_BUFFER, source_addr, source_port);

		if (received != -1)
		{
			header = (EnbUdpHeader*)m_RecvBuffer;

			unsigned short bytes = header->size - sizeof(EnbUdpHeader);
			short opcode = header->opcode;
			long player_id = header->player_id;
			char *msg = (char*)(m_RecvBuffer + sizeof(EnbUdpHeader));
			m_RecvBuffer[header->size] = 0;

			// Make sure CRC & Bytes match
			if (received == (int)(bytes + sizeof(EnbUdpHeader)) )
			{
				switch (m_ServerType)
				{
				case CONNECTION_TYPE_MVAS_TO_PROXY:
					HandleMVASOpcode(msg, header, source_addr, source_port);
					break;

				case CONNECTION_TYPE_GLOBAL_SERVER_TO_PROXY:
					HandleGlobalOpcode(msg, header, source_addr, source_port);
					break;

				case CONNECTION_TYPE_SECTOR_SERVER_TO_PROXY:
					HandleClientOpcode(msg, header, source_addr, source_port);
					break;

				case CONNECTION_TYPE_MASTER_SERVER_TO_PROXY:
					HandleMasterOpcode(msg, header, source_addr, source_port);
					break;

				default:
					LogMessage("Unknown reception opcode, port %d\n", m_Port);
					break;
				}
			}
		}

		if (m_Socket_Reset_Required)
		{
			m_Socket_Reset_Required = false;
			LogMessage("WARNING: Socket loss, Attempting to reset socket\n");
			Reset_Socket();
		}
	}

    closesocket(m_Socket);
    m_Socket = INVALID_SOCKET;
}

void UDP_Connection::Reset_Socket()
{
	closesocket(m_Socket);

	m_Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if(m_Socket == INVALID_SOCKET)
	{
		LogMessage("Socket Reset: Invalid Socket %d for UDP connection.\n", GetLastError());
		return;
	}

    UDP_SetBroadcast(m_Socket);

	if (!UDP_BindPort(m_Port, m_Socket))
	{
		m_Socket = INVALID_SOCKET;
		return;
	}
}

void UDP_Connection::UDP_Send(const char *buffer, int bufferLen, long IPaddr, short port)
{
	sockaddr_in lSockAddr;
	memset(&lSockAddr, 0, sizeof(lSockAddr));
	lSockAddr.sin_family = AF_INET;
	lSockAddr.sin_addr.s_addr = IPaddr;
	lSockAddr.sin_port = htons(port);

	unsigned char *ip = (unsigned char *) &IPaddr;

	LogDebug("Sending signal to %d (%d.%d.%d.%d)\n",IPaddr, ip[0], ip[1], ip[2], ip[3]);

	// Write out the whole buffer as a single message.
	if (sendto(m_Socket, buffer, bufferLen, 0,
		(sockaddr *) &lSockAddr, sizeof(lSockAddr)) != bufferLen)
	{
		LogMessage("Send failed.\n");
	}
}

void UDP_Connection::SetServerManager(ServerManager * server_mgr)
{
	m_ServerMgr = server_mgr;
}

int UDP_Connection::UDP_RecvS(char *buffer, int size, long &source_addr, unsigned short &source_port)
{
	sockaddr_in clntAddr;
	int rtn;
#ifdef WIN32
	int addrLen = sizeof(clntAddr);

	if ((rtn = recvfrom(m_Socket, buffer, size, 0, (sockaddr *) &clntAddr, (int *) &addrLen)) < 0)
	{
		Sleep(200);
		DWORD dwError = WSAGetLastError();
		if (dwError == WSAENOTSOCK)
		{
			//return to thread and perform a socket reset, see if this helps
			m_Socket_Reset_Required = true;
		}
		//LogMessage("Receive failed.\n");
	}
#else
	socklen_t addrLen = sizeof(clntAddr);

	if ((rtn = recvfrom(m_Socket, buffer, size, 0, (sockaddr *) &clntAddr, &addrLen)) < 0)
	{
		Sleep(200);
		DWORD dwError = WSAGetLastError();
		if (dwError == ENOTSOCK)
		{
			//return to thread and perform a socket reset, see if this helps
			m_Socket_Reset_Required = true;
		}
		//LogMessage("Receive failed.\n");
	}


#endif

	source_addr = clntAddr.sin_addr.s_addr;
	source_port = ntohs(clntAddr.sin_port);

	return rtn;
}

void UDP_Connection::SendOpcode(short opcode, unsigned char *data, size_t length, long player_ip, short port)
{
    unsigned char buffer[2060];

	EnbUdpHeader * header = (EnbUdpHeader *) &buffer[0];
	header->size = (short) length + sizeof(EnbUdpHeader);
	header->opcode = opcode;
    header->player_id = 0;
    header->packet_sequence = 0;

    if ((length + sizeof(EnbUdpHeader)) > 2060)
    {
        LogMessage("UDP send exceeds stack allocation! DEBUG ME: size = %d\n", (length + sizeof(EnbUdpHeader)));
    }
    else
    {
        if (length)
        {
            memcpy(buffer + sizeof(EnbUdpHeader), data, length);
        }
    }

	int bytes = length + sizeof(EnbUdpHeader);

    UDP_Send((const char *) buffer, bytes, player_ip, port);
}

void UDP_Connection::SendOpcode(short opcode, Player *p, unsigned char *data, size_t length, long player_ip, short port, long sequence_num)
{
    unsigned char *buffer = p->GetUDPBuffer();

	EnbUdpHeader * header = (EnbUdpHeader *) &buffer[0];
	header->size = (short) length + sizeof(EnbUdpHeader);
	header->opcode = opcode;
    header->player_id = p->GameID();
    header->packet_sequence = sequence_num;

    if ((length + sizeof(EnbUdpHeader)) > UDP_BUFFER_SEND_SIZE)
    {
        LogMessage("UDP send exceeds player buffer allocation! DEBUG ME: size = %d", (length + sizeof(EnbUdpHeader)));
    }
    else
    {
        if (length)
        {
            memcpy(buffer + sizeof(EnbUdpHeader), data, length);
        }
    }

	int bytes = length + sizeof(EnbUdpHeader);

    //LogMessage("Opcode: %04x, Length: %x\n", opcode, length);
    //DumpBuffer(buffer, bytes);

    UDP_Send((const char *) buffer, bytes, player_ip, port);
}

void UDP_Connection::Shutdown()
{
	closesocket(m_Socket);
	m_Socket = INVALID_SOCKET;
}
