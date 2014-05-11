// SSL_Connection.h

#ifndef _SSL_CONNECTION_H_INCLUDED_
#define _SSL_CONNECTION_H_INCLUDED_

#include "Mutex.h"
#include "openssl/ssl.h"

// WARNING: This limits the size of images on the secure web pages.
#define MAX_SERVER_FILE_SIZE    16384
#define USERNAME_TAG            "username="
#define PASSWORD_TAG            "password="
#define PORT_TAG                "port="
#define MAX_SECTORS_TAG         "max_sectors="
#define VERSION_TAG             "version="
#define FORUM_NAME_TAG          "forumname="
#define EMAIL_TAG               "email="
#define LKEY_TAG                "lkey="

#define SSL_RECV_BUFFER			2048
class ServerManager;

class SSL_Connection
{
public:
	SSL_Connection();
	virtual ~SSL_Connection();

public:
	SSL_Connection * ReSetSSL_Connection(SOCKET s, ServerManager *server_mgr, unsigned long *ip_addr, SSL_CTX *ssl_context);
	void KillConnection(bool AbortSSL = false);

	void    RunThread();

	void	SSL_ConnectionThread();
	void	SSL_ThreadLockCheck(u32 tick);

	bool	IsActive();

	long	InactivityTime()		{ return m_InactivityTimer;  }
	void	SetInactivityTime(long secs)	{ m_MaxInactivityTime = secs; }

	long	GameID()			{ return m_AvatarID; } //for <MemorySlot> template access
	void	SetGameID(long id)		{ m_Mutex.Lock(); m_AvatarID = id; m_Mutex.Unlock(); }
	u32	CheckLastThreadTime()		{ return m_LastThreadComms; }
//	void	TerminateSSLThread(); -- OBSOLETE

private:
	bool	SocketReady(int ttimeout);
	void	WakeupThread();
	
	char  * GetResponse(char *request, size_t *response_length);
	char  *	HttpResult(size_t *response_length, char *data, char *content_type="text/html", size_t length=0);
	char  *	AuthLogin(size_t *response_length, char *recv_buffer);
	char  *	TouchSession(size_t *response_length, char *recv_buffer);
	char  *	SectorServer(size_t *response_length, char *recv_buffer);

#ifdef WIN32
	static UINT WINAPI SSL_Connection::SSLRecvThread(void *param);
#else
	static void	*SSLRecvThread(void *param);
#endif

	bool		OpenSSL_Link();

private:
	Mutex		m_Mutex;
	SOCKET		m_Socket;
	long		m_IpAddress;
	ServerManager	*m_ServerMgr;
#ifdef WIN32
	HANDLE		m_RecvThreadHandle;
#else
	pthread_t	m_RecvThreadHandle;
	pthread_cond_t	m_RecvThreadCond;
	pthread_mutex_t	m_RecvThreadMtx;		// Note: this is NOT m_Mutex!
#endif
	bool		m_SslConnectionThreadRunning;
	bool		m_SslConnectionThreadTerminated;
	bool		m_ConnectionActive;
	bool		m_ServerShutdown;
	char		m_Buffer[MAX_SERVER_FILE_SIZE];
	char		m_Recv_Buffer[SSL_RECV_BUFFER];
	u32		m_LastThreadComms;
	long		m_AvatarID;
	SSL		*m_SSL;

	long		m_InactivityTimer;		// Inactivity on this connection, in seconds
	long		m_MaxInactivityTime;		// Maximum inactivity on this connection (0 is Infinite)

};

#endif // _SSL_CONNECTION_H_INCLUDED_
