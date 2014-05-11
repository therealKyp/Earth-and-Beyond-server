#include "AuxPlayerIndex.h"

bool AuxPlayerIndex::BuildPacket()
{
	return BuildPacket(PacketBuffer, PacketSize);
}

bool AuxPlayerIndex::BuildPacket(unsigned char *buffer, long &index)
{
	if (!(Flags[0] & 0x02))
	{
		return false;
	}

	index = 0;

	AddData(buffer, u32(0), index);
	AddData(buffer, short(0), index);
	AddData(buffer, char(1), index);

	AddFlags(Flags, sizeof(Flags), buffer, index);

	if (Flags[0] & 0x10)	//ExtendedFlags[2] & 0x40
	{
		AddData(buffer, Data.Credits, index);
	}

	if (Flags[0] & 0x20)	//ExtendedFlags[2] & 0x80
	{
		AddData(buffer, Data.XPDebt, index);
	}

	if (Flags[0] & 0x40)	//ExtendedFlags[3] & 0x01
	{
	    SecureInv.BuildPacket(buffer, index);
	}

	if (Flags[0] & 0x80)	//ExtendedFlags[3] & 0x02
	{
	    VendorInv.BuildPacket(buffer, index);
	}

	if (Flags[1] & 0x01)	//ExtendedFlags[3] & 0x04
	{
	    RewardInv.BuildPacket(buffer, index);
	}

	if (Flags[1] & 0x02)	//ExtendedFlags[3] & 0x08
	{
	    OverflowInv.BuildPacket(buffer, index);
	}

	if (Flags[1] & 0x04)	//ExtendedFlags[3] & 0x10
	{
	    RPGInfo.BuildPacket(buffer, index);
	}

	if (Flags[1] & 0x08)	//ExtendedFlags[3] & 0x20
	{
		AddString(buffer, Data.CommunityEventFlags, index);
	}

	if (Flags[1] & 0x10)	//ExtendedFlags[3] & 0x40
	{
		AddData(buffer,Data.MusicID,index);
	}

	if (Flags[1] & 0x20)	//ExtendedFlags[3] & 0x80
	{
	    Missions.BuildPacket(buffer, index);
	}

	if (Flags[1] & 0x40)	//ExtendedFlags[4] & 0x01
	{
	    Reputation.BuildPacket(buffer, index);
	}

	if (Flags[1] & 0x80)	//ExtendedFlags[4] & 0x02
	{
		AddData(buffer, Data.PIPAvatarID, index);
	}

	if (Flags[2] & 0x01)	//ExtendedFlags[4] & 0x04
	{
		AddString(buffer, Data.RegistrationStarbase, index);
	}

	if (Flags[2] & 0x02)	//ExtendedFlags[4] & 0x08
	{
		AddString(buffer, Data.RegistrationStarbaseSector, index);
	}

	if (Flags[2] & 0x04)	//ExtendedFlags[4] & 0x10
	{
		AddString(buffer, Data.SectorName, index);
	}

	if (Flags[2] & 0x08)	//ExtendedFlags[4] & 0x20
	{
		AddData(buffer, Data.SectorNum, index);
	}

	if (Flags[2] & 0x10)	//ExtendedFlags[4] & 0x40
	{
		AddData(buffer, Data.ClientSendUITriggers ,index);
	}

	if (Flags[2] & 0x20)	//ExtendedFlags[4] & 0x80
	{
	    GroupInfo.BuildPacket(buffer, index);
	}

	*((short *) &buffer[4]) = short(index - 6);

	memset(Flags,0,sizeof(Flags));

	return true;
}

bool AuxPlayerIndex::BuildExtendedPacket()
{
	return BuildExtendedPacket(PacketBuffer, PacketSize);
}

