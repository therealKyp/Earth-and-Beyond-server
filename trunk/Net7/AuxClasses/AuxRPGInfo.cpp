#include "AuxRPGInfo.h"

void AuxRPGInfo::BuildPacket(unsigned char *buffer, long &index)
{
	AddFlags(Flags, sizeof(Flags), buffer, index);

	if (Flags[0] & 0x10)	//ExtendedFlags[2] & 0x10
	{
		AddData(buffer, Data->Race, index);
	}

	if (Flags[0] & 0x20)	//ExtendedFlags[2] & 0x20
	{
		AddData(buffer, Data->Profession, index);
	}

	if (Flags[0] & 0x40)	//ExtendedFlags[2] & 0x40
	{
		Skills.BuildPacket(buffer, index);
	}

	if (Flags[0] & 0x80)	//ExtendedFlags[2] & 0x80
	{
		Abilities.BuildPacket(buffer, index);
	}

	if (Flags[1] & 0x01)	//ExtendedFlags[3] & 0x01
	{
		AddData(buffer, Data->SkillPoints, index);
	}

	if (Flags[1] & 0x02)	//ExtendedFlags[3] & 0x02
	{
		AddData(buffer, Data->TotalSkillPoints, index);
	}

	if (Flags[1] & 0x04)	//ExtendedFlags[3] & 0x04
	{
		AddData(buffer, Data->CombatXP, index);
	}

	if (Flags[1] & 0x08)	//ExtendedFlags[3] & 0x08
	{
		AddData(buffer, Data->CombatLevel, index);
	}

	if (Flags[1] & 0x10)	//ExtendedFlags[3] & 0x10
	{
		AddData(buffer, Data->TradeXP, index);
	}

	if (Flags[1] & 0x20)	//ExtendedFlags[3] & 0x20
	{
		AddData(buffer, Data->TradeLevel, index);
	}

	if (Flags[1] & 0x40)	//ExtendedFlags[3] & 0x40
	{
		AddData(buffer, Data->ExploreXP, index);
	}

	if (Flags[1] & 0x80)	//ExtendedFlags[3] & 0x80
	{
		AddData(buffer, Data->ExploreLevel, index);
	}

	if (Flags[2] & 0x01)	//ExtendedFlags[4] & 0x01
	{
		AddData(buffer, Data->HullUpgradeLevel, index);
	}

	if (Flags[2] & 0x02)	//ExtendedFlags[4] & 0x02
	{
		AddData(buffer, Data->SkillPowerUpStartTime, index);
	}

	if (Flags[2] & 0x04)	//ExtendedFlags[4] & 0x04
	{
		AddData(buffer, Data->SkillPowerUpEndTime, index);
	}

	if (Flags[2] & 0x08)	//ExtendedFlags[4] & 0x08
	{
		AddData(buffer, Data->SkillPowerUpAbilityNumber, index);
	}

	memset(Flags,0,sizeof(Flags));
}

