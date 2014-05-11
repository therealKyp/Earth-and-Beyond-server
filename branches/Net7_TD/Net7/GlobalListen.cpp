// GlobalListen.cpp

#include "Net7.h"
#include "GlobalListen.h"
#include "GlobalConnection.h"

GlobalListen::GlobalListen(unsigned int ip_address, unsigned short port)
:   TcpListen(ip_address, port)
{
}

GlobalListen::~GlobalListen()
{
}

TcpConnection *GlobalListen::CreateConnection(SOCKET sock, unsigned short port)
{
    return new GlobalConnection(sock, port);
}
