// HuskClass.cpp

#include "Net7.h"
#include "MemoryHandler.h"
#include "ObjectClass.h"
#include "HuskClass.h"
#include "ServerManager.h"
#include "ObjectManager.h"
#include <float.h>

Husk::Husk(long object_id) : Object (object_id)
{
    m_Type = OT_HUSK;
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
	m_HuskCredits = 0;
}

Husk::Husk() : Object (0)
{
    m_Type = OT_HUSK;
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
	m_HuskCredits = 0;
}

Husk::~Husk()
{
    // TODO: destroy everything
}

void Husk::SetLevel(short level)
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

void Husk::SetBasset(short basset)
{
    m_CreateInfo.BaseAsset = basset;
	AssetData *asset = g_ServerMgr->AssetList()->GetAssetData(basset);

	m_Type = OT_HUSK;
	if (asset->m_Name[0] == 'O')
	{
		m_Resource_type = ORGANIC_HULK;
	}
	else
	{
		m_Resource_type = INORGANIC_HULK;
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

//this should go into the object base class really, since it's used for resource class too
void Husk::AddItem(ItemBase *item, long stack)
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

	//see if Husk already has this
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
}

ItemBase * Husk::GetItem(long slot)
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

short Husk::GetStack(long slot)
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

float Husk::RemoveItem(long slot_id, long stack)
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

void Husk::PopulateHusk(long mob_type)
{
	MOBData *mob_data = g_ServerMgr->MOBList()->GetMOBData(mob_type);
	long loot_item_id;
	short stack;
	float chance_to_drop, drop_number;
	long highest_count = mob_data->m_Loot.size();

	//pick a number of slots
	long random = rand()%25 + rand()%25 + rand()%25 + rand()%25 + 4;
	long slots = 1;

	if (random > 55) slots++;
	if (random > 75) slots++;
	if (random > 90) slots++;
	if (random > 96) slots++;
	if (random > 99) slots++;

	if (highest_count != 0)
	{
		for (int x = 0; x < slots; x++)
		{
			int rand_item = (rand()%highest_count);

			loot_item_id = mob_data->m_Loot[rand_item]->item_base_id;
			if (loot_item_id == -1) continue;

			//see if this item drops or not
			chance_to_drop = mob_data->m_Loot[rand_item]->drop_chance;

			drop_number = (float)(rand()%250 + rand()%250 + rand()%250 + rand()%250)/1000.0f;

			if (chance_to_drop >= drop_number)
			{
				stack = rand()%(mob_data->m_Loot[rand_item]->quantity) + 1;
				AddItem(g_ItemBaseMgr->GetItem(loot_item_id), stack);
			}
		}
	}
	else
	{
		LogMessage("Husk %s has no loot choice\n", Name());
	}

	if (m_Resource_start_value > 0) 
	{
		m_Resource_remains = 1.0f;
	}
	else
	{
		m_Resource_remains = 0.0f;
	}

    SetRespawnTick(0);

    m_ToBeRemoved = false;
}

void Husk::SendObjectReset()
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

void Husk::SendObjectDrain(long slot)
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
				p->SetHuskDrainLevel(this, slot);
			}
		}
	}

    if (m_Resource_remains == 0)
    {
		RemoveDestroyTimer();
		SetRespawnTick(-1);
		Remove();
    }
}

//Send object to all players who can see this object. Currently used only for resource hijacks
void Husk::SendToVisibilityList(bool include_player)
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

void Husk::SetHuskName(char *name)
{
	char buffer[128];
	m_NameLen = _snprintf(buffer, 128, "Corpse of %s", name);
	m_Name = g_StringMgr->GetStr(buffer);
}

void Husk::SendPosition(Player *player)
{
    player->SendConstantPositionalUpdate(
        GameID(),
        PosX(),
        PosY(),
        PosZ(),
        Orientation());
}

void Husk::SendAuxDataPacket(Player *player)
{
	player->SendHuskName(this);
}

//On creation of resource graphic object for Player
void Husk::OnCreate(Player *player)
{
    //if 'TargetOnCreate' is set for this object, we want a player to auto-target it 
    //when it comes into their view. (eg Husks).
    /*if (player->GameID() == GetPlayerLootLock()) 
    {
        player->SendHuskContent(this);
        player->SendPacketCache();
        player->ShipIndex()->SetTargetGameID(GameID());
        player->SendAuxShip();
        player->BlankVerbs();
        player->AddVerb(VERBID_LOOT, 2000.0f);
        SetPlayerLootLock(0);
    }*/
}

//Called every time this resource is targeted.
void Husk::OnTargeted(Player *player)
{
    player->BlankVerbs();
	player->SendHuskContent(this);
	player->AddVerb(VERBID_LOOT, 6000.0f);
}

void Husk::SetDestroyTimer(long time_delay, long respawn_delay)
{
	if (time_delay == 0)
	{
		//destroy now
		Remove();
	}
	else
	{
		GetSectorManager()->AddTimedCallPNode(&m_ObjectTimeSlot, B_DESTROY_HUSK, time_delay, this, respawn_delay);
	}
}

void Husk::RemoveDestroyTimer()
{
	if (m_ObjectTimeSlot.event_time != 0) 
	{
		GetSectorManager()->RemoveTimedCall(&m_ObjectTimeSlot, true);
	}
}

void Husk::DestroyHusk()
{
    Player *p = 0;
    u32 * sector_list = m_ObjectMgr->GetSectorList();

	SetRespawnTick(-1);

	if (!Active()) return;

	//have a little removal effect
	ObjectEffect ObjExplosion;

	ObjExplosion.Bitmask = 0x07;
	ObjExplosion.GameID = GameID();
	ObjExplosion.EffectDescID = 393; //orange shockwave
	ObjExplosion.EffectID = m_ObjectMgr->GetAvailableSectorID();
	ObjExplosion.TimeStamp = GetNet7TickCount();
	ObjExplosion.Duration = 4000;

	while (g_PlayerMgr->GetNextPlayerOnList(p, sector_list))
	{
		if (p)
		{
			if (GetIndex(p->ObjectRangeList()))
			{
				p->SendObjectEffect(&ObjExplosion);
			}
		}
	}

	Remove();
}

void Husk::SetLootTimer(long time_delay)
{
	if (time_delay == 0)
	{
		m_LootTime = 0;
	}
	else
	{
		m_LootTime = GetNet7TickCount() + time_delay;
	}
}