bool AuxPlayerIndex::BuildExtendedPacket(unsigned char *buffer, long &index)
{
	if (!(ExtendedFlags[0] & 0x02))
	{
		return false;
	}

	index = 0;

	AddData(buffer, u32(0), index);
	AddData(buffer, short(0), index);
	AddData(buffer, char(1), index);

	AddFlags(ExtendedFlags, sizeof(ExtendedFlags), buffer, index);

	if (ExtendedFlags[0] & 0x10)	//ExtendedFlags[2] & 0x40
	{
		AddData(buffer, Data.Credits, index);
	}

	if (ExtendedFlags[0] & 0x20)	//ExtendedFlags[2] & 0x80
	{
		AddData(buffer, Data.XPDebt, index);
	}

	if (ExtendedFlags[0] & 0x40)	//ExtendedFlags[3] & 0x01
	{
	    SecureInv.BuildExtendedPacket(buffer, index);
	}
	else if (ExtendedFlags[3] & 0x01)
	{
		AddData(buffer,char(0x05),index);
	}

	if (ExtendedFlags[0] & 0x80)	//ExtendedFlags[3] & 0x02
	{
	    VendorInv.BuildExtendedPacket(buffer, index);
	}
	else if (ExtendedFlags[3] & 0x02)
	{
		AddData(buffer,char(0x05),index);
	}

	if (ExtendedFlags[1] & 0x01)	//ExtendedFlags[3] & 0x04
	{
	    RewardInv.BuildExtendedPacket(buffer, index);
	}
	else if (ExtendedFlags[3] & 0x04)
	{
		AddData(buffer,char(0x05),index);
	}

	if (ExtendedFlags[1] & 0x02)	//ExtendedFlags[3] & 0x08
	{
	    OverflowInv.BuildExtendedPacket(buffer, index);
	}
	else if (ExtendedFlags[3] & 0x08)
	{
		AddData(buffer,char(0x05),index);
	}

	if (ExtendedFlags[1] & 0x04)	//ExtendedFlags[3] & 0x10
	{
	    RPGInfo.BuildExtendedPacket(buffer, index);
	}
	else if (ExtendedFlags[3] & 0x10)
	{
		AddData(buffer,char(0x05),index);
	}

	if (ExtendedFlags[1] & 0x08)	//ExtendedFlags[3] & 0x20
	{
		AddString(buffer, Data.CommunityEventFlags, index);
	}

	if (ExtendedFlags[1] & 0x10)	//ExtendedFlags[3] & 0x40
	{
		AddData(buffer,Data.MusicID,index);
	}

	if (ExtendedFlags[1] & 0x20)	//ExtendedFlags[3] & 0x80
	{
	    Missions.BuildExtendedPacket(buffer, index);
	}
	else if (ExtendedFlags[3] & 0x80)
	{
		AddData(buffer,char(0x05),index);
	}

	if (ExtendedFlags[1] & 0x40)	//ExtendedFlags[4] & 0x01
	{
	    Reputation.BuildExtendedPacket(buffer, index);
	}
	else if (ExtendedFlags[4] & 0x01)
	{
		AddData(buffer,char(0x05),index);
	}

	if (ExtendedFlags[1] & 0x80)	//ExtendedFlags[4] & 0x02
	{
		AddData(buffer, Data.PIPAvatarID, index);
	}

	if (ExtendedFlags[2] & 0x01)	//ExtendedFlags[4] & 0x04
	{
		AddString(buffer, Data.RegistrationStarbase, index);
	}

	if (ExtendedFlags[2] & 0x02)	//ExtendedFlags[4] & 0x08
	{
		AddString(buffer, Data.RegistrationStarbaseSector, index);
	}

	if (ExtendedFlags[2] & 0x04)	//ExtendedFlags[4] & 0x10
	{
		AddString(buffer, Data.SectorName, index);
	}

	if (ExtendedFlags[2] & 0x08)	//ExtendedFlags[4] & 0x20
	{
		AddData(buffer, Data.SectorNum, index);
	}

	if (ExtendedFlags[2] & 0x10)	//ExtendedFlags[4] & 0x40
	{
		AddData(buffer, Data.ClientSendUITriggers ,index);
	}

	if (ExtendedFlags[2] & 0x20)	//ExtendedFlags[4] & 0x80
	{
	    GroupInfo.BuildExtendedPacket(buffer, index);
	}
	else if (ExtendedFlags[4] & 0x80)
	{
		AddData(buffer,char(0x05),index);
	}

	*((short *) &buffer[4]) = short(index - 6);

	return true;
}

/******************************
*         GET METHODS         *
******************************/

_PlayerIndex * AuxPlayerIndex::GetData()                {return &Data;}

u64 AuxPlayerIndex::GetCredits()						{return Data.Credits;}
u32 AuxPlayerIndex::GetXPDebt()							{return Data.XPDebt;}
char * AuxPlayerIndex::GetCommunityEventFlags()			{return Data.CommunityEventFlags;}
u32 AuxPlayerIndex::GetMusicID()						{return Data.MusicID;}
u32 AuxPlayerIndex::GetPIPAvatarID()					{return Data.PIPAvatarID;}
char * AuxPlayerIndex::GetRegistrationStarbase()		{return Data.RegistrationStarbase;}
char * AuxPlayerIndex::GetRegistrationStarbaseSector()	{return Data.RegistrationStarbaseSector;}
char * AuxPlayerIndex::GetSectorName()					{return Data.SectorName;}
u32 AuxPlayerIndex::GetSectorNum()						{return Data.SectorNum;}
u32 AuxPlayerIndex::GetClientSendUITriggers()			{return Data.ClientSendUITriggers;}

/******************************
*         SET METHODS         *
******************************/

