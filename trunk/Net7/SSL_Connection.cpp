// SSL_Connection.cpp
//
// Test URL: https://local.net-7.org/certificate.html
// Test URL: https://local.net-7.org/shutdown.cgi
// Test URL: https://local.net-7.org/sectorserver.cgi?username=VectoR&port=3500&max_sectors=20&version=0.2
// Test URL: https://local.net-7.org/AuthLogin?username=VectoR&password=go&serviceID=2184&version=2.5
//
// TODO: Add code for subsxml to return XML data from resource files
// TODO: Fix all the HTTP(s) responses.


#include "Net7.h"
#include "SSL_Connection.h"
#include "ServerManager.h"
#include "PacketStructures.h"
#include "mysql/mysqlplus.h"

SSL_Connection::SSL_Connection()
{
	// Initialize parameters
	m_SslConnectionThreadRunning	= false;
	m_SslConnectionThreadTerminated	= false;
	m_ConnectionActive				= false;

	m_Socket 	= INVALID_SOCKET;
	m_SSL		= NULL;
	m_ServerMgr	= 0;
	m_IpAddress	= 0;
	m_AvatarID	= 0;

	m_LastThreadComms	= 0;
	m_ServerShutdown	= false;

#ifdef WIN32
	UINT uiThreadId = 0;
	m_RecvThreadHandle = (HANDLE)_beginthreadex(NULL, 1024, SSLRecvThread, this, 0, &uiThreadId);
#else
	pthread_mutex_init(&m_RecvThreadMtx, NULL);
	pthread_cond_init(&m_RecvThreadCond, NULL);
	pthread_create(&m_RecvThreadHandle, NULL, SSLRecvThread, this);
#endif

	if (!m_RecvThreadHandle) LogMessage("Unable to create a new SSL receive thread\n");
}

SSL_Connection * SSL_Connection::ReSetSSL_Connection(SOCKET s, ServerManager *server_mgr, unsigned long *ip_addr, SSL_CTX *ssl_context)
{
	if (!m_RecvThreadHandle) // Just as a safeguard
	{
		LogMessage("!Restarting SSL Connection receive thread\n");
#ifdef WIN32
		UINT uiThreadId = 0;
		m_RecvThreadHandle = (HANDLE)_beginthreadex(NULL, 1024, SSLRecvThread, this, 0, &uiThreadId);
#else
		pthread_mutex_init(&m_RecvThreadMtx, NULL);
		pthread_cond_init(&m_RecvThreadCond, NULL);
		pthread_create(&m_RecvThreadHandle, NULL, SSLRecvThread, this);
#endif
	}

	m_Mutex.Lock();

	m_LastThreadComms		= GetNet7TickCount();
	m_ConnectionActive		= true;
	m_ServerShutdown		= false;
	m_AvatarID			= 1;

	m_Socket			= s;
	m_ServerMgr			= server_mgr;
	if (ip_addr) m_IpAddress 	= *ip_addr;
	
	m_Mutex.Unlock();

	// Set SSL Parameters
	m_SSL = SSL_new(ssl_context);
	if (!m_SSL)
	{
		LogMessage("Unable to reanimate SSL receive thread: SSL_new failed\n");
		m_ConnectionActive = false;
		return this;
	}

	SSL_set_fd(m_SSL, m_Socket);	

	LogMessage("Reanimating SSL receive thread.\n");

	WakeupThread();

	return this;
}

SSL_Connection::~SSL_Connection()
{
	LogMessage("SSL Connection Destroyed\n");

	KillConnection();

	// Let thread know it is terminated
	if (!m_SslConnectionThreadTerminated)
	{
		m_SslConnectionThreadTerminated = true;
		WakeupThread();

		m_Mutex.Lock();
		Sleep(1);
		m_Mutex.Unlock();
	}
}

bool SSL_Connection::IsActive()
{
	// Good opportunity to check g_ServerShutdown
	if (g_ServerShutdown) WakeupThread();

	return m_ConnectionActive;
}

void SSL_Connection::WakeupThread()
{
	m_SslConnectionThreadRunning = true;

#ifndef WIN32
	pthread_cond_signal(&m_RecvThreadCond);
#else
	ResumeThread(m_RecvThreadHandle);
#endif
}

