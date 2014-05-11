#include <math.h>
#include "../Net7.h"
#include "../AbilityBase.h"
#include "AbilityCloak.h"
#include "../PlayerClass.h"
#include "../ServerManager.h"
#include "../ObjectManager.h"

class PlayerClass;

/*
lvl 1: 7 second init, .5 drain per second, must be stopped to cloak, 1/2 speed once cloaked
lvl 2: 6 second init, .5 drain
lvl 3: 5 second init, 1.5 drain, must be stopped to cloak, full speed while cloaked
lvl 4: 4 second init
lvl 5: 3 second init, 3 drain, ship may move to cloak, full speed, 2x unmodified beam damage for 5 seconds after uncloaking
lvl 6: 3 second init, 6 drain, cloaks ship + all ships within 1K. May warp while cloaked, reduces ship sig for group members?
lvl 7: 3 second init, 10 drain, all ships fully cloaked in 1K, may warp.
*/

/*
* This calculates the activation cost of the skill.
* TO-DO: Balance this value
*/
float ACloak::CalculateEnergy ( float SkillLevel, long SkillRank )
{
	return SkillRank * 5.0f;
}

float ACloak::CalculateEnergyPerSecond (long SkillRank )
{
	switch(SkillRank)
	{
	case 1:
		return 0.5f;
	case 3:
		return 1.5f;
	case 5:
		return 3.0f;
	case 6:
		return 6.0f;
	case 7:
		return 10.0f;
	default:
		return -1;
	}
}

float ACloak::CalculateChargeUpTime ( float SkillLevel, long SkillRank )
{
	return SkillLevel < 6.0f ? 8.0f - SkillLevel : 3.0f;
}

float ACloak::CalculateCoolDownTime ( float SkillLevel, long SkillRank )
{
	return 0.0f;
}

/*
* Cloak effects the player only, has no range.
*/
float ACloak::CalculateRange ( float SkillLevel, long SkillRank )
{
	return 0.0f;
}

/*
* Levels 6 & 7 of cloak have a range over which they effect group members. This
* computes that range.
*/
float ACloak::CalculateAOE ( float SkillLevel, long SkillRank )
{					 //CHANGE? Do we want to add increased AoE range per skill level?
	return 1000.0f + 0.0f * SkillLevel;
}

/*
* Stealth detection is based on your overall level + 5 times your skill at cloak.
* So, a lvl 150 JD with lvl 5 cloak will have an effective, unmodified, cloak skill of
* 175. Therefore, any mob with Clvl 55 or lower will be unable to see a cloaked player.
* Scan skill will increase the mob's ability to see through stealth above the MOB's total
* level. Checks against this number will be simple: if(MOBSeeCloakedSkill > CloakSkill)
*  then SeeThisPlayer = false;
*/
float ACloak::CalculateStealthLevel ( long SkillLevel )
{
	return m_Player->TotalLevel() + (5.0f * SkillLevel);
}

/*
* Determine's which rank of the skill was used based on the SkillID.
*/
long ACloak::DetermineSkillRank(int SkillID)
{
	switch(SkillID)
	{
	case CLOAK:
		return 1;
	case ADVANCED_CLOAK:
		return 3;
	case COMBAT_CLOAK:
		return 5;
	case GROUP_STEALTH:
		return 6;
	case GROUP_CLOAK:
		return 7;
	default:
		return -1;
	}
}

/*
* By default this enables half of max speed for Rank 1 cloak.
* Pass false in to enable full speed again.
*/
void ACloak::EnableHalfSpeed(bool answer)
{
	float NewSpeed;

	if(answer)
	{
		m_OldShipSpeed = m_Player->m_Stats.GetStat(STAT_IMPULSE);
		NewSpeed = m_Player->m_Stats.GetStat(STAT_IMPULSE)/2.0f;
		m_Player->ShipIndex()->CurrentStats.SetSpeed((s32)NewSpeed);
	}
	else
	{
		NewSpeed = m_Player->m_Stats.GetStat(STAT_IMPULSE);
		m_Player->ShipIndex()->CurrentStats.SetSpeed((s32)m_OldShipSpeed);
	}

	//Send updated speed to the client.
	m_Player->ShipIndex()->SetMaxSpeed(NewSpeed);
	m_Player->SendAuxShip();
}

