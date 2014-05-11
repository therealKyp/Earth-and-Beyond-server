//==============================================================================
// ClientDetours.cpp
//
// written by VectoR.360
//
// see http://research.microsoft.com/sn/detours/
//
//==============================================================================

#include <stdio.h>
#include <windows.h>
#include <process.h>
#include "detours.h"

#pragma comment(lib, "wsock32")
#pragma comment(lib, "detours")

static	CRITICAL_SECTION	g_Mutex;
static	bool				g_Shutdown = false;
static	bool				g_FirstTime = true;
static	char				g_LogFilename[MAX_PATH];
static  char                g_Buffer[16384] = {NULL};


////////////////////////////////////////////////////////////////////////////////
// Class : MessageQueue
////////////////////////////////////////////////////////////////////////////////

class MessageQueue
{
public:
	MessageQueue();
	virtual ~MessageQueue();

// Public methods
public:
	void Add( char *buffer );
	void AddHead( char *buffer );
	bool CheckQueue( char **pMessage );

// Private member attributes
private:
    CRITICAL_SECTION m_mutex;

	struct MessageEntry;
	struct MessageEntry
	{
		char *message;
		struct MessageEntry *next;
	};

	MessageEntry *m_Queue;

	DWORD m_TotalAdded;
	DWORD m_TotalAddedToHead;
	DWORD m_TotalAddedToTail;
	DWORD m_TotalRemoved;
	DWORD m_EntriesInQueue;
};

static	MessageQueue g_Queue;

MessageQueue::MessageQueue()
{
	m_Queue = NULL;
	m_TotalAdded = 0;
	m_TotalAddedToHead = 0;
	m_TotalAddedToTail = 0;
	m_TotalRemoved = 0;
	m_EntriesInQueue = 0;
    InitializeCriticalSection( &m_mutex );
}

MessageQueue::~MessageQueue()
{
	char *message;

	while (CheckQueue( &message ))
	{
		delete message;
	}

    // Delete the mutex lock
    DeleteCriticalSection( &m_mutex );
}

void MessageQueue::Add( char *buffer )
{
	EnterCriticalSection( &m_mutex );

	MessageEntry *new_entry = new MessageEntry;
	new_entry->message = new char[strlen(buffer)+1];
	strcpy( new_entry->message, buffer );
	new_entry->next = NULL;

	// Is the Queue empty?
	if (m_Queue)
	{
		// No, Find the end of the linked list
		MessageEntry *entry = m_Queue;
		while (entry->next)
		{
			entry = entry->next;
		}

		// Add this entry to the end of the linked list
		entry->next = new_entry;
	}
	else
	{
		// The linked list is empty, so this is the first entry
		m_Queue = new_entry;
	}

	m_TotalAdded++;
	m_TotalAddedToTail++;
	m_EntriesInQueue++;

	LeaveCriticalSection( &m_mutex );
}


void MessageQueue::AddHead( char *buffer )
{
	EnterCriticalSection( &m_mutex );

	MessageEntry *new_entry = new MessageEntry;
	new_entry->message = new char[strlen(buffer)+1];
	strcpy( new_entry->message, buffer );
	new_entry->next = m_Queue;

	// Insert this entry at the head of the Queue
	m_Queue = new_entry;

	m_TotalAdded++;
	m_TotalAddedToHead++;
	m_EntriesInQueue++;

	LeaveCriticalSection( &m_mutex );
}


// returns true if message was removed from the Queue
bool MessageQueue::CheckQueue( char **pMessage )
{
	bool success = false;
	EnterCriticalSection( &m_mutex );

	if (m_Queue)
	{
		MessageEntry *entry = m_Queue;
		m_Queue = entry->next;
		*pMessage = entry->message;
		delete entry;

		m_TotalRemoved++;
		m_EntriesInQueue--;
		success = true;
	}

	LeaveCriticalSection( &m_mutex );

	return (success);
}

//==============================================================================
// This function formats a message and adds it to the message queue
//==============================================================================

void LogMessage(char *format, ...)
{
    char buffer[4096];

    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

	g_Queue.Add(buffer);
}

//==============================================================================
// This thread writes the messages from the queue to the log file
//==============================================================================

