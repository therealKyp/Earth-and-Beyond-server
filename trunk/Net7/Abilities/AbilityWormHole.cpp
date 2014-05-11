#include "..\net7.h"
#include "..\AbilityBase.h"
#include "AbilityWormHole.h"
#include "PlayerClass.h"
#include "ServerManager.h"
#include "ObjectManager.h"

class PlayerClass;

/*
* This calculates the activation cost of the skill.
*/
float AWormHole::CalculateEnergy ( float SkillLevel, long SkillRank )
{
	float EnergyCost;

	switch (SkillRank)
	{
		case 1:
			EnergyCost = 35;
			break;
		case 2:
			EnergyCost = 50;
			break;
		case 3:
			EnergyCost = 75;
			break;
		case 4:
			EnergyCost = 100;
			break;
		case 5:
			EnergyCost = 150;
			break;
		case 6:
			EnergyCost = 250;
			break;
		case 7:
			EnergyCost = 350;
			break;
	}

	return EnergyCost;
}





/*
 * See what SectorID we should use based on the ability thats used
 */
int AWormHole::GetSectorID( long SkillRank )
{
	int SectorID = 0;

	switch (SkillRank)
	{
		case 1: // KAILAASA
			SectorID = 1910;
			break;
		case 2: // Jupiter
			SectorID = 1070;
			break;
		case 3: // SWOOPING_EAGLE
			SectorID = 4120;
			break;
		case 4: // VALKYRIE_TWINS
			SectorID = 1705;
			break;
		case 5: // ASTEROID_BELT_BETA
			SectorID = 1077;
			break;
		case 6: // CARPENTER
			SectorID = 4520;
			break;
		case 7: // ENDRIAGO
			SectorID = 2210;
			break;
	}
	return SectorID;
}

/*
* Calculate how much time must pass before the skill activates.
*
* Results are returned in seconds.
*/
float AWormHole::CalculateChargeUpTime ( float SkillLevel, long SkillRank )
{
	return 3000.0f;
}

/*
* Compute how much time must pass between skill uses.
*
* Results are returned in seconds.
*/
float AWormHole::CalculateCoolDownTime ( float SkillLevel, long SkillRank ) 
{
	return 0.0f;
}

/*
* Calculate the maximum range this rank of the skill can be used at.
*/
float AWormHole::CalculateRange ( float SkillLevel, long SkillRank ) 
{
	return 5000.0f;
}

/*
* Determine's which rank of the skill was used based on the SkillID.
*/
long AWormHole::DetermineSkillRank(int SkillID)
{
	switch(SkillID)
	{
		case WORMHOLE_KAILAASA:
			return 1;
		case WORMHOLE_JUPITER:
			return 2;
		case WORMHOLE_SWOOPING_EAGLE:
			return 3;
		case WORMHOLE_VALKYRIE_TWINS:
			return 4;
		case WORMHOLE_ASTEROID_BELT_BETA:
			return 5;
		case WORMHOLE_CARPENTER:
			return 6;
		case WORMHOLE_ENDRIAGO:
			return 7;
		default:
			return -1;
	}
}

// --------------------------------------------


