//Equipable.cpp

#include "Net7.h"
#include "Equipable.h"
#include "ServerManager.h"
#include "PlayerClass.h"
#include "MOBClass.h"
#include "ObjectManager.h"

Equipable::Equipable()
{
    m_Player = 0;
    m_ItemBase = 0;
    m_Slot = 0;
    m_ReadyTime = GetNet7TickCount();
    m_UsesAmmo = false;
    m_AuxEquipItem = 0;
    m_AuxAmmoItem = 0;
	m_MaxID = 0;
	m_EEffectID = 0;

    memset(&m_ItemInstance, 0, sizeof(ItemInstance));
    memset(&m_AmmoInstance, 0, sizeof(AmmoInstance));
    memset(&m_TimeNode, 0, sizeof(TimeNode));

	m_AmmoBase = 0;
}

Equipable::~Equipable()
{
}

void Equipable::Init(Player * Owner, int SlotNum)
{
    m_Player = Owner;
    m_Slot = SlotNum;

    if (SlotNum == 0)
    {
        m_Type = EQUIP_SHIELD;
    }
    else if (SlotNum == 1)
    {
        m_Type = EQUIP_REACTOR;
    }
    else if (SlotNum == 2)
    {
        m_Type = EQUIP_ENGINE;
    }
    else if (SlotNum >= 3 && SlotNum <= 8)
    {
        m_Type = EQUIP_WEAPON;
    }
    else if (SlotNum >= 9 && SlotNum <= 14)
    {
        m_Type = EQUIP_DEVICE;
    }

	m_AmmoBase = 0;
    m_UsesAmmo = false;
    m_AuxEquipItem = &Owner->ShipIndex()->Inventory.EquipInv.EquipItem[SlotNum];
    m_AuxAmmoItem = &Owner->ShipIndex()->Inventory.AmmoInv.Item[SlotNum];
    m_ReadyTime = m_AuxEquipItem->GetReadyTime();

	// Make sure we are calculating quality on item
	Owner->QualityCalculator(m_AuxEquipItem->GetItemData());
}

/* We need to initialize this class with Aux data */
void Equipable::PullAuxData()
{
    m_ItemBase = g_ItemBaseMgr->GetItem(m_AuxEquipItem->GetItemTemplateID());

    if (!m_ItemBase)
    {
        return;
    }

    m_UsesAmmo = (m_ItemBase->SubCategory() == 101 || m_ItemBase->SubCategory() == 102);

	m_ItemInstance = m_ItemBase->GetItemInstance(m_AuxEquipItem->GetInstanceInfo());

	if (m_UsesAmmo && m_AuxAmmoItem->GetItemTemplateID() > 0)
	{
		if (m_AmmoBase = g_ItemBaseMgr->GetItem(m_AuxAmmoItem->GetItemTemplateID()))
		{
			m_AmmoInstance = m_AmmoBase->GetAmmoInstance(m_AuxAmmoItem->GetInstanceInfo());
		}
	}

    //TODO: Once server is stable, this needs to check if ReadyTime < GetNet7TickCount() first
    m_AuxEquipItem->SetReadyTime(GetNet7TickCount());

    /* Only reactors, engines and shields have stats. Other items only have effects */
    //if (m_Slot < 3)
    //{
        SetStats();
    //}

    //AddEffects();
}

/* Checks if an item can be equiped in this slot */
bool Equipable::CanEquip(_Item * NewItem)
{
    /* If this is a weapon/device then we can unequip it */
	if (m_Slot >= 3 && m_Slot <= 14 && NewItem->ItemTemplateID == -1)
    {
		return true;
    }

	ItemBase * myItemBase = g_ItemBaseMgr->GetItem(NewItem->ItemTemplateID);

    /* If we fail to find an itembase - exit */
	if (!myItemBase)
    {
		return false;
    }

    int SubCat = myItemBase->SubCategory();

    /* Now check to see if this is ammo for current item */
    if (SubCat == 103 && m_UsesAmmo && !CorrectAmmo(NewItem))
    {
		m_Player->SendPriorityMessageString("The ammo does not fit here","MessageLine",2000,4);
        //printf("CanEquip - Wrong ammo\n");
        return false;
    }

    /* Cannot equip ammo without a launcher */
    if (SubCat == 103 && !m_UsesAmmo)
    {
		m_Player->SendPriorityMessageString("Weapon doesn't require ammo","MessageLine",2000,4);
		//m_Player->SendVaMessage("Weapon doesn't use ammo");
        //printf("CanEquip - Ammo with no launcher\n");
        return false;
    }

    /* Now check that the item matches the slot type */
	if ((m_Slot == 0 && SubCat != 122) ||  // Shield
        (m_Slot == 1 && SubCat != 120) ||  // Reactor
        (m_Slot == 2 && SubCat != 121) ||  // Engine
        (m_Slot >= 3 && m_Slot <= 8 && SubCat != 100 && SubCat != 101 && SubCat != 102 && SubCat != 103) || //Weapon/Ammo
        (m_Slot >= 9 && m_Slot <= 15 && SubCat != 110))  //Device
    {
		m_Player->SendPriorityMessageString("Item does not fit here","MessageLine",2000,4);
        //printf("CanEquip - Wrong item for slot\n");
		return false;
    }


    AuxSkill * Skills = &m_Player->PlayerIndex()->RPGInfo.Skills.Skill[0];

    /* Now check skill requirements */
    if ((SubCat == 100 && Skills[SKILL_BEAM_WEAPON].GetLevel() < myItemBase->TechLevel()) ||
        (SubCat == 101 && Skills[SKILL_PROJECTILE_WEAPON].GetLevel() < myItemBase->TechLevel()) ||
        (SubCat == 102 && Skills[SKILL_MISSILE_WEAPON].GetLevel() < myItemBase->TechLevel()) ||
        (SubCat == 110 && Skills[SKILL_DEVICE_TECH].GetLevel() < myItemBase->TechLevel()) ||
        (SubCat == 120 && Skills[SKILL_REACTOR_TECH].GetLevel() < myItemBase->TechLevel()) ||
        (SubCat == 121 && Skills[SKILL_ENGINE_TECH].GetLevel() < myItemBase->TechLevel()) ||
        (SubCat == 122 && Skills[SKILL_SHIELD_TECH].GetLevel() < myItemBase->TechLevel()))
    {
		m_Player->SendPriorityMessageString("You need more skill to equip this item","MessageLine",2000,4);
        //printf("CanEquip - bad skill\n");
        return false;
    }

    ItemRequirements Req = myItemBase->GetItemRequirements();

    /* Now check for race restrictions */
    if (Req.RaceRestriction & (0x01 << m_Player->Race()))
    {
        //printf("CanEquip - Race restriction\n");
		m_Player->SendPriorityMessageString("Your Race can not equip this item","MessageLine",2000,4);
        return false;
    }

    /* Also check for race lore restrictions */
    if ((m_Player->Race() == 1 && Req.LoreRestriction == 0x02) ||
        (m_Player->Race() == 2 && Req.LoreRestriction == 0x01))
    {
        //printf("CanEquip - Lore restriction\n");
		m_Player->SendPriorityMessageString("Your Lore can not equip this item","MessageLine",2000,4);
        return false;
    }

    /* Now check for profession restrictions */
    if (Req.ProfessionRestriction & (0x01 << m_Player->Profession()))
    {
		m_Player->SendPriorityMessageString("Your profession can not equip this item","MessageLine",2000,4);
        //printf("CanEquip - Profession restriction\n");
        return false;
    }

    /* Now check for level requirements */
    if ((Req.CombatRequirement > m_Player->CombatLevel()) ||
        (Req.ExploreRequirement > m_Player->ExploreLevel()) ||
        (Req.TradeRequirement > m_Player->TradeLevel()) ||
        (Req.OverallRequirement > m_Player->TotalLevel()))
    {
		m_Player->SendPriorityMessageString("You can not equip this item","MessageLine",2000,4);
        //printf("CanEquip - level restriction\n");
        //printf("Combat Req %d Act %d\n",Req.CombatRequirement,m_Player->CombatLevel());
        //printf("Explore Req %d Act %d\n",Req.CombatRequirement,m_Player->ExploreLevel());
        //printf("Trade Req %d Act %d\n",Req.CombatRequirement,m_Player->TradeLevel());
        //printf("Overall Req %d Act %d\n",Req.CombatRequirement,m_Player->TotalLevel());
        return false;
    }

	return true;
}

