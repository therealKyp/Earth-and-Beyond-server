#include "../Net7.h"
#include "../AbilityBase.h"
#include "AbilityHullPatch.h"
#include "../PlayerClass.h"
#include "../ServerManager.h"
#include "../ObjectManager.h"

class PlayerClass;

/* TO-DO: Get better values (if needed) for hull repair per lvl
Skill properties:
lvl 1: 50 energy, 90 hull per skill lvl, only useable on self
lvl 2: null
lvl 3: 125 energy, 3K range, only useable on other players & self, 360pts per lvl
lvl 4: range increased to 3250
lvl 5: 250 energy, 3.5K range, 1440pts per lvl
lvl 6: 500 energy, 3.75k range, 2880pts per lvl, all friendly ships within 1500 units of target by 540pts per lvl
lvl 7: 1000 energy, 4k range, 5760pts per lvl, all friendly ships within 3000 units of target by 1800pts per lvl
*/

/*
* This calculates the activation cost of the skill.
*/
float AHullPatch::CalculateEnergy ( float SkillLevel, long SkillRank )
{
	float EnergyCost;

	switch(SkillRank)
	{
	case 1:
		EnergyCost = 50.0f;
		break;
	case 3:
		EnergyCost = 125.0f;
		break;
	case 5:
		EnergyCost = 250.0f;
		break;
	case 6:
		EnergyCost = 500.0f;
		break;
	case 7:
		EnergyCost = 1000.0f;
		break;
	}

	EnergyCost =
		((1.0f + m_Player->m_Stats.GetStatType(STAT_HULL_PATCH_ECOST, STAT_BUFF_MULT)) * EnergyCost) +
		m_Player->m_Stats.GetStatType(STAT_HULL_PATCH_ECOST, STAT_BUFF_VALUE);

	EnergyCost = EnergyCost < 0.0f ? 0.0f : EnergyCost;

	return EnergyCost;
}

/*
* Calculate how much time must pass before the skill activates.
*
* Results are returned in seconds.
*/
float AHullPatch::CalculateChargeUpTime ( float SkillLevel, long SkillRank )
{
	//Yes David, right here I actually DO want their skill at shield recharge.
	float Skill = m_Player->m_Stats.GetStat(STAT_SKILL_HULL_PATCH) + SkillLevel;

	//cap skill level
	Skill = Skill > MAX_SKILL_LEVEL ? MAX_SKILL_LEVEL : Skill;

	//minus 1 second per lvl of skill above the rank
	float ChargeTime = 5.0f - (Skill - SkillRank);

	//ensure wierdness didn't happen
	ChargeTime = ChargeTime > 5.0f ? 5.0f : ChargeTime;

	//apply any direct bonuses to chargetime
	ChargeTime =
		((1.0f - m_Player->m_Stats.GetStatType(STAT_HULL_PATCH, STAT_BUFF_MULT)) * ChargeTime) -
		m_Player->m_Stats.GetStatType(STAT_HULL_PATCH, STAT_BUFF_VALUE);

	//ensure charge time is still positive, or 0
	ChargeTime = ChargeTime > 0.0f ? ChargeTime : 0.0f;

	return ChargeTime;
}

/*
* Compute how much time must pass between skill uses.
*
* Results are returned in seconds.
*/
float AHullPatch::CalculateCoolDownTime ( float SkillLevel, long SkillRank )
{
	return 0.0f;
}

/*
* Calculate the maximum range this rank of the skill can be used at.
*/
float AHullPatch::CalculateRange ( float SkillLevel, long SkillRank )
{
	if(SkillRank < 3)
	{
		return 0.0f;
	}
	else
	{
		float Range = 3000 + (SkillLevel - 3);

		Range =
			((1.0f + m_Player->m_Stats.GetStatType(STAT_HULL_PATCH_RANGE, STAT_BUFF_MULT)) * Range) +
			m_Player->m_Stats.GetStatType(STAT_HULL_PATCH_RANGE, STAT_BUFF_VALUE);
		return Range;
	}
}

/*
* Computes the AoE per skill level for an ability.
*/
float AHullPatch::CalculateAOE ( float SkillLevel, long SkillRank )
{
	return 1500.0f + (1500.0f * (SkillLevel-6));
}

