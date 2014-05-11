#ifndef _ABILITY_TEMPLATE_H_INCLUDED_
#define _ABILITY_TEMPLATE_H_INCLUDED_

#include "AbilityBase.h"
#include "ServerManager.h"
#include "MOBClass.h"

class AShieldSap : public AbilityBase
{
public:
	AShieldSap(Player * me) { 
		m_Player = me; 
		m_LastUse = 0;
		m_InUse = false;
		m_TargetMob = NULL;
	};

public:
	bool Use(int TargetID);						// When a player trys to use an ability
	void Confirmation(bool Confirm);			// Used when a conformation is needed
	void Execute();								// Used when ability can be used on other players
	bool CanUse(int TargetID, long AbilityID, long SkillID);	// Used to see if player can use this ability
	void Update();								// Updated by a timer

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
	
	float CalculateShieldDrainPercent(float SkillLevel, long SkillRank);
	float CalculateShieldRestorePercent(float SkillLevel, long SkillRank);
	float CalculateShieldRestoreValue(float SkillLevel, long SkillRank, float shields_drained);
	inline float DrainShields();
	inline float DrainShieldsAOE();

	MOB* m_TargetMob;
};

#endif