// SslConnection.cpp
//
// Test URL: https://local.net-7.org/certificate.html
// Test URL: https://local.net-7.org/shutdown.cgi
// Test URL: https://local.net-7.org/sectorserver.cgi?username=VectoR&port=3500&max_sectors=20&version=0.2
// Test URL: https://local.net-7.org/AuthLogin?username=VectoR&password=go&serviceID=2184&version=2.5
//
// TODO: Add code for subsxml to return XML data from resource files
//
// T3hD0gg: NOT SURE IF THIS WORKS. It was a very terrible hack.
//          If it doesn't, I'll reimplement it later.

#include "Net7.h"
#include "SslConnection.h"
#include <openssl/ssl.h>

// This helper function is referenced by _beginthread to launch the SSL Connection thread.
#ifdef WIN32

void __cdecl RunSslConnectionThread(void *arg)
{
    ((SslConnection *)arg)->RunThread();
    _endthread();
}

#else // Linux

void * RunSslConnectionThread(void *arg)
{
    ((SslConnection *)arg)->RunThread();
    return NULL;
}

#endif

SslConnection::SslConnection(SOCKET sock, long ip_address)
: m_Socket(sock), m_IpAddress(ip_address)
{
	// Mark this as an active connection
	m_Active = true;

    // Thread is not running (yet)
    m_Running = false;

    // Launch the Listener thread
#ifdef WIN32
    _beginthread(&RunSslConnectionThread, 0, this);
#else
    pthread_create(&m_Thread, NULL, &RunSslConnectionThread, (void *) this);
#endif
}

SslConnection::~SslConnection()
{
    Shutdown();

	// Allow the thread to die
	int limit = 100;
	while (m_Running)
	{
		Sleep(20);
		if (--limit < 0)
			break;
	}
	
    SSL_free(m_Ssl);
    SSL_CTX_free(m_SslContext);
}

void SslConnection::Shutdown()
{
    m_Mutex.Lock();
    
    m_Active = false;
    m_Running = false;

    if (m_Socket != INVALID_SOCKET)
    {
        closesocket(m_Socket);
        m_Socket = INVALID_SOCKET;
    }

    m_Mutex.Unlock();
}

bool SslConnection::IsActive()
{
    return m_Active;
}

void SslConnection::RunThread()
{
    if(!Initialize())
    {
        m_Active = false;
        return;
    }

    Loop();
}

bool SslConnection::Initialize()
{
	SSL_load_error_strings();
	SSLeay_add_ssl_algorithms();
	
	m_SslContext = SSL_CTX_new(SSLv23_server_method());
	if (!m_SslContext)
	{
		g_LogError.Print("SSL_CTX_new failed.");
		return false;
	}

    char certf[40];
    char keyf[40];
    sprintf(certf, "%s.cer", NET7_DOMAIN_NAME);
    sprintf(keyf, "%s.pem", NET7_DOMAIN_NAME);

	if (SSL_CTX_use_certificate_file(m_SslContext, certf, SSL_FILETYPE_PEM) <= 0)
	{
		g_LogError.Print("SSL_CTX_use_certificate_file failed.");
		return false;
	}

	if (SSL_CTX_use_PrivateKey_file(m_SslContext, keyf, SSL_FILETYPE_PEM) <= 0)
	{
		g_LogError.Print("SSL_CTX_use_PrivateKey_file failed.");
		return false;
	}

	if (!SSL_CTX_check_private_key(m_SslContext))
	{
		g_LogError.Print("Private key does not match the certificate public key.");
		return false;
	}

	m_Ssl = SSL_new(m_SslContext);
	if (!m_Ssl)
	{
		g_LogError.Print("SSL_new failed.");
		return false;
	}

	SSL_set_fd(m_Ssl, m_Socket);

	if (SSL_accept(m_Ssl) == -1)
	{
		g_LogError.Print("SSL_accept failed.");
		return false;
	}
	
	return true;
}

void SslConnection::Loop()
{
    m_Running = true;
    
	while(true)
    {
        Sleep(1);
        
        // Will this be too resource intensive? Doesn't look like it will be...
        m_Mutex.Lock();
        
        if(!m_Running)
            break;      
        
        m_Mutex.Unlock();

		//printf("SSL_Connection calling SSL_read\n");
		int bytes = SSL_read(m_Ssl, m_RecvBuffer, SSL_RECV_BUFFER_SIZE - 1);
		
		if (bytes < 0)
		{
		    int error = SSL_get_error(m_Ssl, bytes);
            if (error == SSL_ERROR_SYSCALL)
            {
                error = WSAGetLastError();
            }
			if ((error != SSL_ERROR_ZERO_RETURN) &&
                (error != WSAECONNRESET))
			{
				g_LogError.Print("SSL_Connection SSL_read failed, error=%d", error);
			}
			// terminate the connection
			Shutdown();
		}
		else if (bytes == 0)
		{
			//printf("SSL_Connection closed gracefully\n");
			// terminate the connection
			Shutdown();
		}
		else if (bytes > 0)
		{
			//printf("SSL_Connection SSL_read returned %d\n", bytes);
			m_RecvBuffer[bytes] = '\0';

			bool response = GetResponse();
			if (response)
			{
				//printf("SSL_Connection calling SSL_write\n");
				int ret = SSL_write(m_Ssl, m_SendBuffer, m_SendBufferLength);
				if (ret < 0)
				{
					int error = SSL_get_error(m_Ssl, ret);
					g_LogError.Print("SSL_Connection SSL_write failed, error=%d", error);
					// terminate the connection
					Shutdown();
				}
				//delete [] response;
			}
		}
	}
}

