// Connection.cpp

#include "Net7.h"
#include "Connection.h"
#include "ServerManager.h"
#include "PacketStructures.h"
#include "PlayerClass.h"

Connection::Connection()
{
    // Mark this as an active connection
	m_ConnectionActive = true;
	m_Socket = 0;
	m_TcpPort = 0;
	m_ServerType = 0;
	m_IPaddr = 0;
	m_AvatarID = 0;
	m_LastOwner = (0);
	m_RecvThreadHandle = 0;

    // Thread is not running (yet)
    m_TcpThreadRunning = false;

    // Disable packet logging
    m_PacketLoggingEnabled = false;

	m_SendQueue = new MessageQueue(g_ServerMgr->GetTCPCBuffer());

	// Don't launch the receive thread until connection is called into effect.
}

Connection * Connection::ReSetConnection(SOCKET s, ServerManager &server_mgr, short port, int server_type, unsigned long* ip_addr)
{
	m_Socket = s;
	m_TcpPort = port;
	m_ServerType = server_type;
    // Mark this as an active connection
	m_ConnectionActive = true;

    // Thread is not running (yet)
    m_TcpThreadRunning = false;

    // Disable packet logging
    m_PacketLoggingEnabled = false;

	m_LoginHandoff = false;

	if (ip_addr)
	{
		m_IPaddr = *ip_addr;
	}

	// Launch the Key exchange - send 'thread' is blipped by using PulseConnectionOutput
	RunKeyExchange();

	//if no receive thread, start one, otherwise restart the thread. The thread will always be in a suspended state
	if (m_RecvThreadHandle)
	{
		LogMessage("Reanimating receive thread\n");
		m_TcpThreadRunning = true;
#ifdef WIN32
		ResumeThread(m_RecvThreadHandle);
#else
#endif
	}
	else
	{
		LogMessage("Creating new receive thread\n");
		//need to create a receive thread for this connection
#ifdef WIN32
		DWORD uiThreadId;
		m_RecvThreadHandle = CreateThread (NULL, 2048, SocketRecvThread, this, 0, &uiThreadId);
#else
		pthread_create(&m_RecvThreadHandle, NULL, SocketRecvThread, this);
#endif
	}

	return this;
}

Connection::Connection(SOCKET s, ServerManager &server_mgr, short port, int server_type, unsigned long* ip_addr)
	: m_TcpPort(port),
	  m_Socket(s),
	  m_ServerType(server_type)
{
    // Mark this as an active connection
	m_ConnectionActive = true;

    // Thread is not running (yet)
    m_TcpThreadRunning = false;

    // Disable packet logging
    m_PacketLoggingEnabled = false;

	m_LoginHandoff = false;

	if (ip_addr)
	{
		m_IPaddr = *ip_addr;
	}

	LogMessage("I shouldn't be here. There's nothing for you here!\n");

	m_SendQueue = new MessageQueue(g_ServerMgr->GetTCPCBuffer());

	// Launch the Key exchange - send and receive are blipped by using PulseConnectionInputs and PulseConnectionOutput
	RunKeyExchange();

	//if no receive thread, start one, otherwise restart the thread. The thread will always be in a suspended state
	if (m_RecvThreadHandle)
	{
		LogMessage("Reanimating receive thread\n");
		m_TcpThreadRunning = true;
#ifdef WIN32
				ResumeThread(m_RecvThreadHandle);
#else
#endif
	}
	else
	{
		LogMessage("Creating new receive thread\n");
		//need to create a receive thread for this connection
#ifdef WIN32
		DWORD uiThreadId;
		m_RecvThreadHandle = CreateThread (NULL, 2048, SocketRecvThread, this, 0, &uiThreadId);
#else
		pthread_create(&m_RecvThreadHandle, NULL, SocketRecvThread, this);
#endif
	}
}

