// SectorServerToSectorServer.cpp

/******************************************************
 *   //////////////////////////////////////////////   *
 *   //  SECTOR SERVER TO SECTOR SERVER OPCODES  //   *
 *   //////////////////////////////////////////////   *
 ******************************************************/

#include "Net7.h"
#include "Connection.h"
#include "Opcodes.h"
#include "ServerManager.h"
#include "PacketStructures.h"

void Connection::ProcessSectorServerToSectorServerOpcode(short opcode, short bytes)
{
	switch (opcode)
	{
	case ENB_OPCODE_7802_REQUEST_CHARACTER_DATA :
		SendCharacterData();
		break;

	default :
		LogMessage("ProcessSectorServerToSectorServerOpcode - UNRECOGNIZED OPCODE 0x%04x\n", opcode);
		break;
	}
}

