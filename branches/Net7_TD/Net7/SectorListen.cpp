// SectorListen.cpp

#include "Net7.h"
#include "SectorListen.h"
#include "SectorConnection.h"

SectorListen::SectorListen(unsigned int ip_address, unsigned short port)
:   TcpListen(ip_address, port)
{
}

SectorListen::~SectorListen()
{
}

TcpConnection *SectorListen::CreateConnection(SOCKET sock, unsigned short port)
{
    return new SectorConnection(sock, port);
}
