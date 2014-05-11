// Connection.cpp
		
#include "Net7.h"
#include "Connection.h"
#include "ServerManager.h"
#include "PacketStructures.h"
#include "Opcodes.h"
#include "UDPClient.h"
//#include "PlayerClass.h"

extern bool g_ShuttingDown;

// This helper function is referenced by _beginthread to launch the TCP thread.
#ifdef WIN32
void __cdecl LaunchSendThread(void *arg)
{
    ((Connection *) arg)->RunSendThread();
    _endthread();
}
void __cdecl LaunchRecvThread(void *arg)
{
    ((Connection *) arg)->RunRecvThread();
    _endthread();
}
#else // Linux
void * LaunchSendThread(void *arg)
{
    ((Connection *) arg)->RunSendThread();
    return NULL;
}
void * LaunchRecvThread(void *arg)
{
    ((Connection *) arg)->RunRecvThread();
    return NULL;
}
#endif

Connection::Connection(SOCKET s, ServerManager &server_mgr, short port, int server_type, unsigned long* ip_addr)
	: m_Socket(s),
	  m_ServerMgr(server_mgr),
	  m_TcpPort(port),
	  m_ServerType(server_type)
{
    // Mark this as an active connection
	m_ConnectionActive = true;

    // Thread is not running (yet)
    m_TcpThreadRunning = false;

	m_TcpShutdownCycle = 0;

    // Disable packet logging
    m_PacketLoggingEnabled = false;

    m_AvatarID = -1;

	if (ip_addr)
	{
		m_IPaddr = *ip_addr;
	}

	m_Tilt_Sent = 0;
	m_Turn_Sent = 0;

	// Launch the Receiver thread
#ifdef WIN32
	_beginthread(&LaunchRecvThread, 0, this);
#else
    pthread_create(&m_Thread, NULL, &LaunchRecvThread, (void *) this);
#endif
}

Connection::~Connection()
{
    if (m_Socket != INVALID_SOCKET)
    {
        closesocket(m_Socket);
		m_Socket = INVALID_SOCKET;
    }
	
	m_TcpThreadRunning = false;

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
    m_SendQueue.Add(Buffer, length);
    ResumeThread(m_SendThreadHandle);
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

    LogMessage("Client Key exchange complete\n");

	return (true);
}

void Connection::RunSendThread()
{
	unsigned char *msg = NULL;
    long suspend_counter = 40;
    int length;
    while (!g_ServerShutdown && m_TcpThreadRunning)
    {
        while (m_SendQueue.CheckQueue(&msg, &length))
        {
            suspend_counter = 40;
            if (length > 0)
            {
                if (m_ServerType != CONNECTION_TYPE_SECTOR_SERVER_TO_PROXY)
                {
                    m_CryptOut.RC4(msg, length);
                }
				/*else
				{
					LogMessage("Sent opcode 0x%04x\n", *((short *) &msg[2]));
				}*/

                send(m_Socket, (char *) msg, length, 0);
                
                delete [] msg;
            }
        }
        suspend_counter--;
        if (suspend_counter < 1)
        {
            //LogMessage("Suspend thread\n");
            SuspendThread(m_SendThreadHandle); //suspend until we're stimulated again
        }
        Sleep(5);
    }

    _endthread();
}

/*void Connection::RunSendThread()
{
	unsigned char *msg = NULL;
    int length;
    int cumulative_length;
    bool sent = false;
    while (!g_ServerShutdown && m_TcpThreadRunning)
    {
        cumulative_length = 0;
        
        //see if we can build a big packet to send
        while (cumulative_length < 400 && !sent && m_SendQueue.CheckQueue(&msg, &length))
        {
            sent = false;
            if (cumulative_length + length >= 400)
            {
                //send cumulative queue
                if (cumulative_length > 0)
                {
                    m_Mutex.Lock();
                    send(m_Socket, (char *) m_MessageBuffer, cumulative_length, 0);
                    m_Mutex.Unlock();
                }

                //send final message
                m_Mutex.Lock();
                send(m_Socket, (char *) msg, length, 0);
                m_Mutex.Unlock();
                sent = true;
                cumulative_length = 0;
            }
            else
            {
                //add to buffer
                memcpy((m_MessageBuffer + cumulative_length), msg, length);
                cumulative_length += length;
            }
            delete [] msg;
        }

        if (!sent && cumulative_length > 0)
        {
            //send buffer, wasn't enough to fill 400
            m_Mutex.Lock();
            send(m_Socket, (char *) m_MessageBuffer, cumulative_length, 0);
            m_Mutex.Unlock();
        }
        SuspendThread(m_SendThreadHandle); //suspend until we're stimulated again
        Sleep(1);
    }
}*/

