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
#include "PlayerClass.h"

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
	// The player's is displaying the inter-sector wait screen
	MasterJoin * join = (MasterJoin *) m_RecvBuffer;
	long sector_id = ntohl(join->ToSectorID);

	//m_AvatarID = ntohl(join->avatar_id_lsb);
	//LogMessage("Received MasterJoin packet, ToSectorID = %d\n", sector_id);


	// Redirect the client to the appropriate sector server
	SendServerRedirect(sector_id);
}

void Connection::SendServerRedirect(long sector_id)
{
	// Redirect the client to the correct Sector Server
	ServerRedirect redirect;

    memset(&redirect, 0, sizeof(redirect));
	redirect.sector_id = ntohl(sector_id);


	if (g_ServerMgr->m_SectorServerMgr.LookupSectorServer(redirect))
	{
		if (g_ServerMgr->m_IpAddressInternal > 0)
		{
			redirect.ip_address = ntohl(g_ServerMgr->m_IpAddress);
		} 
        else 
        {
			redirect.ip_address = ntohl(redirect.ip_address);
		}
		LogMessage("Master Server sending ServerRedirect packet, SectorID = %d\n", sector_id);
		SendResponse(ENB_OPCODE_0036_SERVER_REDIRECT, (unsigned char *) &redirect, sizeof(redirect));
	}
	else
	{
		LogMessage("Unable to redirect client to a working server\n");
		// TODO: GlobalError();
	}
}

