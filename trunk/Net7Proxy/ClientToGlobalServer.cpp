// ClientToGlobalServer.cpp

/***********************************************
 *   ///////////////////////////////////////   *
 *   //  CLIENT TO GLOBAL SERVER OPCODES  //   *
 *   ///////////////////////////////////////   *
 ***********************************************/

#include "Net7.h"
#include "Connection.h"
#include "Opcodes.h"
#include "ServerManager.h"
#include "PacketStructures.h"
#include "PacketMethods.h"
#include "UDPClient.h"

void Connection::GlobalError(int Error)
{
	static char _ErrorMsg[][200] = 
	{
		{"Error: You have been temporarily banned."},
		{"Sorry, that name has already been taken. Please try again."},
		{"Sorry, your name can only contain the letters a-z. No spaces or other special characters are allowed.  Please try again."},
		{"Sorry, this name is too short. It must contain at least 3 characters. Please try again."},
		{"Sorry, this name needs enough vowels (a,e,i,o,u & y) to be pronouncable. Please try again."},
		{"Sorry, there are too many repeating characters in this name. Please try again."},
		{"Sorry, this name contains a reserved or illegal word. Please try again."},
		{"Error: Ticket Validation Failed."},
		{"Error: Authentication Server (AUTHD) is unavailable.  Try again in a few minutes."},
		{"Error: You have not compleated registration."},
		{"Sorry, that ship name is not allowed. Please try again"},
		{"Sorry, Net7 experienced an internal error. Please submit a bug report"},
	};

    //TODO: Recode this to use new packet builder methods
	char Buffer[1024];
	char *p = Buffer;

	*((int *) p) = strlen(_ErrorMsg[Error]); p+= 4;
	*((int *) p) = ntohl(Error+7); p+= 4;

	memcpy(p, _ErrorMsg[Error], strlen(_ErrorMsg[Error]));
	p+=strlen(_ErrorMsg[Error]);

	SendResponse(ENB_OPCODE_0075_GLOBAL_ERROR, (unsigned char *) &Buffer, (p - Buffer));

	LogDebug("GlobalError: %s\n", _ErrorMsg[Error]);
}


void Connection::ProcessGlobalServerOpcode(short opcode, short bytes)
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
        //LogMessage("GlobalServer received MasterJoin packet from client\n");
        break;

    case ENB_OPCODE_006D_GLOBAL_CONNECT :
        HandleGlobalConnect();
        break;

    case ENB_OPCODE_006E_GLOBAL_TICKET_REQUEST :
        fprintf(stderr,"ticket RQ\n");
        HandleGlobalTicketRequest();
        break;

    case ENB_OPCODE_0071_GLOBAL_DELETE_CHARACTER :
        fprintf(stderr,"delete character\n");
        HandleDeleteCharacter();
        break;

    case ENB_OPCODE_0072_GLOBAL_CREATE_CHARACTER :
        fprintf(stderr,"Create character 2\n");
        HandleCreateCharacter();
        break;

    default:
        LogMessage("ProcessGlobalServerOpcode -- UNRECOGNIZED OPCODE 0x%04x\n", opcode);
        break;
    }
}

void Connection::HandleVersionRequest()
{
    VersionRequest * request = (VersionRequest *) m_RecvBuffer;

    long major = ntohl(request->Major);
    long minor = ntohl(request->Minor);

    if (major == 42 && minor == 0)
    {
        // Version is good
        SendVersionResponse(0);
    }
    else if (major < 42)
    {
        // The client version is old, need to apply the patch
        SendVersionResponse(1);
    }
    else
    {
        // The client version is newer than this server.
        SendVersionResponse(2);
    }
}

void Connection::SendVersionResponse(long status)
{
    SendResponse(ENB_OPCODE_0001_VERSION_RESPONSE, (unsigned char *) &status, sizeof(status));
}

//This gets invoked when a user logs into the account AFTER being validated
void Connection::HandleGlobalConnect()
{
    char *ticket = (char *) &m_RecvBuffer[4];

    //transmit ticket to server
    char *avatar_list = g_ServerMgr->m_UDPConnection->SendTicket(ticket);
    //server should now reply with either a 'Global Error' or an Avatar List'

	if (avatar_list)
	{
		m_AccountUsername = strtok(ticket, "-");
		g_ServerMgr->m_UDPConnection->SetAccountName(m_AccountUsername);
		memcpy(&m_Player_Avatar_List, avatar_list, sizeof(GlobalAvatarList));
        SendResponse(ENB_OPCODE_0070_GLOBAL_AVATAR_LIST, (unsigned char *) avatar_list, sizeof(GlobalAvatarList));
    }
}

void Connection::HandleDeleteCharacter()
{
    long character_slot = ntohl(*(long *) m_RecvBuffer);
    char *avatar_list = g_ServerMgr->m_UDPConnection->DeleteCharacter(character_slot);
    
    if (avatar_list)
    {
		memcpy(&m_Player_Avatar_List, avatar_list, sizeof(GlobalAvatarList));
        SendResponse(ENB_OPCODE_0070_GLOBAL_AVATAR_LIST, (unsigned char *) avatar_list, sizeof(GlobalAvatarList));
    }
}