bool Equipable::CorrectAmmo(_Item * Ammo)
{
    /* If our current item is empty, cant equip ammo */
    if (!m_ItemBase)
    {
        return false;
    }

    /* If our current item does not use ammo, then we cannot equip any */
    if (!m_UsesAmmo)
    {
        return false;
    }

    /* If we are removing ammo and have ammo to remove, return true */
    if (Ammo->ItemTemplateID == -1 && m_AuxAmmoItem->GetItemTemplateID() != -1)
    {
        return true;
    }

    /* If for some reason the ammo field is null, print an error and exit */
    if (m_ItemInstance.WeaponAmmo == 0)
    {
        LogMessage("Item ID: [%d] uses ammo but has null ammo field\n", m_AuxEquipItem->GetItemTemplateID());
        return false;
    }

	ItemBase * newItemBase = g_ItemBaseMgr->GetItem(Ammo->ItemTemplateID);

    /* If we fail to find an itembase - exit */
	if (!newItemBase)
    {
		return false;
    }

    /* Check to see if this is ammo */
    if (newItemBase->SubCategory() != 103)
    {
        return false;
    }

    /* Now see if the ammo matches */
    if (!strstr(newItemBase->Fields(1)->sData, m_ItemInstance.WeaponAmmo))
    {
		m_Player->SendVaMessage("Wrong ammo type. Trying to install '%s'. Weapon takes '%s'", newItemBase->Fields(1)->sData, m_ItemInstance.WeaponAmmo);
        return false;
    }

    return true;
}

_Item *Equipable::GetItem()
{
	if (m_AuxEquipItem)
	{
		return (m_AuxEquipItem->GetItemData());
	}
	else
	{
		return (0);
	}
}

/* This returns the item thats un-equiped as its possible to unequip ammo and leave launcher */
_Item Equipable::Equip(_Item * NewItem)
{
    /* NOTE: This item has already passed equip checks */
    if (NewItem->ItemTemplateID == -2)
    {
        return *NewItem;
    }

    /* Check to see if we are equiping ammo */
    if (CorrectAmmo(NewItem))
    {
		return EquipAmmo(NewItem);
    }

    /* Unequip item */
    if (NewItem->ItemTemplateID == -1)
    {
        RemoveEffects();
		SetStats(true);		// Remove old Stats

        _Item OldItem = *m_AuxEquipItem->GetItemData();

        m_AuxEquipItem->SetItemData(NewItem);
        m_ItemBase = 0;
        m_AmmoBase = 0;
        m_UsesAmmo = false;

        /* If this was a weapon, remove the Asset */
        if (m_Type == EQUIP_WEAPON)
        {
            m_Player->ShipIndex()->Lego.Attachments.Attachment[m_Slot-3].Clear();
        }

		m_Player->SaveEquipmentChange(m_Slot, NewItem);

        return OldItem;
    }

	/* If we have ammo, it gets removed with the launcher */
	if (m_UsesAmmo && m_AuxAmmoItem->GetItemTemplateID() > 0)
	{
        m_Player->CargoAddItem(m_AuxAmmoItem->GetData());
        m_AuxAmmoItem->Clear();
        m_AmmoBase = 0;
		m_Player->SaveAmmoChange(m_Slot, m_AuxAmmoItem->GetData());
	}

	// Make sure we are calculating quality on item
	m_Player->QualityCalculator(NewItem);

    ItemBase * NewItemBase = g_ItemBaseMgr->GetItem(NewItem->ItemTemplateID);

    /* Make sure we have an itembase, if not, return the item back */
    if (!NewItemBase)
    {
        LogMessage("Could not find ItemBase for ItemID %d\n",NewItem->ItemTemplateID);
        return *NewItem;
    }

    /* Begin equip process */
    RemoveEffects();
	SetStats(true);		// Remove old Stats

	m_ItemBase = NewItemBase;
    m_ItemInstance = m_ItemBase->GetItemInstance(NewItem->InstanceInfo);

    /* If this is a launcher set the Ammo flag */
    m_UsesAmmo = (m_ItemBase->SubCategory() == 101 || m_ItemBase->SubCategory() == 102);

    /* For now, make equip time be 10 seconds for each activatable effect, plus weapon reload time */
    unsigned long EquipTime = (unsigned long)(m_ItemBase->ActivatableCount() * 10000) +
							  (unsigned long)(m_ItemInstance.WeaponReload * 1000.0f);

    _Item OldItem = *m_AuxEquipItem->GetItemData();

    /* Install this item */
    m_AuxEquipItem->SetItemData(NewItem);

    /* If this was a weapon, set the Asset */
    if (m_Type == EQUIP_WEAPON)
    {
        m_Player->ShipIndex()->Lego.Attachments.Attachment[m_Slot-3].Clear();
        m_Player->ShipIndex()->Lego.Attachments.Attachment[m_Slot-3].SetAsset(m_ItemBase->GameBaseAsset());
        m_Player->ShipIndex()->Lego.Attachments.Attachment[m_Slot-3].SetType(2);
        m_Player->ShipIndex()->Lego.Attachments.Attachment[m_Slot-3].SetBoneName(m_Player->ShipIndex()->Inventory.MountBones.GetMountBoneName(m_Slot));

        /* Turn on autofire for this item */
        m_AuxEquipItem->SetItemState(m_AuxEquipItem->GetItemState() | ITEM_STATE_AUTO_FIRE_ENABLE);
    }


    Install(EquipTime);

	m_Player->SaveEquipmentChange(m_Slot, NewItem);

    return OldItem;
}

