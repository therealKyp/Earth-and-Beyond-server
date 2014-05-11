// Net7.cpp
#define _WIN32_WINNT _WIN32_WINNT_WINXP
#include <process.h>
#include "Net7.h"
#include "ServerManager.h"
#include "UDPConnection.h"
#include "UDPClient.h"

#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "ssleay32.lib")
#pragma comment(lib, "libeay32.lib")

char g_LogFilename[MAX_PATH];
char g_InternalIP[MAX_PATH];
char g_DomainName[MAX_PATH];
char *g_server_addr = (0);
char *default_addr = "127.0.0.1";
char *g_internal_addr = (0);

bool g_Debug = false;
bool g_ServerShutdown = false; // Terminated the global Server

char *g_exe;
char *g_cmd;

ServerManager * g_ServerMgr = 0;
GMemoryHandler * g_GlobMemMgr = 0;
AccountManager * g_AccountMgr = 0;

bool g_LoggedIn = false;
unsigned long g_StartTick;
bool g_LocalCert = false;
unsigned short ssl_port = SSL_PORT;

void Usage()
{
	printf("Net7Proxy Usage:\n\n");
	printf("Starts E&B client to interface with server:\n");
	printf("   Net7Proxy /ADDRESS:(ip address)\n");
}

bool StartENBClient()
{
	STARTUPINFO si = {NULL};
	PROCESS_INFORMATION pi = {NULL};
	int success;
	char cur_dir[MAX_PATH+2];
    char start_dir[MAX_PATH+2];
	_splitpath(g_exe, cur_dir, &cur_dir[2], NULL, NULL);
	cur_dir[1] = ':';

	if (!engine_open_process("Earth & Beyond"))
	{
        GetCurrentDirectory(MAX_PATH+2, start_dir);

		SetCurrentDirectory(cur_dir);
		
		success = CreateProcess(g_exe, g_cmd, NULL, NULL, TRUE, 
			CREATE_DEFAULT_ERROR_MODE, NULL, NULL, &si, &pi);
		   
		LogMessage("Starting E&B...\n");
        SetCurrentDirectory(start_dir);
	}
   
	if (GetProcessHandle())
	{
		LogMessage("Launch E&B successful\n");
	}
	else
	{
		fprintf(stderr,"\nAppears to be an initialisation problem starting E&B\nTerminate E&B and restart Launcher\n");
		return (false);
	}

	return (true);
}

