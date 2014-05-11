// ObjectManager.cpp

#include "Net7.h"
#include "ObjectManager.h"
#include "stdlib.h"

#define MAX_OBJECTS_IN_SECTOR 16384
#define MAX_STATIC_OBJECTS_IN_SECTOR 400

GlobalObjectList g_SectorObjects;

ObjectManager::ObjectManager()
{
    m_StartObjectID     = 100000;
    m_StartSectorFXID   = 1000000;
    m_SectorFXID        = 0;
    m_NumberOfObjects   = 0;
    m_TempResources     = new MemorySlot<Resource>(DEFAULT_TEMP_OBJLIST_SIZE); //create slotted array of temporary objects

    m_SectorMOBCount    = 0;

	m_SectorMgr			= 0;

	m_UseDynamicListSnapshot1 = false;
	m_ObjectListUpdated = 0;

    m_Lockdown = false;
}

ObjectManager::~ObjectManager()
{
    //TODO: go through deleting each object for sector list (temp list gets deconstructed elsewhere)
	m_StaticSectorList.clear();
    m_DynamicSectorList.clear();
    m_MOBSectorList.clear();
}

Object * ObjectManager::AddNewMOB(bool static_obj) //if static obj is set true this will be a permanent MOB
{
    long object_index; 
    long object_id;
    MOB *object;

    //TODO: mutex this

    /*if (!static_obj)
    {
        object = m_TempMOBs->GetNode();
        HandleTempCreation(object); //destroy the object if necessary
        object_index = object->GetObjectIndex();
        object_id = object->GameID();
    }
    else*/
    {
        object_index = m_NumberOfObjects;
        object_id = GetNewGameID();
        object = new MOB(object_id);
        //Add to MOB list
        m_MOBSectorList.push_back(object);
        //m_DynamicSectorList.push_back(object);
    }

    object->SetObjectIndex(object_index);
    object->SetActive(true);
    object->SetObjectManager(this);

    m_SectorIndexList.push_back(object);
    m_SectorIndexList[object_index] = object;

    m_SectorMOBCount++;

    return (object);
}

Object * ObjectManager::AddNewMOBSpawn() 
{
    long object_index; 
    long object_id;
    MOBSpawn *object;

    //TODO: mutex this

    object_index = m_NumberOfObjects;
    object_id = GetNewGameID();
    object = new MOBSpawn(object_id);
    m_MOBSectorList.push_back(object);
    //m_DynamicSectorList.push_back(object);

    object->SetObjectIndex(object_index);
    object->SetActive(true);
    object->SetObjectManager(this);

    m_SectorIndexList.push_back(object);
    m_SectorIndexList[object_index] = object;

    return (object);
}

Object * ObjectManager::AddNewField(bool static_obj) //if static obj is set true this will be a permanent field.
{
    long object_index; 
    long object_id;
    Field *object;

    //TODO: mutex this

    /*if (!static_obj)
    {
        object = m_TempMOBs->GetNode();
        HandleTempCreation(object); //destroy the object if necessary
        object_index = object->GetObjectIndex();
        object_id = object->GameID();
    }
    else*/
    {
        object_index = m_NumberOfObjects;
        object_id = GetNewGameID();
        object = new Field(object_id);
        m_DynamicSectorList.push_back(object);
    }

    object->SetObjectIndex(object_index);
    object->SetActive(true);
    object->SetObjectManager(this);

	object->SetBasset(2241); //use generic nav buoy
	object->SetScale(3.0f);
	object->SetSignature(40000.0f);

    m_SectorIndexList.push_back(object);
    m_SectorIndexList[object_index] = object;

    return (object);
}

