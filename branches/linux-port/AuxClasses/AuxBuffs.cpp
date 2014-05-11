#include "AuxBuffs.h"

void AuxBuffs::BuildPacket(unsigned char *buffer, long &index)
{
	AddFlags(Flags, sizeof(Flags), buffer, index);

	for (int i=0;i<16;i++)
	{
		if (CheckFlagBit(i))
		{
			Buff[i].BuildPacket(buffer, index);
		}
	}

	memset(Flags,0,sizeof(Flags));
}

void AuxBuffs::BuildExtendedPacket(unsigned char *buffer, long &index)
{
	AddFlags(ExtendedFlags, sizeof(ExtendedFlags), buffer, index);

    for (int i=0;i<16;i++)
    {
	    if (CheckExtendedFlagBit1(i))
	    {
		    Buff[i].BuildExtendedPacket(buffer, index);
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

_Buffs * AuxBuffs::GetData()    {return Data;}

/******************************
*         SET METHODS         *
******************************/

void AuxBuffs::SetData(_Buffs *NewData)
{
	for (int i=0;i<16;i++)
	{
		Buff[i].SetData(&NewData->Buff[i]);
	}
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxBuffs::ClearFlags()
{
	memset(Flags,0,sizeof(Flags));

	for (int i=0;i<16;i++)
	{
        Buff[i].ClearFlags();
	}
}