void AuxRPGInfo::BuildExtendedPacket(unsigned char *buffer, long &index)
{
	AddFlags(ExtendedFlags, sizeof(ExtendedFlags), buffer, index);

	if (ExtendedFlags[0] & 0x10)	//ExtendedFlags[2] & 0x10
	{
		AddData(buffer, Data->Race, index);
	}

	if (ExtendedFlags[0] & 0x20)	//ExtendedFlags[2] & 0x20
	{
		AddData(buffer, Data->Profession, index);
	}

	if (ExtendedFlags[0] & 0x40)	//ExtendedFlags[2] & 0x40
	{
		Skills.BuildExtendedPacket(buffer, index);
	}
	else if (ExtendedFlags[2] & 0x40)
	{
		AddData(buffer, char(0x05), index);
	}

	if (ExtendedFlags[0] & 0x80)	//ExtendedFlags[2] & 0x80
	{
		Abilities.BuildExtendedPacket(buffer, index);
	}
	else if (ExtendedFlags[2] & 0x80)
	{
		AddData(buffer, char(0x05), index);
	}

	if (ExtendedFlags[1] & 0x01)	//ExtendedFlags[3] & 0x01
	{
		AddData(buffer, Data->SkillPoints, index);
	}

	if (ExtendedFlags[1] & 0x02)	//ExtendedFlags[3] & 0x02
	{
		AddData(buffer, Data->TotalSkillPoints, index);
	}

	if (ExtendedFlags[1] & 0x04)	//ExtendedFlags[3] & 0x04
	{
		AddData(buffer, Data->CombatXP, index);
	}

	if (ExtendedFlags[1] & 0x08)	//ExtendedFlags[3] & 0x08
	{
		AddData(buffer, Data->CombatLevel, index);
	}

	if (ExtendedFlags[1] & 0x10)	//ExtendedFlags[3] & 0x10
	{
		AddData(buffer, Data->TradeXP, index);
	}

	if (ExtendedFlags[1] & 0x20)	//ExtendedFlags[3] & 0x20
	{
		AddData(buffer, Data->TradeLevel, index);
	}

	if (ExtendedFlags[1] & 0x40)	//ExtendedFlags[3] & 0x40
	{
		AddData(buffer, Data->ExploreXP, index);
	}

	if (ExtendedFlags[1] & 0x80)	//ExtendedFlags[3] & 0x80
	{
		AddData(buffer, Data->ExploreLevel, index);
	}

	if (ExtendedFlags[2] & 0x01)	//ExtendedFlags[4] & 0x01
	{
		AddData(buffer, Data->HullUpgradeLevel, index);
	}

	if (ExtendedFlags[2] & 0x02)	//ExtendedFlags[4] & 0x02
	{
		AddData(buffer, Data->SkillPowerUpStartTime, index);
	}

	if (ExtendedFlags[2] & 0x04)	//ExtendedFlags[4] & 0x04
	{
		AddData(buffer, Data->SkillPowerUpEndTime, index);
	}

	if (ExtendedFlags[2] & 0x08)	//ExtendedFlags[4] & 0x08
	{
		AddData(buffer, Data->SkillPowerUpAbilityNumber, index);
	}
}

/******************************
*         GET METHODS         *
******************************/

_RPGInfo * AuxRPGInfo::GetData()                    {return Data;}

u32 AuxRPGInfo::GetRace()							{return Data->Race;}
u32 AuxRPGInfo::GetProfession()						{return Data->Profession;}
u32 AuxRPGInfo::GetSkillPoints()					{return Data->SkillPoints;}
u32 AuxRPGInfo::GetTotalSkillPoints()				{return Data->TotalSkillPoints;}
float AuxRPGInfo::GetCombatXP()						{return Data->CombatXP;}
u32 AuxRPGInfo::GetCombatLevel()					{return Data->CombatLevel;}
float AuxRPGInfo::GetTradeXP()						{return Data->TradeXP;}
u32 AuxRPGInfo::GetTradeLevel()						{return Data->TradeLevel;}
float AuxRPGInfo::GetExploreXP()					{return Data->ExploreXP;}
u32 AuxRPGInfo::GetExploreLevel()					{return Data->ExploreLevel;}
u32 AuxRPGInfo::GetHullUpgradeLevel()				{return Data->HullUpgradeLevel;}
u32 AuxRPGInfo::GetSkillPowerUpStartTime()			{return Data->SkillPowerUpStartTime;}
u32 AuxRPGInfo::GetSkillPowerUpEndTime()			{return Data->SkillPowerUpEndTime;}
s32 AuxRPGInfo::GetSkillPowerUpAbilityNumber()		{return Data->SkillPowerUpAbilityNumber;}

/******************************
*         SET METHODS         *
******************************/