Object * ObjectManager::AddNewResource(bool static_obj) //if static obj is set true this will be a permanent asteroid
{
    long object_index; 
    long object_id;
    Resource *object;

    //mutex this

    if (!static_obj)
    {
        object = this->m_TempResources->GetNode();
        HandleTempCreation(object); //destroy the object if necessary
        object_index = object->ObjectIndex();
        object_id = object->GameID();
    }
    else
    {
        object_index = m_NumberOfObjects;
        object_id = GetNewGameID();
        object = new Resource(object_id);
        m_DynamicSectorList.push_back(object);
    }

    object->SetObjectIndex(object_index);
    object->SetActive(true);
    object->SetObjectManager(this);

    m_SectorIndexList.push_back(object);
    m_SectorIndexList[object_index] = object;

    return (object);
}

Object * ObjectManager::AddNewObject(object_type ot, bool static_obj)
{
    Object *object;
    switch (ot)
    {
        case OT_STATION :          
        case OT_STARGATE:       
        case OT_PLANET  :
            object = AddNewNav(true);
            break;
        case OT_CAPSHIP :
        case OT_DECO    :
        case OT_NAV     :   
            object = AddNewNav();
            break;

		case OT_RESOURCE:
            object = AddNewResource(static_obj);
            break;

        case OT_MOB :	
            object = AddNewMOB(static_obj);
            break;

        case OT_MOBSPAWN:
            object = AddNewMOBSpawn();
            break;

        case OT_FIELD:
			object = AddNewField(true);
			break;

        case OT_HULK:
        case OT_HUSK:
            object = AddNewResource(static_obj);
            break;        

        case OT_FLOATING_ORE:
            object = AddNewResource(false);
            break;

        default:
            LogMessage("ObjectManager::AddNewObject - Attempted to create an invalid object [%d]\n", ot);
            return (Object*)(0);
            break;
    }

    if (object)
    {
        object->SetObjectType(ot);
        object->SetObjectManager(this);
    }

    return (object);
}

Object * ObjectManager::AddNewNav(bool appears_in_radar)
{
    StaticMap *object = new StaticMap(0);
    m_StaticSectorList.push_back(object); //add nav to static sector list
    AssignStaticID(object);
    object->SetObjectManager(this);
    object->SetActive(true);
    if (appears_in_radar) object->SetAppearsInRadar();
    return (object);
}

void ObjectManager::AssignStaticID(Object *object)
{
    long object_index = m_NumberOfObjects;
    long object_id = GetNewGameID();

    object->SetGameID(object_id);
    object->SetObjectIndex(object_index);

    m_SectorIndexList.push_back(object);
    m_SectorIndexList[object_index] = object; //assign spot in object index
}

long ObjectManager::GetNewGameID()
{
    long game_id = m_StartObjectID + m_NumberOfObjects;
    ++m_NumberOfObjects;
    return (game_id);
}

void ObjectManager::HandleTempCreation(Object *object)
{
    if (object->ObjectIndex() == -1)
    {
        //object slot hasn't been used yet
        object->SetObjectIndex(m_NumberOfObjects);
        object->SetGameID(GetNewGameID());
        m_DynamicSectorList.push_back(object); //add to sector list
    }
    else if (object->Active())
    {
        //has been used, and object is currently active
        object->SetRequiresReset(true);
    }
    else
    {
        //has been used, but has been subsequently destroyed correctly
        object->SetActive(true);
        m_DynamicSectorList.push_back(object); //add to sector list
    }
}

//These two methods are an attempt to cut down on sector load times
//First we send all normal navs, there's never too many of these
//and any Deco that's within 10000k
void ObjectManager::SendAllNavs(Player *player)
{
    ObjectList::iterator itrOList;
    Object *obj;

	for (itrOList = m_StaticSectorList.begin(); itrOList < m_StaticSectorList.end(); ++itrOList) 
	{
        obj = (*itrOList);
        if (obj->ObjectType() != OT_DECO)
        {
            if ((obj->ObjectType() == OT_PLANET) || obj->GetEIndex(player->ExposedNavList()) )
            {
                obj->SendObject(player);
                obj->SetEIndex(player->ExposedNavList());
                obj->SetIndex(player->ObjectRangeList());
            }
        }
	}
}

