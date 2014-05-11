// ResourceClass.cpp

#include "Net7.h"
#include "ObjectClass.h"
#include "FieldClass.h"
#include "ObjectManager.h"
#include "MOBclass.h"
#include <float.h>

Field::Field(long object_id) : Object (object_id)
{
    m_Type = OT_FIELD;
    m_FieldRadius = 0.0f;
    m_FieldCount = 0;
    m_FieldType = 0;
    m_FirstFieldID = 0;
}

Field::~Field()
{
    // TODO: destroy everything
}

void Field::SetHSV(float h1, float h2, float h3)
{
    m_Level = (short)h3;
    m_FieldType = (short)h2;
    m_FieldCount = (short)h1;
}

void Field::PopulateField(bool live, bool repopulate)
{
    m_Signature = m_FieldRadius;
    if (m_FieldRadius == 0) return; //don't populate yet
    if (m_Level == 0 || m_FieldCount == 0)
    {
        LogMessage("Unable to create asteroid field '%s'. level:%d,Type:%d,count:%d\n",Name(), m_Level, m_FieldType, m_FieldCount);
        m_FieldCount = 0;
        return;
    }

	if (m_Level > 8) m_Level = 8;

    if (m_FieldType == 0) //random
    {
        m_FieldType = GetTickCount()%5 + 1;
    }

    //create and populate asteroid field
	float pos[3];
	float inc;
	unsigned int count;
	float angle = 0.0f;
	inc = (2*3.14159f)/m_FieldCount;
	pos[2] = m_Position_info.Position[2];

	u16 types_size;
	//char types[16];
	u16 type_index;
	int this_level;
	int level_seed;
	u16 field_spread;
	float factor1, factor2, factor3 = 0.0f;
    Object *obj;

	long current_id = GameID() + 1;

    PopulateTypes(field_spread);

	if (field_spread == 4 || field_spread == 5) inc = inc * 3;

    types_size = m_ResourceIDs.size();

	for (count = 0; count < m_FieldCount; count++)
	{
		factor2 = (float)(rand()%2000 - 1000);
		switch (field_spread)
		{
		case 1: //ring shape
			pos[0] = m_Position_info.Position[0] + m_FieldRadius * cosf(inc * count);
			pos[1] = m_Position_info.Position[1] + m_FieldRadius * sinf(inc * count);
			pos[2] = m_Position_info.Position[2] + (rand()%1000 - 500);
			break;
		case 3: //cylinder shape
			factor2 = rand()%(int)(m_FieldRadius) - (m_FieldRadius/2);
			//drop through
		case 2: //donut shape
			factor1 = 1.0f - (float)(rand()%20)/100.0f;
			pos[0] = m_Position_info.Position[0] + (m_FieldRadius * factor1) * cosf(inc * count);
			pos[1] = m_Position_info.Position[1] + (m_FieldRadius * factor1) * sinf(inc * count);
			pos[2] = m_Position_info.Position[2] + factor2;
			break;
		case 4:	//regular sphere
			factor1 = (rand()%360)/(180.0f/PI); //random Z plane
			factor2 = (rand()%360)/(180.0f/PI); //random Y plane
			factor3 = (rand()%1000) * (m_FieldRadius/1000.0f);
			pos[0] = m_Position_info.Position[0] + (factor3*cosf(factor2)) * cosf(factor1);
			pos[1] = m_Position_info.Position[1] + (factor3*cosf(factor2)) * sinf(factor1);
			pos[2] = m_Position_info.Position[2] + (factor3*sinf(factor2));
			break;
        case 5:	//centre weighted sphere
			factor1 = (rand()%360)/(180.0f/PI); //random Z plane
			factor2 = (rand()%360)/(180.0f/PI); //random Y plane
			factor3 = (rand()%1000) * (m_FieldRadius/1000.0f);

			//produce centre weight (simple, dirty, but it looks right!)
			if (rand()%10 > 5) factor3 = factor3 * 0.7f;
			if (rand()%10 > 5) factor3 = factor3 * 0.7f;
			pos[0] = m_Position_info.Position[0] + (factor3*cosf(factor2)) * cosf(factor1);
			pos[1] = m_Position_info.Position[1] + (factor3*cosf(factor2)) * sinf(factor1);
			pos[2] = m_Position_info.Position[2] + (factor3*sinf(factor2));
			break;

		default:
			factor1 = 1.0f - (float)(rand()%20)/100.0f;
			pos[0] = m_Position_info.Position[0] + (m_FieldRadius * factor1) * cosf(inc * count);
			pos[1] = m_Position_info.Position[1] + (m_FieldRadius * factor1) * sinf(inc * count);
			pos[2] = m_Position_info.Position[2] + factor2;
			break;
		}

		//now move the roid WRT field orientation.
		if (field_spread == 1 || field_spread == 2 || field_spread == 3)
		{
			TransformCoords(pos, this->Position(), this->Orientation());
		}

		type_index = rand()%types_size;
		level_seed = rand()%20;
		this_level = m_Level;
		if (level_seed > 17)
		{
			this_level++;
		}
		else if (level_seed < 4)
		{
			this_level--;
		}

		if (repopulate)
		{
			obj = m_ObjectMgr->GetObjectFromID(current_id);
			current_id++;
		}
		else
		{
			obj = m_ObjectMgr->AddNewObject(OT_RESOURCE, true);
		}
        obj->SetTypeAndName((short)m_ResourceIDs[type_index]);
        obj->SetPosition(pos);
        obj->SetLevel(this_level);
        obj->RandomiseOrientation();
        obj->SetContainerField(this);

        if (m_FirstFieldID == 0)
        {
            m_FirstFieldID = obj->GameID();
        }

        if (live || repopulate)
        {
			obj->SendObjectReset();
            obj->ResetResource();
        }
    }

    //Add guardians
    AddFieldGuardian(repopulate);
    
}

