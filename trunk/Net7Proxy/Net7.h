// Net7.h

#ifndef _NET_7_H_INCLUDED_
#define _NET_7_H_INCLUDED_

#define VERSION "1.46"
#define VERSION_N 146

#define SSL_IN_NET7PROXY

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <ctype.h>
#include <crtdbg.h>

#pragma warning(disable:4996)


#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#endif

#include <windows.h>
#include <winsock2.h>
#include <objbase.h>
#include <process.h>
#include <malloc.h> 
#define SERVER_LOGS_PATH        "..\\logs\\"
#define SERVER_HTML_PATH        "..\\html\\"
#define SERVER_DATABASE_PATH	"..\\database\\"
#define SERVER_USER_PATH		"..\\database\\Users\\"
#define CONFIG_FILE				"Net7Config.cfg"
#pragma pack(1)
#define ATTRIB_PACKED

typedef unsigned __int64 u64;
typedef signed __int64  s64;
typedef unsigned __int32 u32;
typedef signed __int32  s32;
typedef unsigned short  u16;
typedef signed short    s16;
typedef unsigned char   u8;
typedef signed char     s8;



// This should be incremented as needed to prevent obsolete Sector Servers
// from connecting to the Master Server.
#define SECTOR_SERVER_MAJOR_VERSION			0
#define SECTOR_SERVER_MINOR_VERSION			2

// Three server types are supported by the TcpListener and Connection classes
#define CONNECTION_TYPE_CLIENT_TO_GLOBAL_SERVER			1
#define CONNECTION_TYPE_CLIENT_TO_MASTER_SERVER			2
#define	CONNECTION_TYPE_CLIENT_TO_SECTOR_SERVER			3
#define CONNECTION_TYPE_MASTER_SERVER_TO_SECTOR_SERVER	4
#define CONNECTION_TYPE_SECTOR_SERVER_TO_SECTOR_SERVER	5
#define CONNECTION_TYPE_PROXY_TO_SECTOR_SERVER          6
#define CONNECTION_TYPE_SECTOR_SERVER_TO_PROXY          9
#define CONNECTION_TYPE_GLOBAL_PROXY_TO_SERVER		    10

#define CLIENT_TYPE_FIXED_PORT                          1
#define CLIENT_TYPE_MULTI_PORT                          2

#define	MAX_BUFFER					9000
#define SSL_PORT					443 	// handles authentication (0x01BB)
extern unsigned short ssl_port;

#define GLOBAL_SERVER_PORT			3805	// handles multiple galaxies
#define MASTER_SERVER_PORT			3801	// handles a single galaxy
#define SECTOR_SERVER_PORT			3500	// handles a single sector

#define MVAS_LOGIN_PORT				3806
#define UDP_GLOBAL_SERVER_PORT      3807
#define UDP_MASTER_SERVER_PORT      3808
#define PROXY_SERVER_PORT           3809
#define PROXY_GLOBAL_PORT			3810

#define	RACE_TERRAN					0
#define RACE_JENQUAI				1
#define RACE_PROGEN					2

#define PROFESSION_WARRIOR			0
#define PROFESSION_TRADER		    1
#define PROFESSION_EXPLORER			2

extern char g_LogFilename[MAX_PATH];
extern char g_InternalIP[MAX_PATH];
extern char g_DomainName[MAX_PATH];
extern char g_MySQL_User[MAX_PATH];
extern char g_MySQL_Pass[MAX_PATH];
extern char g_MySQL_Host[MAX_PATH];
extern int g_DASE;

extern char g_Galaxy_Name[MAX_PATH];
extern long g_AddrStore;


void LockMessageQueue();
void UnlockMessageQueue();
void LogMessage(char *format, ...);
void LogDebug(char *format, ...);
void LogChatMsg(char *format, ...);
void DumpBuffer(unsigned char *buffer, int length);
void DumpBufferToFile(unsigned char *buffer, int length, char *filename, bool rawData);
bool engine_open_process(char * processwindowtitle);
bool engine_read_process(LPVOID lpBaseAddress, LPVOID lpBuffer, DWORD nSize);
bool GetProcessHandle();
bool StartENBClient();
void PatchClient();
bool ClientStillRunning();
void WaitForEngineReady();
bool ShutdownClient();
void WaitForLogin();
unsigned long GetNet7TickCount();

class GMemoryHandler;
class ServerManager;
class PlayerManager;
class StringManager;
class ItemBaseManager;
class AccountManager;

extern ServerManager * g_ServerMgr;
extern GMemoryHandler * g_GlobMemMgr;
extern PlayerManager * g_PlayerMgr;
extern ItemBaseManager * g_ItemBaseMgr;
extern AccountManager * g_AccountMgr;
extern bool g_LoggedIn;

extern bool g_Debug;
extern bool g_ServerShutdown;
extern bool g_LocalCert;

static inline void check_memory ()
{
#ifdef WIN32
    _ASSERTE (_CrtCheckMemory ());
#endif
}

#endif // _NET_7_H_INCLUDED_
