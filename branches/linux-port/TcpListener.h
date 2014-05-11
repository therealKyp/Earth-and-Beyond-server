// TcpListener.h

#ifndef _TCP_LISTENER_H_INCLUDED_
#define _TCP_LISTENER_H_INCLUDED_

#include "Mutex.h"
#include "MemoryHandler.h"

class ServerManager;

class TcpListener
{
public:
    TcpListener(unsigned long ip_address, unsigned short port, ServerManager &server_mgr, int server_type);
    virtual ~TcpListener();

public:
    void RunThread();
	void Shutdown();

private:
    unsigned long m_IpAddress;
    WORD    m_TcpPort;
	ServerManager &m_ServerMgr;
	int		m_ServerType;
    SOCKET  m_TcpListenerSocket;
	bool	m_TcpListenerThreadRunning;

#ifndef WIN32
    pthread_t m_Thread;
#endif
};

#endif // _TCP_LISTENER_H_INCLUDED_
