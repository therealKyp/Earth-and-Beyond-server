// PlayerMisc.cpp

// These are routines that don't really have a proper home at the moment, placed here to avoid cluttering up PlayerClass & PlayerConnection

#pragma warning(disable:4786)

#include "PlayerClass.h"
#include "ServerManager.h"
#include "ObjectClass.h"
#include "ObjectManager.h"
#include "Opcodes.h"
#include "PacketMethods.h"
#include "StaticData.h"
#include "UDPConnection.h"
#include "MOBDatabase.h"

long Player::GenerateTalkTree(TalkTree *tree, int tree_node_id)
{
	char *ptr = m_TalkTreeBuffer;
	TalkNode *talk_node = tree->Nodes[tree_node_id];
	BranchList::iterator itrBranchList;
    TalkBranch *branch;
	long length;
	
	if (talk_node == (0))
	{
		return 0;
	}

	memset(ptr, 0, TALKTREE_BUFFER_SIZE);

	length = ParseTalkTokens(ptr, talk_node->Text);

	ptr = ptr + 2 + length;
	*ptr = (char)talk_node->NumBranches; //set branch count
	ptr++;

	//now add in each branch
	for (itrBranchList = talk_node->Branches.begin(); itrBranchList != talk_node->Branches.end(); ++itrBranchList) 
	{
		branch = (*itrBranchList);
		*ptr = (char)branch->BranchDestination; //set branch destination
		ptr += 4;
		length = ParseTalkTokens(ptr, branch->Text);
		ptr = ptr + 1 + length;
	}

	length = (long)(ptr - m_TalkTreeBuffer);

	return length;
}

long Player::ParseTalkTokens(char *write_buffer, char *input_buffer)
{
    char *profession[] =
    {
        "Warrior",
        "Trader",
        "Explorer"
    };
    char *race[] =
    {
        "Terran",
        "Jenquai",
        "Progen"
    };
	char *aprofession[] =
	{
		"a Warrior",
		"a Trader",
		"an Explorer"
	};

	char *ptr = write_buffer;
	char *token_ptr;

	long length = 0;
	//scan text for flags
	token_ptr = strchr(input_buffer, '@');

	if (token_ptr)
	{
		char *tree_ptr = input_buffer;
		while (*tree_ptr != 0)
		{
			if (*tree_ptr == '@')
			{
				token_ptr = tree_ptr+1;
				switch (token_ptr[0])
				{
				case 'a': //a[n] @profession
					if (strncmp(token_ptr, "aprofession", 11) == 0)
					{
						strcpy(ptr, aprofession[Profession()]);
						ptr += strlen(aprofession[Profession()]);
						tree_ptr += 11;
					}
					break;
				case 'c': //class (== profession)
					if (token_ptr[1] == 'l' && token_ptr[2] == 'a' && token_ptr[3] == 's' &&
						token_ptr[1] == 's')
					{
						strcpy(ptr, profession[Profession()]);
						ptr += strlen(profession[Profession()]);
						tree_ptr += 10;
					}
					break;
				case 'n': //name
					if (token_ptr[1] == 'a' && token_ptr[2] == 'm' && token_ptr[3] == 'e')
					{
						strcpy(ptr, Name());
						ptr += strlen(Name());
						tree_ptr += 4;
					}
					break;
				case 'r': //race
					if (token_ptr[1] == 'a' && token_ptr[2] == 'c' && token_ptr[3] == 'e')
					{
						strcpy(ptr, race[Race()]);
						ptr += strlen(race[Race()]);
						tree_ptr += 4;
					}
					break;
				case 'p':
					if (strncmp(token_ptr, "profession", 10) == 0)
					{
						strcpy(ptr, profession[Profession()]);
						ptr += strlen(profession[Profession()]);
						tree_ptr += 10;
					}
					break;
				default:
					*ptr++ = *tree_ptr;
					break;
				}
			}
			else
			{
				*ptr++ = *tree_ptr;
			}
			tree_ptr++;
		}
	}
	else
	{
		strcpy(ptr, input_buffer);
		ptr += strlen(input_buffer);
	}

	length = (long) (ptr - write_buffer);

	return length;
}

