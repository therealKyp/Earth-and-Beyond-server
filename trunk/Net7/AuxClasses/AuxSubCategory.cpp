#include "AuxSubCategory.h"

void AuxSubCategory::BuildPacket(unsigned char *buffer, long &index, bool TwoBitFlags)
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
		AddData(buffer, Data->SubCategoryID, index);
	}

	if (Flags[0] & 0x40)	//ExtendedFlags[1] & 0x02
	{
		AddData(buffer, char(Data->IsVisible), index);
	}

	memset(Flags,0,sizeof(Flags));
}

/******************************
*         GET METHODS         *
******************************/

_SubCategory * AuxSubCategory::GetData()		{return Data;}

char * AuxSubCategory::GetName()                {return Data->Name;}
u32 AuxSubCategory::GetSubCategoryID()          {return Data->SubCategoryID;}
bool AuxSubCategory::GetIsVisible()             {return Data->IsVisible;}

/******************************
*         SET METHODS         *
******************************/

void AuxSubCategory::SetData(_SubCategory * NewData)
{
	ReplaceString(Data->Name, NewData->Name, 0);
	ReplaceData(Data->SubCategoryID, NewData->SubCategoryID, 1);
	ReplaceData(Data->IsVisible, NewData->IsVisible, 2);

	CheckData();
}

void AuxSubCategory::SetName(char * NewName)
{
	ReplaceString(Data->Name, NewName, 0);
	CheckData();
}

void AuxSubCategory::SetSubCategoryID(u32 NewSubCategoryID)
{
	ReplaceData(Data->SubCategoryID, NewSubCategoryID, 1);
	CheckData();
}

void AuxSubCategory::SetIsVisible(bool NewIsVisible)
{
	ReplaceData(Data->IsVisible, NewIsVisible, 2);
	CheckData();
}

/******************************
*     PARENT FLAG METHODS     *
******************************/

void AuxSubCategory::CheckData()
{
	SetParentExtendedFlag(HasData());
}

int AuxSubCategory::HasData()
{
	return (ExtendedFlags[0] & 0x70);
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxSubCategory::ClearFlags()
{
	memset(Flags,0,sizeof(Flags));
}