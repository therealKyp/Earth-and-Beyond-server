// Connection.h

#ifndef _TCP_CONNECTION_H_INCLUDED_
#define _TCP_CONNECTION_H_INCLUDED_

#include "Mutex.h"
#include "WestwoodRSA.h"
#include "WestwoodRC4.h"
#include "PacketStructures.h"
//#include "PlayerManager.h"
#include "MessageQueue.h"
//#include "ItemBase.h"
#include "SectorManager.h"

#define RC4_KEY_SIZE        8
#define RC4_UDP_KEY_SIZE    16
#define TCP_BUFFER_SIZE     (128 * 1024)

class ServerManager;
class SectorManager;
class PlayerManager;
class Groups;
struct TimeNode;
class Object;
struct PositionInformation;
class UDPClient;

class Connection
{
//////////////////////////////
//  Constructor/Destructor  //
//////////////////////////////
public:
    Connection(SOCKET s, ServerManager &server_mgr, short port, int server_type, unsigned long* ip_addr = 0);
    virtual ~Connection();

//////////////////////
//  Public Methods  //
//////////////////////
public:
    void    RunRecvThread();
    void    RunSendThread();
    bool    IsActive();
    void    SetRC4Key(unsigned char *rc4_key);
    void    SendSectorAssignment(long sector_id);
    void    SendResponse(short opcode, unsigned char *data=NULL, size_t length=0, long sequence_num = -1);
	void	SendResponseTestFile(short opcode, char *filename=NULL);
    void    SendResponseDirect(short opcode, unsigned char *data=NULL, size_t length=0);
    void    SendDataFile(unsigned char *buffer, short length);
    void    SendQueuedPacket(unsigned char *tcp_packet, short length);
    void    SendQueuedPacketOO(unsigned char *tcp_packet, short length, long sequence_num);
    void    QueuePacket(u8 *packet, short &index, unsigned char *data, size_t length);
	void    QueueResponse(unsigned char *packet, short &index, short opcode, unsigned char *data=NULL, size_t length=0);
	void    HandleMasterJoin();                     // opcode 0x35

	void	SetInSpace(bool in_space);
	void	SetActive(bool is_active);
  	void	SendWarpIndex(int index);
	void	TradeAction(long GameID, int Action);

    void    TerminateConnection();

	//Stargate Manipulation
	void	OpenStargate_1();
	void	OpenStargate_2(long object_id);
	void	CloseStargate(long object_id);
	void	GateSequenceHandoff(long sector_id);
	void	GateSequenceEnd();

	void	ProcessConfirmedActionOffer();
	void	ForceLogout();
	void	SendClientDamage(long target_id, long source_id, float damage, float modifier, long type, long inflicted = 0);
    void    SendObjectLinkedEffect(short bitmask, long UID, long effectID, short effectDID, long effect_time);
    void    QueueObjectLinkedEffect(u8* packet, short &index, short bitmask, long avatar_id, long UID, long effectID, short effectDID, long effect_time, long timestamp);
	void	GlobalError(int Error);					// Send Error
    void    SendPlayerConnectionConfirm(long player_id);
	void	SendLoginLink(UDPClient *client);
	void	SetToProxyLink();

	void	SetTCPShutdownTime(unsigned long time)	{ m_TcpShutdownCycle = time; }
	bool	CheckTCPShutdownCycle();

///////////////////////
//  Private Methods  //
///////////////////////
private:
    bool    DoKeyExchange();
    bool    DoClientKeyExchange();
    void    Send(unsigned char *Buffer, int length);

    static UINT WINAPI Connection::SocketSendThread(void *param);

    void    ProcessGlobalServerOpcode(short opcode, short bytes);
    void    ProcessMasterServerOpcode(short opcode, short bytes);
    void    ProcessSectorServerOpcode(short opcode, short bytes);
	void	ProcessProxyServerOpcode (short opcode, short bytes);
	void	ProcessProxyGlobalOpcode (short opcode, short bytes);
    void    ProcessMasterServerToSectorServerOpcode(short opcode, short bytes);
    void    ProcessSectorServerToSectorServerOpcode(short opcode, short bytes);

    void    HandleWaitingForMasterServer(short opcode, short bytes);
    void    HandleClientOpcode(short opcode, short bytes);
	bool	HandleCustomOpcode(short opcode, short bytes);

	void	HandleAccountValid(short bytes);
	void	HandleAvatarList(short bytes);

	void	ResetConnection();

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

