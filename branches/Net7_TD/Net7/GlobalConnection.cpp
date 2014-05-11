// GlobalConnection.cpp

#include "GlobalConnection.h"
#include "Opcodes.h"
#include "PacketStructures.h"

GlobalConnection::GlobalConnection(SOCKET sock, unsigned short port)
: ClientConnection(sock, port)
{
}

GlobalConnection::~GlobalConnection()
{
}

void GlobalConnection::ProcessOpcode(short opcode, short bytes)
{
    switch (opcode)
    {
    case ENB_OPCODE_0000_VERSION_REQUEST :
        HandleVersionRequest();
        break;

    case ENB_OPCODE_0035_MASTER_JOIN :
        // TODO: figure out why the client sometimes sends a Master Join
        // packet to the Global Server.  The Master Join packet should only
        // go to the Master Server.  Perhaps we should dump this oddball to
        // a data file so we can study it.
        g_LogDebug.Print("GlobalServer received MasterJoin packet from client\n");
        break;

    case ENB_OPCODE_006D_GLOBAL_CONNECT :
        HandleGlobalConnect();
        break;

    case ENB_OPCODE_006E_GLOBAL_TICKET_REQUEST :
        HandleGlobalTicketRequest();
        break;

    case ENB_OPCODE_0071_GLOBAL_DELETE_CHARACTER :
        HandleDeleteCharacter();
        break;

    case ENB_OPCODE_0072_GLOBAL_CREATE_CHARACTER :
        HandleCreateCharacter();
        break;

    default :
        g_LogDebug.Print("ProcessGlobalServerOpcode -- UNRECOGNIZED OPCODE 0x%04x\n", opcode);
        break;
    }
}

#define SEND_VERSION_GOOD 0
#define SEND_VERSION_OLD 1
#define SEND_VERSION_NEW 2

void GlobalConnection::HandleVersionRequest()
{
    VersionRequest *request = (VersionRequest *) m_Buffer;

    long major = ntohl(request->Major);
    long minor = ntohl(request->Minor);

    g_LogDebug.Print("Received VersionRequest packet -- client version is %d.%d", major, minor);

    if ((major == 42) &&
        (minor == 0))
    {
        // Version is good
        SendVersionResponse(SEND_VERSION_GOOD);
    }
    else if ((major < 42))
    {
        // The client version is old, need to apply the patch
        SendVersionResponse(SEND_VERSION_OLD);
    }
    else
    {
        // The client version is newer than this server.
        SendVersionResponse(SEND_VERSION_NEW);
    }
}

void GlobalConnection::SendVersionResponse(int status)
{
    g_LogDebug.Print("Sending VersionResponse packet.");

    SendResponse(ENB_OPCODE_0001_VERSION_RESPONSE, (unsigned char *) &status, sizeof(status));
}

void GlobalConnection::HandleGlobalConnect()
{

}

void GlobalConnection::HandleGlobalTicketRequest()
{

}

void GlobalConnection::HandleDeleteCharacter()
{

}

void GlobalConnection::HandleCreateCharacter()
{

}


