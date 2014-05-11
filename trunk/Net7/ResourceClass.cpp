// ResourceClass.cpp

#include "Net7.h"
#include "MemoryHandler.h"
#include "ObjectClass.h"
#include "ResourceClass.h"
#include "ServerManager.h"
#include "ObjectManager.h"
#include <float.h>

Resource::Resource(long object_id) : Object (object_id)
{
    m_Type = OT_RESOURCE;
    m_Position_info.type = POSITION_CONSTANT;
    m_Content_count = 0;
    m_Resource_value = 0;
    m_Respawn_tick = 0;
    m_Resource_type = 0;
    m_CreateInfo.Type = 38;
    m_Resource_remains = 0.0f;
    m_Resource_start_value = 0;
	m_Field_Container = 0;
    memset(m_Resource_contents, 0, MAX_ITEMS_PER_RESOURCE*sizeof(ContentSlot));
	memset(&m_ObjectTimeSlot, 0, sizeof(m_ObjectTimeSlot));
}

Resource::Resource() : Object (0)
{
    m_Type = OT_RESOURCE;
    m_Position_info.type = POSITION_CONSTANT;
    m_Content_count = 0;
    m_Resource_value = 0;
    m_Respawn_tick = 0;
    m_Resource_type = 0;
    m_CreateInfo.Type = 38;
    m_Resource_remains = 0.0f;
    m_Resource_start_value = 0;
    memset(m_Resource_contents, 0, MAX_ITEMS_PER_RESOURCE*sizeof(ContentSlot));
	memset(&m_ObjectTimeSlot, 0, sizeof(m_ObjectTimeSlot));
}

Resource::~Resource()
{
    // TODO: destroy everything
}

void Resource::SetLevel(short level)
{
    float obj_colour;

	switch (level)
	{
    case 1:
		obj_colour = 180.0f;
		break;
	case 2:
		obj_colour = 120.0f;
		break;
	case 3:
		obj_colour = 30.0f;
		break;
	case 4:
		obj_colour = 20.0f;
		break;
	case 5:
		obj_colour = 10.0f;
		break;
	case 6:
		obj_colour = 0.0f;
		break;
	case 7:
		obj_colour = -60.0f;
		break;
	case 8:
		obj_colour = -70.0f;
		break;
	case 9:
		obj_colour = -100.0f;
		break;
	case 10:
		obj_colour = -110.0f;
		break;
	case 11:
		obj_colour = -120.0f;
		break;

	default:
		obj_colour = 180.0f;
        level = 1;
		break;
	}
    
    m_Level = level;
    m_CreateInfo.HSV[0] = obj_colour; //NB: this can still be overriden easily with a SetColour
}

static char *resource_name[] = {
    "Asteroid",             //0x71E
	"Hydrocarbon Deposit",	//0x726
	"Crystalline Asteroid", //0x727
	"Gas Cloud",			//0x72a
	"Tazeron Asteroid",
	"Mordanite Asteroid",
	"Hulk",
	0
};	

void Resource::SetTypeAndName(short basset)
{
	short name_type = 0;

	//check to see if this is an 'asteroid' type
	AssetData *asset = g_ServerMgr->AssetList()->GetAssetData(basset);
	if (strcmp(asset->m_CatName, "Asteroids") == 0)
	{
		//select name and main type based on sub type
		switch (asset->m_SubCatName[0])
		{
		case 'R':
			name_type = 0;
			m_Resource_type = REACTIVE_ASTEROID;
			break;

		case 'M':
			name_type = 0;
			m_Resource_type = ROCKY_ASTEROID;
			break;

		case 'H':
			name_type = 1;
			m_Resource_type = HYDROCARBON;
			break;

		case 'G':
			if (asset->m_SubCatName[1] == 'a')
			{
				name_type = 3;
				m_Resource_type = GAS_CLOUD;
			}
			else
			{
				name_type = 2;
				m_Resource_type = CRYSTALLINE_ASTEROID;
			}
			break;

		case 'C':
			name_type = 1;
			m_Resource_type = CARBONACEOUS_ASTEROID;
			break;

		case 'N':
			name_type = 0;
			m_Resource_type = NICKEL_IRON;
			break;

		case 'D':
			name_type = 1;
			m_Resource_type = DIRTY_ICE_ASTEROID;
			break;

		default:
			name_type = 3;
			m_Resource_type = GAS_CLOUD;
			break;
		}
	}
	else
	{
		switch (asset->m_SubCatName[0])
		{
		case 'I':
			name_type = 6;
			m_Resource_type = INORGANIC_HULK;
			SetScale(3.0f);
			break;

		case 'O':
			name_type = 6;
			m_Resource_type = ORGANIC_HULK;
			SetScale(3.0f);
			break;

		default:
			name_type = 6;
			m_Resource_type = INORGANIC_HULK;
			break;
		}
	}

	SetBasset(basset);
    SetName(resource_name[name_type]);
    //LogMessage("0x%04x:%s\n", m_CreateInfo.BaseAsset, Name());
}

