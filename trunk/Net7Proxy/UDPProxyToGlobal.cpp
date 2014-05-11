#include "Net7.h"
#include "UDPClient.h"
#include "opcodes.h"
#include "PacketStructures.h"
#include "PacketMethods.h"
#include "ServerManager.h"
#include "Connection.h"

void __cdecl LoginCheckThread(void *Param);
bool login_thread_running = false;
void __cdecl ShutdownThread(void *Param);

#if 1
//This function doesn't actually use UDP any more, but it keeps things
//simpler if we keep with this format
void UDPClient::SendAccount(char *username, char *password, char *info)
{
    fprintf(stderr,"\nSending username/password.\n");

    //build packet structure.
    unsigned char data[256];
    int index = 0;
    memset (data, 0, 256);
    m_ticket_length = 0;
    m_global_account_rcv = false;
    m_ticket = 0;

	if (EstablishGlobalConnection(m_IPAddr))
	{
		//data is size,username,size,password
		AddDataLS(data, username, index);
		AddDataLS(data, password, index);

		m_ServerTCP->SendResponse(ENB_OPCODE_2000_ACCOUNTDATA, data, index);

		WaitForResponse();
		
		if (m_global_account_rcv) 
		{
			LogMessage("Received account request.\n");
			memcpy(info, m_ticket, m_ticket_length);
			memcpy(m_AccountName, username, strlen(username));
		}
	}
	return;
}

void UDPClient::SetAccountName(char *name)
{
	if (name)
	{
		strcpy(m_AccountName, name);
	}
}

void UDPClient::ValidAccount(unsigned char *msg, short len)
{
	m_global_account_rcv = true;
	memcpy(m_RecvBuffer, msg, len);
	m_ticket = (char*)m_RecvBuffer;
	m_ticket_length = len;
}

void UDPClient::ProcessAvatarList(unsigned char *msg, short len)
{
	m_global_account_rcv = true;
	memcpy(m_RecvBuffer, msg, len);
	m_ticket = (char*)m_RecvBuffer;
	m_ticket_length = len;
}

void UDPClient::ValidateAccount(char *msg, EnbUdpHeader *header)
{
    m_global_account_rcv = true;
    m_ticket = msg;
    m_ticket_length = header->size;
}

char* UDPClient::SendTicket(char *ticket)
{
    m_global_account_rcv = false;
    int length = strlen(ticket);
    m_ticket = 0;

	m_ServerTCP->SendResponse(ENB_OPCODE_2002_TICKET, (unsigned char*)ticket, length);

	WaitForResponse();

	if (m_global_account_rcv)
    {
        LogMessage("Received Avatar list.\n");
    }
    else
    {
        fprintf(stderr,"\nGlobal ERROR!!, server appears to be offline\n");
    }

	//now switch this connection to a sector login
	//m_ServerTCP->SetToProxyLink();

    return m_ticket;
}

void UDPClient::ReceiveAvatarList(char *msg, EnbUdpHeader *header)
{
    m_global_account_rcv = true;
    //ok, send this avatar list to the client, or fail if the ticket is invalid
    m_ticket = msg;
}

void UDPClient::AvatarLoginConfirm(char *msg, EnbUdpHeader *header)
{
    m_global_account_rcv = true;
    m_ticket = msg;
    m_PlayerID = header->player_id;
    g_ServerMgr->m_UDPClient->SetPlayerID(m_PlayerID);
    LogMessage("Fixed port ID set to 0x%08x\n", m_PlayerID);
    g_ServerMgr->m_UDPClient->FixedClientComm();
}

