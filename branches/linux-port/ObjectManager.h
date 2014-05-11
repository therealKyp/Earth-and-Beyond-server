// ObjectManager.h

#ifndef _OBJECT_MANAGER_H_INCLUDED_
#define _OBJECT_MANAGER_H_INCLUDED_

#ifdef WIN32
#pragma warning(disable:4786)
#endif

#include "Connection.h"
#include "ObjectClass.h"
#include "FieldClass.h"
#include "ResourceClass.h"
#include "MOBClass.h"
#include "MOBSpawnClass.h"
#include "PlayerClass.h"
#include "NavTypeClass.h"
#include "MemoryHandler.h"
#include <vector>
#include <map>

struct EffectCancel
{
    long time_delay;
    long effect_id;
};

#define DEFAULT_TEMP_OBJLIST_SIZE 200 //by default have 200 temporary object slots per sector

typedef std::vector<Object*> ObjectList;
typedef std::map<long, Object*> GlobalObjectList;

class ObjectManager
{
// Constructor/Destructor
public:
    ObjectManager();
	virtual ~ObjectManager();

public:
    Object    * AddNewObject(object_type ot, bool static_obj = false);
    Object    * GetObjectFromID(long object_id);
    Object    * GetObjectFromName(char *object_name); //NB this call is slow! Try to avoid use within game loop (used for setting up missions).
    Object    * FindStation(short station_number);
    Object    * FindGate(long gate_id);
    Object    * FindFirstNav();
    Object    * NearestNav(float *position);

    void        SetLockdown(bool lockdown) { m_Lockdown = lockdown; };

	void        DestroyObject(Object *obj, long time_to_destroy, long duration);

    void        SendAllNavs(Player *player);
    void        SendRemainingStaticObjs(Player *player);
	void		MakeDecosClickable(Player *player);

    void        SpawnRandomMOB(float *position);
    void        SpawnSpecificMOB(float *position, short mob_type, short level);

    void        SendObject(Player *player, Object *obj); //this may be better off in private
    void        SendPosition(Player *player, Object *obj); //handy for direct position sends

    void        SetSectorManager(SectorManager *sect_manager);
    SectorManager *GetSectorManager()         { return (m_SectorMgr); };

    void        RemovePlayerFromMOBRangeLists(Player *p);

    void        DisplayDynamicObjects(Player *player, bool all_objects = false);

    void        InitialiseResourceContent();

    void        SectorSetup(ObjectList &stations);

    long        GetAvailableSectorID();

    u32		  *	GetSectorList();

    bool        CheckNavRanges(Player *player);
    void        Explored(Player *player, Object *obj);
    ObjectList *GetMOBList()                { return (&m_MOBSectorList); };

	void		SetObjectsAtRange(Player *p, float range, long *rangelist, long *rangelist2 = 0);

//Movement methods to preserve encapsulation
    void        CalcNewPosition(Object *obj, unsigned long current_time);

private:
    long        GetNewGameID();
    void        HandleTempCreation(Object *object);
    void        AssignStaticID(Object *object);
    short       HandleObjectUpdate(Player *player, Object *obj);
    short       HandleMOBUpdate(Player *player, Object *obj);
    short       HandleFieldUpdate(Player *player, u32 index, ObjectList *sector_list);
    void        RemoveAllFieldAsteroids(Player *player, u32 index, ObjectList *sector_list);
    Object    * AddNewMOB(bool static_obj);
    Object    * AddNewField(bool static_obj);
    Object    * AddNewNav(bool appears_in_radar=false);
    Object    * AddNewResource(bool temp);
    Object    * AddNewMOBSpawn();

    void        CheckCreationTarget(Object *obj);

	ObjectList *GetDynamicSectorList();

private:
    ObjectList  m_StaticSectorList;   //all static sector objects: navs, starbases. These objects are only ever sent at first connection
    ObjectList  m_DynamicSectorList;  //all sector object that require range handling, eg asteroids etc
    ObjectList  m_SectorIndexList;    //list of pointers to each object that is clickable in the sector. This gives instant translation from GameID to Object
    ObjectList  m_MOBSectorList;      //all MOBs that require more specialised range handling. It'd be a waste of time and power to do this for all objects, but when
                                      //we're running the server on Terrabit RAM with Quantum processors clocking at 'fuck off' speeds it'll be fine.

	ObjectList	m_DynamicSectorListSnapshot1;  //these are snapshots we can use that are guaranteed not to change while we're using them
	ObjectList	m_DynamicSectorListSnapshot2;
	bool		m_UseDynamicListSnapshot1;
	u32			m_ObjectListUpdated;

    MemorySlot<Resource>    *m_TempResources;   //Re-usable memory for temp resources (hulks, asteroids, corpses, anything temp the GMs toss in etc).

    //methods involved in sending object data
    void        SendCreateInfo(Player *player, Object *obj);
    void        SendObjectEffects(Player *player, Object *obj);
    void        SendRelationship(Player *player, Object *obj);
    void        SendAuxDataPacket(Player *player, Object *obj);
    void        SendNavigation(Player *player, Object *obj);

    long        m_StartObjectID;
    long        m_NumberOfObjects;
    long        m_StartSectorFXID;
    long        m_SectorFXID;

    long        m_SectorMOBCount;

    bool        m_Lockdown;                 //lockdown is imposed when we create asteroid fields, to avoid MOBs/Player temporary stuff etc getting into field space
    SectorManager * m_SectorMgr;
};

extern GlobalObjectList g_SectorObjects;

#endif // _OBJECT_MANAGER_H_INCLUDED_
