#include "AuxInventory6.h"

void AuxInventory6::BuildPacket(unsigned char *buffer, long &index)
{
	AddFlags(Flags, sizeof(Flags), buffer, index);

	for (int i=0;i<6;i++)
	{
		if (CheckFlagBit(i))
		{
			Item[i].BuildPacket(buffer, index);
		}
	}

	memset(Flags,0,sizeof(Flags));
}

void AuxInventory6::BuildExtendedPacket(unsigned char *buffer, long &index)
{
	AddFlags(ExtendedFlags, sizeof(ExtendedFlags), buffer, index);

	for (int i=0;i<6;i++)
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

_Inventory6 * AuxInventory6::GetData()  {return Data;}

/******************************
*         SET METHODS         *
******************************/

void AuxInventory6::SetData(_Inventory6 *NewData)
{
	for (int i=0;i<6;i++)
	{
		Item[i].SetData(&NewData->Item[i]);
	}
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxInventory6::Clear()
{
	for (int i=0;i<6;i++)
	{
		Item[i].Clear();
	}
}

void AuxInventory6::Empty()
{
	for (int i=0;i<6;i++)
	{
		Item[i].Empty();
	}
}

void AuxInventory6::ClearFlags()
{
	memset(Flags,0,sizeof(Flags));

	for (int i=0;i<6;i++)
	{
        Item[i].ClearFlags();
	}
}