// SSL_Listener.h

#ifndef _SSL_LISTENER_H_INCLUDED_
#define _SSL_LISTENER_H_INCLUDED_

#include "Mutex.h"
#include "openssl/ssl.h"

class ServerManager;

class SSL_Listener
{
public:
	SSL_Listener(unsigned long ip_address, unsigned short port, ServerManager &server_mgr);
	virtual ~SSL_Listener();

public:
	void		RunThread();
	void		Shutdown();

	bool		ListenerThreadRunning()	{ return m_SslListenerThreadRunning; }

private:
	bool		SocketReady(int ttimeout);
	Mutex		m_Mutex;
	WORD		m_TcpPort;
	unsigned long	m_IpAddress;
	ServerManager	&m_ServerMgr;
	SOCKET		m_ListenerSocket;
	bool		m_SslListenerThreadRunning;

	SSL_CTX 	*m_ssl_context;

#ifndef WIN32
	pthread_t m_Thread;
#endif
};

#endif // _SSL_LISTENER_H_INCLUDED_

