#include "AuxSkillAbility.h"

void AuxSkillAbility::BuildPacket(unsigned char *buffer, long &index)
{
	AddFlags(Flags, sizeof(Flags), buffer, index);

	if (Flags[0] & 0x10)	//ExtendedFlags[1] & 0x08
	{
		AddData(buffer, char(Data->HasAbility), index);
	}

	if (Flags[0] & 0x20)	//ExtendedFlags[1] & 0x10
	{
		AddData(buffer, Data->Usage, index);
	}

	if (Flags[0] & 0x40)	//ExtendedFlags[1] & 0x20
	{
		AddData(buffer, Data->Targets, index);
	}

	if (Flags[0] & 0x80)	//ExtendedFlags[1] & 0x40
	{
		AddData(buffer, Data->Range, index);
	}

	if (Flags[1] & 0x01)	//ExtendedFlags[1] & 0x80
	{
		AddData(buffer, Data->Radius, index);
	}

	if (Flags[1] & 0x02)	//ExtendedFlags[2] & 0x01
	{
		AddData(buffer, Data->Validity, index);
	}

	if (Flags[1] & 0x04)	//ExtendedFlags[2] & 0x02
	{
		AddData(buffer, char(Data->CanBeUsedWhileIncapacitated), index);
	}

	memset(Flags,0,sizeof(Flags));
}

void AuxSkillAbility::BuildExtendedPacket(unsigned char *buffer, long &index)
{
	AddFlags(ExtendedFlags, sizeof(ExtendedFlags), buffer, index);

	if (ExtendedFlags[0] & 0x10)	//ExtendedFlags[1] & 0x08
	{
		AddData(buffer, char(Data->HasAbility), index);
	}

	if (ExtendedFlags[0] & 0x20)	//ExtendedFlags[1] & 0x10
	{
		AddData(buffer, Data->Usage, index);
	}

	if (ExtendedFlags[0] & 0x40)	//ExtendedFlags[1] & 0x20
	{
		AddData(buffer, Data->Targets, index);
	}

	if (ExtendedFlags[0] & 0x80)	//ExtendedFlags[1] & 0x40
	{
		AddData(buffer, Data->Range, index);
	}

	if (ExtendedFlags[1] & 0x01)	//ExtendedFlags[1] & 0x80
	{
		AddData(buffer, Data->Radius, index);
	}

	if (ExtendedFlags[1] & 0x02)	//ExtendedFlags[2] & 0x01
	{
		AddData(buffer, Data->Validity, index);
	}

	if (ExtendedFlags[1] & 0x04)	//ExtendedFlags[2] & 0x02
	{
		AddData(buffer, char(Data->CanBeUsedWhileIncapacitated), index);
    }
}

/******************************
*         GET METHODS         *
******************************/

_SkillAbility * AuxSkillAbility::GetData()				{return Data;}

bool AuxSkillAbility::GetHasAbility()					{return Data->HasAbility;}
u32 AuxSkillAbility::GetUsage()							{return Data->Usage;}
u32 AuxSkillAbility::GetTargets()						{return Data->Targets;}
u32 AuxSkillAbility::GetRange()							{return Data->Range;}
u32 AuxSkillAbility::GetRadius()						{return Data->Radius;}
u32 AuxSkillAbility::GetValidity()						{return Data->Validity;}
bool AuxSkillAbility::GetCanBeUsedWhileIncapacitated()	{return Data->CanBeUsedWhileIncapacitated;}

/******************************
*         SET METHODS         *
******************************/

void AuxSkillAbility::SetData(_SkillAbility * NewData)
{
	ReplaceData(Data->HasAbility, NewData->HasAbility, 0);
	ReplaceData(Data->Usage, NewData->Usage, 1);
	ReplaceData(Data->Targets, NewData->Targets, 2);
	ReplaceData(Data->Range, NewData->Range, 3);
	ReplaceData(Data->Radius, NewData->Radius, 4);
	ReplaceData(Data->Validity, NewData->Validity, 5);
	ReplaceData(Data->CanBeUsedWhileIncapacitated, NewData->CanBeUsedWhileIncapacitated, 6);

    CheckData();
}

void AuxSkillAbility::SetHasAbility(bool NewHasAbility)
{
	ReplaceData(Data->HasAbility, NewHasAbility, 0);
    CheckData();
}

void AuxSkillAbility::SetUsage(u32 NewUsage)
{
	ReplaceData(Data->Usage, NewUsage, 1);
    CheckData();
}

void AuxSkillAbility::SetTargets(u32 NewTargets)
{
	ReplaceData(Data->Targets, NewTargets, 2);
    CheckData();
}

void AuxSkillAbility::SetRange(u32 NewRange)
{
	ReplaceData(Data->Range, NewRange, 3);
    CheckData();
}

void AuxSkillAbility::SetRadius(u32 NewRadius)
{
	ReplaceData(Data->Radius, NewRadius, 4);
    CheckData();
}

void AuxSkillAbility::SetValidity(u32 NewValidity)
{
	ReplaceData(Data->Validity, NewValidity, 5);
    CheckData();
}

void AuxSkillAbility::SetCanBeUsedWhileIncapacitated(bool NewCanBeUsedWhileIncapacitated)
{
	ReplaceData(Data->CanBeUsedWhileIncapacitated, NewCanBeUsedWhileIncapacitated, 6);
    CheckData();
}

/******************************
*     PARENT FLAG METHODS     *
******************************/

void AuxSkillAbility::CheckData()
{
	SetParentExtendedFlag(HasData());
}

int AuxSkillAbility::HasData()
{
	return (ExtendedFlags[0] & 0xF0 || ExtendedFlags[1] & 0x07);
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxSkillAbility::Clear()
{
    _SkillAbility ClearData;

	ClearData.HasAbility = false;
	ClearData.Usage = 0;
	ClearData.Targets = 0;
	ClearData.Range = 0; 
	ClearData.Radius = 0;
	ClearData.Validity = 0;
	ClearData.CanBeUsedWhileIncapacitated = 0;

    SetData(&ClearData);
}

void AuxSkillAbility::ClearFlags()
{
	memset(Flags,0,sizeof(Flags));
}