    void    HandleVersionRequest();                 // opcode 0x00
    void    HandleLogin();                          // opcode 0x02
    void    HandleStartAck();                       // opcode 0x06
	void	HandleTurn();           				// opcode 0x12
	void	HandleTilt();	            			// opcode 0x13
    void    HandleMove();                           // opcode 0x14
    void    HandleRequestTarget();                  // opcode 0x17
	void	HandleRequestTargetsTarget();			// opcode 0x18
    void    HandleDebug();                          // opcode 0x1A
	void	HandleInventoryMove();					// opcode 0x27 
	void	HandleItemState();                      // opcode 0x29
    void    HandleAction();							// opcode 0x2C
    void    ProcessAction();
    void    HandleOption();                         // opcode 0x2E
    void    HandleClientChat();                     // opcode 0x33
    
    void    HandleRequestTime();                    // opcode 0x44
    void    HandleStarbaseRequest();                // opcode 0x4E
	void	HandleSkillStringRequest();				// opcode 0x51
    void    HandleSelectTalkTree();                 // opcode 0x55
	void	HandleSkillAction();					// opcode 0x57
	void	HandleSkillAbility();					// opcode 0x58
	void	HandleEquipUse();						// opcode 0x5D
    void    HandleVerbRequest();                    // opcode 0x5A
	void    HandleChatStream();						// opcode 0x5E
    void    HandleGlobalConnect();                  // opcode 0x6D
    void    HandleGlobalTicketRequest();            // opcode 0x6E
    void    HandleDeleteCharacter();                // opcode 0x71
    void    HandleCreateCharacter();                // opcode 0x72
	void	HandleMissionForfeit();					// opcode 0x86
	void	HandleMissionDismissal();				// opcode 0x87
    void    HandlePetitionStuck();                  // opcode 0x88
    void    HandleIncapacitanceRequest();           // opcode 0x8D
    void    HandleGalaxyMapRequest();               // opcode 0x98
	void	HandleWarp();							// opcode 0x9B
    void    HandleStarbaseAvatarChange();           // opcode 0x9D
    void    HandleStarbaseRoomChange();             // opcode 0x9F
    void    HandleTriggerEmote();                   // opcode 0xA1
    void    HandleClientChatRequest();              // opcode 0xA3
    void    HandleLogoffRequest();                  // opcode 0xB9
	void	HandleCTARequest();						// opcode 0xBC

	//socket test action
	void	HandleActionResponse();

	bool	HandleWormholeRequest(char *sector);	// used by '/wormhole' command
    bool    HandleScaleRequest(char *param);
	bool	HandleOrientationRequest(char *orientation);	//used by '/orientation' command
	bool	HandleEulerOrientationRequest(char *orientation);	// used by '/euler' command
	bool	HandleMoveRequest(char *param);
	bool	MatchOptWithParam(char *option, char *arg, char *&param, bool &msg_sent);
	bool	HandleKick(char *param);
	bool	HandleFetchRequest();
	bool	HandleFaceRequest(long Target);
	bool	HandleGotoRequest();
    bool    HandleObjectHijack();
    void    HandleReleaseHijack();
	bool	HandleRenderStateRequest();
	bool	HandleRenderStateInitRequest(char *render_state);
	bool	HandleRenderStateActivateRequest(char *param);
	bool	HandleRenderStateActivateNextRequest(char * param);
	bool	HandleRenderStateDeactivate();
	void	HandleSendVerbRequest(char *param);
	bool	HandleMobCreateRequest(char *param);
	bool	HandleObjCreateRequest(char *param);
	bool	HandleResetMaster();

	bool	CheckResourceLock(long object_id);


    ////////////////////////////////
    //  Server to Client Opcodes  //
    ////////////////////////////////
public:
    bool	HandleRangeRequest();

    void    SendVersionResponse(long status);       // opcode 0x01
                                                    // opcode 0x04
    void    SendCreate(int game_id, float scale, short asset, int type, float h=0.0, float s=0.0, float v=0.0);
    void    QueueObjectCreate(u8* packet, short &index, int game_id, float scale, short asset, int type, float h=0.0f, float s=0.0f, float v=0.0f);

    void    SendStart(long start_id);               // opcode 0x05
                                                    // opcode 0x08
    void    SendSimplePositionalUpdate(long object_id, PositionInformation * position_info);
                                                    // opcode 0x09
    void    SendObjectEffect(ObjectEffect *object_effect);
                                                    // opcode 0x0a
    void    QueueEffect(u8* packet, short &index, int player_id, int target_id, char *message, short effect_type, long effect_id, long timestamp, long effect_time);

