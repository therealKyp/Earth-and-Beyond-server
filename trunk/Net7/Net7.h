// Net7.h

#ifndef _NET_7_H_INCLUDED_
#define _NET_7_H_INCLUDED_

#define UPPER_VER	0
#define LOWER_VER	94
#define BUILD_VER   1302
#define VER_TYPE	"UDP Beta"

#define SQL_ENABLE

//#define SSL_IN_NET7PROXY

//#define DEV_QUICK_START  // This is for server devs to quick start a server locally for development. Most sectors will be missing

#ifdef SQL_ENABLE
    #define USE_MYSQL_ACCOUNT_DATA
	#define USE_MYSQL_STATIONS
    #define USE_MYSQL_SECTOR 
    #define USE_MYSQL_ITEMS
#endif

#ifdef USE_MYSQL_ACCOUNT_DATA
    #define SQL_ACCOUNT_STRING " - SQL Accounts"
#else
    #define SQL_ACCOUNT_STRING ""
#endif

//#define BETA_TESTING

#define _CRT_SECURE_NO_WARNINGS 1		// Disable Warning messages about new Secure Functions in VS2008
#pragma warning(disable:4996)

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <ctype.h>

#ifdef WIN32
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

	#define WIN32_THREAD_STACK_SIZE 0x10000

	typedef unsigned __int64 u64;
	typedef signed __int64  s64;
	typedef unsigned __int32 u32;
	typedef signed __int32  s32;
    typedef unsigned short  u16;
    typedef signed short    s16;
    typedef unsigned char   u8;
    typedef signed char     s8;

#else // LINUX
	#define ATTRIB_PACKED __attribute__((packed))
	#define TRUE            1
	#define FALSE           0
	#define MAX_PATH        260
	#define WORD            unsigned short
	#define DWORD           unsigned long
	#define SOCKET          int
	#define INVALID_SOCKET	-1
	#define closesocket	close
	#define stricmp strcasecmp
	#define WSAGetLastError() (errno)
	#define WSAECONNRESET	ECONNRESET

	#define ULONG 			unsigned long
	#define UINT 			unsigned int
	#define UCHAR			unsigned char
    	#define BOOL			bool
	#define u64				u_int64_t
	#define s64				int64_t
    	#define u32				u_int32_t
	#define s32				int32_t
	#define u16				u_int16_t
	#define s16				int16_t
	#define u8				u_int8_t
	#define s8				int8_t


	//#include <syslog.h>
	//#include <fcntl.h>
	//#include <netinet/in.h>
	//#include <sys/stat.h>
	//#include <sys/select.h>
	#include <arpa/inet.h>
	#include <sys/socket.h>
	#include <sys/types.h>
	#include <sys/times.h>
	#include <sys/time.h>
	#include <errno.h>
	#include <netdb.h>
	#include <pthread.h>
	#include <unistd.h>

	#define SERVER_LOGS_PATH        "../logs/"
	#define SERVER_HTML_PATH        "../html/"
	#define SERVER_DATABASE_PATH    "../database/"
	#define SERVER_USER_PATH        "../database/Users/"
	#define CONFIG_FILE				"Net7Config.cfg"

	// Some MSVC <-> GCC redefines
	#define _snprintf snprintf
	#define _strcmpi strcasecmp
	#define _isnan isnan
	#define _alloca alloca
	#define _sleep Sleep
	#define _stricmp strcasecmp
	// This might not be right... _atoi64 is specific to Windows
	#define _atoi64 atoll

	long GetTickCount();
	unsigned long GetCurrentDirectory(unsigned long size, char *path);
	int SetCurrentDirectory(const char *path);
	void Sleep(unsigned long dwMilliseconds);
	bool DeleteFile(const char *filename);

#endif

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
#define CONNECTION_TYPE_MVAS_TO_PROXY                   6
#define CONNECTION_TYPE_GLOBAL_SERVER_TO_PROXY          7
#define CONNECTION_TYPE_MASTER_SERVER_TO_PROXY          8
#define CONNECTION_TYPE_SECTOR_SERVER_TO_PROXY          9
#define CONNECTION_TYPE_GLOBAL_PROXY_TO_SERVER		    10


#define	MAX_BUFFER					9000
#define SSL_PORT					443		// handles authentication (0x01BB)
//#define SSL_PORT					8891	// handles authentication - HTTPS protocol (0x22BB)

#define GLOBAL_SERVER_PORT			3805	// handles multiple galaxies
#define MASTER_SERVER_PORT			3801	// handles a single galaxy
#define SECTOR_SERVER_PORT			3501	// handles a single sector - note we start from 3501 now because 3500 is used as the local TCP port in Net7Proxy
#define MVAS_LOGIN_PORT				3806

#define UDP_GLOBAL_SERVER_PORT      3807
#define UDP_MASTER_SERVER_PORT      3808
#define PROXY_SERVER_PORT           3809

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

void LockMessageQueue();
void UnlockMessageQueue();
void LogMessage(char *format, ...);
void LogDebug(char *format, ...);
void LogChatMsg(char *format, ...);
void LogMySQLMsg(char *format, ...);
void DumpBuffer(unsigned char *buffer, int length);
void DumpBufferToFile(unsigned char *buffer, int length, char *filename, bool rawData);
unsigned long GetNet7TickCount();

class GMemoryHandler;
class ServerManager;
class PlayerManager;
class StringManager;
class ItemBaseManager;
class AccountManager;
class SaveManager;

extern ServerManager * g_ServerMgr;
extern GMemoryHandler * g_GlobMemMgr;
extern PlayerManager * g_PlayerMgr;
extern StringManager * g_StringMgr;
extern ItemBaseManager * g_ItemBaseMgr;
extern AccountManager * g_AccountMgr;
extern SaveManager	  * g_SaveMgr;

extern bool g_Debug;
extern bool g_ServerShutdown;

#endif // _NET_7_H_INCLUDED_
