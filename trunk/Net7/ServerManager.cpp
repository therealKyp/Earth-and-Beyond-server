// ServerManager.cpp

#include "Net7.h"
#include "ServerManager.h"
#include "SSL_Listener.h"
#include "TcpListener.h"
#include "UDPConnection.h"
#include "MessageQueue.h"
#include "Connection.h"
#include "openssl/ssl.h"
#include "SectorContentParser.h"
#include "ItemBase.h"
#include "StationLoader.h"
#include "CBAssetParser.h"
#include "SaveManager.h"
#include "SSL_Connection.h"

// Constructor
ServerManager::ServerManager(bool is_master_server, unsigned long ip_address, short port, short max_sectors, bool standalone, unsigned long internal_ip_address)
	:
	m_IsMasterServer(is_master_server),
	m_IpAddress(ip_address),
	m_IpAddressInternal(internal_ip_address),
	m_Port(port),
    m_MaxSectors(max_sectors),
    m_IsStandaloneServer(standalone)
{
    m_SectorServerMgr.SetServerManager(this);
    m_LogFileTimer = 0;
    m_LogFile = (0);
    m_ChatFileTimer = 0;
	m_MySQLFileTimer = 0;
    m_ChatFile = (0);
	m_MySQLFile = (0);
	m_AllowCreate = false;
	m_DumpXML = false;
    m_SkillList = (0);
	m_SectorUpdateSelect = false;

	g_ServerMgr = this;

	//now allocate the global circular buffers, one for TCP player send and one for UDP player send
	long buffer_space = 0x80000 * (MAX_ONLINE_PLAYERS / 50); //reserve 1/2 meg per 50 players
	long buffer_slots = 20000 * (MAX_ONLINE_PLAYERS / 50); //reserve 20000 slots per 50 players
	m_UDPSendBuffer = new CircularBuffer(buffer_space/4, buffer_slots/4); //never need as much UDP space as we need for TCP logins
	m_TCPSendBuffer = new CircularBuffer(buffer_space, buffer_slots);

    m_StringMgr = new StringManager();
    g_StringMgr = m_StringMgr;

    m_GlobMemMgr = new GMemoryHandler(MAX_ONLINE_PLAYERS);
    g_GlobMemMgr = m_GlobMemMgr;

    m_AccountMgr = new AccountManager();
    g_AccountMgr = m_AccountMgr;

	m_SaveMgr = new SaveManager();
	g_SaveMgr = m_SaveMgr;

    g_PlayerMgr = &m_PlayerMgr;
    g_ItemBaseMgr = &m_ItemBaseMgr;

	m_SectorAssignmentsComplete = false;
	
	m_Connections = new MemorySlot<Connection>(MAX_ONLINE_PLAYERS);
#ifndef SSL_IN_NET7PROXY
	m_SSL_Connections = new MemorySlot<SSL_Connection>(75); //allow 75 people to be auth'd simultaneously, should be enough
#endif
}

// Destructor
ServerManager::~ServerManager()
{
    // TODO: The server manager must wait for all threads to die before destructing!!!
    // The PlayerManager takes a while to save all Player information to disk!!!
}

// This is the entry point for running the server
void ServerManager::RunServer()
{
	if (m_IsMasterServer || m_IsStandaloneServer)
	{
        // This is a Master Server or a Standalone Server
		RunMasterServer();
	}
	else
	{
        // This is a Sector Server
		RunSectorServer();
	}
}