_Item Equipable::EquipAmmo(_Item * NewAmmo)
{
    _Item OldAmmo = *m_AuxAmmoItem->GetData();

    if (m_AmmoBase = g_ItemBaseMgr->GetItem(NewAmmo->ItemTemplateID))
    {
        m_AmmoInstance = m_AmmoBase->GetAmmoInstance(NewAmmo->InstanceInfo);
		UpdateRange();
    }

    m_AuxAmmoItem->SetData(NewAmmo);

	m_Player->SaveAmmoChange(m_Slot, NewAmmo);

    if (m_AuxAmmoItem->GetItemTemplateID() < 0)
    {
        AddItemStateFlag(ITEM_STATE_NO_AMMO);
		AddItemStateFlag(ITEM_STATE_NO_TARGETING);
        AddItemStateFlag(ITEM_STATE_DISABLED);
    }
    else if (m_AuxEquipItem->GetItemState() & ITEM_STATE_NO_AMMO)
    {
        //TODO: Check activation requirements
        RemoveItemStateFlag(ITEM_STATE_NO_AMMO);
		RemoveItemStateFlag(ITEM_STATE_NO_TARGETING);

        /* If the item is read, remove the disabled flag aswell */
        if (m_ReadyTime < GetNet7TickCount())
        {
		    RemoveItemStateFlag(ITEM_STATE_DISABLED);
        }
    }

    return OldAmmo;
}

void Equipable::Install(unsigned long InstallDelay)
{
    m_ReadyTime = GetNet7TickCount() + InstallDelay;
    m_AuxEquipItem->SetReadyTime(m_ReadyTime);

    /* Disable the item since its being installed */
    AddItemStateFlag(ITEM_STATE_DISABLED);

    /* If item has no equip time, install it immediately */
    if (InstallDelay == 0)
    {
        FinishInstall();
    }
    else
    {
        RemoveTimeNode();

		m_TimeNode.player_id = m_Player->GameID();
        m_TimeNode.func = B_ITEM_INSTALL;
        m_TimeNode.i1 = m_Slot;
        m_Player->m_SectorMgr->AddTimedCallPNode(&m_TimeNode, InstallDelay);

        //m_TimeNode = m_Player->m_SectorMgr->AddTimedCall(m_Player, B_ITEM_INSTALL, InstallDelay, 0, m_Slot);
    }
}

void Equipable::Hack(unsigned long InstallDelay)
{

}

void Equipable::FinishInstall()
{
    /* Remove our time node */
    RemoveTimeNode();

    /* Set the ItemState */
    if (!(m_AuxEquipItem->GetItemState() & ITEM_STATE_NO_AMMO))
    {
        RemoveItemStateFlag(ITEM_STATE_DISABLED);
    }

    /* Add the item's stats, if needed */
    SetStats();

    AddEffects();
    m_Player->SendAuxShip();
}

void Equipable::ManualActivate()
{
    /* If we have no item, exit */
    if (m_AuxEquipItem->GetItemTemplateID() < 0)
    {
        return;
    }

    /* If they use the item with it auto firing, it cancels autofire */
    if (m_AuxEquipItem->GetItemState() & ITEM_STATE_AUTO_FIRE)
    {
        RemoveItemStateFlag(ITEM_STATE_AUTO_FIRE);
        m_Player->SendAuxShip();
        return;
    }

    /* Check if item is ready */
    if (!ItemReady())
    {
        return;
    }

    Activate();

    m_Player->SendAuxShip();
}

//TODO: Add checks to activate item when enough energy present
void Equipable::AutoActivate()
{
    //LogMessage("Autofire Item: %d\n", m_Slot);
    Activate();
}

void Equipable::Activate()
{
    Object * Target = (0);
    if (m_Player->m_ObjectMgr)
    {
        Target = m_Player->m_ObjectMgr->GetObjectFromID(m_Player->ShipIndex()->GetTargetGameID());
    }
    else
    {
        return;
    }

    switch (m_Type)
    {
    case EQUIP_WEAPON:
        if (!UseWeapon(Target))
        {
            return;
        }

        break;

    default:
        return;
    };

    /* Set Ready time and send Aux */
    m_AuxEquipItem->SetReadyTime(m_ReadyTime);

    AddItemStateFlag(ITEM_STATE_DISABLED);

    if (m_ReadyTime < GetNet7TickCount()) //sometimes cooldown didn't happen because readytime was before current tick
    {
        LogMessage("Instant Cooldown\n");
        m_Player->m_Equip[m_Slot].CoolDown();
    }
    else
    {
        RemoveTimeNode();

        m_TimeNode.player_id = m_Player->GameID();
        m_TimeNode.func = B_ITEM_COOLDOWN;
        m_TimeNode.i1 = m_Slot;
        m_Player->m_SectorMgr->AddTimedCallPNode(&m_TimeNode, m_ReadyTime - GetNet7TickCount());
    }
}

