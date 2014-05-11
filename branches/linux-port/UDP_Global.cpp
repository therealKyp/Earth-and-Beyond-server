#include "Net7.h"

#include "ServerManager.h"
#include "UDPConnection.h"
#include "Opcodes.h"
#include "PacketStructures.h"
#include "PacketMethods.h"
#include "PlayerClass.h"
#include "MemoryHandler.h"

#define G_ERROR_BANNED_ACCOUNT		0
#define G_ERROR_NICKNAME_USED		1
#define	G_ERROR_INVALID_CHARS		2
#define	G_ERROR_TOO_SHORT			3
#define	G_ERROR_ONE_VOWEL			4
#define	G_ERROR_REPEATING_CHAR		5
#define	G_ERROR_RESTRICTED_LIST		6
#define	G_ERROR_TICKET_INVALID		7
#define	G_ERROR_AUTH_SERVER_DOWN	8
#define G_ERROR_INACTIVE_ACCOUNT	9
#define	G_ERROR_RESTRICTED_SHIP		10
#define	G_ERROR_NET7_INTERNAL		11

void UDP_Connection::HandleGlobalOpcode(char *msg, EnbUdpHeader *hdr, const long source_addr, const short source_port)
{
    switch (hdr->opcode)
    {
    case ENB_OPCODE_2000_ACCOUNTDATA:
        LogMessage("UDP AccountData received\n");
        VerifyAccountInfo(msg, hdr, source_addr, source_port);
        break;

    case ENB_OPCODE_2002_TICKET:
        LogMessage("Received ticket!\n");
        ProcessTicketInfo(msg, hdr, source_addr, source_port);
        break;

    case ENB_OPCODE_2004_AVATARLOGIN:
        LogMessage("Received AvatarLogin confirm (start player)\n");
        HandleGlobalTicketRequest(msg, hdr, source_addr, source_port);
        break;

    case ENB_OPCODE_200B_CREATE_AVATAR:
        LogMessage("Received Create Avatar packet\n");
        HandleAvatarCreateRequest(msg, hdr, source_addr, source_port);
        break;

    case ENB_OPCODE_200D_DELETE_AVATAR:
        LogMessage("Received Delete Avatar packet\n");
        HandleAvatarDeleteRequest(msg, hdr, source_addr, source_port);
        break;

    default:
        LogMessage("bad G-UDP opcode, id 0x%04X\n",hdr->opcode);
        break;
    }
}

bool UDP_Connection::VerifyAccountInfo(char *msg, EnbUdpHeader *hdr, const long source_addr, const short source_port)
{
    char username[64];
    char password[64];
    int index = 0;
    char info[128] = "Valid=False\r\n";
    long length = 13;

    ExtractDataLS((unsigned char*)msg, username, index);
    ExtractDataLS((unsigned char*)msg, password, index);

    LogMessage("Account info received: %s %s\n", username, password);

	if (username && password)
	{
        char * ticket = 0;

        unsigned char *ip = (unsigned char *)&source_addr;
	    LogMessage("User '%s' logging in from IP: %u.%u.%u.%u\n", username, ip[0], ip[1], ip[2], ip[3]);

        // TODO: This is a good place to check for "banned" IP addresses.
        // TODO: we need to create a list of accounts which are logged in successfully, so Net7Proxy create/delete can't be abused

        if (ticket = g_AccountMgr->IssueTicket(username, password))
        {
		    length = sprintf(info, "Valid=TRUE\r\nTicket=%s\r\n", ticket);
            LogMessage("Valid ticket %s\n",ticket);
        }
	}

    //now transmit a 2001 validate account message
    SendOpcode(ENB_OPCODE_2001_ACCOUNTVALID, (unsigned char *) &info, length, source_addr, source_port);

    return true;
}

bool UDP_Connection::ProcessTicketInfo(char *msg, EnbUdpHeader *hdr, const long source_addr, const short source_port)
{
    //ok, we have a new ticket, lets validate the ticket and send the avatarlist
    char *ticket = (char *) &msg[0];

    //This is a pointer assign since GetUsernameFromTicket() uses the string manager
    char *account_name = g_ServerMgr->m_AccountMgr->GetUsernameFromTicket(ticket);

    fprintf(stderr,"We got accountname back from UDP: %s\n", account_name);

    if (!account_name)
    {
		LogDebug("HandleGlobalConnect() - Invalid Username\n");
		SendGlobalError(G_ERROR_NET7_INTERNAL, source_addr, source_port);
        return false;
    }

    long account_id = g_AccountMgr->GetAccountID(account_name);
	long account_status = g_AccountMgr->GetAccountStatus(account_name);

    if (account_id == -1)
    {
		LogDebug("HandleGlobalConnect() - Invalid AccountID\n");
		SendGlobalError(G_ERROR_NET7_INTERNAL, source_addr, source_port);
        return false;
    }

	if (account_status == -1)		// Banned
	{
		LogDebug("HandleGlobalConnect() - Banned Account\n");
		SendGlobalError(G_ERROR_BANNED_ACCOUNT, source_addr, source_port);
		return false;
	}

	if (account_status == -2)		// Complete Registration
	{
		LogDebug("HandleGlobalConnect() - Inactive Account\n");
		SendGlobalError(G_ERROR_INACTIVE_ACCOUNT, source_addr, source_port);
		return false;
	}

    SendAvatarList(g_AccountMgr->GetAccountID(account_name), source_addr, source_port);

    return true;
}

