// SectorConnection.cpp

#include "SectorConnection.h"
#include "Opcodes.h"
#include "PacketStructures.h"

SectorConnection::SectorConnection(SOCKET sock, unsigned short port)
: ClientConnection(sock, port)
{
}

SectorConnection::~SectorConnection()
{
}

void SectorConnection::ProcessOpcode(short opcode, short bytes)
{

}
