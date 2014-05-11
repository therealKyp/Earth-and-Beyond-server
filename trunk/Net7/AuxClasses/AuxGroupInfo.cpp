#include "AuxGroupInfo.h"

void AuxGroupInfo::BuildPacket(unsigned char *buffer, long &index)
{
	AddFlags(Flags, sizeof(Flags), buffer, index);

	if (Flags[0] & 0x10)	//ExtendedFlags[1] & 0x80
	{
		AddData(buffer, char(Data->IsGroupLeader), index);
	}

	if (Flags[0] & 0x20)	//ExtendedFlags[2] & 0x01
	{
		AddData(buffer, char(Data->LookingForGroup), index);
	}

	if (Flags[0] & 0x40)	//ExtendedFlags[2] & 0x02
	{
		AddData(buffer, char(Data->AllowGroupInvite), index);
	}

	if (Flags[0] & 0x80)	//ExtendedFlags[2] & 0x04
	{
		AddData(buffer, char(Data->ShowNonCombatantActivities), index);
	}

	if (Flags[1] & 0x01)	//ExtendedFlags[2] & 0x08
	{
		AddData(buffer, char(Data->ForceAutoSplit), index);
	}

	if (Flags[1] & 0x02)	//ExtendedFlags[2] & 0x10
	{
		AddData(buffer, char(Data->RestrictedLootingRights), index);
	}

	if (Flags[1] & 0x04)	//ExtendedFlags[2] & 0x20
	{
		AddData(buffer, char(Data->AutoReleaseLootingRights), index);
	}

	if (Flags[1] & 0x08)	//ExtendedFlags[2] & 0x40
	{
		AddString(buffer, Data->FormationName, index);
	}

	if (Flags[1] & 0x10)	//ExtendedFlags[2] & 0x80
	{
		AddData(buffer, Data->Formation, index);
	}

	if (Flags[1] & 0x20)	//ExtendedFlags[3] & 0x01
	{
		AddData(buffer, Data->Position, index);
	}

	if (Flags[1] & 0x40)	//ExtendedFlags[3] & 0x80
	{
		Members.BuildPacket(buffer, index);
	}

	memset(Flags,0,sizeof(Flags));
}

void AuxGroupInfo::BuildExtendedPacket(unsigned char *buffer, long &index)
{
	AddFlags(ExtendedFlags, sizeof(ExtendedFlags), buffer, index);

	if (ExtendedFlags[0] & 0x10)	//ExtendedFlags[1] & 0x80
	{
		AddData(buffer, char(Data->IsGroupLeader), index);
	}

	if (ExtendedFlags[0] & 0x20)	//ExtendedFlags[2] & 0x01
	{
		AddData(buffer, char(Data->LookingForGroup), index);
	}

	if (ExtendedFlags[0] & 0x40)	//ExtendedFlags[2] & 0x02
	{
		AddData(buffer, char(Data->AllowGroupInvite), index);
	}

	if (ExtendedFlags[0] & 0x80)	//ExtendedFlags[2] & 0x04
	{
		AddData(buffer, char(Data->ShowNonCombatantActivities), index);
	}

	if (ExtendedFlags[1] & 0x01)	//ExtendedFlags[2] & 0x08
	{
		AddData(buffer, char(Data->ForceAutoSplit), index);
	}

	if (ExtendedFlags[1] & 0x02)	//ExtendedFlags[2] & 0x10
	{
		AddData(buffer, char(Data->RestrictedLootingRights), index);
	}

	if (ExtendedFlags[1] & 0x04)	//ExtendedFlags[2] & 0x20
	{
		AddData(buffer, char(Data->AutoReleaseLootingRights), index);
	}

	if (ExtendedFlags[1] & 0x08)	//ExtendedFlags[2] & 0x40
	{
		AddString(buffer, Data->FormationName, index);
	}

	if (ExtendedFlags[1] & 0x10)	//ExtendedFlags[2] & 0x80
	{
		AddData(buffer, Data->Formation, index);
	}

	if (ExtendedFlags[1] & 0x20)	//ExtendedFlags[3] & 0x01
	{
		AddData(buffer, Data->Position, index);
	}

	if (ExtendedFlags[1] & 0x40)	//ExtendedFlags[3] & 0x80
	{
		Members.BuildExtendedPacket(buffer, index);
	}
	else if (ExtendedFlags[3] & 0x80)
	{
		AddData(buffer, char(0x05), index);
	}
}

