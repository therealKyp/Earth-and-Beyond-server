#include "AuxManufacturingIndex.h"

bool AuxManufacturingIndex::BuildPacket(bool TwoBitFlags)
{
	return BuildPacket(PacketBuffer, PacketSize, TwoBitFlags);
}

bool AuxManufacturingIndex::BuildPacket(unsigned char *buffer, long &index, bool TwoBitFlags)
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

	if (Flags[0] & 0x10)	//ExtendedFlags[3] & 0x02
	{
		AddString(buffer,Data.Name,index);
	}

	if (Flags[0] & 0x20)	//ExtendedFlags[3] & 0x04
	{
		AddData(buffer,Data.Mode,index);
	}

	if (Flags[0] & 0x40)	//ExtendedFlags[3] & 0x08
	{
		AddData(buffer,Data.Validity,index);
	}

	if (Flags[0] & 0x80)	//ExtendedFlags[3] & 0x10
	{
		AddString(buffer,Data.FailureMessage,index);
	}

	if (Flags[1] & 0x01)	//ExtendedFlags[3] & 0x20
	{
		AddData(buffer,Data.Difficulty,index);
	}

	if (Flags[1] & 0x02)	//ExtendedFlags[3] & 0x40
	{
        if (TwoBitFlags)
        {
		    Target.BuildExtendedPacket(buffer, index);
        }
        else
        {
		    Target.BuildPacket(buffer, index);
        }
	}
	else if (TwoBitFlags && ExtendedFlags[3] & 0x40)
	{
		AddData(buffer,char(0x05),index);
	}

	if (Flags[1] & 0x04)	//ExtendedFlags[3] & 0x80
	{
        if (TwoBitFlags)
        {
		    Components.BuildExtendedPacket(buffer, index);
        }
        else
        {
		    Components.BuildPacket(buffer, index);
        }
	}
	else if (TwoBitFlags && ExtendedFlags[3] & 0x80)
	{
		AddData(buffer,char(0x05),index);
	}

	if (Flags[1] & 0x08)	//ExtendedFlags[4] & 0x01
	{
        if (TwoBitFlags)
        {
		    Override.BuildExtendedPacket(buffer, index);
        }
        else
        {
		    Override.BuildPacket(buffer, index);
        }
	}
	else if (TwoBitFlags && ExtendedFlags[4] & 0x01)
	{
		AddData(buffer,char(0x05),index);
	}

	if (Flags[1] & 0x10)	//ExtendedFlags[4] & 0x02
	{
		AddData(buffer,Data.NegotiatedCost,index);
	}

	if (Flags[1] & 0x20)	//ExtendedFlags[4] & 0x04
	{
		AddData(buffer,Data.BaseCost,index);
	}

	if (Flags[1] & 0x40)	//ExtendedFlags[4] & 0x08
	{
        PrimaryCategories.BuildPacket(buffer, index, TwoBitFlags);
	}
	else if (TwoBitFlags && ExtendedFlags[4] & 0x08)
	{
		AddData(buffer,char(0x05),index);
	}

	if (Flags[1] & 0x80)	//ExtendedFlags[4] & 0x10
	{
        KnownFormulas.BuildPacket(buffer, index, TwoBitFlags);
	}
	else if (TwoBitFlags && ExtendedFlags[4] & 0x10)
	{
		AddData(buffer,char(0x05),index);
	}

	if (Flags[2] & 0x01)	//ExtendedFlags[4] & 0x20
	{
        PreviousAttempts.BuildPacket(buffer, index, TwoBitFlags);
	}
	else if (TwoBitFlags && ExtendedFlags[4] & 0x20)
	{
		AddData(buffer,char(0x05),index);
	}

	if (Flags[2] & 0x02)	//ExtendedFlags[4] & 0x40
	{
		AddData(buffer,Data.CurrentItemCat,index);
	}

	if (Flags[2] & 0x04)	//ExtendedFlags[4] & 0x80
	{
		AddData(buffer,Data.SuccessProbability,index);
	}

	if (Flags[2] & 0x08)	//ExtendedFlags[5] & 0x01
	{
		AddData(buffer,Data.CriticalSuccessProbability,index);
	}

	if (Flags[2] & 0x10)	//ExtendedFlags[5] & 0x02
	{
		AddData(buffer,Data.ExpectedQuality,index);
	}

	if (Flags[2] & 0x20)	//ExtendedFlags[5] & 0x04
	{
		AddData(buffer,Data.MinimumQuality,index);
	}

	if (Flags[2] & 0x40)	//ExtendedFlags[5] & 0x08
	{
		AddData(buffer,Data.MaximumQuality,index);
	}

	if (Flags[2] & 0x80)	//ExtendedFlags[5] & 0x10
	{
		AddData(buffer,Data.AdditionalIterations,index);
	}

	if (Flags[3] & 0x01)	//ExtendedFlags[5] & 0x20
	{
		AddData(buffer,Data.TechFilterBitField,index);
	}

	*((short *) &buffer[4]) = short(index - 6);

	memset(Flags,0,sizeof(Flags));

	return true;
}