long UDPClient::SendAvatarLogin(long char_slot)
{
    LogMessage("Sending Avatar Login.\n");

    //build packet structure.
    unsigned char data[128];
    int index = 0;
    memset (data, 0, 8);
    m_ticket_length = 0;
    m_global_account_rcv = false;
    m_ticket = 0;

    //data is character slot and account name
    AddData(data, ntohl(char_slot), index);
    AddDataLS(data, m_AccountName, index);

	m_ServerTCP->SendResponse(ENB_OPCODE_2004_AVATARLOGIN, data, index);

	WaitForResponse();

	m_PlayerID = ntohl(*((long*) (m_ticket + 20)));
	g_ServerMgr->m_UDPClient->SetPlayerID(m_PlayerID);
    LogMessage("Fixed port ID set to 0x%08x\n", m_PlayerID);

    //WaitForResponse(UDP_GLOBAL_SERVER_PORT, ENB_OPCODE_2004_AVATARLOGIN, data, index);

	if (m_global_account_rcv && m_PlayerID != 0)
	{
		//LogMessage("Received avatar login confirm.\n");
        return (*((long*) m_ticket));
    }
    else
    {
        return -1;
    }
}

void UDPClient::ProcessGlobalError(char *msg, EnbUdpHeader *header)
{
    if (g_ServerMgr->m_GlobalConnection)
    {
        g_ServerMgr->m_GlobalConnection->GlobalError(*((long*) msg));
    }
}

#endif

void UDPClient::FixedClientComm()
{
    //LogMessage("Sending Comm from Fixed-Port connection.\n");
    SendResponse(0, ENB_OPCODE_200F_COMM_PORT, 0, 0);
}


void UDPClient::MasterLoginConfirm(char *msg, EnbUdpHeader *header)
{
    m_global_account_rcv = true;
    m_ticket = msg;
}

short UDPClient::SendMasterLogin(long avatar_id, long sector_id, long *sector_ipaddr)
{
    m_PlayerID = avatar_id;
    g_ServerMgr->m_UDPClient->SetPlayerID(m_PlayerID);
    g_ServerMgr->m_UDPClient->FixedClientComm();

    //build packet structure.
    unsigned char data[8];
    int index = 0;
    memset (data, 0, 8);
    m_ticket_length = 0;
    m_global_account_rcv = false;
    m_ticket = 0;

    //data is sector_id
    AddData(data, sector_id, index);

	//Leave this as UDP
	//m_ServerTCP->SendResponse(ENB_OPCODE_2008_MASTER_HANDOFF, (unsigned char*)data, index);
	//WaitForResponse();

    WaitForResponse(UDP_MASTER_SERVER_PORT, ENB_OPCODE_2008_MASTER_HANDOFF, data, index);

    m_CurrentPacketNum = 0;

	if (m_global_account_rcv)
	{
		LogMessage("Received Master login confirm, new UDP port is %d Avatar_id = %d\n", *((short*) &m_ticket[4]), m_PlayerID );
        *sector_ipaddr = *((long*) m_ticket);
        //Now we can establish a TCP link to the server
        EstablishTCPConnection(m_IPAddr);

        //start login doublecheck thread here. If we haven't received a start packet within 10 seconds, send another handoff
        return (*((short*) &m_ticket[4]));
    }
    else
    {
        LogMessage("Error, did not receive master handoff ...\n");
        *sector_ipaddr = 0;
        return -1;
    }
}

void UDPClient::CreateDeleteConfirm(char *msg, EnbUdpHeader *header)
{
    m_global_account_rcv = true;
    m_ticket = msg;
}

char * UDPClient::CreateCharacter(GlobalCreateCharacter *create)
{
    fprintf(stderr,"\nSending Avatar Create.\n");
    m_global_account_rcv = false;
    m_ticket = 0;

	m_ServerTCP->SendResponse(ENB_OPCODE_200B_CREATE_AVATAR, (unsigned char*)create, sizeof(GlobalCreateCharacter));

	WaitForResponse();

    //re-send until we recieve the avatarlist
    //WaitForResponse(UDP_GLOBAL_SERVER_PORT, ENB_OPCODE_200B_CREATE_AVATAR, (unsigned char*)create, sizeof(GlobalCreateCharacter));

    //was the return valid?
	if (m_global_account_rcv)
	{
		fprintf(stderr,"\nReceived create avatar confirm.\n");
        return (m_ticket);
    }
    else
    {
        return 0;
    }
}