/*
* Returns the ammount of sheilds that have been recharged.
*/
float AHullPatch::CalculateHullRepair ( float SkillLevel, long SkillRank )
{
	//Yes David, right here I actually DO want their skill at shield recharge.
	float Skill = m_Player->m_Stats.GetStat(STAT_SKILL_HULL_PATCH) + SkillLevel;
	float ChargeAmount;

	//cap skill level
	Skill = Skill > MAX_SKILL_LEVEL ? MAX_SKILL_LEVEL : Skill;

	//TO-DO: Write in code for buffs to shield charge ammount per lvl &
	//  to overall ammount charged. If needed.

	switch(SkillRank)
	{
	case 1:
		ChargeAmount = Skill * 90;
		break;
	case 3:
		ChargeAmount = Skill * 360;
		break;
	case 5:
		ChargeAmount = Skill * 1440;
		break;
	case 6:
		ChargeAmount = Skill * 2880;
		break;
	case 7:
		ChargeAmount = Skill * 5760;
		break;
	default:
		ChargeAmount = 0;
		break;
	}

	return ChargeAmount;
}

/*
* Returns the ammount of sheilds that have been recharged to AOE targets.
*/
float AHullPatch::CalculateAOEHullRepair ( float SkillLevel, long SkillRank )
{
	//Yes David, right here I actually DO want their skill at shield recharge.
	float Skill = m_Player->m_Stats.GetStat(STAT_SKILL_HULL_PATCH) + SkillLevel;
	float ChargeAmount;

	//cap skill level
	Skill = Skill > MAX_SKILL_LEVEL ? MAX_SKILL_LEVEL : Skill;

	//TO-DO: Write in code for buffs to shield charge ammount per lvl &
	//  to overall ammount charged. If needed.

	switch(SkillRank)
	{
	case 6:
		ChargeAmount = Skill * 540;
		break;
	case 7:
		ChargeAmount = Skill * 1800;
		break;
	default:
		ChargeAmount = 0;
		break;
	}

	return ChargeAmount;
}

/*
* Determine's which rank of the skill was used based on the SkillID.
*/
long AHullPatch::DetermineSkillRank(int SkillID)
{
	switch(SkillID)
	{
	case PATCH_HULL:
		return 1;
	case REPAIR_HULL:
		return 3;
	case COMBAT_HULL_REPAIR:
		return 5;
	case AREA_HULL_REPAIR:
		return 6;
	case IMPROVED_AREA_HULL_REPAIR:
		return 7;
	default:
		return -1;
	}
}

// --------------------------------------------


bool AHullPatch::CanUse(int TargetID, long AbilityID, long SkillID)
{
	// Get the skill level
	m_SkillLevel = (float) m_Player->PlayerIndex()->RPGInfo.Skills.Skill[SkillID].GetLevel();
	m_SkillRank = DetermineSkillRank(AbilityID);
	m_AbilityID = AbilityID;
	m_SkillID = SkillID;

	//Skill does not exist.
	if(m_SkillRank == -1)
	{
		return false;
	}

	// See if skill is being used
	if (m_InUse)
	{
		return false;
	}

	// Make sure we are not dead
	if (m_Player->ShipIndex()->GetIsIncapacitated())
	{
		SendError("Can not use this ability while dead!");
		return false;
	}

	// See if we can use this skill
	if (TargetID > 0 && m_SkillRank >= 3)
	{
		Object * Target = m_Player->GetObjectManager()->GetObjectFromID(TargetID);	// Get Target

		if (Target && Target->ObjectType() != m_TargetType)
		{
			SendError("Incorrect target type!");
			return false;
		}

		// See if we are in range
		if (Target->RangeFrom(m_Player->Position()) > CalculateRange(m_SkillLevel, m_SkillRank))
		{
			SendError("Out of range!");
			return false;
		}

		if(g_GlobMemMgr->GetPlayer(TargetID)->ShipIndex()->GetIsIncapacitated())
		{
			SendError("Cannot recharge a dead player!");
			return false;
		}
	}

	//If we are prospecting we cannot use this skill
	if (m_Player->Prospecting())
	{
		SendError("Cannot use while prospecting.");
		return false;
	}

	//if we are warping we cannot use the skill
	if (m_Player->WarpDrive())
	{
		SendError("Cannot use while in warp.");
		return false;
	}

	return true;
}


