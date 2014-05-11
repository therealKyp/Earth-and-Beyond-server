#include "AuxMountBoneNames.h"

void AuxMountBoneNames::BuildPacket(unsigned char *buffer, long &index)
{
	AddFlags(Flags, sizeof(Flags), buffer, index);

	for (int i=0;i<20;i++)
	{
		if (CheckFlagBit(i))
		{
			AddString(buffer, Data->MountBoneName[i], index);
		}
	}

	memset(Flags,0,sizeof(Flags));
}

void AuxMountBoneNames::BuildExtendedPacket(unsigned char *buffer, long &index)
{
	AddFlags(ExtendedFlags, sizeof(ExtendedFlags), buffer, index);

	for (int i=0;i<20;i++)
	{
		if (CheckExtendedFlagBit1(i))
		{
			AddString(buffer, Data->MountBoneName[i], index);
		}
	}
}

/******************************
*         GET METHODS         *
******************************/

_MountBones * AuxMountBoneNames::GetData()					        {return Data;}

char * AuxMountBoneNames::GetMountBoneName(unsigned int Index)		{return Data->MountBoneName[Index];}

/******************************
*         SET METHODS         *
******************************/

void AuxMountBoneNames::SetData(_MountBones *NewData)
{
	for (int i=0;i<20;i++)
	{
		ReplaceString(Data->MountBoneName[i], NewData->MountBoneName[i], i);
	}
}

void AuxMountBoneNames::SetMountBoneName(unsigned int Index, const char *NewMount)
{
	ReplaceString(Data->MountBoneName[Index], NewMount, Index);
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxMountBoneNames::ClearFlags()
{
	memset(Flags,0,sizeof(Flags));
}