void ServerManager::RunMasterServer()
{
	UDP_Connection udp_global_server_listener(UDP_GLOBAL_SERVER_PORT, this, CONNECTION_TYPE_GLOBAL_SERVER_TO_PROXY);

	// set this #if to zero to test local cert login on the same machine as the server (Net7Proxy /LC) - otherwise local cert won't work with this source active
	// this might be done if you're a dev and you need to test something in the local cert login
#if 1
	// Instantiate the SSL Listener object
	m_SSL_listener = new SSL_Listener(m_IpAddressInternal, SSL_PORT, *this);
	// Instantiate the TCP Listener object for the Global Server
	TcpListener global_server_listener(m_IpAddressInternal, GLOBAL_SERVER_PORT, *this, CONNECTION_TYPE_CLIENT_TO_GLOBAL_SERVER);

	RegisterSectorServer(SECTOR_SERVER_PORT, m_MaxSectors);
#endif
	
	// This is all the old pre-net7proxy stuff
	//  Instantiate the TCP Listener object for the Global Server
	//  TcpListener global_server_listener(m_IpAddressInternal, GLOBAL_SERVER_PORT, *this, CONNECTION_TYPE_CLIENT_TO_GLOBAL_SERVER);
    //  UdpListener global_server_listener(m_IpAddressInternal, UDP_GLOBAL_SERVER_PORT, *this, CONNECTION_TYPE_CLIENT_TO_GLOBAL_SERVER);
    //  UDP_Connection global_server_listener(UDP_GLOBAL_SERVER_PORT, this, CONNECTION_TYPE_GLOBAL_SERVER_TO_PROXY);
    //  m_GlobalConnection = &global_server_listener;
	// end pre-net7proxy stuff

	// Instantiate the TCP Listener object for the Master (galaxy) Server
	TcpListener proxy_tcp_listener(m_IpAddressInternal, PROXY_SERVER_PORT, *this, CONNECTION_TYPE_SECTOR_SERVER_TO_PROXY);
    //UdpListener master_tcp_listener(m_IpAddressInternal, MASTER_SERVER_PORT, *this, CONNECTION_TYPE_CLIENT_TO_MASTER_SERVER);
    UDP_Connection master_udp_listener(UDP_MASTER_SERVER_PORT, this, CONNECTION_TYPE_MASTER_SERVER_TO_PROXY);
    m_UDPMasterConnection = &master_udp_listener;

    //TcpListener sector_comms(m_IpAddressInternal, SECTOR_SERVER_PORT, *this, CONNECTION_TYPE_CLIENT_TO_SECTOR_SERVER);
    //RegisterSectorServer(SECTOR_SERVER_PORT, m_MaxSectors);

	SkillParser	SkillLoad;

#ifdef USE_MYSQL_SECTOR
	m_SkillsList.LoadSkillsContent();
	m_BuffData.LoadBuffContent();
    m_MOBList.LoadMOBContent();
	m_AssetList.LoadAssetContent();
	m_FactionData.LoadFactions();
#endif

	if(!m_CBassetList.ParseRadii())
	{
		//Error - Couldn't parse cbasset.xml
		LogMessage("Error - Could not parse \'cbasset.xml\'.\n");
	}

	m_ItemBaseMgr.Initialize();
	m_SectorContent.LoadSectorContent();

	// Load Stations from MySQL
#ifdef USE_MYSQL_STATIONS
	m_StationMgr.LoadStations();
#endif
 	
	SkillLoad.LoadSkills();
	m_SkillList = SkillLoad.GetSkillList();
	//m_SkillAbilities = SkillLoad.GetAbilityList();

#ifdef USE_MYSQL_STATIONS
	m_Missions.LoadMissionContent();
#endif

	if (m_IsStandaloneServer)
	{
		int i;
		for (i = 0; i < m_MaxSectors; i++)
		{
			short port = m_Port + i;
			//LogMessage("Starting listener on port %d\n", port);
			m_SectorMgrList[i] = new SectorManager(this);
			//m_SectorMgrList[i]->SetSectorData(m_SectorContent.GetSec);
			m_SectorMgrList[i]->StartListener(port);
			m_SectorMgrList[i]->SetBoundaries(i);
			m_SectorMgrList[i]->SetSectorNumber(i);
		}

		m_SectorCount = i;

		// Wait 2 seconds for the listeners to start before registering
		for (i = 0; i < 40; i++)
		{
			// Loop 20x per second
			Sleep(50);
			ServerCheck();
		}

		LogMessage("Registering sector server: port=%d, max_sectors=%d\n", m_Port, m_MaxSectors);
		//RegisterSectorServer(m_Port, m_MaxSectors);
		//RegisterSectorServer(GLOBAL_SERVER_PORT, m_MaxSectors);

		m_SectorServerMgr.SectorLockdown();

		MainLoop();

		for (i = 0; i < m_SectorCount; i++)
		{
			delete m_SectorMgrList[i];
			m_SectorMgrList[i] = NULL;
		}
    }
    else
    {
		m_SectorServerMgr.SectorLockdown();
    	MainLoop();
    }

	m_SSL_listener->Shutdown();
	//global_server_listener.Shutdown();
	udp_global_server_listener.Shutdown();
	master_udp_listener.Shutdown();
}

