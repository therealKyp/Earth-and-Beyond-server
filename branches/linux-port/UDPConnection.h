// UDPConnection.h

#ifndef _UDP_CONNECTION_H_INCLUDED_
#define _UDP_CONNECTION_H_INCLUDED_

#include <vector>
#include "Mutex.h"
#include "MessageQueue.h"
#include "PlayerManager.h"
#include "PacketStructures.h"

class Player;
class Object;

enum Connection_Type
{
	AUTH_SERVER = 1, POLL_SERVER
};

struct connection_node
{
	long ip_addr;
	long player_id;
	short port;
	Player *player;
	long frequency;
};


class UDP_Connection
{
public:
	UDP_Connection(unsigned short port, ServerManager *server_mgr, int server_type);
	virtual ~UDP_Connection();

    void				RunRecvThread();
    void				RunSendThread();
	void				AddConnection(long ip_addr);
	void				UDP_Send(const char *buffer, int length, long IPaddr, short port);
	void				SendOpcode(short opcode, Player *p, unsigned char *data, size_t length, long player_ip, short port, long sequence_num = 0);
    void				SendOpcode(short opcode, unsigned char *data, size_t length, long player_ip, short port);
	void				SetServerManager(ServerManager * server_mgr);
	void				SendMVASPosReset(long IPaddr, short port);
	void				SendMVASSectorLogin(long player_id, long IPaddr, short port);
	void				SendPositionPoll(long IPaddr, short port);
	long				MatchMVASConnection(long IPaddr, const Player *player);
	void				QueueResetAndLogin(long MVAS_index);
    void                FlushQueue();
  	void                Shutdown();

    void                ValidateSectorServer(long sector_id, short port, long ip_address);

private:
	unsigned long		checksum(char *buffer, int size);
	bool				UDP_BindPort(short port, SOCKET socket);
	void				UDP_SetBroadcast(SOCKET socket);
	int					UDP_Recv(char *buffer, int length);
	int					UDP_RecvS(char *buffer, int length, long &sourceAddress, unsigned short &sourcePort);
	void				Reset_Socket();

    //MVAS methods
	void				HandleMoveAssistRegister(char *buffer, long IPaddr, short port);
    void                SendMVASPlayerIndex(long addr, short port, long index);
	void				HandleMVASPosReturn(char *buffer, EnbUdpHeader *hdr, long IPaddr, short port);
	void				SendToggleFrequency(connection_node *UDPconnection);
    void                HandleLogout(char *buffer, const long IPaddr);
    void                HandleMVASOpcode(char *msg, EnbUdpHeader *hdr, const long source_addr, const short source_port);
    void                HandleCommReturn(char *msg, EnbUdpHeader *hdr, const long source_addr, const short source_port);
    void                HandleLoungeReady(EnbUdpHeader *hdr);
    void                HandleKeepCommsAlive(EnbUdpHeader *hdr, long addr, short port);

    //Global connection methods
    void                HandleGlobalOpcode(char *msg, EnbUdpHeader *hdr, const long source_addr, const short source_port);
    bool                VerifyAccountInfo(char *msg, EnbUdpHeader *hdr, const long source_addr, const short source_port);
    bool                ProcessTicketInfo(char *msg, EnbUdpHeader *hdr, const long source_addr, const short source_port);
    void                SendAvatarList(long account_id, const long source_addr, const short source_port);
    void                SendGlobalError(int error, const long source_addr, const short source_port);
    void                HandleGlobalTicketRequest(char *msg, EnbUdpHeader *hdr, const long source_addr, const short source_port);
    void                HandleAvatarCreateRequest(char *msg, EnbUdpHeader *hdr, const long source_addr, const short source_port);
    void                HandleAvatarDeleteRequest(char *msg, EnbUdpHeader *hdr, const long source_addr, const short source_port);

    //Master Connection methods
    void                HandleMasterOpcode(char *msg, EnbUdpHeader *hdr, const long source_addr, const short source_port);
    void                ProcessHandoff(char *msg, EnbUdpHeader *hdr, const long source_addr, const short source_port);
    void                HandleLoginFailed(Player *p);

    //Sector connection methods
    void                HandleClientOpcode(char *msg, EnbUdpHeader *hdr, const long source_addr, const short source_port);
    void                ProcessClientOpcode(char *msg, EnbUdpHeader *hdr, const long source_addr, const short source_port);
    void                ProcessServerValidation(char *msg, EnbUdpHeader *hdr, const long source_addr, const short source_port);

	SOCKET				m_Socket;
	//Agfot: unused and doesn't exist on linux so commented out
	//SOCKADDR_IN			m_SocketAddr;
	short				m_Port;
    bool                m_ThreadRunning;			// true if thread is running
    unsigned char       m_RecvBuffer[MAX_BUFFER];			// UDP Receive buffer
	unsigned char       m_SendBuffer[MAX_BUFFER];   // UDP Send buffer
	ServerManager     * m_ServerMgr;				// Reference to the Server Manager
	int		            m_ServerType;
    char              * m_AccountUsername;
    bool                m_Sector_Operational;
	bool				m_Socket_Reset_Required;
    long                m_SectorID;
	unsigned long		m_LoginAttempts;


	Mutex			    m_Mutex;
#ifndef WIN32
    pthread_t			m_Thread;
#endif


};

#endif // _UDP_CONNECTION_H_INCLUDED_
