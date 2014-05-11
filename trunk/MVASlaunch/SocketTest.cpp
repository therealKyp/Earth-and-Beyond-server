// SOCKETTEST.CPP
//
// This acts as a movement send client to the server.
// It works by first injecting some code into the client to store the pointer to where the xyz position of the player ship is calculated
// in a pre-determined and fixed place (I used part of the string c:\program files\ea games\earth and beyond ... in a data area).
// luckily for us, the first thing through the transform engine is the player ship, and it seems to not use the calculation area for anything else (fingers crossed!)
// The socket client logs into a sector server and the server then handles it to ensure it's always matched up to a particular client (via the IP addr).
// The server polls the sockclient for an XYZ, the sockclient reads the xyz from the E&B client's memory and then transmits it back,
//
// This is kind of a gigantic hack, but it gives us lag-free movement given any kind of connection, and it seems to work very well too!
// Needs further testing over the internet though.
// 
// The system has been re-jigged to use one UDP port now, which is extremely efficient and protects against connection resets
// This will also give us a leg up for if we ever wanted to convert the whole client/server connection system to UDP.

#include <stdio.h>
#include <process.h>
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <winsock2.h>
#include <objbase.h>


#include <tlhelp32.h>
#include <time.h>

int gListenPort = 3806;
int gSendPort = 3806;
long gPlayerIndex = -1;
DWORD g_addr_off = 0;
long g_AddrStore = 0x00b6e5a8; //this virtual offset places us within the known .data area offset.
bool g_client_running = true;
bool g_logged_in = false;
volatile int g_thread_speed = 10;
bool g_global_fail = false;
unsigned char g_last_position[32];


#define RC4_KEY_SIZE        8
#define RC4_UDP_KEY_SIZE    16
#define TCP_BUFFER_SIZE     (128 * 1024)
#define	MAX_BUFFER					128

#define ENB_OPCODE_1000_MVAS_REGISTER_C_S				0x1000
#define ENB_OPCODE_1001_MVAS_LOGIN_S_C					0x1001
#define ENB_OPCODE_1002_MVAS_RECONNECT_C_S				0x1002
#define ENB_OPCODE_1003_MVAS_POLL_POSITION_S_C			0x1003
#define ENB_OPCODE_1004_MVAS_SEND_POSITION_C_S			0x1004
#define	ENB_OPCODE_1005_MVAS_SEND_POSITION_S_C			0x1005
#define ENB_OPCODE_1006_MVAS_RESET_POSITION_S_C			0x1006
#define ENB_OPCODE_1007_MVAS_TOGGLE_SEND_FREQ_S_C		0x1007
#define ENB_OPCODE_1008_MVAS_LOGOFF_C_S                 0x1008
#define ENB_OPCODE_1009_MVAS_BAD_LOGIN_S_C              0x1009


#define GLOBAL_LOGIN 1
#define SECTOR_LOGIN 2

#define VERSION "1.01"
#define VERSION_N 101


#define ATTRIB_PACKED

void SetRC4Key(unsigned char *rc4_key);
bool DoKeyExchange();
bool DoClientKeyExchange();
void SendResponse(short opcode, unsigned char *data, size_t length);
bool ConnectedToServer();
bool ConnectToSectorServer(long login_type);
bool HandleHandoff();
bool HandleSendPosition();
void LogMessage(char *format, ...);
bool ClientStillRunning();
void RecvThread(void *arg);
void SendThread(void *arg);
void ResetCoordsPtr();

//char *SERVER_IP = "85.228.238.90"; //Pritcher's server
char *SERVER_IP = "127.0.0.1"; //local loopback

unsigned char       m_SendBuffer[MAX_BUFFER]; 
unsigned char       m_RecvBuffer[MAX_BUFFER]; 

struct EnbTcpHeader
{
    short   size;
    short   opcode;
} ATTRIB_PACKED;

struct MVASHandoff
{
	long	player_id;
	long	port;
} ATTRIB_PACKED;

//SOCKET m_Send_Socket;
SOCKET m_Listen_Socket;

static volatile HANDLE 	g_ProcessHandle 	= (HANDLE) INVALID_HANDLE_VALUE;
static volatile bool	g_EngineInUse		= FALSE;

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
			g_ProcessHandle = OpenProcess(PROCESS_ALL_ACCESS,NULL, (DWORD)PID);

			if (g_ProcessHandle) 
			{
				// process succesfully opened
				g_EngineInUse = TRUE;
				return TRUE;
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

	return SuccessCode;
}