void Equipable::CancelAutofire()
{
    RemoveItemStateFlag(ITEM_STATE_AUTO_FIRE);
    m_Player->SendAuxShip();
}

void Equipable::CoolDown()
{
    //LogMessage("Cooldown Item: %d\n", m_Slot);

    /* Remove disable flag incase were not autofiring or firing fails */
    RemoveItemStateFlag(ITEM_STATE_DISABLED);

    /* Check if we are autofiring */
    if (m_AuxEquipItem->GetItemState() & ITEM_STATE_AUTO_FIRE)
    {
        RemoveItemStateFlag(ITEM_STATE_AUTO_FIRE);  //FOR NOW
        AutoActivate();
    }

    m_Player->SendAuxShip();
}

bool Equipable::UseWeapon(Object * Target)
{
    if (m_ItemBase->SubCategory() == 99) // TODO: check for this weapon properly
    {
        if (m_Player->FireEnergyCannon(&m_ItemInstance))
        {
            m_ReadyTime = GetNet7TickCount() + (unsigned long)(m_ItemInstance.WeaponReload * 1000.0f);
            return true;
        }
        else
        {
            return false;
        }
    }

    if (Target && Target->ObjectType() == OT_MOB && !m_Player->ShipIndex()->GetIsIncapacitated())
    {
        if (!CheckRange(Target))
        {
			m_Player->SendPriorityMessageString("Out of range","MessageLine",2000,4);
            return false;
        }

        if (!CheckOrientation(Target))
        {
			m_Player->SendPriorityMessageString("You must face target","MessageLine",2000,4);
            return false;
        }

        if (m_Player->GetEnergyValue() < m_ItemInstance.EnergyUse)
        {
			m_Player->SendPriorityMessageString("Not enough energy!","MessageLine",2000,4);
	        return false;
        }

		// Make sure we have ammo
		if ((m_ItemBase->SubCategory() == 101 || m_ItemBase->SubCategory() == 102))
		{
			if (m_AuxAmmoItem->GetItemTemplateID() < 0)
			{
				AddItemStateFlag(ITEM_STATE_NO_AMMO);
				AddItemStateFlag(ITEM_STATE_NO_TARGETING);
				AddItemStateFlag(ITEM_STATE_DISABLED);
				m_Player->SendPriorityMessageString("Out of ammo","MessageLine",2000,4);
				return false;
			}
		}

        //see if we are prospecting, if so, cancel prospect.
	    m_Player->AbortProspecting(false, false);

		//see if we are cloaked, if so, interrupt
		if(m_Player->ShipIndex()->GetIsCloaked())
		{
			m_Player->m_AbilityList[CLOAK]->InterruptSkillOnAction(SHOOTING);
		}

        /* Use the energy */
        m_Player->RemoveEnergy(m_ItemInstance.EnergyUse);

        /*If autofire is on item, autofire */
        if (m_AuxEquipItem->GetItemState() & ITEM_STATE_AUTO_FIRE_ENABLE)
        {
            AddItemStateFlag(ITEM_STATE_AUTO_FIRE);
        }

		/* Calculate ready time */
		unsigned long myTime = GetNet7TickCount();
		m_ReadyTime = myTime + ((unsigned long)(m_ItemInstance.WeaponReload * 1000.0f));

		int AmmoShots = m_ItemBase->Fields(22)->iData;
		m_Target = m_Player->ShipIndex()->GetTargetGameID();

		// Calculate delay
		long Delay = (int)(m_ItemInstance.WeaponReload/(float)AmmoShots/2.0f) * 1000;

		if (Delay > 400)
		{
			Delay = 400;
		}

		// Send out # of ammo used
		if (m_ItemBase->SubCategory() != 100)
		{
			if (AmmoShots > 1)
			{
				// Calculate delay
				ShootAmmo(Target->GameID());			// No delay for first one
				for(int x=1;x<AmmoShots;x++)
				{
					// Have it call a timer to shoot off each peice of ammo
					m_Player->m_SectorMgr->AddTimedCall(m_Player, B_SHOOT_AMMO,(long)(Delay * (float) x), NULL, m_Slot, Target->GameID());
				}
			}
			else
			{
				// No need for timer on single ammo
				ShootAmmo(Target->GameID());
			}
		}
		else
		{
			// No need for timer on beams
			ShootAmmo(Target->GameID());
		}
        return true;
    }
    else
    {
        //LogMessage("Weapon use failed\n");
        return false;
    }
}

