// SectorManager.h

#ifndef _SECTOR_MANAGER_H_INCLUDED_
#define _SECTOR_MANAGER_H_INCLUDED_

#include "Mutex.h"
#include "PlayerManager.h"
//#include "MemoryHandler.h"
#include "TimeNode.h"
#include "StationLoader.h"
#include <vector>

struct CharacterDatabase;
class ServerManager;
struct SectorData;
class MemoryHandler;
class SectorMemoryManager;
class Object;

typedef std::vector<TimeNode*> TimeNodeVec;
typedef std::vector<Object*> ObjectList;



enum object_type
{
	OT_NONE, OT_STATION, OT_STARGATE, OT_OBJECT, OT_RESOURCE, OT_PLAYER, OT_HULK, 
    OT_ABS, OT_FLOATING_ORE, OT_MOB, OT_NAV, OT_PLANET, OT_FIELD, OT_CAPSHIP, OT_DECO,
    OT_HUSK, OT_MOBSPAWN, OT_GROUP
};

enum sector_type
{
    ST_SPACE = 0, ST_PLANET, ST_GAS_GIANT
};

struct StationBroadcastNode
{
    Object *station;
};

#ifdef _WIN32
void __cdecl RunEventThreadAPI(void *arg);
#endif
//TODO: sort out linux

// There is one instance of this class for each sector


//This defines how long in seconds we want timeslots to be used
//before we switch back to the older style of timenodes.
#define TIMESLOT_DURATION 60

#define LONG_TERM_NODES 500

class SectorManager
{
public:
	SectorManager(ServerManager *server_mgr);
	virtual ~SectorManager();

public:
	int		m_EffectID;				// ID used for each effect

    void    SetSectorData(SectorData * sector_list);
    void    StartListener(short port);
	bool	RegisterSectorServer(short first_port, short max_sectors);
	bool	SetupSectorServer(long sector_id);
    long    GetSectorID();
    short   GetTcpPort();
    bool    IsSectorServerReady();
    void    SetSectorServerReady(bool ready);
    bool    HandleSectorLogin(Player *player);
	void	HandleSectorLogin2(Player *player); //split the actual login over 2 cycles. This helps to conserve buffer space
    char *  GetSystemName(long sector_id);
    char *  GetSectorName(long sector_id);
    long    GetSectorIDFromName(char *sector_name);
	void	GateJump(Player *player);
	void	DestroyMOB(Object *mob, long player_id);
	void	RemoveMOB(Object *mob);
	void	SetBoundaries(int sector);
	void	CheckMissionPlayerLocation(Player *player, long GameID);
    ServerParameters *GetSectorParams();
    void    ProcessMOBs(u32 current_tick, long check_tick, bool handle_attacks);

	// client calls
	void	SendServerParameters(Player *player);
    void    LaunchIntoSpace(Player *player);
    void    Dock(Player *player, long target);
    void    Gate(Player *player, long target);
	void	GateActivate(Player *player, long target);
	void	SectorServerHandoff(Player *player, int sector_id);
	Object *GetObject(long object_id);
    Object *GetObject(char *object_name);
    float	PlayerSeparation(long object_id, Player *player, object_type = OT_OBJECT);
	bool	GetBitEntry(long *bit_array, long index);
	void	SetBitEntry(long *bit_array, long index);
	void	UnsetBitEntry(long *bit_array, long index);

	TimeNode * AddTimedCall(Player *player, broadcast_function func, long time_offset, Object *obj, long i1 = 0, long i2 = 0, long i3 = 0, long i4 = 0, char *ch = 0, float a = 0.0f);
    void    AddTimedCallPNode(TimeNode *node, unsigned long time_offset);
    void    AddTimedCallPNode(TimeNode *this_node, broadcast_function func, long time_offset, Object *obj, long i1 = 0, long i2 = 0, long i3 = 0, long i4 = 0, char *ch = 0, float a = 0.0f);
	void	MakeTimedCall(TimeNode *node);
	void	RemoveTimedCall(TimeNode *node, bool force = false);
	void	RunSectorEventThread();
	void	InitialiseSector();
	void	SlotTimedCall(TimeNode *node, unsigned long time_offset);

    long    GetSectorNextObjID();
	
	//all client->sector broadcast calls which can be placed on the timer queue
	void	RechargeReactor(Player *player);
	void	RechargeShield(Player *player);
	void	ForceLogout(Player *player);
	void	BuffTimeout(Player *player);

    u32	  * GetSectorPlayerList();
    void    AddPlayerToSectorList(Player *player);
    void    RemovePlayerFromSectorList(Player *player);

    ObjectManager * GetObjectManager()      { return (m_ObjectMgr); };

    void    SetSectorNumber(short number)   { m_SectorNumber = number; };
    short   GetSectorNumber()               { return (m_SectorNumber); };
	long	GetSectorType()					{ return (m_SectorData->sector_type); };

private:
    void    StationLogin(Player *player);
    void    SectorLogin(Player *player);
	void	StationLogin2(Player *player);
	void	SectorLogin2(Player *player);
    bool    CallSlotEvents(long index);
	bool	CallLongTermEvents(long index);
    long    GetSlotIndex(long time_offset);

	void	SetIndex(u32 index);
	void	UnSetIndex(u32 index);
	bool	GetIndex(u32 index);

private:
    Mutex           m_Mutex;
	int				m_ShutDownMark;			// Makes the shutdown Message
	int		        m_SectorID;             // Sector ID for THIS sector
    int             m_SystemID;             // System ID for THIS sector
    SectorData    * m_SectorData;           // Pointer to Sector Data for THIS sector
    ServerManager * m_ServerMgr;
    UDP_Connection* m_SectorConnection;     // UDP connection for sector
    short           m_TcpPort;
	bool	        m_IsSectorServerReady;
    short           m_SectorNumber;         // primarily used to index into the player explored nav lists

    char          * m_SectorName;			// or station name, eg. Earth Station
    char          * m_SystemName;			// eg. Sol
    char          * m_ParentSectorName;		// Sector name for Stations, i.e. Earth
    char          * m_Greetings;			// Greetings upon entering station or sector

	TimeNode	  * m_CoarseEventSlots[LONG_TERM_NODES];// 200 additional slots for coarse memory usage

	TimeNode	  * m_EventSlots[TIMESLOT_DURATION*10][100]; //100 slots per 100ms timeslot
	u8				m_EventSlotsIndex[TIMESLOT_DURATION*10]; //index into timeslot
    long            m_EventSlotIndex;

	u32				m_check_tick;
	ObjectList		m_Stations;

	MemoryHandler * m_MemoryMgr;
	SectorMemoryManager * m_ObjectMemMgr;

    u32				m_PlayerList[MAX_ONLINE_PLAYERS/32 + 1];           // this is a list of players in the sector
	u32				m_PlayerCount;

    ObjectManager * m_ObjectMgr;

	bool			m_SectorThreadRunning;

	float			m_xmin;
	float			m_xmax;
	float			m_ymin;
	float			m_ymax;
	float			m_xctr;
	float			m_yctr;
};


#endif // _SECTOR_MANAGER_H_INCLUDED_