bool SSL_Connection::OpenSSL_Link()
{
	int ret;
	int error;

	ret = SSL_accept(m_SSL);
	Sleep(1);

	if ( (ret <= 0) && (m_SSL) )
	{
		error = SSL_get_error(m_SSL, ret);
		if (error == SSL_ERROR_SYSCALL)	error = WSAGetLastError();

		LogMessage("SSL_accept failed, error = %d\n", error);
		return false;
	}
	else if (!m_SSL)
	{
		//LogMessage("SSL_accept failed. Connection was aborted by ThreadLockCheck().\n");
		return false;
	}

	return true;
}

void SSL_Connection::SSL_ConnectionThread()
{
	int bytes;
	int error;
	size_t response_length;
	char * response;

	while (!g_ServerShutdown && !m_SslConnectionThreadTerminated)
	{
		while (m_SslConnectionThreadRunning)
		{
			// NOTE: OpenSSL_Link() has been moved here from ReSetSSL_Connection because
			// otherwise the SSL_ThreadLockCheck never gets called while still in SSL_accept (blocking, remember?)
			if (!OpenSSL_Link())
			{
				m_SslConnectionThreadRunning = false;
				break;		
			}

			bytes = 0;
			m_LastThreadComms = GetNet7TickCount();

			if (m_SSL)
			{
				bytes = SSL_read(m_SSL, m_Recv_Buffer, SSL_RECV_BUFFER - 1);
			}
			if (bytes < 0)
			{
				error = SSL_get_error(m_SSL, bytes);
				if (error == SSL_ERROR_SYSCALL)	error = WSAGetLastError();

				if ( (error != SSL_ERROR_ZERO_RETURN) && (error != WSAECONNRESET) && (error != 1) )
				{
					LogMessage("SSL_Connection SSL_read failed, error=%d\n", error);
				}
			}
			else if (bytes > 0)
			{
				if (bytes > SSL_RECV_BUFFER - 1) bytes = SSL_RECV_BUFFER - 1;
				m_Recv_Buffer[bytes] = '\0';
				response = GetResponse(m_Recv_Buffer, &response_length);

				if (response && m_SSL)
				{
					SSL_write(m_SSL, response, response_length);
					delete [] response;

					if (m_ServerShutdown)
					{
						Sleep(200);
						g_ServerShutdown = true;
					}
				}
			}

			m_SslConnectionThreadRunning = false;
		}

		KillConnection();

		// m_SslConnectionThreadRunning set to false. Go to sleep... Zzz...
#ifdef WIN32
		SuspendThread(m_RecvThreadHandle);
#else
		pthread_cond_wait(&m_RecvThreadCond, &m_RecvThreadMtx);
#endif
	}

	m_SslConnectionThreadTerminated = true;

	// Do any special thread cleanup here
}