/*
* Send confirmation to a player.
*/
void AHullPatch::Confirmation(bool Confirm)
{
	//Not used
}

/*
* If the call to Confirmation() succeeds, this is called.
*/
void AHullPatch::Execute()
{
	//Not used
}

/*
* This will be the first function called once the skill is determined
* as useable.
*/
bool AHullPatch::Use(int TargetID)
{
	float ChargeTime;
	float EnergyCost;

	if(m_SkillRank >= 3)
	{
		// Target player
		if(TargetID < 0)
		{
			m_PlayerTarget = m_Player;
		}
		else
		{
			m_PlayerTarget = g_GlobMemMgr->GetPlayer(TargetID);
		}

		// If we cant find player return
		if (!m_PlayerTarget)
		{
			return false;
		}
	}

	if(TargetID <= 0)
	{
		m_PlayerTarget = m_Player;
	}

	m_EffectID = GetNet7TickCount();

	// Make sure no one else can use this skill
	m_InUse = true;

	EnergyCost = CalculateEnergy(m_SkillLevel, m_SkillRank);

	// not enough energy
	if (m_Player->GetEnergyValue() < EnergyCost)
	{
		SendError("Not enough energy!");
		m_InUse = false;
		return false;
	}

	// Remove the energy
	m_Player->RemoveEnergy(EnergyCost);

	ChargeTime = CalculateChargeUpTime(m_SkillLevel, m_SkillRank);

	//charge up, apply shield charge after charge is finished.
	SetTimer((long)ChargeTime*1000);

	//This uses the same animation that Shield Recharge does
	ObjectEffect HullPatchChargeEffect;

	memset(&HullPatchChargeEffect, 0, sizeof(HullPatchChargeEffect));		// Zero out memory

	HullPatchChargeEffect.Bitmask = 3;
	HullPatchChargeEffect.GameID = m_Player->GameID();
	HullPatchChargeEffect.TimeStamp = m_EffectID;
	HullPatchChargeEffect.EffectID = m_EffectID;
	HullPatchChargeEffect.Duration = (short)ChargeTime;
	HullPatchChargeEffect.EffectDescID = 733;
	m_Player->SendObjectEffectRL(&HullPatchChargeEffect);

	return true;
}

/*
* This function is called when the SetTimer call returns.
*/
void AHullPatch::Update()
{
	m_Player->RemoveEffectRL(m_EffectID);

	m_EffectID = GetNet7TickCount();

	float HullPoints = CalculateHullRepair(m_SkillLevel , m_SkillRank);

	if(m_SkillRank < 3)
	{
		 // Make sure we dont over fill your shields
        if (HullPoints > m_Player->ShipIndex()->GetMaxHullPoints())
        {
			HullPoints = m_Player->ShipIndex()->GetMaxHullPoints(); // Set it at full
        }

		m_Player->ShipIndex()->SetHullPoints(HullPoints);
	}
	else
	{
		// Make sure we dont over fill your shields
		if (HullPoints > m_PlayerTarget->ShipIndex()->GetMaxHullPoints())
		{
			HullPoints = m_PlayerTarget->ShipIndex()->GetMaxHullPoints();		// Set it at full
		}

		// Update shield
		m_PlayerTarget->ShipIndex()->SetHullPoints(HullPoints);

		// Send Shield update/Energy update
		m_PlayerTarget->SendAuxShip();
	}

	// Update our energy
	m_Player->SendAuxShip();

	if(m_SkillRank < 3)
	{
		ObjectEffect HullPatchEffect;

		memset(&HullPatchEffect, 0, sizeof(HullPatchEffect));		// Zero out memory

		HullPatchEffect.Bitmask = 3;
		HullPatchEffect.GameID = m_Player->GameID();
		HullPatchEffect.TimeStamp = m_EffectID;
		HullPatchEffect.EffectID = m_EffectID;
		HullPatchEffect.Duration = 1000;
		HullPatchEffect.EffectDescID = 136;
		m_Player->SendObjectEffectRL(&HullPatchEffect);
		m_Player->RemoveEffectRL(m_EffectID);
	}
	else
	{
		//beam to target ship
		ObjectToObjectEffect HullPatchBeamEffect;

		memset(&HullPatchBeamEffect, 0, sizeof(HullPatchBeamEffect));		// Zero out memory

		HullPatchBeamEffect.Bitmask = 3;
		HullPatchBeamEffect.GameID = m_Player->GameID();
		HullPatchBeamEffect.TimeStamp = m_EffectID+1;
		HullPatchBeamEffect.EffectID = m_EffectID+1;
		HullPatchBeamEffect.Duration = 1000;
		HullPatchBeamEffect.EffectDescID = 139;
		HullPatchBeamEffect.TargetID = m_PlayerTarget->GameID();
		m_Player->SendObjectToObjectEffectRL(&HullPatchBeamEffect);
		m_Player->RemoveEffectRL(m_EffectID+1);

		//Target ship recharge orb
		ObjectToObjectEffect HullPatchEffect;

		memset(&HullPatchEffect, 0, sizeof(HullPatchEffect));		// Zero out memory

		HullPatchEffect.Bitmask = 3;
		HullPatchEffect.GameID = m_Player->GameID();
		HullPatchEffect.TimeStamp = m_EffectID;
		HullPatchEffect.EffectID = m_EffectID;
		HullPatchEffect.Duration = 1000;
		HullPatchEffect.EffectDescID = 166;
		HullPatchEffect.TargetID = m_PlayerTarget->GameID();
		m_Player->SendObjectToObjectEffectRL(&HullPatchEffect);
		m_Player->RemoveEffectRL(m_EffectID);
	}

	// Allow skill to cool down
	m_InUse = false;
}

