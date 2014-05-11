// Net7.cpp

#include "Net7.h"
#include "ServerManager.h"
#include "UDPConnection.h"

//DIMA: I don't think these are needed
#define MASTER_INSTANCE_MUTEX_NAME	"Net7 Master Server Instance Mutex"
#define SECTOR_INSTANCE_MUTEX_NAME	"Net7 Sector Server port %ld Instance Mutex"

#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "libmySQL.lib")
#pragma comment(lib, "ssleay32.lib")
#pragma comment(lib, "libeay32.lib")

char g_MySQL_User[MAX_PATH];
char g_MySQL_Pass[MAX_PATH];
char g_MySQL_Host[MAX_PATH];
char g_Galaxy_Name[MAX_PATH];
int g_DASE = 0;

char g_LogFilename[MAX_PATH];
char g_InternalIP[MAX_PATH];
char g_DomainName[MAX_PATH];
unsigned long g_StartTick;

long g_Sector_Start = 973;
long g_Max_Space_Sector = 9000; //2210;

bool g_Debug = false;
bool g_ServerShutdown = false; // Terminated the global Server

ServerManager * g_ServerMgr = 0;
GMemoryHandler * g_GlobMemMgr = 0;
PlayerManager * g_PlayerMgr = 0;
StringManager * g_StringMgr = 0;
ItemBaseManager * g_ItemBaseMgr = 0;
AccountManager * g_AccountMgr = 0;
SaveManager	  * g_SaveMgr = 0;

void Usage()
{
	printf("Net7 Usage:\n\n");
	printf("to run the main server:\n");
	printf("   Net7 /MASTER /ADDRESS:(ip address)\n\n");
	printf("to run a sector server:\n");
	printf("   Net7 /PORT:3500 /ADDRESS:(ip address) /MAX_SECTORS:(num sectors) /ALTSECTORS\n\n");
}

