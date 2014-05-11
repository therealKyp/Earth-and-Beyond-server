// SslConnection.h

#ifndef _SSL_CONNECTION_H_INCLUDED_
#define _SSL_CONNECTION_H_INCLUDED_

#include "Mutex.h"
#include "TcpConnection.h"
#include <openssl/ssl.h>

// WARNING: This limits the size of images on the secure web pages.
#define MAX_SERVER_FILE_SIZE    16384
#define SSL_RECV_BUFFER_SIZE    2048
#define USERNAME_TAG            "username="
#define PASSWORD_TAG            "password="
#define PORT_TAG                "port="
#define MAX_SECTORS_TAG         "max_sectors="
#define VERSION_TAG             "version="
#define FORUM_NAME_TAG          "forumname="
#define EMAIL_TAG               "email="
#define LKEY_TAG                "lkey="

class SslConnection
{
public:
    SslConnection(SOCKET sock, long ip_address);
    virtual ~SslConnection();
    
    void Shutdown();
    bool IsActive();
    bool Initialize();
    void Loop();
    void RunThread();

private:
/*
	char *GetResponse(char *request, size_t *response_length);
	char *HttpResult(size_t *response_length, char *data, char *content_type="text/html", size_t length=0);
	char *AuthLogin(size_t *response_length, char *recv_buffer);
	char *TouchSession(size_t *response_length, char *recv_buffer);
	char *SectorServer(size_t *response_length, char *recv_buffer);
*/
	bool GetResponse();
	bool HttpResult(char *content_type="text/html");
	bool AuthLogin();
	bool TouchSession();
	bool SectorServer();

private:
    Mutex m_Mutex;
    SOCKET m_Socket;
	long m_IpAddress;
    bool m_Running;
	bool m_Active;
    char m_SendBuffer[MAX_SERVER_FILE_SIZE];
    size_t m_SendBufferLength;
    char m_EarlyBuffer[MAX_SERVER_FILE_SIZE];
    size_t m_EarlyBufferLength;
    char m_RecvBuffer[SSL_RECV_BUFFER_SIZE];
#ifndef WIN32
    pthread_t m_Thread;
#endif
    
    SSL *m_Ssl;
    SSL_CTX *m_SslContext;
};

#endif // _SSL_CONNETION_H_INCLUDED_
