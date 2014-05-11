#include "AuxStats.h"

void AuxStats::BuildPacket(unsigned char *buffer, long &index)
{
	AddFlags(Flags, sizeof(Flags), buffer, index);

	if (Flags[0] & 0x10)	//ExtendedFlags[2] & 0x08
	{
		AddData(buffer, Data->Defence, index);
	}

	if (Flags[0] & 0x20)	//ExtendedFlags[2] & 0x10
	{
		AddData(buffer, Data->MissleDefence, index);
	}

	if (Flags[0] & 0x40)	//ExtendedFlags[2] & 0x20
	{
		AddData(buffer, Data->Speed, index);
	}

	if (Flags[0] & 0x80)	//ExtendedFlags[2] & 0x40
	{
		AddData(buffer, Data->WarpSpeed, index);
	}

	if (Flags[1] & 0x01)	//ExtendedFlags[2] & 0x80
	{
		AddData(buffer, Data->WarpPowerLevel, index);
	}

	if (Flags[1] & 0x02)	//ExtendedFlags[3] & 0x01
	{
		AddData(buffer, Data->TurnRate, index);
	}

	if (Flags[1] & 0x04)	//ExtendedFlags[3] & 0x02
	{
		AddData(buffer, Data->ScanRange, index);
	}

	if (Flags[1] & 0x08)	//ExtendedFlags[3] & 0x04
	{
		AddData(buffer, Data->Visibility, index);
	}

	if (Flags[1] & 0x10)	//ExtendedFlags[3] & 0x08
	{
		AddData(buffer, Data->ResistImpact, index);
	}

	if (Flags[1] & 0x20)	//ExtendedFlags[3] & 0x10
	{
		AddData(buffer, Data->ResistExplosion, index);
	}

	if (Flags[1] & 0x40)	//ExtendedFlags[3] & 0x20
	{
		AddData(buffer, Data->ResistPlasma, index);
	}

	if (Flags[1] & 0x80)	//ExtendedFlags[3] & 0x40
	{
		AddData(buffer, Data->ResistEnergy, index);
	}

	if (Flags[2] & 0x01)	//ExtendedFlags[3] & 0x80
	{
		AddData(buffer, Data->ResistEMP, index);
	}

	if (Flags[2] & 0x02)	//ExtendedFlags[4] & 0x01
	{
		AddData(buffer, Data->ResistChemical, index);
	}

	if (Flags[2] & 0x04)	//ExtendedFlags[4] & 0x02
	{
		AddData(buffer, Data->ResistPsionic, index);
	}

	memset(Flags,0,sizeof(Flags));
}

void AuxStats::BuildExtendedPacket(unsigned char *buffer, long &index)
{
	AddFlags(ExtendedFlags, sizeof(ExtendedFlags), buffer, index);

	if (ExtendedFlags[0] & 0x10)	//ExtendedFlags[2] & 0x08
	{
		AddData(buffer, Data->Defence, index);
	}

	if (ExtendedFlags[0] & 0x20)	//ExtendedFlags[2] & 0x10
	{
		AddData(buffer, Data->MissleDefence, index);
	}

	if (ExtendedFlags[0] & 0x40)	//ExtendedFlags[2] & 0x20
	{
		AddData(buffer, Data->Speed, index);
	}

	if (ExtendedFlags[0] & 0x80)	//ExtendedFlags[2] & 0x40
	{
		AddData(buffer, Data->WarpSpeed, index);
	}

	if (ExtendedFlags[1] & 0x01)	//ExtendedFlags[2] & 0x80
	{
		AddData(buffer, Data->WarpPowerLevel, index);
	}

	if (ExtendedFlags[1] & 0x02)	//ExtendedFlags[3] & 0x01
	{
		AddData(buffer, Data->TurnRate, index);
	}

	if (ExtendedFlags[1] & 0x04)	//ExtendedFlags[3] & 0x02
	{
		AddData(buffer, Data->ScanRange, index);
	}

	if (ExtendedFlags[1] & 0x08)	//ExtendedFlags[3] & 0x04
	{
		AddData(buffer, Data->Visibility, index);
	}

	if (ExtendedFlags[1] & 0x10)	//ExtendedFlags[3] & 0x08
	{
		AddData(buffer, Data->ResistImpact, index);
	}

	if (ExtendedFlags[1] & 0x20)	//ExtendedFlags[3] & 0x10
	{
		AddData(buffer, Data->ResistExplosion, index);
	}

	if (ExtendedFlags[1] & 0x40)	//ExtendedFlags[3] & 0x20
	{
		AddData(buffer, Data->ResistPlasma, index);
	}

	if (ExtendedFlags[1] & 0x80)	//ExtendedFlags[3] & 0x40
	{
		AddData(buffer, Data->ResistEnergy, index);
	}

	if (ExtendedFlags[2] & 0x01)	//ExtendedFlags[3] & 0x80
	{
		AddData(buffer, Data->ResistEMP, index);
	}

	if (ExtendedFlags[2] & 0x02)	//ExtendedFlags[4] & 0x01
	{
		AddData(buffer, Data->ResistChemical, index);
	}

	if (ExtendedFlags[2] & 0x04)	//ExtendedFlags[4] & 0x02
	{
		AddData(buffer, Data->ResistPsionic, index);
	}
}

