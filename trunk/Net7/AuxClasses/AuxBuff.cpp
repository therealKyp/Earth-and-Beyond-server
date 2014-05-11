#include "AuxBuff.h"

void AuxBuff::BuildPacket(unsigned char *buffer, long &index)
{
	AddFlags(Flags, sizeof(Flags), buffer, index);

	if (Flags[0] & 0x10)	//ExtendedFlags[1] & 0x02
	{
		AddString(buffer, Data->BuffType, index);
	}

	if (Flags[0] & 0x20)	//ExtendedFlags[1] & 0x04
	{
		AddString(buffer, Data->ScrubTypeName, index);
	}

	if (Flags[0] & 0x40)	//ExtendedFlags[1] & 0x08
	{
		AddData(buffer, char(Data->IsPermanent), index);
	}

	if (Flags[0] & 0x80)	//ExtendedFlags[1] & 0x10
	{
		AddData(buffer, Data->BuffRemovalTime, index);
	}

	if (Flags[1] & 0x01)	//ExtendedFlags[1] & 0x20
	{
		Elements.BuildPacket(buffer, index);
	}

	memset(Flags,0,sizeof(Flags));
}

void AuxBuff::BuildExtendedPacket(unsigned char *buffer, long &index)
{
	AddFlags(ExtendedFlags, sizeof(ExtendedFlags), buffer, index);

	if (ExtendedFlags[0] & 0x10)	//ExtendedFlags[1] & 0x02
	{
		AddString(buffer, Data->BuffType, index);
	}

	if (ExtendedFlags[0] & 0x20)	//ExtendedFlags[1] & 0x04
	{
		AddString(buffer, Data->ScrubTypeName, index);
	}

	if (ExtendedFlags[0] & 0x40)	//ExtendedFlags[1] & 0x08
	{
		AddData(buffer, char(Data->IsPermanent), index);
	}

	if (ExtendedFlags[0] & 0x80)	//ExtendedFlags[1] & 0x10
	{
		AddData(buffer, Data->BuffRemovalTime, index);
	}

	if (ExtendedFlags[1] & 0x01)	//ExtendedFlags[1] & 0x20
	{
		Elements.BuildExtendedPacket(buffer, index);
	}
	else if (ExtendedFlags[1] & 0x20)
	{
		AddData(buffer, char(0x05), index);
	}
}

/******************************
*         GET METHODS         *
******************************/

_Buff * AuxBuff::GetData()          {return Data;}

char * AuxBuff::GetBuffType()		{return Data->BuffType;}
char * AuxBuff::GetScrubTypeName()	{return Data->ScrubTypeName;}
bool AuxBuff::GetIsPermanent()		{return Data->IsPermanent;}
u32 AuxBuff::GetBuffRemovalTime()	{return Data->BuffRemovalTime;}

/******************************
*         SET METHODS         *
******************************/

void AuxBuff::SetData(_Buff *NewData)
{
	ReplaceString(Data->BuffType, NewData->BuffType, 0);
	ReplaceString(Data->ScrubTypeName, NewData->ScrubTypeName, 1);
	ReplaceData(Data->IsPermanent, NewData->IsPermanent, 2);
	ReplaceData(Data->BuffRemovalTime, NewData->BuffRemovalTime, 3);

	Elements.SetData(&NewData->Elements);

	CheckData();
}

void AuxBuff::SetBuffType(char *NewBuffType)
{
	ReplaceString(Data->BuffType, NewBuffType, 0);
	CheckData();
}

void AuxBuff::SetScrubTypeName(char *NewScrubTypeName)
{
	ReplaceString(Data->ScrubTypeName, NewScrubTypeName, 1);
	CheckData();
}

void AuxBuff::SetIsPermanent(bool NewIsPermanent)
{
	ReplaceData(Data->IsPermanent, NewIsPermanent, 2);
	CheckData();
}

void AuxBuff::SetBuffRemovalTime(u32 NewBuffRemovalTime)
{
	ReplaceData(Data->BuffRemovalTime, NewBuffRemovalTime, 3);
	CheckData();
}

/******************************
*     PARENT FLAG METHODS     *
******************************/

void AuxBuff::CheckData()
{
	SetParentExtendedFlag(HasData());
}

int AuxBuff::HasData()
{
	return (ExtendedFlags[0] & 0xF0 || ExtendedFlags[1] & 0x01);
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxBuff::Clear()
{
    _Buff ClearData;

	*ClearData.BuffType = 0; 
	*ClearData.ScrubTypeName = 0; 
	ClearData.IsPermanent = 0;
	ClearData.BuffRemovalTime = 0;
    ClearData.Elements = Elements.GetClearStruct();

    SetData(&ClearData);
}

void AuxBuff::ClearFlags()
{
	memset(Flags,0,sizeof(Flags));
}