char* UDPClient::DeleteCharacter(long character_slot)
{
    fprintf(stderr,"\nSending Delete character.\n");

    //build packet structure.
    unsigned char data[128];
    int index = 0;
    memset (data, 0, 8);
    m_ticket_length = 0;
    m_global_account_rcv = false;
    m_ticket = 0;

	//data is character slot and account name
    AddData(data, ntohl(character_slot), index);
    AddDataLS(data, m_AccountName, index);

	m_ServerTCP->SendResponse(ENB_OPCODE_200D_DELETE_AVATAR, (unsigned char*)data, index);

	WaitForResponse();

	if (m_global_account_rcv)
    {
        fprintf(stderr,"\nReceived Avatar list.\n");
    }
    else
    {
        fprintf(stderr,"\nGlobal ERROR!!, server appears to be offline\n");
    }

    return m_ticket;
}

Connection * UDPClient::EstablishGlobalConnection(long ip_addr)
{
	m_ServerTCP = (0);
	Connection *global_connect = EstablishTCPConnection(ip_addr);

	if (global_connect)
	{
		//connection established. Set this connection as a login link
		global_connect->SendLoginLink(this);
		LogMessage("Login link established to server.\n");
	}
	else
	{
		LogMessage("Unable to establish login link to server ...\n");
	}

	return (global_connect);
}

Connection * UDPClient::EstablishTCPConnection(long ip_addr)
{
    Connection *connection = (0);

    if (m_ServerTCP)
    {
		m_ServerTCP->SetToProxyLink(); //automatically set this link to be a proxy link if link already exists.
		m_ServerTCP->SendPlayerConnectionConfirm(m_PlayerID);
        return (m_ServerTCP);
    }

    SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s == INVALID_SOCKET)
    {
        LogMessage("SectorServerManager unable to create socket\n");
        return 0;
    }
      
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = ip_addr;
    address.sin_port = htons(PROXY_SERVER_PORT);
    
    unsigned char * ip = (unsigned char *) &ip_addr;
    
    if (connect(s, (struct sockaddr*) &address, sizeof(address)))
    {
        LogMessage("Unable to connect to Sector Server: %d.%d.%d.%d:%d\n",
            ip[0], ip[1], ip[2], ip[3], PROXY_SERVER_PORT);
        return 0;
    }
    
    connection = new Connection(s, *g_ServerMgr, PROXY_SERVER_PORT, CONNECTION_TYPE_SECTOR_SERVER_TO_PROXY);

	m_UsingTCP = true;

    Sleep(50);

    connection->SendPlayerConnectionConfirm(m_PlayerID);

    m_ServerTCP = connection;

    return (connection);
}

void UDPClient::KillTCPConnection()
{
    long player_id = m_PlayerID;
    if (m_ServerTCP)
    {
        //m_ServerTCP->SendResponseDirect(ENB_OPCODE_3003_TCP_LOGIN_CLOSE, (unsigned char *) &player_id, sizeof(player_id));
        //m_ServerTCP->TerminateConnection();
		m_ServerTCP->SetTCPShutdownTime(GetNet7TickCount() + 2000);
        m_ServerTCP = (0);
    }
	m_UsingTCP = false;
}

void UDPClient::BlankTCPConnection()
{
    m_ServerTCP = (0);
}

void UDPClient::StartLoginTimer()
{
	if (login_thread_running)
    {
        return; //don't wait to reactivate thread, we're already in a login cycle
    }

	ResetGalaxyStart();
    SetLoginComplete(false);

    _beginthread( LoginCheckThread, 0, (void *) 0 );
}

void UDPClient::LoginFailAck()
{
    m_global_account_rcv = true;
}