void UDP_Connection::SendGlobalError(int error, const long source_addr, const short source_port)
{
    SendOpcode(ENB_OPCODE_200E_GLOBAL_ERROR, (unsigned char *) &error, sizeof(int), source_addr, source_port);
}

//This sends the client the avatar list after: Loggin in, deleting character, creating character
void UDP_Connection::SendAvatarList(long account_id, const long source_addr, const short source_port)
{
    GlobalAvatarList avatar_list;
    g_AccountMgr->BuildAvatarList(&avatar_list, account_id);
    SendOpcode(ENB_OPCODE_2003_AVATARLIST, (unsigned char *) &avatar_list, sizeof(GlobalAvatarList), source_addr, source_port);
    fprintf(stderr,"Sent avatarlist (%ld)\n", sizeof(GlobalAvatarList));
}

void UDP_Connection::HandleGlobalTicketRequest(char *msg, EnbUdpHeader *hdr, const long source_addr, const short source_port)
{
    // The player selected a character
    long char_slot = *(long *) msg;
    char username[64];
    int index = 4;
    ExtractDataLS((unsigned char*)msg, username, index);
    long avatar_id = g_AccountMgr->GetAvatarID(username, char_slot);

    if (avatar_id == -1)
    {
        LogMessage("GlobalTicketRequest() Error obtaining global charachter slot id\n");
        return;
    }

    LogMessage("Received GlobalTicketRequest packet -- User: `%s`, Slot: %d\n", username, char_slot);

    //Now finally we can allocate a player slot, since the player is logging into the game
    Player *player = g_ServerMgr->m_GlobMemMgr->GetPlayerNode(username);

    if (player)
    {
        LogMessage("Allocated %s a playerID of 0x%08x [%d]\n", username, player->GameID(), player->CharacterID());

        player->SetCharacterID(avatar_id);
        player->SetCharacterSlot(char_slot);
        player->SetAccountUsername(username);

        g_PlayerMgr->SetupPlayer(player, source_addr);

        //Read the character database
        g_AccountMgr->ReadDatabase(player->Database(), avatar_id);
        player->SetName(player->Database()->avatar.avatar_first_name);

        //Now send the avatar_id and relay the new player's GameID to confirm login
        SendOpcode(ENB_OPCODE_2005_AVATARLOGIN_CONFIRM, player, (unsigned char *) &avatar_id, sizeof(long), source_addr, source_port);
    }
    else
    {
		LogMessage("** Critical Error: Can't get AvatarID [%d]! **\n", avatar_id);
        SendGlobalError(G_ERROR_NET7_INTERNAL, source_addr, source_port);
        //SendGlobalTicket(player->GameID(), 0, 1002, false); //galaxy full
    }
}

//this method pings the corresponding sector connection to activate it, and assign a sector id
void UDP_Connection::ValidateSectorServer(long sector_id, short port, long ip_address)
{
    //ping the sector server with its assigned sector
    //SendOpcode(ENB_OPCODE_2006_SECTOR_VALIDATE, (unsigned char *) &sector_id, sizeof(long), ip_address, port);
    if (m_ServerMgr->SetupSectorServer(sector_id))
    {
        m_ServerMgr->SetSectorServerReady(port, true);
        LogMessage("Port: %d, Sector: %d '%s'\n", port, sector_id, g_ServerMgr->GetSectorName(sector_id));
    }
	else
	{
		LogMessage("Error loading Sector: %d '%s'\n", sector_id, g_ServerMgr->GetSectorName(sector_id));
	}
}

void UDP_Connection::HandleAvatarCreateRequest(char *msg, EnbUdpHeader *hdr, const long source_addr, const short source_port)
{
    GlobalCreateCharacter *create = (GlobalCreateCharacter *) msg;

    char *username = create->account_username;//m_AccountUsername = g_StringMgr->GetStr(create->account_username);
    long ret_val = g_AccountMgr->CreateCharacter(create);

    if (ret_val != -1)
    {
        SendGlobalError(ret_val, source_addr, source_port);
        return;
    }

    LogDebug("New character `%s` created\n", create->avatar.avatar_first_name);

    SendAvatarList(g_AccountMgr->GetAccountID(username), source_addr, source_port);
}

void UDP_Connection::HandleAvatarDeleteRequest(char *msg, EnbUdpHeader *hdr, const long source_addr, const short source_port)
{
    long character_slot;
    char username[64];
    int index = 0;

    character_slot = ExtractLong((unsigned char*)msg, index);
    ExtractDataLS((unsigned char*)msg, username, index);

	long avatar_id = g_AccountMgr->GetAvatarID(username, character_slot);

	LogMessage("Delete character %d\n", avatar_id);

    g_AccountMgr->DeleteCharacter(avatar_id);

    SendAvatarList(g_AccountMgr->GetAccountID(username), source_addr, source_port);
}
