// MasterServerToSectorServer.cpp

/******************************************************
 *   //////////////////////////////////////////////   *
 *   //  MASTER SERVER TO SECTOR SERVER OPCODES  //   *
 *   //////////////////////////////////////////////   *
 ******************************************************/

#include "Net7.h"
#include "Connection.h"
#include "Opcodes.h"
#include "ServerManager.h"
#include "PacketStructures.h"

void Connection::ProcessMasterServerToSectorServerOpcode(short opcode, short bytes)
{
	switch (opcode)
	{
	case ENB_OPCODE_7902_CHARACTER_DATA :
		HandleCharacterData(bytes);
		break;

	case ENB_OPCODE_7802_REQUEST_CHARACTER_DATA :
		HandleRequestCharacterData();
		break;

	default :
		LogMessage("ProcessMasterServerToSectorServerOpcode -- UNRECOGNIZED OPCODE 0x%04x\n", opcode);
		break;
	}
}

void Connection::SendSectorAssignment(long sector_id)
{
	SendResponse(ENB_OPCODE_7801_SECTOR_ASSIGNMENT, (unsigned char *) &sector_id, sizeof(sector_id));
}

void Connection::HandleRequestCharacterData()
{
	//LogMessage("Received DatabaseRequest packet\n");

	if (g_ServerMgr->m_IsMasterServer || g_ServerMgr->m_IsStandaloneServer)
	{
		SendCharacterData();
	}
}

void Connection::SendCharacterData()
{
	long response = 0;

	//LogMessage("Sending DatabaseResponse packet\n");

	SendResponse(ENB_OPCODE_7902_CHARACTER_DATA, (unsigned char *) &response, sizeof(response));
}

void Connection::HandleCharacterData(short bytes)
{
	// Are we a Sector Server?
	if (g_ServerMgr->m_IsStandaloneServer || !g_ServerMgr->m_IsMasterServer)
	{
		// Yes, store the character data
		if (bytes == sizeof(CharacterDatabase))
		{
			CharacterDatabase * database = (CharacterDatabase *) m_RecvBuffer;
			//g_ServerMgr->StoreCharacterData(*database);
		}
	}
}

