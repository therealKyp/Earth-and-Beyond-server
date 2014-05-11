#include "AuxSkill.h"

void AuxSkill::BuildPacket(unsigned char *buffer, long &index)
{
	AddFlags(Flags, sizeof(Flags), buffer, index);

	if (Flags[0] & 0x10)	//ExtendedFlags[1] & 0x08
	{
		AddData(buffer, Data->Level, index);
	}

	if (Flags[0] & 0x20)	//ExtendedFlags[1] & 0x10
	{
		AddData(buffer, Data->RecycleTime, index);
	}

	if (Flags[0] & 0x40)	//ExtendedFlags[1] & 0x20
	{
		AddData(buffer, Data->LastActivationTime, index);
	}

	if (Flags[0] & 0x80)	//ExtendedFlags[1] & 0x40
	{
		AddData(buffer, Data->Availability[0], index);
		AddData(buffer, Data->Availability[1], index);
		AddData(buffer, Data->Availability[2], index);
		AddData(buffer, Data->Availability[3], index);
	}

	if (Flags[1] & 0x01)	//ExtendedFlags[1] & 0x80
	{
		AddData(buffer, Data->QuestOnlyLevel, index);
	}

	if (Flags[1] & 0x02)	//ExtendedFlags[2] & 0x01
	{
		AddData(buffer, Data->MaxSkillLevel, index);
	}

	if (Flags[1] & 0x04)	//ExtendedFlags[2] & 0x02
	{
		AddString(buffer, Data->Affiliation, index);
	}

	memset(Flags,0,sizeof(Flags));
}

void AuxSkill::BuildExtendedPacket(unsigned char *buffer, long &index)
{
	AddFlags(ExtendedFlags, sizeof(ExtendedFlags), buffer, index);

	if (ExtendedFlags[0] & 0x10)	//ExtendedFlags[1] & 0x08
	{
		AddData(buffer, Data->Level, index);
	}

	if (ExtendedFlags[0] & 0x20)	//ExtendedFlags[1] & 0x10
	{
		AddData(buffer, Data->RecycleTime, index);
	}

	if (ExtendedFlags[0] & 0x40)	//ExtendedFlags[1] & 0x20
	{
		AddData(buffer, Data->LastActivationTime, index);
	}

	if (ExtendedFlags[0] & 0x80)	//ExtendedFlags[1] & 0x40
	{
		AddData(buffer, Data->Availability[0], index);
		AddData(buffer, Data->Availability[1], index);
		AddData(buffer, Data->Availability[2], index);
		AddData(buffer, Data->Availability[3], index);
	}

	if (ExtendedFlags[1] & 0x01)	//ExtendedFlags[1] & 0x80
	{
		AddData(buffer, Data->QuestOnlyLevel, index);
	}

	if (ExtendedFlags[1] & 0x02)	//ExtendedFlags[2] & 0x01
	{
		AddData(buffer, Data->MaxSkillLevel, index);
	}

	if (ExtendedFlags[1] & 0x04)	//ExtendedFlags[2] & 0x02
	{
		AddString(buffer, Data->Affiliation, index);
	}
}

/******************************
*         GET METHODS         *
******************************/

_Skill * AuxSkill::GetData()			{return Data;}

u32	AuxSkill::GetLevel()				{return Data->Level;}
u32	AuxSkill::GetRecycleTime()			{return Data->RecycleTime;}
u32	AuxSkill::GetLastActivationTime()	{return Data->LastActivationTime;}
u32	* AuxSkill::GetAvailability()		{return Data->Availability;}
u32	AuxSkill::GetQuestOnlyLevel()		{return Data->QuestOnlyLevel;}
u32	AuxSkill::GetMaxSkillLevel()		{return Data->MaxSkillLevel;}
char * AuxSkill::GetAffiliation()		{return Data->Affiliation;}

/******************************
*         SET METHODS         *
******************************/

void AuxSkill::SetData(_Skill * NewData)
{
	ReplaceData(Data->Level, NewData->Level, 0);
	ReplaceData(Data->RecycleTime, NewData->RecycleTime, 1);
	ReplaceData(Data->LastActivationTime, NewData->LastActivationTime, 2);
	ReplaceAvail(Data->Availability, NewData->Availability, 3);
	ReplaceData(Data->QuestOnlyLevel, NewData->QuestOnlyLevel, 4);
	ReplaceData(Data->MaxSkillLevel, NewData->MaxSkillLevel, 5);
	ReplaceString(Data->Affiliation, NewData->Affiliation, 6);
}

void AuxSkill::SetLevel(u32 NewLevel)
{
	ReplaceData(Data->Level, NewLevel, 0);
}

void AuxSkill::SetRecycleTime(u32 NewRecycleTime)
{
	ReplaceData(Data->RecycleTime, NewRecycleTime, 1);
}

void AuxSkill::SetLastActivationTime(u32 NewLastActivationTime)
{
	ReplaceData(Data->LastActivationTime, NewLastActivationTime, 2);
}

void AuxSkill::SetAvailability(u32 * NewAvailability)
{
	ReplaceAvail(Data->Availability, NewAvailability, 3);
}

void AuxSkill::SetQuestOnlyLevel(u32 NewQuestOnlyLevel)
{
	ReplaceData(Data->QuestOnlyLevel, NewQuestOnlyLevel, 4);
}

void AuxSkill::SetMaxSkillLevel(u32 NewMaxSkillLevel)
{
	ReplaceData(Data->MaxSkillLevel, NewMaxSkillLevel, 5);
}

void AuxSkill::SetAffiliation(char * NewAffiliation)
{
	ReplaceString(Data->Affiliation, NewAffiliation, 6);
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxSkill::Clear()
{
    _Skill ClearData;

	ClearData.Level = 0;
	ClearData.RecycleTime = 0;
	ClearData.LastActivationTime = 1;	/* Always set in captured logs */
	ClearData.Availability[0] = 1;
	ClearData.Availability[1] = 0;
	ClearData.Availability[2] = 0;
	ClearData.Availability[3] = 0;
	ClearData.QuestOnlyLevel = 9;
	ClearData.MaxSkillLevel = 0;
	*ClearData.Affiliation = 0;

    SetData(&ClearData);
}

void AuxSkill::ClearFlags()
{
	memset(Flags,0,sizeof(Flags));
}