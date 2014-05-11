#ifndef _ABILITY_CLOAK_H_INCLUDED_
#define _ABILITY_CLOAK_H_INCLUDED_

#include "AbilityBase.h"

class ACloak : public AbilityBase
{
public:
	ACloak(Player * me) { 
		m_Player = me; 
		m_LastUse = 0;
		m_InUse = false;
		m_DoubleDamageActive = false;
		m_DamageTaken = 0.0f;
		m_OldShipSpeed = 0;
		m_BuffID = -1;
	};

public:
	bool Use(int TargetID);						// When a player trys to use an ability
	void Confirmation(bool Confirm, long AbilityID, long GameID);			// Used when a conformation is needed
	void Execute();								// Used when ability can be used on other players
	bool CanUse(int TargetID, long AbilityID, long SkillID);	// Used to see if player can use this ability
	void Update();								// Updated by a timer

	bool SkillInterruptable(bool* OnMotion, bool* OnDamage, bool* OnAction);	//Can this skill be interrupted by anything?
	bool InterruptSkillOnDamage(float Damage);		//Returns if this skill was interrupted based on damage taken
	bool InterruptSkillOnMotion(float Speed);		//Returns if this skill was interrupted based on current motion.
	bool InterruptSkillOnAction(int Type);			//Returns if this skill was interrupted based on action taken or not.

	// Calculate skill level Data
private:
	float CalculateEnergy ( float SkillLevel, long SkillRank );
	float CalculateEnergyPerSecond (long SkillRank );
	float CalculateChargeUpTime ( float SkillLevel, long SkillRank );
	float CalculateCoolDownTime ( float SkillLevel, long SkillRank );
	float CalculateRange ( float SkillLevel, long SkillRank );
	float CalculateAOE ( float SkillLevel, long SkillRank );
	float CalculateStealthLevel ( long SkillLevel );
	long DetermineSkillRank(int SkillID);
	void EnableHalfSpeed(bool answer = true);
	void EnableDoubleDamage(bool answer = true);

	bool m_DoubleDamageActive;
	float m_OldShipSpeed;
	int m_BuffID;
};


#endif