void Resource::SetBasset(short basset)
{
    m_CreateInfo.BaseAsset = basset;
	AssetData *asset = g_ServerMgr->AssetList()->GetAssetData(basset);

	switch (asset->m_CatName[0])
	{
	case 'H':
		//hulk
		m_Type = OT_HULK;
		if (asset->m_SubCatName[0] == 'I')
		{
			m_Resource_type = INORGANIC_HULK;
		}
		else
		{
			m_Resource_type = ORGANIC_HULK;
		}
		break;

	default:
		break;
	}

    switch (m_CreateInfo.BaseAsset)
    {
    case 1822:
    case 1823:
    case 1824:
        m_ObjectRadius = 800.0f;
        break;

    case 1825:
    case 1826:
    case 1827:
        m_ObjectRadius = 630.0f;
        break;

    case 1828:
    case 1829:
    case 1830:
        m_ObjectRadius = 510.0f;
        break;

    case 1831:
    case 1832:
    case 1833:
        m_ObjectRadius = 750.0f;
        break;

	case 1834:
		m_ObjectRadius = 0;
		break;

    default:
		m_ObjectRadius = g_ServerMgr->BAssetRadii()->GetRadius(basset);
        break;
    };
}

void Resource::AddItem(ItemBase *item, long stack)
{
    int i;
	int j;

    if (item == (0) || stack == 0)
    {
        LogMessage("NULL item passed to AddItem\n");
        return;
    }

    //locate first empty slot
    for (i=0; i < MAX_ITEMS_PER_RESOURCE; i++)
    {
        if (m_Resource_contents[i].stack == 0)
        {
            break;
        }
    }

	//see if resource already has this item
    for (j=0; j < MAX_ITEMS_PER_RESOURCE; j++)
    {
		if (m_Resource_contents[j].item == item)
        {
			if ((stack + m_Resource_contents[j].stack) <= item->MaxStack())
			{
				m_Resource_contents[j].stack += (u16)stack;
				m_Resource_start_value += (u16)stack;
			}
			else
			{
				m_Resource_start_value += (item->MaxStack() - m_Resource_contents[j].stack);
				m_Resource_contents[j].stack = item->MaxStack();
			}
			m_Resource_value = m_Resource_start_value;
            return;
        }
    }
    
    if (i < MAX_ITEMS_PER_RESOURCE)
    {
        m_Resource_contents[i].item = item;
        m_Resource_contents[i].stack = (u16)stack;
        m_Content_count++;
        m_Resource_start_value += (u16)stack;
    }

    m_Resource_value = m_Resource_start_value;

    SetRequiresReset(true);
}

ItemBase * Resource::GetItem(long slot)
{
    if (slot >= 0 && slot < MAX_ITEMS_PER_RESOURCE)
    {
        return (m_Resource_contents[slot].item);
    }
    else
    {
        return (0);
    }
}

short Resource::GetStack(long slot)
{
    if (slot >= 0 && slot < MAX_ITEMS_PER_RESOURCE)
    {
        return (m_Resource_contents[slot].stack);
    }
    else
    {
        return (0);
    }
}

