#include "AuxHulkIndex.h"

bool AuxHulkIndex::BuildPacket(bool TwoBitFlags)
{
	return BuildPacket(PacketBuffer, PacketSize, TwoBitFlags);
}

bool AuxHulkIndex::BuildPacket(unsigned char *buffer, long &index, bool TwoBitFlags)
{
	index = 0;

	if (!GameID)
	{
		return false;
	}

	AddData(buffer, GameID, index);
	AddData(buffer, short(0), index);
	AddData(buffer, char(1), index);

	if (TwoBitFlags)
	{
		AddFlags(ExtendedFlags, sizeof(ExtendedFlags), buffer, index);
		memcpy(Flags, ExtendedFlags, sizeof(Flags));
	}
	else
	{
		AddFlags(Flags, sizeof(Flags), buffer, index);
	}

	if (!(Flags[0] & 0x02))
	{
		return false;
	}

	if (Flags[0] & 0x10)	//ExtendedFlags[1] & 0x10
	{
		AddString(buffer, Data.Name, index);
	}

	if (Flags[0] & 0x20)	//ExtendedFlags[1] & 0x20
	{
		AddString(buffer, Data.Owner, index);
	}

	if (Flags[0] & 0x40)	//ExtendedFlags[1] & 0x40
	{
        if (TwoBitFlags)
        {
		    QuadrantDamage.BuildExtendedPacket(buffer, index);
        }
        else
        {
		    QuadrantDamage.BuildPacket(buffer, index);
        }
	}
	else if (TwoBitFlags && ExtendedFlags[1] & 0x40)
	{
		AddData(buffer,char(0x05),index);
	}

	if (Flags[0] & 0x80)	//ExtendedFlags[1] & 0x80
	{
        if (TwoBitFlags)
        {
		    DamageSpot.BuildExtendedPacket(buffer, index);
        }
        else
        {
		    DamageSpot.BuildPacket(buffer, index);
        }
	}
	else if (TwoBitFlags && ExtendedFlags[1] & 0x80)
	{
		AddData(buffer,char(0x05),index);
	}

	if (Flags[1] & 0x01)	//ExtendedFlags[2] & 0x01
	{
        if (TwoBitFlags)
        {
		    DamageLine.BuildExtendedPacket(buffer, index);
        }
        else
        {
		    DamageLine.BuildPacket(buffer, index);
        }
	}
	else if (TwoBitFlags && ExtendedFlags[2] & 0x01)
	{
		AddData(buffer,char(0x05),index);
	}

	if (Flags[1] & 0x02)	//ExtendedFlags[2] & 0x02
	{
        if (TwoBitFlags)
        {
		    DamageBlotch.BuildExtendedPacket(buffer, index);
        }
        else
        {
		    DamageBlotch.BuildPacket(buffer, index);
        }
	}
	else if (TwoBitFlags && ExtendedFlags[2] & 0x02)
	{
		AddData(buffer,char(0x05),index);
	}

	if (Flags[1] & 0x04)	//ExtendedFlags[2] & 0x04
	{
        if (TwoBitFlags)
        {
		    EquipInv.BuildExtendedPacket(buffer, index);
        }
        else
        {
		    EquipInv.BuildPacket(buffer, index);
        }
	}
	else if (TwoBitFlags && ExtendedFlags[2] & 0x04)
	{
		AddData(buffer,char(0x05),index);
	}

	if (Flags[1] & 0x08)	//ExtendedFlags[2] & 0x08
	{
        if (TwoBitFlags)
        {
		    CargoInv.BuildExtendedPacket(buffer, index);
        }
        else
        {
		    CargoInv.BuildPacket(buffer, index);
        }
	}
	else if (TwoBitFlags && ExtendedFlags[2] & 0x08)
	{
		AddData(buffer,char(0x05),index);
	}

	*((short *) &buffer[4]) = short(index - 6);

	memset(Flags,0,sizeof(Flags));

	return true;
}

/******************************
*         GET METHODS         *
******************************/

_HulkIndex * AuxHulkIndex::GetData()    {return &Data;}

u32 AuxHulkIndex::GetGameID()		    {return GameID;}
char * AuxHulkIndex::GetName()		    {return Data.Name;}
char * AuxHulkIndex::GetOwner()		    {return Data.Owner;}

/******************************
*         SET METHODS         *
******************************/

void AuxHulkIndex::SetData(_HulkIndex *NewData)
{
	ReplaceString(Data.Name, NewData->Name, 0);
	ReplaceString(Data.Owner, NewData->Owner, 1);

	QuadrantDamage.SetData(&NewData->QuadrantDamage);
	DamageSpot.SetData(&NewData->DamageSpot);
	DamageLine.SetData(&NewData->DamageLine);
	DamageBlotch.SetData(&NewData->DamageBlotch);
	EquipInv.SetData(&NewData->EquipInv);
	CargoInv.SetData(&NewData->CargoInv);
}

void AuxHulkIndex::SetGameID(u32 NewGameID)
{
	GameID = NewGameID;
}

void AuxHulkIndex::SetName(char * NewName)
{
	ReplaceString(Data.Name, NewName, 0);
}

void AuxHulkIndex::SetOwner(char * NewOwner)
{
	ReplaceString(Data.Owner, NewOwner, 1);
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxHulkIndex::Reset()
{
    memset(Flags,0, sizeof(Flags));

    GameID = 0;

	*Data.Name = 0;
	*Data.Owner = 0;
	QuadrantDamage.Init(2, this, &Data.QuadrantDamage);
	DamageSpot.Init(3, this, &Data.DamageSpot);
	DamageLine.Init(4, this, &Data.DamageLine);
	DamageBlotch.Init(5, this, &Data.DamageBlotch);
	EquipInv.Init(6, this, &Data.EquipInv);
	CargoInv.Init(7, this, &Data.CargoInv);

	ExtendedFlags[0] = char(0x06);
	ExtendedFlags[1] = char(0xFC);
	ExtendedFlags[2] = char(0x0F);
}

void AuxHulkIndex::ClearFlags()
{
	memset(Flags,0, sizeof(Flags));
	QuadrantDamage.ClearFlags();
	DamageSpot.ClearFlags();
	DamageLine.ClearFlags();
	DamageBlotch.ClearFlags();
	EquipInv.ClearFlags();
	CargoInv.ClearFlags();
}