int main(int argc, char* argv[])
{
    // Let the user know when this was compiled for reference purposes
    printf("Net7: Built on %s, at %s\n\n",__DATE__, __TIME__);
    g_StartTick = GetNet7TickCount();

    bool standalone = false;
    bool master_server = false;
    bool sector_server = false;

    long port = SECTOR_SERVER_PORT;
    char address[32];
    char *domain = "";
    char *max_sectors_str = new char[4];
	char *server_name;
	char mutex_name[80];

	//sprintf(max_sectors_str, "74");
	sprintf(max_sectors_str,"300");

	g_MySQL_User[0] = 0;
	g_MySQL_Pass[0] = 0;
	g_Galaxy_Name[0] = 0;
	g_DASE = false;

	srand((unsigned)GetNet7TickCount());

	FILE *f = fopen(CONFIG_FILE, "r");
    if (f)
    {
        fseek(f, 0, SEEK_END);
        long file_size = ftell(f);
        fseek(f, 0, SEEK_SET);
        char *data = new char[file_size + 1];
        if (data)
        {
			char *Info;
			char *VarName;
            long size = fread(data, 1, file_size, f);
            data[size] = 0;
			VarName = strtok(data, "=");
			Info = strtok(NULL, "\n");
			do
            {
				if (!_strcmpi(VarName, "domain"))
                {
					strcpy(g_DomainName, Info);
				}
				if (!_strcmpi(VarName, "internal_ip"))
                {
					strcpy(g_InternalIP, Info);
				}
				if (!_strcmpi(VarName, "mysql_user"))
                {
					strcpy(g_MySQL_User, Info);
                }
				if (!_strcmpi(VarName, "mysql_pass"))
                {
					strcpy(g_MySQL_Pass, Info);
				}
				if (!_strcmpi(VarName, "mysql_host"))
                {
					strcpy(g_MySQL_Host, Info);
				}
				if (!_strcmpi(VarName, "galaxy_name"))
                {
					strcpy(g_Galaxy_Name, Info);
				}
				if (!_strcmpi(VarName, "use_dase"))
				{
					g_DASE = atoi(Info);
				}
				VarName = strtok(NULL, "=");
				Info = strtok(NULL, "\n");
			}
            while(Info != NULL);

            delete [] data;
        }
        fclose(f);
    }
    else
    {
		char filedata[128];
        printf("Error opening %s\n", CONFIG_FILE);
		sprintf(g_DomainName, "local.net-7.org");
		sprintf(filedata, "domain=local.net-7.org\nmysql_user=enb\nmysql_pass=enbserver\nmysql_host=localhost:3307\nmysql_db=net7\ngalaxy_name=Cassiopeia");
		f = fopen(CONFIG_FILE, "w");
		fwrite(filedata,1,strlen(filedata),f);
		fclose(f);
    }

	// if no galaxy name set one!
	if (g_Galaxy_Name[0] == 0)
		strcpy(g_Galaxy_Name, "Cassiopeia");

#ifdef SQL_ENABLE
	printf("MySQL: Host: %s, User: %s\n", g_MySQL_Host, g_MySQL_User);
#endif

    // No arguments indicate a standalone server via localhost
    for (int i = 1; i < argc; i++)
    {
	    if ((strncmp(argv[i], "/DOMAIN:", 8) == 0))
	    {
            domain = argv[i] + 8;
        }
        else if ((strncmp(argv[i], "/MASTER", 7) == 0) && !master_server)
        {
            master_server = true;
    		server_name = "Master Server";
		    strcpy(mutex_name, MASTER_INSTANCE_MUTEX_NAME);
		    sprintf(g_LogFilename, "%sNet7_server", SERVER_LOGS_PATH);
		    LogMessage("Net7 Master Server (Auth:%d, Global:%d, Master:%d)\n",
                SSL_PORT, GLOBAL_SERVER_PORT, MASTER_SERVER_PORT);
        }
        else if ((strncmp(argv[i], "/PORT:", 6) == 0) && !sector_server)
        {
            sector_server = true;
		    port = atoi(argv[i] + 6);
		    sprintf(mutex_name, SECTOR_INSTANCE_MUTEX_NAME, port);
		    server_name = "Sector Server";
		    sprintf(g_LogFilename, "%ssector_server_port_%ld", SERVER_LOGS_PATH, port);
		    LogMessage("Net7 Sector Server (Port %d)\n", port);
        }
        else if ((strncmp(argv[i], "/MAX_SECTORS:", 13) == 0) && sector_server)
        {
		    max_sectors_str = argv[i] + 13;
            g_Max_Space_Sector = 4595;
        }
        else if (strncmp(argv[i], "/ALTSECTORS",11) == 0)
        {
            g_Sector_Start = 1910;
            g_Max_Space_Sector = 4595;
        }
        else if (strncmp(argv[i], "/ALLSECTORS",11) == 0)
        {
            g_Sector_Start = 973;
            g_Max_Space_Sector = 4595;
			sprintf(max_sectors_str, "300");
            printf("ALL SECTORS flag\n");
        }
        else if (strncmp(argv[i], "/STARTSECTOR:",13) == 0)
        {
            g_Sector_Start = atoi(argv[i]+13);
            printf("Starting at Sector %ld\n", g_Sector_Start);
        }
		else if (strncmp(argv[i], "/DEBUG", 6) == 0)
		{
			g_Debug = true;
            printf("DEBUG flag\n");
		}
        else
        {
            printf("Unrecognized switch: '%s'\n", argv[i]);
            Usage();
            return(1);
        }
    }

	printf("Domain set to: %s\n", g_DomainName);


#ifdef WIN32
    // Winsock startup
    WSADATA	wsaData = {NULL};
	WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

	if (strlen(domain)>0)
		strcpy(g_DomainName, domain);

	struct hostent * host = gethostbyname(g_DomainName);
	if (!host)
	{
        int err = WSAGetLastError();
        printf("Unable to resolve IP address for %s (error=%d)\n", g_DomainName, err);
        return(1);
    }
    unsigned char *ip = (unsigned char *) host->h_addr;
    sprintf(address, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);

    if (!master_server && !sector_server)
    {
		sprintf(g_LogFilename, "%sNet7_server", SERVER_LOGS_PATH);
		LogMessage("Net7 Standalone Server (Auth:%d, Global:%d, Master:%d \n\tMaxSectors: %s Version: %d.%d-%s%s Build %d)\n",
            SSL_PORT, GLOBAL_SERVER_PORT, MASTER_SERVER_PORT, max_sectors_str, UPPER_VER, LOWER_VER, VER_TYPE, SQL_ACCOUNT_STRING,  BUILD_VER);
        standalone = true;
        LogMessage("Net7 IP addr = %s\n", address);
    }

    if (master_server && sector_server)
    {
        printf("Can't combine /MASTER and /PORT switches\n");
		Usage();
		return(1);
	}

	unsigned long ip_address_internal = inet_addr(g_InternalIP);
	unsigned long ip_address = inet_addr(address);

    long max_sectors = atoi(max_sectors_str);

    if ((port < 3500) || (port > 32767))
    {
        printf("Invalid /PORT specified for Sector Server\n");
		return(1);
    }

    if ((max_sectors < 1) || (max_sectors > 300))
    {
        printf("Invalid /MAX_SECTORS specified for Sector Server\n");
		return(1);
    }

#ifdef WIN32
    // First, make sure we only have one instance of the Global Server running
    HANDLE instance_mutex = ::CreateMutex(NULL, TRUE, mutex_name);
    if (instance_mutex == INVALID_HANDLE_VALUE)
	{
		::MessageBox(NULL, "Error creating instance mutex", "Net7", MB_ICONERROR);
		return(1);
	}

    // if we did not create this mutex then .. another instance
    // is already running
    if (::GetLastError() == ERROR_ALREADY_EXISTS)
    {
        // close the mutex
        ::CloseHandle(instance_mutex);
		::MessageBox(NULL, "Another instance of the Net-7 Server is already running", "Net7", MB_ICONERROR);
		return(1);
    }
#endif

    // Delete the previous log file and start a new one
	//DeleteFile(g_LogFilename);
    ServerManager server_mgr(master_server, ip_address, (short) port, (short) max_sectors, standalone, ip_address_internal);
    server_mgr.SetPlayerMgrGlobalMemoryHandler();

	//MVAS Login UDP connection - needs to be done after global memory manager setup.
	UDP_Connection MVASauth(MVAS_LOGIN_PORT, &server_mgr, CONNECTION_TYPE_MVAS_TO_PROXY);
    server_mgr.SetUDPConnection(&MVASauth);
    MVASauth.SetServerManager(&server_mgr);

	server_mgr.RunServer();

#ifdef WIN32
    // Winsock cleanup
    WSACleanup();
#endif

#ifdef WIN32
	::CloseHandle(instance_mutex);
#endif

    return 0;
}
#ifdef WIN32
unsigned long GetNet7TickCount()
{
    return (GetTickCount() - g_StartTick);
}

// Functions added for Linux port
#else
unsigned long GetNet7TickCount()
{
    timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

unsigned long GetCurrentDirectory(unsigned long size, char *path)
{
    if (getcwd(path, size) < 0)
    {
        return 0;
    }
    return (strlen(path));
}

int SetCurrentDirectory(const char *path)
{
    if (chdir(path) < 0)
    {
        return 0;
    }
    return 1;
}

void Sleep(unsigned long dwMilliseconds)
{
    usleep((unsigned int) dwMilliseconds * 1000);
}

bool DeleteFile(const char *file)
{
    return (!remove(file));
}

#endif
