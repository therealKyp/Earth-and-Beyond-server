#include "..\net7.h"
#include "..\AbilityBase.h"
#include "AbilityRechargeShields.h"
#include "PlayerClass.h"
#include "ServerManager.h"
#include "ObjectManager.h"

class PlayerClass;

/*
Skill properties:
lvl 1: 50 energy, 90 shields per skill lvl, only useable on self
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
float ARechargeShields::CalculateEnergy ( float SkillLevel, long SkillRank )
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
		((1.0f + m_Player->m_Stats.GetStatType(STAT_SHIELD_RECHARGING_ECOST, STAT_BUFF_MULT)) * EnergyCost) +
		m_Player->m_Stats.GetStatType(STAT_SHIELD_RECHARGING_ECOST, STAT_BUFF_VALUE);

	EnergyCost < 0.0f ? 0.0f : EnergyCost;

	return EnergyCost;
}

/*
* Calculate how much time must pass before the skill activates.
*
* Results are returned in seconds.
*/
float ARechargeShields::CalculateChargeUpTime ( float SkillLevel, long SkillRank )
{
	//Yes David, right here I actually DO want their skill at shield recharge.
	float Skill = m_Player->m_Stats.GetStat(STAT_SKILL_SHIELD_RECHARGE) + SkillLevel;

	//cap skill level
	Skill = Skill > MAX_SKILL_LEVEL ? MAX_SKILL_LEVEL : Skill;
	
	//minus 1 second per lvl of skill above the rank
	float ChargeTime = 5.0f - (Skill - SkillRank);

	//ensure wierdness didn't happen
	ChargeTime = ChargeTime > 5.0f ? 5.0f : ChargeTime;

	//apply any direct bonuses to chargetime
	ChargeTime = 
		((1.0f - m_Player->m_Stats.GetStatType(STAT_SHIELD_RECHARGING, STAT_BUFF_MULT)) * ChargeTime) -
		m_Player->m_Stats.GetStatType(STAT_SHIELD_RECHARGING, STAT_BUFF_VALUE);

	//ensure charge time is still positive, or 0
	ChargeTime = ChargeTime > 0.0f ? ChargeTime : 0.0f;

	return ChargeTime;
}

/*
* Compute how much time must pass between skill uses.
*
* Results are returned in seconds.
*/
float ARechargeShields::CalculateCoolDownTime ( float SkillLevel, long SkillRank ) 
{
	return 0.0f;
}

/*
* Calculate the maximum range this rank of the skill can be used at.
*/
float ARechargeShields::CalculateRange ( float SkillLevel, long SkillRank ) 
{
	if(SkillRank < 3)
	{
		return 0.0f;
	}
	else
	{
		float Range = 3000 + (SkillLevel - 3);

		Range = 
			((1.0f + m_Player->m_Stats.GetStatType(STAT_SHIELD_RECHARGING_RANGE, STAT_BUFF_MULT)) * Range) +
			m_Player->m_Stats.GetStatType(STAT_SHIELD_RECHARGING_RANGE, STAT_BUFF_VALUE);
		return Range;
	}
}

/*
* Computes the AoE per skill level for an ability.
*/
float ARechargeShields::CalculateAOE ( float SkillLevel, long SkillRank )
{
	return 1500.0f + (1500.0f * (SkillLevel-6));
}