// This runs a single sector server on a single port
void ServerManager::RunSectorServer()
{
    SectorContentParser parser;
    if (!parser.LoadSectorContent())
    {
        printf("Fatal error parsing sector content XML file. Program aborted.\n");
    }
    else
    {
        // Start a sector manager for each sector
        int i;
        for (i = 0; i < m_MaxSectors; i++)
        {
            short port = m_Port + i;
            //LogMessage("Launching SectorManager on port %d\n", port);
            m_SectorMgrList[i] = new SectorManager(this);
            //m_SectorMgrList[i]->SetSectorData(parser.GetSectorData());
            m_SectorMgrList[i]->StartListener(port);
        }

        // Wait 2 seconds for the listeners to start before registering
        for (i = 0; i < 40; i++)
        {
            // Loop 20x per second
            Sleep(50);
            ServerCheck();
        }

	    // Register this Sector Server with the Authentication Server
        LogMessage("Registering sector server with Authentication Server\n");
        RegisterSectorServer(m_Port, m_MaxSectors);

	    MainLoop();
    }
}

FILE *OpenLogFile(FILE *logfile, char *name)
{
    // We have at least one message in the queue
    if (!logfile)
    {
        // If the log file is not open, then open it
        // Create log filename with the current date
        SYSTEMTIME systime;
        GetSystemTime(&systime);
        char filename[MAX_PATH];
        sprintf(filename, "%s_%04d_%02d_%02d.log", name, systime.wYear, systime.wMonth, systime.wDay);
        logfile = fopen(filename, "a+");
    }
    return logfile;
}

void ServerManager::ServerCheck()
{
    // called by the Main thread in MainLoop
	// Kill any connections that have closed
	m_ConnectionMgr.CheckConnections();
	m_ConnectionMgr.CheckSslConnections();

	// process saves
	m_SaveMgr->CheckSaves();

	// run player updates
	if (m_SectorUpdateSelect)
	{
		m_PlayerMgr.RunMovementThread();
	}

	if (m_SectorAssignmentsComplete)
	{
		for (int i = 0; i < m_SectorCount; i++)
		{		
			if ((m_SectorUpdateSelect && i % 2 == 1) || (!m_SectorUpdateSelect && i % 2 == 0))
			{
				m_SectorMgrList[i]->RunSectorEventThread();
			}
		}
	}

	m_SectorUpdateSelect = !m_SectorUpdateSelect;

	if (!m_SectorAssignmentsComplete && (m_IsMasterServer || m_IsStandaloneServer))
	{
		m_SectorAssignmentsComplete = m_SectorServerMgr.CheckConnections();
	}

    //===========================================
    // Check for messages in the Server Log queue
    //===========================================

    /*if (m_LogFileTimer)
    {
		// if the log file has been idle for 2 seconds, close it
        m_Mutex.Lock();
        m_LogFileTimer--;
        if (m_LogFileTimer == 0 && m_LogFile != NULL)
        {
            fclose(m_LogFile);  // close the log file
			m_LogFile = NULL;   // forget the file handle
        }
        m_Mutex.Unlock();
    }

    //===========================================
    // Check for messages in the Chat Msg queue
    //===========================================

    if (m_ChatFileTimer)
    {
		// if the chat file has been idle for 2 seconds, close it
        m_Mutex.Lock();
        m_ChatFileTimer--;
        if (m_ChatFileTimer == 0 && m_ChatFile != NULL)
        {
            fclose(m_ChatFile);  // close the chat file
			m_ChatFile = NULL;   // forget the file handle
        }
        m_Mutex.Unlock();
    }

    //===========================================
    // Check for messages in the Chat Msg queue
    //===========================================

    if (m_MySQLFileTimer)
    {
		// if the chat file has been idle for 2 seconds, close it
        m_Mutex.Lock();
        m_MySQLFileTimer--;
        if (m_MySQLFileTimer == 0 && m_MySQLFile != NULL)
        {
            fclose(m_MySQLFile);  // close the chat file
			m_MySQLFile = NULL;   // forget the file handle
        }
        m_Mutex.Unlock();
    }*/
}

