#include "AuxHarvestable.h"

bool AuxHarvestable::BuildNamePacket()
{
	return BuildNamePacket(PacketBuffer, PacketSize);
}

bool AuxHarvestable::BuildNamePacket(unsigned char *buffer, long &index)
{
	index = 0;

	if (!GameID)
	{
		return false;
	}

	AddData(buffer, GameID, index);
	AddData(buffer, short(0), index);
	AddData(buffer, char(1), index);

	AddData(buffer, char(0x16), index);
	AddData(buffer, char(0x04), index);

	AddString(buffer, Data.Name, index);

	*((short *) &buffer[4]) = short(index - 6);

	return true;
}

bool AuxHarvestable::BuildPacket(bool TwoBitFlags)
{
	return BuildPacket(PacketBuffer, PacketSize, TwoBitFlags);
}

bool AuxHarvestable::BuildPacket(unsigned char *buffer, long &index, bool TwoBitFlags)
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

	if (Flags[0] & 0x10)	//ExtendedFlags[1] & 0x01
	{
		AddString(buffer, Data.Name, index);
	}

	if (Flags[0] & 0x20)	//ExtendedFlags[1] & 0x02
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
	else if (TwoBitFlags && ExtendedFlags[1] & 0x02)
	{
		AddData(buffer,char(0x05),index);
	}

	if (Flags[0] & 0x40)	//ExtendedFlags[1] & 0x04
	{
		AddData(buffer, Data.PercentFull, index);
	}

	if (Flags[0] & 0x80)	//ExtendedFlags[1] & 0x08
	{
		AddData(buffer, Data.TechLevel, index);
	}

	*((short *) &buffer[4]) = short(index - 6);

	memset(Flags,0,sizeof(Flags));

	return true;
}

/******************************
*         GET METHODS         *
******************************/

_Harvestable  * AuxHarvestable::GetData()   {return &Data;}

u32 AuxHarvestable::GetGameID()			    {return GameID;}
char * AuxHarvestable::GetName()		    {return Data.Name;}
float AuxHarvestable::GetPercentFull()	    {return Data.PercentFull;}
u32 AuxHarvestable::GetTechLevel()		    {return Data.TechLevel;}

/******************************
*         SET METHODS         *
******************************/

void AuxHarvestable::SetData(_Harvestable *NewData)
{
	ReplaceString(Data.Name, NewData->Name, 0);
	CargoInv.SetData(&NewData->CargoInv);
	ReplaceData(Data.PercentFull, NewData->PercentFull, 2);
	ReplaceData(Data.TechLevel, NewData->TechLevel, 3);
}

void AuxHarvestable::SetGameID(u32 NewGameID)
{
	GameID = NewGameID;
}

void AuxHarvestable::SetName(char * NewName)
{
	ReplaceString(Data.Name, NewName, 0);
}

void AuxHarvestable::SetPercentFull(float NewPercentFull)
{
	ReplaceData(Data.PercentFull, NewPercentFull, 2);
}

void AuxHarvestable::SetTechLevel(u32 NewTechLevel)
{
	ReplaceData(Data.TechLevel, NewTechLevel, 3);
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxHarvestable::Reset()
{
	memset(Flags,0, sizeof(Flags));

    GameID = 0;

	*Data.Name = 0;
	CargoInv.Init(1, this, &Data.CargoInv);
	Data.PercentFull = 1.0f;
	Data.TechLevel = 0;

	ExtendedFlags[0] = char(0x66);
	ExtendedFlags[1] = char(0x0B);
}

void AuxHarvestable::ClearFlags()
{
	memset(Flags,0, sizeof(Flags));
    CargoInv.ClearFlags();
}
