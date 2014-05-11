#include "AuxOverflowInventory.h"

void AuxOverflowInv::BuildPacket(unsigned char *buffer, long &index)
{
	AddFlags(Flags, sizeof(Flags), buffer, index);

	for (int i=0;i<8;i++)
	{
		if (CheckFlagBit(i))
		{
			Item[i].BuildPacket(buffer, index);
		}
	}

	memset(Flags,0,sizeof(Flags));
}

void AuxOverflowInv::BuildExtendedPacket(unsigned char *buffer, long &index)
{
	AddFlags(ExtendedFlags, sizeof(ExtendedFlags), buffer, index);

	for (int i=0;i<8;i++)
	{
		if (CheckExtendedFlagBit1(i))
		{
			Item[i].BuildExtendedPacket(buffer, index);
		}
		else if (CheckExtendedFlagBit2(i))
		{
			AddData(buffer, char(0x05), index);
		}
	}
}

/******************************
*         GET METHODS         *
******************************/

_OverflowInv * AuxOverflowInv::GetData()      {return Data;}

/******************************
*         SET METHODS         *
******************************/

void AuxOverflowInv::SetData(_OverflowInv *NewData)
{
	for (int i=0;i<8;i++)
	{
		Item[i].SetData(&NewData->Item[i]);
	}
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxOverflowInv::ClearFlags()
{
	memset(Flags,0,sizeof(Flags));

	for (int i=0;i<8;i++)
	{
        Item[i].ClearFlags();
	}
}