void ServerManager::MainLoop()
{
    //LogMessage("Entering MainLoop\n");
    //m_Missions.Initialise();
	u32 check_tick;
	long sleep_time;
	while (!g_ServerShutdown)
	{
		check_tick = GetNet7TickCount();
		ServerCheck();
		sleep_time = (long)(check_tick - GetNet7TickCount()) + 50; 
		if (sleep_time < 0) sleep_time = 0;
		Sleep(sleep_time);        
	}

	LogMessage("Server Shutting down ...\n");

	ServerCheck();
	ServerCheck(); //blip servercheck to clear any remaining messages to players

    if (m_LogFile)
    {
        fclose(m_LogFile);
		m_LogFile = NULL;
	}

	// TODO: Use event notification to make this safe
	// Wait for clean shutdown
	Sleep(5000);
}

SectorManager *ServerManager::GetSectorManager(short port)
{
    for (int i = 0; i < m_MaxSectors; i++)
    {
        if (m_SectorMgrList[i] && (m_SectorMgrList[i]->GetTcpPort() == port))
        {
            return m_SectorMgrList[i];
        }
    }
    return NULL;
}

SectorManager *ServerManager::GetSectorManager(long sector_id)
{
    for (int i = 0; i < m_MaxSectors; i++)
    {
        if (m_SectorMgrList[i] && (m_SectorMgrList[i]->GetSectorID() == sector_id))
        {
            return m_SectorMgrList[i];
        }
    }
    return NULL;
}

bool ServerManager::SetupSectorServer(long sector_id)
{
	bool success = false;

    // Get a sector manager that has not been assigned a sector yet
    SectorManager *mgr = GetSectorManager((long) -1);
    if (mgr)
    {
	    if (mgr->SetupSectorServer(sector_id))
	    {
		    success = true;
	    }
    }

	return (success);
}

bool ServerManager::IsSectorServerReady(short port)
{
    bool ready = false;
    SectorManager *mgr = GetSectorManager(port);
    if (mgr)
    {
        ready = mgr->IsSectorServerReady();
    }

    return (ready);
}

void ServerManager::SetSectorServerReady(short port, bool ready)
{
    SectorManager *mgr = GetSectorManager(port);
    if (mgr)
    {
        mgr->SetSectorServerReady(ready);
    }
}

// This function formats a message and adds it to the message queue
void LogMySQLMsg(char *format, ...)
{
    char buffer[8192];
    char timestr[20];
	char LogFile[MAX_PATH];
	time_t rawtime;
	struct tm * timeinfo;

    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	// Add _MySQL to the log file name
	sprintf(LogFile, "%s_MySQL", g_LogFilename);
	
	strftime(timestr, 18, "%d/%m/%y %H:%M:%S",timeinfo);

    if (g_ServerMgr && g_ServerMgr->m_MySQLFile)
    {
        g_ServerMgr->ResetMySQLFileTimer(); //m_ChatFileTimer = 40;
		g_ServerMgr->m_MySQLFile = OpenLogFile(g_ServerMgr->m_MySQLFile, LogFile);
		fprintf(g_ServerMgr->m_MySQLFile, "%s %s", timestr , buffer);
    }
}

// This function formats a message and adds it to the message queue
void LogChatMsg(char *format, ...)
{
    char buffer[8192];
    char timestr[20];
	time_t rawtime;
	struct tm * timeinfo;

    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	
	strftime(timestr, 18, "%d/%m/%y %H:%M:%S",timeinfo);

    if (g_ServerMgr)
    {
        g_ServerMgr->ResetChatFileTimer(); //m_ChatFileTimer = 40;
        g_ServerMgr->m_ChatFile = OpenLogFile(g_ServerMgr->m_ChatFile, g_LogFilename);
        fprintf(g_ServerMgr->m_ChatFile, "%s %s", timestr , buffer);
    }
}

void ServerManager::ResetMySQLFileTimer()
{
    m_Mutex.Lock();
    g_ServerMgr->m_MySQLFileTimer = 40;
    m_Mutex.Unlock();
}

void ServerManager::ResetChatFileTimer()
{
    m_Mutex.Lock();
    m_ChatFileTimer = 40;
    m_Mutex.Unlock();
}

void ServerManager::ResetLogFileTimer()
{
    m_Mutex.Lock();
    m_LogFileTimer = 40;
    m_Mutex.Unlock();
}

