// PlayerCombat.cpp

#include <float.h>
#include "PlayerClass.h"
#include "ServerManager.h"
#include "ObjectManager.h"
#include "Opcodes.h"
#include "PacketMethods.h"

void Player::ShipDamage(long source_id, float weapon_damage, short type)
{
	bool Ignored = false, OnDamage = false;

	if (!m_ObjectMgr)
	{
		return;
	}

    Object *obj = m_ObjectMgr->GetObjectFromID(source_id);
    if (obj && !DebugPlayer())
    {
		float TakeDamage = 0;
		float DamageAbsorbed = 0;

        //terminate warp, prospecting
        //TerminateWarp();						// This should only be done for hull damage
        AbortProspecting(true, false);

		if (m_DamageAbsorb > 0)
		{
			TakeDamage = m_DamageAbsorb - weapon_damage;
			if (TakeDamage < 0)
			{
				// Take damage when we run out of absorb
				m_DamageAbsorb = 0;
				TakeDamage *= -1;
				// Calculate the damage absorbed by the shield
				DamageAbsorbed -= (weapon_damage - TakeDamage);
			}
			else
			{
				// We take no damage because of the shield
				TakeDamage = 0;
				m_DamageAbsorb -= weapon_damage;
				// Calculate the damage absorbed by the shield
				DamageAbsorbed -= weapon_damage;
			}
		}
		else
		{
			// No shield take all damage
			TakeDamage = weapon_damage;
		}

		// TODO: Add code to reduce damage from buffs and deflects

		/*****************
		* Damage reduction section.
		******************/
		float redux_percent;
		float BeforeDamage = TakeDamage;
		switch(type)
		{
		case DAMAGE_IMPACT:
			//is this needed?
			redux_percent = m_Stats.GetStat(STAT_IMPACT_DEFLECT);
			TakeDamage *= (1.0f - (redux_percent > 0.5f ? 0.5f : redux_percent));
			break;
		case DAMAGE_EXPLOSIVE:
			redux_percent = m_Stats.GetStat(STAT_EXPLOSIVE_DEFLECT);
			TakeDamage *= (1.0f - (redux_percent > 0.5f ? 0.5f : redux_percent));
			break;
		case DAMAGE_PLASMA:
			redux_percent = m_Stats.GetStat(STAT_PLASMA_DEFLECT);
			TakeDamage *= (1.0f - (redux_percent > 0.5f ? 0.5f : redux_percent));
			break;
		case DAMAGE_ENERGY:
			redux_percent = m_Stats.GetStat(STAT_ENERGY_DEFLECT);
			TakeDamage *= (1.0f - (redux_percent > 0.5f ? 0.5f : redux_percent));
			break;
		case DAMAGE_EMP:
			redux_percent = m_Stats.GetStat(STAT_EMP_DEFLECT);
			TakeDamage *= (1.0f - (redux_percent > 0.5f ? 0.5f : redux_percent));
			break;
		case DAMAGE_CHEMICAL:
			redux_percent = m_Stats.GetStat(STAT_CHEM_DEFLECT);
			TakeDamage *= (1.0f - (redux_percent > 0.5f ? 0.5f : redux_percent));
			break;
		}

		// Find out if we absorbed any damage with the deflects
		DamageAbsorbed -= (BeforeDamage - TakeDamage);

        SendClientDamage(GameID(), source_id, TakeDamage, DamageAbsorbed, type, 1);

        bool send_shield_hit = GetShieldValue() >= TakeDamage ? true : false;
        if (send_shield_hit)
        {
            m_Mutex.Lock();

			Player *p = (0);
			
			while (g_PlayerMgr->GetNextPlayerOnList(p, m_RangeList))
			{
                p->SendObjectToObjectLinkedEffect(this, obj, 0x0021, 0x0003);
            }

            m_Mutex.Unlock();
        }

        //DB: Note - Shake does not occur on first hit
        //Shake stops when you run out of shields as it expects hull to decrease then
        ShipIndex()->Shake.SetDamage(TakeDamage);

        float shield_level = GetShieldValue();
		float dmg_reduce_percent = 0;

		if(m_CurrentSkill && m_CurrentSkill->SkillInterruptable(&Ignored, &OnDamage, &Ignored))
		{
			if(OnDamage)
			{
				m_CurrentSkill->InterruptSkillOnDamage(TakeDamage);
			}
		}

        if (TakeDamage > shield_level)
        {
            float hull_dmg = TakeDamage - shield_level;
            if (shield_level > 0.0f)
            {
                RemoveShield(shield_level);
            }

			//if dmg type is chemical, do bonus dmg to hull
			if(type == DAMAGE_CHEMICAL)
			{
				hull_dmg *= 1.2f; //20% bonus damage
			}

			//calculate in damage control, TW/PW only
			//Note: This is normal %, if you want the "effective percent" (like Turbo uses) you need to change this.
			if ( (Profession() == PROFESSION_WARRIOR && Race() == RACE_PROGEN) || 
				(Profession() == PROFESSION_WARRIOR && Race() == RACE_TERRAN) )
			{
				dmg_reduce_percent = 1.0f - m_Stats.GetStat(STAT_DAMAGE_CONTROL);
				if(dmg_reduce_percent < 0)
				{
					dmg_reduce_percent = 0;
				}
				hull_dmg = hull_dmg * dmg_reduce_percent;
			}

			//if dmg type is EMP, take no damage to hull (EMP can't hurt the hull)
			if(type == DAMAGE_EMP)
			{
				hull_dmg = 0;
			}

            RemoveHull(hull_dmg, obj);
			TerminateWarp();
        }
        else
        {
			//bonus damage for plasma
			if(type == DAMAGE_PLASMA)
			{
				TakeDamage *= 1.2f; //20% bonus damage
			}
            RemoveShield(TakeDamage);
        }
    }
}

