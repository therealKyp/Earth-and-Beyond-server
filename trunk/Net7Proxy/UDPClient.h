// UDPClient.h
// Defines the UDPClient class which connects to the UDP Connection server in the Net7 server

#ifndef _UDP_CLIENT_H_INCLUDED_
#define _UDP_CLIENT_H_INCLUDED_

#define	MAX_UDPC_BUFFER					16384
#define MAX_QUEUE_BUFFER                4096

#pragma warning(disable:4786) 

#include "PacketStructures.h"
#include <vector>
#include <map>

struct EffectCancel
{
    long time_delay;
    long effect_id;
};

typedef std::map<unsigned long, char *> PacketList;

class Connection;

class UDPClient
{
public:
    UDPClient(short port, short connection_type, long ip_addr);
	virtual ~UDPClient();

    void    SetBroadcast(SOCKET socket);
    void    RecvThread();
    void    MVASThread();
    bool    VerifyConnection();

	unsigned long checksum(char *buffer, int size);		// checksum

    void    SendLogin();
    void    SendAccount(char *username, char *password, char *info);
    void    SendResponse(long player_id, short port, short opcode, unsigned char *data, size_t length);
    char  * SendTicket(char *ticket);
    void    SetPlayerID(long player_id)             { m_PlayerID = player_id; }
    long    PlayerID()                              { return m_PlayerID; }
    void    SetLoginComplete(bool flag)             { m_LoginComplete = flag; }
    bool    GetLoginComplete()                      { return m_LoginComplete; }
	void	SetReceivedGalaxyMap()					{ m_GalaxyMapReceived = true; }
	void	SetStartReceived()						{ m_StartReceived = true; }
	bool	GetStartFailed()						{ return (m_GalaxyMapReceived && !m_StartReceived); }
	bool	GetStartReceived()						{ return m_StartReceived; }
	void	SetStartID(long start_id)				{ m_Start_ID = start_id; }
	long	GetStartID()							{ return m_Start_ID; }
	void	ResetGalaxyStart()						{ m_GalaxyMapReceived = false; m_StartReceived = false; }
    long    SendAvatarLogin(long char_slot);
    char  * CreateCharacter(GlobalCreateCharacter *create);
    char  * DeleteCharacter(long character_slot);
	void	SetAccountName(char *name);

    short   SendMasterLogin(long avatar_id, long sector_id, long *sector_ipaddr);
    void    ForwardClientOpcode(short opcode, short bytes, char *packet);

    void    SetClientPort(short port)               { m_ClientPort = port; }
    void    SetClientIP(long addr)                  { m_ClientIP = addr; }
    void    SetSectorID(long sector_id)             { m_SectorID = sector_id; }
    void    SetConnectionActive(bool active)        { m_ConnectionActive = active; }
    bool    ConnectionActive()                      { return m_ConnectionActive; }
    long    GetClientIP()                           { return m_IPAddr; }
    long    GetSectorID()                           { return m_SectorID; }
	void	SetPreferTCP()							{ m_PreferTCP = true; }
	bool	PreferTCP()								{ return m_PreferTCP; }
    void    SendPositionIfChanged();

    void    StartLoginTimer();
    void    KillTCPConnection();
    void    BlankTCPConnection();
    void    SendLoginFail();

    Connection *EstablishTCPConnection(long ip_addr);
	Connection *EstablishGlobalConnection(long ip_addr);
    void    IncommingOpcodePreProcessing(short opcode, char *msg, short bytes, bool tcp = false); //examine incomming opcodes and react accordingly
	bool	HandleCustomOpcode(short opcode, char *ptr, u8 *tcp_packet, short &tcp_index);
	u8    * GetQueueBuffer()						{ return m_QueueBuffer; }
	void	ValidAccount(unsigned char *msg, short len);
	void	ProcessAvatarList(unsigned char *msg, short len);

private:
    int     UDP_RecvFromServer(char *buffer, int size);
    void    SignalWrongVersion(char *msg);
    void    TerminateClient(char *msg);
    void    TerminateClient();
    void    SendResponse(short port, short opcode, unsigned char *data, size_t length);
    void    UDP_Send(short port, const char *buffer, int bufferLen);
	void	WaitForResponse();
    void    WaitForResponse(short port, short opcode, unsigned char *data, size_t length);
    void    CreateFrom(long ip_addr, short port);

    bool    OpenFixedPort(short port, long ip_addr);
    bool    OpenMultiPort(short port, long ip_addr);
    void    FixedClientComm();

	void	RecordLastHandoff(char *msg, short bytes);
    void    ValidateAccount     (char *msg, EnbUdpHeader *header);
    void    ReceiveAvatarList   (char *msg, EnbUdpHeader *header);
    void    AvatarLoginConfirm  (char *msg, EnbUdpHeader *header);
    void    CreateDeleteConfirm (char *msg, EnbUdpHeader *header);
    void    ProcessGlobalError  (char *msg, EnbUdpHeader *header);
    void    LoginFailAck();

    void    MasterLoginConfirm  (char *msg, EnbUdpHeader *header);

    void    ProcessClientOpcode  (char *msg, EnbUdpHeader *header);
    void    SendClientDataFile  (char *msg, EnbUdpHeader *header);
    void    SendClientPacketSequence(char *msg);
    void    SendPacketSequence  (char *msg, EnbUdpHeader *header);
    void    SendCachedGalaxyMap ();
    void    StartProspecting    (char *msg, u8* packet, short &index);
    void    TractorOre          (char *msg, u8* packet, short &index);
    void    LootItem            (char *msg, u8* packet, short &index);
    
    void    IncommingOpcodePostProcessing(short opcode, char *msg, short bytes); 

    void    SendLoungeOpcodes(unsigned char *data, size_t length);

    bool    CheckOpcodeOrder(short opcode, char *msg, EnbUdpHeader *header, int received);

    //MVAS
    bool    CheckPosition();
    void    ToggleSendFrequency(char *msg);

    //keepalive
    void    SendClientAlive();
    void    SendCommsAlive();

private:
    long m_Port;
    long m_IPAddr;
    long m_SectorID;
    long m_PlayerID;
	long m_Start_ID;
    bool m_recv_thread_running;
    SOCKET m_Listen_Socket;
    SOCKADDR_IN m_SockAddr;
    short m_ConnectionType;
    unsigned char m_RecvBuffer[MAX_UDPC_BUFFER];
    unsigned char m_SendBuffer[MAX_UDPC_BUFFER];
    unsigned char m_QueueBuffer[MAX_QUEUE_BUFFER];
    bool m_logged_in;
    bool m_global_account_rcv;
    bool m_ConnectionActive;
    char *m_ticket;
    short m_ticket_length;
    char m_AccountName[128];
    short m_QueueBufferFill;

	bool m_GalaxyMapReceived;
	bool m_StartReceived;

    short m_ClientPort;
    long m_ClientIP;
    long m_CurrentPacketNum;
    bool m_AlternatePorts;
    bool m_LoginComplete;
    bool m_Resync;
	bool m_PreferTCP;
	bool m_UsingTCP;
	unsigned long m_PacketResendTimer;

    PacketList m_Packets;
    short m_PacketTimeout;
	short m_PacketDropThisSession;

	ServerHandoff m_Server_handoff;
    Connection *m_ServerTCP;

    struct ReSend
    {
        long packet_start;    
        long packet_count;
    };
};


#endif