    void    SendPointEffect(int effect_id, float x, float y, float z, short duration,
                        short effect_desc_id, float scale, float h=0.0, float s=0.0, float v=0.0);
													// opcode 0x0b
	void	SendObjectToObjectEffect(ObjectToObjectEffect *obj_effect, long sequence);
    void    QueueObjectToObjectEffect(u8* packet, short &index, ObjectToObjectEffect *obj_effect);
													// opcode 0x0f
	void	SendRemoveEffect(int target_id);
                                                    // opcode 0x10
    void    SendDecal(int game_id, int decal_id, int decal_count);
                                                    // opcode 0x19
    void    SendSetTarget(int game_id, int target_id);

    void    SendMessageString(char *msg, char color=5, bool log=true);
													// opcode 0x20
    void    QueueMessageString(u8* packet, short &index, char *msg, char color=5, bool log=true);

	void	SendPriorityMessageString(char *msg1, char *msg2, long time, long priority);
													// opcode 0x25
    void    SendSetZBand(float min, float max);     // opcode 0x2a
                                                    // opcode 0x2b
    void    SendSetBBox(float xmin, float ymin, float xmax, float ymax);
													// opcode 0x2f
	void	SendInitRenderState(long game_id, unsigned long render_state_id);
                                                    // opcode 0x30
    void    SendActivateRenderState(long game_id, unsigned long render_state_id);
													// opcode 0x31
	void	SendActivateNextRenderState(long game_id, unsigned long render_state_id);
													// opcode 0x32
	void	SendDeactivateRenderState(long game_id);
    void    SendClientSetTime(long TimeSent);       // opcode 0x34
    void    SendServerRedirect(long sector_id);     // opcode 0x36
                                                    // opcode 0x3a
    void    SendServerHandoff(long from_sector_id, long to_sector_id,
                char *from_sector, char *from_system, char *to_sector, char *to_system);
    void    SendClientType(long client_type);       // opcode 0x3c
                                                    // opcode 0x3e
    void    SendAdvancedPositionalUpdate(long object_id);
    void    SendAdvancedPositionalUpdate(long object_id, PositionInformation * position_info);
                                                    // opcode 0x3f
    void    SendPlanetPositionalUpdate(long object_id, PositionInformation * position_info);
                                                    // opcode 0x40
    void    SendConstantPositionalUpdate(long game_id, float x, float y, float z, float *orientation=NULL);
	void 	SendFormationPositionalUpdate(long leader_id, long target_id, float x, float y, float z);

                                                    // opcode 0x46
    void    SendComponentPositionalUpdate(long object_id, PositionInformation * position_info, long timestamp=0);
                                                    // opcode 0x4a
    void    SendCreateAttachment(int parent, int child, int slot);
                                                    // opcode 0x4f
    void    SendStarbaseSet(long sector, char action, char exit_mode);
    void    QueueStarbaseSet(u8* packet, short &index, long sector, char action, char exit_mode);
    void    SendTalkTreeAction(long action);        // opcode 0x56
	void	SendClientSound(char *sound_name, long channel = 0, char queue = 0);
    void    SendGlobalTicket(long avatar_id, long sector_id, long level, bool issue);
    void    ProcessGlobalTicket(long char_slot);
    void    SendAvatarList(long account_id);                       // opcode 0x70
    void    SetManufactureID(long mfg_id);          // opcode 0x7f
                                                    // opcode 0x89
    void    SendRelationship(long ObjectID, long Reaction, bool IsAttacking);
    void    QueueRelationship(u8* packet, short &index, long ObjectID, long Reaction, bool IsAttacking);
    void    SendCameraControl(long Message, long GameID);
    void    QueueCameraControl(u8* packet, short &index, long Message, long GameID);
                                                    // opcode 0x97
    void    SendGalaxyMap(char *system, char *sector, char *station);
                                                    // opcode 0x99
    void    SendNavigation(int game_id, float signature, char visited, int nav_type, char is_huge);
                                                    // opcode 0xa2
    void    SendNotifyEmote(long game_id, long emote);
                                                    // opcode 0xb2

    void    SendNameDecal(int game_id, char *shipname, float h=1.0, float s=1.0, float v=1.0);
    void    SendLogoffConfirmation();               // opcode 0xba
	void	SendPushMessage(char *msg1, char *type, long time, long priority);
	void	AddMOBDestroyExperience(short mob_level, char *mob_name);

	bool	SendLoungeNPC(long StationID);

	long	CurrentResourceTarget();

	void	SendClientChatEvent(int Unknown, int Unknown2, char * FName, char * LName, char * Channel, char * Message);


	// Trade
	void	CancelTrade();

