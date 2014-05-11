#include "AuxMounts.h"

void AuxMounts::BuildPacket(unsigned char *buffer, long &index)
{
	AddFlags(Flags, sizeof(Flags), buffer, index);

	for (int i=0;i<20;i++)
	{
		if (CheckFlagBit(i))
		{
			AddData(buffer, Data->Mount[i], index);
			AddData(buffer, s32(-1), index);
			AddData(buffer, s32(-1), index);
		}
	}

	memset(Flags,0,sizeof(Flags));
}

void AuxMounts::BuildExtendedPacket(unsigned char *buffer, long &index)
{
	AddFlags(ExtendedFlags, sizeof(ExtendedFlags), buffer, index);

	for (int i=0;i<20;i++)
	{
		if (CheckExtendedFlagBit1(i))
		{
			AddData(buffer, Data->Mount[i], index);
			AddData(buffer, s32(-1), index);
			AddData(buffer, s32(-1), index);
		}
	}
}

/******************************
*         GET METHODS         *
******************************/

_Mounts * AuxMounts::GetData()					{return Data;}

u32 AuxMounts::GetMount(unsigned int Index)		{return Data->Mount[Index];}

/******************************
*         SET METHODS         *
******************************/

void AuxMounts::SetData(_Mounts *NewData)
{
	for (int i=0;i<20;i++)
	{
		ReplaceData(Data->Mount[i], NewData->Mount[i], i);
	}
}

void AuxMounts::SetMount(unsigned int Index, u32 NewMount)
{
	ReplaceData(Data->Mount[Index], NewMount, Index);
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxMounts::ClearFlags()
{
	memset(Flags,0,sizeof(Flags));
}