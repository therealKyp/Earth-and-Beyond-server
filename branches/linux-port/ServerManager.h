// ServerManager.h

#ifndef _SERVER_MANAGER_H_INCLUDED_
#define _SERVER_MANAGER_H_INCLUDED_

#include "AccountManager.h"
#include "SectorServerManager.h"
#include "ConnectionManager.h"
#include "SectorManager.h"
#include "ItemBaseManager.h"
#include "SkillParser.h"
#include "StationLoader.h"
#include "MemoryHandler.h"
#include "StringManager.h"
#include "MissionParser.h"
#include "MOBDatabase.h"
#include "AssetDatabase.h"
#include "SectorContentParser.h"
#include "CBAssetParser.h"
#include "MissionDatabaseSQL.h"
#include "SkillsDatabase.h"
#include "BuffDatabaseSQL.h"

class Connection;
class Player;
class SaveManager;
class CircularBuffer;

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
    void    StoreCharacterData(CharacterDatabase &character_database);
    bool    RequestCharacterDatabase(long avatar_id, CharacterDatabase &character_database);
	void	SetUDPConnection(UDP_Connection* connection);
    void    SetPlayerMgrGlobalMemoryHandler();

	bool	IsSectorAssignmentsComplete()				{ return m_SectorAssignmentsComplete; }

	void	AddSector(long sector_id, char *sector_name, char *system_name, char *parent_sector_name);

    SectorManager *GetSectorManager(short port);
    SectorManager *GetSectorManager(long sector_id);   

	char * GetSectorName(long sector_id);
	char * GetSystemName(long sector_id);

	Connection	  *GetConnection();

	void	ResetMySQLFileTimer();
    void    ResetChatFileTimer();
    void    ResetLogFileTimer();

    MOBContent		* MOBList()			{ return &m_MOBList; }
	AssetContent	* AssetList()		{ return &m_AssetList; }
	CBAssetParser	* BAssetRadii()		{ return &m_CBassetList; }

	CircularBuffer	* GetUDPCBuffer()	{ return m_UDPSendBuffer; }
	CircularBuffer	* GetTCPCBuffer()	{ return m_TCPSendBuffer; }

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
	SectorServerManager	m_SectorServerMgr;

	// Applies only to Sector Server
	SectorManager	  * m_SectorMgrList[MAX_SECTORS];
	StationLoader		m_StationMgr;
	BuffContent			m_BuffData;			// Holds data on buff effects
    PlayerManager       m_PlayerMgr;
	ItemBaseManager		m_ItemBaseMgr;		// ItemBase
	MissionHandler		m_Missions;			// Contains all the missions
	SkillData		  * m_SkillList;
    MOBContent          m_MOBList;
	SkillsContent		m_SkillsList;
	AssetContent		m_AssetList;
	CBAssetParser		m_CBassetList;
	SectorContentParser m_SectorContent;
	UDP_Connection	  * m_UDPConnection;	// for MVAS
    UDP_Connection    * m_UDPMasterConnection;
    GMemoryHandler    * m_GlobMemMgr;
    StringManager     * m_StringMgr;
	SaveManager		  * m_SaveMgr;
	MemorySlot<Connection> *m_Connections;
	MemorySlot<SSL_Connection> *m_SSL_Connections;
	short				m_Port;
    short               m_MaxSectors;
	unsigned long		m_IpAddress;
	unsigned long		m_IpAddressInternal;
	long				m_SectorID;
    int                 m_LogFileTimer;
    int                 m_ChatFileTimer;
	int					m_MySQLFileTimer;
    FILE              * m_LogFile;
    FILE              * m_ChatFile;
	FILE			  * m_MySQLFile;
	bool				m_AllowCreate;
	bool				m_DumpXML;

private:
    Mutex               m_Mutex;
	bool				m_SectorAssignmentsComplete;

	CircularBuffer	  * m_UDPSendBuffer;
	CircularBuffer    * m_TCPSendBuffer;

};

#endif // _SERVER_MANAGER_H_INCLUDED_