u16 Player::GetNodeFlags(TalkTree *tree, int tree_node_id)
{
	TalkNode *talk_node = tree->Nodes[tree_node_id];
	u16 flags = 0;
	if (talk_node != (0))
	{
		flags = talk_node->Flags;
	}

	return flags;
}

void Player::ResetWeaponMounts()
{
    //now reset each mount
    u32 weapons = 0;
	m_WeaponSlots = 0;
	u32 i;

	for(i=0;i<(u32)WeaponTable[ClassIndex() * 7];i++) 
	{
        AddWeapon(i+1);
	}

	for (i = 1; i <= PlayerIndex()->RPGInfo.GetHullUpgradeLevel(); i++)
	{
		if (WeaponTable[ClassIndex() * 7 + i] != 0)
		{
			AddWeapon(m_WeaponSlots + 1);
		}
	}
}

void Player::ResetDeviceMounts()
{
    //now reset each mount
    u32 devices = 0;
	u32 i;
	m_DeviceSlots = 0;

    for (i = 0; i <= PlayerIndex()->RPGInfo.GetHullUpgradeLevel(); i++)
    {
        devices += DeviceTable[ClassIndex()*7 + i];
    }

	for(i=0;i<devices;i++)
	{
		ShipIndex()->Inventory.Mounts.SetMount(9+i, DeviceMount);
		ShipIndex()->Inventory.EquipInv.EquipItem[9+i].SetItemTemplateID(-1);
		m_DeviceSlots++;
	}
}

void Player::DebugPlayerDock(bool flag)
{
    if (m_MyDebugPlayer == (0)) return;

    if (flag)
    {
        //issue docking impulses
        m_MyDebugPlayer->m_Oldroom = -1;
        m_MyDebugPlayer->m_Room = 0;
        m_MyDebugPlayer->SetActionFlag(65);
        m_MyDebugPlayer->SendStarbaseAvatarList();
        m_MyDebugPlayer->m_Oldroom = 0;
        m_MyDebugPlayer->SetActionFlag(65);
        m_MyDebugPlayer->SendStarbaseAvatarList();
        Sleep(300);
		if (m_MyDebugPlayer)
		{
			m_MyDebugPlayer->BroadcastPosition();
		}
    }
    else
    {
        m_MyDebugPlayer->m_Oldroom = 0;
        m_MyDebugPlayer->m_Room = -1;
        m_MyDebugPlayer->SetActionFlag(65);
        m_MyDebugPlayer->SendStarbaseAvatarList();
        m_MyDebugPlayer->BroadcastPosition();
    }
}

ObjectManager* Player::GetObjectManager()
{
	SectorManager *sm = GetSectorManager();
	ObjectManager *om = (0);
	if (sm)
	{
		om = sm->GetObjectManager();
		m_ObjectMgr = om;
	}

	return om;
}

SectorManager* Player::GetSectorManager()
{
	return g_ServerMgr->GetSectorManager((long)PlayerIndex()->GetSectorNum());
}

long Player::GetSectorNextObjID()
{
	SectorManager *sect_manager = GetSectorManager();
	if (sect_manager)
	{
		return sect_manager->GetSectorNextObjID();
	}
	else
	{
		return 0;
	}
}

void Player::ExposeDecosOn(Object *obj)
{
	if (m_ExposeDecos)
	{
		obj->SetObjectType(OT_NAV);
		obj->SetNavType(2);
	}
}

void Player::ExposeDecosOff(Object *obj)
{
	if (m_ExposeDecos)
	{
		obj->SetObjectType(OT_DECO);
		obj->SetNavType(0);
	}
}

u32 * Player::GetSectorPlayerList()
{
	u32 * sector_list = 0;
    SectorManager *sect_manager = g_ServerMgr->GetSectorManager((long)PlayerIndex()->GetSectorNum());
	if (sect_manager)
	{
		sector_list = sect_manager->GetSectorPlayerList();
	}

	return sector_list;
}

