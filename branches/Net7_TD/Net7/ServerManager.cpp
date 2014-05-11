// ServerManager.cpp

#include "Net7.h"
#include "ServerManager.h"

ServerManager::ServerManager(unsigned int server_type, unsigned int ip_address, int max_sectors)
: m_ServerType(server_type), m_IpAddress(ip_address), m_MaxSectors(max_sectors)
{
    if(server_type & SERVER_TYPE_GLOBAL)
    {
        m_GlobalServer = new GlobalServer(m_IpAddress);
        m_AccountServer = new AccountServer(m_IpAddress);
    }
    else
    {
        m_GlobalServer = NULL;
        m_AccountServer = NULL;
    }
    
    if(server_type & SERVER_TYPE_MASTER)
        m_MasterServer = new MasterServer(m_IpAddress);
    else
        m_MasterServer = NULL;
    
    if(server_type & SERVER_TYPE_SECTOR)
        m_SectorServer = new SectorServer(m_IpAddress);
    else
        m_SectorServer = NULL;
}

ServerManager::~ServerManager()
{
    if(m_GlobalServer)
        delete m_GlobalServer;
    
    if(m_AccountServer)
        delete m_AccountServer;
    
    if(m_MasterServer)
        delete m_MasterServer;
    
    if(m_SectorServer)
        delete m_SectorServer;
}

void ServerManager::Run()
{

}