/******************************
*         GET METHODS         *
******************************/

_Stats * AuxStats::GetData()		{return Data;}

s32 AuxStats::GetDefence()			{return Data->Defence;}
s32 AuxStats::GetMissleDefence()	{return Data->MissleDefence;}
s32 AuxStats::GetSpeed()			{return Data->Speed;}
s32 AuxStats::GetWarpSpeed()		{return Data->WarpSpeed;}
s32 AuxStats::GetWarpPowerLevel()	{return Data->WarpPowerLevel;}
s32 AuxStats::GetTurnRate()			{return Data->TurnRate;}
s32 AuxStats::GetScanRange()		{return Data->ScanRange;}
s32 AuxStats::GetVisibility()		{return Data->Visibility;}
s32 AuxStats::GetResistImpact()		{return Data->ResistImpact;}
s32 AuxStats::GetResistExplosion()	{return Data->ResistExplosion;}
s32 AuxStats::GetResistPlasma()		{return Data->ResistPlasma;}
s32 AuxStats::GetResistEnergy()		{return Data->ResistEnergy;}
s32 AuxStats::GetResistEMP()		{return Data->ResistEMP;}
s32 AuxStats::GetResistChemical()	{return Data->ResistChemical;}
s32 AuxStats::GetResistPsionic()	{return Data->ResistPsionic;}

/******************************
*         SET METHODS         *
******************************/

void AuxStats::SetData(_Stats *NewData)
{
	ReplaceData(Data->Defence, NewData->Defence, 0);
	ReplaceData(Data->MissleDefence, NewData->MissleDefence, 1);
	ReplaceData(Data->Speed, NewData->Speed, 2);
	ReplaceData(Data->WarpSpeed, NewData->WarpSpeed, 3);
	ReplaceData(Data->WarpPowerLevel, NewData->WarpPowerLevel, 4);
	ReplaceData(Data->TurnRate, NewData->TurnRate, 5);
	ReplaceData(Data->ScanRange, NewData->ScanRange, 6);
	ReplaceData(Data->Visibility, NewData->Visibility, 7);
	ReplaceData(Data->ResistImpact, NewData->ResistImpact, 8);
	ReplaceData(Data->ResistExplosion, NewData->ResistExplosion, 9);
	ReplaceData(Data->ResistPlasma, NewData->ResistPlasma, 10);
	ReplaceData(Data->ResistEnergy, NewData->ResistEnergy, 11);
	ReplaceData(Data->ResistEMP, NewData->ResistEMP, 12);
	ReplaceData(Data->ResistChemical, NewData->ResistChemical, 13);
	ReplaceData(Data->ResistPsionic, NewData->ResistPsionic, 14);
}

void AuxStats::SetDefence(s32 NewDefence)
{
	ReplaceData(Data->Defence, NewDefence, 0);
}

void AuxStats::SetMissleDefence(s32 NewMissleDefence)
{
	ReplaceData(Data->MissleDefence, NewMissleDefence, 1);
}

void AuxStats::SetSpeed(s32 NewSpeed)
{
	ReplaceData(Data->Speed, NewSpeed, 2);
}

void AuxStats::SetWarpSpeed(s32 NewWarpSpeed)
{
	ReplaceData(Data->WarpSpeed, NewWarpSpeed, 3);
}

void AuxStats::SetWarpPowerLevel(s32 NewWarpPowerLevel)
{
	ReplaceData(Data->WarpPowerLevel , NewWarpPowerLevel, 4);
}

void AuxStats::SetTurnRate(s32 NewTurnRate)
{
	ReplaceData(Data->TurnRate, NewTurnRate, 5);
}

void AuxStats::SetScanRange(s32 NewScanRange)
{
	ReplaceData(Data->ScanRange, NewScanRange, 6);
}

void AuxStats::SetVisibility(s32 NewVisibility)
{
	ReplaceData(Data->Visibility, NewVisibility, 7);
}

void AuxStats::SetResistImpact(s32 NewResistImpact)
{
	ReplaceData(Data->ResistImpact, NewResistImpact, 8);
}

void AuxStats::SetResistExplosion(s32 NewResistExplosion)
{
	ReplaceData(Data->ResistExplosion, NewResistExplosion, 9);
}

void AuxStats::SetResistPlasma(s32 NewResistPlasma)
{
	ReplaceData(Data->ResistPlasma, NewResistPlasma, 10);
}

void AuxStats::SetResistEnergy(s32 NewResistEnergy)
{
	ReplaceData(Data->ResistEnergy, NewResistEnergy, 11);
}

void AuxStats::SetResistEMP(s32 NewResistEMP)
{
	ReplaceData(Data->ResistEMP, NewResistEMP, 12);
}

void AuxStats::SetResistChemical(s32 NewResistChemical)
{
	ReplaceData(Data->ResistChemical, NewResistChemical, 13);
}

void AuxStats::SetResistPsionic(s32 NewResistPsionic)
{
	ReplaceData(Data->ResistPsionic, NewResistPsionic, 14);
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxStats::ClearFlags()
{
	memset(Flags,0,sizeof(Flags));
}