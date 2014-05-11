// TcpListen.cpp

#include "Net7.h"
#include "TcpListen.h"

// This helper function is referenced by _beginthread to launch the TCP Listener thread.
#ifdef WIN32

void __cdecl RunTcpListenerThread(void *arg)
{
    ((TcpListen *) arg)->RunThread();
	_endthread();
}

#else // Linux

void *RunTcpListenerThread(void *arg)
{
    ((TcpListen *) arg)->RunThread();
    return NULL;
}

#endif

TcpListen::TcpListen(unsigned int ip_address, unsigned short port)
: m_IpAddress(ip_address), m_Port(port)
{
	// Thread is not running (yet)
	m_Running = false;

#ifdef WIN32
    // Winsock startup
    WSADATA	wsaData = {NULL};
	WSAStartup(MAKEWORD(2, 0), &wsaData);
#endif

	// Socket not opened (yet)
    m_Socket = INVALID_SOCKET;

    // Launch the Listener thread
#ifdef WIN32
    _beginthread(&RunTcpListenerThread, 0, this);
#else
    pthread_create(&m_Thread, NULL, &RunTcpListenerThread, (void *) this);
#endif
}

TcpListen::~TcpListen()
{
	// Shut down the listener thread
	Shutdown();

	// Allow the listener thread to die
	Sleep(1);

#ifdef WIN32
    // Winsock cleanup
    WSACleanup();
#endif
}

void TcpListen::Shutdown()
{
    m_Mutex.Lock();
    
    m_Running = false;
    
    // close the Listener socket
    if (m_Socket != INVALID_SOCKET)
    {
        closesocket(m_Socket);
        m_Socket = INVALID_SOCKET;
    }
    
    m_Mutex.Unlock();
}

void TcpListen::RunThread()
{
    if(!Initialize())
    {
        Shutdown();
        return;
    }
    
    m_Running = true;
    
    Loop();
}

bool TcpListen::Initialize()
{
    m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(m_Socket == INVALID_SOCKET)
    {
        g_LogError.Print("Failed to create a TCP listening socket.");
        return false;
    }
    
    struct sockaddr_in name;
    memset(&name, 0, sizeof(name));
    name.sin_family = AF_INET;    //name.sin_addr.s_addr = m_IpAddress;
    name.sin_addr.s_addr = INADDR_ANY;
    name.sin_port = htons(m_Port);
    
	int ret;
	while((ret = bind(m_Socket, (struct sockaddr *)&name, sizeof(name))))
    {
        static int tries = 0;
        tries++;
        
        if(ret == EACCES)
        {
            g_LogError.Print("Failed to bind to port %u because of insufficient privileges.", m_Port);
            return false;
        }
        else if(tries >= MAX_TRIES)
        {
            int error = errno;
            g_LogError.Print("Failed to bind to port %u after %d tries. %s. Aborting.", m_Port, strerror(error), tries);
            return false;
        }
        else
        {
            int error = errno;
            g_LogError.Print("Failed to bind to port %u. %s. Retrying in %d seconds...", m_Port, strerror(error), RETRY_SECONDS);
            Sleep(RETRY_SECONDS * 1000);
        }
    }
    
	if (listen(m_Socket, SOMAXCONN))
    {
        int error = errno;
        g_LogError.Print("Failed to listen on port %u. %s", m_Port, strerror(error));
        return false;
    }
    
    return true;
}

void TcpListen::Loop()
{
    struct sockaddr_in from;
    memset(&from, 0, sizeof(from));
#ifdef WIN32
    int from_length;
#else
    socklen_t from_length;
#endif
    SOCKET from_socket;
    
    while(m_Running && (m_Socket != INVALID_SOCKET))
    {
        from_length = sizeof(from);
        
        from_socket = accept(m_Socket, (sockaddr *)&from, &from_length);
        
        if(from_socket != INVALID_SOCKET)
        {
            m_Mutex.Lock();
            
            // Received successful connection... Now to deal with them.
            //TcpConnection *connection = CreateConnection(from_socket, m_Port);
            
            // Now we need to throw it in the list of connections to be checked.
            
            m_Mutex.Unlock();
        }
        else
        {
            int error = errno;
            g_LogDebug.Print("Accept failed on port %u. %s.", m_Port, strerror(error)); 
        }
        
        Sleep(10);
    }
    
    m_Running = false;
    
    g_LogError.Print("Listener thread for port %u has exited.", m_Port);
}