/******************************
*         GET METHODS         *
******************************/

_GroupInfo * AuxGroupInfo::GetData()                {return Data;}

bool AuxGroupInfo::GetIsGroupLeader()				{return Data->IsGroupLeader;}
bool AuxGroupInfo::GetLookingForGroup()				{return Data->LookingForGroup;}
bool AuxGroupInfo::GetAllowGroupInvite()			{return Data->AllowGroupInvite;}
bool AuxGroupInfo::GetShowNonCombatantActivities()	{return Data->ShowNonCombatantActivities;}
bool AuxGroupInfo::GetForceAutoSplit()				{return Data->ForceAutoSplit;}
bool AuxGroupInfo::GetRestrictedLootingRights()		{return Data->RestrictedLootingRights;}
bool AuxGroupInfo::GetAutoReleaseLootingRights()	{return Data->AutoReleaseLootingRights;}
char * AuxGroupInfo::GetFormationName()				{return Data->FormationName;}
u32 AuxGroupInfo::GetFormation()					{return Data->Formation;}
u32 AuxGroupInfo::GetPosition()						{return Data->Position;}

/******************************
*         SET METHODS         *
******************************/

void AuxGroupInfo::SetData(_GroupInfo *NewData)
{
	ReplaceData(Data->IsGroupLeader, NewData->IsGroupLeader, 0);
	ReplaceData(Data->LookingForGroup, NewData->LookingForGroup, 1);
	ReplaceData(Data->AllowGroupInvite, NewData->AllowGroupInvite, 2);
	ReplaceData(Data->ShowNonCombatantActivities, NewData->ShowNonCombatantActivities, 3);
	ReplaceData(Data->ForceAutoSplit, NewData->ForceAutoSplit, 4);
	ReplaceData(Data->RestrictedLootingRights, NewData->RestrictedLootingRights, 5);
	ReplaceData(Data->AutoReleaseLootingRights, NewData->AutoReleaseLootingRights, 6);
	ReplaceString(Data->FormationName, NewData->FormationName, 7);
	ReplaceData(Data->Formation, NewData->Formation, 8);
	ReplaceData(Data->Position, NewData->Position, 9);
	Members.SetData(&NewData->Members);
}

void AuxGroupInfo::SetIsGroupLeader(bool NewIsGroupLeader)
{
	ReplaceData(Data->IsGroupLeader, NewIsGroupLeader, 0);
}

void AuxGroupInfo::SetLookingForGroup(bool NewLookingForGroup)
{
	ReplaceData(Data->LookingForGroup, NewLookingForGroup, 1);
}

void AuxGroupInfo::SetAllowGroupInvite(bool NewAllowGroupInvite)
{
	ReplaceData(Data->AllowGroupInvite, NewAllowGroupInvite, 2);
}

void AuxGroupInfo::SetShowNonCombatantActivities(bool NewShowNonCombatantActivities)
{
	ReplaceData(Data->ShowNonCombatantActivities, NewShowNonCombatantActivities, 3);
}

void AuxGroupInfo::SetForceAutoSplit(bool NewForceAutoSplit)
{
	ReplaceData(Data->ForceAutoSplit, NewForceAutoSplit, 4);
}

void AuxGroupInfo::SetRestrictedLootingRights(bool NewRestrictedLootingRights)
{
	ReplaceData(Data->RestrictedLootingRights, NewRestrictedLootingRights, 5);
}

void AuxGroupInfo::SetAutoReleaseLootingRights(bool NewAutoReleaseLootingRights)
{
	ReplaceData(Data->AutoReleaseLootingRights, NewAutoReleaseLootingRights, 6);
}

void AuxGroupInfo::SetFormationName(char * NewFormationName)
{
	ReplaceString(Data->FormationName, NewFormationName, 7);
}

void AuxGroupInfo::SetFormation(u32 NewFormation)
{
	ReplaceData(Data->Formation, NewFormation, 8);
}

void AuxGroupInfo::SetPosition(u32 NewPosition)
{
	ReplaceData(Data->Position, NewPosition, 9);
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxGroupInfo::ClearFlags()
{
	memset(Flags,0,sizeof(Flags));
    Members.ClearFlags();
}