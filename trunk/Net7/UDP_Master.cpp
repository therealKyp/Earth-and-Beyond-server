#include "Net7.h"

#include "ServerManager.h"
#include "UDPConnection.h"
#include "Opcodes.h"
#include "PacketStructures.h"
#include "PacketMethods.h"
#include "PlayerClass.h"
#include "MemoryHandler.h"

#define ENB_OPCODE_2008_MASTER_HANDOFF                  0x2008
#define ENB_OPCODE_2009_MASTER_HANDOFF_CONFIRM          0x2009


void UDP_Connection::HandleMasterOpcode(char *msg, EnbUdpHeader *hdr, const long source_addr, const short source_port)
{
    switch (hdr->opcode)
    {
    case ENB_OPCODE_2008_MASTER_HANDOFF:
        ProcessHandoff(msg, hdr, source_addr, source_port);
        break;

    default:					
        LogMessage("bad Master UDP opcode, id 0x%04X\n",hdr->opcode);
        break;
    }   
}

void UDP_Connection::ProcessHandoff(char *msg, EnbUdpHeader *hdr, const long source_addr, const short source_port)
{
    unsigned char data[32];
    int index = 0;

    long sector_id = *((long*) msg);

    LogMessage("Master handoff received from player id %x, to sector = %d\n", hdr->player_id, sector_id);

    Player *player = m_ServerMgr->m_PlayerMgr.GetPlayer(hdr->player_id);

    if (player)
	{
        ServerRedirect redirect;
        memset(&redirect, 0, sizeof(redirect));
        redirect.sector_id = ntohl(sector_id);
        LogMessage("Processing player '%s', sector %d\n", player->Name(), sector_id);
        if (m_ServerMgr->m_SectorServerMgr.LookupSectorServer(redirect))
        {
            LogMessage("Found sector %d\n",sector_id);
            //build response, ip addr of sector and sector port
            AddData(data, redirect.ip_address, index);
            AddData(data, redirect.port, index);
			long game_id = player->GameID();
			AddData(data, game_id, index);
            SendOpcode(ENB_OPCODE_2009_MASTER_HANDOFF_CONFIRM, player, data, index, source_addr, source_port);
            player->SetUDPConnection(g_ServerMgr->m_UDPConnection);
            player->SetHandoffReceived(true);
        }
        else
        {
            LogMessage("Unable to locate sector server for sector %d\n", sector_id);
        }
    }
    else
    {
        LogMessage("SERVER ERROR: Unable to find player [%x]\n", hdr->player_id);
    }
}

