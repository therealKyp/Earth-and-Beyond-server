// MasterListen.h

#ifndef _MASTER_LISTEN_H_INCLUDED_
#define _MASTER_LISTEN_H_INCLUDED_

#include "TcpListen.h"
#include "TcpConnection.h"

class MasterListen : public TcpListen
{
public:
    MasterListen(unsigned int ip_address, unsigned short port);
    ~MasterListen();

protected:
    TcpConnection *CreateConnection(SOCKET sock, unsigned short port);
};

#endif // _MASTER_LISTEN_H_INCLUDED_