void AuxRPGInfo::SetData(_RPGInfo *NewData)
{
	ReplaceData(Data->Race, NewData->Race, 0);
	ReplaceData(Data->Profession, NewData->Profession, 1);
	Skills.SetData(&NewData->Skills);
	Abilities.SetData(&NewData->Abilities);
	ReplaceData(Data->SkillPoints, NewData->SkillPoints, 4);
	ReplaceData(Data->TotalSkillPoints, NewData->TotalSkillPoints, 5);
	ReplaceData(Data->CombatXP, NewData->CombatXP, 6);
	ReplaceData(Data->CombatLevel, NewData->CombatLevel, 7);
	ReplaceData(Data->TradeXP, NewData->TradeXP, 8);
	ReplaceData(Data->TradeLevel, NewData->TradeLevel, 9);
	ReplaceData(Data->ExploreXP, NewData->ExploreXP, 10);
	ReplaceData(Data->ExploreLevel, NewData->ExploreLevel, 11);
	ReplaceData(Data->HullUpgradeLevel, NewData->HullUpgradeLevel, 12);
	ReplaceData(Data->SkillPowerUpStartTime, NewData->SkillPowerUpStartTime, 13);
	ReplaceData(Data->SkillPowerUpEndTime, NewData->SkillPowerUpEndTime, 14);
	ReplaceData(Data->SkillPowerUpAbilityNumber, NewData->SkillPowerUpAbilityNumber, 15);
}

void AuxRPGInfo::SetRace(u32 NewRace)
{
	ReplaceData(Data->Race, NewRace, 0);
}

void AuxRPGInfo::SetProfession(u32 NewProfession)
{
	ReplaceData(Data->Profession, NewProfession, 1);
}

void AuxRPGInfo::SetSkillPoints(u32 NewSkillPoints)
{
	ReplaceData(Data->SkillPoints, NewSkillPoints, 4);
}

void AuxRPGInfo::SetTotalSkillPoints(u32 NewTotalSkillPoints)
{
	ReplaceData(Data->TotalSkillPoints, NewTotalSkillPoints, 5);
}

void AuxRPGInfo::SetCombatXP(float NewCombatXP)
{
	ReplaceData(Data->CombatXP, NewCombatXP, 6);
}

void AuxRPGInfo::SetCombatLevel(u32 NewCombatLevel)
{
	ReplaceData(Data->CombatLevel, NewCombatLevel, 7);
}

void AuxRPGInfo::SetTradeXP(float NewTradeXP)
{
	ReplaceData(Data->TradeXP, NewTradeXP, 8);
}

void AuxRPGInfo::SetTradeLevel(u32 NewTradeLevel)
{
	ReplaceData(Data->TradeLevel, NewTradeLevel, 9);
}

void AuxRPGInfo::SetExploreXP(float NewExploreXP)
{
	ReplaceData(Data->ExploreXP, NewExploreXP, 10);
}

void AuxRPGInfo::SetExploreLevel(u32 NewExploreLevel)
{
	ReplaceData(Data->ExploreLevel, NewExploreLevel, 11);
}

void AuxRPGInfo::SetHullUpgradeLevel(u32 NewHullUpgradeLevel)
{
	ReplaceData(Data->HullUpgradeLevel, NewHullUpgradeLevel, 12);
}

void AuxRPGInfo::SetSkillPowerUpStartTime(u32 NewSkillPowerUpStartTime)
{
	ReplaceData(Data->SkillPowerUpStartTime, NewSkillPowerUpStartTime, 13);
}

void AuxRPGInfo::SetSkillPowerUpEndTime(u32 NewSkillPowerUpEndTime)
{
	ReplaceData(Data->SkillPowerUpEndTime, NewSkillPowerUpEndTime, 14);
}

void AuxRPGInfo::SetSkillPowerUpAbilityNumber(s32 NewSkillPowerUpAbilityNumber)
{
	ReplaceData(Data->SkillPowerUpAbilityNumber, NewSkillPowerUpAbilityNumber, 15);
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxRPGInfo::ClearFlags()
{
	memset(Flags,0,sizeof(Flags));
    Skills.ClearFlags();
    Abilities.ClearFlags();
}