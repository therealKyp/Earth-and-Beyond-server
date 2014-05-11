#include "AuxInventory20.h"

void AuxInventory20::BuildPacket(unsigned char *buffer, long &index)
{
	AddFlags(Flags, sizeof(Flags), buffer, index);

	for (int i=0;i<20;i++)
	{
		if (CheckFlagBit(i))
		{
			Item[i].BuildPacket(buffer, index);
		}
	}

	memset(Flags,0,sizeof(Flags));
}

void AuxInventory20::BuildExtendedPacket(unsigned char *buffer, long &index)
{
	AddFlags(ExtendedFlags, sizeof(ExtendedFlags), buffer, index);

	for (int i=0;i<20;i++)
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

_Inventory20 * AuxInventory20::GetData()      {return Data;}

/******************************
*         SET METHODS         *
******************************/

void AuxInventory20::SetData(_Inventory20 *NewData)
{
	for (int i=0;i<20;i++)
	{
		Item[i].SetData(&NewData->Item[i]);
	}
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxInventory20::Clear()
{
	for (int i=0;i<20;i++)
	{
		Item[i].Clear();
	}
}

void AuxInventory20::ClearFlags()
{
	memset(Flags,0,sizeof(Flags));

	for (int i=0;i<20;i++)
	{
        Item[i].ClearFlags();
	}
}