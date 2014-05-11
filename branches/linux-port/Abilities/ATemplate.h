#ifndef _ABILITY_TEMPLATE_H_INCLUDED_
#define _ABILITY_TEMPLATE_H_INCLUDED_

#include "../AbilityBase.h"

class ATemplate : public AbilityBase
{
public:
	ATemplate(Player * me) {
		m_Player = me;
		m_LastUse = 0;
		m_InUse = false;
	};

public:
	bool Use(int TargetID);						// When a player trys to use an ability
	void Confirmation(bool Confirm);			// Used when a conformation is needed
	void Execute();								// Used when ability can be used on other players
	bool CanUse(int TargetID, long AbilityID, long SkillID);	// Used to see if player can use this ability
	void Update();								// Updated by a timer
	bool InUse();								// Used to see if this ability is activated
												// (some abilitys can not be activated together)

	bool SkillInterruptable(bool* OnMotion, bool* OnDamage, bool* OnAction);	//Can this skill be interrupted by anything?
	bool InterruptSkillOnDamage(float Damage);		//Returns if this skill was interrupted based on damage taken
	bool InterruptSkillOnMotion(float Speed);		//Returns if this skill was interrupted based on current motion.
	bool InterruptSkillOnAction(int Type);

	// Calculate skill level Data
private:
	float CalculateEnergy ( float SkillLevel, long SkillRank );
	float CalculateChargeUpTime ( float SkillLevel, long SkillRank );
	float CalculateCoolDownTime ( float SkillLevel, long SkillRank );
	float CalculateRange ( float SkillLevel, long SkillRank );
	float CalculateAOE ( float SkillLevel, long SkillRank );
	long DetermineSkillRank(int SkillID);
};

#endif