    // Special purpose methods
    void    SendDataFileToClient(char *filename, long avatar_id=0);
    long    TryLoungeFile(long sector_id);
	void	RemovePlayer();
  	void	QueueContrails(u8* packet, short &index, long player_id, bool contrails);
    void    QueueStart(u8* packet, short &index, long player_id);

	void	OpenInterface(long UIChange, long UIType);
	void	CheckObjectRanges();
	void	UnSetTarget(long GameID = 0);
	void	ChangeSectorID(long SectorID);
	void	ActivateBeamEffect(long player_id, long targetID, char *message, short effect_type, long effectUID, long timestamp, short effect_time = 0);
    void    QueueBeamEffect(u8* packet, short &index, long player_id, long targetID, char *message, short effect_type, long effectUID, long timestamp, short effect_time = 0);
	void	SetResourceDrainLevel(Object *obj, unsigned char slot);
	void	SendResourceName(long resourceID, char *resource_name);
    void    QueueResourceName(u8* packet, short &index, long resourceID, char *resource_name);
    void    SendHuskName(Object *husk);
    void    SendHuskContent(Object *husk);
	void	SendProspectAUX(long timestamp, int type);
    void    QueueProspectAUX(u8* packet, short &index, long timestamp, int type);
	void	CreateTractorComponent(float *position, float decay, float tractor_speed, long player_id, long article_id, long effect_id, long timestamp);
    void    QueueTractorComponent(u8* packet, short &index, float *position, float tractor_speed, long player_id, long article_id, long effect_id, long timestamp);
	void	SendResourceContentsAUX(Object *obj);
	void	SendResourceLevel(long target_id);
	void	Dialog(char * Stringd, int Type);
    void    SendObjectToObjectLinkedEffect(Object *target, Object *source, short effect1, short effect2, float speedup = 1.0f);

	void	RemoveObject(long object_id);
	void	CloseInterfaceIfTargetted(long target_id);
    void    CloseInterfaceIfOpen();
	void	PointEffect(float *position, short effect_id, float scale = 1.0f);
	void	SendChangeBasset(ChangeBaseAsset *NewAsset);
	void	SendAttackerUpdates(long mob_id, long update);
	void	SendConfirmedActionOffer();

	//XP methods (assuming no single XP increase will be more than 65k)  //Dima 
	void	AddCombatXP(char * name, long XP_Gain, bool SkipGroup=false);
	void	AddExploreXP(char * name, long XP_Gain, bool SkipGroup=false);
	void	AddTradeXP(char * name, long XP_Gain, bool SkipGroup=false);

    void    SendAuxNameSignature(Object *obj);
    void    SendAuxNameResource(Object *obj);
    void    SendSimpleAuxName(Object *obj);
    void    SendMobName(Object *mob);



/////////////////////////////////
//        Manufacting	       //
/////////////////////////////////
private:
	void	HandleManufactureTerminal();            // opcode 0x79
	void	HandleManufactureCategorySelection();   // opcode 0x7A
	void	HandleManufactureSetItem();             // opcode 0x7B
	void	HandleRefineSetItem();                  // opcode 0x7C
	void	HandleManufactureAction();				// opcode 0x7E
    void    HandleManufactureLevelFilter();         // opcode 0x80

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
	unsigned long		m_TcpShutdownCycle;			// thread shutdown time
    unsigned char       m_RecvBuffer[MAX_BUFFER];	// TCP/IP Receive buffer
	unsigned char       m_SendBuffer[MAX_BUFFER];   // TCP/IP Send buffer
    unsigned char       m_MessageBuffer[400];
    int                 m_ServerType;				// Server type (1=GS, 2=MS, 3=SS)
    short               m_TcpPort;					// TCP/IP port number

	UDPClient		*	m_UDPClient;				// Used for account reference
#ifndef WIN32
    pthread_t m_Thread;
#endif

    char * m_AccountUsername;

public:
    ServerManager &     m_ServerMgr;				// Reference to the Server Manager


    //Pretty much all this is now obsolete, should be in the Player Class
    long                m_AvatarID;         // used by GS,MS,SS
    long                m_SectorID;
	bool				m_PacketLoggingEnabled;
    GlobalAvatarList    m_Player_Avatar_List;
    long                m_IPaddr;

	// The following are for use the slash commands
private:
    MessageQueue        m_SendQueue;
    HANDLE              m_SendThreadHandle;
    Mutex			    m_Mutex;
	unsigned long		m_Tilt_Sent;
	unsigned long		m_Turn_Sent;
};

#endif // _TCP_CONNECTION_H_INCLUDED_