void Player::CheckArrivalTriggers()
{
	//first check for objects within 40K - only need to check these every 2 seconds
	if (m_MovementID%20) // NB movement ID in this func will always be a multiple of 5
	{
		GetObjectManager()->SetObjectsAtRange(this, TRIGGER_RANGE_1, m_NavRange_1, 0);
		GetObjectManager()->SetObjectsAtRange(this, TRIGGER_RANGE_2, m_NavRange_2, m_NavRange_1);
		GetObjectManager()->SetObjectsAtRange(this, TRIGGER_RANGE_3, m_NavRange_3, m_NavRange_2);
	}
}

int Player::GetVenderBuyPrice(int ItemID)
{
	VendorItemList *ItemL = 0;
	ItemBase * Item = 0;

	if (!m_CurrentNPC)
	{
		Item = g_ItemBaseMgr->GetItem(ItemID);
		if (Item)
		{
			return Item->Cost();
		}
		else
		{
			return 0;
		}
	}

    for(vecItemList::const_iterator v_item = m_CurrentNPC->Vendor.Items.begin(); v_item < m_CurrentNPC->Vendor.Items.end(); ++v_item)
    {
		ItemL = g_ServerMgr->m_StationMgr.GetVendorItem(*v_item);

		if (ItemL && ItemL->ItemID == ItemID && (Item = g_ItemBaseMgr->GetItem(ItemL->ItemID)) )
		{
			if (ItemL->BuyPrice > 0)
			{
				return ItemL->BuyPrice;
			}
			else if (ItemL->BuyPrice == -1)
			{
				return 0;
			}
			else //panic! NPC buys for low cost
			{
				return (int)(Item->Cost() * 0.50f);
			}
		}
	}

	Item = g_ItemBaseMgr->GetItem(ItemID);
	if (Item)
	{
		return Item->Cost();
	}

	return 0;
}

/*void Player::CheckArrival()
{
	if (!m_Arrival_Flag && !ObjectIsMoving())
	{
		m_Arrival_Flag = true;
		CheckMissionArrivedAt();
	}
}*/

void Player::AwardCredits(u64 credits, long XP_earned, bool screen_message)
{
	char msg_buffer[128];

	if (credits == 0 && XP_earned == 0) return;

	int sp_len = _snprintf(msg_buffer, 128, "You have gained %ld credits", credits);

	if (XP_earned > 0)
	{
		_snprintf(msg_buffer + sp_len, 128 - sp_len, " and %ld trade experience", XP_earned);
	}

	SendMessageString(msg_buffer, 3);

	if (credits > 0)
	{
		SendClientSound("coin.wav");
		if (screen_message)
		{
			_snprintf(msg_buffer, 128, "%ld credits looted.", credits);
			SendPriorityMessageString(msg_buffer,"MessageLine",5000,4);
		}
	}

	if (XP_earned > 0)
	{
		sprintf(msg_buffer, "Item(s) eligible for");
		AwardTradeXP(msg_buffer, XP_earned);
	}

	PlayerIndex()->SetCredits(PlayerIndex()->GetCredits() + credits);
	SaveCreditLevel();
	SendAuxPlayer();
}

void Player::AwardFaction(long faction_id, long faction_change)
{
	float faction = PlayerIndex()->Reputation.Factions.Faction[faction_id].GetReaction();
	float new_faction_value = faction + (float)faction_change;

	PlayerIndex()->Reputation.Factions.Faction[faction_id].SetReaction(new_faction_value);
	SaveFactionChange(faction_id, new_faction_value);

	SendAuxPlayer();
}

float Player::GetFactionStanding(Object *obj)
{
	float standing = 0.0f;
	if (!obj) return standing;

	long faction_id = obj->GetFactionID();
	long faction_count = g_ServerMgr->m_FactionData.GetFactionCount();
	long player_faction_id = faction_list[ClassIndex()];

	//see if this faction is one of the PDA factions
	if (g_ServerMgr->m_FactionData.GetPDA(faction_id))
	{
		long pda_order = m_PDAFactionID[faction_id];
		//this faction can be influenced by the player's actions, read value from player indicies
		standing = PlayerIndex()->Reputation.Factions.Faction[pda_order].GetReaction();
	}
	else
	{
		//read the faction standing from the database
		standing = g_ServerMgr->m_FactionData.GetFactionStanding(player_faction_id, faction_id);
	}

	return standing;

}