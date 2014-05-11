#include "AuxSecureInventory.h"

void AuxSecureInv::BuildPacket(unsigned char *buffer, long &index)
{
	AddFlags(Flags, sizeof(Flags), buffer, index);

	for (int i=0;i<96;i++)
	{
		if (CheckFlagBit(i))
		{
			Item[i].BuildPacket(buffer, index);
		}
	}

	memset(Flags,0,sizeof(Flags));
}

void AuxSecureInv::BuildExtendedPacket(unsigned char *buffer, long &index)
{
	AddFlags(ExtendedFlags, sizeof(ExtendedFlags), buffer, index);

	for (int i=0;i<96;i++)
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

_SecureInv * AuxSecureInv::GetData()      {return Data;}

/******************************
*         SET METHODS         *
******************************/

void AuxSecureInv::SetData(_SecureInv *NewData)
{
	for (int i=0;i<96;i++)
	{
		Item[i].SetData(&NewData->Item[i]);
	}
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxSecureInv::ClearFlags()
{
	memset(Flags,0,sizeof(Flags));

	for (int i=0;i<96;i++)
	{
        Item[i].ClearFlags();
	}
}