void __cdecl LogWriterThread(void *arg)
{
    int timer = 0;
    FILE *f = NULL;

	while (!g_Shutdown)
	{
		// Loop 20x per second
		Sleep(50);

        // Check for messages in the message logger queue
		char *msg = NULL;
        if (g_Queue.CheckQueue(&msg))
        {
            // We have at least one message in the queue
			if (!f)
			{
                // If the log file is not open, then open it
	            f = fopen(g_LogFilename, "a+");
			}
            // Did we successfully open the log file?
            if (f)
            {
                // Write the message to the log file
                fprintf(f, "%s", msg);
				delete msg;
                // Loop through any other messages in the queue
				while (g_Queue.CheckQueue(&msg))
				{
	                fprintf(f, "%s", msg);
					delete msg;
				}
                // flush our data to the log file immediately
				fflush(f);
                // set a timer so that we don't keep opening and closing
                // the log file.
                timer = 40;	// leave the file open for at least 2 seconds (2000ms / 50ms)
            }
			else
			{
				delete msg;
			}
        }
        else if (timer)
        {
			// if the log file has been idle for 2 seconds, close it
            timer--;
            if (timer == 0)
            {
                fclose(f);  // close the log file
				f = NULL;   // forget the file handle
            }
        }
	}

    if (f)
    {
        fclose(f);
		f = NULL;
	}

    _endthread();
}

//==============================================================================
// DumpHex utility
//==============================================================================

void DumpHex(char *buffer, int count)
{
	//LogMessage("Dumping %d bytes:\n", count);
	for (int i = 0; i < count; i++)
	{
		if ((i % 16) == 0)
		{
			LogMessage("    ");
		}
		LogMessage(" %02X", (unsigned char) buffer[i]);
		if ((i % 16) == 15)
		{
			LogMessage("\n");
		}
	}
	if ((i % 16) != 0)
	{
		LogMessage("\n");
	}
}

//==============================================================================
// OutputDebugString
//==============================================================================

DETOUR_TRAMPOLINE(void __stdcall OutputDebugStringATrampoline(LPCSTR lpOutputString),
                  OutputDebugString);

void __stdcall OutputDebugStringADetour(LPCSTR lpOutputString)
{
    __try
    {
        OutputDebugStringATrampoline(lpOutputString);
    }
    __finally
    {
        EnterCriticalSection(&g_Mutex);
		LogMessage("%s", lpOutputString);
        LeaveCriticalSection(&g_Mutex);
    };
}

//==============================================================================
// Detour: socket
//==============================================================================

DETOUR_TRAMPOLINE(int WINAPI socketTrampoline(  int af,
												int type,
												int protocol
												),
                  socket);

SOCKET WINAPI socketDetour(int af,
						   int type,
						   int protocol
						   )
{
    SOCKET rval = 0;
    __try
    {
        rval = socketTrampoline(af, type, protocol);
    }
    __finally
    {
        EnterCriticalSection(&g_Mutex);
		char af_str[64];
		char type_str[64];
		char proto_str[64];

		switch (af)
		{
		case AF_INET :
			strcpy(af_str, "AF_INET");
			break;
		case AF_NETBIOS :
			strcpy(af_str, "AF_NETBIOS");
			break;
		default :
			sprintf(af_str, "%d", af);
		}
		switch (type)
		{
		case SOCK_STREAM :
			strcpy(type_str, "SOCK_STREAM");
			break;
		case SOCK_DGRAM :
			strcpy(type_str, "SOCK_DGRAM");
			break;
		case SOCK_RAW :
			strcpy(type_str, "SOCK_RAW");
			break;
		default :
			sprintf(type_str, "%d", type);
		}
		switch (protocol)
		{
		case IPPROTO_IP :
			strcpy(proto_str, "IPPROTO_IP");
			break;
		case IPPROTO_UDP :
			strcpy(proto_str, "IPPROTO_UDP");
			break;
		case IPPROTO_TCP :
			strcpy(proto_str, "IPPROTO_TCP");
			break;
		default:
			sprintf(proto_str, "%d", protocol);
		}
	    LogMessage("socket(af=%s, type=%s, protocol=%s) returned SOCKET %d\n",
			af_str,
			type_str,
			proto_str,
			rval);
        LeaveCriticalSection(&g_Mutex);
    };

    return rval;
}

//==============================================================================
// Detour: connect
//==============================================================================

DETOUR_TRAMPOLINE(int WINAPI connectTrampoline(SOCKET s,                          
											  const struct sockaddr FAR *name,  
											  int namelen
											  ),
                  connect);