//TODO: remove this function completely when we switch over to new field descriptors
void Field::PopulateTypes(u16 &field_spread)
{   
    //New DASE2 driven field data
    if (m_ResourceIDs.size() > 0)
    {
        field_spread = m_FieldType;
    }
    else
    {
        //old hardcoded style
        switch (m_FieldType)
        {
        case 1:
            m_ResourceIDs.push_back(6 + 0x71E);
            m_ResourceIDs.push_back(7 + 0x71E);
            m_ResourceIDs.push_back(8 + 0x71E);
            m_ResourceIDs.push_back(10 + 0x71E);
            field_spread = 1;
            break;
            
        case 2:
            m_ResourceIDs.push_back(2 + 0x71E);
            m_ResourceIDs.push_back(4 + 0x71E);
            m_ResourceIDs.push_back(5 + 0x71E);
            m_ResourceIDs.push_back(11 + 0x71E);
            m_ResourceIDs.push_back(12 + 0x71E);
            field_spread = 3;
            break;
            
        case 3:
            m_ResourceIDs.push_back(3 + 0x71E);
            m_ResourceIDs.push_back(6 + 0x71E);
            m_ResourceIDs.push_back(7 + 0x71E);
            m_ResourceIDs.push_back(9 + 0x71E);
            m_ResourceIDs.push_back(12 + 0x71E);
            m_ResourceIDs.push_back(11 + 0x71E);
            field_spread = 2;
            break;
            
        case 4:
            m_ResourceIDs.push_back(1 + 0x71E);
            m_ResourceIDs.push_back(3 + 0x71E);
            m_ResourceIDs.push_back(4 + 0x71E);
            m_ResourceIDs.push_back(6 + 0x71E);
            m_ResourceIDs.push_back(12 + 0x71E);
            m_ResourceIDs.push_back(10 + 0x71E);
            field_spread = 4;
            break;
            
        case 5: //gas cloud clump
            m_ResourceIDs.push_back(12 + 0x71E);
            field_spread = 4;
            break;
            
        default:
            m_ResourceIDs.push_back(1 + 0x71E);
            m_ResourceIDs.push_back(0 + 0x71E);
            m_ResourceIDs.push_back(6 + 0x71E);
            m_ResourceIDs.push_back(9 + 0x71E);
            field_spread = 1;
            break;
        }
    }
}

void Field::AddMOBID(long mob_id)
{
    //LogMessage("Adding mob %d to field %s.\n", mob_id, Name());
    m_MOBIDs.push_back(mob_id);
}

void Field::AddResource(long resource)
{
    m_ResourceIDs.push_back(resource);
}

void Field::AddItemID(long item_id, float frequency)
{
	//see if item already in list, or if we can re-use a blank entry
	for (ItemIDList::iterator itrItem = m_AdditionalOreItemIDs.begin(); itrItem != m_AdditionalOreItemIDs.end(); ++itrItem)
	{
		if ( (*itrItem)->item_id == item_id || (*itrItem)->item_id == 0 )
		{
			(*itrItem)->item_id = item_id;
			(*itrItem)->frequency = frequency;
			return;
		}
	}

	OreNode *node = new OreNode;
	node->item_id = item_id;
	node->frequency = frequency;
	m_AdditionalOreItemIDs.push_back(node);
}

void Field::BlankItemIDs()
{
	for (ItemIDList::iterator itrItem = m_AdditionalOreItemIDs.begin(); itrItem != m_AdditionalOreItemIDs.end(); ++itrItem)
	{	
		(*itrItem)->frequency = 0;
		(*itrItem)->item_id = 0;
	}
}

ItemIDList* Field::GetAdditionalItemIDs()
{
	return (&m_AdditionalOreItemIDs);
}

