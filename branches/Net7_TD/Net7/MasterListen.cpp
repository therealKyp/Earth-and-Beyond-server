// MasterListen.cpp

#include "Net7.h"
#include "MasterListen.h"
#include "MasterConnection.h"

MasterListen::MasterListen(unsigned int ip_address, unsigned short port)
:   TcpListen(ip_address, port)
{
}

MasterListen::~MasterListen()
{
}

TcpConnection *MasterListen::CreateConnection(SOCKET sock, unsigned short port)
{
    return new MasterConnection(sock, port);
}
