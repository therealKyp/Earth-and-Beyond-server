#include "AuxElement.h"

void AuxElement::BuildPacket(unsigned char *buffer, long &index)
{
	AddFlags(Flags, sizeof(Flags), buffer, index);

	if (Flags[0] & 0x10)	//ExtendedFlags[1] & 0x02
	{
		AddString(buffer, Data->SourceEntity, index);
	}

	if (Flags[0] & 0x20)	//ExtendedFlags[1] & 0x04
	{
		AddString(buffer, Data->SourceObject, index);
	}

	if (Flags[0] & 0x40)	//ExtendedFlags[1] & 0x08
	{
		AddData(buffer, Data->Magnitude, index);
	}

	if (Flags[0] & 0x80)	//ExtendedFlags[1] & 0x10
	{
		AddData(buffer, char(Data->IsActive), index);
	}

	if (Flags[1] & 0x01)	//ExtendedFlags[1] & 0x20
	{
		AddData(buffer, Data->ExpirationTime, index);
	}

	memset(Flags,0,sizeof(Flags));
}

void AuxElement::BuildExtendedPacket(unsigned char *buffer, long &index)
{
	AddFlags(ExtendedFlags, sizeof(ExtendedFlags), buffer, index);

	if (ExtendedFlags[0] & 0x10)	//ExtendedFlags[1] & 0x02
	{
		AddString(buffer, Data->SourceEntity, index);
	}

	if (ExtendedFlags[0] & 0x20)	//ExtendedFlags[1] & 0x04
	{
		AddString(buffer, Data->SourceObject, index);
	}

	if (ExtendedFlags[0] & 0x40)	//ExtendedFlags[1] & 0x08
	{
		AddData(buffer, Data->Magnitude, index);
	}

	if (ExtendedFlags[0] & 0x80)	//ExtendedFlags[1] & 0x10
	{
		AddData(buffer, char(Data->IsActive), index);
	}

	if (ExtendedFlags[1] & 0x01)	//ExtendedFlags[1] & 0x20
	{
		AddData(buffer, Data->ExpirationTime, index);
	}
}

/******************************
*         GET METHODS         *
******************************/

_Element * AuxElement::GetData()		{return Data;}

char * AuxElement::GetSourceEntity()	{return Data->SourceEntity;}
char * AuxElement::GetSourceObject()	{return Data->SourceObject;}
u32 AuxElement::GetMagnitude()			{return Data->Magnitude;}
bool AuxElement::GetIsActive()			{return Data->IsActive;}
u32 AuxElement::GetExpirationTime()		{return Data->ExpirationTime;}

/******************************
*         SET METHODS         *
******************************/

void AuxElement::SetData(_Element *NewData)
{
	ReplaceString(Data->SourceEntity, NewData->SourceEntity, 0);
	ReplaceString(Data->SourceObject, NewData->SourceObject, 1);
	ReplaceData(Data->Magnitude, NewData->Magnitude, 2);
	ReplaceData(Data->IsActive, NewData->IsActive, 3);
	ReplaceData(Data->ExpirationTime, NewData->ExpirationTime, 4);

	CheckData();
}

void AuxElement::SetSourceEntity(char *NewSourceEntity)
{
	ReplaceString(Data->SourceEntity, NewSourceEntity, 0);
	CheckData();
}

void AuxElement::SetSourceObject(char *NewSourceObject)
{
	ReplaceString(Data->SourceObject, NewSourceObject, 1);
	CheckData();
}

void AuxElement::SetMagnitude(u32 NewMagnitude)
{
	ReplaceData(Data->Magnitude, NewMagnitude, 2);
	CheckData();
}

void AuxElement::SetIsActive(bool NewIsActive)
{
	ReplaceData(Data->IsActive, NewIsActive, 3);
	CheckData();
}

void AuxElement::SetExpirationTime(u32 NewExpirationTime)
{
	ReplaceData(Data->ExpirationTime, NewExpirationTime, 4);
	CheckData();
}

/******************************
*     PARENT FLAG METHODS     *
******************************/

void AuxElement::CheckData()
{
	SetParentExtendedFlag(HasData());
}

int AuxElement::HasData()
{
	return (ExtendedFlags[0] & 0xF0 || ExtendedFlags[1] & 0x01);
}

/******************************
*       UTILITY METHODS       *
******************************/

_Element AuxElement::GetClearStruct()
{
    _Element ClearData;

    *ClearData.SourceEntity = 0; 
	*ClearData.SourceObject = 0; 
	ClearData.Magnitude = 0;
	ClearData.IsActive = 0;
	ClearData.ExpirationTime = 0;

    return ClearData;
}

void AuxElement::Clear()
{
    SetData(&GetClearStruct());
}

void AuxElement::ClearFlags()
{
	memset(Flags,0,sizeof(Flags));
}