#include "AuxItem.h"

void AuxItem::BuildPacket(unsigned char *buffer, long &index)
{
	AddFlags(Flags, sizeof(Flags), buffer, index);

	if (Flags[0] & 0x10)	//ExtendedFlags[1] & 0x40
	{
		AddData(buffer, Data->ItemTemplateID, index);
	}

	if (Flags[0] & 0x20)	//ExtendedFlags[1] & 0x80
	{
		AddData(buffer, Data->StackCount, index);
	}

	if (Flags[0] & 0x40)	//ExtendedFlags[2] & 0x01
	{
		AddData(buffer, Data->Price, index);
	}

	if (Flags[0] & 0x80)	//ExtendedFlags[2] & 0x02
	{
		AddData(buffer, Data->AveCost, index);
	}

	if (Flags[1] & 0x01)	//ExtendedFlags[2] & 0x04
	{
		AddData(buffer, Data->Structure, index);
	}

	if (Flags[1] & 0x02)	//ExtendedFlags[2] & 0x08
	{
		AddData(buffer, Data->Quality, index);
	}

	if (Flags[1] & 0x04)	//ExtendedFlags[2] & 0x10
	{
		AddString(buffer, Data->InstanceInfo, index);
	}

	if (Flags[1] & 0x08)	//ExtendedFlags[2] & 0x20
	{
		AddString(buffer, Data->ActivatedEffectInstanceInfo, index);
	}

	if (Flags[1] & 0x10)	//ExtendedFlags[2] & 0x40
	{
		AddString(buffer, Data->EquipEffectInstanceInfo, index);
	}

	if (Flags[1] & 0x20)	//ExtendedFlags[2] & 0x80
	{
		AddString(buffer, Data->BuilderName, index);
	}

	memset(Flags,0,sizeof(Flags));
}

void AuxItem::BuildExtendedPacket(unsigned char *buffer, long &index)
{
	AddFlags(ExtendedFlags, sizeof(ExtendedFlags), buffer, index);

	if (ExtendedFlags[0] & 0x10)	//ExtendedFlags[1] & 0x40
	{
		AddData(buffer, Data->ItemTemplateID, index);
	}

	if (ExtendedFlags[0] & 0x20)	//ExtendedFlags[1] & 0x80
	{
		AddData(buffer, Data->StackCount, index);
	}

	if (ExtendedFlags[0] & 0x40)	//ExtendedFlags[2] & 0x01
	{
		AddData(buffer, Data->Price, index);
	}

	if (ExtendedFlags[0] & 0x80)	//ExtendedFlags[2] & 0x02
	{
		AddData(buffer, Data->AveCost, index);
	}

	if (ExtendedFlags[1] & 0x01)	//ExtendedFlags[2] & 0x04
	{
		AddData(buffer, Data->Structure, index);
	}

	if (ExtendedFlags[1] & 0x02)	//ExtendedFlags[2] & 0x08
	{
		AddData(buffer, Data->Quality, index);
	}

	if (ExtendedFlags[1] & 0x04)	//ExtendedFlags[2] & 0x10
	{
		AddString(buffer, Data->InstanceInfo, index);
	}

	if (ExtendedFlags[1] & 0x08)	//ExtendedFlags[2] & 0x20
	{
		AddString(buffer, Data->ActivatedEffectInstanceInfo, index);
	}

	if (ExtendedFlags[1] & 0x10)	//ExtendedFlags[2] & 0x40
	{
		AddString(buffer, Data->EquipEffectInstanceInfo, index);
	}

	if (ExtendedFlags[1] & 0x20)	//ExtendedFlags[2] & 0x80
	{
		AddString(buffer, Data->BuilderName, index);
	}
}

/******************************
*         GET METHODS         *
******************************/

_Item * AuxItem::GetData()							{return Data;}

