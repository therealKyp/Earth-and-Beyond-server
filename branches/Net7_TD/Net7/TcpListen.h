// TcpListen.h

#ifndef _TCP_LISTEN_H_INCLUDED_
#define _TCP_LISTEN_H_INCLUDED_

#include "Mutex.h"
#include "TcpConnection.h"

#define RETRY_SECONDS 30
#define MAX_TRIES 3

class TcpListen
{
public:
    TcpListen(unsigned int ip_address, unsigned short port);
    virtual ~TcpListen();

    void RunThread();
    virtual bool Initialize();
    virtual void Loop();
    void Shutdown();
    
    virtual TcpConnection *CreateConnection(SOCKET sock, unsigned short port) = 0;
    
private:
    Mutex   m_Mutex;
    
    unsigned int m_IpAddress;
    unsigned short m_Port;

    SOCKET  m_Socket;
	bool	m_Running;
#ifndef WIN32
    pthread_t m_Thread;
#endif
};

#endif // _TCP_LISTEN_H_INCLUDED_
