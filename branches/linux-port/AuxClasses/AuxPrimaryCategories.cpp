#include "AuxPrimaryCategories.h"

void AuxPrimaryCategories::BuildPacket(unsigned char *buffer, long &index, bool TwoBitFlags)
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
		PrimaryCategory[0].BuildPacket(buffer, index, TwoBitFlags);
	}
	else if (TwoBitFlags && ExtendedFlags[0] & 0x40)
	{
		AddData(buffer, char(0x05), index);
	}

	if (Flags[0] & 0x20)	//ExtendedFlags[0] & 0x80
	{
		PrimaryCategory[1].BuildPacket(buffer, index, TwoBitFlags);
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

_PrimaryCategories * AuxPrimaryCategories::GetData()    {return Data;}

/******************************
*         SET METHODS         *
******************************/

void AuxPrimaryCategories::SetData(_PrimaryCategories *NewData)
{
	PrimaryCategory[0].SetData(&NewData->PrimaryCategory[0]);
	PrimaryCategory[1].SetData(&NewData->PrimaryCategory[1]);

    CheckData();
}

/******************************
*     PARENT FLAG METHODS     *
******************************/

void AuxPrimaryCategories::CheckData()
{
	SetParentExtendedFlag(HasData());
}

int AuxPrimaryCategories::HasData()
{
	return (ExtendedFlags[0] & 0x30);
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxPrimaryCategories::ClearFlags()
{
	memset(Flags,0,sizeof(Flags));

	for (int i=0;i<2;i++)
	{
        PrimaryCategory[i].ClearFlags();
	}
}