void AuxManufacturingIndex::TerminalReset(unsigned int NumKnown)
{
    SetName("Manufacturing Lab");
    SetMode(1);
    SetValidity(0);
    SetFailureMessage("");
    SetDifficulty(0);

    // Target, Components, and Override are reset elsewhere

    SetNegotiatedCost(0);
    SetBaseCost(0);

    // Clear KnownFormulas and Previous Attempts
    KnownFormulas.ResetKnownFormulas(NumKnown);
    PreviousAttempts.Clear();

    SetCurrentItemCat(0);
    SetSuccessProbability(0);
    SetCriticalSuccessProbability(0);
    SetExpectedQuality(1);
    SetMinimumQuality(1);
    SetMaximumQuality(1);
    SetAdditionalIterations(0);
    SetTechFilterBitField(0x01FF);

    BuildPacket();
}

/******************************
*         GET METHODS         *
******************************/

_ManufacturingIndex * AuxManufacturingIndex::GetData()          {return &Data;}

u32 AuxManufacturingIndex::GetGameID()		                    {return GameID;}
char * AuxManufacturingIndex::GetName()		                    {return Data.Name;}
u32 AuxManufacturingIndex::GetMode()                            {return Data.Mode;}
u32 AuxManufacturingIndex::GetValidity()                        {return Data.Validity;}
char * AuxManufacturingIndex::GetFailureMessage()               {return Data.FailureMessage;}
u32 AuxManufacturingIndex::GetDifficulty()                      {return Data.Difficulty;}
u64 AuxManufacturingIndex::GetNegotiatedCost()                  {return Data.NegotiatedCost;}
u64 AuxManufacturingIndex::GetBaseCost()                        {return Data.BaseCost;}
u32 AuxManufacturingIndex::GetCurrentItemCat()                  {return Data.CurrentItemCat;}
float AuxManufacturingIndex::GetSuccessProbability()            {return Data.SuccessProbability;}
float AuxManufacturingIndex::GetCriticalSuccessProbability()    {return Data.CriticalSuccessProbability;}
float AuxManufacturingIndex::GetExpectedQuality()               {return Data.ExpectedQuality;}
float AuxManufacturingIndex::GetMinimumQuality()                {return Data.MinimumQuality;}
float AuxManufacturingIndex::GetMaximumQuality()                {return Data.MaximumQuality;}
u32 AuxManufacturingIndex::GetAdditionalIterations()            {return Data.AdditionalIterations;}
u32 AuxManufacturingIndex::GetTechFilterBitField()              {return Data.TechFilterBitField;}

/******************************
*         SET METHODS         *
******************************/

