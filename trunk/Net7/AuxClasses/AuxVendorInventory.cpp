#include "AuxVendorInventory.h"

void AuxVendorInv::BuildPacket(unsigned char *buffer, long &index)
{
	AddFlags(Flags, sizeof(Flags), buffer, index);

	for (int i=0;i<128;i++)
	{
		if (CheckFlagBit(i))
		{
			Item[i].BuildPacket(buffer, index);
		}
	}

	memset(Flags,0,sizeof(Flags));
}

void AuxVendorInv::BuildExtendedPacket(unsigned char *buffer, long &index)
{
	AddFlags(ExtendedFlags, sizeof(ExtendedFlags), buffer, index);

	for (int i=0;i<128;i++)
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

_VendorInv * AuxVendorInv::GetData()      {return Data;}

/******************************
*         SET METHODS         *
******************************/

void AuxVendorInv::SetData(_VendorInv *NewData)
{
	for (int i=0;i<128;i++)
	{
		Item[i].SetData(&NewData->Item[i]);
	}
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxVendorInv::Clear()
{
	for (int i=0;i<128;i++)
	{
		Item[i].Clear();
	}
}

void AuxVendorInv::ClearFlags()
{
	memset(Flags,0,sizeof(Flags));

	for (int i=0;i<128;i++)
	{
        Item[i].ClearFlags();
	}
}