//Now we stream the DECOs in as and when they are needed (10k from sig should be fine).
//This should cut the loading times right down
void ObjectManager::SendRemainingStaticObjs(Player *player)
{
    ObjectList::iterator itrOList;
    Object *obj;

	for (itrOList = m_StaticSectorList.begin(); itrOList < m_StaticSectorList.end(); ++itrOList) 
	{
        obj = (*itrOList);
        if (obj->ObjectType() == OT_DECO && !obj->GetIndex(player->ObjectRangeList()) &&
           (obj->RangeFrom(player->Position()) < (obj->Signature() + 10000.0f) ) )
        {
			player->ExposeDecosOn(obj);

            obj->SendObject(player);
            obj->SetIndex(player->ObjectRangeList());

			player->ExposeDecosOff(obj);
        }
	}
}

bool ObjectManager::CheckNavRanges(Player *player)
{
    ObjectList::iterator itrOList;
    Object *obj;
    float range;
    float explore_range = 3000.0f;
    bool hidden;
    bool unexplored;
    bool unexplored_navs = false;
    float scan_range = (float)player->ShipIndex()->CurrentStats.GetScanRange();

    for (itrOList = m_StaticSectorList.begin(); itrOList < m_StaticSectorList.end(); ++itrOList) 
    {
        obj = (*itrOList);
        if (obj->ObjectType() != OT_DECO && obj->ObjectType() != OT_PLANET)
        {
            hidden =     !obj->GetEIndex(player->ExposedNavList());
            unexplored = !obj->GetEIndex(player->ExploredNavList());
            
            //do we need to check this nav?
            if (hidden || unexplored)
            {
                range = obj->RangeFrom(player->Position());
                
                if (hidden)
                {
                    if ( (obj->AppearsInRadar() && range < (obj->RadarRange() + scan_range) ) || //if this is a minimap object, uncover as '?' at 'RadarRange + scanrange'
                         (range < (obj->Signature())) ) //if it's not minimap, uncover when it's in visual range
                    {               
                        //object is exposed
                        obj->SetEIndex(player->ExposedNavList());
						player->SaveDiscoverNav(obj->GetDatabaseUID());
                        //send the static nav
                        obj->SendObject(player);
                    }
                }
                
                if (obj->Signature() < explore_range) explore_range = obj->Signature();
                
                if (range < explore_range)
                {
                    obj->SetEIndex(player->ExploredNavList());
                    obj->SendNavigation(player);
                    player->AwardNavExploreXP(obj);
                }
                
                unexplored_navs = true;
            }
        }
    }

    return unexplored_navs;
}

void ObjectManager::Explored(Player *player, Object *obj)
{
    if (!obj->GetEIndex(player->ExploredNavList()) )
    {
        obj->SetEIndex(player->ExploredNavList());
        obj->SendNavigation(player);
        player->AwardNavExploreXP(obj);
    }
}

Object *ObjectManager::GetObjectFromID(long object_id)
{
    Object *obj = (0);

    if (object_id >= m_StartObjectID && object_id < (m_StartObjectID + m_NumberOfObjects))
    {
        obj = m_SectorIndexList[object_id - m_StartObjectID];
        obj->SetLastAccessTime(GetNet7TickCount());
    }
    else if (IS_PLAYER(object_id))
    {
        obj = g_GlobMemMgr->GetPlayer(object_id);
    }

    return (obj);
}

//NB object_name must be in string manager
Object *ObjectManager::GetObjectFromName(char *object_name)
{
    Object *obj = (0);
    ObjectList::iterator itrOList;

	for (itrOList = m_StaticSectorList.begin(); itrOList < m_StaticSectorList.end(); ++itrOList) 
	{
        if ((*itrOList)->Name() == object_name)
        {
            return (*itrOList);
        }
	}

	ObjectList *sector_list = GetDynamicSectorList();

    //this assumes the object name is in the string manager
	for (itrOList = sector_list->begin(); itrOList != sector_list->end(); ++itrOList) 
	{
        if ((*itrOList)->Name() == object_name) //using string manager the ptrs will match
        {
            return (*itrOList);
        }
    }

    return (0);
}