void AuxManufacturingIndex::SetData(_ManufacturingIndex *NewData)
{
    ReplaceString(Data.Name, NewData->Name, 0);
    ReplaceData(Data.Mode, NewData->Mode, 1);
    ReplaceData(Data.Validity, NewData->Validity, 2);
	ReplaceString(Data.FailureMessage, NewData->FailureMessage, 3);
    ReplaceData(Data.Difficulty, NewData->Difficulty, 4);
	Target.SetData(&NewData->Target);
	Components.SetData(&NewData->Components);
	Override.SetData(&NewData->Override);
    ReplaceData(Data.NegotiatedCost, NewData->NegotiatedCost, 8);
    ReplaceData(Data.BaseCost, NewData->BaseCost, 9);
	PrimaryCategories.SetData(&NewData->PrimaryCategories);
	KnownFormulas.SetData(&NewData->KnownFormulas);
	PreviousAttempts.SetData(&NewData->PreviousAttempts);
    ReplaceData(Data.CurrentItemCat, NewData->CurrentItemCat, 13);
    ReplaceData(Data.SuccessProbability, NewData->SuccessProbability, 14);
    ReplaceData(Data.CriticalSuccessProbability, NewData->CriticalSuccessProbability, 15);
    ReplaceData(Data.ExpectedQuality, NewData->ExpectedQuality, 16);
    ReplaceData(Data.MinimumQuality, NewData->MinimumQuality, 17);
    ReplaceData(Data.MaximumQuality, NewData->MaximumQuality, 18);
    ReplaceData(Data.AdditionalIterations, NewData->AdditionalIterations, 19);
    ReplaceData(Data.TechFilterBitField, NewData->TechFilterBitField, 20);
}

void AuxManufacturingIndex::SetGameID(u32 NewGameID)
{
	GameID = NewGameID;
}

void AuxManufacturingIndex::SetName(const char * NewName)
{
	ReplaceString(Data.Name, NewName, 0);
}

void AuxManufacturingIndex::SetMode(u32 NewMode)
{
    ReplaceData(Data.Mode, NewMode, 1);
}

void AuxManufacturingIndex::SetValidity(u32 NewValidity)
{
    ReplaceData(Data.Validity, NewValidity, 2);
}

void AuxManufacturingIndex::SetFailureMessage(const char * NewFailureMessage)
{
	ReplaceString(Data.FailureMessage, NewFailureMessage, 3);
}

void AuxManufacturingIndex::SetDifficulty(u32 NewDifficulty)
{
    ReplaceData(Data.Difficulty, NewDifficulty, 4);
}

void AuxManufacturingIndex::SetNegotiatedCost(u64 NewNegotiatedCost)
{
    ReplaceData(Data.NegotiatedCost, NewNegotiatedCost, 8);
}

void AuxManufacturingIndex::SetBaseCost(u64 NewBaseCost)
{
    ReplaceData(Data.BaseCost, NewBaseCost, 9);
}

void AuxManufacturingIndex::SetCurrentItemCat(u32 NewCurrentItemCat)
{
    ReplaceData(Data.CurrentItemCat, NewCurrentItemCat, 13);
}

void AuxManufacturingIndex::SetSuccessProbability(float NewSuccessProbability)
{
    ReplaceData(Data.SuccessProbability, NewSuccessProbability, 14);
}

void AuxManufacturingIndex::SetCriticalSuccessProbability(float NewCriticalSuccessProbability)
{
    ReplaceData(Data.CriticalSuccessProbability, NewCriticalSuccessProbability, 15);
}

void AuxManufacturingIndex::SetExpectedQuality(float NewExpectedQuality)
{
    ReplaceData(Data.ExpectedQuality, NewExpectedQuality, 16);
}

void AuxManufacturingIndex::SetMinimumQuality(float NewMinimumQuality)
{
    ReplaceData(Data.MinimumQuality, NewMinimumQuality, 17);
}

void AuxManufacturingIndex::SetMaximumQuality(float NewMaximumQuality)
{
    ReplaceData(Data.MaximumQuality, NewMaximumQuality, 18);
}

void AuxManufacturingIndex::SetAdditionalIterations(u32 NewAdditionalIterations)
{
    ReplaceData(Data.AdditionalIterations, NewAdditionalIterations, 19);
}