int WINAPI connectDetour( SOCKET s,                          
						  const struct sockaddr FAR *name,  
						  int namelen
						  )
{
    int rval = 0;
    __try
    {
        rval = connectTrampoline(s, name, namelen);
    }
    __finally
    {
        EnterCriticalSection(&g_Mutex);
		if (rval == -1)
		{
		    LogMessage("connect(s=%d, name=0x%08x, namelen=%d) returned failure, error=%d\n", s, name, namelen, WSAGetLastError());
		}
		else if (rval == 0)
		{
		    LogMessage("connect(s=%d, name=0x%08x, namelen=%d) returned success\n", s, name, namelen);
		}
		else
		{
		    LogMessage("connect(s=%d, name=0x%08x, namelen=%d) returned %d\n", s, name, namelen, rval);
		}
		sockaddr_in *addr = (sockaddr_in *) name;
		unsigned char *ip = (unsigned char *) &addr->sin_addr.S_un.S_addr;
		unsigned short port = ntohs(addr->sin_port);
		LogMessage("    ip address = %d.%d.%d.%d  port=%d\n", ip[0], ip[1], ip[2], ip[3], port);
        LeaveCriticalSection(&g_Mutex);
    };

    return rval;
}

//==============================================================================
// Detour: select
//==============================================================================

DETOUR_TRAMPOLINE(int WINAPI selectTrampoline(int nfds,
                                              fd_set FAR *readfds,
                                              fd_set FAR *writefds,
                                              fd_set FAR *exceptfds,
                                              const struct timeval FAR *timeout
                                              ),
                  select);

int WINAPI selectDetour(int nfds,
                        fd_set FAR *readfds,
                        fd_set FAR *writefds,
                        fd_set FAR *exceptfds,
                        const struct timeval FAR *timeout
					    )
{
    int rval = 0;
    __try
    {
        rval = selectTrampoline(nfds, readfds, writefds, exceptfds, timeout);
    }
    __finally
    {
        EnterCriticalSection(&g_Mutex);
        if (rval == SOCKET_ERROR)
        {
	        LogMessage("select() failed, error=%d\n", WSAGetLastError());
        }
        else
        {
            if (rval > 0)
            {
                SOCKET read_socket = INVALID_SOCKET;
                unsigned int i;
	            LogMessage("select(nfds=n/a, readfds=0x%08x, writefds=0x%08x, exceptfds=0x%08x, timeout=%d.%03d) returned %d\n",
                    readfds, writefds, exceptfds, timeout->tv_sec, timeout->tv_usec, rval);
                if (readfds)
                {
                    for (i = 0; i < readfds->fd_count; i++)
                    {
                        LogMessage("readfds[%d] = SOCKET %d\n", i, readfds->fd_array[i]);
                        read_socket = readfds->fd_array[i];
                    }
                }
                if (writefds)
                {
                    for (i = 0; i < writefds->fd_count; i++)
                    {
                        LogMessage("writefds[%d] = SOCKET %d\n", i, writefds->fd_array[i]);
                    }
                }
                if (exceptfds)
                {
                    for (i = 0; i < exceptfds->fd_count; i++)
                    {
                        LogMessage("exceptfds[%d] = SOCKET %d\n", i, exceptfds->fd_array[i]);
                    }
                }
                if (read_socket != INVALID_SOCKET)
                {
                    LogMessage("Peeking at SOCKET %d\n", read_socket);
                    // Take a peek at the data
                    struct sockaddr_in from = {NULL};
                    int from_length = sizeof(from);
                    int n = recvfrom(read_socket, g_Buffer, sizeof(g_Buffer), MSG_PEEK, (sockaddr *) &from, &from_length);
                    if (n > 0)
                    {
                        LogMessage("    MSG_PEEK successful, returned %d bytes\n", n);
                    }
                    else
                    {
                        LogMessage("    MSG_PEEK failed, returned %d, error=%d\n", n, WSAGetLastError());
                    }
                }
            }
        }
        LeaveCriticalSection(&g_Mutex);
    };

    return rval;
}

//==============================================================================
// Detour: recv
//==============================================================================

DETOUR_TRAMPOLINE(int WINAPI recvTrampoline(SOCKET s,
                                       char* buf,
                                       int len,
                                       int flags),
                  recv);

int WINAPI recvDetour(SOCKET s,
                      char* buf,
                      int len,
                      int flags
					  )
{
    LogMessage("[[[ recv ]]]\n");
    int rval = 0;
    __try
    {
        rval = recvTrampoline(s, buf, len, flags);
    }
    __finally
    {
    };

    EnterCriticalSection(&g_Mutex);
	LogMessage("recv(s=%d, buf=0x%08x, len=%d, flags=%d) returned %d\n", s, buf, len, flags, rval);
	if (rval > 0)
	{
		DumpHex(buf, rval);
	}
    LeaveCriticalSection(&g_Mutex);

    return rval;
}