void ObjectManager::SetSectorManager(SectorManager *sect_manager)
{
    m_SectorMgr = sect_manager;
}

//need to ensure that only one of these is running per sector at any time. This can be ensured by only sequentially processing players
void ObjectManager::DisplayDynamicObjects(Player *player, bool all_objects)
{
    float scan_range = (float)(player->ShipIndex()->CurrentStats.GetScanRange()); //TODO: should be in Player class
	ObjectList::iterator itrOList;
	bool flag_erase = false;
    Object *obj;
	unsigned long current_tick = GetNet7TickCount();
	short broadcast_count = 0;
    short broadcast_max = player->WarpDrive() ? 3 : 10;
	u32 list_size = 0;
	u32 index = 0;

    m_Lockdown = true; //we don't want any object creation while we're scanning the list

	ObjectList *sector_list = GetDynamicSectorList();
	list_size = sector_list->size();

	while (sector_list->size() == list_size && index < list_size)
	{
        obj = (*sector_list)[index];

        if (obj->RespawnTick() == 0)
        {
            switch (obj->ObjectType())
            {
            case OT_RESOURCE:
            case OT_HULK:
            case OT_FLOATING_ORE:
            case OT_HUSK:
                broadcast_count += HandleObjectUpdate(player, obj);
                break;
                
            case OT_FIELD:
                index += HandleFieldUpdate(player, index, sector_list);
                break;

            case OT_MOBSPAWN:
                break;
                
            default:
                LogMessage("WARNING: unhandled object '%s' [%d] in %s sector\n", obj->Name(), obj->GameID(), m_SectorMgr->GetSectorName(m_SectorMgr->GetSectorID()) );
                break;
            }
        }
        else if (obj->RespawnTick() == -1 || obj->IsToBeRemoved())
        {
			obj->SetActive(false);
			flag_erase = true;
        }
        else if (current_tick > obj->RespawnTick())
        {
            obj->ResetResource();
        }
	
		++index;

        if (!all_objects && broadcast_count > broadcast_max)
        {
            break;
        }
	}

	if (flag_erase)
	{
		for (itrOList = m_DynamicSectorList.begin(); itrOList != m_DynamicSectorList.end(); ++itrOList)
		{
			obj = (*itrOList);
			if (obj->RespawnTick() == -1 || obj->IsToBeRemoved())
			{
				m_DynamicSectorList.erase(itrOList); //only remove one object at a time
				break;
			}
		}
	}

	m_Lockdown = false;

    SendRemainingStaticObjs(player);
}

short ObjectManager::HandleObjectUpdate(Player *player, Object *obj)
{
	if (!obj->Active()) return 0; //early return for inactive objects

    float scan_range = (float)(player->ShipIndex()->CurrentStats.GetScanRange());
    short broadcast_count = 0;
    if (player->GrailAffinity() && obj->Level() >= 9 && obj->BaseAsset() == 0x726) //grail affinity device
    {
        scan_range += 12000.0f;
    }

    bool is_active = obj->GetIndex(player->ObjectRangeList());
    bool in_range = obj->IsInRange(player->Position(), scan_range, is_active);

    if (in_range && !is_active)
    {
        obj->SetIndex(player->ObjectRangeList());
        obj->SendObject(player);
        broadcast_count = 1;
    }
    else if (!in_range && is_active)
    {
        obj->UnSetIndex(player->ObjectRangeList());
        obj->UnSetIndex(player->ResourceSendList()); //this should fix the issue with empty looking but non-empty roids.
        player->RemoveObject(obj->GameID());
    }

    return broadcast_count;
}

short ObjectManager::HandleMOBUpdate(Player *player, Object *obj)
{
    short broadcast_count = 0;
    //we need to traverse the MOB's rangelist and see 
    return broadcast_count;
}

