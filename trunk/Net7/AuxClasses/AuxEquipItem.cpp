#include "AuxEquipItem.h"

void AuxEquipItem::BuildPacket(unsigned char *buffer, long &index)
{
	AddFlags(Flags, sizeof(Flags), buffer, index);

	if (Flags[0] & 0x10)	//ExtendedFlags[2] & 0x04
	{
		AddData(buffer, Data->ItemTemplateID, index);
	}

	if (Flags[0] & 0x20)	//ExtendedFlags[2] & 0x08
	{
		AddData(buffer, Data->StackCount, index);
	}

	if (Flags[0] & 0x40)	//ExtendedFlags[2] & 0x10
	{
		AddData(buffer, Data->Price, index);
	}

	if (Flags[0] & 0x80)	//ExtendedFlags[2] & 0x20
	{
		AddData(buffer, Data->AveCost, index);
	}

	if (Flags[1] & 0x01)	//ExtendedFlags[2] & 0x40
	{
		AddData(buffer, Data->Structure, index);
	}

	if (Flags[1] & 0x02)	//ExtendedFlags[2] & 0x80
	{
		AddData(buffer, Data->Quality, index);
	}

	if (Flags[1] & 0x04)	//ExtendedFlags[3] & 0x01
	{
		AddString(buffer, Data->InstanceInfo, index);
	}

	if (Flags[1] & 0x08)	//ExtendedFlags[3] & 0x02
	{
		AddString(buffer, Data->ActivatedEffectInstanceInfo, index);
	}

	if (Flags[1] & 0x10)	//ExtendedFlags[3] & 0x04
	{
		AddString(buffer, Data->EquipEffectInstanceInfo, index);
	}

	if (Flags[1] & 0x20)	//ExtendedFlags[3] & 0x08
	{
		AddString(buffer, Data->BuilderName, index);
	}

	if (Flags[1] & 0x40)	//ExtendedFlags[3] & 0x10
	{
		AddData(buffer, Data->ReadyTime, index);
	}

	if (Flags[1] & 0x80)	//ExtendedFlags[3] & 0x20
	{
		AddData(buffer, Data->TargetRange, index);
	}

	if (Flags[2] & 0x01)	//ExtendedFlags[3] & 0x40
	{
		AddData(buffer, Data->ItemState, index);
	}

	if (Flags[2] & 0x02)	//ExtendedFlags[3] & 0x80
	{
		Effect.BuildPacket(buffer, index);
	}

	memset(Flags,0,sizeof(Flags));
}

void AuxEquipItem::BuildExtendedPacket(unsigned char *buffer, long &index)
{
	AddFlags(ExtendedFlags, sizeof(ExtendedFlags), buffer, index);

	if (ExtendedFlags[0] & 0x10)	//ExtendedFlags[2] & 0x04
	{
		AddData(buffer, Data->ItemTemplateID, index);
	}

	if (ExtendedFlags[0] & 0x20)	//ExtendedFlags[2] & 0x08
	{
		AddData(buffer, Data->StackCount, index);
	}

	if (ExtendedFlags[0] & 0x40)	//ExtendedFlags[2] & 0x10
	{
		AddData(buffer, Data->Price, index);
	}

	if (ExtendedFlags[0] & 0x80)	//ExtendedFlags[2] & 0x20
	{
		AddData(buffer, Data->AveCost, index);
	}

	if (ExtendedFlags[1] & 0x01)	//ExtendedFlags[2] & 0x40
	{
		AddData(buffer, Data->Structure, index);
	}

	if (ExtendedFlags[1] & 0x02)	//ExtendedFlags[2] & 0x80
	{
		AddData(buffer, Data->Quality, index);
	}

	if (ExtendedFlags[1] & 0x04)	//ExtendedFlags[3] & 0x01
	{
		AddString(buffer, Data->InstanceInfo, index);
	}

	if (ExtendedFlags[1] & 0x08)	//ExtendedFlags[3] & 0x02
	{
		AddString(buffer, Data->ActivatedEffectInstanceInfo, index);
	}

	if (ExtendedFlags[1] & 0x10)	//ExtendedFlags[3] & 0x04
	{
		AddString(buffer, Data->EquipEffectInstanceInfo, index);
	}

	if (ExtendedFlags[1] & 0x20)	//ExtendedFlags[3] & 0x08
	{
		AddString(buffer, Data->BuilderName, index);
	}

	if (ExtendedFlags[1] & 0x40)	//ExtendedFlags[3] & 0x10
	{
		AddData(buffer, Data->ReadyTime, index);
	}

	if (ExtendedFlags[1] & 0x80)	//ExtendedFlags[3] & 0x20
	{
		AddData(buffer, Data->TargetRange, index);
	}

	if (ExtendedFlags[2] & 0x01)	//ExtendedFlags[3] & 0x40
	{
		AddData(buffer, Data->ItemState, index);
	}

	if (ExtendedFlags[2] & 0x02)	//ExtendedFlags[3] & 0x80
	{
		Effect.BuildExtendedPacket(buffer, index);
	}
	else if (ExtendedFlags[3] & 0x80)
	{
		AddData(buffer, char(0x05), index);
	}
}

/******************************
*         GET METHODS         *
******************************/

_EquipItem * AuxEquipItem::GetData()                    {return Data;}
_Item * AuxEquipItem::GetItemData()                     {return ((_Item *) Data);}

