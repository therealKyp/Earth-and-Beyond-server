#include "AuxMobIndex.h"

void AuxMobIndex::SetupPackets()
{
    BuildCreatePacket();
    m_ClickPacket = &m_PacketBuffer[m_CreateSize + 100];  //Leave 100 as a floating buffer just incase

    BuildClickPacket();
    m_DiffPacket = &m_PacketBuffer[m_CreateSize + m_ClickSize + 200];
}

bool AuxMobIndex::BuildDiffPacket(bool shields, bool hull)
{
	if (shields)
	{
		Flags[0] |= 0x02;
		Flags[1] |= 0x04;
	}

	if (hull)
	{
		Flags[0] |= 0x02;
		Flags[1] |= 0x10;
	}

	return BuildDiffPacket(m_DiffPacket, m_DiffSize);
}

bool AuxMobIndex::BuildDiffPacket(unsigned char *buffer, long &index)
{
    //The only thing that can change on a mob is its Shields/Hull
    //Along with all of the Interup members and the EngineState
    //Possibly the IS flags
    //The rest of the data is send with the Create/Click packets and WILL NOT CHANGE

	index = 0;

	if (!GameID || !(Flags[0] & 0x02))
	{
		return false;
	}

	AddData(buffer, GameID, index);
	AddData(buffer, short(0), index);
	AddData(buffer, char(1), index);

	AddFlags(Flags, sizeof(Flags), buffer, index);

	if (Flags[1] & 0x04)	//ExtendedFlags[8] & 0x10
	{
		Shield.BuildPacket(buffer, index);
	}

	if (Flags[1] & 0x10)	//ExtendedFlags[8] & 0x40
	{
		AddData(buffer,Data.HullPoints,index);
	}

	if (Flags[3] & 0x01)	//ExtendedFlags[10] & 0x04
	{
		AddData(buffer,char(Data.IsCountermeasureActive),index);
	}

	if (Flags[3] & 0x02)	//ExtendedFlags[10] & 0x08
	{
		AddData(buffer,char(Data.IsIncapacitated),index);
	}

	if (Flags[3] & 0x04)	//ExtendedFlags[10] & 0x10
	{
		AddData(buffer,char(Data.IsOrganic),index);
	}

	if (Flags[3] & 0x08)	//ExtendedFlags[10] & 0x20
	{
		AddData(buffer,char(Data.IsInPVP),index);
	}

	if (Flags[3] & 0x20)	//ExtendedFlags[10] & 0x80
	{
		AddData(buffer,char(Data.IsRescueBeaconActive),index);
	}

	if (Flags[4] & 0x10)	//ExtendedFlags[11] & 0x40
	{
		AddData(buffer,Data.GlobalWarpState,index);
	}

	if (Flags[5] & 0x02)	//ExtendedFlags[12] & 0x08
	{
		QuadrantDamage.BuildPacket(buffer, index);
	}

	if (Flags[5] & 0x04)	//ExtendedFlags[12] & 0x10
	{
		DamageSpot.BuildPacket(buffer, index);
	}

	if (Flags[5] & 0x08)	//ExtendedFlags[12] & 0x20
	{
		DamageLine.BuildPacket(buffer, index);
	}

	if (Flags[5] & 0x10)	//ExtendedFlags[12] & 0x40
	{
		DamageBlotch.BuildPacket(buffer, index);
	}

	if (Flags[6] & 0x04)	//ExtendedFlags[13] & 0x10
	{
		AddData(buffer,Data.EngineThrustState,index);
	}

	if (Flags[7] & 0x02)	//ExtendedFlags[14] & 0x08
	{
		AddString(buffer,Data.InterruptibleAbilityName,index);
	}

	if (Flags[7] & 0x04)	//ExtendedFlags[14] & 0x10
	{
		AddData(buffer,Data.InterruptState,index);
	}

	if (Flags[7] & 0x08)	//ExtendedFlags[14] & 0x20
	{
		AddData(buffer,Data.InterruptibleActivationTime,index);
	}

	if (Flags[7] & 0x10)	//ExtendedFlags[14] & 0x40
	{
		AddData(buffer,Data.InterruptProgress,index);
	}

	*((short *) &buffer[4]) = short(index - 6);

	memset(Flags,0,sizeof(Flags));

	return true;
}