short ObjectManager::HandleFieldUpdate(Player *player, u32 index, ObjectList *sector_list)
{
    short object_advance = 0;
    Object *obj = (*sector_list)[index];

	if (obj && !obj->Active()) return obj->FieldCount(); //early return for inactive fields

    float scan_range = (float)(player->ShipIndex()->CurrentStats.GetScanRange()); //TODO: should be in Player class (via AuxPlayer method)
    bool is_active = obj->GetIndex(player->ObjectRangeList());
    bool in_range = obj->IsInRange(player->Position(), scan_range, is_active);

    if (in_range && !is_active)
    {
        obj->SetIndex(player->ObjectRangeList());
		//display field centre
		if (player->AdminLevel() >= 80)	obj->SendObject(player);
    }
    else if (!in_range)
    {
        if (is_active)
        {
            obj->UnSetIndex(player->ObjectRangeList());
            RemoveAllFieldAsteroids(player, index, sector_list);
			if (player->AdminLevel() >= 80)	player->RemoveObject(obj->GameID());
        }
        object_advance = obj->FieldCount(); //set to last object in field
    }

    return (object_advance);
}

void ObjectManager::RemoveAllFieldAsteroids(Player *player, u32 index, ObjectList *sector_list)
{
    Object *obj = (*sector_list)[index];
    short field_count = obj->FieldCount();
    short i;
    index++;

	//loop through all asteroids, ensure they are all removed
	for (i = 0; i < field_count && index < sector_list->size(); ++index, ++i)
	{
        obj = (*sector_list)[index];
		if (obj->GetIndex(player->ObjectRangeList()))
		{
            player->RemoveObject(obj->GameID());			
            obj->UnSetIndex(player->ObjectRangeList());
		}
	}
}

//This function will either set a timer to destroy an object, or will do it immediately
void ObjectManager::DestroyObject(Object *obj, long time_to_destroy, long duration)
{
    if (duration == -1)
    {
        obj->SetRespawnTick(-1); //This schedules the object for destruction from sector object list
        obj->SetRemove();
    }
    else
    {
        obj->SetRespawnTick(GetNet7TickCount() + duration);
    }

    if (time_to_destroy != 0)
    {
        m_SectorMgr->AddTimedCall(0, B_DESTROY_RESOURCE, time_to_destroy, obj, duration); //set a timer to destroy the asteroid, so we can see the destroy FX
    }
    else
    {
        //now handle graphic removal
        obj->Remove();
    }
}

void ObjectManager::InitialiseResourceContent()
{
    ObjectList::iterator itrOList;
    Object *obj;

    m_Lockdown = true; //we don't want any object creation while we're scanning the list

	for (itrOList = m_DynamicSectorList.begin(); itrOList < m_DynamicSectorList.end(); ++itrOList) 
	{
        obj = (*itrOList);

        switch (obj->ObjectType())
        {
            case OT_RESOURCE:
            case OT_HULK:
            case OT_HUSK:
                obj->ResetResource();
                break;

            default:
                break;
        }

	}
}

Object * ObjectManager::FindStation(short station_number)
{
    ObjectList::iterator itrOList;

    if (station_number == 0) station_number = 1;

	for (itrOList = m_StaticSectorList.begin(); itrOList < m_StaticSectorList.end(); ++itrOList) 
	{
        if ( ((*itrOList)->ObjectType() == OT_STATION) )
        {
            return (*itrOList);
        }
	}

    return (0);
}

Object * ObjectManager::FindGate(long gate_destination)
{
    ObjectList::iterator itrOList;

	for (itrOList = m_StaticSectorList.begin(); itrOList < m_StaticSectorList.end(); ++itrOList) 
	{
        if ( ((*itrOList)->Destination() == gate_destination) || 
            (gate_destination == 0 && (*itrOList)->Destination() > 0) )
        {
            return (*itrOList);
        }
	}

    return (0);
}

Object * ObjectManager::FindFirstNav()
{
    ObjectList::iterator itrOList;

	for (itrOList = m_StaticSectorList.begin(); itrOList < m_StaticSectorList.end(); ++itrOList) 
	{
        if ( (*itrOList)->ObjectType() == OT_NAV  )
        {
            return (*itrOList);
        }
	}

    return (0);
}

