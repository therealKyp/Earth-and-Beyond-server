#include "AuxSkillAbilities.h"

void AuxSkillAbilities::BuildPacket(unsigned char *buffer, long &index)
{
	AddFlags(Flags, sizeof(Flags), buffer, index);

	for (int i=0;i<170;i++)
	{
		if (CheckFlagBit(i))
		{
			Ability[i].BuildPacket(buffer, index);
		}
	}

	memset(Flags,0,sizeof(Flags));
}

void AuxSkillAbilities::BuildExtendedPacket(unsigned char *buffer, long &index)
{
	AddFlags(ExtendedFlags, sizeof(ExtendedFlags), buffer, index);

	for (int i=0;i<170;i++)
	{
		if (CheckExtendedFlagBit1(i))
		{
			Ability[i].BuildExtendedPacket(buffer, index);
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

_SkillAbilities * AuxSkillAbilities::GetData()  {return Data;}

/******************************
*         SET METHODS         *
******************************/

void AuxSkillAbilities::SetData(_SkillAbilities *NewData)
{
	for (int i=0;i<170;i++)
	{
		Ability[i].SetData(&NewData->Ability[i]);
	}
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxSkillAbilities::ClearFlags()
{
	memset(Flags,0,sizeof(Flags));

	for (int i=0;i<170;i++)
	{
        Ability[i].ClearFlags();
	}
}