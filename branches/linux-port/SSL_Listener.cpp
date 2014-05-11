// SSL_Listener.cpp
//
// Instantiating this class will create a new thread
// to listen on the specified port.
//

#include "Net7.h"
#include "SSL_Listener.h"
#include "SSL_Connection.h"
#include "ServerManager.h"

// This helper function is referenced by _beginthread to launch the TCP Listener thread.
#ifdef WIN32
void __cdecl RunSslListenerThread(void *arg)
{
    ((SSL_Listener *) arg)->RunThread();
	_endthread();
}
#else // Linux
void * RunSslListenerThread(void *arg)
{
    ((SSL_Listener *) arg)->RunThread();
    return NULL;
}
#endif

// Constructor
SSL_Listener::SSL_Listener(unsigned long ip_address, unsigned short port, ServerManager &server_mgr)
	: m_IpAddress(ip_address),
      m_TcpPort(port),
	  m_ServerMgr(server_mgr)
{
	// Thread is not running (yet)
	m_SslListenerThreadRunning = false;

	// Socket not opened (yet)
    m_ListenerSocket = INVALID_SOCKET;

    // Launch the Listener thread
#ifdef WIN32
    _beginthread(&RunSslListenerThread, 0, this);
#else
    pthread_create(&m_Thread, NULL, &RunSslListenerThread, (void *) this);
#endif
}

// Destructor
SSL_Listener::~SSL_Listener()
{
	// Shut down the listener thread
	m_SslListenerThreadRunning = false;

    m_Mutex.Lock();

    // close the Listener socket
    if (m_ListenerSocket != INVALID_SOCKET)
    {
        closesocket(m_ListenerSocket);
        m_ListenerSocket = INVALID_SOCKET;
    }

    m_Mutex.Unlock();

	// Allow the listener thread to die
	Sleep(1);
}

// This is the entry point for the listener thread
void SSL_Listener::RunThread()
{
	// Create a socket
	m_ListenerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_ListenerSocket == INVALID_SOCKET)
    {
        LogMessage("Unable to create SSL Listener socket\n");
        return;
    }

	struct sockaddr_in name;
    memset(&name, 0, sizeof(name));
	name.sin_family = AF_INET;
	name.sin_addr.s_addr = m_IpAddress;
	name.sin_port = htons(m_TcpPort);

	struct in_addr in;
#ifdef WIN32
	in.S_un.S_addr = m_IpAddress;
#else
	in.s_addr = m_IpAddress;
#endif

	if (bind(m_ListenerSocket, (struct sockaddr *) &name, sizeof(name)))
    {
        LogMessage("Listener unable to bind to socket on port %s:%d\n", inet_ntoa(in), m_TcpPort);
        closesocket(m_ListenerSocket);
        return;
    }

	if (listen(m_ListenerSocket, SOMAXCONN))
    {
        LogMessage("Listen failed on port %d\n", m_TcpPort);
        closesocket(m_ListenerSocket);
        return;
    }

    // LogMessage("Listening for incoming SSL connections on port %d\n", m_TcpPort);
    m_SslListenerThreadRunning = true;

    struct sockaddr_in from;
#ifdef WIN32
    int from_length = sizeof(from);
#else
    socklen_t from_length = sizeof(from);
#endif
    SOCKET s;

    while ((m_SslListenerThreadRunning) &&
           (m_ListenerSocket != INVALID_SOCKET))
    {
        s = accept(m_ListenerSocket,(sockaddr *) &from, &from_length);
        if (s != INVALID_SOCKET)
	    {
			//LogMessage("Accepted SSL connection on port %d\n", m_TcpPort);
            unsigned char *ip = (unsigned char *) &from.sin_addr;
			LogMessage("Accepted SSL connection from %d.%d.%d.%d\n",
                ip[0], ip[1], ip[2], ip[3]);

            m_Mutex.Lock();

			// Create a new SSL_Connection using the new socket
#ifdef WIN32
			SSL_Connection * ssl_connection = new SSL_Connection(s, m_ServerMgr, from.sin_addr.S_un.S_addr);
#else
			SSL_Connection * ssl_connection = new SSL_Connection(s, m_ServerMgr, from.sin_addr.s_addr);
#endif

			// Add the SSL connection to the connection list
			m_ServerMgr.m_ConnectionMgr.AddSslConnection(ssl_connection);

            m_Mutex.Unlock();
	    }
		else
		{
			// add error handling
		}

		Sleep(10);
    }

    m_SslListenerThreadRunning = false;

    LogMessage("SSL Listener Thread exiting\n");
}

void SSL_Listener::Shutdown()
{
	closesocket(m_ListenerSocket);
	m_ListenerSocket = INVALID_SOCKET;
}