//==============================================================================
// Detour: send
//==============================================================================

DETOUR_TRAMPOLINE(int WINAPI sendTrampoline(SOCKET s,
										    char* buf,
										    int len,
										    int flags),
                  send);

int WINAPI sendDetour(SOCKET s,
                      char* buf,
                      int len,
                      int flags
					  )
{
    int rval = 0;
    __try
    {
        rval = sendTrampoline(s, buf, len, flags);
    }
    __finally
    {
    };

    EnterCriticalSection(&g_Mutex);
	LogMessage("send(s=%d, buf=0x%08x, len=%d, flags=%d) returned %d\n", s, buf, len, flags, rval);
	if (len > 0)
	{
		DumpHex(buf, len);
	}
    LeaveCriticalSection(&g_Mutex);

    return rval;
}

//==============================================================================
// Detour: recvfrom
//==============================================================================

DETOUR_TRAMPOLINE(int WINAPI recvfromTrampoline(SOCKET s,
											    char* buf,
											    int len,
											    int flags,
											    struct sockaddr FAR *from,
											    int FAR *fromlen),
                  recvfrom);

int WINAPI recvfromDetour(SOCKET s,
                          char* buf,
                          int len,
                          int flags,
						  struct sockaddr FAR *from,
						  int FAR *fromlen
						  )
{
    int rval = 0;
    __try
    {
        rval = recvfromTrampoline(s, buf, len, flags, from, fromlen);
    }
    __finally
    {
        EnterCriticalSection(&g_Mutex);
	    LogMessage("recvfrom(s=%d, buf=0x%08x, len=%d, flags=%d, from=0x%08x, fromlen=%d) returned %d\n", s, buf, len, flags, from, *fromlen, rval);
		sockaddr_in *addr = (sockaddr_in *) from;
		unsigned char *ip = (unsigned char *) &addr->sin_addr.S_un.S_addr;
		unsigned short port = ntohs(addr->sin_port);
		LogMessage("    ip address = %d.%d.%d.%d  port=%d\n", ip[0], ip[1], ip[2], ip[3], port);
		if (rval > 0)
		{
			DumpHex(buf, rval);
		}
        LeaveCriticalSection(&g_Mutex);
    };

    return rval;
}

//==============================================================================
//  Detour: sendto
//==============================================================================

DETOUR_TRAMPOLINE(int WINAPI sendtoTrampoline(SOCKET s,
                                         char* buf,
                                         int len,
                                         int flags,
                                         sockaddr* to,
                                         int tolen),
                  sendto);

int WINAPI sendtoDetour(SOCKET s,
                       char* buf,
                       int len,
                       int flags,
                       sockaddr* to,
                       int tolen)
{
    int rval = 0;
    __try
    {
        rval = sendtoTrampoline(s, buf, len, flags, to, tolen);
    }
    __finally
    {
        EnterCriticalSection(&g_Mutex);
		LogMessage("sendto(s=%d, buf=0x%08x, len=%d, flags=%d, to=0x%08x, tolen=%d) returned %d\n",
			s, buf, len, flags, to, tolen, rval);
		sockaddr_in *addr = (sockaddr_in *) to;
		unsigned char *ip = (unsigned char *) &addr->sin_addr.S_un.S_addr;
		unsigned short port = ntohs(addr->sin_port);
		LogMessage("    ip address = %d.%d.%d.%d  port=%d\n", ip[0], ip[1], ip[2], ip[3], port);
		if (len > 0)
		{
			DumpHex(buf, len);
		}
        LeaveCriticalSection(&g_Mutex);
    };

    return rval;
}

//==============================================================================
//  Detour: gethostname
//==============================================================================

DETOUR_TRAMPOLINE(int WINAPI gethostnameTrampoline(char FAR *name, int namelen),
                  gethostname);

int WINAPI gethostnameDetour(char FAR *name, int namelen)
{
    int rval = 0;
    __try
    {
        rval = gethostnameTrampoline(name, namelen);
    }
    __finally
    {
        EnterCriticalSection(&g_Mutex);
        if (rval == 0)
        {
    		LogMessage("gethostname(name='%s', namelen='%d') succeeded\n", name, namelen);
        }
        else
        {
    		LogMessage("gethostname failed, error=%d\n", WSAGetLastError());
        }
        LeaveCriticalSection(&g_Mutex);
    };

    return rval;
}

