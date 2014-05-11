#include "AuxMissions.h"

void AuxMissions::BuildPacket(unsigned char *buffer, long &index)
{
	AddFlags(Flags, sizeof(Flags), buffer, index);

	for (int i=0;i<12;i++)
	{
		if (CheckFlagBit(i))
		{
			Mission[i].BuildPacket(buffer, index);
		}
	}

	memset(Flags,0,sizeof(Flags));
}

void AuxMissions::BuildExtendedPacket(unsigned char *buffer, long &index)
{
	AddFlags(ExtendedFlags, sizeof(ExtendedFlags), buffer, index);

	for (int i=0;i<12;i++)
	{
		if (CheckExtendedFlagBit1(i))
		{
			Mission[i].BuildExtendedPacket(buffer, index);
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

_Missions * AuxMissions::GetData()  {return Data;}

/******************************
*         SET METHODS         *
******************************/

void AuxMissions::SetData(_Missions *NewData)
{
	for (int i=0;i<12;i++)
	{
		Mission[i].SetData(&NewData->Mission[i]);
	}
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxMissions::ClearFlags()
{
	memset(Flags,0,sizeof(Flags));

	for (int i=0;i<12;i++)
	{
        Mission[i].ClearFlags();
	}
}