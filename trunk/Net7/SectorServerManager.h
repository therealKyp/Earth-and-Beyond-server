// SectorServerManager.h

#ifndef _SECTOR_SERVER_MANAGER_H_INCLUDED_
#define _SECTOR_SERVER_MANAGER_H_INCLUDED_

#include "Mutex.h"

#define MAX_SECTOR_SERVERS			        300
#define MAX_SECTORS					        300
#define MAX_SECTOR_CONNECTIONS_PER_SERVER   300

class Connection;
class ServerManager;
struct ServerRedirect;
class UDP_Connection;

class SectorServerManager
{
public:
	SectorServerManager();
	virtual ~SectorServerManager();

public:
	bool	RegisterSectorServer(unsigned long ip_address, short port_number, short max_sectors, char *username);
	bool	CheckConnections();
	void	SetServerManager(ServerManager * server_mgr);
	char  * GetSystemName(long sector_id);
	char  * GetSectorName(long sector_id);
	bool	LookupSectorServer(ServerRedirect & redirect);
	bool	SendPacket(long sector_id, short opcode, unsigned char *data=NULL, size_t length=0);
	void	AddSector(long sector_id, char *sector_name, char *system_name, char *parent_sector_name);
	long	GetNextSectorID(long sector_id);
	void	SectorLockdown()	{ m_ServerLockdown = true; }

private:
	// Linked list of "registered" sector servers
	struct	SectorServer;
	struct	SectorServer
	{
		unsigned long	ip_address;
		short			port;
        short           max_sectors;
		char		  * username;
		//char		  * toon;
		//char		  * email;
		//char		  * server_name;
		long			countdown_to_ready;
		UDP_Connection* udp_connection[MAX_SECTOR_CONNECTIONS_PER_SERVER];
        long            sector_assigned[MAX_SECTOR_CONNECTIONS_PER_SERVER];
		struct SectorServer * next;
	};

private:
	void	LoadSectorList();
	bool	LoadSectorServers();
	bool	ConnectBackToSectorServer(SectorServer *server);
	bool	AssignSectorToAvailableServer(long sector_id, char * sector_name);

private:
    Mutex   m_Mutex;
	ServerManager * m_ServerMgr;
	bool	m_ServerLockdown;

	// sector servers
	long	m_NumSectorServers;
	SectorServer * m_ServerList;

	// sectors
	long	m_NumSectors;
	long	m_NumUnassignedSectors;
	long	m_SectorID[MAX_SECTORS];
	char  *	m_SectorName[MAX_SECTORS];
	char  *	m_SystemName[MAX_SECTORS];
	char  *	m_ParentSectorName[MAX_SECTORS];
	bool	m_SectorAssigned[MAX_SECTORS];
	//short	m_SectorPort[MAX_SECTORS];
	//long	m_SectorAddr[MAX_SECTORS];
};

#endif // _SECTOR_SERVER_MANAGER_H_INCLUDED_

