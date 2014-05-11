// SslListen.h

#ifndef _SSL_LISTEN_H_INCLUDED_
#define _SSL_LISTEN_H_INCLUDED_

#include "Mutex.h"
#include "Net7.h"

#define RETRY_SECONDS 30
#define MAX_TRIES 3

class SslListen
{
public:
    SslListen(unsigned long ip_address, unsigned short port);
    ~SslListen();
    
    void RunThread();
    bool Initialize();
    void Loop();
    void Shutdown();
    
private:
    Mutex m_Mutex;
    unsigned long m_IpAddress;
    unsigned short m_Port;
    SOCKET m_Socket;
	bool m_Running;
#ifndef WIN32
    pthread_t m_Thread;
#endif
};

#endif // _SSL_LISTEN_H_INCLUDED_