Connection::~Connection()
{
    if (m_Socket != INVALID_SOCKET)
    {
        closesocket(m_Socket);
		m_Socket = INVALID_SOCKET;
    }

    // Allow the thread to die
	int limit = 100;
	while (m_TcpThreadRunning)
	{
		Sleep(20);
		if (--limit < 0)
		{
			break;
		}
	}

    // Give the send thread time to die
	Sleep(200);
}

bool Connection::IsActive()
{
	return m_ConnectionActive;
}

void Connection::Send(unsigned char *Buffer, int length)
{
    if (m_PacketLoggingEnabled)
    {
        LogMessage("Adding packet to Send Queue, length=%d\n", length);
        DumpBuffer(Buffer, length);
    }

	m_SendQueue->Add(Buffer, length, m_AvatarID);
    //ResumeThread(m_SendThreadHandle);
}

void Connection::SetRC4Key(unsigned char *rc4_key)
{
    /*
    LogMessage("SetRC4Key: %02x %02x %02x %02x %02x %02x %02x %02x\n",
        rc4_key[0],
        rc4_key[1],
        rc4_key[2],
        rc4_key[3],
        rc4_key[4],
        rc4_key[5],
        rc4_key[6],
        rc4_key[7] );
    */

    m_CryptOut.PrepareKey(rc4_key, RC4_KEY_SIZE);
	m_CryptIn.PrepareKey(rc4_key, RC4_KEY_SIZE);
}

bool Connection::DoKeyExchange()
{
    unsigned char buffer[128];
    unsigned char *p = buffer;
    int length = 0;

	//LogMessage("DoKeyExchange sending public RSA-155 key:\n");

    // Send the RSA Public Key to the client
    length += m_WestwoodRSA.GetModulus(&p);
    length += m_WestwoodRSA.GetPublicExponent(&p);
    send(m_Socket, (char *) buffer, length, 0);

	// Read only 4 bytes to obtain the key length
    length = recv(m_Socket, (char *) buffer, 4, 0);
	if (length <= 0)
	{
        // Lost connection, exit without displaying an error
        // This allows a server ping with no consequences
		return false;
	}

    if (length != 4)
	{
		LogMessage("ERROR: DoKeyExchange expecting 4 byte key length, length = %d\n", length);
        return false;
	}

    long key_length = (long) ntohl((*((unsigned long *) buffer)));
    if ((key_length < 64) || (key_length > 65))
    {
		LogMessage("ERROR: DoKeyExchange key_length = %d\n", key_length);
		return false;
	}

    // Get the encrypted RC4 Session Key response from the client
    length = recv(m_Socket, (char *) buffer, key_length, 0);
	if (length <= 0)
	{
        // Lost connection, exit without displaying an error
		return false;
	}

    // make sure we were able to read the entire key
    if (length != key_length)
	{
		LogMessage("ERROR: DoKeyExchange key_length = %d, recv_length = %d\n", key_length, length);
		return false;
	}

    //LogMessage("DoKeyExchange received encrypted RC4 session key from client\n");
    // disregard leading byte if it is zero
    p = buffer;
    if ((key_length == WWRSA_BLOCK_SIZE + 1) && (*p == 0))
    {
        key_length--;
        p++;
    }

    // validate the key length against the expected value
    if (key_length != WWRSA_BLOCK_SIZE)
	{
		LogMessage("ERROR: DoKeyExchange key_length = %d\n", key_length);
        return false;
	}

    // Decrypt the RC4 Session Key
    unsigned char rc4key[WWRSA_BLOCK_SIZE];
    if (!m_WestwoodRSA.Decrypt(p, WWRSA_BLOCK_SIZE, rc4key))
    {
		LogMessage("ERROR: DoKeyExchange m_WestwoodRSA.Decrypt failed\n");
        return false;
	}

    unsigned char rc4_key_buffer[RC4_KEY_SIZE];

    // Reverse the order of the decrypted RC4 Session Key
	rc4_key_buffer[0] = rc4key[0x3f];
	rc4_key_buffer[1] = rc4key[0x3e];
	rc4_key_buffer[2] = rc4key[0x3d];
	rc4_key_buffer[3] = rc4key[0x3c];
	rc4_key_buffer[4] = rc4key[0x3b];
	rc4_key_buffer[5] = rc4key[0x3a];
	rc4_key_buffer[6] = rc4key[0x39];
	rc4_key_buffer[7] = rc4key[0x38];

	SetRC4Key(rc4_key_buffer);

    return (true);
}