void Player::RemoveHull(float hull_dmg, Object *mob)
{
    float hull = ShipIndex()->GetHullPoints() - hull_dmg;

    ShipIndex()->SetHullPoints(hull);

    //If we have used all of the hull, the player goes BOOM!
    if (hull <= 0.0f)
    {
        ShipIndex()->SetHullPoints(0);
        ShipIndex()->SetIsIncapacitated(true);

		if(GetCurrentSkill())
		{
			GetCurrentSkill()->InterruptSkillOnAction(INCAPACITATE);
		}

		// Break formation/Leave Formation if in group
		if (GroupID() != -1)
		{
			g_PlayerMgr->BreakFormation(GameID());
			g_PlayerMgr->LeaveFormation(GameID());
		}

		// Stop regen
		RemoveEnergy(0);
		ShipIndex()->Shield.SetStartValue(GetShield());
		ShipIndex()->Shield.SetEndTime(GetNet7TickCount());		// Set end time now!

        ImmobilisePlayer();
        TerminateWarp();    //This sends the packet (TODO: Remove packet send from here)

		// Send Data
		SendAuxShip();

		//now deselect player's target - this causes the immobilisation to occur
		UnSetTarget(-1);

        //Stop any mobs from attacking
		//ObjectManager *om = GetObjectManager();
        //if (om) om->RemovePlayerFromMOBRangeLists(this);
        if (mob && mob->HostilityTarget() == this->GameID())
        {
            mob->LostTarget(this);
        }

        m_Mutex.Lock();

        Player *p = (0);

		while (g_PlayerMgr->GetNextPlayerOnList(p, m_RangeList))
		{
            p->PointEffect(Position(), 1017);
        }

        m_Mutex.Unlock();
    }
}

void Player::ImmobilisePlayer()
{
    ShipIndex()->SetLockOrient(true);
    ShipIndex()->SetLockSpeed(true);
    ShipIndex()->SetEngineThrustState(0);  
}

void Player::RemobilisePlayer()
{
    ShipIndex()->SetLockOrient(false);
    ShipIndex()->SetLockSpeed(false);
}

void Player::AttackMusicUpdate(bool update, long mob_id)
{
    if (update)
    {
        m_AttackerCount++;
        SendAttackerUpdates(ntohl(mob_id), 1);
    }
    else
    {
        m_AttackerCount--;
        SendAttackerUpdates(ntohl(mob_id), 0);
        
        if (m_AttackerCount < 0)
        {
            LogMessage("**** ERROR *: Attack count negative for %s\n", Name());
        }
    }
}

void Player::FireAllWeapons()
{
    if (ShipIndex()->GetTargetGameID() > 0)
    {
        for (int i=0; i<6; i++)
        {
            m_Equip[i+3].ManualActivate();
        }
        SendPacketCache();
    }
}

