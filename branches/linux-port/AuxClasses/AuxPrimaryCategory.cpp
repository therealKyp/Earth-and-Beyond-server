#include "AuxPrimaryCategory.h"

void AuxPrimaryCategory::BuildPacket(unsigned char *buffer, long &index, bool TwoBitFlags)
{
	if (TwoBitFlags)
	{
		AddFlags(ExtendedFlags, sizeof(ExtendedFlags), buffer, index);
		memcpy(Flags, ExtendedFlags, sizeof(Flags));
	}
	else
	{
		AddFlags(Flags, sizeof(Flags), buffer, index);
	}

	if (Flags[0] & 0x10)	//ExtendedFlags[0] & 0x40
	{
		AddString(buffer, Data->Name, index);
	}

	if (Flags[0] & 0x20)	//ExtendedFlags[0] & 0x80
	{
		Categories.BuildPacket(buffer, index, TwoBitFlags);
	}
	else if (TwoBitFlags && ExtendedFlags[0] & 0x80)
	{
		AddData(buffer, char(0x05), index);
	}

	memset(Flags,0,sizeof(Flags));
}

/******************************
*         GET METHODS         *
******************************/

_PrimaryCategory * AuxPrimaryCategory::GetData()    {return Data;}

char * AuxPrimaryCategory::GetName()                {return Data->Name;}

/******************************
*         SET METHODS         *
******************************/

void AuxPrimaryCategory::SetData(_PrimaryCategory * NewData)
{
	ReplaceString(Data->Name, NewData->Name, 0);
    Categories.SetData(&NewData->Categories);

    CheckData();
}

void AuxPrimaryCategory::SetName(char * NewName)
{
	ReplaceString(Data->Name, NewName, 0);
    CheckData();
}

/******************************
*     PARENT FLAG METHODS     *
******************************/

void AuxPrimaryCategory::CheckData()
{
	SetParentExtendedFlag(HasData());
}

int AuxPrimaryCategory::HasData()
{
	return (ExtendedFlags[0] & 0x30);
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxPrimaryCategory::ClearFlags()
{
	memset(Flags,0,sizeof(Flags));
    Categories.ClearFlags();
}