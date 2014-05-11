// Win32.h

#ifndef _WIN32_H_INCLUDED_
#define _WIN32_H_INCLUDED_

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <winsock2.h>
#include <objbase.h>
#include <process.h>
#include <malloc.h>
#define alloca _alloca
#define SERVER_LOGS_PATH        "..\\logs\\"
#define SERVER_HTML_PATH        "..\\html\\"
#define SERVER_DATABASE_PATH	"..\\database\\"
#define SERVER_USER_PATH		"..\\database\\Users\\"
#pragma pack(1)
#define ATTRIB_PACKED

#endif // _WIN32_H_INCLUDED_
