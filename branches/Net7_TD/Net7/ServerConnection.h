// ServerConnection.h

#ifndef _SERVER_CONNECTION_H_INCLUDED_
#define _SERVER_CONNECTION_H_INCLUDED_

#include "TcpConnection.h"

class ServerConnection : public TcpConnection
{
public:
    ServerConnection(SOCKET sock, unsigned short port);
    virtual ~ServerConnection();
    
protected:
    bool KeyExchange();
};

#endif // _SERVER_CONNECTION_H_INCLUDED_
