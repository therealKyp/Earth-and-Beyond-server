// TcpConnection.cpp

#include "Net7.h"
#include "TcpConnection.h"

// This helper function is referenced by _beginthread to launch the TCP thread.
#ifdef WIN32

void __cdecl RunTcpThread(void *arg)
{
    ((TcpConnection *) arg)->RunThread();
    _endthread();
}

#else // Linux

void * RunTcpThread(void *arg)
{
    ((TcpConnection *) arg)->RunThread();
    return NULL;
}

#endif

TcpConnection::TcpConnection(SOCKET sock, unsigned short port)
: m_Socket(sock), m_Port(port)
{
    // Mark this as an active connection
    m_Active = true;

    // Thread is not running (yet)
    m_Running = false;

    // Disable packet logging
    m_Logging = false;
    
    // Owning Server
    //m_OwningServer = NULL;

    // Launch the Connection thread
#ifdef WIN32
    _beginthread(&RunTcpThread, 0, this);
#else
    pthread_create(&m_Thread, NULL, &RunTcpThread, (void *)this);
#endif
}

TcpConnection::~TcpConnection()
{
    Shutdown();

    // Allow the thread to die
    int limit = 100;
    while (m_Running)
    {
        Sleep(20);
        if (--limit < 0)
            break;
    }

    //if (m_Buffer)
    //    delete [] m_Buffer;
}

void TcpConnection::Shutdown()
{
    m_Mutex.Lock();
    
    m_Active = false;
    m_Running = false;

    if (m_Socket != INVALID_SOCKET)
    {
        closesocket(m_Socket);
        m_Socket = INVALID_SOCKET;
    }

    m_Mutex.Unlock();
}

bool TcpConnection::IsActive()
{
    return m_Active;
}

void TcpConnection::RunThread()
{
    if(!Initialize())
    {
        m_Active = false;
        return;
    }

    Loop();
}

bool TcpConnection::Initialize()
{
    if (!KeyExchange())
    {
        g_LogError.Print("Key exchange failed. Connection thread exiting.");
        return false;
    }
    return true;
}

void TcpConnection::Loop()
{
    EnbTcpHeader header;
    memset(&header, 0, sizeof(header));
    
    m_Running = true;
    
    while(true)
    {
        Sleep(1);
        
        // Will this be too resource intensive? Doesn't look like it will be...
        m_Mutex.Lock();
        
        if(!m_Running)
            break;      
        
        m_Mutex.Unlock();
        
		int length = recv(m_Socket, (char *) &header, 4, 0);
		if (length == 4)
		{
			m_CryptIn.RC4((unsigned char *) &header, length);

			unsigned short bytes = header.size - sizeof(EnbTcpHeader);
			short opcode = header.opcode;

            Sleep(1);

            // TODO: peek to determine if we have enough data...
			// TODO: wait if we don't have enough data
			length = recv(m_Socket, (char *)m_Buffer, bytes, 0);
			if (length == bytes)
			{
				m_CryptIn.RC4(m_Buffer, bytes);
				
				ProcessOpcode(opcode, bytes);
			}
			else
			{
				g_LogError.Print("Error receiving TCP packet on port %d, got %d bytes, expecting %d -- aborting!", m_Port, length, bytes);
				
				if (length > 0)
					DumpBuffer(m_Buffer, length);
				
                Shutdown();
			}
		}
		else if (length == 0)
		{
			g_Log.Print("TCP connection on port %d closed gracefully.", m_Port);
			Shutdown();
		}
		else if (length < 0)
		{
            u32_t error = WSAGetLastError();
            
            if (error == WSAECONNRESET)
    			g_LogError.Print("TCP connection port %d was reset", m_Port);
            else
    			g_LogError.Print("Error receiving header on port %d, error=%d -- aborting", m_Port, error);
            
			Shutdown();
		}
		else
		{
			g_LogError.Print("Error receiving 4 byte header on port %d, length = %d bytes -- aborting", m_Port, length);
			Shutdown();
		}
    }
    
    Shutdown();
}

void TcpConnection::SetRC4Key(unsigned char *rc4_key)
{
	m_CryptOut.PrepareKey(rc4_key, RC4_KEY_SIZE);
	m_CryptIn.PrepareKey(rc4_key, RC4_KEY_SIZE);
}

void TcpConnection::DumpBuffer(unsigned char *buffer, int length)
{
    char line[150];
    //g_LogDebug->Print("Dumping %d bytes:\n", length);
    memset(line, 0, sizeof(line));
    for (int i = 0; i < length; i++)
    {
        sprintf(line, "%s %02X", line, buffer[i]);
        if ((i % 16) == 15)
        {
            g_LogDebug.Print("%s", line);
            memset(line, 0, sizeof(line));
        }
    }
    
    if (length > 0)
        g_LogDebug.Print("%s", line);
}

void TcpConnection::Send(unsigned char *buffer, int length)
{
	//LogMessage("Sending %d bytes...\n", length);
	//DumpBuffer(m_TcpBuffer,length);
	
    Sleep(1);
    
    if(send(m_Socket, (char *)buffer, length, 0))
    {
        int error = errno;
        if(error == ECONNRESET)
        {
            g_LogDebug.Print("Connection reset on port %u.", m_Port);
            Shutdown();
        }
        else
        {
            g_LogDebug.Print("Error sending %d bytes of data. %s.", length, strerror(error));
        }
    }
}

void TcpConnection::SendResponse(short opcode, unsigned char *data, int length)
{
	unsigned char buffer[MAX_BUFFER];

	EnbTcpHeader * header = (EnbTcpHeader *) buffer;
	header->size = (short) length + sizeof(EnbTcpHeader);
	header->opcode = opcode;
	
	if (length)
		memcpy(buffer + sizeof(EnbTcpHeader), data, length);

	int bytes = length + sizeof(EnbTcpHeader);

    if (m_Logging)
    {
        g_LogDebug.Print("Sending %d byte message:", bytes);
	    DumpBuffer(buffer, bytes);
    }

	m_CryptOut.RC4(buffer, bytes);
	Send(buffer,bytes);
}
