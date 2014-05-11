// GlobalListen.h

#ifndef _GLOBAL_LISTEN_H_INCLUDED_
#define _GLOBAL_LISTEN_H_INCLUDED_

#include "TcpListen.h"
#include "TcpConnection.h"

class GlobalListen : public TcpListen
{
public:
    GlobalListen(unsigned int ip_address, unsigned short port);
    ~GlobalListen();

protected:
    TcpConnection *CreateConnection(SOCKET sock, unsigned short port);
};

#endif // _GLOBAL_LISTEN_H_INCLUDED_