char *SSL_Connection::GetResponse(char *recv_buffer, size_t *response_length)
{
	//printf("%s\n", recv_buffer);
	char *response = NULL;
	*response_length = 0;
	char text[256];
	bool UserTaken = false;

	if (strstr(recv_buffer, "certificate.html"))
	{
	        sprintf(text, 
			"<html>\r\n"
			"<head>\r\n"
			"<META HTTP-EQUIV=\"Pragma\" CONTENT=\"no-cache\">\r\n"
			"</head>\r\n"
			"<body>\r\n"
			"<h3><tt>%s certificate successfully installed!</tt></h3>\r\n"
			"<h2>Please close the browser to continue<h2>\r\n"
			"</body>\r\n"
			"</html>\r\n",
			g_DomainName);

		response = HttpResult(response_length, text);
	}
	else if (strstr(recv_buffer, "NewPlayer.cgi"))
	{
		char filename[MAX_PATH];
		sprintf(filename, "%sNewPlayer.cgi.txt", SERVER_LOGS_PATH);
		FILE *f = fopen(filename, "a");
		if (f)
		{
			fprintf(f, "%s", recv_buffer);
			fclose(f);
		}
        // TODO: Process the form submitted by the player
        char *data = strstr(recv_buffer, "\r\n\r\n");
        if (data)
        {
            data += 4;
            char *username  = strtok(data, "&\r\n ");
            char *password  = strtok(NULL, "&\r\n ");
            char *forumname = strtok(NULL, "&\r\n ");
            char *email     = strtok(NULL, "&\r\n ");
            if (username && password && forumname  && email)
            {
                if ((strncmp(username, USERNAME_TAG, strlen(USERNAME_TAG)) == 0) &&
                    (strncmp(password, PASSWORD_TAG, strlen(PASSWORD_TAG)) == 0) &&
                    (strncmp(forumname, FORUM_NAME_TAG, strlen(FORUM_NAME_TAG)) == 0) &&
                    (strncmp(email, EMAIL_TAG, strlen(EMAIL_TAG)) == 0))
                {
                    username += strlen(USERNAME_TAG);
                    password += strlen(PASSWORD_TAG);
                    forumname += strlen(FORUM_NAME_TAG);
                    email += strlen(EMAIL_TAG);

#ifdef ADD_USERS
					m_ServerMgr->m_AccountMgr.AddUser(username, password, "U");				
#endif

                    // TODO: urldecode the strings
                    // TODO: verify that the fields are not blank
                    // TODO: verify that the fields do not contain invalid characters
                    // TODO: verify that the email address is valid format
                    // TODO: verify that the forum name exists on www.enb-emulator.com

                    // TODO: Log the IP ADDRESS!
                    // TODO: take out mutex lock on account manager
                    // TODO: Verify that the IP ADDRESS is not on the banned list
                    // TODO: verify that the forum name has no more than 2 accounts
                    // TODO: see if this username already exists in accounts list or pending list
                    // TODO: add account name to the pending list with a date stamp
                    // TODO: release mutex lock on account manager
                    // TODO: send an email to the user
                    // TODO: return a valid HTML page telling the player to check their email account

					if (UserTaken)
						strcpy(recv_buffer, "GET /UsernameTaken.html HTTP/1.1");
					else
						strcpy(recv_buffer, "GET /PendingAccountEmailSent.html HTTP/1.1");
                }
            }
        }
	}
    else if (strstr(recv_buffer, "/who.cgi"))
    {
        response = m_ServerMgr->m_PlayerMgr.WhoHtml(response_length);
    }
    else if (strstr(recv_buffer, "/AuthLogin"))
	{
		response = AuthLogin(response_length, recv_buffer);
	}
	else if (strstr(recv_buffer, "/subsxml/subsxml.jsp"))
	{
        char filename[MAX_PATH];
        sprintf(filename, "%shttps_subsxml.txt", SERVER_LOGS_PATH);
		FILE *f = fopen(filename, "w");
		if (f)
		{
			fprintf(f, "%s", recv_buffer);
			fclose(f);
		}
	}
	else if (strstr(recv_buffer, "/touchsession.jsp"))
	{
		response = TouchSession(response_length, recv_buffer);
	}
	else if (strstr(recv_buffer, "/sectorserver.cgi"))
	{
		response = SectorServer(response_length, recv_buffer);
	}
/*
#ifdef _DEBUG
	else if (strstr(recv_buffer, "shutdown.cgi"))
	{
        printf("Shutting down server!\n");
		m_ServerShutdown = true;

		char *text =
			"<html>\r\n"
            "<head>\r\n"
            "<META HTTP-EQUIV=\"Pragma\" CONTENT=\"no-cache\">\r\n"
            "</head>\r\n"
			"<body>\r\n"
			"<h3><tt>Server shutdown in progress!</tt></h3>\r\n"
			"</body>\r\n"
			"</html>\r\n";

		response = HttpResult(response_length, text);
	}
#endif
*/

	if (!response)
    {
        // Throw away everything after the first line
        strtok(recv_buffer, "\r\n");
        char *p = strstr(recv_buffer, " HTTP/");
        if (p)
        {
            // throw away the tail end of the line
            *p = 0;

            // This does not appear to be handled above.  See if we can parse the filename
            if (strncmp(recv_buffer, "GET /", 5) == 0)
	        {
                p = recv_buffer + 5;
                if (p[0] == 0)
                {
                    p = "redirect.html";
                }
                if ((strlen(recv_buffer) + strlen(SERVER_HTML_PATH)) < MAX_PATH - 1)
                {
                    // "NewPlayer.html" length=14
                    // ext = p+8
                    int length = strlen(p);
                    char *ext3 = p + length - 4;
                    char *ext4 = p + length - 5;
                    bool legal = false;
                    char *mime_type = NULL;
                    char *open_type = NULL;

                    // This is a list of legal file extensions for our "web server"
                    if (strcmp(ext3, ".css") == 0)
                    {
                        legal = true;
                        mime_type = "text/plain";
                        open_type = "r";
                    }
                    else if (strcmp(ext4, ".html") == 0)
                    {
                        legal = true;
                        mime_type = "text/html";
                        open_type = "r";
                    }
                    else if (strcmp(ext3, ".gif") == 0)
                    {
                        legal = true;
                        mime_type = "image/gif";
                        open_type = "rb";
                    }

                    if (legal && mime_type && open_type)
                    {
                        // Pretend to be a web server and see if a file exists using the specified name
                        char filename[MAX_PATH];
                        sprintf(filename, "%s%s", SERVER_HTML_PATH, p);
                        FILE *f = fopen(filename, open_type);
                        if (f)
                        {
                            fseek(f,0,SEEK_END);
                            size_t length = ftell(f);
                            if ((length > 0) && (length < MAX_SERVER_FILE_SIZE))
                            {
                                fseek(f,0,SEEK_SET);
                                size_t actual = fread(m_Buffer, 1, length, f);
                                if (actual > 0)
                                {
                                    response = HttpResult(response_length, m_Buffer, mime_type, actual);
                                }
                            }
                            fclose(f);
                        }
                    }
                }
	        }
        }

        // If all else fails, dump the request to a file
        if (!response)
	    {
            char filename[MAX_PATH];
            sprintf(filename, "%shttps_unrecognized.txt", SERVER_LOGS_PATH);
		    FILE *f = fopen(filename, "a");
		    if (f)
		    {
                // Append the request to the log file
			    fprintf(f, "%s\n", recv_buffer);
			    fclose(f);
		    }
	    }
    }

	if (!response)
	{
		// TODO: send 404 response
		static const char * error_404 =
            "HTTP/1.1 404 File Not Found\r\n"
            "Server: AuthServer/2.5\r\n"
            "Keep-Alive: timeout=15, max=100\r\n"
            "Connection: Keep-Alive\r\n"
            "Content-Length: 2\r\n"
            "Content-type: text/plain\r\n"
            "\r\n"
            "\r\n";
        response = new char[strlen(error_404) + 1];
		strcpy(response, error_404);
	}

	return response;
}