bool Player::FireEnergyCannon(ItemInstance *item)
{
    float pos[3];
    float *_heading = Heading();
    float range = (float)item->WeaponRange;
    
    if (GetEnergyValue() < item->EnergyUse)
    {
        SendVaMessage("Not enough energy! Need: %f", item->EnergyUse);
        return false;
    }
   
    /* Use the energy */
    RemoveEnergy(item->EnergyUse);
       
    pos[0] = PosX() + ( range * _heading[0] );
    pos[1] = PosY() + ( range * _heading[1] );
    pos[2] = PosZ() + ( range * _heading[2] );
   
    RangeListVec::iterator itrRList;
    Player *p = (0);
    
	while (g_PlayerMgr->GetNextPlayerOnList(p, m_RangeList))
	{        
        p->PointEffect(pos, 1017, 3.0f);
        //see if this player is in range of explosion.
        float range_to_blast = p->RangeFrom(pos, true);
        if (p != this)
        {
            SendVaMessage("Range to blast: %.2f", range_to_blast);
        }
        if (range_to_blast < 1000.0f)
        {
            float damage = p->CaughtInEnergyBlast(GameID(), range_to_blast);
            if (damage > 0)
            {
                SendVaMessage("Blast Damage: %.2f", damage);
                SendClientDamage(p->GameID(), GameID(), damage, 0, 0);
            }
        }
    }

    return true;
}

float Player::CaughtInEnergyBlast(long source_id, float blast_range)
{
    float blast_damage = 1.0f;
    if (blast_range < 750.0f)
    {
        blast_damage = 5.0f;
    }
    if (blast_range < 500.0f)
    {
        blast_damage = 10.0f;
    }
    if (blast_range < 400.0f)
    {
        blast_damage = 15.0f;
    }
    if (blast_range < 300.0f)
    {
        blast_damage = 20.0f;
    }
    if (blast_range < 200.0f)
    {
        blast_damage = 30.0f;
    }
    if (blast_range < 100.0f)
    {
        blast_damage = 75.0f;
    }
    if (blast_range < 50.0f)
    {
        blast_damage = 200.0f;
    }

    ShipDamage(source_id, blast_damage, 0);

    return (blast_damage);
}

float Player::CalcDamage(int weapon_damage, int subcat, bool *critical)
{
	const short FRACTION_MULTIPLIER = 10000; //This value determines how many decimals of a percent are kept for rolling damage tables.
	const short FRACTION_FIXUP = 100;
    float damage_bonus;
    float damage_inflicted;
    short critical_chance = 5 * FRACTION_FIXUP; //5%
	short miss_chance = 5 * FRACTION_FIXUP; //5% , miss_chance of 0 = never miss.

	//This calculates a different crit value for any class that should have critical targeting.
    critical_chance += (short) 
		( (m_Stats.GetStatType(STAT_CRITICAL_RATE, STAT_BUFF_MULT)*FRACTION_MULTIPLIER) +
		(m_Stats.GetStatType(STAT_CRITICAL_RATE, STAT_BUFF_VALUE)*FRACTION_FIXUP) );

	if(FRACTION_MULTIPLIER-critical_chance < 0)
	{
		critical_chance = 0; //100% crits, when you hit.
	}

	//TO-DO: compute miss_chance based on weapon skill + buffs

    // Get Damage bonus by weapon type
    switch(subcat)
    {
    case 100:		// Beam
        damage_bonus = 1.0f + m_Stats.GetStatType(STAT_BEAM_DAMAGE, STAT_BUFF_MULT);
		
		//can't call CalculateStat because STAT_BASE_VALUE = 0, and so any % modifiers would be lost
		miss_chance -= (short) 
			( (m_Stats.GetStatType(STAT_BEAM_ACCURACY, STAT_BUFF_MULT)*FRACTION_MULTIPLIER) +
			(m_Stats.GetStatType(STAT_BEAM_ACCURACY, STAT_BUFF_VALUE)*FRACTION_FIXUP) );
        break;

    case 101:		// Projectile
        damage_bonus = 1.0f + m_Stats.GetStatType(STAT_PROJECTILES_DAMAGE, STAT_BUFF_MULT);
		
		//can't call CalculateStat because STAT_BASE_VALUE = 0, and so any % modifiers would be lost
		miss_chance -= (short) 
			( (m_Stats.GetStatType(STAT_PROJECTILES_ACCURACY, STAT_BUFF_MULT)*FRACTION_MULTIPLIER) +
			(m_Stats.GetStatType(STAT_PROJECTILES_ACCURACY, STAT_BUFF_VALUE)*FRACTION_FIXUP) );
        break;

    case 102:		// Missiles
        damage_bonus = 1.0f + m_Stats.GetStatType(STAT_MISSILE_DAMAGE, STAT_BUFF_MULT);

		//can't call CalculateStat because STAT_BASE_VALUE = 0, and so any % modifiers would be lost
		miss_chance -= (short) 
			( (m_Stats.GetStatType(STAT_MISSILE_ACCURACY, STAT_BUFF_MULT)*FRACTION_MULTIPLIER) +
			(m_Stats.GetStatType(STAT_MISSILE_ACCURACY, STAT_BUFF_VALUE)*FRACTION_FIXUP) );
        break;

    default:
        LogMessage("ERROR: Weapon subcategory [%d] wrong for %s\n", subcat, Name());
        damage_bonus = 0.0f;
        break;
    }

    //find the base weapon damage, first have we got a critical or missed?
    short to_hit = (rand() % FRACTION_MULTIPLIER) + 1;

    float damage_fraction = 1.0f;

    //This system rolls a number, and then determines which category of damage it falls into.
	//It is still possible to miss with a 100% crit rate. It is possible to miss on a crit.
    if (to_hit <= miss_chance)
	{
        //Weapon missed, damage zero
        damage_bonus = 0;
    }
    else if (to_hit >= FRACTION_MULTIPLIER-critical_chance)
    {
        //Critical!! Double damage
        damage_bonus = damage_bonus * 2.0f;
        *critical = true;
    }
	//IMPLIED else: did normal damage.

	//TO-DO: Put in fractional damage FOR BEAMS ONLY, and also DOTs for Chemical/Plasma damage.

    //Now calculate the damage based on weapon base damage and bonus.
    damage_inflicted = damage_fraction * (float)(weapon_damage) * damage_bonus;

    return (damage_inflicted);
}