/*
* By default this enables the 2x bonus to beam damage for rank 5 cloak.
* Pass false in to disable the bonus.
*/
void ACloak::EnableDoubleDamage(bool answer)
{
	if(answer)
	{
		//prevent 2x dmg stacking
		if(!m_DoubleDamageActive)
		{
			if(m_BuffID == -1)
			{
				m_BuffID = m_Player->m_Stats.SetStat(STAT_BUFF_MULT, STAT_BEAM_DAMAGE, 1.0f, "SKILL_COMBAT_CLOAK");
			}
			else
			{
				m_Player->m_Stats.ChangeStat(m_BuffID, 1.0f);
			}
		}
		m_DoubleDamageActive = true;
	}
	else
	{
		if(m_BuffID == -1)
			{
				m_BuffID = m_Player->m_Stats.SetStat(STAT_BUFF_MULT, STAT_BEAM_DAMAGE, 0.0f, "SKILL_COMBAT_CLOAK");
			}
			else
			{
				m_Player->m_Stats.ChangeStat(m_BuffID, 0.0f);
			}

		m_DoubleDamageActive = false;
	}
}

// --------------------------------------------


bool ACloak::CanUse(int TargetID, long AbilityID, long SkillID)
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

	if(m_Player->Prospecting())
	{
		return false;
	}
	if(m_Player->Looting())
	{
		return false;
	}
	if(m_Player->WarpDrive() && m_SkillRank < 6)
	{
		return false;
	}

	//are we moving?
	if(m_SkillRank < 5 && m_Player->ObjectIsMoving() && !m_InUse)
	{
		SendError("Must be stopped to use this rank of the skill.");
		return false;
	}

	// Make sure we are not dead
	if (m_Player->ShipIndex()->GetIsIncapacitated())
	{
		SendError("Can not use this ability while dead!");
		return false;
	}

	return true;
}


/*
* Send confirmation to a player.
*/
void ACloak::Confirmation(bool Confirm)
{
	//No confirmation needed
}

/*
* If the call to Confirmation() succeeds, this is called.
*/
void ACloak::Execute()
{
	//Not used for this ability
}

/*
* Cloak is a toggle ability. Keep that in mind while reading though this
*  function call. This function must deal with turning the skill on and off.
*/
bool ACloak::Use(int TargetID)
{
	//Compute cloak time
	float CloakTime = CalculateChargeUpTime(m_SkillLevel, m_SkillRank); //in seconds
	ObjectEffect CloakEffect;

	//grab a number for the effectID & timestamp
	if(!m_InUse)
	{
		m_EffectID = GetNet7TickCount();
	}

	//cap cloak at 3 seconds
	CloakTime = CloakTime < 3.0f ? 3.0f : CloakTime;;

	//TO-DO: Add in debuff effects here
	CloakTime = CloakTime *
		( (1.0f - m_Player->m_Stats.GetStatType(STAT_CLOAK, STAT_BUFF_MULT)) ) +
		( m_Player->m_Stats.GetStatType(STAT_CLOAK, STAT_BUFF_VALUE) );

	//subtract reactor energy
	if(!m_InUse)
	{
		if(m_Player->GetEnergyValue() <= CalculateEnergy(m_SkillLevel, m_SkillRank))
		{
			return false;
		}
		m_Player->RemoveEnergy(CalculateEnergy(m_SkillLevel, m_SkillRank));
	}//else: Do nothing.

	//mark skill as in use (this sets m_InUse to true on first call, since it's default is false)
	m_InUse = !m_InUse;

	if(m_InUse)
	{
		//perform animation
		switch(m_SkillRank)
		{
		case 1:
			EnableHalfSpeed();
			CloakEffect.EffectDescID = 180;
			break;
		case 3:
			CloakEffect.EffectDescID = 181;
			break;
		case 5:
			CloakEffect.EffectDescID = 182;
			break;
		case 6:
			CloakEffect.EffectDescID = 183;
			break;
		case 7:
			CloakEffect.EffectDescID = 184;
			break;
		}

		//send energy update
		m_Player->SendAuxShip();

		// Send Effect
		CloakEffect.Bitmask = 3;
		CloakEffect.GameID = m_Player->GameID();
		CloakEffect.EffectID = m_EffectID;
		CloakEffect.TimeStamp = m_EffectID;
		CloakEffect.Duration = (short)CloakTime;

		m_Player->SendObjectEffectRL(&CloakEffect);

		//cloak in X seconds (skill rank dependant)
		SetTimer((long)CloakTime*1000); //calls Update when finished.
	}
	else
	{
		//Mark player as uncloaked
		m_Player->ShipIndex()->SetIsCloaked(false);

		m_Player->RemoveEffectRL(m_EffectID);

		if(m_SkillRank == 1)
		{
			EnableHalfSpeed(false);
		}

		//combat cloak damage bonus
		if(m_SkillRank == 5)
		{
			EnableDoubleDamage();

			m_LastUse = GetNet7TickCount();

			SetTimer(5000); //5 seconds of 2x damage
		}

		m_DamageTaken = 0;
	}

	return false;
}