void AuxMobIndex::BuildCreatePacket()
{
	BuildCreatePacket(m_CreatePacket, m_CreateSize);
}

void AuxMobIndex::BuildCreatePacket(unsigned char *buffer, long &index)
{
	/*
	Needs the following indexes by my estimations:

	0, 1, 2, 3, 9, 19, 20, 21, 22, 23, 25, 26,
	32, 37, 38, 39, 40, 41, 46, 47

    Name, Owner, Title, Rank, MaxHullPoints, IsCloaked, IsCountermeasureActive
    IsIncapacitated, IsOrganic, IsInPVP, IsRescueBeaconActive
    CombatLevel, GlobalWarpState, QuadrantDamage, DamageSpot
    DamageLine, DamageBlotch, Legos, EngineThrustState, EngineTrailType
	*/

	index = 0;

	if (!GameID)
	{
		return;
	}

	AddData(buffer, GameID, index);
	AddData(buffer, short(0), index);
	AddData(buffer, char(1), index);


	AddData(buffer, ExtendedFlags[0], index);               // Indexes 0,1,2,3
	AddData(buffer, char(ExtendedFlags[1] & 0x20), index);  // Indexes 9
	AddData(buffer, char(ExtendedFlags[2] & 0x80), index);  // Indexes 19
	AddData(buffer, char(ExtendedFlags[3] & 0x6F), index);  // Indexes 20,21,22,23,25,26
	AddData(buffer, char(ExtendedFlags[4] & 0x10), index);  // Indexes 32
	AddData(buffer, char(ExtendedFlags[5] & 0x3E), index);  // Indexes 37,38,39,40,41
	AddData(buffer, char(ExtendedFlags[6] & 0x0C), index);  // Indexes 46,47
	AddData(buffer, char(ExtendedFlags[7] & 0xC0), index);  // Indexes rollaround to 0,1
	AddData(buffer, char(ExtendedFlags[8] & 0x83), index);  // Indexes 2,3,9
	AddData(buffer, char(0), index);                        // None
	AddData(buffer, char(ExtendedFlags[10] & 0xBE), index); // Indexes 19,20,21,22,23,25
	AddData(buffer, char(ExtendedFlags[11] & 0x42), index); // Indexes 26,32
	AddData(buffer, char(ExtendedFlags[12] & 0xF8), index); // Indexes 37,38,39,40,41
	AddData(buffer, char(ExtendedFlags[13] & 0x30), index); // Indexes 46,47
	AddData(buffer, char(0), index);                        // None

    // Since the header is preceding, there are 4 + 2 + 1 bytes before this

	if (buffer[7] & 0x10)	//Index 0
	{
		AddString(buffer,Data.Name,index);
	}

	if (buffer[7] & 0x20)	//Index 1
	{
		AddString(buffer,Data.Owner,index);
	}

	if (buffer[7] & 0x40)	//Index 2
	{
		AddString(buffer,Data.Title,index);
	}

	if (buffer[7] & 0x80)	//Index 3
	{
		AddString(buffer,Data.Rank,index);
	}

	if (buffer[8] & 0x20)	//Index 9
	{
		AddData(buffer,Data.MaxHullPoints,index);
	}

	if (buffer[9] & 0x80)	//Index 19
	{
		AddData(buffer,char(Data.IsCloaked),index);
	}

	if (buffer[10] & 0x01)	//Index 20
	{
		AddData(buffer,char(Data.IsCountermeasureActive),index);
	}

	if (buffer[10] & 0x02)	//Index 21
	{
		AddData(buffer,char(Data.IsIncapacitated),index);
	}

	if (buffer[10] & 0x04)	//Index 22
	{
		AddData(buffer,char(Data.IsOrganic),index);
	}

	if (buffer[10] & 0x08)	//Index 23
	{
		AddData(buffer,char(Data.IsInPVP),index);
	}

	if (buffer[10] & 0x20)	//Index 25
	{
		AddData(buffer,char(Data.IsRescueBeaconActive),index);
	}

	if (buffer[10] & 0x40)	//Index 26
	{
		AddData(buffer,Data.CombatLevel,index);
	}

	if (buffer[11] & 0x10)	//Index 32
	{
		AddData(buffer,Data.GlobalWarpState,index);
	}

	if (buffer[12] & 0x02)	//Index 37
	{
		QuadrantDamage.BuildExtendedPacket(buffer, index);
	}
	else if (buffer[19] & 0x08)
	{
		AddData(buffer,char(0x05),index);
	}

	if (buffer[12] & 0x04)	//Index 38
	{
		DamageSpot.BuildExtendedPacket(buffer, index);
	}
	else if (buffer[19] & 0x10)
	{
		AddData(buffer,char(0x05),index);
	}

	if (buffer[12] & 0x08)	//Index 39
	{
		DamageLine.BuildExtendedPacket(buffer, index);
	}
	else if (buffer[19] & 0x20)
	{
		AddData(buffer,char(0x05),index);
	}

	if (buffer[12] & 0x10)	//Index 40
	{
		DamageBlotch.BuildExtendedPacket(buffer, index);
	}
	else if (buffer[19] & 0x40)
	{
		AddData(buffer,char(0x05),index);
	}

	if (buffer[12] & 0x20)	//Index 41
	{
		Lego.BuildExtendedPacket(buffer, index);
	}
	else if (buffer[19] & 0x80)
	{
		AddData(buffer,char(0x05),index);
	}

	if (buffer[13] & 0x04)	//Index 46
	{
		AddData(buffer,Data.EngineThrustState,index);
	}

	if (buffer[13] & 0x08)	//Index 47
	{
		AddData(buffer,Data.EngineTrailType,index);
	}

	*((short *) &buffer[4]) = short(index - 6);
}

