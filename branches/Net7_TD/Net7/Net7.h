// Net7.h

#ifndef _NET7_H_INCLUDED_
#define _NET7_H_INCLUDED_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <memory.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <ctype.h>

#ifdef WIN32
#include "Win32.h"
#else // LINUX
#include "Linux.h"
#endif

typedef unsigned char u8_t;
typedef unsigned short u16_t;
typedef unsigned int u32_t;
typedef unsigned long long u64_t;

typedef signed char s8_t;
typedef signed short s16_t;
typedef signed int s32_t;
typedef signed long long s64_t;

typedef float f32_t;
typedef double f64_t;

// This should be incremented as needed to prevent obsolete Sector Servers
// from connecting to the Master Server.
#define SECTOR_SERVER_MAJOR_VERSION			0
#define SECTOR_SERVER_MINOR_VERSION			3

// Three server types are supported by the TcpListener and Connection classes
#define CONNECTION_TYPE_CLIENT_TO_GLOBAL_SERVER			1
#define CONNECTION_TYPE_CLIENT_TO_MASTER_SERVER			2
#define	CONNECTION_TYPE_CLIENT_TO_SECTOR_SERVER			3
#define CONNECTION_TYPE_MASTER_SERVER_TO_SECTOR_SERVER	4
#define CONNECTION_TYPE_SECTOR_SERVER_TO_SECTOR_SERVER	5

// Original version uses https protocol on port 443
// Java version uses http protocol on port 80
// Net-7 will use https protocol on port 8891
// The SSL Port is stored in authlogin.dll at 0x0082a0
//      'BB 01' = port 443
//      '50 00' = port 80
//      'BB 22' = port 8891
// The protocol flag is stored in authlogin.dll at 0x008328
//      '40' = http
//      'C0' = https (encrypted)
//
#define	MAX_BUFFER					9000

#define SSL_PORT                    443		// handles authentication (0x01BB)

#define DEFAULT_GLOBAL_SERVER_PORT			3805	// handles multiple galaxies
#define DEFAULT_MASTER_SERVER_PORT			3801	// handles a single galaxy
#define DEFAULT_SECTOR_SERVER_PORT			3500	// handles a single sector
#define DEFAULT_MAX_SECTORS                 30
#define	ONE_LOGIN					FALSE

#define	RACE_TERRAN					0
#define RACE_JENQUAI				1
#define RACE_PROGEN					2

#define PROFESSION_WARRIOR			0
#define PROFESSION_TRADESMAN		1
#define PROFESSION_EXPLORER			2

#define SERVER_TYPE_GLOBAL          (0x1 << 0)
#define SERVER_TYPE_MASTER          (0x1 << 1)
#define SERVER_TYPE_SECTOR          (0x1 << 2)
#define SERVER_TYPE_STANDALONE      (SERVER_TYPE_GLOBAL | SERVER_TYPE_MASTER | SERVER_TYPE_SECTOR)

#define NET7_DOMAIN_NAME "local.net-7.org"

extern bool g_ServerShutdown;

#include "LogManager.h"
#include "Log.h"

extern LogManager g_LogManager;
extern Log g_Log;
extern Log g_LogDebug;
extern Log g_LogError;

#endif // _NET7_H_INCLUDED_
