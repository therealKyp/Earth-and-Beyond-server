#include "AuxInventory40.h"

void AuxInventory40::BuildPacket(unsigned char *buffer, long &index)
{
	AddFlags(Flags, sizeof(Flags), buffer, index);

	for (int i=0;i<40;i++)
	{
		if (CheckFlagBit(i))
		{
			Item[i].BuildPacket(buffer, index);
		}
	}

	memset(Flags,0,sizeof(Flags));
}

void AuxInventory40::BuildExtendedPacket(unsigned char *buffer, long &index)
{
	AddFlags(ExtendedFlags, sizeof(ExtendedFlags), buffer, index);

	for (int i=0;i<40;i++)
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

_Inventory40 * AuxInventory40::GetData()      {return Data;}

/******************************
*         SET METHODS         *
******************************/

void AuxInventory40::SetData(_Inventory40 *NewData)
{
	for (int i=0;i<40;i++)
	{
		Item[i].SetData(&NewData->Item[i]);
	}
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxInventory40::Clear()
{
	for (int i=0;i<40;i++)
	{
		Item[i].Clear();
	}
}

void AuxInventory40::ClearFlags()
{
	memset(Flags,0,sizeof(Flags));

	for (int i=0;i<40;i++)
	{
        Item[i].ClearFlags();
	}
}