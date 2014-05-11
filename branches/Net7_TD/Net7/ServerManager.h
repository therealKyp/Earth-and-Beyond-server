// ServerManager.h

#ifndef _SERVER_MANAGER_H_INCLUDED_
#define _SERVER_MANAGER_H_INCLUDED_

#include "Mutex.h"
#include "GlobalServer.h"
#include "AccountServer.h"
#include "MasterServer.h"
#include "SectorServer.h"

class ServerManager
{
public:
    ServerManager(unsigned int server_type, unsigned int ip_address, int max_sectors);
    ~ServerManager();
    
    void Run();

private:
    unsigned int m_ServerType;
    unsigned int m_IpAddress;
    int m_MaxSectors;
    
    GlobalServer *m_GlobalServer;
    AccountServer *m_AccountServer;
    MasterServer *m_MasterServer;
    SectorServer *m_SectorServer;
};

#endif // _SERVER_MANAGER_H_INCLUDED_