char *SSL_Connection::TouchSession(size_t *response_length, char *recv_buffer)
{
	char * response = NULL;
	char * lkey = strstr(recv_buffer, LKEY_TAG);

	if (lkey)
	{
		lkey += strlen(LKEY_TAG);
		strtok(lkey, "&\r\n ");

		char *info = "HTTP/1.1 200 \r\n"
			"Server: AuthServer/2.5\r\n"
			"Content-Type: text/plain; charset=ISO-8859-1\r\n"
			"Transfer-Encoding: chunked\r\n"
			"\r\n"
			"7\r\n"
			"Success\r\n"
			"0\r\n"
			"\r\n";

		response = new char[strlen(info) + 1];
		strcpy(response, info);

		//printf("TouchSession Response:\n%s", response);
		*response_length = strlen(response);
	}

	return response;
}

char *SSL_Connection::AuthLogin(size_t *response_length, char *recv_buffer)
{
	char info[128] = "Valid=False\r\n";

	/*
		Here is an example of what the client sends to the AuthServer:

		GET /AuthLogin?username=VectoR&password=go&serviceID=2184&version=2.5 HTTP/1.1
		User-Agent: AuthLogin
		Host: local.net-7.org
		Connection: Keep-Alive
		Cache-Control: no-cache
	*/

	char * username = strstr(recv_buffer, USERNAME_TAG);
	char * password = strstr(recv_buffer, PASSWORD_TAG);

	if (username && password)
	{
		char * ticket = 0;

		username += strlen(USERNAME_TAG);
		password += strlen(PASSWORD_TAG);

		strtok(username, "& ");
		strtok(password, "& ");

		unsigned char *ip = (unsigned char *)&m_IpAddress;
		    LogMessage("User '%s' logging in from IP: %u.%u.%u.%u\n", username, ip[0], ip[1], ip[2], ip[3]);

		// TODO: This is a good place to check for "banned" IP addresses.

		if (ticket = g_AccountMgr->IssueTicket(username, password))
		{
			sprintf(info, "Valid=TRUE\r\nTicket=%s\r\n", ticket);
		}
	}

	return HttpResult(response_length, info, "text/plain");
}

char *SSL_Connection::HttpResult(size_t *response_length, char *data, char *content_type, size_t length)
{
	if (length == 0)
	{
		length = strlen(data);
	}

	char header[256];
	sprintf(header,
		"HTTP/1.1 200 OK\r\n"
		"Content-Type: %s\r\n"
		"Server: AuthServer/2.5\r\n"
		"Content-Length: %d\r\n"
		"\r\n",
		content_type,
		length);

	size_t header_length = strlen(header);
	*response_length = header_length + length;

	char * response = new char[header_length + length + 1];

	if (response)
	{
		strcpy(response, header);
		memcpy(response + header_length, data, length);
		response[header_length + length] = 0;
	}

	return response;
}

