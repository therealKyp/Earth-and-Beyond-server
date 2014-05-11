// ObjectClass.cpp

#include "Net7.h"
#include "ObjectClass.h"
#include "PlayerClass.h"
#include "ObjectManager.h"
#include "StringManager.h"
#include "ServerManager.h"

Object::Object(long object_id)
{
    m_CreateInfo.GameID = object_id;
    m_CreateInfo.Scale = 1.0f;
    m_CreateInfo.BaseAsset = 0;
    m_CreateInfo.HSV[0] = 0;
    m_CreateInfo.HSV[1] = 0;
    m_CreateInfo.HSV[2] = 0;

	m_Signature = 0;
    m_RadarRange = 0;
    m_Position_info.type = POSITION_ADVANCED; //default to advanced position
    m_Type = OT_OBJECT; //default to generic object
    m_Level = 1;
    m_Hostile = OT_NONE; //default to non-hostile
    m_HostilityTargetID = 0;

    m_ObjectIndex = -1;

    m_Effects = (0);

    m_NavInfo = false;
    m_TempObject = false;
    m_ToBeRemoved = false;
    m_RenderState = 0;

    m_Active = false;
    m_LastUpdate = 0;

    BlankPositionInfo();

    m_PlayerHijack = (0);

    m_TargetOnCreate = 0;
    m_ObjectRadius = 0;

    m_ReceivedMovement = false;
    m_UpdateRate = 50;
    m_BroadcastID = 0;
	m_Velocity = 0;
	m_MovementID = 0;

    m_Name = (0);
    memset (&m_Position_info, 0, sizeof(m_Position_info));
}

Object::~Object()
{

}

Object& Object::operator=(const Object& obj) //assignment operator overload for easy Object *x = y; type stuff.
{
    if (this != &obj)
    {
        m_CreateInfo.GameID = obj.m_CreateInfo.GameID;
        m_CreateInfo.BaseAsset = obj.m_CreateInfo.BaseAsset;
        m_CreateInfo.HSV[0] = obj.m_CreateInfo.HSV[0];
        m_CreateInfo.HSV[1] = obj.m_CreateInfo.HSV[1];
        m_CreateInfo.HSV[2] = obj.m_CreateInfo.HSV[2];
        m_CreateInfo.Scale  = obj.m_CreateInfo.Scale;

        m_Signature = obj.m_Signature;
        memcpy(&m_Position_info, &obj.m_Position_info, sizeof(m_Position_info));
        m_Name = obj.m_Name;
    }
    return *this;
}

void Object::SetHSV(float h1, float h2, float h3)
{
    m_CreateInfo.HSV[0] = h1;
    m_CreateInfo.HSV[1] = h2;
    m_CreateInfo.HSV[2] = h3;
}

void Object::SetName(const char *name)
{
	if (name)
	{
		m_Name = g_StringMgr->GetStr(name);
		m_NameLen = strlen(name);
	}
	else
	{
		m_Name = g_StringMgr->GetStr("Unknown");
		m_NameLen = 7;
	}
}

ObjectEffect * Object::AddNewEffect()
{
    if (m_Effects == (0))
    {
        m_Effects = new EffectList; //start effect list
    }

    ObjectEffect * effect = new ObjectEffect;

    memset(effect, 0, sizeof(ObjectEffect));

    m_Effects->push_back(effect);

    return (effect);
}

void Object::SetPosition(float x, float y, float z)
{
    m_Position_info.Position[0] = x;
    m_Position_info.Position[1] = y;
    m_Position_info.Position[2] = z;
}

void Object::SetPosition(float *pos)
{
    m_Position_info.Position[0] = pos[0];
    m_Position_info.Position[1] = pos[1];
    m_Position_info.Position[2] = pos[2];
}

void Object::SetOrientation(float o1, float o2, float o3, float o4)
{
    m_Position_info.Orientation[0] = o1;
    m_Position_info.Orientation[1] = o2;
    m_Position_info.Orientation[2] = o3;
    m_Position_info.Orientation[3] = o4;
}

void Object::SetOrientation(float *ori)
{
    m_Position_info.Orientation[0] = ori[0];
    m_Position_info.Orientation[1] = ori[1];
    m_Position_info.Orientation[2] = ori[2];
    m_Position_info.Orientation[3] = ori[3];
}

void Object::RandomiseOrientation()
{
	m_Position_info.Orientation[0] = (float)(1.0f/(rand()%6+1));
	m_Position_info.Orientation[1] = (float)(1.0f/(rand()%6+1));
	m_Position_info.Orientation[2] = (float)(1.0f/(rand()%6+1));
	m_Position_info.Orientation[3] = (float)(1.0f/(rand()%6+1));
}