// This function formats a message and adds it to the message queue
void LogMessage(char *format, ...)
{
    char buffer[8192];
    char timestr[20];
	time_t rawtime;
	struct tm * timeinfo;

    va_list args;
    va_start(args, format);
	try
	{
		vsprintf(buffer, format, args);
	}
	catch (...)
	{
		va_end(args);
		fprintf(stderr,"Bad Log attempt\n");
		return;
	}
    va_end(args);

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	strftime(timestr, 18, "%d/%m/%y %H:%M:%S",timeinfo);

    //print and store - why do we try to buffer this anyway?

    if (g_ServerMgr)
    {
        g_ServerMgr->ResetLogFileTimer();//m_LogFileTimer = 40;
        g_ServerMgr->m_LogFile = OpenLogFile(g_ServerMgr->m_LogFile, g_LogFilename);
        fprintf(g_ServerMgr->m_LogFile, "%s %s", timestr, buffer);
    }

    fprintf(stdout, "%s %s", timestr, buffer); //TODO: put this on a 'verbose' switch
}

void LogDebug(char *format, ...)
{  
    if (!g_Debug) return;

	return; //no logdebugs for now, crashes the server

    char buffer[8192];
    char timestr[20];
	time_t rawtime;
	struct tm * timeinfo;


    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	
	strftime(timestr, 18, "%d/%m/%y %H:%M:%S",timeinfo);

    if (g_ServerMgr)
    {
        g_ServerMgr->m_LogFile = OpenLogFile(g_ServerMgr->m_LogFile, g_LogFilename);
        g_ServerMgr->m_LogFileTimer = 40;
        fprintf(g_ServerMgr->m_LogFile, "%s %s", timestr , buffer);
    }
}

void DumpBuffer(unsigned char *buffer, int length)
{
	char line[128];
    line[0] = 0;
	for (int i = 0; i < length; i++)
	{
		sprintf(line + strlen(line), "%02X ", buffer[i]);
		if ((i % 16) == 15)
		{
			LogMessage("%s\n",line);
            line[0] = 0;
		}
	}
    if (line[0])
    {
        LogMessage("%s\n",line);
    }
}

void DumpBufferToFile(unsigned char *buffer, int length, char *filename, bool rawData)
{
	FILE *f = fopen(filename, "wb");

	if (f)
	{
		if (rawData)
		{
			fwrite(buffer,1,length,f);
		}
		else
		{
			char line[128];
			line[0] = 0;
			for (int i = 0; i < length; i++)
			{
				sprintf(line + strlen(line), "%02X ", buffer[i]);
				if ((i % 16) == 15)
				{
					fprintf(f, "%s\n", line);
					line[0] = 0;
				}
			}
			if (line[0])
			{
				fprintf(f, "%s\n", line);
			}
		}
		LogMessage("Data written to %s\n",filename);
		fclose(f);
	}
	else
	{
		LogMessage("Could not open %s\n",filename);
	}
}