void AuxMobIndex::BuildClickPacket()
{
    BuildClickPacket(m_ClickPacket, m_ClickSize);
}

void AuxMobIndex::BuildClickPacket(unsigned char *buffer, long &index)
{
	/*
	Needs the following indexes by my estimations:

	6, 7, 8, 53, 54, 55, 56, 57

    Shields, MaxShield, HullLevel, InterruptibleAbilityName, InterruptState,
    InterruptibleActivationTime, InterruptProgress, FactionIdentifier
	*/

	index = 0;

	if (!GameID)
	{
		return;
	}

	AddData(buffer, GameID, index);
	AddData(buffer, short(0), index);
	AddData(buffer, char(1), index);

	AddData(buffer, char(0x06), index);                     // None
	AddData(buffer, char(ExtendedFlags[1] & 0x1C), index);  // Indexes 6, 7, 8
	AddData(buffer, char(0), index);                        // None
	AddData(buffer, char(0), index);                        // None
	AddData(buffer, char(0), index);                        // None
	AddData(buffer, char(0), index);                        // None
	AddData(buffer, char(0), index);                        // None
	AddData(buffer, char(ExtendedFlags[7] & 0x3E), index);  // Indexes 53, 54, 55, 56, 57
	AddData(buffer, char(ExtendedFlags[8] & 0x70), index);  // Indexes 6, 7, 8
	AddData(buffer, char(0), index);                        // None
	AddData(buffer, char(0), index);                        // None
	AddData(buffer, char(0), index);                        // None
	AddData(buffer, char(0), index);                        // None
	AddData(buffer, char(0), index);                        // None
	AddData(buffer, char(ExtendedFlags[14] & 0xF8), index); // Indexes 53, 54, 55, 56, 57

	if (buffer[8] & 0x04)	//Index 6
	{
		Shield.BuildExtendedPacket(buffer, index);
	}
	else if (buffer[15] & 0x10)
	{
		AddData(buffer,char(0x05),index);
	}

	if (buffer[8] & 0x08)	//Index 7
	{
		AddData(buffer,Data.MaxShield,index);
	}

	if (buffer[8] & 0x10)	//Index 8
	{
		AddData(buffer,Data.HullPoints,index);
	}

	if (buffer[14] & 0x02)	//Index 53
	{
		AddString(buffer,Data.InterruptibleAbilityName,index);
	}

	if (buffer[14] & 0x04)	//Index 54
	{
		AddData(buffer,Data.InterruptState,index);
	}

	if (buffer[14] & 0x08)	//Index 55
	{
		AddData(buffer,Data.InterruptibleActivationTime,index);
	}

	if (buffer[14] & 0x10)	//Index 56
	{
		AddData(buffer,Data.InterruptProgress,index);
	}

	if (buffer[14] & 0x20)	//Index 57
	{
		AddString(buffer,Data.FactionIdentifier,index);
	}

	*((short *) &buffer[4]) = short(index - 6);

	memset(Flags,0,sizeof(Flags));
}