//==============================================================================
//  Detour: gethostbyname
//==============================================================================

DETOUR_TRAMPOLINE(struct hostent FAR * WINAPI gethostbynameTrampoline(const char FAR *name),
                  gethostbyname);

struct hostent FAR * WINAPI gethostbynameDetour(const char FAR *name)
{
    struct hostent FAR * rval = NULL;
    __try
    {
        rval = gethostbynameTrampoline(name);
    }
    __finally
    {
        EnterCriticalSection(&g_Mutex);
        if (rval == NULL)
        {
    		LogMessage("gethostbyname(name='%s') failed, error=%d\n", name, WSAGetLastError());
        }
        else
        {
            unsigned char * ip = (unsigned char *) rval->h_addr_list[0];
    		LogMessage("gethostbyname(name='%s') succeeded, ip = %d.%d.%d.%d\n",
                name, ip[0], ip[1], ip[2], ip[3]);
        }
        LeaveCriticalSection(&g_Mutex);
    };

    return rval;
}

//==============================================================================
//  Detour: inet_ntoa
//==============================================================================

DETOUR_TRAMPOLINE(char FAR * WINAPI inet_ntoaTrampoline(struct in_addr in),
                  inet_ntoa);

char FAR * WINAPI inet_ntoaDetour(struct in_addr in)
{
    char FAR * rval = NULL;
    __try
    {
        rval = inet_ntoaTrampoline(in);
    }
    __finally
    {
        EnterCriticalSection(&g_Mutex);
        if (rval == NULL)
        {
    		LogMessage("inet_ntoa(0x%08x) failed, error=%d\n", in.S_un.S_addr, WSAGetLastError());
        }
        else
        {
    		LogMessage("inet_ntoa(in=0x%08x) returned '%s'\n", in.S_un.S_addr, rval);
        }
        LeaveCriticalSection(&g_Mutex);
    };


    return rval;
}

//==============================================================================
//  Detour: closesocket
//==============================================================================

DETOUR_TRAMPOLINE(int WINAPI closesocketTrampoline(SOCKET s),
                  closesocket);

int WINAPI closesocketDetour(SOCKET s)
{
    int rval = NULL;
    __try
    {
        rval = closesocketTrampoline(s);
    }
    __finally
    {
        EnterCriticalSection(&g_Mutex);
        if (rval == 0)
        {
    		LogMessage("closesocket(s=%d) successful\n", s);
        }
        else
        {
    		LogMessage("closesocket(s=%d) failed, error=%d\n", s, WSAGetLastError());
        }
        LeaveCriticalSection(&g_Mutex);
    };

    return rval;
}

//==============================================================================
//  Detour: listen
//==============================================================================

DETOUR_TRAMPOLINE(int WINAPI listenTrampoline(SOCKET s, int backlog),
                  listen);

int WINAPI listenDetour(SOCKET s, int backlog)
{
    int rval = NULL;
    __try
    {
        rval = listenTrampoline(s, backlog);
    }
    __finally
    {
        EnterCriticalSection(&g_Mutex);
        if (rval == 0)
        {
    		LogMessage("listen(s=%d, backlog=%d) successful\n", s, backlog);
        }
        else
        {
    		LogMessage("listen(s=%d, backlog=%d) failed, error=%d\n", s, backlog, WSAGetLastError());
        }
        LeaveCriticalSection(&g_Mutex);
    };

    return rval;
}

//==============================================================================
//  Detour: accept
//==============================================================================

DETOUR_TRAMPOLINE(int WINAPI acceptTrampoline(SOCKET s,
                                              struct sockaddr FAR *addr,
                                              int FAR *addrlen
                                              ),
                  accept);

int WINAPI acceptDetour(SOCKET s,
                        struct sockaddr FAR *addr,
                        int FAR *addrlen
                        )
{
    int rval = NULL;
    __try
    {
        rval = acceptTrampoline(s, addr, addrlen);
    }
    __finally
    {
        EnterCriticalSection(&g_Mutex);
        if (rval != INVALID_SOCKET)
        {
    		LogMessage("accept(s=%d, addr=0x%08x, *addrlen=%d) returned SOCKET %d\n", s, addr, *addrlen, rval);
        }
        else
        {
    		LogMessage("accept(s=%d, addr=0x%08x, *addrlen=%d) failed, error=%d\n", 
                s, addr, *addrlen, WSAGetLastError());
        }
        LeaveCriticalSection(&g_Mutex);
    };

    return rval;
}

