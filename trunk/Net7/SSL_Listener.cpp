// SSL_Listener.cpp
//
// Instantiating this class will create a new thread
// to listen on the specified port.
//
// Changed by Myatu (Michael A. Green) on 17 Feb 2009, based on Revision 1808 - myatus@gmail.com
// + Added SSL Context handler, which was moved from SSL_Connection
// = Simplified listener a little, mainly for Linux compatibility
// = General cleanup
// + Added SocketReady() function, which replaced the Sleep() for the thread. Quicker response time and lower CPU usage.
// = Fixed Bug: Didn't look for g_ServerShutdown

#include "Net7.h"
#include "SSL_Listener.h"
#include "SSL_Connection.h"
#include "ServerManager.h"
#include "UDPConnection.h"


// This helper function is referenced by _beginthread to launch the TCP Listener thread.
#ifdef WIN32
void __cdecl RunSslListenerThread(void *arg)
#else
void * RunSslListenerThread(void *arg)
#endif
{
	((SSL_Listener *) arg)->RunThread();
#ifdef WIN32
	_endthread();
#else
	return NULL;
#endif	
}

// Constructor
SSL_Listener::SSL_Listener(unsigned long ip_address, unsigned short port, ServerManager &server_mgr) 
	: m_IpAddress(ip_address), m_TcpPort(port), m_ServerMgr(server_mgr)
{
	char certf[40];
	char keyf[40];
	SSL_METHOD * ssl_server_method;

	m_SslListenerThreadRunning = false;
	m_ListenerSocket = INVALID_SOCKET;

	// Initialize SSL Context
	SSL_load_error_strings();
	SSLeay_add_ssl_algorithms();
	ssl_server_method = SSLv23_server_method();

	sprintf(certf,	"%s.cer", g_DomainName);
	sprintf(keyf,	"%s.pem", g_DomainName);

	m_ssl_context = SSL_CTX_new(ssl_server_method);
	if ( !m_ssl_context )
	{
		LogMessage("SSL Init: SSL_CTX_new failed\n");
		return;
	}

	if ( SSL_CTX_use_certificate_file(m_ssl_context, certf, SSL_FILETYPE_PEM) <= 0 ) 
	{
		LogMessage("SSL Init: SSL_CTX_use_certificate_file failed\n");
		return;
	}

	if ( SSL_CTX_use_PrivateKey_file(m_ssl_context, keyf, SSL_FILETYPE_PEM) <= 0 )
	{
		LogMessage("SSL Init: SSL_CTX_use_PrivateKey_file failed\n");
		return;
	}

	if ( !SSL_CTX_check_private_key(m_ssl_context) )
	{
		LogMessage("SSL Init: Private key does not match the certificate public key\n");
		return;
	}

	SSL_CTX_set_mode(m_ssl_context, SSL_MODE_AUTO_RETRY); 	// Avoids the SSL_ERROR_WANT_READ / WRITE

#ifdef WIN32
	_beginthread(&RunSslListenerThread, 0, this);
#else
	pthread_create(&m_Thread, NULL, &RunSslListenerThread, (void *) this);
#endif
}

// Destructor
SSL_Listener::~SSL_Listener()
{
	Shutdown();

	if (m_ssl_context) SSL_CTX_free(m_ssl_context);

	// Allow the listener thread to die
	Sleep(1);
}

// This is the entry point for the listener thread
void SSL_Listener::RunThread()
{
	struct sockaddr_in name;
	struct in_addr in;
	struct sockaddr_in from;
#ifdef WIN32
	int from_length;
#else
	socklen_t from_length;
#endif
	SOCKET s;
	unsigned char *ip;
	long addr;

	// Create a socket
	m_ListenerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_ListenerSocket == INVALID_SOCKET)
	{
		LogMessage("Unable to create SSL Listener socket\n");
		return;
	}

	memset(&name, 0, sizeof(name));
	name.sin_family = AF_INET;
	name.sin_addr.s_addr = m_IpAddress;
	name.sin_port = htons(m_TcpPort);

#ifdef WIN32
	in.S_un.S_addr = m_IpAddress;
#else
	in.s_addr = m_IpAddress;
#endif

	if (bind(m_ListenerSocket, (struct sockaddr *) &name, sizeof(name)))
	{
		LogMessage("Listener unable to bind to socket on %s:%d\n", inet_ntoa(in), m_TcpPort);
		Shutdown();
		return;
	}

	if (listen(m_ListenerSocket, SOMAXCONN))
	{
		LogMessage("Listen failed on port %d\n", m_TcpPort);
		Shutdown();
		return;
	}

	// LogMessage("Listening for incoming SSL connections on port %d\n", m_TcpPort);
	m_SslListenerThreadRunning = true;

	while ( (!g_ServerShutdown) && (m_SslListenerThreadRunning) && (m_ListenerSocket != INVALID_SOCKET) )
	{
		from_length = sizeof(from);		
		s = accept(m_ListenerSocket,(sockaddr *) &from, &from_length);

		if (s != INVALID_SOCKET)
		{
			ip = (unsigned char *) &from.sin_addr;
			addr = from.sin_addr.s_addr;

			LogMessage("Accepted SSL connection from %d.%d.%d.%d\n", ip[0], ip[1], ip[2], ip[3]);

			if (addr == m_IpAddress || g_ServerMgr->m_UDPConnection->IsRegisteredIP(addr))
			{
				SSL_Connection * node = m_ServerMgr.GetSSLConnection();
				if (node)
				{
					SSL_Connection * ssl_connection = node->ReSetSSL_Connection(s, &m_ServerMgr, (unsigned long*)&from.sin_addr, m_ssl_context);

					m_ServerMgr.m_ConnectionMgr.AddSslConnection(ssl_connection);
				}
			}
			else
			{
				LogMessage("Refused connection to %d.%d.%d.%d : No MVAS register.\n", ip[0], ip[1], ip[2], ip[3]);
			}
		}
		else
		{
			// add error handling
		}

		SocketReady(-1); // Wait infinite until something is happening on the socket (a Shutdown() would trigger this too...)
	}

	Shutdown();
}

bool SSL_Listener::SocketReady(int ttimeout)
{
	fd_set fds;
	struct timeval timeout;
	long ret;

	FD_ZERO(&fds);
	FD_SET(m_ListenerSocket, &fds);

	if (ttimeout >= 0)
	{
		timeout.tv_sec	= ttimeout;
		timeout.tv_usec	= 0;

		ret = select(sizeof(fds)*8, &fds, NULL, NULL, &timeout);
	}
	else
	{
		ret = select(sizeof(fds)*8, &fds, NULL, NULL, NULL);
	}

	return bool(ret > 0);	
}

void SSL_Listener::Shutdown()
{
	m_Mutex.Lock();

	if (m_ListenerSocket != INVALID_SOCKET)
	{
		shutdown(m_ListenerSocket, 2);
		closesocket(m_ListenerSocket);
		m_ListenerSocket = INVALID_SOCKET;
	}

	m_SslListenerThreadRunning = false;

	m_Mutex.Unlock();
}
