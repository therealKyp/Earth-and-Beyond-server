#include "AuxLego.h"

void AuxLego::BuildPacket(unsigned char *buffer, long &index)
{
	AddFlags(Flags, sizeof(Flags), buffer, index);

	if (Flags[0] & 0x10)
	{
		AddData(buffer, Data->Scale, index);
	}

	if (Flags[0] & 0x20)
	{
		Attachments.BuildPacket(buffer, index);
	}
	
    memset(Flags,0,sizeof(Flags));
}

void AuxLego::BuildExtendedPacket(unsigned char *buffer, long &index)
{
	AddFlags(ExtendedFlags, sizeof(ExtendedFlags), buffer, index);

	if (ExtendedFlags[0] & 0x10)
	{
		AddData(buffer, Data->Scale, index);
	}

	if (ExtendedFlags[0] & 0x20)
	{
		Attachments.BuildExtendedPacket(buffer, index);
	}
	else if (ExtendedFlags[0] & 0x80)
	{
		AddData(buffer, char(0x05), index);
	}
}

void AuxLego::BuildSpecialPacket(unsigned char *buffer, long &index)
{
	AddFlags(Flags, sizeof(Flags), buffer, index);

	if (Flags[0] & 0x10)
	{
		AddData(buffer, Data->Scale, index);
	}

	if (Flags[0] & 0x20)
	{
		Attachments.BuildSpecialPacket(buffer, index);
	}
}

/******************************
*         GET METHODS         *
******************************/

_Lego * AuxLego::GetData()      {return Data;}

float AuxLego::GetScale()		{return Data->Scale;}

/******************************
*         SET METHODS         *
******************************/

void AuxLego::SetData(_Lego *NewData)
{
	ReplaceData(Data->Scale, NewData->Scale, 0);
	Attachments.SetData(&NewData->Attachments);
}

void AuxLego::SetScale(float NewScale)
{
	ReplaceData(Data->Scale, NewScale, 0);
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxLego::ClearFlags()
{
	memset(Flags,0,sizeof(Flags));
    Attachments.ClearFlags();
}