/*
* Returns true in the case that this skill can be interrupted.
* What can interrupt the skill is returned inside the OnMotion
*  and OnDamage pointers.
*/
bool AHullPatch::SkillInterruptable(bool* OnMotion, bool* OnDamage, bool* OnAction)
{
	//REMOVE ME
	return false;
}

/*
* Determines whether or not this skill was interrupted based on damage taken.
*/
bool AHullPatch::InterruptSkillOnDamage(float Damage)
{
	ObjectEffect InterruptEffect;

	if(!m_InUse) //cannot interrupt non-active skill.
	{
		return false;
	}

	/*if(damage <= 0.0f)
	{
		return false;
	}*/

	//remove current effect
	m_Player->RemoveEffectRL(m_EffectID);

	//get new effectID
	m_EffectID = GetNet7TickCount();

	InterruptEffect.Bitmask = 3;
	InterruptEffect.GameID = m_Player->GameID();
	InterruptEffect.EffectDescID = 735;
	InterruptEffect.EffectID = m_EffectID;
	InterruptEffect.TimeStamp = m_EffectID;

	m_Player->SendObjectEffect(&InterruptEffect);

	//TO-DO: I don't know if this will work or not.
	m_Player->RemoveEffectRL(m_EffectID);

	m_InUse = false;

	//REMOVE ME
	return false;
}

/*
* Determines whether or not this skill was interrupted based on current motion.
*/
bool AHullPatch::InterruptSkillOnMotion(float Speed)
{
	ObjectEffect InterruptEffect;

	if(!m_InUse) //skill not in use, cannot interrupt.
	{
		return false;
	}

	//remove current effect
	m_Player->RemoveEffectRL(m_EffectID);

	//get new effectID
	m_EffectID = GetNet7TickCount();

	InterruptEffect.Bitmask = 3;
	InterruptEffect.GameID = m_Player->GameID();
	InterruptEffect.EffectDescID = 735;
	InterruptEffect.EffectID = m_EffectID;
	InterruptEffect.TimeStamp = m_EffectID;

	m_Player->SendObjectEffect(&InterruptEffect);

	//TO-DO: I don't know if this will work or not.
	m_Player->RemoveEffectRL(m_EffectID);

	m_InUse = false;

	//REMOVE ME
	return false;
}

/*
* Returns true if this skill was interrupted based on player action.
*
* Action = Shooting guns, using devices or activated effects, starting to warp,
*  anything except basic impulse or opening windows on the player's client.
*/
bool AHullPatch::InterruptSkillOnAction(int Type)
{
	//Remove stealth effect
	m_Player->RemoveEffectRL(m_EffectID);

	//mark skill as not in use
	m_InUse = false;

	return true;
}
