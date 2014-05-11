// ClientConnection.h

#ifndef _CLIENT_CONNECTION_H_INCLUDED_
#define _CLIENT_CONNECTION_H_INCLUDED_

#include "TcpConnection.h"

class ClientConnection : public TcpConnection
{
public:
    ClientConnection(SOCKET sock, unsigned short port);
    virtual ~ClientConnection();
    
protected:
    bool KeyExchange();
};

#endif // _CLIENT_CONNECTION_H_INCLUDED_