void Field::ResetResource()
{
	m_ResourceIDs.clear();
	m_MOBIDs.clear();
}

void Field::AddFieldGuardian(bool repopulate)
{
    u32 mob_count = m_MOBIDs.size();

    if (m_FieldRadius < 2500.0f)
    {
        //LogMessage("Field %d(%s) too small for MOBs\n", m_DatabaseUID, Name());
		m_FieldRadius = 2500.0f;
    }

	unsigned long index = 0;

    for (u32 x = 0; x < mob_count; x++)
    {
		MOB *mob = (0);
		if (repopulate)
		{
			//use MOBs from this field
			//see if we can find an existing mob for this
			if (index < m_MOBGameIDs.size())
			{
				mob = (MOB*) m_ObjectMgr->GetObjectFromID(m_MOBGameIDs[index]);
				index++;
			}

			if (!mob)
			{
				//didn't find a suitable MOB
				mob = (MOB*)m_ObjectMgr->AddNewObject(OT_MOB); //MOB creation
				mob->AddBehaviourObject(this);
				m_MOBGameIDs.push_back(mob->GameID());
			}
		}
		else
		{
			mob = (MOB*)m_ObjectMgr->AddNewObject(OT_MOB); //MOB creation
			mob->AddBehaviourObject(this);
			m_MOBGameIDs.push_back(mob->GameID());
		}
        
        if (mob)
        {
            mob->SetMOBType((short)m_MOBIDs[x]);
            mob->SetPosition(Position());
            mob->MovePosition(-200.0f, 0.0f, -200.0f);
            mob->SetActive(true);
            mob->SetRespawnTick(0);
            mob->SetHostileTo(OT_PLAYER);
            mob->SetOrientation(0.0f, 0.0f, 0.0f, 1.0f);
            mob->SetUpdateRate(50);
            float velocity = (float)(rand()%100 + 130);
            float turn = (float)(rand()%3 + 2)*0.02f;
            if (rand()%10 > 7) turn = -(float)(rand()%3 + 2)*0.02f;
            if (turn == 0.0f) turn = 0.02f;
            mob->SetVelocity(velocity);
            mob->SetBehaviour(PATROL_FIELD);
            mob->SetDefaultStats(turn, PATROL_FIELD, velocity, 50);
			mob->SetSpawnGroup(this);
        }
    }
}

Object * Field::SetDestination(Object *current)
{
    //choose a destination somewhere a reasonable distance from the current asteroid.
    Object *obj;
    long id;
    short spin_count = 0;
    if (!current)
    {
        current = m_ObjectMgr->GetObjectFromID(m_FirstFieldID);
    }

    id = current->GameID();

    Object *target_obj = (0);

    while (!target_obj && spin_count < 3)
    {
		id += ((rand()%3) + 1);
        if (id >= (m_FirstFieldID + m_FieldCount - 1)) 
        {
            id = m_FirstFieldID;
            spin_count++;
        }

        obj = m_ObjectMgr->GetObjectFromID(id);

        if (obj->RangeFrom(current->Position()) > 2000.0f) //this assumes a minimum field size ...
        {
            target_obj = obj;
        }
    }

    if (!target_obj)
    {
        id = m_FirstFieldID + rand()%5;
        target_obj = m_ObjectMgr->GetObjectFromID(id);
    }

	if (!target_obj)
	{
		target_obj = m_ObjectMgr->GetObjectFromID(m_FirstFieldID);
	}

	if (!target_obj)
	{
		LogMessage("Critical error. Unable to find any asteroids in field ID %d '%s'\n", GetDatabaseUID(), Name());
	}

    return (target_obj);
}

void Field::SendAuxDataPacket(Player *player)
{
	player->SendAuxNameSignature(this);
}

void Field::SendPosition(Player *player)
{
	player->SendConstantPositionalUpdate(
		GameID(),
		PosX(),
		PosY(),
		PosZ(),
		Orientation());
}

void Field::SendObjectEffects(Player *player)
{
	ObjectEffect ObjEffect;
	ObjEffect.Bitmask = 3;
	ObjEffect.GameID = GameID();
	ObjEffect.EffectDescID = 407; 
	ObjEffect.EffectID = GetSectorManager()->GetSectorNextObjID();
	ObjEffect.TimeStamp = GetNet7TickCount();

	player->SendEffect(&ObjEffect);
}

void Field::SendToVisibilityList(bool include_player)
{
	Player * p = (0);
    u32 * sector_list = m_ObjectMgr->GetSectorList();
	if (sector_list)
	{
		while (g_PlayerMgr->GetNextPlayerOnList(p, sector_list))
		{
			if (p) 
			{
				p->SendAdvancedPositionalUpdate(GameID(), PosInfo());
			}
		}
	}
}