int main(int argc, char* argv[])
{
    long port = SECTOR_SERVER_PORT;
    char *domain = "";
	char cmd_buffer[MAX_PATH];
	g_cmd = &cmd_buffer[0];
	char exe_buffer[MAX_PATH];
	g_exe = &exe_buffer[0];
	char exeL_buffer[MAX_PATH];
	char *exeL = &exeL_buffer[0];
	bool UseDetours = false;
    bool local = false;

	g_StartTick = GetTickCount();
    g_internal_addr = default_addr;

    printf("Net7Proxy version %s\n", VERSION);

    GetCurrentDirectory(sizeof(exeL_buffer),exeL_buffer);
  	strcat(exeL, "\\Detours.exe");
	strcpy(g_exe, "C:\\Program Files\\EA GAMES\\Earth & Beyond\\release\\client.exe");

	srand((unsigned)GetTickCount());

    // No arguments indicate a standalone server via localhost
    for (int i = 1; i < argc; i++)
    {
	    if ((strncmp(argv[i], "/ADDRESS:", 9) == 0))
	    {
            g_server_addr = argv[i] + 9;
            if (strcmp(g_server_addr, "127.0.0.1") == 0)
            {
                g_server_addr = 0;
            }
        }
		else if (strncmp(argv[i], "/LC", 3) == 0)
		{
			g_LocalCert = true;
		}
		else if (strncmp(argv[i], "/L", 2) == 0)
		{
			UseDetours = true;
		}
		else if (strncmp(argv[i], "/CLIENT:", 8) == 0)
		{
			g_exe = argv[i] + 8;
		}
		else if (strncmp(argv[i], "/SSL:", 5) == 0)
		{
			long ssl_p = atoi(argv[i] + 5);
			if (ssl_p > 0)
			{
				ssl_port = (unsigned short)ssl_p;
			}
		}
        else
        {
            printf("Unrecognized switch: '%s'\n", argv[i]);
            Usage();
            return(1);
        }
    }

    if (g_server_addr == 0)
    {
        g_server_addr = default_addr;
        local = true;
    }

	if (UseDetours == true)
	{
		sprintf(g_cmd, "Detours.exe /ADDR:%s /CLIENT:\"%s\"",g_server_addr, g_exe);

		g_exe = exeL;
	}
	else
	{
		if (g_LocalCert)
		{
			sprintf(g_cmd, "client.exe -SERVER_ADDR %s -PROTOCOL TCP", default_addr);
		}
		else
		{
			sprintf(g_cmd, "client.exe -SERVER_ADDR %s -PROTOCOL TCP", g_server_addr);
		}
	}

    sprintf(g_DomainName, "local.net-7.org");

    // Winsock startup
    WSADATA	wsaData = {NULL};
	WSAStartup(MAKEWORD(2, 0), &wsaData);

	struct hostent * host = gethostbyname(g_DomainName);
	if (!host)
	{
        int err = WSAGetLastError();
        printf("Unable to resolve IP address for %s (error=%d)\n", g_DomainName, err);
        return(1);
    }
    unsigned char *ip = (unsigned char *) host->h_addr;

	unsigned long ip_address_internal = inet_addr(g_internal_addr);
	unsigned long net7_server_ip_address = inet_addr(g_server_addr);

    if (local)
    {
        char strLocal[MAX_PATH] = { 0 };
        if (SOCKET_ERROR != gethostname(strLocal, MAX_PATH))
        {
            struct hostent* hp;
            hp = gethostbyname(strLocal);
            if (hp != NULL)	
            {
                strcpy(strLocal, hp->h_name);
                net7_server_ip_address = *((ULONG *) hp->h_addr_list[0]);
            }
        }
    }

    //first establish contact with server

    //open UDP receive connection
    UDPClient UDP_connection(MVAS_LOGIN_PORT, CLIENT_TYPE_FIXED_PORT, net7_server_ip_address);

    LogMessage("Attempting to connect to IP address: %s\n", g_server_addr);

    if (UDP_connection.VerifyConnection())
    {
        if (!StartENBClient())
        {
			::MessageBox(NULL, "Unable to start E&B client", "Net7Proxy", MB_ICONERROR);
            return (1);
        }

        UDPClient UDP_sendport(GLOBAL_SERVER_PORT, CLIENT_TYPE_MULTI_PORT, net7_server_ip_address);
        //now start the TCP link to E&B
        ServerManager server_mgr(false, ip_address_internal, (short) port, (short) 1, true, ip_address_internal);

        //server_mgr.SetPlayerMgrGlobalMemoryHandler();  
        server_mgr.SetUDPConnections(&UDP_connection, &UDP_sendport);

        //now patch E&B
        PatchClient();
    
        server_mgr.RunServer();
    }
	else
	{
		::MessageBox(NULL, "Server Failed to respond to Login attempt", "Net7Proxy", MB_ICONERROR);
	}

    // Winsock cleanup
    WSACleanup();

    return 0;
}

static volatile HANDLE 	g_ProcessHandle 	= (HANDLE) INVALID_HANDLE_VALUE;
static volatile bool	g_EngineInUse		= FALSE;
long g_AddrStore = 0x00b6e5a8; //this virtual offset places us within the known .data area offset.

//=========================

bool engine_close_process() 
{
	if (g_EngineInUse) 
	{
		// are we in use?
		if (CloseHandle(g_ProcessHandle)) 
		{
			// yup, so close the process handle
			g_EngineInUse = FALSE;
			return TRUE;
		}
	}

	return FALSE;
}

//=========================

bool engine_open_process(char * processwindowtitle) 
{

	HWND 	TargetWindowHandle	= (HWND) -1;
	DWORD	Process_Id;
	LPDWORD PID;
	HANDLE 	WindowProcessId		= (HANDLE) INVALID_HANDLE_VALUE;

	if (g_EngineInUse) 
	{
		// we are already in use...
		return FALSE;
	}

	TargetWindowHandle = FindWindow(NULL, processwindowtitle); 	// see if it exists

	if (TargetWindowHandle) 
	{
		// got the window handle...
		Process_Id = GetWindowThreadProcessId(TargetWindowHandle, (LPDWORD)&PID); //get a PROCESS number

		if (Process_Id) 
		{
			// we have a valid process id, now to open it...
			g_ProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (DWORD)PID);

			if (g_ProcessHandle) 
			{
				// process succesfully opened
				g_EngineInUse = TRUE;
				return TRUE;
			}
			else
			{
				 long error_code = GetLastError();
				 LogMessage("Error code: %d [%x]\n", error_code, error_code);
			}


		}
	}

	return FALSE;
}