void Object::SetVelocityVector(float *velocity)
{
    m_Position_info.Velocity[0] = velocity[0];
    m_Position_info.Velocity[1] = velocity[1];
    m_Position_info.Velocity[2] = velocity[2];
}

void Object::MovePosition(float x, float y, float z)
{
    m_Position_info.Position[0] = m_Position_info.Position[0] + x;
    m_Position_info.Position[1] = m_Position_info.Position[1] + y;
    m_Position_info.Position[2] = m_Position_info.Position[2] + z;
}

void Object::CopyPostionInfo(PositionInformation *pos)
{
    // Advanced Positional Update
	m_Position_info.Bitmask = pos->Bitmask;				// flags for condional fields
	m_Position_info.CurrentSpeed = pos->CurrentSpeed;
	m_Position_info.SetSpeed = pos->SetSpeed;
	m_Position_info.Acceleration = pos->Acceleration;
	m_Position_info.RotY = pos->RotY;
	m_Position_info.DesiredY = pos->DesiredY;
	m_Position_info.RotZ = pos->RotZ;
	m_Position_info.DesiredZ = pos->DesiredZ;
	m_Position_info.ImpartedVelocity[0] = pos->ImpartedVelocity[0];
    m_Position_info.ImpartedVelocity[1] = pos->ImpartedVelocity[1];
    m_Position_info.ImpartedVelocity[2] = pos->ImpartedVelocity[2];

	m_Position_info.ImpartedSpin = pos->ImpartedSpin;
	m_Position_info.ImpartedRoll = pos->ImpartedRoll;
	m_Position_info.ImpartedPitch = pos->ImpartedPitch;
    // Planet Positional Update
    m_Position_info.OrbitID = pos->OrbitID;
    m_Position_info.OrbitDist = pos->OrbitDist;
    m_Position_info.OrbitAngle = pos->OrbitAngle;
    m_Position_info.OrbitRate = pos->OrbitRate;
    m_Position_info.RotateAngle = pos->RotateAngle;
    m_Position_info.RotateRate = pos->RotateRate;
    m_Position_info.TiltAngle = pos->TiltAngle;
    // Component Positional Update
    m_Position_info.ImpartedDecay = pos->ImpartedDecay;
    m_Position_info.TractorSpeed = pos->TractorSpeed;
    m_Position_info.TractorID = pos->TractorID;
    m_Position_info.TractorEffectID = pos->TractorEffectID;
	m_Position_info.UpdatePeriod = pos->UpdatePeriod;
}

void Object::SetTilt(float tilt)
{
	m_Position_info.TiltAngle = tilt;
}

void Object::SetSpin(float rate)
{
	m_Position_info.RotateRate = rate;
}

void Object::SetMovementID(long mov_id)
{
    m_Mutex.Lock();
    m_MovementID = mov_id;
    m_Position_info.MovementID = mov_id;
    m_Mutex.Unlock();
}

//object index methods
void Object::SetIndex(long *index_array)
{
	long *entry = (long*) (index_array + (m_ObjectIndex/(sizeof(long)*8)));

	//now set the specific bit
	*entry |= (1 << m_ObjectIndex%32);
}

void Object::UnSetIndex(long *index_array)
{
	long *entry = (long*) (index_array + (m_ObjectIndex/(sizeof(long)*8)));

	//now unset the specific bit
	*entry &= (0xFFFFFFFF ^ (1 << m_ObjectIndex%32));
}