/******************************
*         GET METHODS         *
******************************/

_MobIndex * AuxMobIndex::GetData()                  {return &Data;}

u32 AuxMobIndex::GetGameID()						{return GameID;}

char * AuxMobIndex::GetName()						{return Data.Name;}
char * AuxMobIndex::GetOwner()						{return Data.Owner;}
char * AuxMobIndex::GetTitle()						{return Data.Title;}
char * AuxMobIndex::GetRank()						{return Data.Rank;}
float AuxMobIndex::GetMaxShield()					{return Data.MaxShield;}
float AuxMobIndex::GetHullPoints()					{return Data.HullPoints;}
float AuxMobIndex::GetMaxHullPoints()				{return Data.MaxHullPoints;}
bool AuxMobIndex::GetIsCloaked()					{return Data.IsCloaked;}
bool AuxMobIndex::GetIsCountermeasureActive()		{return Data.IsCountermeasureActive;}
bool AuxMobIndex::GetIsIncapacitated()				{return Data.IsIncapacitated;}
bool AuxMobIndex::GetIsOrganic()					{return Data.IsOrganic;}
bool AuxMobIndex::GetIsInPVP()						{return Data.IsInPVP;}
bool AuxMobIndex::GetIsRescueBeaconActive()		    {return Data.IsRescueBeaconActive;}
u32 AuxMobIndex::GetCombatLevel()					{return Data.CombatLevel;}
u32 AuxMobIndex::GetGlobalWarpState()				{return Data.GlobalWarpState;}
u32 AuxMobIndex::GetEngineThrustState()			    {return Data.EngineThrustState;}
u32 AuxMobIndex::GetEngineTrailType()				{return Data.EngineTrailType;}
char * AuxMobIndex::GetInterruptibleAbilityName()	{return Data.InterruptibleAbilityName;}
u32 AuxMobIndex::GetInterruptState()				{return Data.InterruptState;}
u32 AuxMobIndex::GetInterruptibleActivationTime()	{return Data.InterruptibleActivationTime;}
float AuxMobIndex::GetInterruptProgress()			{return Data.InterruptProgress;}
char * AuxMobIndex::GetFactionIdentifier()			{return Data.FactionIdentifier;}

/******************************
*         SET METHODS         *
******************************/