//This method is called once at sector startup
void ObjectManager::SectorSetup(ObjectList &stations)
{
    ObjectList::iterator itrOList;
    Object *obj;
    Object *last_obj = 0;

	for (itrOList = m_StaticSectorList.begin(); itrOList < m_StaticSectorList.end(); ++itrOList) 
	{
        obj = (*itrOList);
        if (obj->ObjectType() == OT_STATION)
        {
            if (last_obj == 0 || obj->RangeFrom(last_obj->Position()) > 40000.0f)
            {
                stations.push_back(obj);
                obj->SetBroadcastID(obj->GetBroadcastID());
                last_obj = obj;
            }
        }
        else if (obj->BroadcastID() > 0)
        {
            stations.push_back(obj);
            LogMessage("Setting up effect for %s [%d]\n", obj->Name(), obj->BroadcastID());
        }
	}

#if 0 //TODO: use MOB spawn in editor
    //hardcoded mission MOB for now
    if (m_SectorMgr->GetSectorID() == 1070)
    {
        float pos[3] = {358840.0f,26650.0f,-2070.0f};
        
        SpawnSpecificMOB(pos, MOB_RedDragonPatrolFighter, 25);
        pos[2] = -2200.0f;
        SpawnSpecificMOB(pos, MOB_RedDragonPatrolFighter, 25);
        pos[2] = -1000.0f;
        SpawnSpecificMOB(pos, MOB_RedDragonPatrolFighter, 25);
        pos[2] = -1500.0f;
        SpawnSpecificMOB(pos, MOB_RedDragonPatrolFighter, 25);
    }
#endif
}

long ObjectManager::GetAvailableSectorID()
{
    if (m_SectorFXID == 0 || m_SectorFXID > 0x0FFFFFFF)
    {
        m_SectorFXID = m_StartSectorFXID;
    }

    return m_SectorFXID++;
}

void ObjectManager::CalcNewPosition(Object *obj, unsigned long current_time)
{
    obj->CalcNewPosition(current_time);
}

Object *ObjectManager::NearestNav(float *position)
{
    ObjectList::iterator itrOList;
    Object *obj;

	if (!m_SectorMgr || m_SectorMgr->GetSectorID() > 9999 || m_StaticSectorList.size() == 0)
	{
		return (0);
	}

    Object *closest_obj = m_StaticSectorList[0];

    if (!closest_obj)
    {
        return (0);
    }

	for (itrOList = m_StaticSectorList.begin(); itrOList < m_StaticSectorList.end(); ++itrOList) 
	{
        obj = (*itrOList);
        if (obj->ObjectType() != OT_DECO && 
            obj->RangeFrom(position) < closest_obj->RangeFrom(position))
        {
            closest_obj = obj;
        }
	}

    return closest_obj;
}

void ObjectManager::SetObjectsAtRange(Player *p, float range, long *rangelist, long *rangelist2)
{
    ObjectList::iterator itrOList;
    Object *obj;
	float *position = p->Position();
	bool in_range;
	bool in_range2;

	if (!m_SectorMgr || m_SectorMgr->GetSectorID() > 9999 || m_StaticSectorList.size() == 0)
	{
		return;
	}

	for (itrOList = m_StaticSectorList.begin(); itrOList < m_StaticSectorList.end(); ++itrOList) 
	{
        obj = (*itrOList);
		in_range = obj->GetIndex(rangelist);
		in_range2 = rangelist2 ? obj->GetIndex(rangelist2) : true; //if we aren't in range of the bigger range list, don't bother checking

		if (in_range2)
		{
			if (in_range && obj->RangeFrom(position) > range + 200.0f)
			{
				obj->UnSetIndex(rangelist);
				obj->OutOfRangeTrigger(p, range);
			}
			else if (obj->RangeFrom(position) < range)
			{
				obj->SetIndex(rangelist);
				obj->InRangeTrigger(p, range);
			}
		}
	}
}

