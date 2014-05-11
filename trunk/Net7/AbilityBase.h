#ifndef _ABILITY_BASE_H_INCLUDED_
#define _ABILITY_BASE_H_INCLUDED_

/*
* A nore on the difference between "SkillLevel" and "SkillRank".
* -SkillLevel refers to the actual level of the skill that the player has. That is, how
*   many golden buttons appear on his or her character screen for a given skill.
* -SkillRank refers to the actual rank of the skill being used. For example, the Skill earned
*   by spending a single point in the skill is the 1st Rank of a skill, the skill obtained by
*   putting 3 golden buttons into a skill is the 3rd rank and so on.
* -SkillRank will tell you if they are using the AoE version of a skill or not. SkillLevel will
*   tell you how much skill to use in your calculations for a given SkillRank of a skill.
*/

#include "Net7.h"
#include "Mutex.h"
#include "PlayerSkills.h"

class Player;
class Connection;
class Mutex;

typedef enum
{
	WARPING,
	SHOOTING,
	ITEM_USE,
	INCAPACITATE,
	OTHER
};

//this is the skill cap, after all buffs/bonuses have been applied.
#define MAX_SKILL_LEVEL 10

class AbilityBase
{
protected:
	AbilityBase() {};
	~AbilityBase() {};

public:
	// Virtual methods
	virtual float CalculateEnergy ( float SkillLevel, long SkillRank ) { return 0.0f; };
	virtual float CalculateChargeUpTime ( float SkillLevel, long SkillRank ) { return 0.0f; };
	virtual float CalculateCoolDownTime ( float SkillLevel, long SkillRank ) { return 0.0f; };
	virtual float CalculateRange ( float SkillLevel, long SkillRank ) { return 0.0f; };
	virtual float CalculateAOE ( float SkillLevel, long SkillRank ) {return 0.0f; };
	virtual long DetermineSkillRank (int SkillID) {return 0;};

	virtual	bool Use(int TargetID) { return false; };	// When a player trys to use an ability
	virtual void Update() {};									// Used to update by a timer
	virtual void Confirmation(bool Confirm, long AbilityID, long GameID) {};					// Used when a conformation is needed
	virtual void Execute() {};									// Used when ability can be used on other players
	virtual bool CanUse(int TargetID, long AbilityID, long SkillID) { return false; };	// Used to see if player can use this ability
	virtual bool InUse() { return m_InUse; };						// Used to see if this ability is activated
																// (some abilitys can not be activated together)
	//Write in a way to search though all ability IDs associated with this skill
	// and also change the search in PlayerAbilities.cpp
	
	virtual bool SkillInterruptable(bool* OnMotion, bool* OnDamage, bool* OnAction) { return false; };	//Can this skill be interrupted by anything?
	virtual bool InterruptSkillOnDamage(float damage) { return false; };				//Returns if this skill was interrupted based on damage taken
	virtual bool InterruptSkillOnMotion(float speed) { return false; };					//Returns if this skill was interrupted based on current motion.
	virtual bool InterruptSkillOnAction(int type) { return false; };					//Returns if this skill was interrupted based on action taken or not.

	void SetAbilityID(int ID) { m_AbilityID = ID; };
	float GetInterruptThreshHold();

protected:
	// Setup call back
	void	SetTimer(long Duration);
	// Send out error messages
	void	SendError(char * EMsg);

protected:
	float m_SkillLevel;					// Used to hold the current skill level
	long m_SkillRank;					//Hold the rank of the skill being used

	Player	* m_Player;				// My player
	int		m_TargetID;
	long	m_EffectID;
	long	m_AbilityID;
	long	m_SkillID;
	float	m_DamageTaken;

	// Uses
	int		m_LastUse;
	bool	m_InUse;

	Mutex	m_Mutex;
};

#endif