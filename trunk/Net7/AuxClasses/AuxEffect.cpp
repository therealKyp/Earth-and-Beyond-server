#include "AuxEffect.h"

void AuxEffect::BuildPacket(unsigned char *buffer, long &index)
{
	AddFlags(Flags, sizeof(Flags), buffer, index);

	if (Flags[0] & 0x10)	//ExtendedFlags[1] & 0x01
	{
		AddData(buffer, Data->Range, index);
	}

	if (Flags[0] & 0x20)	//ExtendedFlags[1] & 0x02
	{
		AddData(buffer, Data->Usage, index);
	}

	if (Flags[0] & 0x40)	//ExtendedFlags[1] & 0x04
	{
		AddData(buffer, Data->Targets, index);
	}

	if (Flags[0] & 0x80)	//ExtendedFlags[1] & 0x08
	{
		AddData(buffer, Data->Validity, index);
	}

	memset(Flags,0,sizeof(Flags));
}

void AuxEffect::BuildExtendedPacket(unsigned char *buffer, long &index)
{
	AddFlags(ExtendedFlags, sizeof(ExtendedFlags), buffer, index);

	if (ExtendedFlags[0] & 0x10)	//ExtendedFlags[1] & 0x01
	{
		AddData(buffer, Data->Range, index);
	}

	if (ExtendedFlags[0] & 0x20)	//ExtendedFlags[1] & 0x02
	{
		AddData(buffer, Data->Usage, index);
	}

	if (ExtendedFlags[0] & 0x40)	//ExtendedFlags[1] & 0x04
	{
		AddData(buffer, Data->Targets, index);
	}

	if (ExtendedFlags[0] & 0x80)	//ExtendedFlags[1] & 0x08
	{
		AddData(buffer, Data->Validity, index);
	}
}

/******************************
*         GET METHODS         *
******************************/

_Effect * AuxEffect::GetData()			{return Data;}

float AuxEffect::GetRange()				{return Data->Range;}
u32 AuxEffect::GetUsage()				{return Data->Usage;}
u32 AuxEffect::GetTargets()				{return Data->Targets;}
u32 AuxEffect::GetValidity()			{return Data->Validity;}

/******************************
*         SET METHODS         *
******************************/

void AuxEffect::SetData(_Effect * NewData)
{
	ReplaceData(Data->Range, NewData->Range, 0);
	ReplaceData(Data->Usage, NewData->Usage, 1);
	ReplaceData(Data->Targets, NewData->Targets, 2);
	ReplaceData(Data->Validity, NewData->Validity, 3);

	CheckData();
}

void AuxEffect::SetRange(float NewRange)
{
	ReplaceData(Data->Range, NewRange, 0);
	CheckData();
}

void AuxEffect::SetUsage(u32 NewUsage)
{
	ReplaceData(Data->Usage, NewUsage, 1);
	CheckData();
}

void AuxEffect::SetTargets(u32 NewTargets)
{
	ReplaceData(Data->Targets, NewTargets, 2);
	CheckData();
}

void AuxEffect::SetValidity(u32 NewValidiy)
{
	ReplaceData(Data->Validity, NewValidiy, 3);
	CheckData();
}

/******************************
*     PARENT FLAG METHODS     *
******************************/

void AuxEffect::CheckData()
{
	SetParentExtendedFlag(HasData());
}

int AuxEffect::HasData()
{
	return (ExtendedFlags[0] & 0xF0);
}

/******************************
*       UTILITY METHODS       *
******************************/

_Effect AuxEffect::GetClearStruct()
{
    _Effect ClearData;

	ClearData.Range = 0; 
	ClearData.Usage = 0;
	ClearData.Targets = 0;
	ClearData.Validity = 0;

    return ClearData;
}

void AuxEffect::Clear()
{
    SetData(&GetClearStruct());
}

void AuxEffect::ClearFlags()
{
	memset(Flags,0,sizeof(Flags));
}