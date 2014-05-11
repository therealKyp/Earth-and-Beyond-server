#include "../Net7.h"
#include "../AbilityBase.h"
#include "AbilityJumpStart.h"
#include "../PlayerClass.h"
#include "../ServerManager.h"
#include "../ObjectManager.h"

class PlayerClass;

/*
Skill properties:
lvl 1: 7%, 1K
lvl 2: 14%, 1.25K
lvl 3: 21%, 1.5K
lvl 4: 28%, 1.75K
lvl 5: 35%, 2K
lvl 6: 42%, 2.25K
lvl 7: 49%, 2.5K
*/

/*
* This calculates the activation cost of the skill.
*/
float AJumpStart::CalculateEnergy ( float SkillLevel, long SkillRank )
{
	float EnergyCost = 20.0f * SkillLevel;

	//TO-DO: implement buffs
	//EnergyCost =
	//	((1.0f - m_Player->m_Stats.GetStatType(, STAT_BUFF_MULT)) * EnergyCost) -
	//	m_Player->m_Stats.GetStatType(, STAT_BUFF_VALUE);

	//EnergyCost < 0.0f ? 0.0f : EnergyCost;

	return EnergyCost;
}

/*
* Calculate how much time must pass before the skill activates.
*
* Results are returned in seconds.
*/
float AJumpStart::CalculateChargeUpTime ( float SkillLevel, long SkillRank )
{
	float ChargeTime = 7.0f;

	//TO-DO: implement buffs
	//ChargeTime =
	//	((1.0f - m_Player->m_Stats.GetStatType(, STAT_BUFF_MULT)) * ChargeTime) -
	//	m_Player->m_Stats.GetStatType(, STAT_BUFF_VALUE);

	//ChargeTime < 0.0f ? 0.0f : ChargeTime;

	return ChargeTime;
}

/*
* Compute how much time must pass between skill uses.
*
* Results are returned in seconds.
*/
float AJumpStart::CalculateCoolDownTime ( float SkillLevel, long SkillRank )
{
	return 0.0f;
}

/*
* Calculate the maximum range this rank of the skill can be used at.
*/
float AJumpStart::CalculateRange ( float SkillLevel, long SkillRank )
{
	//TO-DO: implement buffs
	return 1000.0f + 250.0f * SkillLevel;
}

float AJumpStart::CalculateHullRepair ( float SkillLevel )
{
	return 10 * SkillLevel;
}

/*
* Computes the AoE per skill level for an ability.
*/
float AJumpStart::CalculateAOE ( float SkillLevel, long SkillRank )
{
	return 0.0f;
}

/*
* Determine's which rank of the skill was used based on the SkillID.
*/
long AJumpStart::DetermineSkillRank(int SkillID)
{
	return -1; //not used
}

// --------------------------------------------


bool AJumpStart::CanUse(int TargetID, long AbilityID, long SkillID)
{
	// Get the skill level
	m_SkillLevel = (float) m_Player->PlayerIndex()->RPGInfo.Skills.Skill[SkillID].GetLevel();
	m_SkillRank = DetermineSkillRank(AbilityID);
	m_AbilityID = AbilityID;
	m_SkillID = SkillID;

	// Make sure skill is not in use at this time
	if (m_InUse)
	{
		return false;
	}

	// See if we can use this skill
	if (TargetID > 0)
	{
		Object * Target = m_Player->GetObjectManager()->GetObjectFromID(TargetID);	// Get Target

		if (Target && Target->ObjectType() != m_TargetType)
		{
			SendError("Incorrect target type!");
			return false;
		}

		// See if we have a skill level and we are in range
		if (Target && Target->RangeFrom(m_Player->Position()) > CalculateRange(m_SkillLevel, m_SkillRank))
		{
			SendError("Out of range!");
			return false;
		}
	}

	return true;
}

/*
* Send confirmation to a player.
*/
void AJumpStart::Confirmation(bool Confirm)
{
	if (Confirm)
	{
		// Jumpstart was accepted
		Execute();
	}
	else
	{
		// Jumpstart was not accepted
		m_Player->SendVaMessage("Jump start was declined");
		m_InUse = false;
		return;
	}
}