/*
* This function is called when the SetTimer call returns.
*
* DO NOT modify m_InUse in this function.
* DO NOT removeeffects in here. This is done elsewhere.
*/
void ACloak::Update()
{
	if(m_InUse)
	{
		if(m_Player->GetEnergyValue() <= CalculateEnergy(m_SkillLevel, m_SkillRank))
		{
			InterruptSkillOnAction(OTHER);
			return;
		}

		//Mark player as cloaked
		m_Player->ShipIndex()->SetIsCloaked(true);
		m_Player->SetStealthLevel((int)CalculateStealthLevel((long)m_SkillLevel));

		//startup <energy> per second drain
		m_Player->RemoveEnergy(CalculateEnergyPerSecond(m_SkillRank));

		//send energy update
		m_Player->SendAuxShip();

		SetTimer(1000);
	}
	else if(m_DoubleDamageActive && m_LastUse + 5000 <= (int)GetNet7TickCount())
	{
		//5 seconds of 2x beam damage have passed.
		EnableDoubleDamage(false);
	}
	//else, do nothing, skill interrupted during timer.
}

/*
* Returns true if the skill can be interrupted, what can interrupt it
*   is returned by the boolean pointers.
*/
bool ACloak::SkillInterruptable(bool* OnMotion, bool* OnDamage, bool* OnAction)
{
	if(m_SkillRank < 5)
	{
		*OnMotion = true;
	}

	*OnDamage = true;

	if(m_SkillRank < 6)
	{
		*OnAction = true;
	}
	return true;
}

/*
* Determines whether or not this skill was interrupted based on damage taken.
* Note: if you get shot while cloaked, it doesn't play the interrupt effect, it just
*   removes the cloak effect.
*/
bool ACloak::InterruptSkillOnDamage(float damage)
{
	ObjectEffect InterruptEffect;

	if(!m_InUse) //cannot interrupt non-active skill.
	{
		return false;
	}

	if(damage <= 0.0f)
	{
		return false;
	}

	m_DamageTaken += damage;

	if(m_Player->ShipIndex()->GetIsCloaked())
	{
		m_Player->ShipIndex()->SetIsCloaked(false);
		m_InUse = false;

		if(m_SkillRank < 5)
		{
			EnableHalfSpeed(false);

			//remove cloak effect
			m_Player->RemoveEffectRL(m_EffectID);
		}
		else if(m_SkillRank == 5)
		{
			EnableDoubleDamage();

			//remove cloak effect
			m_Player->RemoveEffectRL(m_EffectID);

			SetTimer(5000);
		}
		else
		{
			//remove cloak effect
			m_Player->RemoveEffectRL(m_EffectID);

			//TO-DO: Remove AoE cloak effects as well.
		}

		m_DamageTaken = 0;

		return true;
	}
	else if(m_DamageTaken >= pow((float)m_Player->TotalLevel(), 4) + 20 + m_SkillLevel*100)
	{
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

		m_DamageTaken = 0;

		return true;
	}

	return false;
}

/*
* Determines whether or not this skill was interrupted based on current motion.
*/
bool ACloak::InterruptSkillOnMotion(float speed)
{
	ObjectEffect InterruptEffect;

	if(!m_InUse) //skill not in use, cannot interrupt.
	{
		return false;
	}

	if(m_Player->ShipIndex()->GetIsCloaked())
	{
		return false;
	}
	else if(m_SkillRank >= 5)
	{
		return false;
	}

	//remove current effect
	m_Player->RemoveEffectRL(m_EffectID);

	//ensure we are not flagged as cloaked, for good measure.
	m_Player->ShipIndex()->SetIsCloaked(false);

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
	m_DamageTaken = 0;

	//REMOVE ME
	return false;
}

/*
* Returns true if this skill was interrupted based on player action.
*
* Action = Shooting guns, using devices or activated effects, starting to warp,
*  anything except basic impulse or opening windows on the player's client.
*/
bool ACloak::InterruptSkillOnAction(int Type)
{
	if(Type == WARPING && m_SkillRank >= 6)
	{
		return false;
	}

	//Remove stealth effect
	m_Player->RemoveEffectRL(m_EffectID);
	m_Player->ShipIndex()->SetIsCloaked(false);

	//mark skill as not in use
	m_InUse = false;

	//if combat cloak enable double damage
	if(m_SkillRank == 5)
	{
		EnableDoubleDamage();
		m_LastUse = GetNet7TickCount();
		SetTimer(5000);
	}

	//if rank 1,3 cloak, restore full speed.
	if(m_SkillRank <= 3)
	{
		EnableHalfSpeed(false);
	}

	m_DamageTaken = 0;

	return true;
}
