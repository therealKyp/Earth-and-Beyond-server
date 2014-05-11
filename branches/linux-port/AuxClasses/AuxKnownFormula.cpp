#include "AuxKnownFormula.h"

void AuxKnownFormula::BuildPacket(unsigned char *buffer, long &index, bool TwoBitFlags)
{
	AddFlags(Flags, sizeof(Flags), buffer, index);

	if (Flags[0] & 0x10)	//ExtendedFlags[0] & 0x80
	{
		AddString(buffer, Data->ItemName, index);
	}

	if (Flags[0] & 0x20)	//ExtendedFlags[1] & 0x01
	{
		AddData(buffer, Data->ItemID, index);
	}

	if (Flags[0] & 0x40)	//ExtendedFlags[1] & 0x02
	{
		AddData(buffer, Data->TechLevel, index);
	}

	memset(Flags,0,sizeof(Flags));
}

/******************************
*         GET METHODS         *
******************************/

_KnownFormula * AuxKnownFormula::GetData()			{return Data;}

char * AuxKnownFormula::GetItemName()               {return Data->ItemName;}
u32 AuxKnownFormula::GetItemID()                    {return Data->ItemID;}
u32 AuxKnownFormula::GetTechLevel()                 {return Data->TechLevel;}

/******************************
*         SET METHODS         *
******************************/

void AuxKnownFormula::SetData(_KnownFormula * NewData)
{
	ReplaceString(Data->ItemName, NewData->ItemName, 0);
	ReplaceData(Data->ItemID, NewData->ItemID, 1);
	ReplaceData(Data->TechLevel, NewData->TechLevel, 2);
}

void AuxKnownFormula::SetItemName(const char * NewItemName)
{
	ReplaceString(Data->ItemName, NewItemName, 0);
}

void AuxKnownFormula::SetItemID(u32 NewItemID)
{
	ReplaceData(Data->ItemID, NewItemID, 1);
}

void AuxKnownFormula::SetTechLevel(u32 NewTechLevel)
{
	ReplaceData(Data->TechLevel, NewTechLevel, 2);
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxKnownFormula::ClearFlags()
{
    memset(Flags, 0, sizeof(Flags));
}
