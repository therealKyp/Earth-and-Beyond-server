// Connection.h

#ifndef _TCP_CONNECTION_H_INCLUDED_
#define _TCP_CONNECTION_H_INCLUDED_

#include "Mutex.h"
#include "WestwoodRSA.h"
#include "WestwoodRC4.h"
#include "PacketStructures.h"
#include "PlayerManager.h"
#include "MessageQueue.h"
#include "ItemBase.h"
#include "SectorManager.h"

#ifndef WIN32
	#include <pthread.h>
#endif

#define RC4_KEY_SIZE        8
#define RC4_UDP_KEY_SIZE    16
#define TCP_BUFFER_SIZE     (128 * 1024)

class ServerManager;
class SectorManager;
class PlayerManager;
class Groups;
struct PositionInformation;
struct TimeNode;
class Object;

class Connection
{
//////////////////////////////
//  Constructor/Destructor  //
//////////////////////////////
public:
    Connection(SOCKET s, ServerManager &server_mgr, short port, int server_type, unsigned long* ip_addr = 0);
	Connection();
    virtual ~Connection();

	Connection * ReSetConnection(SOCKET s, ServerManager &server_mgr, short port, int server_type, unsigned long* ip_addr = 0);

//////////////////////
//  Public Methods  //
//////////////////////
public:
    void    RunRecvThread();
    void    RunSendThread();
	void	RunKeyExchange();
    bool    IsActive();
    void    SetRC4Key(unsigned char *rc4_key);
    void    SendSectorAssignment(long sector_id);
    void    SendResponse(short opcode, unsigned char *data=NULL, size_t length=0);
	void	SendResponseTestFile(short opcode, char *filename=NULL);
    void    Send(unsigned char *Buffer, int length);

	long	GameID()			{ return m_AvatarID; }
	void	SetGameID(long id)	{ m_AvatarID = id;   }

	void	PulseConnectionOutput(bool player_thread); //this method is the same as a send thread cycle for the connection, we just check for outputs and send them

	bool	CheckStatus(long size); // this method checks if there's enough space in the message queue for another message

///////////////////////
//  Private Methods  //
///////////////////////
private:
    bool    DoKeyExchange();
    bool    DoClientKeyExchange();

    void    ProcessGlobalServerOpcode(short opcode, short bytes);
    void    ProcessMasterServerOpcode(short opcode, short bytes);
    void    ProcessSectorServerOpcode(short opcode, short bytes);
    void    ProcessMasterServerToSectorServerOpcode(short opcode, short bytes);
    void    ProcessSectorServerToSectorServerOpcode(short opcode, short bytes);
    void    ProcessProxyClientOpcode(short opcode, short bytes);
    void    ProxyClientOpcode(short opcode, short bytes);

    void    HandleClientOpcode(short opcode, short bytes);
	void	ResetConnection();

#ifdef WIN32
	static DWORD WINAPI Connection::SocketRecvThread(void *param);
#else
	static void* SocketRecvThread(void *param);
#endif

    ////////////////////////////////
    //  Server to Server Opcodes  //
    ////////////////////////////////

    void    HandleSectorServerAssignment();         // opcode 0x8701
    void    HandleRequestCharacterData();           // opcode 0x8702
    void    SendCharacterData();                    // opcode 0x8802
    void    HandleCharacterData(short length);      // opcode 0x8802

    ////////////////////////////////
    //  Client to Server Opcodes  //
    ////////////////////////////////

	void	GlobalError(int Error);					// Send Error
    void    HandleVersionRequest();                 // opcode 0x00
    void    HandleLogin();                          // opcode 0x02

    void    HandleMasterJoin();                     // opcode 0x35
    void    HandleGlobalConnect();                  // opcode 0x6D
    void    HandleGlobalTicketRequest();            // opcode 0x6E
    void    HandleDeleteCharacter();                // opcode 0x71
    void    HandleCreateCharacter();                // opcode 0x72


    void    ValidateLoginLink();                    // opcode 0x3002 (Net7Proxy)
    void    ShutdownLoginLink();                    // opcode 0x3003
    void    CommenceNavSend();                      // opcode 0x3004 (Net7Proxy)
    void    HandleLoginFailed();                    // opcode 0x3006
	void	HandleStarbaseLoginComplete();			// opcode 0x3008 (Net7Proxy)

    ////////////////////////////////
    //  Server to Client Opcodes  //
    ////////////////////////////////
public:
    void    SendVersionResponse(long status);       // opcode 0x01
                                                    // opcode 0x04

    void    SendServerRedirect(long sector_id);     // opcode 0x36
                                                    // opcode 0x3a
    void    SendServerHandoff(long from_sector_id, long to_sector_id,
                char *from_sector, char *from_system, char *to_sector, char *to_system);
    void    SendClientType(long client_type);       // opcode 0x3c
                                                    // opcode 0x3e

    void    SendGlobalTicket(long avatar_id, long sector_id, long level, bool issue);
    void    ProcessGlobalTicket(Player *player);
    void    SendAvatarList(long account_id);                       // opcode 0x70
	void	ShutdownTCPLink(long game_id);
	int		GetServerType()							{ return m_ServerType; }
	bool	LoginHandoff()							{ return m_LoginHandoff; }
	void	SetLoginHandoff(bool handoff)			{ m_LoginHandoff = handoff; }


/////////////////////////////////
//  Public Member Attributes  //
/////////////////////////////////
public:
    WestwoodRC4         m_CryptIn;          // RC4 decryption for inbound data
    WestwoodRC4         m_CryptOut;         // RC4 encryption for outbound data

/////////////////////////////////
//  Private Member Attributes  //
/////////////////////////////////
private:
    // Attributes required for all servers
    WestwoodRSA         m_WestwoodRSA;				// RSA-155 encryption
    SOCKET              m_Socket;					// Our TCP/IP socket
    bool                m_ConnectionActive;			// true if the TCP/IP connection is active
    bool                m_TcpThreadRunning;			// true if TCP Thread is running
	bool				m_LoginHandoff;
    unsigned char       m_RecvBuffer[MAX_BUFFER];	// TCP/IP Receive buffer
	unsigned char       m_SendBuffer[MAX_BUFFER];   // TCP/IP Send buffer
    int                 m_ServerType;				// Server type (1=GS, 2=MS, 3=SS)
    short               m_TcpPort;					// TCP/IP port number
#ifndef WIN32
    pthread_t 			m_Thread;
#endif

    char *				m_AccountUsername;
	char *				m_LastOwner;
    long                m_AvatarID;

public:
	bool				m_PacketLoggingEnabled;

private:
    MessageQueue      * m_SendQueue;
#ifdef WIN32
    HANDLE              m_RecvThreadHandle;
#else
    pthread_t			m_RecvThreadHandle;
#endif
    Mutex			    m_Mutex;
    long                m_IPaddr;
};

#endif // _TCP_CONNECTION_H_INCLUDED_