bool SslConnection::GetResponse()
{
    m_SendBufferLength = 0;
    bool response;
    
    //char text[256];

	if (strstr(m_RecvBuffer, "certificate.html"))
	{
        sprintf(m_SendBuffer, 
			"<html>\r\n"
            "<head>\r\n"
            "<META HTTP-EQUIV=\"Pragma\" CONTENT=\"no-cache\">\r\n"
            "</head>\r\n"
			"<body>\r\n"
			"<h3><tt>%s certificate successfully installed!</tt></h3>\r\n"
            "<h2>Please close the browser to continue<h2>\r\n"
			"</body>\r\n"
			"</html>\r\n",
            NET7_DOMAIN_NAME);

		response = HttpResult();
	}
	else if (strstr(m_RecvBuffer, "NewPlayer.cgi"))
	{
        char filename[MAX_PATH];
        sprintf(filename, "%sNewPlayer.cgi.txt", SERVER_LOGS_PATH);
		FILE *f = fopen(filename, "w");
		if (f)
		{
			fprintf(f, "%s", m_RecvBuffer);
			fclose(f);
		}
        // TODO: Process the form submitted by the player
        char *data = strstr(m_RecvBuffer, "\r\n\r\n");
        if (data)
        {
            data += 4;
            char *username = strtok(data, "&\r\n ");
            char *password = strtok(NULL, "&\r\n ");
            char *forumname = strtok(NULL, "&\r\n ");
            char *email = strtok(NULL, "&\r\n ");
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

                    strcpy(m_RecvBuffer, "GET /PendingAccountEmailSent.html HTTP/1.1");
                }
            }
        }
	} 
	else if (strstr(m_RecvBuffer, "/AuthLogin"))
	{
		response = AuthLogin();
	}
	else if (strstr(m_RecvBuffer, "/subsxml/subsxml.jsp"))
	{
        char filename[MAX_PATH];
        sprintf(filename, "%shttps_subsxml.txt", SERVER_LOGS_PATH);
		FILE *f = fopen(filename, "w");
		if (f)
		{
			fprintf(f, "%s", m_RecvBuffer);
			fclose(f);
		}
	}
	else if (strstr(m_RecvBuffer, "/touchsession.jsp"))
	{
		response = TouchSession();
	}
	else if (strstr(m_RecvBuffer, "/sectorserver.cgi"))
	{
		response = SectorServer();
	}

	if (!response)
    {
        // Throw away everything after the first line
        strtok(m_RecvBuffer, "\r\n");
        char *p = strstr(m_RecvBuffer, " HTTP/");
        if (p)
        {
            // throw away the tail end of the line
            *p = 0;

            // This does not appear to be handled above. See if we can parse the filename
            if (strncmp(m_RecvBuffer, "GET /", 5) == 0)
	        {
                p = m_RecvBuffer + 5;
                if (p[0] == 0)
                {
                    p = "redirect.html";
                }
                if ((strlen(m_RecvBuffer) + strlen(SERVER_HTML_PATH)) < MAX_PATH - 1)
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
                                size_t m_EarlyBufferLength = fread(m_EarlyBuffer, 1, length, f);
                                if (m_EarlyBufferLength > 0)
                                {
                                    response = HttpResult(mime_type);
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
			    fprintf(f, "%s\n", m_RecvBuffer);
			    fclose(f);
		    }
	    }
    }

	if (!response)
	{
		// TODO: send 404 response
		static const char *error_404 =
            "HTTP/1.1 404 File Not Found\r\n"
            "Server: AuthServer/2.5\r\n"
            "Keep-Alive: timeout=15, max=100\r\n"
            "Connection: Keep-Alive\r\n"
            "Content-Length: 2\r\n"
            "Content-type: text/plain\r\n"
            "\r\n"
            "\r\n";

		strcpy(m_SendBuffer, error_404);
		response = true;
	}

	return response;
}