//=========================

bool engine_kill_process_and_close() 
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

// see if we've got a live position
bool CheckPosition()
{
	DWORD addr_off = g_AddrStore;
	DWORD proc_read[2];
    proc_read[0] = 0;
    proc_read[1] = 0;
    bool ready_to_send = false;

	engine_read_process((void*)addr_off, (void*)proc_read, 8);

	if (proc_read[0] != 0)
	{
		if ((proc_read[0]+0x48) != g_addr_off)
		{
			g_addr_off = proc_read[0] + 0x48;
			//fprintf(stderr,"\nFound pos at: 0x%08x",g_addr_off);
            fprintf(stderr,"\nSector Change Detected.");
		}
		ready_to_send = true;
	}

    if (proc_read[1] != (unsigned long)gPlayerIndex && (proc_read[1] >= 0x40000000))
    {
        gPlayerIndex = proc_read[1];
        //fprintf(stderr,"\nPlayer Index = [0x%x]", gPlayerIndex);
    }

    if ((ready_to_send == true) && (gPlayerIndex >= 0x40000000 && gPlayerIndex <= 0x40F00000) )
    {
        return true;
    }
    else
    {
        return false;
    }
}

void SendPositionIfChanged()
{
	unsigned char pos_data[32];
	unsigned char buffer[64];
	float x, y, z;
    static short stationary_send_tick = 3;

	//see if we have a live address for position first
	if (g_addr_off == 0)
	{
		return;
	}

    //read position
    engine_read_process((void*)g_addr_off, (void*)buffer, 48);
    
    *((float *) &pos_data[0]) = *((float *) &buffer[12]);
    *((float *) &pos_data[4]) = *((float *) &buffer[28]);
    *((float *) &pos_data[8]) = *((float *) &buffer[44]);
    *((long *) &pos_data[12]) = gPlayerIndex;
    *((float *) &pos_data[16]) = *((float *) &buffer[0]);
    *((float *) &pos_data[20]) = *((float *) &buffer[16]);
    *((float *) &pos_data[24]) = *((float *) &buffer[32]);

	x = *((float *) &pos_data[0]);
	y = *((float *) &pos_data[4]);
	z = *((float *) &pos_data[8]);

	if (x == 0 && y == 0 && z == 0)
	{
		return;
	}

    if (memcmp(&pos_data, &g_last_position[16], 12) != 0) //has orientation changed?
    {
		SendResponse(ENB_OPCODE_1004_MVAS_SEND_POSITION_C_S, pos_data, 28); //send position and orientation change
		memcpy(&g_last_position, &pos_data, 28);
        stationary_send_tick = 2;
    }
    else if (memcmp(&pos_data, &g_last_position, 12) != 0) //has position changed?
	{
		SendResponse(ENB_OPCODE_1004_MVAS_SEND_POSITION_C_S, pos_data, 16); //only send position change
		memcpy(&g_last_position, &pos_data, 28);
        stationary_send_tick = 2;
	}
    else if (stationary_send_tick > 0)
    {
        stationary_send_tick--;
        SendResponse(ENB_OPCODE_1004_MVAS_SEND_POSITION_C_S, pos_data, 28);
    }
}