bool AWormHole::CanUse(int TargetID, long AbilityID, long SkillID)
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

	if (m_SkillRank > m_SkillLevel)
	{
		SendError("You do not have enought skill points in this Skill!");
		return false;
	}

	// Make sure we are not dead
	if (m_Player->ShipIndex()->GetIsIncapacitated())
	{
		SendError("Can not use this ability while dead!");
		return false;
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
void AWormHole::Confirmation(bool Confirm, long AbilityID, long GameID)
{
	Player *p = g_PlayerMgr->GetPlayer(GameID);

	if (Confirm && p)
	{
		// Make the player wormhole
		p->WormHole(GetSectorID(m_SkillRank));
	}

	//reguardless of what the player chooses, this is the "end" of the skill, mark it as such.
	if(p == m_Player)
	{
		m_Player->SetCurrentSkill();
		m_DamageTaken = 0.0f;
		m_InUse = false;
	}
}

/*
* If the call to Confirmation() succeeds, this is called.
*/
void AWormHole::Execute()
{
	//Not used
}

/*
* This will be the first function called once the skill is determined
* as useable.
*/
bool AWormHole::Use(int TargetID)
{
	long ChargeTime;
	float EnergyCost = CalculateEnergy(m_SkillLevel, m_SkillRank);

	//allow the ability to be turned off
	if(m_InUse)
	{
		m_Player->SetCurrentSkill();
		m_DamageTaken = 0.0f;
		return false;
	}

	// Remove the energy
	m_Player->RemoveEnergy(EnergyCost);

	ChargeTime = (long) CalculateChargeUpTime(m_SkillLevel, m_SkillRank);

	m_EffectID = GetNet7TickCount();

	// Make sure no one else can use this skill
	m_InUse = true;

	EnergyCost = CalculateEnergy(m_SkillLevel, m_SkillRank);

	// not enough energy
	if (m_Player->GetEnergyValue() < EnergyCost)
	{
		SendError("Not enough energy!");
		m_InUse = false;
		m_DamageTaken = 0;
		return false;
	}

	// Cast time
	SetTimer(ChargeTime);

	return true;
}

/*
* This function is called when the SetTimer call returns.
*/
void AWormHole::Update()
{
	//see if the user canceled skill during chargeup
	if(!m_InUse)
	{
		m_Player->SetCurrentSkill();
		m_DamageTaken = 0.0f;
		return;
	}
	
	m_InUse = false;

	// Send a message to all the players in range in the group
	Player * p = NULL;
	int GroupID = m_Player->GroupID();
	if (GroupID != 0)
	{
		for(int MID = 0; MID < 6; MID++)
		{
			int MGameID = g_PlayerMgr->GetMemberID(GroupID, MID);

			// Send to everyone but ourself!
			if (MGameID != m_Player->GameID() && MGameID != -1)
			{
				p = g_ServerMgr->m_PlayerMgr.GetPlayer(MGameID);
				if (p)
				{
					// Make sure we are in the same sector
					if (p->PlayerIndex()->GetSectorNum() == m_Player->PlayerIndex()->GetSectorNum())
					{
						// See if we are now in range of the player
						if (m_Player->RangeFrom(p->Position()) < CalculateRange(m_SkillLevel, m_SkillRank))
						{
							p->SendConfirmation("All cargo items in your inventory will take 50% durability damage if you take this wormHole.  Do you want to take this WormHole?", m_Player->GameID(), m_AbilityID);
						}
					}
				}
			}
		}
	}
	m_Player->SendConfirmation("All cargo items in your inventory will take 50% durability damage if you take this wormHole.  Do you want to take this WormHole?", m_Player->GameID(), m_AbilityID);
}

/*
* Returns true in the case that this skill can be interrupted.
* What can interrupt the skill is returned inside the OnMotion 
*  and OnDamage pointers.
*/
bool AWormHole::SkillInterruptable(bool* OnMotion, bool* OnDamage, bool* OnAction)
{
	*OnMotion = false;
	*OnDamage = false;
	*OnAction = false;
	return true;
}

/*
* Determines whether or not this skill was interrupted based on damage taken.
*/
bool AWormHole::InterruptSkillOnDamage(float Damage)
{
	return false;
}

/*
* Determines whether or not this skill was interrupted based on current motion.
*/
bool AWormHole::InterruptSkillOnMotion(float Speed)
{
	return false;
}

/*
* Returns true if this skill was interrupted based on player action.
*
* Action = Shooting guns, using devices or activated effects, starting to warp,
*  anything except basic impulse or opening windows on the player's client.
*/
bool AWormHole::InterruptSkillOnAction(int Type)
{
	return false;
}