void Equipable::ShootAmmo(int TargetID)
{
	// Make sure all pointers are not null
	if (!m_ItemBase || !m_Player || !m_Player->m_ObjectMgr)
	{
		return;
	}

	Object * Target = m_Player->m_ObjectMgr->GetObjectFromID(TargetID);

	if (!Target)
	{
		return;
	}

    unsigned long myTime = GetNet7TickCount();

    /* Damage delay is 0 for beams and 1sec/k for launchers */
    u16 DamageDelay = 0;

    if (m_ItemBase->SubCategory() == 101)
    {
        DamageDelay = u16(Target->RangeFrom(m_Player->Position()) * 1.5f);
    }
    else if (m_ItemBase->SubCategory() == 102)
    {
        DamageDelay = u16(Target->RangeFrom(m_Player->Position()) * 2.0f);
    }

    /* Activate effect */
    ObjectToObjectEffect OBTOBE;

    OBTOBE.Bitmask = 0x07;
    OBTOBE.GameID = m_Player->GameID();
    OBTOBE.TargetID = m_Target;

	// Use ammo effect if it needs ammo
	if ((m_ItemBase->SubCategory() == 101 || m_ItemBase->SubCategory() == 102) && m_AmmoBase)
    {
		OBTOBE.EffectDescID = m_AmmoBase->UseEffect();
    }
	else
    {
		OBTOBE.EffectDescID = m_ItemBase->UseEffect();
    }

    char *message = m_Player->ShipIndex()->Inventory.MountBones.GetMountBoneName(m_Slot);

    OBTOBE.Message = message;
    OBTOBE.TimeStamp = myTime;
    OBTOBE.Duration = DamageDelay;
	OBTOBE.EffectID = myTime;

	m_Player->SendObjectToObjectEffectRL(&OBTOBE, true);

    bool critical = false;
	float Damage;

	if (m_ItemBase->SubCategory() == 100)
    {
        Damage = m_Player->CalcDamage(m_ItemInstance.WeaponDamage, m_ItemBase->SubCategory(), &critical);
        Target->DamageMOB(m_Player->GameID(), m_ItemInstance.WeaponDamageType, Damage, critical ? 3 : 0);
    }

	// Projectile or missile
	if (m_ItemBase->SubCategory() == 101 || m_ItemBase->SubCategory() == 102)
	{
		Damage = m_Player->CalcDamage(m_AmmoInstance.WeaponDamage, m_ItemBase->SubCategory(), &critical);
		m_Player->m_SectorMgr->AddTimedCall(0, B_MOB_DAMAGE, DamageDelay, Target, m_Player->GameID(), m_AmmoInstance.WeaponDamageType, critical ? 3 : 0, 0, 0, Damage);

		u32 StackCount = m_AuxAmmoItem->GetStackCount();

		if (StackCount == 1)
		{
			if (!Reload())
			{
				AddItemStateFlag(ITEM_STATE_NO_AMMO);
				AddItemStateFlag(ITEM_STATE_NO_TARGETING);
				AddItemStateFlag(ITEM_STATE_DISABLED);
				RemoveItemStateFlag(ITEM_STATE_AUTO_FIRE);
				m_AuxAmmoItem->Clear();
				m_AmmoBase = 0;
			}
		}
		else
		{
			m_AuxAmmoItem->SetStackCount(StackCount - 1);
		}
	}
}

/* Checks to see if we are within range of the target */
bool Equipable::CheckRange(Object * Target)
{
	return (Target->RangeFrom(m_Player->Position()) <= m_Range);
}

/* Checks to see if we have the correct orientation relative to the target */
bool Equipable::CheckOrientation(Object * Target)
{
    if (m_ItemBase->SubCategory() == 100 || m_ItemBase->SubCategory() == 101)
    {
        return (fabsf(m_Player->GetAngleTo(Target->Position())) < (PI/4.5f));
    }

    return true;
}

/* We are out of ammo, reload the launcher */
bool Equipable::Reload()
{
    ItemBase * Ammo = 0;
	for(u32 i=0; i<m_Player->ShipIndex()->Inventory.GetCargoSpace(); i++)
	{
        Ammo = g_ItemBaseMgr->GetItem(m_Player->ShipIndex()->Inventory.CargoInv.Item[i].GetItemTemplateID());

        /* If this is ammo that matches the launcher */
		if (Ammo && strstr(Ammo->Name(), m_ItemInstance.WeaponAmmo))
		{
			m_AuxAmmoItem->SetData(m_Player->ShipIndex()->Inventory.CargoInv.Item[i].GetData());
			m_Player->ShipIndex()->Inventory.CargoInv.Item[i].Empty();
			return true;
		}
	}

	return false;
}

//This is for the various effects associated with the item
void Equipable::AddEffects()
{
	ItemBase * myItemBase = g_ItemBaseMgr->GetItem(m_AuxEquipItem->GetItemTemplateID());

	if (myItemBase && myItemBase->EquipEffect() > 0)
	{
		// Add Equip Effect
		ObjectEffect Effect;

		Effect.Bitmask = 0x07;
		Effect.EffectDescID = myItemBase->EquipEffect();
		Effect.GameID = m_Player->GameID();
		Effect.Duration = 0;
		Effect.TimeStamp = GetNet7TickCount();

		m_EEffectID = m_Player->m_Effects.AddEffect(&Effect);
		// ----
	}

    /*
	if (m_ItemBase->m_EffectsEquip > 0)
	{
		// Display Equip Effect
		ObjectEffect OBTOBE;

		OBTOBE.Bitmask = 0x07;
		OBTOBE.GameID = m_Player->GameID();
		OBTOBE.EffectDescID = m_ItemBase->m_EffectsEquip;
		OBTOBE.EffectID = GetNet7TickCount();
		OBTOBE.TimeStamp = GetNet7TickCount();
		OBTOBE.Duration = 1750;

        if (m_Player->ConnectionAvailable())
        {
            m_Player->Connection()->SendObjectEffect(m_Player->GameID(), &OBTOBE, TRUE);	// Sector Wide
        }
	}
    */
}

void Equipable::RemoveEffects()
{
	// Remove effect from player
	if (m_EEffectID > 0)
	{
		m_Player->m_Effects.RemoveEffect(m_EEffectID);
		m_EEffectID = 0;
	}
}

void Equipable::UpdateRange()
{
	float Range_Bonus = 1;

	if (m_Type == EQUIP_WEAPON && m_ItemBase)
	{
		switch(m_ItemBase->SubCategory())
		{
			case 100:		// Beam
				Range_Bonus = 1.0f + m_Player->m_Stats.GetStatType(STAT_BEAM_RANGE, STAT_BUFF_MULT);
				m_Range = m_ItemInstance.WeaponRange * Range_Bonus + m_Player->m_Stats.GetStatType(STAT_BEAM_RANGE, STAT_BUFF_VALUE);
				break;

			case 101:		// Projectile
				Range_Bonus = 1.0f + m_Player->m_Stats.GetStatType(STAT_PROJECTILES_RANGE, STAT_BUFF_MULT);
				m_Range = m_ItemInstance.WeaponRange * Range_Bonus + m_Player->m_Stats.GetStatType(STAT_PROJECTILES_RANGE, STAT_BUFF_VALUE);
				break;

			case 102:		// Missiles
				Range_Bonus = 1.0f + m_Player->m_Stats.GetStatType(STAT_MISSILE_RANGE, STAT_BUFF_MULT);
				m_Range = m_AmmoInstance.WeaponRange * Range_Bonus + m_Player->m_Stats.GetStatType(STAT_MISSILE_RANGE, STAT_BUFF_VALUE);
				break;
		}

		if (Range_Bonus < 1.0f)
		{
			Range_Bonus = 1.0f;
		}

		//printf("WeaponRange: %f\n", m_Range);
		m_AuxEquipItem->SetTargetRange(m_Range);
	}
}

