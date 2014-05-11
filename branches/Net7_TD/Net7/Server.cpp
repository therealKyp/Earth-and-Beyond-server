// Server.cpp

#include "Net7.h"
#include "Server.h"

Server::Server(unsigned int ip_address, const char *name)
: m_IpAddress(ip_address)
{
    strncpy(m_ServerName, name, sizeof(m_ServerName));
}

Server::~Server()
{

}

void Server::Error(const char *error)
{
    char message[256];
    
    snprintf(message, sizeof(message), "ERROR(%s): %s", m_ServerName, error);
    
    g_LogError.Print(message);
}