float Resource::RemoveItem(long slot_id, long stack)
{
    float resource_remains = 0.0f;
    float resource_remaining;

    if (slot_id < MAX_ITEMS_PER_RESOURCE)
    {
        if (m_Resource_contents[slot_id].stack > (u16)stack)
        {
            m_Resource_contents[slot_id].stack -= (u16)stack;
        }
        else
        {
            m_Resource_contents[slot_id].stack = 0;
            m_Resource_contents[slot_id].item = (0);
        }
        
        m_Resource_value -= (u16)stack;

        resource_remaining = ( (float)(m_Resource_value) / (float)(m_Resource_start_value) );

        resource_remains = 0.0f;

        //roundings needed by client
		if (resource_remaining > 0.0f)
		{
			resource_remains = 0.125f;
		}
		if (resource_remaining > 0.125f)
		{
			resource_remains = 0.25f;
		}
		if (resource_remaining > 0.25f)
		{
			resource_remains = 0.4f;
		}
		if (resource_remaining > 0.4f)
		{
			resource_remains = 0.5f;
		}
		if (resource_remaining > 0.5f)
		{
			resource_remains = 0.6f;
		}
		if (resource_remaining > 0.6f)
		{
			resource_remains = 0.88889f;
		}
		if (resource_remaining > 0.95f)
		{
			resource_remains = 1.0f;
		}
    }
    else
    {
        LogMessage("ERROR: slot out of range for object '%s' id [%d]\n", Name(), GameID());
    }

    m_Resource_remains = resource_remains;

    SendObjectDrain(slot_id);

    return (resource_remains);
}

void Resource::SetLevelFromHSV(float h1)
{
	int colour = (int) h1;

	switch (colour)
	{
    case 180:
        m_Level = 1;
        break;
	case 120:
		m_Level = 2;
		break;
	case 30:
		m_Level = 3;
		break;
	case 20:
		m_Level = 4;
		break;
	case 10:
		m_Level = 5;
		break;
	case 0:
		m_Level = 6;
		break;
	case -60:
		m_Level = 7;
		break;
	case -70:
		m_Level = 8;
		break;
	case -100:
		m_Level = 9;
		break;
	case -110:
		m_Level = 10;
		break;

	default:
		m_Level = 1;
		break;
	}
}

void Resource::ResetResource()
{
	int x;

    memset(m_Resource_contents, 0, MAX_ITEMS_PER_RESOURCE*sizeof(ContentSlot));

	m_Content_count = 0;

	if (Level() == 0) //haven't yet assigned a level to this object
	{
		LogMessage("WARNING: no level assigned to object '%s' [%d]\n", Name(), GameID());
        SetLevel(1); //default to level 1
	}

    m_Resource_start_value = 0;

	if (BaseAsset() == 0x726 && Level() == 10)
	{
        AddItem(g_ItemBaseMgr->GetItem(3009), 1); //grail water (quest)
    }
    else
    {
		//pick a number of slots
		long random = rand()%25 + rand()%25 + rand()%25 + rand()%25 + 4;
		long slots = 1;

		if (random > 45) slots++;
		if (random > 70) slots++;
		if (random > 85) slots++;
		if (random > 95) slots++;
		if (random > 99) slots++;

        for (x = 0; x < slots; x++)
		{
			short level = Level() - rand()%2;
			short stack = rand()%5 + 1;
			ItemBase *item_to_add;

			if (level < 1) level = 1;

			SectorManager * SMgr = GetSectorManager();
			if (SMgr->GetSectorID() == 1052 && m_Resource_type == INORGANIC_HULK)
			{
				Sleep(1);
			}
			if (SMgr)
			{
				item_to_add = g_ItemBaseMgr->GetOreTemplate(level, m_Resource_type, SMgr->GetSectorID(), (Field*)m_Field_Container);
			}
			else
			{
				item_to_add = 0;
			}

			if (item_to_add)
			{
				if (item_to_add->MaxStack() == 1)
				{
					stack = 1;
				}
				else if (m_Resource_type == GAS_CLOUD && Level() < 7) //gas cloud
				{
					if (rand()%30 == 29)
					{
						stack = rand()%80 + 1;
					}
					else if (rand()%20 > 18)
					{
						stack = rand()%40 + 1;
					}
					else
					{
						stack = rand()%6 + 1;
					}
				}
				AddItem(item_to_add, stack);
            }
			else
			{
				if (GetSectorManager())
				{
					long id = GetDatabaseUID();
					if (m_Field_Container)
					{
						id = m_Field_Container->GetDatabaseUID();
					}
					LogMessage("--> ERROR: No ore found for Asteroid '%s', ID:%d level %d sector %d\n", Name(), id, Level(), GetSectorManager()->GetSectorID());
				}
				break;
			}
		}
    }

    m_Resource_remains = 1.0f;

    SetRespawnTick(0);

    SetRequiresReset(true);

    m_ToBeRemoved = false;
}

