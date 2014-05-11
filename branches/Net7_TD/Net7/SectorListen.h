// SectorListen.h

#ifndef _SECTOR_LISTEN_H_INCLUDED_
#define _SECTOR_LISTEN_H_INCLUDED_

#include "TcpListen.h"
#include "TcpConnection.h"

class SectorListen : public TcpListen
{
public:
    SectorListen(unsigned int ip_address, unsigned short port);
    ~SectorListen();

protected:
    TcpConnection *CreateConnection(SOCKET sock, unsigned short port);
};

#endif // _SECTOR_LISTEN_H_INCLUDED_