static char *mount_names[] =
{
    "~WEAP_06",     // 0 nose mount 1 (progen only) 
    "~WEAP_05",     // 1 nose mount 2 (All other races, centre).
    "~02/~WEAP_06", // 2 wing mounts
    "~02/~WEAP_05", // 3
    "~02/~WEAP_04", // 4
    "~02/~WEAP_03", // 5
    "~02/~WEAP_02", // 6
    "~02/~WEAP_01", // 7
    "~01/~WEAP_02", // 8 pod mounts for Progen Warriors (on the ends of the pylons).
    "~01/~WEAP_01"  // 9
};

static char MountNameIndexes[] = //choose names from above table
{
    1, 4, 5, 6, 7,-1,  // TW
    1, 4, 5, 6,-1,-1,  // TT
    4, 5, 6, 7,-1,-1,  // TE
    1, 4, 5, 6, 7,-1,  // JW
    4, 5, 6, 7,-1,-1,  // JT
    1, 4, 5,-1,-1,-1,  // JE
    9, 8, 4, 5, 2, 3,  // PW
    2, 3, 4, 5,-1,-1,  // PT
    0, 1, 4, 5,-1,-1   // PE
};

static char PEAltMounts[] =
{
    0, 1, 4, 5,-1,-1,   // PE hull 1
    2, 3, 4, 5,-1,-1,
    0, 1, 4, 5,-1,-1
};

void Player::AddWeapon(long weapon_id)  //weapon 1 to 6
{
    char *bonename;
    if (MountNameIndexes[ClassIndex() * 6 + (weapon_id - 1)] == -1)
    {
        LogMessage("Error adding weapon for %s, class not permitted to have weapon #%d\n", Name(), weapon_id);
        return;
    }
    else
    {
        if (ClassIndex() == 8) //TODO: put class indexes on an enum for clarity. 8 = PE
        {
            bonename = mount_names[PEAltMounts[m_Database.ship_data.hull * 6 + (weapon_id - 1)]];
        }
        else
        {
            bonename = mount_names[MountNameIndexes[ClassIndex() * 6 + (weapon_id - 1)]];
        }
    }
    
    ShipIndex()->Inventory.Mounts.SetMount(2+weapon_id, WeaponMount);
    ShipIndex()->Inventory.EquipInv.EquipItem[2+weapon_id].SetItemTemplateID(-1);
    ShipIndex()->Inventory.MountBones.SetMountBoneName(2 + weapon_id, bonename);

	m_WeaponSlots++;
}

void Player::ChangeMountBoneName(long weapon_id, char *mount_name)
{
    ShipIndex()->Inventory.MountBones.SetMountBoneName(2 + weapon_id, mount_name);
}

void Player::NeatenUpWeaponMounts()
{
    for (int i = 0; i < 6; i++)
    {
        if (MountNameIndexes[ClassIndex() * 6 + i] != -1)
        {
            ShipIndex()->Inventory.MountBones.SetMountBoneName(3 + i, mount_names[MountNameIndexes[ClassIndex() * 6 + i]]);
        }
    }
}
