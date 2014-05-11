#include "AuxSkills.h"

void AuxSkills::BuildPacket(unsigned char *buffer, long &index)
{
	AddFlags(Flags, sizeof(Flags), buffer, index);

	for (int i=0;i<64;i++)
	{
		if (CheckFlagBit(i))
		{
			Skill[i].BuildPacket(buffer, index);
		}
	}

	memset(Flags,0,sizeof(Flags));
}

void AuxSkills::BuildExtendedPacket(unsigned char *buffer, long &index)
{
	AddFlags(ExtendedFlags, sizeof(ExtendedFlags), buffer, index);

	for (int i=0;i<64;i++)
	{
		if (CheckExtendedFlagBit1(i))
		{
			Skill[i].BuildExtendedPacket(buffer, index);
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

_Skills * AuxSkills::GetData()  {return Data;}

/******************************
*         SET METHODS         *
******************************/

void AuxSkills::SetData(_Skills *NewData)
{
	for (int i=0;i<64;i++)
	{
		Skill[i].SetData(&NewData->Skill[i]);
	}
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxSkills::ClearFlags()
{
	memset(Flags,0,sizeof(Flags));

    for (int i=0;i<64;i++)
	{
		Skill[i].ClearFlags();
	}

}