void ObjectManager::RemovePlayerFromMOBRangeLists(Player *p)
{
    ObjectList::iterator itrOList;
    ObjectList * mob_list = GetMOBList();
    Object *mob;
    
    for (itrOList = mob_list->begin(); itrOList < mob_list->end(); ++itrOList) 
    {
        mob = (*itrOList);
        mob->RemovePlayerFromRangeLists(p);
    }
}

ObjectList * ObjectManager::GetDynamicSectorList()
{
	u32 tick_count = GetNet7TickCount();
	
	//ensure objectlist snapshot updates once every second
	if (tick_count >= (m_ObjectListUpdated + 500))
	{
		if (m_UseDynamicListSnapshot1)
		{
			m_DynamicSectorListSnapshot2 = m_DynamicSectorList; //use vector's assignment operator
			m_UseDynamicListSnapshot1 = false;
		}
		else
		{
			m_DynamicSectorListSnapshot1 = m_DynamicSectorList;
			m_UseDynamicListSnapshot1 = true;
		}
		m_ObjectListUpdated = tick_count;
	}

	if (m_UseDynamicListSnapshot1)
	{
		return (&m_DynamicSectorListSnapshot1); 
	}
	else
	{
		return (&m_DynamicSectorListSnapshot2); 
	}
}

u32 *ObjectManager::GetSectorList()
{
	if (m_SectorMgr)
	{
		return (m_SectorMgr->GetSectorPlayerList());
	}
	else
	{
		return (0);
	}
}

#if 0
void ObjectManager::SpawnRandomMOB(float *position)
{
    short mob_type = 0;

    switch (rand()%9)
    {
    case 0:
        mob_type = MOB_EnergyPhoenix;
        break;
    case 1:
        mob_type = MOB_Leviathan;
        break;
    case 2:
        mob_type = MOB_Crystalloid2;
        break;
    case 3:
        mob_type = MOB_Oni;
        break;
    case 4:
        mob_type = MOB_Manta;
        break;
    case 5:
        mob_type = MOB_RedDragonAdvancedFighter;
        break;
    case 6:
        mob_type = MOB_MalefariFighter;
        break;
    case 7:
        mob_type = MOB_NebulaKraken;
        break;
    case 8:
        mob_type = MOB_Worm1;
        break;
    }

    MOB *mob = (MOB*)AddNewObject(OT_MOB); //MOB creation
    
    if (mob)
    {
        mob->SetMOBType(mob_type);
        mob->SetPosition(position);
        mob->MovePosition(-200.0f, 0.0f, -200.0f);
        mob->SetLevel(rand()%20 + 1);
        mob->SetActive(true);
        mob->SetRespawnTick(0);
        mob->SetHostileTo(OT_PLAYER);
        mob->SetVelocity(210.0f);
        mob->SetUpdateRate(50);
        float turn = (float)((rand()%11) - 5) * 0.01f;
        if (turn == 0)
        {
            turn = 0.05f;
        }
        mob->Turn(turn);
        mob->SetDefaultStats(turn, MobData[mob_type].behaviour, 210.0f, 50);
    }
}

void ObjectManager::SpawnSpecificMOB(float *position, short mob_type, short level)
{
    MOB *mob = (MOB*)AddNewObject(OT_MOB); //MOB creation
    
    if (mob)
    {
        mob->SetMOBType(mob_type);
        mob->SetPosition(position);
        mob->SetLevel(level);
        mob->SetActive(true);
        mob->SetRespawnTick(0);
        mob->SetHostileTo(OT_PLAYER);
        mob->SetVelocity(210.0f);
        mob->SetUpdateRate(50);
        mob->SetSignature(10000.0f);
        float turn = (float)((rand()%11) - 5) * 0.01f;
        if (turn == 0)
        {
            turn = 0.05f;
        }
        mob->Turn(turn);
        mob->SetDefaultStats(turn, MobData[mob_type].behaviour, 210.0f, 50);
    }
}
#endif