bool Connection::DoClientKeyExchange()
{
	// Generate RC4 key
	unsigned int i = 0;
	unsigned char rc4key[RC4_KEY_SIZE];
    unsigned char buffer[128];

    memset(rc4key, 0, sizeof(rc4key));
    SetRC4Key(rc4key);

	// Receive the pubic key packet
    Sleep(20);
    memset(buffer, 0, sizeof(buffer));
    int count = recv(m_Socket, (char *) buffer, 74, 0);
	if (count != 74)
	{
		LogMessage("ERROR: DoClientKeyExchange recv returned %d\n", count);
		return false;
	}

    // Ignore whatever public key packet we receive

	// Clear the buffer
	memset(buffer, 0, WWRSA_BLOCK_SIZE - RC4_KEY_SIZE + sizeof(long));

	// Put the length in front of the buffer
	unsigned char *key = buffer + sizeof(long);
	*((unsigned long *) buffer) = ntohl(WWRSA_BLOCK_SIZE);

	// Copy the RC4 key to the bottom of the buffer
	unsigned char *dest = &key[WWRSA_BLOCK_SIZE - 1];
	unsigned char *src = rc4key;
	for (i = 0; i < RC4_KEY_SIZE; i++)
	{
		*dest-- = *src++;
	}

	// Encrypt the RC4 key
	m_WestwoodRSA.Encrypt(key, WWRSA_BLOCK_SIZE, key);

	//LogMessage("Sending encrypted RC4 session key...\n");
	// Send the encrypted RC4 key to the server
    int length = WWRSA_BLOCK_SIZE + sizeof(long);
    send(m_Socket, (char *) buffer, length, 0);

	return (true);
}

#define SEND_BUFFER_SIZE 10240