bool GetProcessHandle()
{
	long count = 0;

	while (count < 20)
	{
		if (engine_open_process("Earth & Beyond"))
		{
			break;
		}
		count++;
		_sleep(1000);
	}

	if (g_ProcessHandle == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool ClientStillRunning()
{
	return (engine_check_process("Earth & Beyond"));
}

void StartRecvThread()
{
	_beginthread(&RecvThread, 0, NULL);
}

void StartSendThread()
{
	_beginthread(&SendThread, 0, NULL);
}

bool HandleLogoff()
{
	MVASHandoff handoff;
	bool retval = true;

	handoff.player_id = *((long *) &m_RecvBuffer[4]);
	handoff.port = *((long *) &m_RecvBuffer[8]);

	gPlayerIndex = (handoff.player_id);

	fprintf(stderr,"\nPlayer Index = [0x%x]", gPlayerIndex);

	if (gPlayerIndex == 0)
	{
		retval = false;
	}

	return retval;
}

bool BindPort(short port, SOCKET socket)  
{
	// Bind the socket to its port
	sockaddr_in localAddr;
	memset(&localAddr, 0, sizeof(localAddr));
	localAddr.sin_family = AF_INET;
	localAddr.sin_addr.s_addr = htonl(INADDR_ANY);//inet_addr(SERVER_IP);//htonl(INADDR_ANY);
	localAddr.sin_port = htons(port);
	
	if (bind(socket, (sockaddr *) &localAddr, sizeof(sockaddr_in)) < 0) 
	{
		fprintf(stderr,"\nSet of local port failed (bind())");
		return false;
	}
	return true;
}

void SetBroadcast(SOCKET socket) 
{
	// If this fails, we'll hear about it when we try to send.  This will allow 
	// system that cannot broadcast to continue if they don't plan to broadcast
	int broadcastPermission = 1;
	setsockopt(socket, SOL_SOCKET, SO_BROADCAST, 
		(char *) &broadcastPermission, sizeof(broadcastPermission));
}

int UDP_RecvFromServer(char *buffer, int size, long &sourceAddress, unsigned short &sourcePort)
{
	int rtn;

	if ((rtn = recv(m_Listen_Socket, buffer, size, 0)) < 0) 
	{
		//fprintf(stderr,"Received failed (recv())");
	}
	
	return rtn;
}

int UDP_Recv(char *buffer, int bufferLen)
{
	int rtn;
	if ((rtn = recv(m_Listen_Socket, buffer, bufferLen, 0)) < 0) 
	{
		//fprintf(stderr,"Received failed.\n");
	}
	return (rtn);
}

int UDP_SendToServer(char *buffer, long size) 
{
	SOCKADDR_IN lSockAddr;
	memset(&lSockAddr,0, sizeof(lSockAddr));
	lSockAddr.sin_family = AF_INET;
	lSockAddr.sin_port = htons(gSendPort);
	lSockAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

	if (sendto(m_Listen_Socket, buffer, size, 0,
             (sockaddr *) &lSockAddr, sizeof(lSockAddr)) != size)
	{
		fprintf(stderr,"\nSend failed (sendto())");
	}

	return (size);
}

void UDP_Send(const char *buffer, int bufferLen) 
{
	int buffsend;
	SOCKADDR_IN lSockAddr;
	memset(&lSockAddr,0, sizeof(lSockAddr));
	lSockAddr.sin_family = AF_INET;
	lSockAddr.sin_port = htons(gSendPort);
	lSockAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

	// Write out the whole buffer as a single message.
	if (buffsend = sendto(m_Listen_Socket, buffer, bufferLen, 0,
		(sockaddr *) &lSockAddr, sizeof(lSockAddr)) != bufferLen) 
	{
		fprintf(stderr,"\nSend failed %d %d", bufferLen, buffsend);
	}
}

//this thread spins and sends position updates periodically to the server
void SendThread(void *arg)
{
	//float last_coords[3];
	memset(&g_last_position, 0, sizeof(g_last_position));

	while (!g_logged_in)
	{
		_sleep(1000);
	}

	while (g_client_running)
	{
		//see if we have an addr for coords yet
		if (CheckPosition())
		{ //OK, we've got a live position
			SendPositionIfChanged();
		}
		_sleep(g_thread_speed*500);
	}
}

void ToggleSendFrequency()
{
	g_thread_speed = *((long *) &m_RecvBuffer[4]);
	if (g_thread_speed < 1)
	{
		fprintf(stderr,"Warning: Bad frequency received: %d\n", g_thread_speed);
		g_thread_speed = 1;
	}
}

void SignalWrongVersion()
{
    float version = *((float *) &m_RecvBuffer[4]);
    version = version *0.01f;
    /*char msg[256];

	sprintf(msg, "Incompatible version of Sockettest.\nLatest version is %.2f", version);
    MessageBox(msg, "Sockettest", MB_ICONERROR);*/

    fprintf(stderr,"\nIncompatible version of sockettest.");
    fprintf(stderr,"\nYou need to upgrade to version %.2f",version);
    fprintf(stderr,"\nCheck www.enb-emulator.com for latest version.");
}

void RecvThread(void *arg)
{
	long source_addr;
	unsigned short sourcePort;
	int received;
	EnbTcpHeader *header;
    memset(&header, 0, sizeof(header));
	bool fail = false;

	while (g_client_running)
    {	
		received = UDP_RecvFromServer((char*)m_RecvBuffer, MAX_BUFFER, source_addr, sourcePort);

		if (received > 0)
		{
			header = (EnbTcpHeader*)m_RecvBuffer;
			
			unsigned short bytes = header->size - sizeof(EnbTcpHeader);
			short opcode = header->opcode;
			
			if (received == (int)(bytes + sizeof(EnbTcpHeader)))
			{
				switch (opcode)
				{
				case ENB_OPCODE_1001_MVAS_LOGIN_S_C:
					g_logged_in = true;
					break;
										
				case ENB_OPCODE_1007_MVAS_TOGGLE_SEND_FREQ_S_C:
					ToggleSendFrequency();
					break;

                case ENB_OPCODE_1009_MVAS_BAD_LOGIN_S_C:
                    SignalWrongVersion();
                    break;
					
				default:
					fprintf(stderr,"bad opcode\n");
					break;
				}
			}
		}
	}

	closesocket(m_Listen_Socket);

	_endthread();
}

bool StartENBClient(char *exe, char *cmd)
{
	STARTUPINFO si = {NULL};
	PROCESS_INFORMATION pi = {NULL};
	int success;
	char cur_dir[MAX_PATH+2];
	_splitpath(exe, cur_dir, &cur_dir[2], NULL, NULL);
	cur_dir[1] = ':';

	if (!engine_open_process("Earth & Beyond"))
	{
		SetCurrentDirectory(cur_dir);
		
		success = CreateProcess(exe, cmd, NULL, NULL, TRUE, 
			CREATE_DEFAULT_ERROR_MODE, NULL, NULL, &si, &pi);
		   
		fprintf(stderr,"Starting E&B...\n");

		/*fprintf(stderr,"\nThis program will start the client for you ...\nExit out of the client and restart Launcher...");
		return (false);*/
	}
   
	if (GetProcessHandle())
	{
		fprintf(stderr,"Launch E&B successful\n");
	}
	else
	{
		fprintf(stderr,"\nAppears to be an initialisation problem starting E&B\nTerminate E&B and restart Launcher\n");
		return (false);
	}

	return (true);
}

int main(int argc, char** argv)
{
	char cmd_buffer[MAX_PATH];
	char *cmd = &cmd_buffer[0];
	char exe_buffer[MAX_PATH];
	char *exe = &exe_buffer[0];
	char exeL_buffer[MAX_PATH];
	char *exeL = &exeL_buffer[0];
	bool UseDetours = false;
    bool UseUDP = false;
	WSADATA wsaData;

	GetCurrentDirectory(sizeof(exeL_buffer),exeL_buffer);

	strcat(exeL, "\\Detours.exe");
	strcpy(exe, "C:\\Program Files\\EA GAMES\\Earth & Beyond\\release\\client.exe");

	fprintf(stderr,"SocketTest v%s\n",VERSION);

	for (int i = 1; i < argc; i++)
    {
	    if (strncmp(argv[i], "/ADDR:", 6) == 0)
	    {
            SERVER_IP = argv[i] + 6;
        }
		else if (strncmp(argv[i], "/L", 2) == 0)
		{
			UseDetours = true;
		}
		else if (strncmp(argv[i], "/UDP", 4) == 0)
		{
			UseUDP = true;
            printf("Client using UDP\n");
		}
		else if (strncmp(argv[i], "/CLIENT:", 8) == 0)
		{
			exe = argv[i] + 8;
		}
    }

	if (UseDetours == true)
	{
		sprintf(cmd, "Detours.exe /ADDR:%s /CLIENT:\"%s\"",SERVER_IP, exe);

        if (UseUDP)
		{
			strcat(cmd, " /UDP");
		}

		exe = exeL;
	}
	else
	{
        if (UseUDP)
        {
		    sprintf(cmd, "client.exe -SERVER_ADDR %s -PROTOCOL UDP", SERVER_IP);
        }
        else
        {
		    sprintf(cmd, "client.exe -SERVER_ADDR %s -PROTOCOL TCP", SERVER_IP);
        }

		fprintf(stderr,"Attempting to connect to IP address: %s\n", SERVER_IP);
	}

	if(WSAStartup(MAKEWORD(2,0),&wsaData) != 0)
	{
		fprintf(stderr,"\nSocket Initialization Error. Program aborted");
		return false;
	}

	{
		m_Listen_Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		
		if(m_Listen_Socket == INVALID_SOCKET)
		{
			fprintf(stderr,"\nInvalid Socket %d. Program Aborted", GetLastError());
			return (1);
		}
		
		/*if (!BindPort(gListenPort, m_Listen_Socket))
		{
			fprintf(stderr,"\nUnable to bind server socket");
			return;
		}*/
		
		SetBroadcast(m_Listen_Socket);
	}

	if (!ConnectToSectorServer(GLOBAL_LOGIN))
	{
		fprintf(stderr,"\nCould not connect to Server. Check Server is running\n");
		_sleep(10000); //pause so user can read error
		return (1);
	}

	if (!StartENBClient(exe, cmd))
	{
		_sleep(10000);
		return (2);
	}

	PatchClient();

	while (ClientStillRunning()) //check client is still running every 5 seconds
	{
		_sleep(10000);
	}

	g_client_running = false;

    //send a 'shutdown' signal.
    SendResponse(ENB_OPCODE_1008_MVAS_LOGOFF_C_S, (unsigned char *) &gPlayerIndex, sizeof(gPlayerIndex));

	//shutdown
	fprintf(stderr,"\n\nENB no longer running. SocketTest exiting\n");
	_sleep(10000); //pause so user can read error

	//closesocket(m_Send_Socket);
	closesocket(m_Listen_Socket);

    return (0);
}

bool ConnectToSectorServer(long login_type)
{
	long count;

	long version = VERSION_N;

	//send a hello to the server, wait for a response
	count = 3;
	SendResponse(ENB_OPCODE_1000_MVAS_REGISTER_C_S, (unsigned char *) &version, sizeof(version));
	StartRecvThread();
	StartSendThread();

	_sleep(1000);

	while (count > 0 && g_logged_in == false && g_global_fail == false)
	{
		fprintf(stderr,"\nSending login attempt\n");
		SendResponse(ENB_OPCODE_1000_MVAS_REGISTER_C_S, (unsigned char *) &version, sizeof(version));
		_sleep(3000);
		count--;
	}

	if (g_logged_in)
	{
		fprintf(stderr,"\nlogin OK\n");
    }
	else
	{
		return false;
	}

	   
	/*if (!DoClientKeyExchange())
	{
		fprintf(stderr,"\nClient key exchange failed.  Connection thread exiting");
		return false;
	}
	else
	{
		fprintf(stderr,"\nkey exchange successful on port %d\n", gPort);
	}*/
	   
	return true;
}

void SendOp(unsigned char *buffer, long length)
{
	UDP_Send((char*) buffer, length);
	//send(m_Socket, (char *) buffer, length, 0);
}

void SendResponse(short opcode, unsigned char *data, size_t length)
{
	EnbTcpHeader * header = (EnbTcpHeader *) m_SendBuffer;
	header->size = (short) length + sizeof(EnbTcpHeader);
	header->opcode = opcode;
	if (length)
	{
		memcpy(m_SendBuffer + sizeof(EnbTcpHeader), data, length);
	}

	int bytes = length + sizeof(EnbTcpHeader);

    //m_CryptOut.RC4(m_SendBuffer, bytes);

    SendOp(&m_SendBuffer[0], bytes);
}

#if 0

void SetRC4Key(unsigned char *rc4_key)
{
    m_CryptOut.PrepareKey(rc4_key, RC4_KEY_SIZE);
	m_CryptIn.PrepareKey(rc4_key, RC4_KEY_SIZE);
}

bool DoClientKeyExchange()
{
	// Generate RC4 key
	unsigned int i = 0;
	unsigned char rc4key[RC4_KEY_SIZE];
    unsigned char buffer[128];

    memset(rc4key, 0, sizeof(rc4key));
    SetRC4Key(rc4key);

	// Receive the pubic key packet
    Sleep(20);
    memset(buffer, 0, sizeof(buffer));
    int count = recv(m_Listen_Socket, (char *) buffer, 74, 0);
	if (count != 74)
	{
		fprintf(stderr,"ERROR: DoClientKeyExchange recv returned %d\n", count);
		return false;
	}

    // Ignore whatever public key packet we receive

	// Clear the buffer
	memset(buffer, 0, WWRSA_BLOCK_SIZE - RC4_KEY_SIZE + sizeof(long));

	// Put the length in front of the buffer
	unsigned char *key = buffer + sizeof(long);
	*((unsigned long *) buffer) = ntohl(WWRSA_BLOCK_SIZE);

	// Copy the RC4 key to the bottom of the buffer
	unsigned char *dest = &key[WWRSA_BLOCK_SIZE - 1];
	unsigned char *src = rc4key;
	for (i = 0; i < RC4_KEY_SIZE; i++)
	{
		*dest-- = *src++;
	}

	// Encrypt the RC4 key
	m_WestwoodRSA.Encrypt(key, WWRSA_BLOCK_SIZE, key);

	//LogMessage("Sending encrypted RC4 session key...\n");
	// Send the encrypted RC4 key to the server
    int length = WWRSA_BLOCK_SIZE + sizeof(long);
    send(m_Socket, (char *) buffer, length, 0);

	return (true);
}

bool DoKeyExchange()
{
    unsigned char buffer[128];
    unsigned char *p = buffer;
    int length = 0;

	//LogMessage("DoKeyExchange sending public RSA-155 key:\n");

    // Send the RSA Public Key to the client
    length += m_WestwoodRSA.GetModulus(&p);
    length += m_WestwoodRSA.GetPublicExponent(&p);
    send(m_Socket, (char *) buffer, length, 0);

	// Read only 4 bytes to obtain the key length
    length = recv(m_Socket, (char *) buffer, 4, 0);
	if (length <= 0)
	{
        // Lost connection, exit without displaying an error
        // This allows a server ping with no consequences
		return false;
	}

    if (length != 4)
	{
		fprintf(stderr,"ERROR: DoKeyExchange expecting 4 byte key length, length = %d\n", length);
        return false;
	}

    long key_length = (long) ntohl((*((unsigned long *) buffer)));
	fprintf(stderr,"got key length : %d\n",key_length);
    if ((key_length < 64) || (key_length > 65))
    {
		fprintf(stderr,"ERROR: DoKeyExchange key_length = %d\n", key_length);
		return false;
	}

    // Get the encrypted RC4 Session Key response from the client
    length = recv(m_Socket, (char *) buffer, key_length, 0);
	if (length <= 0)
	{
        // Lost connection, exit without displaying an error
		return false;
	}

	fprintf(stderr,"get full key\n");

    // make sure we were able to read the entire key
    if (length != key_length)
	{
		fprintf(stderr,"ERROR: DoKeyExchange key_length = %d, recv_length = %d\n", key_length, length);
		return false;
	}

    //LogMessage("DoKeyExchange received encrypted RC4 session key from client\n");
    // disregard leading byte if it is zero
    p = buffer;
    if ((key_length == WWRSA_BLOCK_SIZE + 1) && (*p == 0))
    {
        key_length--;
        p++;
    }

    // validate the key length against the expected value
    if (key_length != WWRSA_BLOCK_SIZE)
	{
		fprintf(stderr,"ERROR: DoKeyExchange key_length = %d\n", key_length);
        return false;
	}

	fprintf(stderr,"decrypt\n");

    // Decrypt the RC4 Session Key
    unsigned char rc4key[WWRSA_BLOCK_SIZE];
    if (!m_WestwoodRSA.Decrypt(p, WWRSA_BLOCK_SIZE, rc4key))
    {
		fprintf(stderr,"ERROR: DoKeyExchange m_WestwoodRSA.Decrypt failed\n");
        return false;
	}

	fprintf(stderr,"decrypted\n");

    unsigned char rc4_key_buffer[RC4_KEY_SIZE];

    // Reverse the order of the decrypted RC4 Session Key
	rc4_key_buffer[0] = rc4key[0x3f];
	rc4_key_buffer[1] = rc4key[0x3e];
	rc4_key_buffer[2] = rc4key[0x3d];
	rc4_key_buffer[3] = rc4key[0x3c];
	rc4_key_buffer[4] = rc4key[0x3b];
	rc4_key_buffer[5] = rc4key[0x3a];
	rc4_key_buffer[6] = rc4key[0x39];
	rc4_key_buffer[7] = rc4key[0x38];

	SetRC4Key(rc4_key_buffer);

	fprintf(stderr,"set\n");

    return (true);
}

#endif

// This function formats a message and adds it to the message queue
void LogMessage(char *format, ...)
{
    char buffer[8192];
	time_t rawtime;
	struct tm * timeinfo;

    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	fprintf(stderr, "%02d/%02d/%d %02d:%02d:%02d %s", timeinfo->tm_mon, timeinfo->tm_mday, timeinfo->tm_year+1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec , buffer);
}