s32 AuxEquipItem::GetItemTemplateID()					{return Data->ItemTemplateID;}
u32 AuxEquipItem::GetStackCount()						{return Data->StackCount;}
u64 AuxEquipItem::GetPrice()							{return Data->Price;}
float AuxEquipItem::GetAveCost()						{return Data->AveCost;}
float AuxEquipItem::GetStructure()						{return Data->Structure;}
float AuxEquipItem::GetQuality()						{return Data->Quality;}
char * AuxEquipItem::GetInstanceInfo()					{return Data->InstanceInfo;}
char * AuxEquipItem::GetActivatedEffectInstanceInfo()	{return Data->ActivatedEffectInstanceInfo;}
char * AuxEquipItem::GetEquipEffectInstanceInfo()		{return Data->EquipEffectInstanceInfo;}
char * AuxEquipItem::GetBuilderName()					{return Data->BuilderName;}
u32	AuxEquipItem::GetReadyTime()						{return Data->ReadyTime;}
float AuxEquipItem::GetTargetRange()					{return Data->TargetRange;}
u32	AuxEquipItem::GetItemState()						{return Data->ItemState;}

/******************************
*         SET METHODS         *
******************************/

void AuxEquipItem::SetData(_EquipItem * NewData)
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
	ReplaceData(Data->ReadyTime, NewData->ReadyTime, 10);
	ReplaceData(Data->TargetRange, NewData->TargetRange, 11);
	ReplaceData(Data->ItemState, NewData->ItemState, 12);
	Effect.SetData(&NewData->Effect);
}

/* NOTE: You need to set ReadyTime, TargetRange, ItemState, and Effect afterwards */

void AuxEquipItem::SetItemData(_Item * NewItemData)
{
	ReplaceData(Data->ItemTemplateID, NewItemData->ItemTemplateID, 0);
	ReplaceData(Data->StackCount, NewItemData->StackCount, 1);
	ReplaceData(Data->Price, NewItemData->Price, 2);
	ReplaceData(Data->AveCost, NewItemData->AveCost, 3);
	ReplaceData(Data->Structure, NewItemData->Structure, 4);
	ReplaceData(Data->Quality, NewItemData->Quality, 5);
	ReplaceString(Data->InstanceInfo, NewItemData->InstanceInfo, 6);
	ReplaceString(Data->ActivatedEffectInstanceInfo, NewItemData->ActivatedEffectInstanceInfo, 7);
	ReplaceString(Data->EquipEffectInstanceInfo, NewItemData->EquipEffectInstanceInfo, 8);
	ReplaceString(Data->BuilderName, NewItemData->BuilderName, 9);

    SetReadyTime(GetNet7TickCount());
    SetTargetRange(0);
    SetItemState(0);
    Effect.Clear();
}

void AuxEquipItem::SetItemTemplateID(s32 NewItemTemplateID)
{	
	ReplaceData(Data->ItemTemplateID, NewItemTemplateID, 0);
}

void AuxEquipItem::SetStackCount(u32 NewStackCount)
{	
	ReplaceData(Data->StackCount, NewStackCount, 1);
}

void AuxEquipItem::SetPrice(u64 NewPrice)
{	
	ReplaceData(Data->Price, NewPrice, 2);
}

void AuxEquipItem::SetAveCost(float NewAveCost)
{	
	ReplaceData(Data->AveCost, NewAveCost, 3);
}

void AuxEquipItem::SetStructure(float NewStructure)
{	
	ReplaceData(Data->Structure, NewStructure, 4);
}

void AuxEquipItem::SetQuality(float NewQuality)
{	
	ReplaceData(Data->Quality, NewQuality, 5);
}

void AuxEquipItem::SetInstanceInfo(char *NewInstanceInfo)
{
	ReplaceString(Data->InstanceInfo, NewInstanceInfo, 6);
}

void AuxEquipItem::SetActivatedEffectInstanceInfo(char * NewActivatedEffectInstanceInfo)
{
	ReplaceString(Data->ActivatedEffectInstanceInfo, NewActivatedEffectInstanceInfo, 7);
}

void AuxEquipItem::SetEquipEffectInstanceInfo(char * NewEquipEffectInstanceInfo)
{
	ReplaceString(Data->EquipEffectInstanceInfo, NewEquipEffectInstanceInfo, 8);
}

void AuxEquipItem::SetBuilderName(char * NewBuilderName)
{
	ReplaceString(Data->BuilderName, NewBuilderName, 9);
}

void AuxEquipItem::SetReadyTime(u32 NewReadyTime)
{
	ReplaceData(Data->ReadyTime, NewReadyTime, 10);
}

void AuxEquipItem::SetTargetRange(float NewTargetRange)
{
	ReplaceData(Data->TargetRange, NewTargetRange, 11);
}

void AuxEquipItem::SetItemState(u32 NewItemState)
{
	ReplaceData(Data->ItemState, NewItemState, 12);
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxEquipItem::Clear()
{
    _EquipItem ClearData;

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
	ClearData.ReadyTime = 0;
	ClearData.TargetRange = 0;
	ClearData.ItemState = 0;
    ClearData.Effect = Effect.GetClearStruct();

    SetData(&ClearData);
}

void AuxEquipItem::Empty()
{
    Clear();
    SetItemTemplateID(-1);
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxEquipItem::ClearFlags()
{
	memset(Flags,0,sizeof(Flags));
    Effect.ClearFlags();
}