void Connection::HandleCreateCharacter()
{
    GlobalCreateCharacter *create= (GlobalCreateCharacter *) m_RecvBuffer;

    char *avatar_list = g_ServerMgr->m_UDPConnection->CreateCharacter(create);

    memcpy(&m_Player_Avatar_List, avatar_list, sizeof(GlobalAvatarList));

    if (avatar_list)
    {
        SendResponse(ENB_OPCODE_0070_GLOBAL_AVATAR_LIST, (unsigned char *) avatar_list, sizeof(GlobalAvatarList));
    }

    //now initialise the character data

}

//This sends the client the avatar list after: Loggin in, deleting character, creating character
void Connection::SendAvatarList(long account_id)
{
    //GlobalAvatarList avatar_list;
    //g_AccountMgr->BuildAvatarList(&avatar_list, account_id);

    //SendResponse(ENB_OPCODE_0070_GLOBAL_AVATAR_LIST, (unsigned char *) &avatar_list, sizeof(GlobalAvatarList));
}

//Occurs when the player selects a charachter
void Connection::HandleGlobalTicketRequest()
{
    // The player selected a character
    long char_slot = ntohl(*(long *) m_RecvBuffer);
    //OK, broadcast ticket request, get back player ID (from the UDP header), and the avatar_id
    long avatar_id = g_ServerMgr->m_UDPConnection->SendAvatarLogin(char_slot);

    if (avatar_id == -1)
    {
        LogMessage("GlobalTicketRequest() Error obtaining global character slot id\n");
        SendGlobalTicket(0x40000000, 0, 1002, false); //galaxy full
        return;
    }

    LogMessage("Received GlobalTicketRequest packet -- User: `%s`, Slot: %d\n", m_AccountUsername, char_slot);

    ProcessGlobalTicket(char_slot);

	//LogMessage("PlayerID: %d[%s] status is: %c\n", slot_id, m_ServerMgr.m_AccountMgr->GetAccountUsername(slot_id), m_ServerMgr.m_AccountMgr->GetAccountStatus(slot_id));
    
    if (0)
    {
		LogMessage("** Critical Error: Can't get AvatarID [%d]! **\n", avatar_id);
        SendGlobalTicket(0x40000000, 0, 1002, false); //galaxy full
    }
}

void Connection::SendGlobalTicket(long avatar_id, long sector_id, long level, bool issue)
{
    GlobalTicket ticket;
    memset(&ticket, 0, sizeof(ticket));
    unsigned char *ptr_ticket = (unsigned char *)&ticket;
    int index = 0;

    // 1000 = 'Error: AvatarSelectionTask-User not authorized'
    //        'Error: You have not been authorized to play this character.'
    // 1002 = 'Error: AvatarSelectionTask-Shard is full'
    //        'Error: This galaxy is full, please select another character.'

    if (issue)
    {
        AddDataFlip4(ptr_ticket, 0, index);
    }
    else
    {
        AddDataFlip4(ptr_ticket, level, index);
    }

    index = 20;

    AddDataFlip4(ptr_ticket, avatar_id, index);
    AddDataFlip4(ptr_ticket, sector_id, index);
    index = 32;
    AddData(ptr_ticket, level, index);
    index = 48;
    AddDataS(ptr_ticket, "MY_Avatar_Ticket", index);
    
    SendResponse(ENB_OPCODE_006F_GLOBAL_TICKET, (unsigned char *) &ticket, sizeof(ticket));
}

//TODO: Recode this
void Connection::ProcessGlobalTicket(long char_slot)
{
    //is this a new char?
    //if ()
    //request sectorID and admin level.
    m_SectorID = ntohl(m_Player_Avatar_List.avatar[char_slot].info.sector_id);
    long admin_level = ntohl(m_Player_Avatar_List.avatar[char_slot].info.admin_level);
    LogMessage("Sending Global Ticket (Proxy->Client): 0x%08x , %d, %d\n", g_ServerMgr->m_UDPConnection->PlayerID(), m_SectorID, admin_level);
    SendGlobalTicket(g_ServerMgr->m_UDPConnection->PlayerID(), m_SectorID, admin_level, true);
}

void Connection::ProcessProxyGlobalOpcode(short opcode, short bytes)
{
	switch (opcode)
    {
	case ENB_OPCODE_2001_ACCOUNTVALIDATE:
		LogMessage("Received account valid from Server\n");
		HandleAccountValid(bytes);
		break;
	case ENB_OPCODE_0070_GLOBAL_AVATAR_LIST:
		LogMessage("Received TCP avatarlist from Server\n");
		HandleAvatarList(bytes);
		break;
	case ENB_OPCODE_006F_GLOBAL_TICKET:
		LogMessage("Received TCP Global Ticket for login\n");
		HandleAccountValid(bytes);
		break;
	case ENB_OPCODE_2009_MASTER_HANDOFF_CONFIRM:
		HandleAccountValid(bytes);
		break;
	default:
		LogMessage("Illegal opcode from Server to Global Account connection\n");
		break;
	}
}

void Connection::HandleAccountValid(short bytes)
{
	m_UDPClient->ValidAccount(m_RecvBuffer, bytes);
}

void Connection::HandleAvatarList(short bytes)
{
	m_UDPClient->ProcessAvatarList(m_RecvBuffer, bytes);
}