bool Object::GetIndex(long *index_array)
{
	long *entry = (long*) (index_array + (m_ObjectIndex/(sizeof(long)*8)));

	//now get the specific bit
	if (*entry & (1 << m_ObjectIndex%32))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Object::IsInRange(float *position, float scan_range, bool is_active)
{
	float range;
    scan_range += FieldRadius();
    scan_range += is_active ? 200.0f : 0.0f; //add an extra 200.0f for objects going out of scan range. This stops object 'ratcheting'.

    m_Mutex.Lock();

	//first check if outside max x y z range for quick reject
	if ( (fabsf(position[0] - m_Position_info.Position[0]) > scan_range) ||
	     (fabsf(position[1] - m_Position_info.Position[1]) > scan_range) ||
	     (fabsf(position[2] - m_Position_info.Position[2]) > scan_range) )
    {
        m_Mutex.Unlock();
        return false;
    }

	range = sqrtf( powf((position[0] - m_Position_info.Position[0]), 2) +
		           powf((position[1] - m_Position_info.Position[1]), 2) +
		           powf((position[2] - m_Position_info.Position[2]), 2));

    m_Mutex.Unlock();

	if (range < scan_range)
	{
		return true;
	}
	else
	{
		return false;
	}
}


// Note: This is in an ini file
void  Object::SetBasset(short basset)
{
    m_CreateInfo.BaseAsset = basset;

	//get the radius from SSMetroid's radii parse system
	m_ObjectRadius = g_ServerMgr->BAssetRadii()->GetRadius(basset);

	//NB some of the sizes in the XML file appear to be incorrect -
	//I think we need to parse the 'RadiusAdjust' if it is positive
	//Also we may need to manually adjust some of the radii too
	//consider putting them into the database and allowing people
	//to edit them via the Client itself

	//see if this basset is a legal 3d asset
	AssetData *asset = g_ServerMgr->AssetList()->GetAssetData(basset);
	char *cat_name = (0);

	if (asset) cat_name = asset->m_CatName;

	if ((cat_name && (0 == strcmp(cat_name, "Effects") ||
		0 == strcmp(cat_name, "Icons") ||
		0 == strcmp(cat_name, "Backgrounds") )) ||
		!cat_name)
	{
		//make this stick out like a sore thumb
		m_CreateInfo.BaseAsset = 3000;
		SetSignature(100000.0f);
		SetRadarRange(100000.0f);
	}

#if 0
    switch (m_CreateInfo.BaseAsset)
    {
        //planets
    case 461:
        m_ObjectRadius = 105900.0f;
        break;
    case 466:
        m_ObjectRadius = 35350.0f;
        break;
    case 475:
        m_ObjectRadius = 35300.0f;
        break;
    case 478:
        m_ObjectRadius = 105900.0f;
        break;
    case 1224:
        m_ObjectRadius = 120210.0f;
        break;
    case 1291:
        m_ObjectRadius = 35450.0f;
        break;
    case 1412:
        m_ObjectRadius = 4900.0f;
        break;
    case 1437:
        m_ObjectRadius = 40350.0f;
        break;

        //asteroids/moons
    case 253:
        m_ObjectRadius = 5000.0f;
        break;
    case 254:
        m_ObjectRadius = 8500.0f;
        break;
    case 252:
        m_ObjectRadius = 5230.0f;
        break;
    case 251: //metis
        m_ObjectRadius = 9150.0f;
        break;
    case 406: //RD base in Jupiter
        m_ObjectRadius = 2100.0f;
        break;

        //stargates
    case 48:
    case 63:
    case 142:
    case 1205:
    case 1207:
    case 1314:
    case 1461:
    case 1463:
    case 1882:
        m_ObjectRadius = 1800.0f;  //Do the different types of gate have different radii?
        break;

        //stations
    case 47:
        m_ObjectRadius = 3450.0f;
        break;
    case 123:
        m_ObjectRadius = 5100.0f;
        break;
    case 128:
        m_ObjectRadius = 5100.0f;
        break;
    case 137:
        m_ObjectRadius = 3110.0f;
        break;
    case 166:
        m_ObjectRadius = 2340.0f;
        break;
    case 312:
        m_ObjectRadius = 4465.0f;
        break;
    case 372:
        m_ObjectRadius = 4200.0f;
        break;
    case 373:
        m_ObjectRadius = 2400.0f;
        break;
    case 375:
        m_ObjectRadius = 2725.0f;
        break;
    case 377:
        m_ObjectRadius = 3900.0f;
        break;
    case 379:
        m_ObjectRadius = 3680.0f;
        break;
    case 380:
        m_ObjectRadius = 11500.0f;
        break;
    case 404:
        m_ObjectRadius = 5580.0f;
        break;
    case 405:
        m_ObjectRadius = 4670.0f;
        break;
    case 407:
        m_ObjectRadius = 6260.0f;
        break;
    case 408:
        m_ObjectRadius = 9160.0f;
        break;
    case 413:
        m_ObjectRadius = 6050.0f;
        break;
    case 457:
        m_ObjectRadius = 23750.0f;
        break;
    case 496:
        m_ObjectRadius = 12000.0f;
        break;
    case 1031:
        m_ObjectRadius = 2350.0f;
        break;
    case 1035:
        m_ObjectRadius = 2480.0f;
        break;
    case 1040:
        m_ObjectRadius = 20100.0f;
        break;
    case 1217:
        m_ObjectRadius = 6150.0f;
        break;
    case 1218:
        m_ObjectRadius = 6400.0f;
        break;
    case 1220:
        m_ObjectRadius = 5550.0f;
        break;
    case 1221:
        m_ObjectRadius = 6200.0f;
        break;
    case 1262:
        m_ObjectRadius = 18625.0f;
        break;
    case 1316:
        m_ObjectRadius = 10300.0f;
        break;
    case 1525:
        m_ObjectRadius = 1870.0f;
        break;
    case 1529:
        m_ObjectRadius = 7050.0f;
        break;
    case 1997:
        m_ObjectRadius = 8555.0f;
        break;
    case 2206:
        m_ObjectRadius = 11250.0f;
        break;
    case 2330:
        m_ObjectRadius = 10600.0f;
        break;
    case 1216:
        m_ObjectRadius = 15850.0f; //Joves Fury
        break;

        //MOBs
    case 398:
        m_ObjectRadius = 1000.0f;
        break;
    case 2004: //tengu
    case 2005:
        m_ObjectRadius = 980.0f;
        break;

    default:
        break;

    }
#endif
}

float Object::RangeFrom(float *position, bool abs)
{
    float range;

    //find separation of objects
	range = sqrtf( powf((position[0] - m_Position_info.Position[0]), 2) +
		           powf((position[1] - m_Position_info.Position[1]), 2) +
		           powf((position[2] - m_Position_info.Position[2]), 2));

    if (abs == true)
    {
        return (range);
    }
    else
    {
        range -= m_ObjectRadius;
    }

	if (range < 0.0f)
	{
		range = 0.0f;
	}

	return (range);
}

char Object::IsNav()
{
    if (ObjectType() == OT_NAV)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void Object::BlankPositionInfo()
{
    memset(&m_Position_info, 0, sizeof(PositionInformation));
	m_Velocity = 0;
}

void Object::Remove()
{
    Player *p = 0;

    u32 * sector_list = m_ObjectMgr->GetSectorList();

    if (ObjectType() == OT_PLAYER)
    {
        return;
    }

    m_Mutex.Lock();

	while (g_PlayerMgr->GetNextPlayerOnList(p, sector_list))
	{
		if (p)
		{
			if (GetIndex(p->ObjectRangeList()))
			{
				p->RemoveObject(GameID());
				UnSetIndex(p->ObjectRangeList());
			}
			UnSetIndex(p->ResourceSendList());
		}
	}

    m_Mutex.Unlock();

    //now we want to remove the object from the display list
    //SetRemove();
}

Player * Object::CheckResourceLock()
{
    Player *p = 0;

    u32 * sector_list = m_ObjectMgr->GetSectorList();

	Player * player_lock = (0);

    m_Mutex.Lock();

	if (sector_list)
	{
		while (g_PlayerMgr->GetNextPlayerOnList(p, sector_list))
		{
			if (GetIndex(p->ObjectRangeList()) && p->CurrentResourceTarget() == GameID())
			{
				player_lock = p;
				break;
			}
		}
	}

    m_Mutex.Unlock();

	return player_lock;
}

SectorManager * Object::GetSectorManager()
{
	if (m_ObjectMgr)
	{
		return m_ObjectMgr->GetSectorManager();
	}
	else
	{
		return (0);
	}
}

Object * Object::NearestNav()
{
	if (m_ObjectMgr)
	{
		return (m_ObjectMgr->NearestNav(Position()) );
	}
	else
	{
		return (0);
	}
}

void Object::SendObject(Player *player)
{
    //first send the create info
    SendCreateInfo(player);

    //send Object Effects (if any)
    SendObjectEffects(player);

    //send Relationship
    SendRelationship(player);

    //Send Position
    SendPosition(player);

    //Send Auxdata
	SendAuxDataPacket(player);

    //Send Navigation
    SendNavigation(player);

    OnCreate(player);
}

void Object::SendCreateInfo(Player *player)
{
    player->SendCreate( GameID(),
                            Scale(),
                            BaseAsset(),
                            CreateType(),
                            HSV0(),
                            HSV1(),
                            HSV2());
}

//TODO: write different versions of this for MOBs, Navs & resources - some stations/gates may be 'SHUN' if faction is low or neutral
//Object's version should be virtual & blank
void Object::SendRelationship(Player *player)
{
    long reaction = RELATIONSHIP_FRIENDLY;

    if (HostileTo() == OT_PLAYER)
    {
        reaction = RELATIONSHIP_ATTACK;
    }

    player->SendRelationship(GameID(), reaction, 0);
}

void Object::SetRadarRange(float range)
{
    m_RadarRange = range;
}

void Object::SendSoundBroadcast(long rcount)
{
    Player *p = 0;
    u32 * sector_list = m_ObjectMgr->GetSectorList();

    if (BroadcastID() == 0)
    {
        return;
    }

    ObjectEffect SoundBroadcast;
	SoundBroadcast.Bitmask = 0x04;
    SoundBroadcast.GameID = GameID();
	SoundBroadcast.EffectDescID = (short)BroadcastID();
	SoundBroadcast.EffectID = BroadcastID();
	SoundBroadcast.Duration = 8000;

    while (g_PlayerMgr->GetNextPlayerOnList(p, sector_list))
	{
        if (p->Active() && (p->GameID()%128 == rcount) && RangeFrom(p->Position()) < 40000.0f)
        {
            LogDebug("Broadcasting for object %s\n", Name());
            p->SendObjectEffect(&SoundBroadcast);
        }
	}
}
