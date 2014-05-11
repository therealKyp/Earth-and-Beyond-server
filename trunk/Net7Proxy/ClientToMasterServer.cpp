// ClientToMasterServer.cpp

/*************************************
 *   /////////////////////////////   *
 *   //  MASTER SERVER OPCODES  //   *
 *   /////////////////////////////   *
 *************************************/

#include "Net7.h"
#include "Connection.h"
#include "Opcodes.h"
#include "ServerManager.h"
#include "PacketStructures.h"
#include "UDPClient.h"

void Connection::ProcessMasterServerOpcode(short opcode, short bytes)
{
	switch (opcode)
	{
	case ENB_OPCODE_0035_MASTER_JOIN :
		HandleMasterJoin();
		break;

	default :
		LogMessage("ProcessMasterServerOpcode -- UNRECOGNIZED OPCODE 0x%04x\n", opcode);
		break;
	}
}

void Connection::HandleMasterJoin()
{
	// The player's client is displaying the inter-sector wait screen
	MasterJoin * join = (MasterJoin *) m_RecvBuffer;
	long sector_id = ntohl(join->ToSectorID);

	m_AvatarID = ntohl(join->avatar_id_lsb);
	//LogMessage("<client> Received MasterJoin packet, ToSectorID = %d [%d] [%d]\n", sector_id, m_AvatarID, ntohl(join->avatar_id_msb));

	g_LoggedIn = true;

    long sector_ipaddr;
    short sector_port;

    g_ServerMgr->m_MasterConnection = this;

    sector_port = g_ServerMgr->m_UDPConnection->SendMasterLogin(m_AvatarID, sector_id, &sector_ipaddr);

	if (sector_port == -1)
	{
		::MessageBox(NULL, "Server Failed to respond to sector login", "Net7Proxy", MB_ICONERROR);
		LogMessage(">> CRITICAL ERROR: Server unable to process sector login\n");
		ShutdownClient();
	}
	else
	{
		LogMessage("<server> Master Login received - UDP sector port: %d\n", sector_port);
		g_ServerMgr->m_UDPConnection->SetClientPort(sector_port);
		g_ServerMgr->m_UDPConnection->SetClientIP(sector_ipaddr);
		g_ServerMgr->m_UDPConnection->SetSectorID(sector_id);
		g_ServerMgr->m_UDPClient->SetSectorID(sector_id);

		// Redirect the client to the appropriate sector server
		SendServerRedirect(sector_id);

		//start login thread here
		g_ServerMgr->m_UDPClient->StartLoginTimer();
	}
}

void Connection::SendServerRedirect(long sector_id)
{
	// Redirect the client to the correct Sector Server!!!
	ServerRedirect redirect;

    memset(&redirect, 0, sizeof(redirect));
	redirect.sector_id = ntohl(sector_id);
	redirect.ip_address = ntohl(m_ServerMgr.m_IpAddress);
    redirect.port = SECTOR_SERVER_PORT;

    //LogMessage("<proxy> Master Server sending ServerRedirect packet, SectorID = %d\n", sector_id);
	SendResponse(ENB_OPCODE_0036_SERVER_REDIRECT, (unsigned char *) &redirect, sizeof(redirect));
}