char *SSL_Connection::SectorServer(size_t *response_length, char *recv_buffer)
{
	// The Sector Server registers with the Global Server via HTTPS
	// The Global Server then turns around and connects to the
	// Sector Server via the specified port (usually 3500).
	char info[256] = "Success=FALSE\r\n";

	/*
		Here is an example of what the Sector Server sends to the AuthServer:

		GET /sectorserver.cgi?username=VectoR&port=3500&max_sectors=20&version=0.2 HTTP/1.1
		User-Agent: AuthLogin
		Host: local.net-7.org
		Connection: Keep-Alive
		Cache-Control: no-cache
	*/

	//printf("Received SSL request to register a Sector Server\n");

	char * username = strstr(recv_buffer, USERNAME_TAG);
	char * port = strstr(recv_buffer, PORT_TAG);
	char * max_sectors = strstr(recv_buffer, MAX_SECTORS_TAG);
	char * version = strstr(recv_buffer, VERSION_TAG);

	if (username && port && max_sectors && version)
	{
		username += strlen(USERNAME_TAG);
		port += strlen(PORT_TAG);
		max_sectors += strlen(MAX_SECTORS_TAG);
		version += strlen(VERSION_TAG);

		strtok(username, "& ");
		strtok(port, "& ");
		strtok(max_sectors, "& ");
		strtok(version, "& ");

		//printf("username='%s'\n", username);
		//printf("port='%s'\n", port);
		//printf("version='%s'\n", version);

		char expected_version[16];
		sprintf(expected_version, "%d.%d", SECTOR_SERVER_MAJOR_VERSION, SECTOR_SERVER_MINOR_VERSION);

		// The version number must match to 
		if (strcmp(version, expected_version) == 0)
		{
			int port_number = atoi(port);
			int num_sectors = atoi(max_sectors);

			if ((port_number >= 3500) && (max_sectors > 0))
			{
				char temp[80];
				sprintf(temp, "%d", port_number);
				if (strcmp(temp, port) == 0)
				{
					if (m_ServerMgr->m_SectorServerMgr.RegisterSectorServer(m_IpAddress, port_number, num_sectors, username))
					{
						strcpy(info, "Success=TRUE\r\n");
					}
					else
					{
						strcat(info, "The specified Sector Server IP Address and port are not pre-registered\r\n");
						strcat(info, "Visit https://");
						strcat(info, g_DomainName);
						strcat(info, "/NewSectorServer.html to register\r\n");
					}
				}
				else
				{
					strcat(info, "Invalid port number\r\n");
				}
			}
			else
			{
				strcat(info, "Port number must be 3500 or above\r\n");
			}
		}
		else
		{
			strcat(info, "Expected Sector Server version is ");
			strcat(info, expected_version);
			strcat(info, "\r\n");
		}
	}
	else
	{
		strcat(info, "Invalid parameters\r\n");
	}

	return HttpResult(response_length, info, "text/plain");
}

#ifdef WIN32
UINT WINAPI SSL_Connection::SSLRecvThread(void *param)
#else
void* SSL_Connection::SSLRecvThread(void *param)
#endif
{
	SSL_Connection* p_this = reinterpret_cast<SSL_Connection*>( param );

	p_this->SSL_ConnectionThread();
#ifdef WIN32
	return 1;
#else
	return NULL;
#endif
}

// Kills an active connection on the Socket, if any, and suspends the thread
void SSL_Connection::KillConnection(bool AbortSSL)
{
	m_Mutex.Lock();

	if ( m_Socket != INVALID_SOCKET )
	{
		LogMessage("Closing SSL Connection.\n");

		if (m_SSL)
		{
			if (!AbortSSL) SSL_shutdown(m_SSL);		// Do a proper shutdown if we're not aborting
			SSL_clear(m_SSL);
		}

		shutdown(m_Socket, 2);		// This is a MUST, otherwise SSL_accept just sits there...
		closesocket(m_Socket);
		m_Socket = INVALID_SOCKET;
	}

	if (m_SSL && m_SSL->version == TLS1_VERSION)
	{
		SSL_free(m_SSL);
	}
	m_SSL = NULL;

	// No longer active, not running
	m_ConnectionActive				= false;
	m_SslConnectionThreadRunning	= false;
	m_AvatarID						= 0;

	m_Mutex.Unlock();
}
