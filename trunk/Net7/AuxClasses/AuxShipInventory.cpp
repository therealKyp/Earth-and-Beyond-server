#include "AuxShipInventory.h"

void AuxShipInv::BuildPacket(unsigned char *buffer, long &index)
{
	AddFlags(Flags, sizeof(Flags), buffer, index);

	if (Flags[0] & 0x10)	//ExtendedFlags[1] & 0x80
	{
		AddData(buffer, Data->CargoSpace, index);
	}

	if (Flags[0] & 0x20)	//ExtendedFlags[2] & 0x01
	{
		AddString(buffer, Data->EquipMountModel, index);
	}

	if (Flags[0] & 0x40)	//ExtendedFlags[2] & 0x02
	{
	    Mounts.BuildPacket(buffer, index);
	}

	if (Flags[0] & 0x80)	//ExtendedFlags[2] & 0x04
	{
	    MountBones.BuildPacket(buffer, index);
	}

	if (Flags[1] & 0x01)	//ExtendedFlags[2] & 0x08
	{
		AddData(buffer, Data->FutureWeapons, index);
	}

	if (Flags[1] & 0x02)	//ExtendedFlags[2] & 0x10
	{
		AddData(buffer, Data->FutureDevices, index);
	}

	if (Flags[1] & 0x04)	//ExtendedFlags[2] & 0x20
	{
	    CargoInv.BuildPacket(buffer, index);
	}

	if (Flags[1] & 0x08)	//ExtendedFlags[2] & 0x40
	{
	    EquipInv.BuildPacket(buffer, index);
	}

	if (Flags[1] & 0x10)	//ExtendedFlags[2] & 0x80
	{
	    AmmoInv.BuildPacket(buffer, index);
	}

	if (Flags[1] & 0x20)	//ExtendedFlags[3] & 0x01
	{
	    HullInv.BuildPacket(buffer, index);
	}

	if (Flags[1] & 0x40)	//ExtendedFlags[3] & 0x02
	{
	    TradeInv.BuildPacket(buffer, index);
	}

	memset(Flags,0,sizeof(Flags));
}

void AuxShipInv::BuildExtendedPacket(unsigned char *buffer, long &index)
{
	AddFlags(ExtendedFlags, sizeof(ExtendedFlags), buffer, index);

	if (ExtendedFlags[0] & 0x10)	//ExtendedFlags[1] & 0x80
	{
		AddData(buffer, Data->CargoSpace, index);
	}

	if (ExtendedFlags[0] & 0x20)	//ExtendedFlags[2] & 0x01
	{
		AddString(buffer, Data->EquipMountModel, index);
	}

	if (ExtendedFlags[0] & 0x40)	//ExtendedFlags[2] & 0x02
	{
		Mounts.BuildExtendedPacket(buffer, index);
	}
	else if (ExtendedFlags[2] & 0x02)
	{
		AddData(buffer, char(0x05), index);
	}

	if (ExtendedFlags[0] & 0x80)	//ExtendedFlags[2] & 0x04
	{
	    MountBones.BuildExtendedPacket(buffer, index);
	}
	else if (ExtendedFlags[2] & 0x04)
	{
		AddData(buffer, char(0x05), index);
	}

	if (ExtendedFlags[1] & 0x01)	//ExtendedFlags[2] & 0x08
	{
		AddData(buffer, Data->FutureWeapons, index);
	}

	if (ExtendedFlags[1] & 0x02)	//ExtendedFlags[2] & 0x10
	{
		AddData(buffer, Data->FutureDevices, index);
	}

	if (ExtendedFlags[1] & 0x04)	//ExtendedFlags[2] & 0x20
	{
	    CargoInv.BuildExtendedPacket(buffer, index);
	}
	else if (ExtendedFlags[2] & 0x20)
	{
		AddData(buffer, char(0x05), index);
	}

	if (ExtendedFlags[1] & 0x08)	//ExtendedFlags[2] & 0x40
	{
	    EquipInv.BuildExtendedPacket(buffer, index);
	}
	else if (ExtendedFlags[2] & 0x40)
	{
		AddData(buffer, char(0x05), index);
	}

	if (ExtendedFlags[1] & 0x10)	//ExtendedFlags[2] & 0x80
	{
	    AmmoInv.BuildExtendedPacket(buffer, index);
	}
	else if (ExtendedFlags[2] & 0x80)
	{
		AddData(buffer, char(0x05), index);
	}

	if (ExtendedFlags[1] & 0x20)	//ExtendedFlags[3] & 0x01
	{
	    HullInv.BuildExtendedPacket(buffer, index);
	}
	else if (ExtendedFlags[3] & 0x01)
	{
		AddData(buffer, char(0x05), index);
	}

	if (ExtendedFlags[1] & 0x40)	//ExtendedFlags[3] & 0x02
	{
	    TradeInv.BuildExtendedPacket(buffer, index);
	}
	else if (ExtendedFlags[3] & 0x02)
	{
		AddData(buffer, char(0x05), index);
	}
}

/******************************
*         GET METHODS         *
******************************/

_ShipInv * AuxShipInv::GetData()            {return Data;}

u32 AuxShipInv::GetCargoSpace()				{return Data->CargoSpace;}
char * AuxShipInv::GetEquipMountModel()		{return Data->EquipMountModel;}
u32 AuxShipInv::GetFutureWeapons()			{return Data->FutureWeapons;}
u32 AuxShipInv::GetFutureDevices()			{return Data->FutureDevices;}

/******************************
*         SET METHODS         *
******************************/

void AuxShipInv::SetData(_ShipInv *NewData)
{
	ReplaceData(Data->CargoSpace, NewData->CargoSpace, 0);
	ReplaceString(Data->EquipMountModel, NewData->EquipMountModel, 1);
	Mounts.SetData(&NewData->Mounts);
	MountBones.SetData(&NewData->MountBones);
	ReplaceData(Data->FutureWeapons, NewData->FutureWeapons, 4);
	ReplaceData(Data->FutureDevices, NewData->FutureDevices, 5);
	CargoInv.SetData(&NewData->CargoInv);
	EquipInv.SetData(&NewData->EquipInv);
	AmmoInv.SetData(&NewData->AmmoInv);
	HullInv.SetData(&NewData->HullInv);
	TradeInv.SetData(&NewData->TradeInv);
}

void AuxShipInv::SetCargoSpace(u32 NewCargoSpace)
{
	ReplaceData(Data->CargoSpace, NewCargoSpace, 0);
}

void AuxShipInv::SetEquipMountModel(char * NewEquipMountModel)
{
	ReplaceString(Data->EquipMountModel, NewEquipMountModel, 1);
}

void AuxShipInv::SetFutureWeapons(u32 NewFutureWeapons)
{
	ReplaceData(Data->FutureWeapons, NewFutureWeapons, 4);
}

void AuxShipInv::SetFutureDevices(u32 NewFutureDevices)
{
	ReplaceData(Data->FutureDevices, NewFutureDevices, 5);
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxShipInv::ClearFlags()
{
	memset(Flags,0,sizeof(Flags));
	Mounts.ClearFlags();
	MountBones.ClearFlags();
	CargoInv.ClearFlags();
	EquipInv.ClearFlags();;
	AmmoInv.ClearFlags();
	HullInv.ClearFlags();
	TradeInv.ClearFlags();
}