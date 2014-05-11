#include "AuxCategories.h"

void AuxCategories::BuildPacket(unsigned char *buffer, long &index, bool TwoBitFlags)
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

	for (int i=0;i<5;i++)
	{
		if (CheckFlagBit(i))
		{
			Category[i].BuildPacket(buffer, index, TwoBitFlags);
		}
		else if (TwoBitFlags && CheckExtendedFlagBit2(i))
		{
			AddData(buffer, char(0x05), index);
		}
	}

	memset(Flags,0,sizeof(Flags));
}

/******************************
*         GET METHODS         *
******************************/

_Categories * AuxCategories::GetData()  {return Data;}

/******************************
*         SET METHODS         *
******************************/

void AuxCategories::SetData(_Categories *NewData)
{
	for (int i=0;i<5;i++)
	{
		Category[i].SetData(&NewData->Category[i]);
	}

    CheckData();
}

/******************************
*     PARENT FLAG METHODS     *
******************************/

void AuxCategories::CheckData()
{
	SetParentExtendedFlag(HasData());
}

int AuxCategories::HasData()
{
	return (ExtendedFlags[0] & 0xF0 || ExtendedFlags[1] & 0x01);
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxCategories::ClearFlags()
{
	memset(Flags,0,sizeof(Flags));

	for (int i=0;i<5;i++)
	{
        Category[i].ClearFlags();
	}
}