void Connection::RunKeyExchange()
{
    if (m_ServerType == CONNECTION_TYPE_SECTOR_SERVER_TO_PROXY)
    {
        LogMessage("Unencrypted TCP link\n");
    }
    else if (m_ServerType < CONNECTION_TYPE_MASTER_SERVER_TO_SECTOR_SERVER)
	{
		if (!DoKeyExchange())
		{
			m_ConnectionActive = false;
			LogMessage("Client key exchange failed.  Connection thread exiting\n");
			return;
		}
	}
	else
	{
		if (!DoClientKeyExchange())
		{
			m_ConnectionActive = false;
			LogMessage("Server/Server key exchange failed.  Connection thread exiting\n");
			return;
		}
	}

#if 0
	EnbTcpHeader header;
    memset(&header, 0, sizeof(header));

    while (!g_ServerShutdown && m_TcpThreadRunning)
    {
		// Read the opcode and the message length
        Sleep(1);
		int length = recv(m_Socket, (char *) &header, 4, 0);

		if (length == 4)
		{
            if (m_PacketLoggingEnabled)
            {
			    LogMessage("Received 4 byte header (encrypted):\n");
                DumpBuffer((unsigned char *) &header, length);
            }

            if (m_ServerType != CONNECTION_TYPE_SECTOR_SERVER_TO_PROXY)
            {
                m_CryptIn.RC4((unsigned char *) &header, length);
            }

            if (m_PacketLoggingEnabled)
            {
			    LogMessage("Received 4 byte header (decrypted):\n");
                DumpBuffer((unsigned char *) &header, length);
            }

            unsigned short bytes = header.size - sizeof(EnbTcpHeader);
			short opcode = header.opcode;

			//LogMessage("Received packet: opcode = 0x%02x, length = %d\n", opcode, bytes);

			// Read the message
            // TODO: peek to determine if we have enough data...
			// TODO: wait if we don't have enough data
            Sleep(1);

			int received = recv(m_Socket, (char *) m_RecvBuffer, bytes, 0);

			while (received < bytes)
			{
                int rcv = recv(m_Socket, (char *) (m_RecvBuffer + received), bytes - received, 0);
                if (rcv > 0)
                {
                    received += rcv;
                }
                else
                {
                    break; //if there's a receive error this loop needs to terminate
                }
			}

			if (received == bytes)
			{
                if (m_ServerType != CONNECTION_TYPE_SECTOR_SERVER_TO_PROXY)
                {
                    m_CryptIn.RC4(m_RecvBuffer, bytes);
                }

                if (m_PacketLoggingEnabled)
                {
                    LogMessage("Received %d byte packet\n", received);
				    DumpBuffer(m_RecvBuffer,bytes);
                }

                switch (m_ServerType)
				{
				case CONNECTION_TYPE_CLIENT_TO_GLOBAL_SERVER :
					ProcessGlobalServerOpcode(opcode, bytes);
					break;

				case CONNECTION_TYPE_CLIENT_TO_MASTER_SERVER :
					ProcessMasterServerOpcode(opcode, bytes);
					break;

				case CONNECTION_TYPE_CLIENT_TO_SECTOR_SERVER :
					//ProcessSectorServerOpcode(opcode, bytes);
                    LogMessage("ERROR!!: Sector Server opcode received!\n");
					break;

				case CONNECTION_TYPE_MASTER_SERVER_TO_SECTOR_SERVER :
					ProcessMasterServerToSectorServerOpcode(opcode, bytes);
					break;

				case CONNECTION_TYPE_SECTOR_SERVER_TO_SECTOR_SERVER :
					ProcessSectorServerToSectorServerOpcode(opcode, bytes);
					break;

                case CONNECTION_TYPE_SECTOR_SERVER_TO_PROXY:
                    ProcessProxyClientOpcode(opcode, bytes);
                    break;

                default:
                    LogMessage("ERROR: Unknown type of connection.\n");
                    break;
				}
			}
			else
			{
				LogMessage("Error receiving TCP packet on port %d, got %d bytes, expecting %d -- aborting!\n", m_TcpPort, received, bytes);
				if (received > 0 && g_Debug && received < 10000)
				{
					DumpBuffer(m_RecvBuffer, received);
				}
				m_TcpThreadRunning = false;
			}
		}
		else if (length == 0)
		{
			LogMessage("TCP connection on port %d closed gracefully\n", m_TcpPort);
			m_TcpThreadRunning = false;
		}
		else if (length == -1)
		{
            DWORD error = WSAGetLastError();
            if (error == WSAECONNRESET)
            {
    			LogMessage("TCP connection on port %d was reset\n", m_TcpPort);
            }
            else
            {
    			LogMessage("Error receiving header on port %d, error=%d -- aborting\n", m_TcpPort, error);
            }
			m_TcpThreadRunning = false;
		}
		else
		{
			LogMessage("Error receiving 4 byte header on port %d, length = %d bytes -- aborting\n", m_TcpPort, length);
			m_TcpThreadRunning = false;
		}
    }

    if (m_ServerType == CONNECTION_TYPE_SECTOR_SERVER_TO_PROXY && m_AvatarID != 0)
    {
        Player *p = g_ServerMgr->m_PlayerMgr.GetPlayer(m_AvatarID);

        if (p)
        {
            LogMessage("Emergency shutdown for TCP comms thread for '%s'\n", p->Name());
            p->SetTCPLoginLink(0);
        }
    }

	LogMessage("Connection thread exiting (%08x)\n", m_AvatarID);

	m_AvatarID = 0;

    m_TcpThreadRunning = false;
	m_ConnectionActive = false;
    ResumeThread(m_SendThreadHandle); //allow send thread to die
#endif
}