void Connection::RunRecvThread()
{
	if (m_ServerType == CONNECTION_TYPE_SECTOR_SERVER_TO_PROXY)
    {
        LogMessage("TCP login link established.\n");
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

    m_TcpThreadRunning = true;

    // Launch the Send Thread
#ifdef WIN32
  	UINT uiThreadId = 0;
    m_SendThreadHandle = (HANDLE)_beginthreadex(NULL, 0, SocketSendThread, this, CREATE_SUSPENDED, &uiThreadId);
	//_beginthread(&LaunchSendThread, 0, this);
#else
    pthread_create(&m_Thread, NULL, &LaunchSendThread, (void *) this);
#endif

	EnbTcpHeader header;
    memset(&header, 0, sizeof(header));
	char *ptr_hdr = (char*)&header;

    while (!g_ServerShutdown && m_TcpThreadRunning && !g_ShuttingDown) 
    {
        // Read the opcode and the message length
        Sleep(1);
        int length = 0;
		int repeats = 0;
		
		length = recv(m_Socket, ptr_hdr, 4, 0);

		while(length < 4 && length > 0)
		{
			length += recv(m_Socket, (ptr_hdr + length), 4-length, 0);

			if(length == -1 || repeats++ > 3)
			{
				//break out and let the program handle it normally.
				break;
			}
			else if(length != 4)
			{
				LogMessage("Format error, length = %d\n", length);
			}
			LogMessage("%s Receive header: %d\n", (m_ServerType == CONNECTION_TYPE_SECTOR_SERVER_TO_PROXY ? "server->proxy" : "client->proxy"), length);
			repeats++;
		}
               
        if (length == 4)
        {
            if (m_PacketLoggingEnabled)
            {
			    LogMessage("Received 4 byte header (encrypted):\n");
                DumpBuffer((unsigned char *) &header, length);
            }

            if (m_ServerType != CONNECTION_TYPE_SECTOR_SERVER_TO_PROXY && m_ServerType != CONNECTION_TYPE_GLOBAL_PROXY_TO_SERVER)
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
                    break;
                }
			}

			if (received == bytes)
			{
                if (m_ServerType != CONNECTION_TYPE_SECTOR_SERVER_TO_PROXY && m_ServerType != CONNECTION_TYPE_GLOBAL_PROXY_TO_SERVER)
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
					//LogMessage("ERROR!!: Global Server opcode received!\n");
					//LogMessage("Received Client to Global Opcode: 0x%x\n", opcode);
					ProcessGlobalServerOpcode(opcode, bytes);
					break;

				case CONNECTION_TYPE_CLIENT_TO_MASTER_SERVER :
					//LogMessage("Received Client to Master Opcode: 0x%x\n", opcode);
					ProcessMasterServerOpcode(opcode, bytes);
					break;

				case CONNECTION_TYPE_CLIENT_TO_SECTOR_SERVER :
					ProcessSectorServerOpcode(opcode, bytes);
                    //LogMessage("Received SS Opcode: 0x%x\n", opcode);
					break;

				case CONNECTION_TYPE_SECTOR_SERVER_TO_PROXY :
                    //send these opcodes straight to the client
					ProcessProxyServerOpcode(opcode, bytes);
					break;

				case CONNECTION_TYPE_SECTOR_SERVER_TO_SECTOR_SERVER :
					//ProcessSectorServerToSectorServerOpcode(opcode, bytes);
					break;

				case CONNECTION_TYPE_GLOBAL_PROXY_TO_SERVER:
					ProcessProxyGlobalOpcode(opcode, bytes);
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

	LogDebug("Connection thread exiting\n");

    switch (m_ServerType)
    {
    case CONNECTION_TYPE_CLIENT_TO_SECTOR_SERVER:
        g_ServerMgr->m_ConnectionCount--;
        if (g_ServerMgr->m_ConnectionCount == 0)
        {
            LogMessage("Client connections terminated\n");
            g_ServerMgr->m_SectorConnection = (0);
        }
        break;

    case CONNECTION_TYPE_CLIENT_TO_MASTER_SERVER:
        g_ServerMgr->m_MasterConnection = (0);
        break;

    case CONNECTION_TYPE_SECTOR_SERVER_TO_PROXY:
	case CONNECTION_TYPE_GLOBAL_PROXY_TO_SERVER:
        g_ServerMgr->m_UDPConnection->BlankTCPConnection();
        break;

    default:
        break;

    }

    m_TcpThreadRunning = false;
	m_ConnectionActive = false;
    ResumeThread(m_SendThreadHandle);
}

void Connection::SendResponse(short opcode, unsigned char *data, size_t length, long sequence_num)
{
    //attempt to isolate why this crashes from time to time.
	*((short *) &m_SendBuffer[0]) = (short) length + sizeof(long);
	*((short *) &m_SendBuffer[2]) = opcode;

	if (length)
	{
		memcpy(m_SendBuffer + sizeof(long), data, length);
	}

	int bytes = length + sizeof(long);

    if (m_PacketLoggingEnabled)
    {
	    LogMessage("Sending %d bytes (unencrypted)\n", bytes);
	    DumpBuffer(m_SendBuffer, bytes);
    }

    //LogMessage("Sending opcode #%x: 0x%04x [%x]\n", sequence_num, opcode, length);

    /*if (m_ServerType != CONNECTION_TYPE_SECTOR_SERVER_TO_PROXY)
    {
        m_CryptOut.RC4(m_SendBuffer, bytes);
    }*/

    //send(m_Socket, (char *) m_SendBuffer, bytes, 0);

	Send(m_SendBuffer, bytes);
}

void Connection::SendResponseDirect(short opcode, unsigned char *data, size_t length)
{
	*((short *) &m_SendBuffer[0]) = (short) length + sizeof(long);
	*((short *) &m_SendBuffer[2]) = opcode;

	if (length)
	{
		memcpy(m_SendBuffer + sizeof(long), data, length);
	}

	int bytes = length + sizeof(long);

    send(m_Socket, (char *) m_SendBuffer, bytes, 0);
}

void Connection::QueueResponse(unsigned char *packet, short &index, short opcode, unsigned char *data, size_t length)
{
    u8 *ptr = packet + index;

    *((short *) &ptr[0]) = (short) length + sizeof(long);
	*((short *) &ptr[2]) = opcode;

	if (length)
	{
		memcpy(ptr + sizeof(long), data, length);
	}

	int bytes = length + sizeof(long);

    /*if (m_ServerType != CONNECTION_TYPE_SECTOR_SERVER_TO_PROXY)
    {
        m_CryptOut.RC4(ptr, bytes);
    }*/

    index += bytes;
}

void Connection::SendQueuedPacket(unsigned char *data, short length)
{
    Send(data, length);
}

void Connection::QueuePacket(u8 *packet, short &index, unsigned char *data, size_t length)
{
	if (length > 0)
	{
		memcpy(packet + index, data, length);
        m_CryptOut.RC4(packet + index, length);
        index += length;
	}
    else
    {
        LogMessage("Packet has no length\n");
    }
}

void Connection::SendResponseTestFile(short opcode, char *filename)
{
	int length = 0;
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

void Connection::SendSectorAssignment(long sector_id)
{
	SendResponse(ENB_OPCODE_7801_SECTOR_ASSIGNMENT, (unsigned char *) &sector_id, sizeof(sector_id));
}

void Connection::TerminateConnection()
{
    m_TcpThreadRunning = false;
	m_ConnectionActive = false;
}

UINT WINAPI Connection::SocketSendThread(void *Param)
{
	Connection* p_this = reinterpret_cast<Connection*>( Param );

	p_this->RunSendThread();

    _endthread();

	return 1;
}

bool Connection::CheckTCPShutdownCycle()
{
	if (m_TcpShutdownCycle != 0)
	{
		unsigned long tick = GetNet7TickCount();
		
		if (m_TcpShutdownCycle < tick)
		{
			LogMessage("Shutdown the thread!\n");
			return true;
		}
	}
	
	return false;
}
