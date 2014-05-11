#include "AuxSubCategories.h"

void AuxSubCategories::BuildPacket(unsigned char *buffer, long &index, bool TwoBitFlags)
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
			SubCategory[i].BuildPacket(buffer, index, TwoBitFlags);
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

_SubCategories * AuxSubCategories::GetData()    {return Data;}

/******************************
*         SET METHODS         *
******************************/

void AuxSubCategories::SetData(_SubCategories *NewData)
{
	for (int i=0;i<5;i++)
	{
		SubCategory[i].SetData(&NewData->SubCategory[i]);
	}

    CheckData();
}

/******************************
*     PARENT FLAG METHODS     *
******************************/

void AuxSubCategories::CheckData()
{
	SetParentExtendedFlag(HasData());
}

int AuxSubCategories::HasData()
{
	return (ExtendedFlags[0] & 0xF0 || ExtendedFlags[1] & 0x01);
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxSubCategories::ClearFlags()
{
	memset(Flags,0,sizeof(Flags));

	for (int i=0;i<5;i++)
	{
        SubCategory[i].ClearFlags();
	}
}