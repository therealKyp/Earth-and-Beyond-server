#include "AuxQuadrantDamage.h"

void AuxQuadrantDamage::BuildPacket(unsigned char *buffer, long &index)
{
	AddFlags(Flags, sizeof(Flags), buffer, index);

	if (Flags[0] & 0x10)	//ExtendedFlags[1] & 0x01
	{
		AddData(buffer, Data->Slot1, index);
	}

	if (Flags[0] & 0x20)	//ExtendedFlags[1] & 0x02
	{
		AddData(buffer, Data->Slot2, index);
	}

	if (Flags[0] & 0x40)	//ExtendedFlags[1] & 0x04
	{
		AddData(buffer, Data->Slot3, index);
	}

	if (Flags[0] & 0x80)	//ExtendedFlags[1] & 0x08
	{
		AddData(buffer, Data->Slot4, index);
	}

	memset(Flags,0,sizeof(Flags));
}

void AuxQuadrantDamage::BuildExtendedPacket(unsigned char *buffer, long &index)
{
	AddFlags(ExtendedFlags, sizeof(ExtendedFlags), buffer, index);

	if (ExtendedFlags[0] & 0x10)	//ExtendedFlags[1] & 0x01
	{
		AddData(buffer, Data->Slot1, index);
	}

	if (ExtendedFlags[0] & 0x20)	//ExtendedFlags[1] & 0x02
	{
		AddData(buffer, Data->Slot2, index);
	}

	if (ExtendedFlags[0] & 0x40)	//ExtendedFlags[1] & 0x04
	{
		AddData(buffer, Data->Slot3, index);
	}

	if (ExtendedFlags[0] & 0x80)	//ExtendedFlags[1] & 0x08
	{
		AddData(buffer, Data->Slot4, index);
	}
}

void AuxQuadrantDamage::BuildSpecialPacket(unsigned char *buffer, long &index)
{
	AddFlags(Flags, sizeof(Flags), buffer, index);

	if (Flags[0] & 0x10)	//ExtendedFlags[1] & 0x01
	{
		AddData(buffer, Data->Slot1, index);
	}

	if (Flags[0] & 0x20)	//ExtendedFlags[1] & 0x02
	{
		AddData(buffer, Data->Slot2, index);
	}

	if (Flags[0] & 0x40)	//ExtendedFlags[1] & 0x04
	{
		AddData(buffer, Data->Slot3, index);
	}

	if (Flags[0] & 0x80)	//ExtendedFlags[1] & 0x08
	{
		AddData(buffer, Data->Slot4, index);
	}
}

/******************************
*         GET METHODS         *
******************************/

_QuadrantDamage * AuxQuadrantDamage::GetData()			{return Data;}

float AuxQuadrantDamage::GetSlot1()						{return Data->Slot1;}
float AuxQuadrantDamage::GetSlot2()						{return Data->Slot2;}
float AuxQuadrantDamage::GetSlot3()						{return Data->Slot3;}
float AuxQuadrantDamage::GetSlot4()						{return Data->Slot4;}

/******************************
*         SET METHODS         *
******************************/

void AuxQuadrantDamage::SetData(_QuadrantDamage *NewData)
{
	ReplaceData(Data->Slot1, NewData->Slot1, 0);
	ReplaceData(Data->Slot2, NewData->Slot2, 1);
	ReplaceData(Data->Slot3, NewData->Slot3, 2);
	ReplaceData(Data->Slot4, NewData->Slot4, 3);
}

void AuxQuadrantDamage::SetSlot1(float NewSlot1)
{
	ReplaceData(Data->Slot1, NewSlot1, 0);
}

void AuxQuadrantDamage::SetSlot2(float NewSlot2)
{
	ReplaceData(Data->Slot2, NewSlot2, 1);
}

void AuxQuadrantDamage::SetSlot3(float NewSlot3)
{
	ReplaceData(Data->Slot3, NewSlot3, 2);
}

void AuxQuadrantDamage::SetSlot4(float NewSlot4)
{
	ReplaceData(Data->Slot4, NewSlot4, 3);
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxQuadrantDamage::Clear()
{
    _QuadrantDamage ClearData;
    
	ClearData.Slot1 = 0;
	ClearData.Slot2 = 0;
	ClearData.Slot3 = 0;
	ClearData.Slot4 = 0;

    SetData(&ClearData);
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxQuadrantDamage::ClearFlags()
{
	memset(Flags,0,sizeof(Flags));
}