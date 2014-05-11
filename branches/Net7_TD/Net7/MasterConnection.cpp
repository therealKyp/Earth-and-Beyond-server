// MasterConnection.cpp

#include "MasterConnection.h"

MasterConnection::MasterConnection(SOCKET sock, unsigned short port)
: ClientConnection(sock, port)
{
}

MasterConnection::~MasterConnection()
{
}

void MasterConnection::ProcessOpcode(short opcode, short bytes)
{
	switch (opcode)
	{
	    case ENB_OPCODE_0035_MASTER_JOIN:
		    HandleMasterJoin();
		    break;

	    default:
		    g_LogDebug.Print("ProcessMasterServerOpcode -- UNRECOGNIZED OPCODE 0x%.4X\n", opcode);
		    break;
	}
}

void MasterConnection::HandleMasterJoin()
{
	// The player's is displaying the inter-sector wait screen
	MasterJoin *join = (MasterJoin *)m_Buffer;

    long sector_id = ntohl(join->ToSectorID);
	g_LogDebug.Print("Received MasterJoin packet, ToSectorID = %d\n", sector_id);

	// Redirect the client to the appropriate sector server
	//SendServerRedirect(sector_id);
}

void MasterConnection::SendServerRedirect(long sector_id)
{
	// Redirect the client to the correct Sector Server!!!
	ServerRedirect redirect;

    memset(&redirect, 0, sizeof(redirect));
	redirect.sector_id = ntohl(sector_id);
/*
	if (m_ServerMgr.m_SectorServerMgr.LookupSectorServer(redirect))
	{
		redirect.ip_address = ntohl(redirect.ip_address);
		g_LogDebug.Print("Sending ServerRedirect packet, SectorID = %d\n", sector_id);
		SendResponse(ENB_OPCODE_0036_SERVER_REDIRECT, (unsigned char *) &redirect, sizeof(redirect));
	}
	else */
	{
		g_LogError.Print("Unable to redirect client to a working server\n");
		//m_OwningServer.Error();
	}

}
