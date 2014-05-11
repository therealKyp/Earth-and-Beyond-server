#include "AuxCategory.h"

void AuxCategory::BuildPacket(unsigned char *buffer, long &index, bool TwoBitFlags)
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

	if (Flags[0] & 0x10)	//ExtendedFlags[0] & 0x80
	{
		AddString(buffer, Data->Name, index);
	}

	if (Flags[0] & 0x20)	//ExtendedFlags[1] & 0x01
	{
		SubCategories.BuildPacket(buffer, index, TwoBitFlags);
	}
	else if (TwoBitFlags && ExtendedFlags[1] & 0x01)
	{
		AddData(buffer, char(0x05), index);
	}

	if (Flags[0] & 0x40)	//ExtendedFlags[1] & 0x02
	{
		AddData(buffer, Data->CategoryID, index);
	}

	memset(Flags,0,sizeof(Flags));
}

/******************************
*         GET METHODS         *
******************************/

_Category * AuxCategory::GetData()  {return Data;}

char * AuxCategory::GetName()       {return Data->Name;}
u32 AuxCategory::GetCategoryID()    {return Data->CategoryID;}

/******************************
*         SET METHODS         *
******************************/

void AuxCategory::SetData(_Category * NewData)
{
	ReplaceString(Data->Name, NewData->Name, 0);
    SubCategories.SetData(&NewData->SubCategories);
	ReplaceData(Data->CategoryID, NewData->CategoryID, 2);

	CheckData();
}

void AuxCategory::SetName(char * NewName)
{
	ReplaceString(Data->Name, NewName, 0);
	CheckData();
}

void AuxCategory::SetCategoryID(u32 NewCategoryID)
{
	ReplaceData(Data->CategoryID, NewCategoryID, 2);
	CheckData();
}

/******************************
*     PARENT FLAG METHODS     *
******************************/

void AuxCategory::CheckData()
{
	SetParentExtendedFlag(HasData());
}

int AuxCategory::HasData()
{
	return (ExtendedFlags[0] & 0x70);
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxCategory::ClearFlags()
{
	memset(Flags,0,sizeof(Flags));
    SubCategories.ClearFlags();
}