//=========================

bool engine_check_process(char * processwindowtitle)
{
	HWND 	TargetWindowHandle	= (HWND) -1;
	DWORD	Process_Id;
	LPDWORD PID;
	HANDLE 	WindowProcessId		= (HANDLE) INVALID_HANDLE_VALUE;

	TargetWindowHandle = FindWindow(NULL, processwindowtitle); 	// see if it exists

	if (TargetWindowHandle) 
	{
		// got the window handle...
		Process_Id = GetWindowThreadProcessId(TargetWindowHandle, (LPDWORD)&PID); //get a PROCESS number

		if (Process_Id) 
		{
			return TRUE;
		}
	}
	return FALSE;
}

bool engine_write_process(LPVOID lpBaseAddress, LPVOID lpBuffer, DWORD nSize) 
{
	bool SuccessCode = FALSE;

	if (g_EngineInUse) 
	{
		// are we in use
		if (g_ProcessHandle) 
		{
			// do we have a process handle

			DWORD BytesWritten = 0;
			bool  ProcessSuspended = FALSE;

			if (SuspendThread(g_ProcessHandle) != (DWORD) -1) 
			{
				// suspend the thread - its safer
				ProcessSuspended = TRUE;
			}

			if (WriteProcessMemory(g_ProcessHandle, lpBaseAddress, lpBuffer, nSize, &BytesWritten) && BytesWritten == nSize) 
			{
				// write was successful
				// flush the instruction cache (for safety)
				FlushInstructionCache(g_ProcessHandle, lpBaseAddress, nSize);
				SuccessCode = TRUE;
			}
			
			// resume the process if we suspended it
			if (ProcessSuspended) 
			{
				ResumeThread(g_ProcessHandle);
			}

		}
	}
	else
	{
		LogMessage("unable to write to process\n");
	}

	return SuccessCode;
}

//=========================

bool engine_read_process(LPVOID lpBaseAddress, LPVOID lpBuffer, DWORD nSize) 
{
	bool SuccessCode = FALSE;

	if (g_EngineInUse) 
    {
		if (g_ProcessHandle) 
        {
			DWORD BytesRead = 0;
			bool  ProcessSuspended = FALSE;
			DWORD BytesWritten;

			if (SuspendThread(g_ProcessHandle) != (DWORD) -1) 
            {
				ProcessSuspended = TRUE;
			}

			if (ReadProcessMemory(g_ProcessHandle, lpBaseAddress, lpBuffer, nSize, &BytesWritten) && BytesRead == nSize) 
            {
				SuccessCode = TRUE;
			}

			if (ProcessSuspended) 
            {
				ResumeThread(g_ProcessHandle);
			}

		}
	}
	else
	{
		LogMessage("unable to read process\n");
	}

	return SuccessCode;
}

//=========================

bool ShutdownClient() 
{
	if (g_EngineInUse) 
    {
		if (TerminateProcess(g_ProcessHandle, (UINT) 0x0D1ED1E)) 
        {
			CloseHandle(g_ProcessHandle);
			g_EngineInUse = FALSE;
			return TRUE;
		}
	}
	
	return FALSE;
}
	
//=========================

