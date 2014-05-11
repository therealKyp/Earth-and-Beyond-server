#include "..\net7.h"
#include "..\AbilityBase.h"
#include "AbilityShieldSap.h"
#include "PlayerClass.h"
#include "ServerManager.h"
#include "ObjectManager.h"
#include "MOBClass.h"

class PlayerClass;

//**************************************************************
//NOTE!!! use m_Player->SetCurrentSkill(); to flag the skill as unused when you are m_InUse is false.
//  m_Player->SetCurrentSkill(); is set just before the Use() function is called, be sure to clear it 
//  in any code after that function call.
//**************************************************************
/*
Skill properties:
lvl 1: Drains up to 15% of target's sheilds or up to 50% of the user's sheilds (15%{target} > 50%{user} ? 50% : 15%;)
		from the target. Range 1K. Energy cost: 35 (rank 1)
lvl 2: Drains up to 18% (as above). Range 1.25K.
lvl 3: Drains up to 21% (as in lvl 1) and transfers 75% of the energy drained to the user's shields. Range: 1.5K. 
		Energy Cost: 75 units (rank 3)
lvl 4: Drains up to 24% (as in lvl 1) and transfers 87.5% of energy drained to the user's shields. Range: 1.75K
lvl 5: Drains up to 27% (as in lvl 1) and either:
		a. Transfers 100% of the energy drained to the user
		b. Transfers 100% / # of group members energy to each member of the group's shields.
		Range: 2.00K. Energy Cost: 150 (rank 5)
lvl 6: Drains up to 30% (as in lvl 1) from all targets within 1.5K of the target and either:
		a. Transfers 112.5% of the energy drained to the user
		b. Transfers 112.5% / # of group members energy to each member of the group's shields.
		Range: 2.25K. Energy Cost: 300 (rank 6)
lvl 7: Drains up to 33% (as in lvl 1) from all targets within 3.0K of the target and either:
		a. Transfers 125% of the energy drained to the user
		b. Transfers 125% / # of group members energy to each member of the group's shields.
		Range: 2.50K. Energy Cost: 600 (rank 7)
*/

/*
* This calculates the activation cost of the skill.
*/
float AShieldSap::CalculateEnergy ( float SkillLevel, long SkillRank )
{
	switch(SkillRank)
	{
	case 1:
		return 35;
		break;
	case 3:
		return 75;
		break;
	case 5:
		return 150;
		break;
	case 6:
		return 300;
		break;
	case 7:
		return 600;
		break;
	default:
		return -1;
		break;
	}
}

/*
* Calculate how much time must pass before the skill activates.
*
* Results are returned in seconds.
*/
float AShieldSap::CalculateChargeUpTime ( float SkillLevel, long SkillRank )
{
	return 3.0f;
}

/*
* Compute how much time must pass between skill uses.
*
* Results are returned in seconds.
*/
float AShieldSap::CalculateCoolDownTime ( float SkillLevel, long SkillRank ) 
{
	return 0.0f;
}

/*
* Calculate the maximum range this rank of the skill can be used at.
*/
float AShieldSap::CalculateRange ( float SkillLevel, long SkillRank ) 
{
	return (float)(SkillRank-1) * 250.0f + 1000.0f; 
}

/*
* Computes the AoE per skill level for an ability.
*/
float AShieldSap::CalculateAOE ( float SkillLevel, long SkillRank )
{
	return SkillRank >= 6 ? (float)(SkillRank-5) * 1500.0f : 0.0f;
}

/*
* Determine's which rank of the skill was used based on the SkillID.
*/
long AShieldSap::DetermineSkillRank(int SkillID)
{
	switch(SkillID)
	{
	case SHIELD_SAP:
		return 1;
	case SHIELD_TRANSFER:
		return 3;
	case GROUP_SAP:
		return 5;
	case SAPPING_SPHERE:
		return 6;
	case GROUP_SAPPING_SPHERE:
		return 7;
	default:
		return -1;
	}
}

float AShieldSap::CalculateShieldDrainPercent(float SkillLevel, long SkillRank)
{
	return SkillLevel * 0.03f + 0.12f;
}

float AShieldSap::CalculateShieldRestorePercent(float SkillLevel, long SkillRank)
{
	return (SkillLevel-3.0f) * 0.125f + 0.75f;
}

