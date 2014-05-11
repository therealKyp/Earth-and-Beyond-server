// PlayerManager.h

#ifndef _PLAYER_MANAGER_H_INCLUDED_
#define _PLAYER_MANAGER_H_INCLUDED_

#define MAX_ONLINE_PLAYERS 200

#include <vector>
#include "Mutex.h"
#include "PacketStructures.h"
#include "SectorData.h"
#include "VectorMath.h"
#include "AuxClasses/AuxGroupInfo.h"

class UDP_Connection;
class Connection;
struct StarbaseAvatarChange;
struct StationBroadcastNode;
class GMemoryHandler;
class Object;
class Player;

struct Group
{
	int GroupID;
	bool ForceAutoSplit;
	bool RestrictedLootingRights;
	bool AutoReleaseLootingRights;
	char FormationName[40];
	_GroupMember Member[6];
	bool AcceptedInvite[6];
	struct Group * next;
} ATTRIB_PACKED;

// There is one instance of this class for all sectors in this process
class PlayerManager
{
public:
    PlayerManager();
    virtual ~PlayerManager();

public:
	void StoreCharacterData(CharacterDatabase &character_database);
	bool GetCharacterData(long avatar_id, CharacterDatabase &character_database);
    void AddPlayer(long avatar_id, Player *player);
    void DropPlayer(long avatar_id);
    void HandleStarbaseAvatarChange(long SectorID, StarbaseAvatarChange *change);
    void HandleStarbaseRoomChange(long SectorID, StarbaseRoomChange *change, int aflag=1);
	void SendAction(long GameID, int Action);

//private:
public:
    typedef std::vector<Player*> PlayerList;

    struct MParam
    {
	    PlayerManager * ClassAdd;
    } ATTRIB_PACKED;


///////////////////////////////
/////  Movement In Space //////
///////////////////////////////

public:
	void	RunMovementThread(void);
	void	RunSaveThread(void);
    void    RunCleanoutThread(void);

	// Chat Stuff
	void	ChatSendEveryone(long GameID, char * Message, bool copy_to_originator = true);
	void	ChatSendPrivate(long GameID, char * Nick, char * Message);
	void	ChatSendChannel(long GameID, char * Channel, char * Message);
	void	GMMessage(char * Message);
    void    GlobalAdminMessage(const char * Message);

	// Send new players the players in the sector
	void	RemovePlayer(Player *player);
	void	ListPlayersAndLocations(Player *send_to);
	void	SendPlayerWithoutConnection(long player_id);
    void    SetSector(Player *player, long sector_id);
    bool    SetupPlayer(Player *player, long IPaddr);
    void    UnallocatePlayer(Player *player);
    void    SetGlobalMemoryHandler(GMemoryHandler *MemMgr);
    void    CheckForDuplicatePlayers(Player *player);
	bool	CheckAccountInUse(char *username);
    u32	  * GetSectorList(Player *player);
	bool	GetNextPlayerOnList(Player *&p, u32 *player_list);
	bool	GetIndex(u32 index, u32 *player_list);
	void	SetIndex(u32 index, u32 *player_list);
	void	UnSetIndex(u32 index, u32 *player_list);

    //new login/logout methods
    void    DropPlayerFromSector(Player *p);
    void    DropPlayerFromGalaxy(Player *p);

	//MVAS methods
	Connection *MatchMVAS(Connection *mvas_connection);
	void	SendMVASUpdate(long GameID);
	void	SetUDPConnection(UDP_Connection *connection);
	Player *FindPlayerFromIP(long IPaddr, long player_index);
    Player *FindPlayerFromIP(long IPaddr);

    // Player utility Methods
	Player *GetPlayer(long GameID, bool sector_login = false);
	Player *GetPlayer(char * Name);
    long    GetGameIDFromName(char * Name);

    char  * WhoHtml(size_t *response_length);

	//Grouping Methods
	long	GetMemberID(int GroupID, int MemberIndex);
	void	GroupInvite(int GroupID, long LeaderID, long PlayerID);
	void	AcceptGroupInvite(int GroupID, long PlayerID);
	void	RejectGroupInvite(int GroupID, long PlayerID);
	void	LeaveGroup(int GroupID, long PlayerID);
	void	KickFromGroup(int GroupID, long LeaderID, long PlayerID);
	void	DisbanGroup(int GroupID, long PlayerID);
	void	RemoveGroup(int GroupID);
	int		GetMemberCount(int GroupID);
	void	RequestGroupAux(int GroupID, int PlayerID);
	void	GroupChat(int GroupID, char * SenderName, char * Message);
	bool	GroupXP(Player *owner, const char *, int XP_Gain, int GroupID, int Type);
    void    GroupCombatXP(Player *owner, char *, int MOB_level);
    int     GetGroupFromPlayer(long PlayerID);
    bool    CheckGrouped(Player *p1, Player *p2);
	float	GetGroupWarpSpeed(Player *p);
	bool 	GroupAction(long SourceID, long TargetID, long Action);
	bool 	SetFormation(long leaderID, long formation, const char* formation_name);
	bool	FormUp(long PID);
	bool	LeaveFormation(long PID);
	bool	CheckGroupFormation(Player *p);
	bool	SendFormation(Player *SendP, Player *TargetP);
	bool 	BreakFormation(long leaderID);
	bool 	RequestTargetMyTarget(long leaderID, long targetID);
	void	FormationEngineOperation(Player *p, bool engine);

private:
	int		GetMemberCount(Group * myGroup);
	int		GetMembersInXPRange(Player * player, Group * myGroup);
	void	DisbanGroup(Group *myGroup);
	void	RemovePlayerFromGroup(Group * myGroup, int MemberIndex);
	void	SendEmptyGroupAux(long PlayerID);
	void	SendAuxToGroup(Group * myGroup);
	void	SetGroupInfoMembersFromGroup(_GroupInfo & myGroupInfo, Group * myGroup, int MemberIndex);
	Group * GetGroupFromID(int GroupID);
	void	SendGroupInvite(Group * myGroup, Player *c);
	void	GetPostFix(Player *p, char *FName, int length);

private:
    void    RemovePlayerFromGlobalList(Player *player);

private:
	Group		* m_GroupList;
	long		  m_NextGroup;
    Mutex		  m_Mutex;
	bool		  m_Movement_thread_running;
	bool		  m_Save_thread_running;
    u32			  m_GlobalPlayerList[MAX_ONLINE_PLAYERS/32 + 1];
    UDP_Connection * m_UDPConnection;
    GMemoryHandler * m_GlobMemMgr;
#ifdef WIN32
    HANDLE		  m_PlayerManagerThreadHandle;
#else
    pthread_t	  m_PlayerManagerThreadHandle;
#endif
};

#endif // _PLAYER_MANAGER_H_INCLUDED_
