#include "PlayerClass.h"
#include "ServerManager.h"
#include "ObjectManager.h"
#include "Opcodes.h"

void Player::HandleSkillAbility(unsigned char *data)
{
    SkillUse * Action = (SkillUse *) data;

	//SendVaMessage("Skill id %d just activated.", Action->AbilityIndex);

	// TODO: Add code here
	long base_skill;
	long level;

	base_skill = ConvertAbilityToBaseSkill(level, Action->AbilityIndex);

	if (base_skill >= 0)
	{
		//check mission completion
		CheckMissionSkillUse(base_skill, level);
	}

	// Execute ability
	// See if we are in range ID for the ability
	// And we have a ability handeler
	int TargetID = ShipIndex()->GetTargetGameID();

	if (Action->AbilityIndex < 150 && Action->AbilityIndex >= 0 && m_AbilityList[Action->AbilityIndex])
	{
		// Ask if we can use this on this player
		if (m_AbilityList[Action->AbilityIndex]->CanUse(TargetID, Action->AbilityIndex, base_skill) && 
			(m_CurrentSkill == m_AbilityList[Action->AbilityIndex] || !m_CurrentSkill))
		{
			//mark skill as the skill currently being used.
			SetCurrentSkill(m_AbilityList[Action->AbilityIndex]);

			// If so lets execute it!
			m_AbilityList[Action->AbilityIndex]->Use(TargetID);
		}
	}
	else
	{
		SendPriorityMessageString("Error: This ability is not yet working. Try later!","MessageLine",1000,4);
	}
}

// This is called on a timer
void Player::AbilityRemove(int AbilityID)
{
	// See if we are in range ID for the ability
	// And we have a ability handeler
	if (AbilityID < 150 && AbilityID >= 0 && m_AbilityList[AbilityID])
	{
		// If so lets update the ability!
		m_AbilityList[AbilityID]->Update();
	}
}

//we need a relational database to convert the used skill to base skill and level
//until then, this is a hardcoded system that should suffice for now

long Player::ConvertAbilityToBaseSkill(long &level, long ability)
{
	long base_skill = g_ServerMgr->m_SkillsList.GetBaseSkillID(ability);
	char *desc = g_ServerMgr->m_SkillsList.GetSkillDescription(ability);
	level = g_ServerMgr->m_SkillsList.GetSkillLevel(ability);

	if (desc)
	{
		SendVaMessage("Skill usage: %s", desc);
	}

	return base_skill;
}