//==============================================================================
//  Detour: fopen
//==============================================================================

DETOUR_TRAMPOLINE_EMPTY(FILE * fopenTrampoline(const char *filename, const char *mode))

FILE * fopenDetour(const char *filename, const char *mode)
{
	if (g_FirstTime)
	{
		g_FirstTime = false;
		char * debug_flag = (char *) 0x00C40488;
		char temp = *debug_flag;
		*debug_flag = 1;
        EnterCriticalSection(&g_Mutex);
		LogMessage("Changed debug_flag from %d to 1\n", temp);
        LeaveCriticalSection(&g_Mutex);
	}

    FILE * rval = 0;
    __try
    {
        rval = fopenTrampoline(filename, mode);
    }
    __finally
    {
        EnterCriticalSection(&g_Mutex);
		LogMessage("fopen(\"%s\", \"%s\") returned FILE * 0x%08x\n", filename, mode, rval);
        LeaveCriticalSection(&g_Mutex);
    };

    return rval;
}

//==============================================================================
//  Detour: fclose
//==============================================================================

DETOUR_TRAMPOLINE_EMPTY(int fcloseTrampoline(FILE *f))

int fcloseDetour(FILE *f)
{
    int rval = 0;
    __try
    {
        rval = fcloseTrampoline(f);
    }
    __finally
    {
        EnterCriticalSection(&g_Mutex);
		LogMessage("fclose(f=0x%08x) returned %d\n", f, rval);
        LeaveCriticalSection(&g_Mutex);
    };

    return rval;
}

//==============================================================================
// dbgmgr
//==============================================================================

DETOUR_TRAMPOLINE_EMPTY(int dbgmgrTrampoline(char *str))

void dbgmgrDetour(char *str)
{
    __try
    {
        dbgmgrTrampoline(str);
    }
    __finally
    {
        // TODO: filter out unnecessary messages
        EnterCriticalSection(&g_Mutex);
		LogMessage("> %s\n", str);
        LeaveCriticalSection(&g_Mutex);
    };
}

//==============================================================================
// CGetpkclassDetour class
//==============================================================================

// Establish a list of virtual methods for the Packet Class
class BasePacketClass
{
public:
    BasePacketClass();
    virtual ~BasePacketClass();     // destructor
    virtual void Create_Packet();
    virtual void Process_Packet();
    virtual void Get_Packet_Length();
    virtual void Display_Packet(char *buffer, long count);

public:
    // VTABLE is contained at this+0
    long    unknown;            // this+4
    long    packet_counter;     // this+8
};

class CconnDetour
{
  public:
    void * connDetour();
    void * connTrampoline();
    // The detour class should not have any member variables or virtual functions.
};

DETOUR_TRAMPOLINE_EMPTY(void * CconnDetour::connTrampoline());

void * CconnDetour::connDetour()
{
    void *rval = NULL;
    __try
    {
        rval = connTrampoline();
    }
    __finally
    {
        EnterCriticalSection(&g_Mutex);
        if (rval)
        {
            BasePacketClass * pk = (BasePacketClass *) rval;
            pk->Display_Packet(g_Buffer, sizeof(g_Buffer));
		    LogMessage(">> %s\n", g_Buffer);
        }
        LeaveCriticalSection(&g_Mutex);
    };

    return (rval);
}

//==============================================================================
// rc4.prepare_key
//==============================================================================

class CPrepareKeyDetour
{
  public:
    void prepareKeyDetour(unsigned char *key_data, unsigned long key_length);
    void prepareKeyTrampoline(unsigned char *key_data, unsigned long key_length);
    // The detour class should not have any member variables or virtual functions.
};

DETOUR_TRAMPOLINE_EMPTY(void CPrepareKeyDetour::prepareKeyTrampoline(unsigned char *key_data, unsigned long key_length));

void CPrepareKeyDetour::prepareKeyDetour(unsigned char *key_data, unsigned long key_length)
{
    __try
    {
        prepareKeyTrampoline(key_data, key_length);
    }
    __finally
    {
        EnterCriticalSection(&g_Mutex);
        LogMessage("rc4.prepare_key()\n");
        DumpHex((char *) key_data, (int) key_length);
        LeaveCriticalSection(&g_Mutex);
    };
}

//==============================================================================
// rc4.RC4
//==============================================================================