void Connection::SendResponse(short opcode, unsigned char *data, size_t length)
{
	if (m_ServerType == CONNECTION_TYPE_SECTOR_SERVER_TO_PROXY && m_AvatarID == 0)
	{
		LogMessage("Invalid attempt to send via TCP: Last Owner = %s\n", m_LastOwner);
		return;
	}

	*((short *) &m_SendBuffer[0]) = (short) length + sizeof(long);
	*((short *) &m_SendBuffer[2]) = opcode;

	if ((length + 4) < MAX_BUFFER)
	{
		memcpy(m_SendBuffer + sizeof(long), data, length);
	}
	else
	{
		LogMessage("Message too long to send via Connection: opcode %04x length %d\n", opcode, length);
		return;
	}

	int bytes = length + sizeof(long);

    if (m_PacketLoggingEnabled)
    {
	    LogMessage("Sending %d bytes (unencrypted)\n", bytes);
	    DumpBuffer(m_SendBuffer, bytes);
    }

    if (m_ServerType != CONNECTION_TYPE_SECTOR_SERVER_TO_PROXY)
    {
        m_CryptOut.RC4(m_SendBuffer, bytes);
    }

    Send(m_SendBuffer, bytes);
}

void Connection::SendResponseTestFile(short opcode, char *filename)
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
            LogMessage("Loading test data '%s'\n", filename);
            fseek(f,0,SEEK_SET);
            unsigned char * buffer = new unsigned char[length];
            fread(buffer, 1, length, f);
			SendResponse(opcode, buffer, length);
            delete [] buffer;
        }
        else
        {
            LogMessage("SendTestPacket: Invalid file length %d : '%s'\n", length, filename);
        }
        fclose(f);
    }
	else
	{
		LogMessage("SendTestData: Unable to open %s\n", filename);
	}
}

#ifdef WIN32
DWORD WINAPI Connection::SocketRecvThread(void *Param)
{
	Connection* p_this = reinterpret_cast<Connection*>( Param );

	p_this->RunRecvThread();

	return 1;
}
#else
void* Connection::SocketRecvThread(void *Param)
{
	Connection* p_this = reinterpret_cast<Connection*>( Param );

	p_this->RunRecvThread();

	return NULL;
}
#endif