//This is for item stats that are part of the item
void Equipable::SetStats(bool Remove)
{
	int RemoveStat;

	if (Remove)
		RemoveStat = -1;
	else
		RemoveStat = 1;

	if (Remove)
	{
		for(int ID=0;ID<m_MaxID;ID++)
		{
			// Loop and remove all the Stats from the ship
			m_Player->m_Stats.DelStat(m_StatIDs[ID]);
		}
	}

	m_MaxID = 0;


    switch(m_Type)
    {
    case EQUIP_WEAPON:
        /* We dont have ammot at this point anyways */
		UpdateRange();

        /* Removes all of the itemstate flags */
        m_AuxEquipItem->SetItemState(m_AuxEquipItem->GetItemState() & 0xFFFFFF00);

        if (m_UsesAmmo && m_AuxAmmoItem->GetItemTemplateID() < 0)
        {
            AddItemStateFlag(ITEM_STATE_NO_AMMO);
			AddItemStateFlag(ITEM_STATE_NO_TARGETING);
		}

        break;

    case EQUIP_DEVICE:
		EquipDevice(!Remove);

        m_AuxEquipItem->SetTargetRange((float)m_ItemInstance.EffectRange);
        break;

    case EQUIP_SHIELD:
        {
			// Dont remove stats from the Stats YET
			if (!Remove)
			{
				// Set BaseValue Stats
				m_StatIDs[m_MaxID++] = m_Player->m_Stats.SetStat(STAT_BASE_VALUE, STAT_SHIELD_RECHARGE, m_ItemInstance.ShieldRecharge, "ITEM_VALUE");
				m_StatIDs[m_MaxID++] = m_Player->m_Stats.SetStat(STAT_BASE_VALUE, STAT_SHIELD, m_ItemInstance.ShieldCap, "ITEM_VALUE");
				// ----

				float StartValue = m_Player->GetShield() / (m_Player->m_Stats.GetStat(STAT_SHIELD) / m_Player->ShipIndex()->GetMaxShield());
				m_Player->ShipIndex()->SetMaxShield(m_Player->m_Stats.GetStat(STAT_SHIELD));

				if (StartValue > 1.0f)
				{
					StartValue = 1.0f;
				}

    			float ChargeRate = (m_Player->m_Stats.GetStat(STAT_SHIELD_RECHARGE) / m_Player->m_Stats.GetStat(STAT_SHIELD)) / 1000.0f;
				unsigned long EndTime = GetNet7TickCount() + (unsigned long)((1.0f - StartValue) / ChargeRate);

				m_Player->ShieldUpdate(EndTime, ChargeRate, StartValue);

				m_AuxEquipItem->SetTargetRange((float)m_ItemInstance.EffectRange);
			}
        }
        break;

    case EQUIP_REACTOR:
        {
			// Dont remove stats from the Stats YET
			if (!Remove)
			{
				// Set BaseValue Stats
				m_StatIDs[m_MaxID++] = m_Player->m_Stats.SetStat(STAT_BASE_VALUE, STAT_ENERGY, m_ItemInstance.ReactorCap, "ITEM_VALUE");
				m_StatIDs[m_MaxID++] = m_Player->m_Stats.SetStat(STAT_BASE_VALUE, STAT_ENERGY_RECHARGE, m_ItemInstance.ReactorRecharge, "ITEM_VALUE");
				// ----

				float MaxEnergy = m_Player->m_Stats.GetStat(STAT_ENERGY);
				float RechargeEnergy = m_Player->m_Stats.GetStat(STAT_ENERGY_RECHARGE);


				float StartValue = m_Player->GetEnergy() / (MaxEnergy / m_Player->ShipIndex()->GetMaxEnergy());
				m_Player->ShipIndex()->SetMaxEnergy(MaxEnergy);

				if (StartValue > 1.0f)
				{
					StartValue = 1.0f;
				}

				float ChargeRate = (RechargeEnergy / MaxEnergy) / 1000.0f;
				unsigned long EndTime = GetNet7TickCount() + (unsigned long)((1.0f - StartValue) / ChargeRate);

				m_Player->EnergyUpdate(EndTime, ChargeRate, StartValue);

				m_AuxEquipItem->SetTargetRange((float)m_ItemInstance.EffectRange);
			}
        }
        break;

    case EQUIP_ENGINE:
		float Speed = (float)m_ItemInstance.EngineSpeed;
		float Acceleration = 100.0f;

		// Dont remove stats from the Stats YET
		if (!Remove)
		{
			// Set BaseValue Stats
			m_StatIDs[m_MaxID++] = m_Player->m_Stats.SetStat(STAT_BASE_VALUE, STAT_IMPULSE, Speed, "ITEM_VALUE");
			m_StatIDs[m_MaxID++] = m_Player->m_Stats.SetStat(STAT_BASE_VALUE, STAT_WARP, (float)m_ItemInstance.EngineWarpSpeed, "ITEM_VALUE");
			m_StatIDs[m_MaxID++] = m_Player->m_Stats.SetStat(STAT_BASE_VALUE, STAT_SIGNATURE, (float)m_ItemInstance.EngineSignature, "ITEM_VALUE");

			if (m_Player->m_Stats.GetStat(STAT_ACCELERATION) == 0)
			{
				m_StatIDs[m_MaxID++] = m_Player->m_Stats.SetStat(STAT_BASE_VALUE, STAT_ACCELERATION, Acceleration);
			}

			m_Player->ShipIndex()->CurrentStats.SetVisibility((s32)m_Player->m_Stats.GetStat(STAT_SIGNATURE));
			m_Player->ShipIndex()->CurrentStats.SetSpeed((s32)m_Player->m_Stats.GetStat(STAT_IMPULSE));
			m_Player->ShipIndex()->CurrentStats.SetWarpSpeed((s32)m_Player->m_Stats.GetStat(STAT_WARP));

			/* Should create a method for this in player */
			m_Player->ShipIndex()->SetMaxSpeed(m_Player->m_Stats.GetStat(STAT_IMPULSE));
			m_Player->ShipIndex()->SetMinSpeed(-m_Player->m_Stats.GetStat(STAT_IMPULSE)/2);
			m_Player->ShipIndex()->SetAcceleration(m_Player->m_Stats.GetStat(STAT_ACCELERATION));

			m_AuxEquipItem->SetTargetRange((float)m_ItemInstance.EffectRange);
		}

        break;
    }

	EquipEffects(RemoveStat);

	/* If this is an empty item, disable the slot */
    if (m_AuxEquipItem->GetItemTemplateID() < 0)
    {
        AddItemStateFlag(ITEM_STATE_DISABLED);
    }
}

