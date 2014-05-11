#include "AuxFactions.h"

void AuxFactions::BuildPacket(unsigned char *buffer, long &index)
{
	AddFlags(Flags, sizeof(Flags), buffer, index);

	for (int i=0;i<32;i++)
	{
		if (CheckFlagBit(i))
		{
			Faction[i].BuildPacket(buffer, index);
		}
	}

	memset(Flags,0,sizeof(Flags));
}

void AuxFactions::BuildExtendedPacket(unsigned char *buffer, long &index)
{
	AddFlags(ExtendedFlags, sizeof(ExtendedFlags), buffer, index);

	for (int i=0;i<32;i++)
	{
		if (CheckExtendedFlagBit1(i))
		{
			Faction[i].BuildExtendedPacket(buffer, index);
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

_Factions * AuxFactions::GetData()  {return Data;}

/******************************
*         SET METHODS         *
******************************/

void AuxFactions::SetData(_Factions *NewData)
{
	for (int i=0;i<32;i++)
	{
		Faction[i].SetData(&NewData->Faction[i]);
	}
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxFactions::ClearFlags()
{
	memset(Flags,0,sizeof(Flags));

	for (int i=0;i<32;i++)
	{
        Faction[i].ClearFlags();
	}
}