void Connection::RunRecvThread()
{
	EnbTcpHeader header;
	memset(&header, 0, sizeof(header));

	m_TcpThreadRunning = true;

	while (!g_ServerShutdown)
	{
		while (m_TcpThreadRunning)
		{
			int length = recv(m_Socket, (char *) &header, 4, 0);

			if (length == 4)
			{
				if (m_PacketLoggingEnabled)
				{
					LogMessage("Received 4 byte header (encrypted):\n");
					DumpBuffer((unsigned char *) &header, length);
				}

				if (m_ServerType != CONNECTION_TYPE_SECTOR_SERVER_TO_PROXY)
				{
					m_CryptIn.RC4((unsigned char *) &header, length);
				}

				if (m_PacketLoggingEnabled)
				{
					LogMessage("Received 4 byte header (decrypted):\n");
					DumpBuffer((unsigned char *) &header, length);
				}

				unsigned short bytes = header.size - sizeof(EnbTcpHeader);
				short opcode = header.opcode;

				//LogMessage("Received packet: opcode = 0x%02x, length = %d\n", opcode, bytes);

				// Read the message
				// TODO: peek to determine if we have enough data...
				// TODO: wait if we don't have enough data
				Sleep(1);

				int received = recv(m_Socket, (char *) m_RecvBuffer, bytes, 0);

				while (received < bytes)
				{
					int rcv = recv(m_Socket, (char *) (m_RecvBuffer + received), bytes - received, 0);
					if (rcv > 0)
					{
						received += rcv;
					}
					else
					{
						break; //if there's a receive error this loop needs to terminate
					}
				}

				if (received == bytes)
				{
					if (m_ServerType != CONNECTION_TYPE_SECTOR_SERVER_TO_PROXY)
					{
						m_CryptIn.RC4(m_RecvBuffer, bytes);
					}

					if (m_PacketLoggingEnabled)
					{
						LogMessage("Received %d byte packet\n", received);
						DumpBuffer(m_RecvBuffer,bytes);
					}

					switch (m_ServerType)
					{
					case CONNECTION_TYPE_CLIENT_TO_GLOBAL_SERVER :
						ProcessGlobalServerOpcode(opcode, bytes);
						break;

					case CONNECTION_TYPE_CLIENT_TO_MASTER_SERVER :
						ProcessMasterServerOpcode(opcode, bytes);
						break;

					case CONNECTION_TYPE_CLIENT_TO_SECTOR_SERVER :
						//ProcessSectorServerOpcode(opcode, bytes);
						LogMessage("ERROR!!: Sector Server opcode received!\n");
						break;

					case CONNECTION_TYPE_MASTER_SERVER_TO_SECTOR_SERVER :
						ProcessMasterServerToSectorServerOpcode(opcode, bytes);
						break;

					case CONNECTION_TYPE_SECTOR_SERVER_TO_SECTOR_SERVER :
						ProcessSectorServerToSectorServerOpcode(opcode, bytes);
						break;

					case CONNECTION_TYPE_SECTOR_SERVER_TO_PROXY:
						ProcessProxyClientOpcode(opcode, bytes);
						break;

					default:
						LogMessage("ERROR: Unknown type of connection.\n");
						break;
					}
				}
				else
				{
					LogMessage("Error receiving TCP packet on port %d, got %d bytes, expecting %d -- aborting!\n", m_TcpPort, received, bytes);
					if (received > 0 && g_Debug && received < 10000)
					{
						DumpBuffer(m_RecvBuffer, received);
					}
					m_TcpThreadRunning = false;
				}
			}
			else if (length == 0)
			{
				LogMessage("TCP connection on port %d closed gracefully\n", m_TcpPort);
				m_TcpThreadRunning = false;
			}
			else if (length == -1)
			{
				DWORD error = WSAGetLastError();
				if (error == WSAECONNRESET)
				{
					LogMessage("TCP connection on port %d was reset\n", m_TcpPort);
				}
				else
				{
					LogMessage("Error receiving header on port %d, error=%d -- aborting\n", m_TcpPort, error);
				}
				m_TcpThreadRunning = false;
			}
			else
			{
				LogMessage("Error receiving 4 byte header on port %d, length = %d bytes -- aborting\n", m_TcpPort, length);
				m_TcpThreadRunning = false;
			}
		}

		if (m_ServerType == CONNECTION_TYPE_SECTOR_SERVER_TO_PROXY && m_AvatarID != 0)
		{
			Player *p = g_ServerMgr->m_PlayerMgr.GetPlayer(m_AvatarID);

			if (p)
			{
				LogMessage("Emergency shutdown for TCP comms thread for '%s'\n", p->Name());
				p->SetTCPLoginLink(0);
			}
		}

		LogMessage("Connection thread is frozen, but alive and in perfect hibernation (%08x)\n", m_AvatarID);

		m_AvatarID = 0;

		m_ConnectionActive = false;

#ifdef WIN32
		SuspendThread(m_RecvThreadHandle); //suspend ourselves
#endif

		// go into loop until the thread is revived
		while (!m_TcpThreadRunning)
		{
			Sleep(100);
		}
	}
}

void Connection::PulseConnectionOutput(bool player_thread)
{
	unsigned char msg[SEND_BUFFER_SIZE];
    int length;

	if (!player_thread && m_LoginHandoff) //don't send anything if we're in the player thread
	{
		return;
	}

	m_Mutex.Lock();
	while (m_SendQueue->CheckQueue(msg, &length, SEND_BUFFER_SIZE))
	{
		send(m_Socket, (char *) msg, length, 0);
	}
	m_Mutex.Unlock();
}