/*
* Returns the ammount of sheilds that have been recharged.
*/
float ARechargeShields::CalculateShieldCharge ( float SkillLevel, long SkillRank )
{
	//Yes David, right here I actually DO want their skill at shield recharge.
	float Skill = m_Player->m_Stats.GetStat(STAT_SKILL_SHIELD_RECHARGE) + SkillLevel;
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
float ARechargeShields::CalculateAOEShieldCharge ( float SkillLevel, long SkillRank )
{
	//Yes David, right here I actually DO want their skill at shield recharge.
	float Skill = m_Player->m_Stats.GetStat(STAT_SKILL_SHIELD_RECHARGE) + SkillLevel;
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
long ARechargeShields::DetermineSkillRank(int SkillID)
{
	switch(SkillID)
	{
	case REGENERATE_SHIELDS:
		return 1;
	case RECHARGE_SHIELDS:
		return 3;
	case COMBAT_RECHARGE_SHIELDS:
		return 5;
	case AREA_SHIELD_RECHARGE:
		return 6;
	case IMPROVED_AREA_RECHARGE:
		return 7;
	default:
		return -1;
	}
}

// --------------------------------------------


bool ARechargeShields::CanUse(int TargetID, long AbilityID, long SkillID)
{
	// Get the skill level
	m_SkillLevel = (float) m_Player->PlayerIndex()->RPGInfo.Skills.Skill[SkillID].GetLevel();
	m_SkillRank = DetermineSkillRank(AbilityID);
	m_AbilityID = AbilityID;
	m_SkillID = SkillID;
	ObjectManager *om = m_Player->GetObjectManager();

	//Skill does not exist.
	if(m_SkillRank == -1)
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
	if (TargetID > 0 && m_SkillRank >= 3 && om)
	{
		Object * Target = om->GetObjectFromID(TargetID);	// Get Target

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

		if(g_PlayerMgr->GetPlayer(TargetID)->ShipIndex()->GetIsIncapacitated())
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
void ARechargeShields::Confirmation(bool Confirm, long AbilityID, long GameID)
{
	//Not used
}

/*
* If the call to Confirmation() succeeds, this is called.
*/
void ARechargeShields::Execute()
{
	//Not used
}

/*
* This will be the first function called once the skill is determined
* as useable.
*/
bool ARechargeShields::Use(int TargetID)
{
	float ChargeTime;
	float EnergyCost;

	if(m_InUse)
	{
		InterruptSkillOnAction(WARPING);
		return false;
	}

	if(m_SkillRank >= 3)
	{
		// Target player
		if(TargetID < 0)
		{
			m_PlayerTarget = m_Player;
		}
		else
		{
			m_PlayerTarget = g_PlayerMgr->GetPlayer(TargetID);
		}

		// If we cant find player return
		if (!m_PlayerTarget)
		{
			m_Player->SetCurrentSkill();
			m_DamageTaken = 0;
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
		m_Player->SetCurrentSkill();
		m_InUse = false;
		m_DamageTaken = 0;
		return false;
	}

	m_Player->RemoveEnergy(EnergyCost);

	ChargeTime = CalculateChargeUpTime(m_SkillLevel, m_SkillRank);

	//charge up, apply shield charge after charge is finished.
	SetTimer((long)ChargeTime*1000);

	ObjectEffect RechargeChargeEffect;

	memset(&RechargeChargeEffect, 0, sizeof(RechargeChargeEffect));		// Zero out memory

	RechargeChargeEffect.Bitmask = 3;
	RechargeChargeEffect.GameID = m_Player->GameID();
	RechargeChargeEffect.TimeStamp = m_EffectID;
	RechargeChargeEffect.EffectID = m_EffectID;
	RechargeChargeEffect.Duration = (short)ChargeTime;
	RechargeChargeEffect.EffectDescID = 733;
	m_Player->SendObjectEffectRL(&RechargeChargeEffect);

	return true;
}

/*
* This function is called when the SetTimer call returns.
*/
void ARechargeShields::Update()
{
	if(!m_InUse)
	{
		m_Player->SetCurrentSkill();
		m_DamageTaken = 0;
		return;
	}

	m_Player->RemoveEffectRL(m_EffectID);

	m_EffectID = GetNet7TickCount();

	float ChargeAmount = CalculateShieldCharge(m_SkillLevel , m_SkillRank);
	float ChargePercent = 0;

	if(m_SkillRank < 3)
	{
		 // Make sure we dont over fill your shields
        if (ChargeAmount + m_Player->GetShieldValue() > m_Player->ShipIndex()->GetMaxShield())
        {
			ChargePercent = 1; // Set it at full
        }
        else
        {
			ChargePercent = m_Player->GetShield() + (ChargeAmount / m_Player->ShipIndex()->GetMaxShield());
        }

		m_Player->ShipIndex()->Shield.SetStartValue(ChargePercent);
	}
	else
	{
		if (!m_PlayerTarget)
		{
			m_PlayerTarget = m_Player;	//not sure if this will work as expected, but if there's no target, 
										//the target should be the player
		}
		// Make sure we dont over fill your shields
		if (ChargeAmount + m_PlayerTarget->GetShieldValue() > m_PlayerTarget->ShipIndex()->GetMaxShield())
		{
			ChargePercent = 1;		// Set it at full
		}
		else
		{
			ChargePercent = m_PlayerTarget->GetShield() + (ChargeAmount / m_PlayerTarget->ShipIndex()->GetMaxShield());
		}

		// Update shield
		m_PlayerTarget->ShipIndex()->Shield.SetStartValue(ChargePercent);

		// Send Shield update/Energy update
		m_PlayerTarget->SendAuxShip();
	}

	// Update our energy
	m_Player->SendAuxShip();

	if(m_SkillRank < 3)
	{
		ObjectEffect RechargeEffect;

		memset(&RechargeEffect, 0, sizeof(RechargeEffect));		// Zero out memory

		RechargeEffect.Bitmask = 3;
		RechargeEffect.GameID = m_Player->GameID();
		RechargeEffect.TimeStamp = m_EffectID;
		RechargeEffect.EffectID = m_EffectID;
		RechargeEffect.Duration = 1000;
		RechargeEffect.EffectDescID = 136;
		m_Player->SendObjectEffectRL(&RechargeEffect);
		m_Player->RemoveEffectRL(m_EffectID);
	}
	else
	{
		//beam to target ship
		ObjectToObjectEffect RechargeBeamEffect;

		memset(&RechargeBeamEffect, 0, sizeof(RechargeBeamEffect));		// Zero out memory

		RechargeBeamEffect.Bitmask = 3;
		RechargeBeamEffect.GameID = m_Player->GameID();
		RechargeBeamEffect.TimeStamp = m_EffectID+1;
		RechargeBeamEffect.EffectID = m_EffectID+1;
		RechargeBeamEffect.Duration = 1000;
		RechargeBeamEffect.EffectDescID = 139;
		RechargeBeamEffect.TargetID = m_PlayerTarget->GameID();
		m_Player->SendObjectToObjectEffectRL(&RechargeBeamEffect);
		m_Player->RemoveEffectRL(m_EffectID+1);

		//recharge orb around target ship.
		ObjectToObjectEffect RechargeEffect;

		memset(&RechargeEffect, 0, sizeof(RechargeEffect));		// Zero out memory

		RechargeEffect.Bitmask = 3;
		RechargeEffect.GameID = m_Player->GameID();
		RechargeEffect.TimeStamp = m_EffectID;
		RechargeEffect.EffectID = m_EffectID;
		RechargeEffect.Duration = 1000;
		RechargeEffect.EffectDescID = 166;
		RechargeEffect.TargetID = m_PlayerTarget->GameID();
		m_Player->SendObjectToObjectEffectRL(&RechargeEffect);
		m_Player->RemoveEffectRL(m_EffectID);
	}

	// Allow skill to cool down
	m_InUse = false;
	m_Player->SetCurrentSkill();
	m_DamageTaken = 0;
}

/*
* Returns true in the case that this skill can be interrupted.
* What can interrupt the skill is returned inside the OnMotion 
*  and OnDamage pointers.
*/
bool ARechargeShields::SkillInterruptable(bool* OnMotion, bool* OnDamage, bool* OnAction)
{
	*OnMotion = false;
	*OnDamage = true;
	*OnAction = true;
	return true;
}

/*
* Determines whether or not this skill was interrupted based on damage taken.
*/
bool ARechargeShields::InterruptSkillOnDamage(float Damage)
{
	ObjectEffect InterruptEffect;

	if(!m_InUse) //cannot interrupt non-active skill.
	{
		m_Player->SetCurrentSkill();
		m_DamageTaken = 0;
		return false;
	}

	if(Damage <= 0.0f)
	{
		m_Player->SetCurrentSkill();
		return false;
	}

	m_DamageTaken += Damage;

	if(m_DamageTaken > GetInterruptThreshHold())
	{
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
		m_Player->SetCurrentSkill();
		m_DamageTaken = 0;

		return true;
	}

	return false;
}

/*
* Determines whether or not this skill was interrupted based on current motion.
*/
bool ARechargeShields::InterruptSkillOnMotion(float Speed)
{
	return false; //not used
}

/*
* Returns true if this skill was interrupted based on player action.
*
* Action = Shooting guns, using devices or activated effects, starting to warp,
*  anything except basic impulse or opening windows on the player's client.
*/
bool ARechargeShields::InterruptSkillOnAction(int Type)
{
	if(!m_InUse)
	{
		m_Player->SetCurrentSkill();
		m_DamageTaken = 0;
		return false;
	}

	if(Type == WARPING)
	{
		//Remove effect
		m_Player->RemoveEffectRL(m_EffectID);

		//mark skill as not in use
		m_InUse = false;
		m_Player->SetCurrentSkill();
		m_DamageTaken = 0;
		return true;
	}

	return false;
}
