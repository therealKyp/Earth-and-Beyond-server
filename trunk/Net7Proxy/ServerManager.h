// ServerManager.h

#ifndef _SERVER_MANAGER_H_INCLUDED_
#define _SERVER_MANAGER_H_INCLUDED_

#include "ConnectionManager.h"
#include "SectorServerManager.h"

class Connection;
class SectorManager;
class ConnectionManager;
class UDPClient;

class ServerManager
{
public:
    ServerManager(bool is_sector_server, unsigned long ip_address, short port, short max_sectors, bool standalone, unsigned long internal_ip_address = 0);
    virtual ~ServerManager();

public:
	void	RunServer();
	bool	SetupSectorServer(long sector_id);
	bool	IsSectorServerReady(short port);
	void	SetSectorServerReady(short port, bool ready);
	void	SetUDPConnections(UDPClient *connection, UDPClient *send);
    void    SetPlayerMgrGlobalMemoryHandler();

    void    ResetChatFileTimer();
    void    ResetLogFileTimer();

private:
    void    ServerCheck();
	void	MainLoop();
	void	RunMasterServer();
	void	RunSectorServer();
    bool    RegisterSectorServer(short first_port, short max_sectors);

public:
	// Applies to all servers
	ConnectionManager	m_ConnectionMgr;
	bool				m_IsMasterServer;
    bool                m_IsStandaloneServer;

	// Applies only to Master Server
	AccountManager    * m_AccountMgr;

	// Applies only to Sector Server
	SectorManager	  * m_SectorMgrList[MAX_SECTORS];
	UDPClient	      * m_UDPConnection;  // used for sending to different ports	
    UDPClient         * m_UDPClient;   // used for receiving from the server
	SectorServerManager	m_SectorServerMgr;
	short				m_Port;
    short               m_MaxSectors;
	unsigned long		m_IpAddress;
	unsigned long		m_IpAddressInternal;
	long				m_SectorID;
    int                 m_LogFileTimer;
    int                 m_ChatFileTimer;
    FILE              * m_LogFile;
    FILE              * m_ChatFile;
	bool				m_AllowCreate;
	bool				m_DumpXML;

    StringManager     * m_StringMgr;

    Connection        * m_SectorConnection; //local connection to client
    Connection        * m_GlobalConnection;
    Connection        * m_MasterConnection;
    long                m_ConnectionCount;

private:
    Mutex               m_Mutex;

};

#endif // _SERVER_MANAGER_H_INCLUDED_