void Resource::SendObjectReset()
{
	Player * p = 0;
    u32 * sector_list = m_ObjectMgr->GetSectorList();
	
    if (sector_list)
    {
		while (g_PlayerMgr->GetNextPlayerOnList(p, sector_list))
		{
			UnSetIndex(p->ResourceSendList());
		}
    }
}

void Resource::SendObjectDrain(long slot)
{
    PlayerList::iterator itrPList;
	Player * p = 0;
    u32 * sector_list = m_ObjectMgr->GetSectorList();
	
    if (slot != -1 && sector_list)
    {		
		while (g_PlayerMgr->GetNextPlayerOnList(p, sector_list))
		{			
			if (GetIndex(p->ObjectRangeList()))
			{
				p->SetResourceDrainLevel(this, slot);
			}
		}
	}

    if (m_Resource_remains == 0)
    {
        DestroyResource();
    }
}

void Resource::DestroyResource()
{
   	long destroy_timer = 2500;
	long respawn_timer = 60000*15; //15mins //TODO: make this selectable from DASE data

	if (BaseAsset() == 0x726 && Level() >= 9)
	{
        respawn_timer = 10000;
    }
   
    if (ObjectType() == OT_FLOATING_ORE)
    {
        m_ObjectMgr->DestroyObject(this, 0, -1);
    }
    else 
	{
		switch (m_Resource_type)
		{
		case GAS_CLOUD:
			m_ObjectMgr->DestroyObject(this, 0, respawn_timer);
			break;

		case INORGANIC_HULK:
		case   ORGANIC_HULK:
			m_ObjectMgr->DestroyObject(this, 0, -1); 
			break;

		case HYDROCARBON:
			destroy_timer += 1500; //hydro's take longer to disintegrate
		case REACTIVE_ASTEROID:
		case ROCKY_ASTEROID:
		case CRYSTALLINE_ASTEROID:
		case CARBONACEOUS_ASTEROID:
		case DIRTY_ICE_ASTEROID:
		case NICKEL_IRON:
			m_ObjectMgr->DestroyObject(this, destroy_timer, respawn_timer); 
			break;
		}
	}

    m_TargetOnCreate = 0;
}

//Send object to all players who can see this object. Currently used only for resource hijacks
void Resource::SendToVisibilityList(bool include_player)
{
	Player * p = 0;
    u32 * sector_list = m_ObjectMgr->GetSectorList();

    if (sector_list)
    {
		while (g_PlayerMgr->GetNextPlayerOnList(p, sector_list))
		{			
			if (GetIndex(p->ObjectRangeList()))
			{
				p->SendAdvancedPositionalUpdate(GameID(), PosInfo());
			}
		}
	}
}

void Resource::SendPosition(Player *player)
{
    player->SendConstantPositionalUpdate(
        GameID(),
        PosX(),
        PosY(),
        PosZ(),
        Orientation());
}

void Resource::SendAuxDataPacket(Player *player)
{
    switch (CreateType())
    {
		case 38:
            player->SendAuxNameResource(this);
			break;
		case 4:
            player->SendResourceName(GameID(), Name());
			break;
		default:
			break;
    }
}

//On creation of resource graphic object for Player
void Resource::OnCreate(Player *player)
{
 
}

//Called every time this resource is targeted.
void Resource::OnTargeted(Player *player)
{
    player->BlankVerbs();

    switch (ObjectType())
    {
    case OT_RESOURCE:
    case OT_HULK:
		if (player->Profession() == PROFESSION_EXPLORER)
        {
            player->AddVerb(VERBID_PROSPECT, float(player->ProspectRange()));
            player->SendResourceLevel(GameID());
            player->SendResourceContentsAUX(this);
        } 
        break;

    case OT_FLOATING_ORE:
        player->AddVerb(VERBID_TRACTOR, 2000.0f);
		break;
    };
}

void Resource::SetDestroyTimer(long time_delay, long respawn_delay)
{
	if (time_delay == 0)
	{
		//destroy now
		Remove();
	}
	else
	{
		GetSectorManager()->AddTimedCallPNode(&m_ObjectTimeSlot, B_DESTROY_RESOURCE, time_delay, this, respawn_delay);
	}
}

void Resource::RemoveDestroyTimer()
{
	if (m_ObjectTimeSlot.event_time != 0) 
	{
		GetSectorManager()->RemoveTimedCall(&m_ObjectTimeSlot, true);
	}
}