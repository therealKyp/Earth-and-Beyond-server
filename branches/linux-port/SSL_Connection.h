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

class ServerManager;

class SSL_Connection
{
public:
    SSL_Connection(SOCKET s, ServerManager& server_mgr, long ip_address);
    virtual ~SSL_Connection();

public:
    void    RunThread();
	bool	IsActive();
	void	SSL_ConnectionCheck();

private:
	char  * GetResponse(char *request, size_t *response_length);
	char  *	HttpResult(size_t *response_length, char *data, const char *content_type="text/html", size_t length=0);
	char  *	AuthLogin(size_t *response_length, char *recv_buffer);
	char  *	TouchSession(size_t *response_length, char *recv_buffer);
	char  *	SectorServer(size_t *response_length, char *recv_buffer);

	bool	OpenSSL_Link();

private:
    Mutex   m_Mutex;
    SOCKET  m_Socket;
	long	m_IpAddress;
	ServerManager &m_ServerMgr;
    bool    m_SslConnectionThreadRunning;
	bool	m_ConnectionActive;
	bool	m_ServerShutdown;
    char    m_Buffer[MAX_SERVER_FILE_SIZE];
	SSL	  *	m_SSL;
	SSL_CTX *m_ssl_context;
#ifndef WIN32
    pthread_t m_Thread;
#endif
};

#endif // _SSL_CONNECTION_H_INCLUDED_