void AuxManufacturingIndex::SetTechFilterBitField(u32 NewTechFilterBitField)
{
    ReplaceData(Data.TechFilterBitField, NewTechFilterBitField, 20);
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxManufacturingIndex::Reset()
{
	memset(Flags,0, sizeof(Flags));

    GameID = 0;

	*Data.Name = 0;
    Data.Mode = 0;
    Data.Validity = 0;
	*Data.FailureMessage = 0;
    Data.Difficulty = 0;
    Target.Init(5, this, &Data.Target);
    Components.Init(6, this, &Data.Components);
    Override.Init(7, this, &Data.Override);
    Data.NegotiatedCost = 0;
    Data.BaseCost = 0;
    PrimaryCategories.Init(10, this, &Data.PrimaryCategories);
    KnownFormulas.Init(11, this, &Data.KnownFormulas);
    PreviousAttempts.Init(12, this, &Data.PreviousAttempts);
    Data.CurrentItemCat = 0;
    Data.SuccessProbability = 0;
    Data.CriticalSuccessProbability = 0;
    Data.ExpectedQuality = 0;
    Data.MinimumQuality = 0;
    Data.MaximumQuality = 0;
    Data.AdditionalIterations = 0;
	Data.TechFilterBitField = 0;

    /*
	ExtendedFlags[0] = char(0xF6);
	ExtendedFlags[1] = char(0x71);
	ExtendedFlags[2] = char(0x74);
	ExtendedFlags[3] = char(0xC1);
	ExtendedFlags[4] = char(0x79);
	ExtendedFlags[5] = char(0x11);
    */

	ExtendedFlags[0] = char(0x06);
	ExtendedFlags[1] = char(0x00);
	ExtendedFlags[2] = char(0x00);
	ExtendedFlags[3] = char(0xFE);
	ExtendedFlags[4] = char(0xFF);
	ExtendedFlags[5] = char(0x3F);

    InitializeCategories();
}

void AuxManufacturingIndex::InitializeCategories()
{
    /* Initializes the data in the category classes for manufacturing */
    _PrimaryCategories InitData;
    memset(&InitData, 0, sizeof(_PrimaryCategories));

    strcpy(InitData.PrimaryCategory[0].Name, "Items");

    strcpy(InitData.PrimaryCategory[0].Categories.Category[0].Name, "Weapons");
    InitData.PrimaryCategory[0].Categories.Category[0].CategoryID = 10;

    strcpy(InitData.PrimaryCategory[0].Categories.Category[0].SubCategories.SubCategory[0].Name, "Beam");
    InitData.PrimaryCategory[0].Categories.Category[0].SubCategories.SubCategory[0].SubCategoryID = 100;
    strcpy(InitData.PrimaryCategory[0].Categories.Category[0].SubCategories.SubCategory[1].Name, "Projectile");
    InitData.PrimaryCategory[0].Categories.Category[0].SubCategories.SubCategory[1].SubCategoryID = 101;
    strcpy(InitData.PrimaryCategory[0].Categories.Category[0].SubCategories.SubCategory[2].Name, "Missile");
    InitData.PrimaryCategory[0].Categories.Category[0].SubCategories.SubCategory[2].SubCategoryID = 102;
    strcpy(InitData.PrimaryCategory[0].Categories.Category[0].SubCategories.SubCategory[3].Name, "Ammo");
    InitData.PrimaryCategory[0].Categories.Category[0].SubCategories.SubCategory[3].SubCategoryID = 103;

    strcpy(InitData.PrimaryCategory[0].Categories.Category[1].Name, "Systems");
    InitData.PrimaryCategory[0].Categories.Category[1].CategoryID = 11;

    strcpy(InitData.PrimaryCategory[0].Categories.Category[1].SubCategories.SubCategory[0].Name, "Basic");
    InitData.PrimaryCategory[0].Categories.Category[1].SubCategories.SubCategory[0].SubCategoryID = 110;
    strcpy(InitData.PrimaryCategory[0].Categories.Category[1].SubCategories.SubCategory[1].Name, "Drone");
    InitData.PrimaryCategory[0].Categories.Category[1].SubCategories.SubCategory[1].SubCategoryID = 111;

    strcpy(InitData.PrimaryCategory[0].Categories.Category[2].Name, "Core");
    InitData.PrimaryCategory[0].Categories.Category[2].CategoryID = 12;

    strcpy(InitData.PrimaryCategory[0].Categories.Category[2].SubCategories.SubCategory[0].Name, "Reactor");
    InitData.PrimaryCategory[0].Categories.Category[2].SubCategories.SubCategory[0].SubCategoryID = 120;
    strcpy(InitData.PrimaryCategory[0].Categories.Category[2].SubCategories.SubCategory[1].Name, "Engine");
    InitData.PrimaryCategory[0].Categories.Category[2].SubCategories.SubCategory[1].SubCategoryID = 121;
    strcpy(InitData.PrimaryCategory[0].Categories.Category[2].SubCategories.SubCategory[2].Name, "Shield");
    InitData.PrimaryCategory[0].Categories.Category[2].SubCategories.SubCategory[2].SubCategoryID = 122;

    strcpy(InitData.PrimaryCategory[0].Categories.Category[3].Name, "Consumables");
    InitData.PrimaryCategory[0].Categories.Category[3].CategoryID = 13;

    strcpy(InitData.PrimaryCategory[0].Categories.Category[3].SubCategories.SubCategory[0].Name, "Consumable X");
    InitData.PrimaryCategory[0].Categories.Category[3].SubCategories.SubCategory[0].SubCategoryID = 130;

    strcpy(InitData.PrimaryCategory[1].Name, "Components");

    strcpy(InitData.PrimaryCategory[1].Categories.Category[0].Name, "Computer / Electronics");
    InitData.PrimaryCategory[1].Categories.Category[0].CategoryID = 50;

    strcpy(InitData.PrimaryCategory[1].Categories.Category[0].SubCategories.SubCategory[0].Name, "Software");
    InitData.PrimaryCategory[1].Categories.Category[0].SubCategories.SubCategory[0].SubCategoryID = 140;
    strcpy(InitData.PrimaryCategory[1].Categories.Category[0].SubCategories.SubCategory[1].Name, "Electronics");
    InitData.PrimaryCategory[1].Categories.Category[0].SubCategories.SubCategory[1].SubCategoryID = 141;
    strcpy(InitData.PrimaryCategory[1].Categories.Category[0].SubCategories.SubCategory[2].Name, "Computer");
    InitData.PrimaryCategory[1].Categories.Category[0].SubCategories.SubCategory[2].SubCategoryID = 142;

    strcpy(InitData.PrimaryCategory[1].Categories.Category[1].Name, "Power");
    InitData.PrimaryCategory[1].Categories.Category[1].CategoryID = 51;

    strcpy(InitData.PrimaryCategory[1].Categories.Category[1].SubCategories.SubCategory[0].Name, "Power Converter");
    InitData.PrimaryCategory[1].Categories.Category[1].SubCategories.SubCategory[0].SubCategoryID = 150;
    strcpy(InitData.PrimaryCategory[1].Categories.Category[1].SubCategories.SubCategory[1].Name, "Power Coupling");
    InitData.PrimaryCategory[1].Categories.Category[1].SubCategories.SubCategory[1].SubCategoryID = 151;
    strcpy(InitData.PrimaryCategory[1].Categories.Category[1].SubCategories.SubCategory[2].Name, "Power Core");
    InitData.PrimaryCategory[1].Categories.Category[1].SubCategories.SubCategory[2].SubCategoryID = 152;
    strcpy(InitData.PrimaryCategory[1].Categories.Category[1].SubCategories.SubCategory[3].Name, "Generator");
    InitData.PrimaryCategory[1].Categories.Category[1].SubCategories.SubCategory[3].SubCategoryID = 153;

    strcpy(InitData.PrimaryCategory[1].Categories.Category[2].Name, "Fabricated");
    InitData.PrimaryCategory[1].Categories.Category[2].CategoryID = 52;

    strcpy(InitData.PrimaryCategory[1].Categories.Category[2].SubCategories.SubCategory[0].Name, "Shielded Casing");
    InitData.PrimaryCategory[1].Categories.Category[2].SubCategories.SubCategory[0].SubCategoryID = 160;
    strcpy(InitData.PrimaryCategory[1].Categories.Category[2].SubCategories.SubCategory[1].Name, "Engine Frame");
    InitData.PrimaryCategory[1].Categories.Category[2].SubCategories.SubCategory[1].SubCategoryID = 161;
    strcpy(InitData.PrimaryCategory[1].Categories.Category[2].SubCategories.SubCategory[2].Name, "Drone Frame");
    InitData.PrimaryCategory[1].Categories.Category[2].SubCategories.SubCategory[2].SubCategoryID = 162;
    strcpy(InitData.PrimaryCategory[1].Categories.Category[2].SubCategories.SubCategory[3].Name, "Mounts");
    InitData.PrimaryCategory[1].Categories.Category[2].SubCategories.SubCategory[3].SubCategoryID = 163;

    strcpy(InitData.PrimaryCategory[1].Categories.Category[3].Name, "Weapon Components");
    InitData.PrimaryCategory[1].Categories.Category[3].CategoryID = 53;

    strcpy(InitData.PrimaryCategory[1].Categories.Category[3].SubCategories.SubCategory[0].Name, "Firing Mechanism");
    InitData.PrimaryCategory[1].Categories.Category[3].SubCategories.SubCategory[0].SubCategoryID = 170;
    strcpy(InitData.PrimaryCategory[1].Categories.Category[3].SubCategories.SubCategory[1].Name, "Ammo Feeder / Loader");
    InitData.PrimaryCategory[1].Categories.Category[3].SubCategories.SubCategory[1].SubCategoryID = 171;
    strcpy(InitData.PrimaryCategory[1].Categories.Category[3].SubCategories.SubCategory[2].Name, "Optics");
    InitData.PrimaryCategory[1].Categories.Category[3].SubCategories.SubCategory[2].SubCategoryID = 172;
    strcpy(InitData.PrimaryCategory[1].Categories.Category[3].SubCategories.SubCategory[3].Name, "Barrel / Launch Tube");
    InitData.PrimaryCategory[1].Categories.Category[3].SubCategories.SubCategory[3].SubCategoryID = 173;

    strcpy(InitData.PrimaryCategory[1].Categories.Category[4].Name, "Ammo");
    InitData.PrimaryCategory[1].Categories.Category[4].CategoryID = 54;

    strcpy(InitData.PrimaryCategory[1].Categories.Category[4].SubCategories.SubCategory[0].Name, "Slug / Rail");
    InitData.PrimaryCategory[1].Categories.Category[4].SubCategories.SubCategory[0].SubCategoryID = 180;
    strcpy(InitData.PrimaryCategory[1].Categories.Category[4].SubCategories.SubCategory[1].Name, "Warhead");
    InitData.PrimaryCategory[1].Categories.Category[4].SubCategories.SubCategory[1].SubCategoryID = 181;
    strcpy(InitData.PrimaryCategory[1].Categories.Category[4].SubCategories.SubCategory[2].Name, "Propellant");
    InitData.PrimaryCategory[1].Categories.Category[4].SubCategories.SubCategory[2].SubCategoryID = 182;
    strcpy(InitData.PrimaryCategory[1].Categories.Category[4].SubCategories.SubCategory[3].Name, "Shell Casing");
    InitData.PrimaryCategory[1].Categories.Category[4].SubCategories.SubCategory[3].SubCategoryID = 183;

    PrimaryCategories.SetData(&InitData);
}

void AuxManufacturingIndex::ClearFlags()
{
	memset(Flags,0, sizeof(Flags));
    Target.ClearFlags();
    Components.ClearFlags();
    Override.ClearFlags();
    PrimaryCategories.ClearFlags();
    KnownFormulas.ClearFlags();
    PreviousAttempts.ClearFlags();
}