void AuxMobIndex::SetData(_MobIndex *NewData)
{
	ReplaceString(Data.Name, NewData->Name, 0);
	ReplaceString(Data.Owner, NewData->Owner, 1);
	ReplaceString(Data.Title, NewData->Title, 2);
	ReplaceString(Data.Rank, NewData->Rank, 3);
	Shield.SetData(&NewData->Shield);
	ReplaceData(Data.MaxShield, NewData->MaxShield, 7);
	ReplaceData(Data.HullPoints, NewData->HullPoints, 8);
	ReplaceData(Data.MaxHullPoints, NewData->MaxHullPoints, 9);
	ReplaceData(Data.IsCloaked, NewData->IsCloaked, 19);
	ReplaceData(Data.IsCountermeasureActive, NewData->IsCountermeasureActive, 20);
	ReplaceData(Data.IsIncapacitated, NewData->IsIncapacitated, 21);
	ReplaceData(Data.IsOrganic, NewData->IsOrganic, 22);
	ReplaceData(Data.IsInPVP, NewData->IsInPVP, 23);
	ReplaceData(Data.IsRescueBeaconActive, NewData->IsRescueBeaconActive, 25);
	ReplaceData(Data.CombatLevel, NewData->CombatLevel, 26);
	ReplaceData(Data.GlobalWarpState, NewData->GlobalWarpState, 32);

    QuadrantDamage.SetData(&NewData->QuadrantDamage);
	DamageSpot.SetData(&NewData->DamageSpot);
	DamageLine.SetData(&NewData->DamageLine);
	DamageBlotch.SetData(&NewData->DamageBlotch);
	Lego.SetData(&NewData->Lego);

    ReplaceData(Data.EngineThrustState, NewData->EngineThrustState, 46);
	ReplaceData(Data.EngineTrailType, NewData->EngineTrailType, 47);

    ReplaceString(Data.InterruptibleAbilityName, NewData->InterruptibleAbilityName, 53);
	ReplaceData(Data.InterruptState, NewData->InterruptState, 54);
	ReplaceData(Data.InterruptibleActivationTime, NewData->InterruptibleActivationTime, 55);
	ReplaceData(Data.InterruptProgress, NewData->InterruptProgress, 56);
	ReplaceString(Data.FactionIdentifier, NewData->FactionIdentifier, 57);
}

void AuxMobIndex::SetGameID(u32 NewGameID)
{
	GameID = NewGameID;
}

void AuxMobIndex::SetName(char *NewName)
{
	ReplaceString(Data.Name, NewName, 0);
}

void AuxMobIndex::SetOwner(char *NewOwner)
{
	ReplaceString(Data.Owner, NewOwner, 1);
}

void AuxMobIndex::SetTitle(char *NewTitle)
{
	ReplaceString(Data.Title, NewTitle, 2);
}

void AuxMobIndex::SetRank(char *NewRank)
{
	ReplaceString(Data.Rank, NewRank, 3);
}

void AuxMobIndex::SetMaxShield(float NewMaxShield)
{
	ReplaceData(Data.MaxShield, NewMaxShield, 7);
}

void AuxMobIndex::SetHullPoints(float NewHullPoints)
{
	ReplaceData(Data.HullPoints, NewHullPoints, 8);
}

void AuxMobIndex::SetMaxHullPoints(float NewMaxHullPoints)
{
	ReplaceData(Data.MaxHullPoints, NewMaxHullPoints, 9);
}

void AuxMobIndex::SetIsCloaked(bool NewIsCloaked)
{
	ReplaceData(Data.IsCloaked, NewIsCloaked, 19);
}

void AuxMobIndex::SetIsCountermeasureActive(bool NewIsCountermeasureActive)
{
	ReplaceData(Data.IsCountermeasureActive, NewIsCountermeasureActive, 20);
}

void AuxMobIndex::SetIsIncapacitated(bool NewIsIncapacitated)
{
	ReplaceData(Data.IsIncapacitated, NewIsIncapacitated, 21);
}

void AuxMobIndex::SetIsOrganic(bool NewIsOrganic)
{
	ReplaceData(Data.IsOrganic, NewIsOrganic, 22);
}

void AuxMobIndex::SetIsInPVP(bool NewIsInPVP)
{
	ReplaceData(Data.IsInPVP, NewIsInPVP, 23);
}

void AuxMobIndex::SetIsRescueBeaconActive(bool NewIsRescueBeaconActive)
{
	ReplaceData(Data.IsRescueBeaconActive, NewIsRescueBeaconActive, 25);
}

void AuxMobIndex::SetCombatLevel(u32 NewCombatLevel)
{
	ReplaceData(Data.CombatLevel, NewCombatLevel, 26);
}