void Equipable::EquipEffects(int RemoveStat)
{
	if (!m_ItemBase)
		return;

	ItemBaseData * ItemBaseD = m_ItemBase->Data();

	if (!ItemBaseD)
		return;

	if (ItemBaseD->EquipableEffects.Count > 6)
		return;

	bool Remove = (RemoveStat == -1);

	for(int Effect=0;Effect<ItemBaseD->EquipableEffects.Count;Effect++)
	{
		char *Name = ItemBaseD->EquipableEffects.Effects[Effect].Name;
		float Value = ItemBaseD->EquipableEffects.Effects[Effect].DescVar[0];

		// Increase Warp Speed
		if (!_stricmp(Name, "Increase Warp Speed Item P"))
		{
			if (!Remove)
			{
				// Set Stats
				m_StatIDs[m_MaxID++] = m_Player->m_Stats.SetStat(STAT_BUFF_VALUE, STAT_WARP, Value, Name);

				// Set AUX Stats
				m_Player->ShipIndex()->CurrentStats.SetWarpSpeed((s32)m_Player->m_Stats.GetStat(STAT_WARP));
			}
		}

		// --- Weapon Range --
		//Increase Missile Range Item P
		if (!_stricmp(Name, "Increase Missile Range Item P"))
		{
			if (!Remove)
			{
				// Set Stats
				m_StatIDs[m_MaxID++] = m_Player->m_Stats.SetStat(STAT_BUFF_MULT, STAT_MISSILE_RANGE, (Value/100));
			}

			// Update the ranges for all weapons
			for(int x=3;x<=8;x++)
				m_Player->m_Equip[x].UpdateRange();
		}
		//Increase Beam Range Item P
		if (!_stricmp(Name, "Increase Beam Range Item P"))
		{
			if (!Remove)
			{
				// Set Stats
				m_StatIDs[m_MaxID++] = m_Player->m_Stats.SetStat(STAT_BUFF_MULT, STAT_BEAM_RANGE, (Value/100));
			}

			// Update the ranges for all weapons
			for(int x=3;x<=8;x++)
				m_Player->m_Equip[x].UpdateRange();
		}
		//Increase Projectile Range Item P
		if (!_stricmp(Name, "Increase Projectile Range Item P"))
		{
			if (!Remove)
			{
				// Set Stats
				m_StatIDs[m_MaxID++] = m_Player->m_Stats.SetStat(STAT_BUFF_MULT, STAT_MISSILE_RANGE, (Value/100));
			}

			// Update the ranges for all weapons
			for(int x=3;x<=8;x++)
				m_Player->m_Equip[x].UpdateRange();
		}
		//----------------------

		// Scan Range
		if (!_stricmp(Name, "Increase Scan Range Item P"))
		{
			if (!Remove)
			{
				// Set Stats
				m_StatIDs[m_MaxID++] = m_Player->m_Stats.SetStat(STAT_BUFF_VALUE, STAT_SCAN_RANGE, Value, Name);

				m_Player->ShipIndex()->CurrentStats.SetScanRange((s32)m_Player->m_Stats.GetStat(STAT_SCAN_RANGE));
			}
		}

		// Reduce Scan Range Item P
		if (!_stricmp(Name, "Reduce Scan Range Item P"))
		{
			if (!Remove)
			{
				// Set Stats
				m_StatIDs[m_MaxID++] = m_Player->m_Stats.SetStat(STAT_BUFF_VALUE, STAT_SCAN_RANGE, -Value, Name);

				m_Player->ShipIndex()->CurrentStats.SetScanRange((s32)m_Player->m_Stats.GetStat(STAT_SCAN_RANGE));
			}
		}

		// Increase Signature Item P
		if (!_stricmp(Name, "Increase Signature Item P"))
		{
			if (!Remove)
			{
				// Set Stats
				m_StatIDs[m_MaxID++] = m_Player->m_Stats.SetStat(STAT_BUFF_MULT, STAT_SIGNATURE, (Value/100));

				m_Player->ShipIndex()->CurrentStats.SetVisibility((s32)m_Player->m_Stats.GetStat(STAT_SIGNATURE));
			}
		}

		// Reduce Signature Item P
		if (!_stricmp(Name, "Reduce Signature Item P"))
		{
			if (!Remove)
			{
				// Set Stats
				m_StatIDs[m_MaxID++] = m_Player->m_Stats.SetStat(STAT_BUFF_MULT, STAT_SIGNATURE, -(Value/100));

				m_Player->ShipIndex()->CurrentStats.SetVisibility((s32)m_Player->m_Stats.GetStat(STAT_SIGNATURE));
			}
		}

		// --- Resistances ---
		//Chemical Resistance Item P
		if (!_stricmp(Name, "Chemical Resistance Item P"))
		{
			if (!Remove)
			{
				// Set Stats
				m_StatIDs[m_MaxID++] = m_Player->m_Stats.SetStat(STAT_BUFF_VALUE, STAT_CHEM_DEFLECT, Value, Name);

				// Set AUX Stats
				m_Player->ShipIndex()->CurrentStats.SetResistChemical((s32)m_Player->m_Stats.GetStat(STAT_CHEM_DEFLECT));
			}
		}
		//Impact Resistance Item P
		if (!_stricmp(Name, "Impact Resistance Item P"))
		{
			if (!Remove)
			{
				// Set Stats
				m_StatIDs[m_MaxID++] = m_Player->m_Stats.SetStat(STAT_BUFF_VALUE, STAT_IMPACT_DEFLECT, Value, Name);

				// Set AUX Stats
				m_Player->ShipIndex()->CurrentStats.SetResistImpact((s32)m_Player->m_Stats.GetStat(STAT_IMPACT_DEFLECT));
			}
		}
		//Explosive Resistance Item P
		if (!_stricmp(Name, "Explosive Resistance Item P"))
		{
			if (!Remove)
			{
				// Set Stats
				m_StatIDs[m_MaxID++] = m_Player->m_Stats.SetStat(STAT_BUFF_VALUE, STAT_EXPLOSIVE_DEFLECT, Value, Name);

				// Set AUX Stats
				m_Player->ShipIndex()->CurrentStats.SetResistExplosion((s32)m_Player->m_Stats.GetStat(STAT_EXPLOSIVE_DEFLECT));
			}
		}
		//Energy Resistance Item P
		if (!_stricmp(Name, "Energy Resistance Item P"))
		{
			if (!Remove)
			{
				// Set Stats
				m_StatIDs[m_MaxID++] = m_Player->m_Stats.SetStat(STAT_BUFF_VALUE, STAT_ENERGY_DEFLECT, Value, Name);

				// Set AUX Stats
				m_Player->ShipIndex()->CurrentStats.SetResistEnergy((s32)m_Player->m_Stats.GetStat(STAT_ENERGY_DEFLECT));
			}
		}
		//EMP Resistance Item P
		if (!_stricmp(Name, "EMP Resistance Item P"))
		{
			if (!Remove)
			{
				// Set Stats
				m_StatIDs[m_MaxID++] = m_Player->m_Stats.SetStat(STAT_BUFF_VALUE, STAT_ENERGY_DEFLECT, Value, Name);

				// Set AUX Stats
				m_Player->ShipIndex()->CurrentStats.SetResistEMP((s32)m_Player->m_Stats.GetStat(STAT_ENERGY_DEFLECT));
			}
		}
		//Plasma Resistance Item P
		if (!_stricmp(Name, "Plasma Resistance Item P"))
		{
			if (!Remove)
			{
				// Set Stats
				m_StatIDs[m_MaxID++] = m_Player->m_Stats.SetStat(STAT_BUFF_VALUE, STAT_PLASMA_DEFLECT, Value, Name);

				// Set AUX Stats
				m_Player->ShipIndex()->CurrentStats.SetResistPlasma((s32)m_Player->m_Stats.GetStat(STAT_PLASMA_DEFLECT));
			}
		}
		//Psionic Resistance Item P
		if (!_stricmp(Name, "Psionic Resistance Item P"))
		{
			if (!Remove)
			{
				// Set Stats
				m_StatIDs[m_MaxID++] = m_Player->m_Stats.SetStat(STAT_BUFF_VALUE, STAT_PSIONIC_DEFLECT, Value, Name);

				// Set AUX Stats
				m_Player->ShipIndex()->CurrentStats.SetResistPsionic((s32)m_Player->m_Stats.GetStat(STAT_PSIONIC_DEFLECT));
			}
		}
		// ------------------
	}
}