void UDPClient::SendLoginFail()
{
    LogMessage("login failed, go back to the login screen\n");

	long player_id = PlayerID();

	WaitForResponse(UDP_MASTER_SERVER_PORT, ENB_OPCODE_3006_PLAYER_LOGIN_FAILED, (unsigned char*)&player_id, sizeof(long));
	g_ServerMgr->m_UDPClient->SendResponse(m_PlayerID, MVAS_LOGIN_PORT, ENB_OPCODE_1008_MVAS_LOGOFF_C_S, (unsigned char *) &m_PlayerID, sizeof(m_PlayerID));

	//kill the TCP login connection
	KillTCPConnection();

	//now kill both links to the client
	if (g_ServerMgr->m_SectorConnection)
	{
		g_ServerMgr->m_SectorConnection->TerminateConnection();
		g_ServerMgr->m_ConnectionCount--;
		g_ServerMgr->m_SectorConnection = 0;
	}

	//this should take us back to the login screen


	//stimulate a re-login
	//g_ServerMgr->m_SectorConnection->SendResponse(ENB_OPCODE_003A_SERVER_HANDOFF, (unsigned char *) &g_ServerMgr->m_UDPConnection->m_Server_handoff, sizeof(ServerHandoff));
	//g_ServerMgr->m_MasterConnection->HandleMasterJoin();

	//LogMessage("Forced client re-login\n");

	//WaitForResponse(UDP_MASTER_SERVER_PORT, ENB_OPCODE_3006_PLAYER_LOGIN_FAILED, (unsigned char*)&player_id, sizeof(long));

	/*if (!m_global_account_rcv)
	{
		LogMessage("Shutdown due to connection loss error.\n");
		::MessageBox(NULL, "Sector login failure. Please re-start E&B via launcher", "Net7Proxy", MB_ICONERROR);
		m_ServerTCP->SendResponseDirect(ENB_OPCODE_3004_PLAYER_SHIP_SENT, (unsigned char *) &player_id, sizeof(player_id));
		SendResponse(m_PlayerID, MVAS_LOGIN_PORT, ENB_OPCODE_1008_MVAS_LOGOFF_C_S, (unsigned char *) &m_PlayerID, sizeof(m_PlayerID));
		KillTCPConnection();
		ShutdownClient();
		//shutdown net7proxy
		g_ServerShutdown = true;
    }
	else
	{
		LogMessage("Sector Re-login requested, server should initiate new login\n");
	}*/

    /*if (g_ServerMgr->m_SectorConnection && m_ServerTCP)
    {
        long player_id = PlayerID();

		//display terminal connection loss error
		LogMessage("Shutdown due to connection loss error.\n");
		::MessageBox(NULL, "Sector login failure. Please re-start E&B via launcher", "Net7Proxy", MB_ICONERROR);
		m_ServerTCP->SendResponseDirect(ENB_OPCODE_3004_PLAYER_SHIP_SENT, (unsigned char *) &player_id, sizeof(player_id));
		SendResponse(m_PlayerID, MVAS_LOGIN_PORT, ENB_OPCODE_1008_MVAS_LOGOFF_C_S, (unsigned char *) &m_PlayerID, sizeof(m_PlayerID));
		KillTCPConnection();
		ShutdownClient();
		//shutdown net7proxy
		g_ServerShutdown = true;
    }*/
    //m_ServerTCP->SendResponse(ENB_OPCODE_3006_PLAYER_LOGIN_FAILED, (unsigned char*)&player_id, 4);
    //WaitForResponse(m_ClientPort, ENB_OPCODE_3006_PLAYER_LOGIN_FAILED, (unsigned char*)&player_id, sizeof(long));
}

void __cdecl LoginCheckThread(void *Param)
{
    long cycles = 5;
    login_thread_running = true;
    LogMessage(">> Commence Login check\n");
    //here we wait for 25 seconds, if login hasn't completed by then, tell the server to re-start login process
	while (cycles > 0 && !g_ServerMgr->m_UDPClient->GetLoginComplete())
    {
        Sleep(5000);
        cycles--;
    }

    bool valid = g_ServerMgr->m_UDPClient->GetLoginComplete();
    LogMessage(">> Login check: %s\n", valid ? "Login OK" : "Login FAILED");

	if (!valid)
	{
		//let's initiate a login re-start
		if (g_ServerMgr->m_UDPClient->GetStartReceived())
		{
			long player_id = g_ServerMgr->m_UDPClient->GetStartID();
			//ok we got galaxy map but no start. Bail the player out with a fake start, as they've started receiving packets
			g_ServerMgr->m_SectorConnection->SendResponse(ENB_OPCODE_0005_START, (unsigned char *) &player_id, sizeof(player_id));
		}
		else
		{
			g_ServerMgr->m_UDPConnection->SendLoginFail();
		}
	}
    
	login_thread_running = false;
}
