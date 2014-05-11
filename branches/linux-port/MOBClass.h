// MOBClass.h

#ifndef _MOB_CLASS_H_INCLUDED_
#define _MOB_CLASS_H_INCLUDED_

#include "AuxClasses/AuxMobIndex.h"
#include "ObjectClass.h"
#include "MOBDatabase.h"

class Player;

typedef std::vector<Object*> ObjectList;

#define HATE_SIZE 4

struct Hate_Info
{
	long GameID;
	long hate;
} ATTRIB_PACKED;

class MOB : public Object
{
public:
    MOB (long object_id);
    virtual ~MOB ();

    unsigned long RespawnTick()                     { return m_Respawn_tick; };
    void        SetRespawnTick(unsigned long respawn)       { m_Respawn_tick = respawn; };

    void        UpdateObject();
    void        SendMOBData(Player *p);

    void        SendToVisibilityList(bool force_update = false);
    void        SendPosition(Player *player);
    void        SendAuxDataPacket(Player *player);
    void        OnCreate(Player *player);
    void        OnTargeted(Player *player);
	void		SendRelationship(Player *player);
	void		SendAggroRelationship(Player *p);
	void		CheckWarningShots(Player *p);

    void        DamageMOB(long game_id, long damage_type, float damage, long inflicted);
    void        SendMobDamage(float damage, long damage_type, long game_id, long inflicted);
    void        DestroyMOB(long game_id);
    void        SendExplosion();
    void        SendShieldUpdate(Player *p);
	void		SendShieldLevel(Player *p);
    void        SendHullUpdate(Player *p);
    void        Remove();
    void        OnRespawn();

    void        SetMOBType(short type);
    void        SetLevel(short type);

	short		GetMOBType()					{ return m_MOB_Type; }
    float       GetHullLevel()                  { return m_HullLevel; }
    float       GetStartHullLevel()             { return m_StartHullLevel; }

	void		AddHate(int GameID, int Hate);		// Add Hate to a player
	void		SubtractHate(int GameID, int Hate);
	void		RemoveHate(int GameID);
	long		GetMaxHateID();

    //Targetting and attacking
    void        ChooseTarget();
	void		LockTarget();
    void        HandleAttack();
    void        SendFXToVisibilityList(long target_id, long source_id, long time_delay, short effect_id);
    void        AddBehaviourObject(Object *obj);
    void        AddBehaviourObject(char *name);
    void        AddBehaviourPosition(float *pos);
    void        SetBehaviour(short new_behaviour);
    void        LostTarget(Player *p = (0));
	void		LostTarget(MOB *b);
	float		GetStealthDetectionLevel();
	float		GetScanSkill()						{return m_ScanSkill;};
	void		SetScanSkill(float NewScanSkill)	{m_ScanSkill = NewScanSkill;};

    //movement
    long        TimeToDestination()                 { return (m_ArrivalTime - GetNet7TickCount()); };
    void        CalcNewHeading(float tdiff);
    void        SetDefaultStats(float turn, short behaviour, float velocity, long update);        //TODO: expand this to set up some initial states

    //rangelist handling
    void        UpdateObjectVisibilityList();
    bool        ObjectInRangeList(ObjectList *object_list, Object *obj);
    void        AddObjectToRangeList(ObjectList *object_list, Object *obj);
    void        RemoveObjectFromRangeList(ObjectList *object_list, Object *obj);
    void        RemovePlayerFromRangeLists(Player *p);
    bool        IsClickedBy(Player *p);

    //MOB specific
    void        UpdateMOB(u32 current_tick, bool handle_attacks);
	void		SendAuxShip(Player * other);

	void		SetSpawnGroup(Object *obj)			{ m_SpawnGroup = obj; };
	Object	  * SpawnGroup()						{ return (m_SpawnGroup); };

private:
    bool        CheckNoAttackingMOBs(Player *p);
    bool        PlayerInRangeList(Player *p_check);
    void        AddPlayerToRangeList(Player *p);
    void        RemovePlayerFromRangeList(Player *p);
    void        SetClickedBy(Player *p);
	void		SendAuxDiff(Player *p, long type);

    void        MOBBehaviour();
    void        TravelToDestination();
    void        UpdatePositionInfo(PositionInformation *pos);
    void        PursueTarget();

private:
	AuxMobIndex		m_ShipIndex;			// Use MOB aux rather than PlayerShip Aux, more efficient and more relevant.
	Hate_Info		m_HateList[HATE_SIZE];	// Hate List for mobs, only track top #HATE_SIZE haterz.
    unsigned long   m_Respawn_tick;
    short           m_MOB_Type;
    unsigned long   m_LastAttackTime;
	MOBData		  * m_MOB_Data;

    float           m_WeaponDPS;
    short           m_DamageType;

	//These won't be required soon (use Aux)
    float           m_StartShieldLevel;
    float           m_ShieldLevel;
    float           m_StartHullLevel;
    float           m_HullLevel;
    float           m_LastTickShieldLevel;
    float           m_LastTickHullLevel;

    u32				m_RangeList[MAX_ONLINE_PLAYERS/32 + 1]; // list of Players who can see this MOB.
	u32				m_ClickedList[MAX_ONLINE_PLAYERS/32 + 1];
	u32				m_PlayerVisibleList[MAX_ONLINE_PLAYERS/32 + 1]; // list of Players visible to this MOB.
    ObjectList      m_MOBVisibleList;       // list of MOBs visible to this MOB.

	Object		  * m_SpawnGroup;			// this points to either a field or MOB spawn, if this MOB is spawned
    ObjectList      m_BehaviourList;        // list of objects that the behviour processing will use.
    float           m_HuntPosition[3];
    u32             m_ArrivalTime;
    short           m_Behaviour;
    short           m_DefaultBehaviour;
    Object        * m_Destination;
    float           m_DestinationPos[3];
    bool            m_DestinationFlag;

	bool			m_Attacking;

    float           m_DefaultTurn;
    float           m_DefaultVelocity;
	float			m_ScanRange;
	float			m_ScanSkill;
    long            m_DefaultUpdateRate;
    short           m_ShieldFXSent;
	u32				m_DamageTime;

    TimeNode        m_DamageNode;
};

typedef enum
{
    CYBERNETIC,
    STRUCTURAL,
    ORGANIC_RED,
    ORGANIC_GREEN,
    CRYSTALLINE,
    ENERGY,
    ROCK_BASED
} MOB_TYPE;

typedef enum
{
	INVALID_BEHAVIOUR,
    PATROL_FIELD,	// only for field guardians
    PATROL_NAV,
    PATROL_POSITION,
    NAV_ROUTE,		// MOB must be given a set of Navs to visit
    HUNT,			// given a point in space the MOB will hunt around this
    CLUSTER,		// will seek out MOBs of the same type
    CURIOUS,		// will check out random objects in space it can see
    PURSUE,			// when in pursue mode MOBs will chase their target
	TURRET,			// Is stationary and will not fire on Players, will attack ALL mobs
	TURRET_ATTACK	// When a turret attacks
} MOB_Behaviour;

struct MOB_Info
{
    char *name;
    short basset;
    short type;
    short behaviour;
    u32   hull;
    u32   shield;
    u8    level;
    float DPS;
} ATTRIB_PACKED;

//MOB_Info* MobData;

#endif // _MOB_CLASS_H_INCLUDED_