void Equipable::RemoveTimeNode()
{
    if (m_TimeNode.player_id)
    {
        m_Player->m_SectorMgr->RemoveTimedCall(&m_TimeNode, true);
    }
}

void Equipable::AddItemStateFlag(unsigned long State)
{
    m_AuxEquipItem->SetItemState(m_AuxEquipItem->GetItemState() | State);
}

void Equipable::RemoveItemStateFlag(unsigned long State)
{
    m_AuxEquipItem->SetItemState(m_AuxEquipItem->GetItemState() & ~State);
}

bool Equipable::ItemReady()
{
    return (m_TimeNode.player_id <= 0);
}

float Equipable::GetQuality()
{
	_Item *item = m_AuxEquipItem->GetItemData();
	return item->Quality;
}

void Equipable::EquipDevice(bool equip)
{
	if (m_AuxEquipItem->GetItemTemplateID() == 5081) //grail affinity device
	{
		m_Player->SetGrailAffinity(equip);
		return;
	}

	if (!m_ItemBase) return;

	float neg = equip ? 1.0f : -1.0f;
	float zero = equip ? 1.0f : 0.0f;

	char *description = m_ItemBase->Description();

	//this is a hack, we need a proper way of describing device stats
	if (description)
	{
		//see if this is a sculptor type device
		if (CheckForItem("increase to your Prospect skill", description))
		{
			//ok this device gives an increase to prospect skill. In the absence of any real data, make up the increase based on device level
			//lets say device level/2.25 is the increase.
			m_Player->ChangeProspectSkill(zero * (float)m_ItemBase->TechLevel() / 2.25f);

			if (strstr(description, "and scan range")) //scan range too?
			{
				m_Player->AddScanSkill(neg * m_ItemBase->TechLevel() * 2000.0f);
			}
		}

		//see if this is a harpy's type device
		if (CheckForItem(description, "increase to your tractor beam speed"))
		{
			//ok this device gives an increase to prospect skill. In the absence of any real data, make up the increase based on device level
			//lets say device level/2.25 is the increase.
			m_Player->ChangeTractorBeamSpeed(zero * (float)m_ItemBase->TechLevel() * 0.18f );
		}
	}
}

bool Equipable::CheckForItem(const char *description, const char *search)
{
	long skill_count = 0;
	bool no_duplicate = true;
	bool retval = false;
	if (strstr(description, search))
	{
		retval = true;
		//check to see if there's already an item like this installed
		for(int i=0;i<6;i++)
		{
			_Item *item = m_Player->ShipIndex()->Inventory.EquipInv.EquipItem[9+i].GetItemData();
			if (item->ItemTemplateID > 0)
			{
				ItemBase *itembase = g_ItemBaseMgr->GetItem(item->ItemTemplateID);

				//protect against equipping a null item, but how can that happen?
				if(itembase)
				{
					//what have we here?
					char *item_desc = itembase->Description();
					if (strstr(item_desc, search))
					{
						//already equipped this item, don't allow it to affect stats again
						skill_count++;
						if (skill_count > 1)
						{
							no_duplicate = false;
							break;
						}
					}
				}
			}
		}
	}

	return (retval && no_duplicate);
}