void AuxMobIndex::SetGlobalWarpState(u32 NewGlobalWarpState)
{
	ReplaceData(Data.GlobalWarpState, NewGlobalWarpState, 32);
}

void AuxMobIndex::SetEngineThrustState(u32 NewEngineThrustState)
{
	ReplaceData(Data.EngineThrustState, NewEngineThrustState, 46);
}

void AuxMobIndex::SetEngineTrailType(u32 NewEngineTrailType)
{
	ReplaceData(Data.EngineTrailType, NewEngineTrailType, 47);
}

void AuxMobIndex::SetInterruptibleAbilityName(char * NewInterruptibleAbilityName)
{
	ReplaceString(Data.InterruptibleAbilityName, NewInterruptibleAbilityName, 53);
}

void AuxMobIndex::SetInterruptState(u32 NewInterruptState)
{
	ReplaceData(Data.InterruptState, NewInterruptState, 54);
}

void AuxMobIndex::SetInterruptibleActivationTime(u32 NewInterruptibleActivationTime)
{
	ReplaceData(Data.InterruptibleActivationTime, NewInterruptibleActivationTime, 55);
}

void AuxMobIndex::SetInterruptProgress(float NewInterruptProgress)
{
	ReplaceData(Data.InterruptProgress, NewInterruptProgress, 56);
}

void AuxMobIndex::SetFactionIdentifier(const char * NewFactionIdentifier)
{
	ReplaceString(Data.FactionIdentifier, NewFactionIdentifier, 57);
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxMobIndex::Reset()
{
	memset(Flags,0, sizeof(Flags));

	GameID = 0;

	*Data.Name = 0;
	*Data.Title = 0;
	*Data.Owner = 0;
	*Data.Rank = 0;

    Shield.Init(6, this, &Data.Shield);
	Data.MaxShield = 0;
	Data.HullPoints = 0;
	Data.MaxHullPoints = 0;

    Data.IsCloaked = 0;
	Data.IsCountermeasureActive = 0;
	Data.IsIncapacitated = 0;
	Data.IsOrganic = 0;
	Data.IsInPVP = 0;
	Data.IsRescueBeaconActive = 0;
	Data.CombatLevel = 0;

	Data.GlobalWarpState = 0;

	QuadrantDamage.Init(37, this, &Data.QuadrantDamage);
	DamageSpot.Init(38, this, &Data.DamageSpot);
	DamageLine.Init(39, this, &Data.DamageLine);
	DamageBlotch.Init(40, this, &Data.DamageBlotch);
	Lego.Init(41, this, &Data.Lego);

	Data.EngineThrustState = 0;
	Data.EngineTrailType = 0;

	*Data.InterruptibleAbilityName = 0;
	Data.InterruptState = 0;
	Data.InterruptibleActivationTime = 0;
	Data.InterruptProgress = 0;
	*Data.FactionIdentifier = 0;

	ExtendedFlags[0] = char(0x06);
	ExtendedFlags[1] = char(0x04);  //Shield
	ExtendedFlags[2] = char(0x00);
	ExtendedFlags[3] = char(0x00);
	ExtendedFlags[4] = char(0x00);
	ExtendedFlags[5] = char(0x20);  //Lego
	ExtendedFlags[6] = char(0x00);
	ExtendedFlags[7] = char(0xC0);
	ExtendedFlags[8] = char(0xF3);
	ExtendedFlags[9] = char(0x00);
	ExtendedFlags[10] = char(0xBE);
	ExtendedFlags[11] = char(0x41);
	ExtendedFlags[12] = char(0xF8);
	ExtendedFlags[13] = char(0x30);
	ExtendedFlags[14] = char(0xF8);
}

void AuxMobIndex::ClearFlags()
{
	memset(Flags,0, sizeof(Flags));
    Shield.ClearFlags();
	QuadrantDamage.ClearFlags();
	DamageSpot.ClearFlags();
	DamageLine.ClearFlags();
	DamageBlotch.ClearFlags();
	Lego.ClearFlags();
}

Mutex * AuxMobIndex::Buffer()
{
    return &m_BufferMutex;
}