/*
* Computes what percent of a player's shield should be restored. If the player is in a group,
*  this function will (BUT CURRENTLY DOES NOT) restore thier shield HP directly. If the player
*  is alone, it will compute what percent of their shields should be restored.
*
* It should be noted that in the event of group healing, no group member will recieve more than 
*  50% of the SKILL USER'S shield hitpoints as restored sheild points. For example, if a PW with
*  100K shields is grouped with a JD @ 0 shields (max 50K), and casts this skill, getting a hit 
*  which heals the PW for 50% of his max shield value, the PW will get at most 50K shields 
*  (a 50% increase). However, the JD will also get up to 50K shield points (a 100% shield restore
*  for him).
*/
float AShieldSap::CalculateShieldRestoreValue(float SkillLevel, long SkillRank, float shields_drained)
{
	float player_shield;

	shields_drained = shields_drained * CalculateShieldRestorePercent(m_SkillLevel, m_SkillRank);

	//TODO: if player in group, heal group & player
	//if(SkillRank >= 5 && m_Player->GroupID != -1)
	//{
		//for each player in group
			//restore sheilds_drained / number_of_group_members shields to each player
			//ensure that no player gets more than 50% of the skill user's shields points restored.
			//ensure no group member gets more than 100% of their shield value.
	//}
	//else
	//{
	player_shield = m_Player->GetShieldValue() + shields_drained;

	if(player_shield < m_Player->ShipIndex()->GetMaxShield())
	{
		return player_shield / m_Player->ShipIndex()->GetMaxShield();
	}
	else
	{
		return 1.0f;
	}
	//}
}

/*
* Computes how much shield energy should be drained from the target and
*  then proceeds to drain the energy.
* Returns how much shielding was drained.
*/
float AShieldSap::DrainShields()
{
	float max_drain = m_Player->ShipIndex()->GetMaxShield() / 2.0f;
	float drain_amount = 0;

	if(m_TargetMob)
	{
		drain_amount = m_TargetMob->GetShieldLevel() * 
			CalculateShieldDrainPercent(m_SkillLevel, m_SkillRank);

		if(drain_amount > m_Player->ShipIndex()->GetMaxShield() / 2.0f)
		{
			drain_amount = m_Player->ShipIndex()->GetMaxShield() / 2.0f;
		}

		if(m_TargetMob->GetShieldLevel() - drain_amount < 0)
		{
			drain_amount = m_TargetMob->GetShieldLevel();
		}

		//perform drain (0 in last parameter means Not a crit)
		m_TargetMob->DamageMOB(m_Player->GameID(), DAMAGE_ENERGY, drain_amount, 0);
	}

	return drain_amount;
}

/*
* Computes how much shield energy should be drained from the target and
*  then proceeds to drain the energy. ***This function is ONLY for the AoE
*  effect targets, NOT the primary target.
* Returns how much shielding was drained.
*/
float AShieldSap::DrainShieldsAOE()
{
	//
	float amount_drained = 0;

	return amount_drained;
}

// --------------------------------------------


