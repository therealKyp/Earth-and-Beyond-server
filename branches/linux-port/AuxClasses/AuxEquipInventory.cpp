#include "AuxEquipInventory.h"

void AuxEquipInv::BuildPacket(unsigned char *buffer, long &index)
{
	AddFlags(Flags, sizeof(Flags), buffer, index);

	for (int i=0;i<20;i++)
	{
		if (CheckFlagBit(i))
		{
			EquipItem[i].BuildPacket(buffer, index);
		}
	}

	memset(Flags,0,sizeof(Flags));
}

void AuxEquipInv::BuildExtendedPacket(unsigned char *buffer, long &index)
{
	AddFlags(ExtendedFlags, sizeof(ExtendedFlags), buffer, index);

	for (int i=0;i<20;i++)
	{
		if (CheckExtendedFlagBit1(i))
		{
			EquipItem[i].BuildExtendedPacket(buffer, index);
		}
		else if (CheckExtendedFlagBit2(i))
		{
			AddData(buffer, char(0x05), index);
		}
	}
}

/*******************************
*		   GET METHODS		   *
*******************************/

_EquipInv * AuxEquipInv::GetData()      {return Data;}

/*******************************
*		   SET METHODS		   *
*******************************/

void AuxEquipInv::SetData(_EquipInv *NewData)
{
	for (int i=0;i<20;i++)
	{
		EquipItem[i].SetData(&NewData->EquipItem[i]);
	}
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxEquipInv::ClearFlags()
{
	memset(Flags,0,sizeof(Flags));

	for (int i=0;i<20;i++)
	{
        EquipItem[i].ClearFlags();
	}
}