#include "AuxRewardInventory.h"

void AuxRewardInv::BuildPacket(unsigned char *buffer, long &index)
{
	AddFlags(Flags, sizeof(Flags), buffer, index);

	for (int i=0;i<2;i++)
	{
		if (CheckFlagBit(i))
		{
			Item[i].BuildPacket(buffer, index);
		}
	}

	memset(Flags,0,sizeof(Flags));
}

void AuxRewardInv::BuildExtendedPacket(unsigned char *buffer, long &index)
{
	AddFlags(ExtendedFlags, sizeof(ExtendedFlags), buffer, index);

	for (int i=0;i<2;i++)
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

_RewardInv * AuxRewardInv::GetData()      {return Data;}

/******************************
*         SET METHODS         *
******************************/

void AuxRewardInv::SetData(_RewardInv *NewData)
{
	Item[0].SetData(&NewData->Item[0]);
	Item[1].SetData(&NewData->Item[1]);
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxRewardInv::ClearFlags()
{
	memset(Flags,0,sizeof(Flags));

	for (int i=0;i<2;i++)
	{
        Item[i].ClearFlags();
	}
}