bool AShieldSap::CanUse(int TargetID, long AbilityID, long SkillID)
{
	// Get the skill level
	m_SkillLevel = (float) m_Player->PlayerIndex()->RPGInfo.Skills.Skill[SkillID].GetLevel();
	m_SkillRank = DetermineSkillRank(AbilityID);
	m_AbilityID = AbilityID;
	m_SkillID = SkillID;
	ObjectManager *om = m_Player->GetObjectManager();

	if(m_SkillRank > m_SkillLevel)
	{
		return false;
	}

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
	if (TargetID > 0 && om)
	{
		Object * Target = om->GetObjectFromID(TargetID);	// Get Target

		if (!Target || (Target->ObjectType() != OT_MOB && Target->ObjectType() != OT_MOBSPAWN))
		{
			SendError("Incorrect target type!");
			return false;
		}

		// See if we are in range
		if (Target && Target->RangeFrom(m_Player->Position()) > CalculateRange(m_SkillLevel, m_SkillRank))
		{
			SendError("Out of range!");
			return false;
		}

		m_TargetMob = (MOB*) Target;
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
void AShieldSap::Confirmation(bool Confirm)
{
	//not needed for this skill
}

/*
* If the call to Confirmation() succeeds, this is called.
*/
void AShieldSap::Execute()
{
	//not needed for this skill
}

/*
* This will be the first function called once the skill is determined
* as useable.
*/
bool AShieldSap::Use(int TargetID)
{
	float EnergyCost;
	float ChargeTime;

	//allow the ability to be toggled off.
	if(m_InUse)
	{
		InterruptSkillOnAction(OTHER);
		return false;
	}

	//grab a number for the effectID & timestamp
	m_EffectID = GetNet7TickCount();

	// If we cant find player return
	if (!m_TargetMob)
	{
		m_Player->SetCurrentSkill();
		m_DamageTaken = 0;
		return false;
	}

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

	ObjectEffect SapEffect;

	memset(&SapEffect, 0, sizeof(SapEffect));		// Zero out memory

	SapEffect.Bitmask = 3;
	SapEffect.GameID = m_Player->GameID();
	SapEffect.TimeStamp = m_EffectID;
	SapEffect.EffectID = m_EffectID;
	SapEffect.Duration = (short)ChargeTime;
	SapEffect.EffectDescID = 733;
	m_Player->SendObjectEffectRL(&SapEffect);

	return true;
}

/*
* This function is called when the SetTimer call returns.
*/
void AShieldSap::Update()
{
	float ChargePercent;
	float DrainAmount;

	if(!m_InUse)
	{
		m_Player->SetCurrentSkill();
		m_DamageTaken = 0;
		return;
	}

	m_Player->RemoveEffectRL(m_EffectID);

	m_EffectID = GetNet7TickCount();

	DrainAmount = DrainShields();
	
	if(m_SkillRank > 1)
	{
		ChargePercent = CalculateShieldRestoreValue(m_SkillLevel, m_SkillRank, DrainAmount);

		m_Player->ShipIndex()->Shield.SetStartValue(ChargePercent);
	}
	m_Player->SendAuxShip();

	//beam to target ship
	ObjectToObjectEffect RechargeBeamEffect;

	memset(&RechargeBeamEffect, 0, sizeof(RechargeBeamEffect));		// Zero out memory

	RechargeBeamEffect.Bitmask = 3;
	RechargeBeamEffect.GameID = m_Player->GameID();
	RechargeBeamEffect.TimeStamp = m_EffectID+1;
	RechargeBeamEffect.EffectID = m_EffectID+1;
	RechargeBeamEffect.Duration = 1000;
	RechargeBeamEffect.EffectDescID = 139;
	RechargeBeamEffect.TargetID = m_TargetMob->GameID();
	m_Player->SendObjectToObjectEffectRL(&RechargeBeamEffect);
	m_Player->RemoveEffectRL(m_EffectID+1);

	//recharge orb around target ship.
	ObjectToObjectEffect StealEffect;

	memset(&StealEffect, 0, sizeof(StealEffect));		// Zero out memory

	StealEffect.Bitmask = 3;
	StealEffect.GameID = m_Player->GameID();
	StealEffect.TimeStamp = m_EffectID;
	StealEffect.EffectID = m_EffectID;
	StealEffect.Duration = 1000;
	StealEffect.EffectDescID = 146;
	StealEffect.TargetID = m_TargetMob->GameID();
	m_Player->SendObjectToObjectEffectRL(&StealEffect);
	m_Player->RemoveEffectRL(m_EffectID);

	m_InUse = false;
	m_Player->SetCurrentSkill();
	m_DamageTaken = 0.0f;
}

/*
* Returns true in the case that this skill can be interrupted.
* What can interrupt the skill is returned inside the OnMotion 
*  and OnDamage pointers.
*/
bool AShieldSap::SkillInterruptable(bool* OnMotion, bool* OnDamage, bool* OnAction)
{
	*OnMotion = false;
	*OnDamage = true;
	*OnAction = true;

	return true;
}

/*
* Determines whether or not this skill was interrupted based on damage taken.
*/
bool AShieldSap::InterruptSkillOnDamage(float Damage)
{
	ObjectEffect InterruptEffect;

	if(!m_InUse) //cannot interrupt non-active skill.
	{
		m_Player->SetCurrentSkill();
		return false;
	}

	if(Damage <= 0.0f)
	{
		Damage = 0.0f;
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

		m_Player->RemoveEffectRL(m_EffectID);
		m_Player->SetCurrentSkill();
		m_InUse = false;
		m_DamageTaken = 0;
		return true;
	}

	return false;
}

/*
* Determines whether or not this skill was interrupted based on current motion.
*
* Note: This refers to impulse motion. Warp is handled by OnAction.
*/
bool AShieldSap::InterruptSkillOnMotion(float Speed)
{
	return false;
}

/*
* Returns true if this skill was interrupted based on player action.
*
* Action = Shooting guns, using devices or activated effects, starting to warp,
*  anything except basic impulse or opening windows on the player's client.
*/
bool AShieldSap::InterruptSkillOnAction(int Type)
{
	if(!m_InUse)
	{
		m_Player->SetCurrentSkill();
		m_DamageTaken = 0;
		return false;
	}

	if(Type == SHOOTING)
	{
		return false;
	}

	//Remove skill effect
	m_Player->RemoveEffectRL(m_EffectID);

	//clear current skill pointer
	m_Player->SetCurrentSkill();

	//mark skill as not in use
	m_InUse = false;
	m_DamageTaken = 0;

	return true;
}