s32 AuxItem::GetItemTemplateID()					{return Data->ItemTemplateID;}
u32 AuxItem::GetStackCount()						{return Data->StackCount;}
u64 AuxItem::GetPrice()								{return Data->Price;}
float AuxItem::GetAveCost()							{return Data->AveCost;}
float AuxItem::GetStructure()						{return Data->Structure;}
float AuxItem::GetQuality()							{return Data->Quality;}
char * AuxItem::GetInstanceInfo()					{return Data->InstanceInfo;}
char * AuxItem::GetActivatedEffectInstanceInfo()	{return Data->ActivatedEffectInstanceInfo;}
char * AuxItem::GetEquipEffectInstanceInfo()		{return Data->EquipEffectInstanceInfo;}
char * AuxItem::GetBuilderName()					{return Data->BuilderName;}
int AuxItem::GetTradeStack()                        {return Data->TradeStack;}

/******************************
*         SET METHODS         *
******************************/

void AuxItem::SetData(_Item * NewData)
{
	ReplaceData(Data->ItemTemplateID, NewData->ItemTemplateID, 0);
	ReplaceData(Data->StackCount, NewData->StackCount, 1);
	ReplaceData(Data->Price, NewData->Price, 2);
	ReplaceData(Data->AveCost, NewData->AveCost, 3);
	ReplaceData(Data->Structure, NewData->Structure, 4);
	ReplaceData(Data->Quality, NewData->Quality, 5);
	ReplaceString(Data->InstanceInfo, NewData->InstanceInfo, 6);
	ReplaceString(Data->ActivatedEffectInstanceInfo, NewData->ActivatedEffectInstanceInfo, 7);
	ReplaceString(Data->EquipEffectInstanceInfo, NewData->EquipEffectInstanceInfo, 8);
	ReplaceString(Data->BuilderName, NewData->BuilderName, 9);

    // Not part of the packet
    SetTradeStack(NewData->TradeStack);
}

void AuxItem::SetItemTemplateID(s32 NewItemTemplateID)
{
	ReplaceData(Data->ItemTemplateID, NewItemTemplateID, 0);
}

void AuxItem::SetStackCount(u32 NewStackCount)
{
	ReplaceData(Data->StackCount, NewStackCount, 1);
}

void AuxItem::SetPrice(u64 NewPrice)
{
	ReplaceData(Data->Price, NewPrice, 2);
}

void AuxItem::SetAveCost(float NewAveCost)
{
	ReplaceData(Data->AveCost, NewAveCost, 3);
}

void AuxItem::SetStructure(float NewStructure)
{
	ReplaceData(Data->Structure, NewStructure, 4);
}

void AuxItem::SetQuality(float NewQuality)
{
	ReplaceData(Data->Quality, NewQuality, 5);
}

void AuxItem::SetInstanceInfo(char *NewInstanceInfo)
{
	ReplaceString(Data->InstanceInfo, NewInstanceInfo, 6);
}

void AuxItem::SetActivatedEffectInstanceInfo(char * NewActivatedEffectInstanceInfo)
{
	ReplaceString(Data->ActivatedEffectInstanceInfo, NewActivatedEffectInstanceInfo, 7);
}

void AuxItem::SetEquipEffectInstanceInfo(char * NewEquipEffectInstanceInfo)
{
	ReplaceString(Data->EquipEffectInstanceInfo, NewEquipEffectInstanceInfo, 8);
}

void AuxItem::SetBuilderName(const char * NewBuilderName)
{
	ReplaceString(Data->BuilderName, NewBuilderName, 9);
}

void AuxItem::SetTradeStack(int NewTradeStack)
{
    // Sanitize input
    Data->TradeStack = ((NewTradeStack < 0  || NewTradeStack > 500) ? 0 : NewTradeStack);
}

void AuxItem::AddTradeStack(int Amount)
{
    SetTradeStack(Data->TradeStack + Amount);
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxItem::Clear()
{
    _Item ClearData;

    ClearData.ItemTemplateID = -2;	/* Invisible Item */
	ClearData.StackCount = 0;
	ClearData.Price = 0;
	ClearData.AveCost = 0;
	ClearData.Structure = 0;
	ClearData.Quality = 0;
	*ClearData.InstanceInfo = 0;
	*ClearData.ActivatedEffectInstanceInfo = 0;
	*ClearData.EquipEffectInstanceInfo = 0;
	*ClearData.BuilderName = 0;

    ClearData.TradeStack = 0;

    SetData(&ClearData);
}

void AuxItem::Empty()
{
    Clear();
    SetItemTemplateID(-1);
}

void AuxItem::ClearFlags()
{
	memset(Flags,0,sizeof(Flags));
}
