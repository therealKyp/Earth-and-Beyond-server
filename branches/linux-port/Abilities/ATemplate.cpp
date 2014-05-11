#include "../Net7.h"
#include "../AbilityBase.h"
#include "ATemplate.h"
#include "../PlayerClass.h"
#include "../ServerManager.h"
#include "../ObjectManager.h"

class PlayerClass;

/*
Skill properties:
lvl 1:
lvl 2:
lvl 3:
lvl 4:
lvl 5:
lvl 6:
lvl 7:
*/

/*
* This calculates the activation cost of the skill.
*/
float ATemplate::CalculateEnergy ( float SkillLevel, long SkillRank )
{
	return SkillRank * 5.0f;
}

/*
* Calculate how much time must pass before the skill activates.
*
* Results are returned in seconds.
*/
float ATemplate::CalculateChargeUpTime ( float SkillLevel, long SkillRank )
{
	return SkillLevel < 6.0f ? 8.0f - SkillLevel : 3.0f;
}

/*
* Compute how much time must pass between skill uses.
*
* Results are returned in seconds.
*/
float ATemplate::CalculateCoolDownTime ( float SkillLevel, long SkillRank )
{
	return 0.0f;
}

/*
* Calculate the maximum range this rank of the skill can be used at.
*/
float ATemplate::CalculateRange ( float SkillLevel, long SkillRank )
{
	return 0.0f;
}

/*
* Computes the AoE per skill level for an ability.
*/
float ATemplate::CalculateAOE ( float SkillLevel, long SkillRank )
{
	return 1000.0f + 0.0f * SkillLevel;
}

/*
* Determine's which rank of the skill was used based on the SkillID.
*/
long ATemplate::DetermineSkillRank(int SkillID)
{
	/*switch(SkillID)
	{
	case :
		return 1;
	case :
		return 3;
	case :
		return 5;
	case :
		return 6;
	case ;
		return 7;
	default:
		return -1;
	}*/

	//REMOVE ME
	return -1;
}

// --------------------------------------------


bool ATemplate::CanUse(int TargetID, long AbilityID, long SkillID)
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

	//REMOVE ME
	return false;
}


/*
* Send confirmation to a player.
*/
void ATemplate::Confirmation(bool Confirm)
{
	//
}

/*
* If the call to Confirmation() succeeds, this is called.
*/
void ATemplate::Execute()
{
	//
}

bool ATemplate::InUse()
{
	return m_InUse;
}

/*
* This will be the first function called once the skill is determined
* as useable.
*/
bool ATemplate::Use(int TargetID)
{
	//grab a number for the effectID & timestamp
	m_EffectID = GetNet7TickCount();

	//REMOVE ME
	return false;
}

/*
* This function is called when the SetTimer call returns.
*/
void ATemplate::Update()
{
	//
}

/*
* Returns true in the case that this skill can be interrupted.
* What can interrupt the skill is returned inside the OnMotion
*  and OnDamage pointers.
*/
bool ATemplate::SkillInterruptable(bool* OnMotion, bool* OnDamage, bool* OnAction)
{
	//REMOVE ME
	return false;
}

/*
* Determines whether or not this skill was interrupted based on damage taken.
*/
bool ATemplate::InterruptSkillOnDamage(float Damage)
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
bool ATemplate::InterruptSkillOnMotion(float Speed)
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
bool ATemplate::InterruptSkillOnAction(int Type)
{
	//Remove stealth effect
	m_Player->RemoveEffectRL(m_EffectID);

	//mark skill as not in use
	m_InUse = false;

	return true;
}
