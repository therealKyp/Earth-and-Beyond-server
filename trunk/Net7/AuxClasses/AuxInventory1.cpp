#include "AuxInventory1.h"

void AuxInventory1::BuildPacket(unsigned char *buffer, long &index)
{
	AddFlags(Flags, sizeof(Flags), buffer, index);

	if (Flags[0] & 0x10)
	{
	    Item[0].BuildPacket(buffer, index);
	}

	memset(Flags,0,sizeof(Flags));
}

void AuxInventory1::BuildExtendedPacket(unsigned char *buffer, long &index)
{
	AddFlags(ExtendedFlags, sizeof(ExtendedFlags), buffer, index);

	if (ExtendedFlags[0] & 0x10)
	{
	    Item[0].BuildExtendedPacket(buffer, index);
	}
	else if (ExtendedFlags[0] & 0x20)
	{
		AddData(buffer, char(0x05), index);
	}
}

/******************************
*         GET METHODS         *
******************************/

_Inventory1 * AuxInventory1::GetData()      {return Data;}

/******************************
*         SET METHODS         *
******************************/

void AuxInventory1::SetData(_Inventory1 *NewData)
{
	Item[0].SetData(&NewData->Item[0]);
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxInventory1::Empty()
{
	Item[0].Empty();
}

void AuxInventory1::ClearFlags()
{
	memset(Flags,0,sizeof(Flags));
    Item[0].ClearFlags();
}