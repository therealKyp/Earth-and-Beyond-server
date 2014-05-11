// Net7.cpp

#include "Net7.h"
#include "ConnectionManager.h"
#include "LogManager.h"
#include "ServerManager.h"

ConnectionManager g_ConnectionManager;
LogManager g_LogManager;
Log g_Log;
Log g_LogDebug;
Log g_LogError;

int main(int argc, char *argv[])
{
    int port = DEFAULT_SECTOR_SERVER_PORT;
    int max_sectors = DEFAULT_MAX_SECTORS;
    char *address = "127.0.0.1";
    unsigned int ip_address;
    unsigned int server_type = 0;
    
    g_Log.Print("Net-7 is starting...");

    if(argc == 1)
    {
        //sprintf(g_LogFile, "%sNet7_server.log", SERVER_LOGS_PATH);
        server_type |= SERVER_TYPE_STANDALONE;
    }
    else
    {
        for (int i = 1; i < argc; i++)
        {
	        if ((strncmp(argv[i], "/ADDRESS:", 9) == 0))
	        {
                address = argv[i] + 9;
            }
            else if ((strcmp(argv[i], "/MASTER") == 0) && !(server_type & SERVER_TYPE_MASTER))
            {
                server_type |= SERVER_TYPE_MASTER;
    		    //server_name = "Master Server";
		        //strcpy(mutex_name, MASTER_INSTANCE_MUTEX_NAME);
		        //sprintf(g_LogFile, "%sNet7_server.log", SERVER_LOGS_PATH);
		        //LogMessage("Net7 Master Server (Auth:%d, Global:%d, Master:%d)\n",
                    //SSL_PORT, GLOBAL_SERVER_PORT, MASTER_SERVER_PORT);
            }
            else if ((strncmp(argv[i], "/PORT:", 6) == 0) && !(server_type & SERVER_TYPE_SECTOR))
            {
                server_type |= SERVER_TYPE_SECTOR;
		        port = atoi(argv[i] + 6);
		        //sprintf(mutex_name, SECTOR_INSTANCE_MUTEX_NAME, port);
		        //server_name = "Sector Server";
		        //sprintf(g_LogFile, "%ssector_server_%d.log", SERVER_LOGS_PATH, port);
		        //LogMessage("Net7 Sector Server (Port %d)\n", port);
            }
            else if ((strncmp(argv[i], "/MAX_SECTORS:", 13) == 0) && (server_type & SERVER_TYPE_SECTOR))
            {
		        max_sectors = atoi(argv[i] + 13);
            }
            else
            {
	            printf("Net7 Usage:\n\n");
	            printf("to run the main server:\n");
	            printf("   Net7 /MASTER /ADDRESS:(ip address)\n\n");
	            printf("to run a sector server:\n");
	            printf("   Net7 /PORT:3500 /ADDRESS:(ip address) /MAX_SECTORS:(num sectors)\n\n");
                return 1;
            }
        }
    }
    
    ip_address = inet_addr(address);
    
    if ((port < 3500) || (port > 32767))
    {
#ifdef WIN32
		::MessageBox(NULL, "Invalid /PORT specified for Sector Server.", "Net7", MB_ICONERROR);
#else
        printf("Invalid /PORT specified for Sector Server.\n");
#endif
		return 1;
    }
    
    if ((max_sectors < 1) || (max_sectors > 128))
    {
#ifdef WIN32
		::MessageBox(NULL, "Invalid /MAX_SECTORS specified for Sector Server.", "Net7", MB_ICONERROR);
#else
        printf("Invalid /MAX_SECTORS specified for Sector Server.\n");
#endif
		return 1;
    }

#ifdef WIN32
    // First, make sure we only have one instance of the Global Server running
    HANDLE instance_mutex = ::CreateMutex(NULL, TRUE, mutex_name);
    if (instance_mutex == INVALID_HANDLE_VALUE)
	{
		::MessageBox(NULL, "Error creating instance mutex.", "Net7", MB_ICONERROR);
		return 1;
	}

    // if we did not create this mutex then .. another instance
    // is already running
    if (::GetLastError() == ERROR_ALREADY_EXISTS)
    {
        // close the mutex
        ::CloseHandle(instance_mutex);
		::MessageBox(NULL, "Another instance of the Net-7 Server is already running.", "Net7", MB_ICONERROR);
		return 1;
    }
#endif

    {
        char logFile[18];
        time_t currentTime;
        struct tm *timeData;
        
        time(&currentTime);
        timeData = localtime(&currentTime);
        
        snprintf(logFile, 18, "Net7_%.4d%.2d%.2d.log", timeData->tm_year + 1900, timeData->tm_mon + 1, timeData->tm_mday);
        
        g_Log.SetFileName(logFile);
        g_LogDebug.SetFileName("Net7_debug.log");
        g_LogError.SetFileName("Net7_error.log");
    }
    
    g_LogManager.Check();
    
    ServerManager server_manager(server_type, ip_address, max_sectors);
    server_manager.Run();

#ifdef WIN32
	::CloseHandle(instance_mutex);
#endif

    return 0;
}
