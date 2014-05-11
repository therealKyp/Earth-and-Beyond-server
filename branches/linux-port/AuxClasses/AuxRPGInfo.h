#ifndef _AUXRPGINFO_H_INCLUDED_
#define _AUXRPGINFO_H_INCLUDED_

#include "AuxSkills.h"
#include "AuxSkillAbilities.h"
	
struct _RPGInfo
{
	u32 Race;
	u32 Profession;
	_Skills Skills;
	_SkillAbilities Abilities;
	u32 SkillPoints;
	u32 TotalSkillPoints;
	float CombatXP;
	u32 CombatLevel;
	float TradeXP;
	u32 TradeLevel;
	float ExploreXP;
	u32 ExploreLevel;
	u32 HullUpgradeLevel;
	u32 SkillPowerUpStartTime;
	u32 SkillPowerUpEndTime;
	s32 SkillPowerUpAbilityNumber;
} ATTRIB_PACKED;

class AuxRPGInfo : public AuxBase
{
public:
    AuxRPGInfo()
	{
	}

    ~AuxRPGInfo()
	{
	}

	void Init(unsigned int MemberIndex, class AuxBase * Parent, _RPGInfo * DataPointer)
	{
		Construct(Flags, ExtendedFlags, 16, Parent, MemberIndex);
        Data = DataPointer;

		memset(Flags,0, sizeof(Flags));

		ExtendedFlags[0] = char(0xC6);
		ExtendedFlags[1] = char(0x00);
		ExtendedFlags[2] = char(0xF8);
		ExtendedFlags[3] = char(0xFF);
		ExtendedFlags[4] = char(0x07);

		Data->Race = 0;
		Data->Profession = 0;
		Skills.Init(2, this, &Data->Skills);
		Abilities.Init(3, this, &Data->Abilities);
		Data->SkillPoints = 0;
		Data->TotalSkillPoints = 0;
		Data->CombatXP = 0;
		Data->CombatLevel = 0;
		Data->TradeXP = 0;
		Data->TradeLevel = 0;
		Data->ExploreXP = 0;
		Data->ExploreLevel = 0;
		Data->HullUpgradeLevel = 0;
		Data->SkillPowerUpStartTime = 0;
		Data->SkillPowerUpEndTime = 0;
		Data->SkillPowerUpAbilityNumber = -1;
	}

    void ClearFlags();

	void BuildPacket(unsigned char *, long &);
	void BuildExtendedPacket(unsigned char *, long &);

	_RPGInfo * GetData();

	u32 GetRace();
	u32 GetProfession();
	u32 GetSkillPoints();
	u32 GetTotalSkillPoints();
	float GetCombatXP();
	u32 GetCombatLevel();
	float GetTradeXP();
	u32 GetTradeLevel();
	float GetExploreXP();
	u32 GetExploreLevel();
	u32 GetHullUpgradeLevel();
	u32 GetSkillPowerUpStartTime();
	u32 GetSkillPowerUpEndTime();
	s32 GetSkillPowerUpAbilityNumber();

	void SetData(_RPGInfo *);

	void SetRace(u32);
	void SetProfession(u32);
	void SetSkillPoints(u32);
	void SetTotalSkillPoints(u32);
	void SetCombatXP(float);
	void SetCombatLevel(u32);
	void SetTradeXP(float);
	void SetTradeLevel(u32);
	void SetExploreXP(float);
	void SetExploreLevel(u32);
	void SetHullUpgradeLevel(u32);
	void SetSkillPowerUpStartTime(u32);
	void SetSkillPowerUpEndTime(u32);
	void SetSkillPowerUpAbilityNumber(s32);

private:
	_RPGInfo * Data;

	unsigned char Flags[3];
	unsigned char ExtendedFlags[5];

public:
	class AuxSkills Skills;
	class AuxSkillAbilities Abilities;
};

#endif