/*
* If the call to Confirmation() succeeds, this is called.
*/
void AJumpStart::Execute()
{
	// Execute actual jumpstart
	ObjectManager *om = m_Player->GetObjectManager();
	Object * Target = (0);
	if (om) Target = om->GetObjectFromID(m_TargetID);	// Get Target
	Player * TargetP = g_GlobMemMgr->GetPlayer(m_TargetID);

	unsigned long myTime = GetNet7TickCount();

	m_InUse = true;

	if (Target && TargetP && Target->ObjectType() == OT_PLAYER && TargetP->ShipIndex()->GetIsIncapacitated())
	{
		// Store this to use for the next part
		int HullLevel = TargetP->PlayerIndex()->RPGInfo.GetHullUpgradeLevel();
		int EnergyUsed = (int) CalculateEnergy((float) HullLevel, m_SkillRank);

		if (HullLevel > m_SkillLevel + 2)		// need more skill
		{
			m_Player->SendVaMessage("Jump Start Failed! You need more skill");
			TargetP->SendVaMessage("Jump Start Failed! Skill level too low");
			m_InUse = false;
			return;
		}

		if (HullLevel > m_SkillLevel)
		{
			EnergyUsed = (int) EnergyUsed * (int) (HullLevel - m_SkillLevel);
		}

		if (m_Player->GetEnergyValue() < EnergyUsed)	// not enough energy
		{
			m_InUse = false;
			return;
		}

		m_Player->RemoveEnergy((float) EnergyUsed);

		m_Player->SendAuxShip();									// Update Energy

		/* Activate effect */
		ObjectToObjectEffect OBTOBE;

		OBTOBE.Bitmask = 0x07;
		OBTOBE.GameID = m_Player->GameID();
		OBTOBE.TargetID = m_TargetID;

		if (m_TargetID == m_Player->GameID())
        {
			OBTOBE.EffectDescID = 411;									// Jumpstart Self
        }
		else
        {
			OBTOBE.EffectDescID = 414;									// Jumpstart Others Effect
        }

		OBTOBE.Message = 0;
		OBTOBE.EffectID = myTime;
		OBTOBE.TimeStamp = myTime;
		OBTOBE.Duration = 500;

		m_Player->SendObjectToObjectEffectRL(&OBTOBE);

		// Save EffectID to remove it later!
		m_EffectID = myTime;

		// Add timed call back
		SetTimer(5000);
	}
}



bool AJumpStart::Use(int TargetID)
{
	Player * p;

	if(TargetID > 0)
	{
		p = g_GlobMemMgr->GetPlayer(TargetID);
	}
	else
	{
		p = m_Player;
	}

	if (p)
	{
		m_TargetID = TargetID;
	}

	// Send a message to other player
	// Target must be a player
	if (p && p != m_Player)
	{
		// See if player is busy
		if (p->ConfirmationBusy())
		{
			m_Player->SendVaMessage("Player is busy try again!");
			return false;
		}

		char msg[512];
		// Create a message
		_snprintf(msg, 512, "Do you want to accept a jumpstart level %d from %s", m_Player->PlayerIndex()->RPGInfo.Skills.Skill[m_SkillID].GetLevel(), m_Player->Name());
		// Send a message & wait for a reply
		p->SendConfirmation(msg, m_Player->GameID(), m_AbilityID);
		return true;
	}
	else
	{
		// Jumpstart ourself
		Execute();
		return true;
	}

	return false;
}
void AJumpStart::Update()
{
	// Updated by timer
	Player * TargetPlayer = g_GlobMemMgr->GetPlayer(m_TargetID);

	// Make sure we can find target
	if (!TargetPlayer)
	{
		return;
	}

	m_InUse = false;

	float HullPoints = (((float)CalculateHullRepair(m_SkillLevel))/100.0f) * TargetPlayer->ShipIndex()->GetMaxHullPoints();

	if (HullPoints > TargetPlayer->ShipIndex()->GetMaxHullPoints())
    {
		HullPoints = TargetPlayer->ShipIndex()->GetMaxHullPoints();
    }

	/* Remove effect */
	m_Player->RemoveEffectRL(m_EffectID);

	TargetPlayer->ShipIndex()->Shield.SetStartValue(0);				// Set shields at 0%
	TargetPlayer->ShipIndex()->Shield.SetChangePerTick(0);			// Set shields at 0%
	TargetPlayer->ShipIndex()->SetHullPoints(HullPoints);			// Set hull points
	TargetPlayer->RemobilisePlayer();								// Allow player to move
	TargetPlayer->ShipIndex()->SetIsIncapacitated(false);			// Tell us that he is now alive
	TargetPlayer->ShipIndex()->SetIsRescueBeaconActive(false);		// Turn off
	TargetPlayer->RechargeReactor();								// Start regening
	TargetPlayer->RechargeShield();
    TargetPlayer->SendAuxShip();

}

/*
* Returns true in the case that this skill can be interrupted.
* What can interrupt the skill is returned inside the OnMotion
*  and OnDamage pointers.
*/
bool AJumpStart::SkillInterruptable(bool* OnMotion, bool* OnDamage, bool* OnAction)
{
	//REMOVE ME
	return false;
}

/*
* Determines whether or not this skill was interrupted based on damage taken.
*/
bool AJumpStart::InterruptSkillOnDamage(float Damage)
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
bool AJumpStart::InterruptSkillOnMotion(float Speed)
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
bool AJumpStart::InterruptSkillOnAction(int Type)
{
	//Remove stealth effect
	m_Player->RemoveEffectRL(m_EffectID);

	//mark skill as not in use
	m_InUse = false;

	return true;
}