void PatchClient()
{
	DWORD zero = 0;
	DWORD addr_off;

	unsigned char inject_buffer1[] =
	{
		0x8B, 0x0D, 0x58, 0x06, 0x21, 0x01, //this assembly code checks to see if there's already an address for
		0x85, 0xC9, //6                     //the player coords, if not then it checks to make sure this is a valid
		0x75, 0x2E,                         //player hull type, if so then it assumes that it will be the player's
		0x80, 0x38, 0x53,                   //hull (since player's hull is always the first location sent).
		0x75, 0x29, //13                    //Then it stores the write location of the player hull coords in a
		0x80, 0x78, 0x02, 0x48,             //fixed position (which is g_AddrStore).
		0x75, 0x23, //19
		0x8B, 0x48, 0x14,
		0xBB, 0x58, 0x06, 0x21, 0x01, //this is the addr to write ptr to (25)
		0x89, 0x0B,
		0x33, 0xDB,
		0xEB, 0x15,
		0x01,  
		0x90,
		0x68, 0x28, 0x3C, 0xB7, 0x00,
		0x50,
		0x51,
		0xE8, 0x6A, 0xE2, 0x19, 0x00,
		0x83, 0xC4, 0x10,
		0xC2, 0x08, 0x00,
		0x90,
		0x8B, 0x48, 0x14,
		0x8D, 0x44, 0x19, 0x48,
		0xE9, 0x30, 0x6E, 0x10, 0x00,	
		0x00, 0x00, 0x00, 0x00
	};

	*((long*)&inject_buffer1[25]) = g_AddrStore;
	*((long*)&inject_buffer1[2]) = g_AddrStore;

	unsigned char inject_buffer2[] =
	{
		0xE9, 0x8E, 0x91, 0xEF, 0xFF,  //jump to 888278
		0x90,
		0x90
	};

	unsigned char inject_buffer3[] =
	{
		0x8B, 0xD5						//set client's perceived time difference between sendtime and receive to 0
	};									//this makes movement appear very smooth

	unsigned char inject_buffer4[] =
	{
		0x55,
		0x33, 0xED,
		0x89, 0x2D, 0xA8, 0xE5, 0xB6, 0x00, //this resets the client coords position each time we change sectors
		0x5D,
		0xC2, 0x04, 0x00,
		0x90,
		0x90,
		0x90,
		0x90
	};

	*((long*)&inject_buffer4[5]) = g_AddrStore;

    unsigned char inject_buffer5[] =
    {
        0x56,                                   //PUSH ESI
        0x8B, 0x77, 0x0C,                       //MOV ESI,DWORD PTR DS:[EDI+C]   //copy gameID to ESI 
        0x89, 0x35, 0xAC, 0xE5, 0xB6, 0x00,     //MOV DWORD PTR DS:[B6E5AC],ESI  //store gameID into 0x00b6e5ac (gameID flag)
        0x5E,                                   //POP ESI
        0xC2, 0x04, 0x00                        //RETN 4
    };

	addr_off = g_AddrStore; //this addr is a fix temp store 
	engine_write_process((void*)addr_off, (void*)&zero, 16); //make room for addr for position, orientation client gameID and also internal orientation update flag
	addr_off = 0x00888278; //this is the addr of the padding around the GPS request code
	engine_write_process((void*)addr_off, (void*)inject_buffer1, sizeof(inject_buffer1));
	addr_off = 0x0098F0E5; //this is the jump hijack for the object coords transform loop
	engine_write_process((void*)addr_off, (void*)inject_buffer2, sizeof(inject_buffer2));
	addr_off = 0x007379E2; //this is the addr of the code to calculate the lag for movement
	engine_write_process((void*)addr_off, (void*)inject_buffer3, sizeof(inject_buffer3));
	addr_off = 0x00767009; //this is the addr of the code at the end of the redirect packet code
	engine_write_process((void*)addr_off, (void*)inject_buffer4, sizeof(inject_buffer4));
    addr_off = 0x00733620; //store client GameID to read buffer
    engine_write_process((void*)addr_off, (void*)inject_buffer5, sizeof(inject_buffer5));
}

unsigned long GetNet7TickCount()
{
    return (GetTickCount() - g_StartTick);
}

bool GetProcessHandle()
{
	long count = 0;

	while (count < 50)
	{
		if (engine_open_process("Earth & Beyond"))
		{
			break;
		}
		count++;
		Sleep(1000);
	}

	if (g_ProcessHandle == INVALID_HANDLE_VALUE)
	{
		LogMessage("Failed to open E&B\n");
		return false;
	}
	else
	{
		return true;
	}
}

void WaitForEngineReady()
{
	long counter = 0;
	while (!g_EngineInUse && counter < 300 && !g_ServerShutdown)
	{
		Sleep(250);
		counter++;
	}
}

void WaitForLogin()
{
	long counter = 0;
	while (!g_LoggedIn && counter < 300 && !g_ServerShutdown)
	{
		Sleep(250);
		counter++;
	}
}

bool ClientStillRunning()
{
	return (engine_check_process("Earth & Beyond"));
}
