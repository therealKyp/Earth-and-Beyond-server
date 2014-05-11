// Linux.h

#ifndef _LINUX_H_INCLUDED_
#define _LINUX_H_INCLUDED_

#define ATTRIB_PACKED __attribute__((packed))
#define TRUE            1
#define FALSE           0
#define MAX_PATH        260
//#define WORD            unsigned short
//#define DWORD           unsigned int
#define SOCKET          int
#define INVALID_SOCKET	-1
#define closesocket	close
#define stricmp strcasecmp
#define WSAGetLastError() (errno)
#define WSAECONNRESET	ECONNRESET

#include <pthread.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/times.h>
#include <netdb.h>
#include <sys/time.h>

#define SERVER_LOGS_PATH        "../logs/"
#define SERVER_HTML_PATH        "../html/"
#define SERVER_DATABASE_PATH    "../database/"
#define SERVER_USER_PATH        "../database/Users/"

unsigned long GetTickCount();
unsigned long GetCurrentDirectory(unsigned long size, char *path);
int SetCurrentDirectory(char *path);
void Sleep(unsigned long dwMilliseconds);
bool DeleteFile(const char *file);

#endif // _LINUX_H_INCLUDED_
