// SectorServer.h

#ifndef _SECTOR_SERVER_H_INCLUDED_
#define _SECTOR_SERVER_H_INCLUDED_

#include "Server.h"

class SectorServer : public Server
{
public:
    SectorServer(unsigned int ip_address);
    ~SectorServer();
    
    void Initialize();
};

#endif // _SECTOR_SERVER_H_INCLUDED_