bool SslConnection::TouchSession()
{
	char *lkey = strstr(m_RecvBuffer, LKEY_TAG);
	
    if (lkey)
    {
        lkey += strlen(LKEY_TAG);
	    strtok(lkey, "&\r\n ");

	    static const char *info =
		    "HTTP/1.1 200 \r\n"
		    "Server: AuthServer/2.5\r\n"
		    "Content-Type: text/plain; charset=ISO-8859-1\r\n"
		    "Transfer-Encoding: chunked\r\n"
		    "\r\n"
		    "7\r\n"
		    "Success\r\n"
		    "0\r\n"
		    "\r\n";

	    strcpy(m_SendBuffer, info);

	    //printf("TouchSession Response:\n%s", response);
        m_SendBufferLength = strlen(m_SendBuffer);
        
        return true;
    }

	return false;
}

bool SslConnection::AuthLogin()
{
	//char info[1024] = "Valid=False\r\n";

	/*
		Here is an example of what the client sends to the AuthServer:

		GET /AuthLogin?username=VectoR&password=go&serviceID=2184&version=2.5 HTTP/1.1
		User-Agent: AuthLogin
		Host: local.net-7.org
		Connection: Keep-Alive
		Cache-Control: no-cache
	*/

	char *username = strstr(m_RecvBuffer, USERNAME_TAG);
	char *password = strstr(m_RecvBuffer, PASSWORD_TAG);

	if (username && password)
	{
		username += strlen(USERNAME_TAG);
		password += strlen(PASSWORD_TAG);

		strtok(username, "& ");
		strtok(password, "& ");

		//printf("username='%s'\n", username);
		//printf("password='%s'\n", password);
		
        /* FIX THIS */
		char *ticket = NULL; // = m_ServerMgr.m_AccountMgr.IssueTicket(username, password);
		if (ticket)
		{
			sprintf(m_EarlyBuffer, "Valid=TRUE\r\n"
						  "Ticket=%s\r\n",
						  ticket);
			delete [] ticket;
			ticket = NULL;
		}
	}

	return HttpResult("text/plain");
}

bool SslConnection::HttpResult(char *content_type)
{
    if (m_EarlyBufferLength == 0)
        m_EarlyBufferLength = strlen(m_EarlyBuffer);

	sprintf(m_SendBuffer,
		"HTTP/1.1 200 OK\r\n"
		"Content-Type: %s\r\n"
		"Server: AuthServer/2.5\r\n"
		"Content-Length: %d\r\n"
		"\r\n",
		content_type,
		m_EarlyBufferLength);

    //printf("%s", buffer);
    size_t header_length = strlen(m_SendBuffer);
    m_SendBufferLength = header_length + m_EarlyBufferLength;

    //char *response = new char[header_length + length + 1];

    //if (response)
    //{
	    //strcpy(response, buffer);
        memcpy(m_SendBuffer + header_length, m_EarlyBuffer, m_EarlyBufferLength);
        m_SendBuffer[header_length + m_EarlyBufferLength] = '\0';
    //}

	return true;
}

bool SslConnection::SectorServer()
{
	// The Sector Server registers with the Global Server via HTTPS
	// The Global Server then turns around and connects to the
	// Sector Server via the specified port (usually 3500).
	//char info[256] = "Success=FALSE\r\n";
	strcpy(m_EarlyBuffer, "Success=FALSE\r\n");

	/*
		Here is an example of what the Sector Server sends to the AuthServer:

		GET /sectorserver.cgi?username=VectoR&port=3500&max_sectors=20&version=0.2 HTTP/1.1
		User-Agent: AuthLogin
		Host: local.net-7.org
		Connection: Keep-Alive
		Cache-Control: no-cache
	*/
	//printf("Received SSL request to register a Sector Server\n");

	char *username = strstr(m_EarlyBuffer, USERNAME_TAG);
	char *port = strstr(m_EarlyBuffer, PORT_TAG);
	char *max_sectors = strstr(m_EarlyBuffer, MAX_SECTORS_TAG);
	char *version = strstr(m_EarlyBuffer, VERSION_TAG);

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
					if (true)//m_ServerMgr.m_SectorServerMgr.RegisterSectorServer(m_IpAddress, port_number, num_sectors, username))
					{
						strcpy(m_EarlyBuffer, "Success=TRUE\r\n");
					}
					else
					{
						strcat(m_EarlyBuffer, "The specified Sector Server IP Address and port are not pre-registered.\r\n");
						strcat(m_EarlyBuffer, "Visit https://");
                        strcat(m_EarlyBuffer, NET7_DOMAIN_NAME);
                        strcat(m_EarlyBuffer, "/NewSectorServer.html to register.\r\n");
					}
				}
				else
				{
					strcat(m_EarlyBuffer, "Invalid port number.\r\n");
				}
			}
			else
			{
				strcat(m_EarlyBuffer, "Port number must be 3500 or above.\r\n");
			}
		}
		else
		{
			strcat(m_EarlyBuffer, "Expected Sector Server version is ");
			strcat(m_EarlyBuffer, expected_version);
			strcat(m_EarlyBuffer, "\r\n");
		}
	}
	else
	{
		strcat(m_EarlyBuffer, "Invalid parameters.\r\n");
	}

	return HttpResult("text/plain");
}