void AuxPlayerIndex::SetData(_PlayerIndex *NewData)
{
	ReplaceData(Data.Credits, NewData->Credits, 0);
	ReplaceData(Data.XPDebt, NewData->XPDebt, 1);

	SecureInv.SetData(&NewData->SecureInv);
	VendorInv.SetData(&NewData->VendorInv);
	RewardInv.SetData(&NewData->RewardInv);
	OverflowInv.SetData(&NewData->OverflowInv);
	RPGInfo.SetData(&NewData->RPGInfo);

	ReplaceString(Data.CommunityEventFlags, NewData->CommunityEventFlags, 7);
	ReplaceData(Data.MusicID, NewData->MusicID, 8);

	Missions.SetData(&NewData->Missions);
	Reputation.SetData(&NewData->Reputation);

	ReplaceData(Data.PIPAvatarID, NewData->PIPAvatarID, 11);
	ReplaceString(Data.RegistrationStarbase, NewData->RegistrationStarbase, 12);
	ReplaceString(Data.RegistrationStarbaseSector, NewData->RegistrationStarbaseSector, 13);
	ReplaceString(Data.SectorName, NewData->SectorName, 14);
	ReplaceData(Data.SectorNum, NewData->SectorNum, 15);
	ReplaceData(Data.ClientSendUITriggers, NewData->ClientSendUITriggers, 16);

	GroupInfo.SetData(&NewData->GroupInfo);
}

void AuxPlayerIndex::SetCredits(u64 NewCredits)
{
	ReplaceData(Data.Credits, NewCredits, 0);
}

void AuxPlayerIndex::SetXPDebt(u32 NewXPDebt)
{
	ReplaceData(Data.XPDebt, NewXPDebt, 1);
}

void AuxPlayerIndex::SetCommunityEventFlags(char * NewCommunityEventFlags)
{
	ReplaceString(Data.CommunityEventFlags, NewCommunityEventFlags, 7);
}

void AuxPlayerIndex::SetMusicID(u32 NewMusicID)
{
	ReplaceData(Data.MusicID, NewMusicID, 8);
}

void AuxPlayerIndex::SetPIPAvatarID(u32 NewPIPAvatarID)
{
	ReplaceData(Data.PIPAvatarID, NewPIPAvatarID, 11);
}

void AuxPlayerIndex::SetRegistrationStarbase(const char * NewRegistrationStarbase)
{
	ReplaceString(Data.RegistrationStarbase, NewRegistrationStarbase, 12);
}

void AuxPlayerIndex::SetRegistrationStarbaseSector(const char * NewRegistrationStarbaseSector)
{
	ReplaceString(Data.RegistrationStarbaseSector, NewRegistrationStarbaseSector, 13);
}

void AuxPlayerIndex::SetSectorName(char * NewSectorName)
{
	ReplaceString(Data.SectorName, NewSectorName, 14);
}

void AuxPlayerIndex::SetSectorNum(u32 NewSectorNum)
{
	ReplaceData(Data.SectorNum, NewSectorNum, 15);
}

void AuxPlayerIndex::SetClientSendUITriggers(u32 NewClientSendUITriggers)
{
	ReplaceData(Data.ClientSendUITriggers, NewClientSendUITriggers, 16);
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxPlayerIndex::Reset()
{
	Data.Credits = 0;
	Data.XPDebt = 0;
	SecureInv.Init(2, this, &Data.SecureInv);
	VendorInv.Init(3, this, &Data.VendorInv);
	RewardInv.Init(4, this, &Data.RewardInv);
	OverflowInv.Init(5, this, &Data.OverflowInv);
	RPGInfo.Init(6, this, &Data.RPGInfo);
	*Data.CommunityEventFlags = 0;
	Data.MusicID = 0;
	Missions.Init(9, this, &Data.Missions);
	Reputation.Init(10, this, &Data.Reputation);
	Data.PIPAvatarID = 0;
	*Data.RegistrationStarbase = 0;
	*Data.RegistrationStarbaseSector = 0;
	*Data.SectorName = 0;
	Data.SectorNum = 0;
	Data.ClientSendUITriggers = 0;
	GroupInfo.Init(17, this, &Data.GroupInfo);

	memset(Flags,0, sizeof(Flags));

	ExtendedFlags[0] = char(0xC6);
	ExtendedFlags[1] = char(0x64);
	ExtendedFlags[2] = char(0xE0);
	ExtendedFlags[3] = char(0xFF);
	ExtendedFlags[4] = char(0xFF);
}

void AuxPlayerIndex::ClearFlags()
{
	memset(Flags,0, sizeof(Flags));
	SecureInv.ClearFlags();
	VendorInv.ClearFlags();
	RewardInv.ClearFlags();
	OverflowInv.ClearFlags();
	RPGInfo.ClearFlags();
	Missions.ClearFlags();
	Reputation.ClearFlags();
	GroupInfo.ClearFlags();
}
