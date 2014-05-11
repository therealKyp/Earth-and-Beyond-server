// ServerManager.cpp

#include "Net7.h"
#include "ServerManager.h"
#include "SSL_Listener.h"
#include "TcpListener.h"
#include "MessageQueue.h"
#include "Connection.h"
#include "openssl/ssl.h"

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
    m_LogFileTimer = 0;
    m_LogFile = NULL;
    m_ChatFileTimer = 0;
    m_ChatFile = NULL;
	m_AllowCreate = false;
	m_DumpXML = false;
    m_GlobalConnection = (0);
    m_SectorConnection = (0);
    m_ConnectionCount = 0;
}

// Destructor
ServerManager::~ServerManager()
{

}

// This is the entry point for running the server
void ServerManager::RunServer()
{
    g_ServerMgr = this;
    
    RunMasterServer();
}

void ServerManager::RunMasterServer()
{
	SSL_Listener *ssl_listener = 0;
	TcpListener *global_server_listener = 0;

	if (g_LocalCert)
	{
		// Instantiate the SSL Listener object
		ssl_listener = new SSL_Listener(m_IpAddressInternal, ssl_port, *this);
		RegisterSectorServer(SECTOR_SERVER_PORT, m_MaxSectors);
		// Instantiate the TCP Listener object for the Global Server
		global_server_listener = new TcpListener(m_IpAddressInternal, GLOBAL_SERVER_PORT, *this, CONNECTION_TYPE_CLIENT_TO_GLOBAL_SERVER);
	}

	// Instantiate the TCP Listener object for the Master (galaxy) Server
	TcpListener master_tcp_listener(m_IpAddressInternal, MASTER_SERVER_PORT, *this, CONNECTION_TYPE_CLIENT_TO_MASTER_SERVER);

    //give ourselves 1 port for sector serving.
    TcpListener sector_comms(m_IpAddressInternal, SECTOR_SERVER_PORT, *this, CONNECTION_TYPE_CLIENT_TO_SECTOR_SERVER);

    MainLoop();

    sector_comms.Shutdown();

	if (g_LocalCert)
	{
		ssl_listener->Shutdown();
		global_server_listener->Shutdown();
	}
	master_tcp_listener.Shutdown();
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
	//m_ConnectionMgr.CheckSslConnections();

    //===========================================
    // Check for messages in the Server Log queue
    //===========================================

    if (m_LogFileTimer)
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
}

void ServerManager::MainLoop()
{
    //LogMessage("Entering MainLoop\n");
	while (!g_ServerShutdown)
	{
		// Loop 20x per second
		Sleep(50);
        ServerCheck();
	}

    if (m_LogFile)
    {
        fclose(m_LogFile);
		m_LogFile = NULL;
	}

	// TODO: Use event notification to make this safe
	// Wait for clean shutdown
	Sleep(5000);
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
    _vsnprintf(buffer, 8192, format, args);
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

    if (g_ServerShutdown) return;

    va_list args;
    va_start(args, format);
    _vsnprintf(buffer, 8192, format, args);
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

    char buffer[8192];
    char timestr[20];
	time_t rawtime;
	struct tm * timeinfo;

    if (g_ServerShutdown) return;

    va_list args;
    va_start(args, format);
    _vsnprintf(buffer, 8192, format, args);
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
		//sprintf_s(line + strlen(line), 128, "%02X ", buffer[i]);
		_snprintf(line + strlen(line), 128, "%02X ", buffer[i]);
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
				_snprintf(line + strlen(line), 128, "%02X ", buffer[i]);
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

void ServerManager::SetUDPConnections(UDPClient *connection, UDPClient *send)
{
	m_UDPConnection = send;
    m_UDPClient     = connection;
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
	address.sin_port = htons(ssl_port);

    unsigned char * ip = (unsigned char *) &ip_address;
    LogMessage("Connecting to Authentication Server on %d.%d.%d.%d:%d\n",
        ip[0], ip[1], ip[2], ip[3], ssl_port);
	if (connect(ssl_socket, (struct sockaddr*) &address, sizeof(address)))
	{
        LogMessage("Unable to connect to Authentication Server on port %d\n", ssl_port);
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
	_snprintf(buffer, 128,
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