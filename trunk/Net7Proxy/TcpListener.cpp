// TcpListener.cpp
//
// Instantiating this class will create a new thread
// to listen on the specified port.
//

#include "Net7.h"
#include "TcpListener.h"
#include "Connection.h"
#include "ServerManager.h"

// This helper function is referenced by _beginthread to launch the TCP Listener thread.
#ifdef WIN32
void __cdecl RunTcpListenerThread(void *arg)
{
    ((TcpListener *) arg)->RunThread();
	_endthread();
}
#else // Linux
void * RunTcpListenerThread(void *arg)
{
    ((TcpListener *) arg)->RunThread();
    return NULL;
}
#endif

// Constructor
TcpListener::TcpListener(unsigned long ip_address, unsigned short port, ServerManager &server_mgr, int server_type)
	: m_IpAddress(ip_address),
      m_TcpPort(port),
	  m_ServerMgr(server_mgr),
	  m_ServerType(server_type)
{
	// Thread is not running (yet)
	m_TcpListenerThreadRunning = false;

	// Socket not opened (yet)
    m_TcpListenerSocket = INVALID_SOCKET;

    // Launch the Listener thread
#ifdef WIN32
    _beginthread(&RunTcpListenerThread, 0, this);
#else
    pthread_create(&m_Thread, NULL, &RunTcpListenerThread, (void *) this);
#endif
}

// Destructor
TcpListener::~TcpListener()
{
	// Shut down the listener thread
	m_TcpListenerThreadRunning = false;

    // close the Listener socket
    if (m_TcpListenerSocket != INVALID_SOCKET)
    {
        closesocket(m_TcpListenerSocket);
        m_TcpListenerSocket = INVALID_SOCKET;
    }

	// Allow the listener thread to die
	Sleep(1);
}

// This is the entry point for the TCP listener thread
void TcpListener::RunThread()
{
	// Create a socket
	m_TcpListenerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_TcpListenerSocket == INVALID_SOCKET)
    {
        LogMessage("Unable to create TCP listener socket\n");
        return;
    }

	struct sockaddr_in name;
    memset(&name, 0, sizeof(name));
	name.sin_family = AF_INET;
	//name.sin_addr.s_addr = m_IpAddress;
	name.sin_addr.s_addr = INADDR_ANY;
	name.sin_port = htons(m_TcpPort);

	if (bind(m_TcpListenerSocket, (struct sockaddr *) &name, sizeof(name)))
    {
        LogMessage("TCP Listener unable to bind to socket on port %d\n", m_TcpPort);
        closesocket(m_TcpListenerSocket);
        return;
    }

	if (listen(m_TcpListenerSocket, SOMAXCONN))
    {
        LogMessage("Listen failed on port %d\n", m_TcpPort);
        closesocket(m_TcpListenerSocket);
        return;
    }

    //LogMessage("Listening for incoming TCP/IP connections on port %d\n", m_TcpPort);

    m_TcpListenerThreadRunning = true;

    struct sockaddr_in from;
    memset(&from, 0, sizeof(from));
#ifdef WIN32
    int from_length;
#else
    socklen_t from_length;
#endif
    SOCKET s;

    while ((!g_ServerShutdown) &&
		   (m_TcpListenerThreadRunning) &&
           (m_TcpListenerSocket != INVALID_SOCKET))
    {
	    from_length = sizeof(from);
        s = accept(m_TcpListenerSocket,(sockaddr *) &from, &from_length);

        if (s != INVALID_SOCKET)
	    {
            BOOL bOptVal = TRUE;
            if (setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, (char*)&bOptVal, sizeof(BOOL)) != 0)
            {
                printf("Set SO_KEEPALIVE failed\n");
            }
            unsigned char *ip = (unsigned char *) &from.sin_addr;
			LogDebug("Accepted TCP connection from %d.%d.%d.%d on port %d\n",
                ip[0], ip[1], ip[2], ip[3], m_TcpPort);

			// Create a new Connection using the new socket
			Connection * client = new Connection(s, m_ServerMgr, m_TcpPort, m_ServerType, (unsigned long*)&from.sin_addr);

            if (m_ServerType == CONNECTION_TYPE_CLIENT_TO_SECTOR_SERVER)
            {
                g_ServerMgr->m_SectorConnection = client;
                g_ServerMgr->m_ConnectionCount++;
            }
            else if (m_ServerType == CONNECTION_TYPE_CLIENT_TO_GLOBAL_SERVER)
            {
                g_ServerMgr->m_GlobalConnection = client;
            }

			// Add the TCP connection to the connection list
			m_ServerMgr.m_ConnectionMgr.AddConnection(client);
	    }
		else
		{
			// add error handling
		}

		Sleep(10);
    }

    m_TcpListenerThreadRunning = false;

    fprintf(stderr,"TCP Listener Thread exiting\n");
}

void TcpListener::Shutdown()
{
	closesocket(m_TcpListenerSocket);
	m_TcpListenerSocket = INVALID_SOCKET;
}