// This is called only for ONE instance of the sector manager
bool ServerManager::RegisterSectorServer(short first_port, short max_sectors)
{
	char buffer[4096];
	SSL_METHOD * ssl_client_method;
	SSL_CTX * ssl_context;
	SSL * ssl;

    SSLeay_add_ssl_algorithms();
	ssl_client_method = SSLv2_client_method();
	SSL_load_error_strings();
	ssl_context = SSL_CTX_new(ssl_client_method);
	if (!ssl_context)
	{
        LogMessage("SSL_CTX_new failed\n");
        return false;
	}

	// Establish a SSL connection to the Authentication Server
	// Create a socket
	SOCKET ssl_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ssl_socket == INVALID_SOCKET)
    {
        LogMessage("Unable to create outgoing SSL socket\n");
        return false;
    }

    unsigned long ip_address = 0x0100007f;
    //if (strstr(g_DomainName, "local") == 0)
    //{
	struct hostent * host;

	if (strlen(g_InternalIP)==0)
	    host = gethostbyname(g_DomainName);
	else
	    host = gethostbyname(g_InternalIP);

	    if (!host)
	    {
           LogMessage("Unable to resolve IP address for %s\n", g_DomainName);
            return false;
	    }
        ip_address = *((unsigned long *) host->h_addr_list[0]);
    //}

	struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = ip_address;
	address.sin_port = htons(SSL_PORT);

    unsigned char * ip = (unsigned char *) &ip_address;
    LogMessage("Connecting to Authentication Server on %d.%d.%d.%d:%d\n",
        ip[0], ip[1], ip[2], ip[3], SSL_PORT);
	if (connect(ssl_socket, (struct sockaddr*) &address, sizeof(address)))
	{
        LogMessage("Unable to connect to Authentication Server on port %d\n", SSL_PORT);
        return false;
	}

    //LogMessage("SSL Connected!\n");

	ssl = SSL_new(ssl_context);
	if (!ssl)
	{
        LogMessage("SSL_new failed\n");
        return false;
	}

	SSL_set_fd(ssl, ssl_socket);

	if (!SSL_connect(ssl))
	{
        LogMessage("SSL_connect failed\n");
        return false;
	}

    // TODO: change this from a hard-coded username something that is set on the command line
    // or a data file.
	sprintf(buffer,
		"GET /sectorserver.cgi?username=VectoR&port=%d&max_sectors=%d&version=%d.%d HTTP/1.1\r\n"
		"User-Agent: AuthLogin\r\n"
        "Host: %s\r\n"
		"Connection: Keep-Alive\r\n"
		"Cache-Control: no-cache\r\n"
		"\r\n",
		first_port,
        max_sectors,
		SECTOR_SERVER_MAJOR_VERSION,
        SECTOR_SERVER_MINOR_VERSION,
        g_DomainName);

    //printf("------\n", buffer);
    //printf("%s", buffer);
    //printf("------\n", buffer);

    //LogMessage("SectorManager calling SSL_write (%d bytes)\n", strlen(buffer) + 1);
	if (SSL_write(ssl, buffer, strlen(buffer) + 1) == -1)
	{
        LogMessage("SSL_write failed\n");
        return false;
	}

    //LogMessage("SectorManager calling SSL_read\n");
	int bytes = SSL_read(ssl, buffer, sizeof(buffer) - 1);
	if (bytes == -1)
	{
        LogMessage("SSL_read failed\n");
		return false;
	}
    buffer[bytes] = 0;

	/* Clean up. */
    //LogMessage("SectorManager calling closesocket\n");
	closesocket(ssl_socket);
    //LogMessage("SectorManager calling SSL_free\n");
	SSL_free(ssl);
    //LogMessage("SectorManager calling SSL_CTX_free\n");
	SSL_CTX_free(ssl_context);

	if (strstr(buffer, "Success=TRUE") == 0)
	{
        LogMessage("Attempt to register the Sector Server failed\n");
        LogMessage("SSL Response:%s\n", buffer);
		return false;
	}
    //else
    //{
    //  LogMessage("Successfully registered the Sector Server!\n");
	//}

	return true;
}

void ServerManager::SetUDPConnection(UDP_Connection* connection)
{
	m_UDPConnection = connection;
	m_PlayerMgr.SetUDPConnection(connection);
}

void ServerManager::SetPlayerMgrGlobalMemoryHandler()
{
    m_PlayerMgr.SetGlobalMemoryHandler(m_GlobMemMgr);
}

Connection* ServerManager::GetConnection()
{
	Connection *c = m_Connections->GetInactiveNode();
	if (c == 0)
	{
		LogMessage(">>>>> CRITICAL ERROR! Out Of Connection Nodes!! This should only happen if we exceed the 100 player mark.\n");
	}
	else
	{
		c->SetGameID(-1);
	}
	return c;
}

SSL_Connection* ServerManager::GetSSLConnection()
{
	SSL_Connection *c;
#ifndef SSL_IN_NET7PROXY
	c = m_SSL_Connections->GetNode();
#else
	c = 0;
	return c;
#endif

	//is this node active? If so, kill it.
	if (c->IsActive())
	{
		c->KillConnection();
	}

	c->SetGameID(1);

	return c;
}

void ServerManager::AddSector(long sector_id, char *sector_name, char *system_name, char *parent_sector_name)
{
	m_SectorServerMgr.AddSector(sector_id, sector_name, system_name, parent_sector_name);
}

char *ServerManager::GetSectorName(long sector_id)
{
	if (sector_id < 10000)
	{
		return (m_SectorContent._GetSectorName(sector_id));
	}
	else
	{
		return (m_StationMgr._GetSectorName(sector_id));
	}
}

char *ServerManager::GetSystemName(long sector_id)
{
	if (sector_id > 9999) sector_id = sector_id / 10;
	return (m_SectorContent._GetSystemName(sector_id));
}
