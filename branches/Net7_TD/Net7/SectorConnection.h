// SectorConnection.h

#ifndef _SECTOR_CONNECTION_H_INCLUDED_
#define _SECTOR_CONNECTION_H_INCLUDED_

#include "ClientConnection.h"

class SectorConnection : public ClientConnection
{
public:
    SectorConnection(SOCKET sock, unsigned short port);
    ~SectorConnection();

protected:
    void ProcessOpcode(short opcode, short bytes);

private:

};

#endif // _SECTOR_CONNECTION_H_INCLUDED_