class CryptoRC4Detour
{
  public:
    void rc4Detour(unsigned char *data, unsigned long length);
    void rc4Trampoline(unsigned char *data, unsigned long length);
    // The detour class should not have any member variables or virtual functions.
};

DETOUR_TRAMPOLINE_EMPTY(void CryptoRC4Detour::rc4Trampoline(unsigned char *data, unsigned long length));

void CryptoRC4Detour::rc4Detour(unsigned char *data, unsigned long length)
{
    LogMessage("rc4.encrypt(data=0x%08x, length=%d) -- before:\n", data, length);
    DumpHex((char *) data, (int) length);
    __try
    {
        rc4Trampoline(data, length);
    }
    __finally
    {
        EnterCriticalSection(&g_Mutex);
        LogMessage("rc4.encrypt -- after:\n");
        DumpHex((char *) data, (int) length);
        LeaveCriticalSection(&g_Mutex);
    };
}

//==============================================================================
// CRC32
//==============================================================================

DETOUR_TRAMPOLINE_EMPTY(unsigned long CRC32Trampoline(unsigned char *data, unsigned long length))

unsigned long CRC32Detour(unsigned char *data, unsigned long length)
{
    unsigned long rval = 0;
    __try
    {
        rval = CRC32Trampoline(data, length);
    }
    __finally
    {
        EnterCriticalSection(&g_Mutex);
		LogMessage("CRC32(data=0x%08x, length=%d) returned 0x%08X\n", data, length, rval);
        DumpHex((char *) data, (int) length);
        LeaveCriticalSection(&g_Mutex);
    };

    return rval;
}

//==============================================================================
// DLL Main entry point
//==============================================================================

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    void * fopenTarget      = (void *) 0x00A27B12;
    void * fcloseTarget     = (void *) 0x00A2794D;
    void * dbgmgrTarget     = (void *) 0x009EA930;
    void * connTarget       = (void *) 0x008236D0; // method in Common\connect.cpp connection class
    void * (CconnDetour::* connDetour)()     = CconnDetour::connDetour;
    void * (CconnDetour::* connTrampoline)() = CconnDetour::connTrampoline;
    void * prepareKeyTarget       = (void *) 0x009E0290;
    void (CPrepareKeyDetour::* prepareKeyDetour)(unsigned char *, unsigned long)     = CPrepareKeyDetour::prepareKeyDetour;
    void (CPrepareKeyDetour::* prepareKeyTrampoline)(unsigned char *, unsigned long) = CPrepareKeyDetour::prepareKeyTrampoline;
    void * rc4Target       = (void *) 0x009E0420;
    void (CryptoRC4Detour::* rc4Detour)(unsigned char *, unsigned long)     = CryptoRC4Detour::rc4Detour;
    void (CryptoRC4Detour::* rc4Trampoline)(unsigned char *, unsigned long) = CryptoRC4Detour::rc4Trampoline;
    void * CRC32Target     = (void *) 0x008B3EC0;
    
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			InitializeCriticalSection(&g_Mutex);
			strcpy(g_LogFilename, "C:\\Net7\\logs\\ClientDetours.log");

			/* Enter a new scope due to case statements */
			{
				char logfile[MAX_PATH];
				char *logf = &logfile[0];

				/* File was created by launcher.exe */
				FILE *f = fopen("DetoursLogPath.txt", "r");
				if (f)
				{
					char buffer[MAX_PATH];
					if (fgets(buffer, sizeof(buffer), f))
					{
						logf = strtok(buffer, "\n\r");
						strcpy(g_LogFilename, logf);
					}
					fclose(f);
				}
			}

            DeleteFile(g_LogFilename);
		    _beginthread(&LogWriterThread, 0, 0);
			LogMessage("Attached ClientDetours.dll to client.exe process\n");
            DetourFunctionWithTrampoline((PBYTE) OutputDebugStringATrampoline,
                                         (PBYTE) OutputDebugStringADetour);
            DetourFunctionWithTrampoline((PBYTE) socketTrampoline,
                                         (PBYTE) socketDetour);
            DetourFunctionWithTrampoline((PBYTE) connectTrampoline,
                                         (PBYTE) connectDetour);
            DetourFunctionWithTrampoline((PBYTE) selectTrampoline,
                                         (PBYTE) selectDetour);
            //DetourFunctionWithTrampoline((PBYTE) recvTrampoline,
            //                             (PBYTE) recvDetour);
            //DetourFunctionWithTrampoline((PBYTE) sendTrampoline,
            //                             (PBYTE) sendDetour);
            //DetourFunctionWithTrampoline((PBYTE) recvfromTrampoline,
            //                             (PBYTE) recvfromDetour);
            //DetourFunctionWithTrampoline((PBYTE) sendtoTrampoline,
            //                             (PBYTE) sendtoDetour);
            DetourFunctionWithTrampoline((PBYTE) gethostnameTrampoline,
                                         (PBYTE) gethostnameDetour);
            DetourFunctionWithTrampoline((PBYTE) gethostbynameTrampoline,
                                         (PBYTE) gethostbynameDetour);
            DetourFunctionWithTrampoline((PBYTE) inet_ntoaTrampoline,
                                         (PBYTE) inet_ntoaDetour);
            DetourFunctionWithTrampoline((PBYTE) closesocketTrampoline,
                                         (PBYTE) closesocketDetour);
            //DetourFunctionWithTrampoline((PBYTE) listenTrampoline,
            //                             (PBYTE) listenDetour);
            //DetourFunctionWithTrampoline((PBYTE) acceptTrampoline,
            //                             (PBYTE) acceptDetour);
            DetourFunctionWithEmptyTrampoline((PBYTE) fopenTrampoline,
                                              (PBYTE) fopenTarget,
                                              (PBYTE) fopenDetour);
            DetourFunctionWithEmptyTrampoline((PBYTE) fcloseTrampoline,
                                              (PBYTE) fcloseTarget,
                                              (PBYTE) fcloseDetour);
            //DetourFunctionWithEmptyTrampoline((PBYTE) dbgmgrTrampoline,
            //                                  (PBYTE) dbgmgrTarget,
            //                                  (PBYTE) dbgmgrDetour);
	        DetourFunctionWithEmptyTrampoline(*(PBYTE*)&connTrampoline,
                                               (PBYTE)  connTarget,
                                              *(PBYTE*)&connDetour);
	        //DetourFunctionWithEmptyTrampoline(*(PBYTE*)&prepareKeyTrampoline,
            //                                   (PBYTE)  prepareKeyTarget,
            //                                  *(PBYTE*)&prepareKeyDetour);
	        //DetourFunctionWithEmptyTrampoline(*(PBYTE*)&rc4Trampoline,
            //                                   (PBYTE)  rc4Target,
            //                                  *(PBYTE*)&rc4Detour);
            //DetourFunctionWithEmptyTrampoline((PBYTE) CRC32Trampoline,
            //                                  (PBYTE) CRC32Target,
            //                                  (PBYTE) CRC32Detour);
            break;

		case DLL_PROCESS_DETACH:
            /*
            DetourRemove((PBYTE) socketTrampoline,
                         (PBYTE) socketDetour);
            DetourRemove((PBYTE) connectTrampoline,
                         (PBYTE) connectDetour);
            DetourRemove((PBYTE) recvTrampoline,
                         (PBYTE) recvDetour);
            DetourRemove((PBYTE) sendTrampoline,
                         (PBYTE) sendDetour);
            DetourRemove((PBYTE) recvfromTrampoline,
                         (PBYTE) recvfromDetour);
            DetourRemove((PBYTE) sendtoTrampoline,
                         (PBYTE) sendtoDetour);
            DetourRemove((PBYTE) gethostnameTrampoline,
                         (PBYTE) gethostnameDetour);
            DetourRemove((PBYTE) gethostbynameTrampoline,
                         (PBYTE) gethostbynameDetour);
            DetourRemove((PBYTE) inet_ntoaTrampoline,
                         (PBYTE) inet_ntoaDetour);
            DetourRemove((PBYTE) closesocketTrampoline,
                         (PBYTE) closesocketDetour);
            DetourRemove((PBYTE) listenTrampoline,
                         (PBYTE) listenDetour);
            DetourRemove((PBYTE) acceptTrampoline,
                         (PBYTE) acceptDetour);
			DetourRemove((PBYTE) fopenTrampoline,
						 (PBYTE) fopenDetour);
			DetourRemove((PBYTE) fcloseTrampoline,
						 (PBYTE) fcloseDetour);
			DetourRemove((PBYTE) dbgmgrTrampoline,
						 (PBYTE) dbgmgrDetour);
            */
			LogMessage("Detached ClientDetours.dll from client.exe process\n");
			Sleep(100);
			g_Shutdown = true;
			Sleep(100);
			DeleteCriticalSection(&g_Mutex);
            break;

        case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
			break;
    }
    return TRUE;
}

