
#include "AuxMission.h"

void AuxMission::BuildPacket(unsigned char *buffer, long &index)
{
	AddFlags(Flags, sizeof(Flags), buffer, index);

	if (Flags[0] & 0x10)	//ExtendedFlags[2] & 0x80
	{
		AddData(buffer, Data->ID, index);
	}

	if (Flags[0] & 0x20)	//ExtendedFlags[3] & 0x01
	{
		AddString(buffer, Data->Name, index);
	}

	if (Flags[0] & 0x40)	//ExtendedFlags[3] & 0x02
	{
		AddString(buffer, Data->Summary, index);
	}

	if (Flags[0] & 0x80)	//ExtendedFlags[3] & 0x04
	{
		AddString(buffer, Data->Reward, index);
	}

	if (Flags[1] & 0x01)	//ExtendedFlags[3] & 0x08
	{
		AddString(buffer, Data->FailureConsequence, index);
	}

	if (Flags[1] & 0x02)	//ExtendedFlags[3] & 0x10
	{
		AddString(buffer, Data->IssuingFaction, index);
	}

	if (Flags[1] & 0x04)	//ExtendedFlags[3] & 0x20
	{
		AddData(buffer, Data->IsTimed, index);
	}

	if (Flags[1] & 0x08)	//ExtendedFlags[3] & 0x40
	{
		AddData(buffer, Data->ExpirationTime, index);
	}

	if (Flags[1] & 0x10)	//ExtendedFlags[3] & 0x80
	{
		AddData(buffer, Data->StartTime, index);
	}

	if (Flags[1] & 0x20)	//ExtendedFlags[4] & 0x01
	{
		AddData(buffer, Data->IsForfeitable, index);
	}

	if (Flags[1] & 0x40)	//ExtendedFlags[4] & 0x02
	{
		AddData(buffer, Data->IsCompleted, index);
	}

	if (Flags[1] & 0x80)	//ExtendedFlags[4] & 0x04
	{
		AddData(buffer, Data->IsFailed, index);
	}

	if (Flags[2] & 0x01)	//ExtendedFlags[4] & 0x08
	{
		AddData(buffer, Data->IsExpired, index);
	}

	if (Flags[2] & 0x02)	//ExtendedFlags[4] & 0x10
	{
		AddData(buffer, Data->IsFullyVisible, index);
	}

	if (Flags[2] & 0x04)	//ExtendedFlags[4] & 0x20
	{
		AddData(buffer, Data->StageCount, index);
	}

	if (Flags[2] & 0x08)	//ExtendedFlags[4] & 0x40
	{
		AddData(buffer, Data->StageNum, index);
	}

	if (Flags[2] & 0x10)	//ExtendedFlags[4] & 0x80
	{
		Stages.BuildPacket(buffer, index);
	}

	if (Flags[2] & 0x20)	//ExtendedFlags[5] & 0x01
	{
		AddData(buffer, Data->StageExpirationTime, index);
	}

	if (Flags[2] & 0x40)	//ExtendedFlags[5] & 0x02
	{
		AddData(buffer, Data->HasGivenNewMissionMessage, index);
	}

	memset(Flags,0,sizeof(Flags));
}

void AuxMission::BuildExtendedPacket(unsigned char *buffer, long &index)
{
	AddFlags(ExtendedFlags, sizeof(ExtendedFlags), buffer, index);

	if (ExtendedFlags[0] & 0x10)	//ExtendedFlags[2] & 0x80
	{
		AddData(buffer, Data->ID, index);
	}

	if (ExtendedFlags[0] & 0x20)	//ExtendedFlags[3] & 0x01
	{
		AddString(buffer, Data->Name, index);
	}

	if (ExtendedFlags[0] & 0x40)	//ExtendedFlags[3] & 0x02
	{
		AddString(buffer, Data->Summary, index);
	}

	if (ExtendedFlags[0] & 0x80)	//ExtendedFlags[3] & 0x04
	{
		AddString(buffer, Data->Reward, index);
	}

	if (ExtendedFlags[1] & 0x01)	//ExtendedFlags[3] & 0x08
	{
		AddString(buffer, Data->FailureConsequence, index);
	}

	if (ExtendedFlags[1] & 0x02)	//ExtendedFlags[3] & 0x10
	{
		AddString(buffer, Data->IssuingFaction, index);
	}

	if (ExtendedFlags[1] & 0x04)	//ExtendedFlags[3] & 0x20
	{
		AddData(buffer, Data->IsTimed, index);
	}

	if (ExtendedFlags[1] & 0x08)	//ExtendedFlags[3] & 0x40
	{
		AddData(buffer, Data->ExpirationTime, index);
	}

	if (ExtendedFlags[1] & 0x10)	//ExtendedFlags[3] & 0x80
	{
		AddData(buffer, Data->StartTime, index);
	}

	if (ExtendedFlags[1] & 0x20)	//ExtendedFlags[4] & 0x01
	{
		AddData(buffer, Data->IsForfeitable, index);
	}

	if (ExtendedFlags[1] & 0x40)	//ExtendedFlags[4] & 0x02
	{
		AddData(buffer, Data->IsCompleted, index);
	}

	if (ExtendedFlags[1] & 0x80)	//ExtendedFlags[4] & 0x04
	{
		AddData(buffer, Data->IsFailed, index);
	}

	if (ExtendedFlags[2] & 0x01)	//ExtendedFlags[4] & 0x08
	{
		AddData(buffer, Data->IsExpired, index);
	}

	if (ExtendedFlags[2] & 0x02)	//ExtendedFlags[4] & 0x10
	{
		AddData(buffer, Data->IsFullyVisible, index);
	}

	if (ExtendedFlags[2] & 0x04)	//ExtendedFlags[4] & 0x20
	{
		AddData(buffer, Data->StageCount, index);
	}

	if (ExtendedFlags[2] & 0x08)	//ExtendedFlags[4] & 0x40
	{
		AddData(buffer, Data->StageNum, index);
	}

	if (ExtendedFlags[2] & 0x10)	//ExtendedFlags[4] & 0x80
	{
		Stages.BuildExtendedPacket(buffer, index);
	}
	else if (ExtendedFlags[4] & 0x80)
	{
		AddData(buffer, char(0x05), index);
	}

	if (ExtendedFlags[2] & 0x20)	//ExtendedFlags[5] & 0x01
	{
		AddData(buffer, Data->StageExpirationTime, index);
	}

	if (ExtendedFlags[2] & 0x40)	//ExtendedFlags[5] & 0x02
	{
		AddData(buffer, Data->HasGivenNewMissionMessage, index);
	}
}

/******************************
*         GET METHODS         *
******************************/

_Mission * AuxMission::GetData()                    {return Data;}

u32 AuxMission::GetID()								{return Data->ID;}
char * AuxMission::GetName()						{return Data->Name;}
char * AuxMission::GetSummary()						{return Data->Summary;}
char * AuxMission::GetReward()						{return Data->Reward;}
char * AuxMission::GetFailureConsequence()			{return Data->FailureConsequence;}
char * AuxMission::GetIssuingFaction()				{return Data->IssuingFaction;}
bool AuxMission::GetIsTimed()						{return Data->IsTimed;}
u32 AuxMission::GetExpirationTime()					{return Data->ExpirationTime;}
u32 AuxMission::GetStartTime()						{return Data->StartTime;}
bool AuxMission::GetIsForfeitable()					{return Data->IsForfeitable;}
bool AuxMission::GetIsCompleted()					{return Data->IsCompleted;}
bool AuxMission::GetIsFailed()						{return Data->IsFailed;}
bool AuxMission::GetIsExpired()						{return Data->IsExpired;}
bool AuxMission::GetIsFullyVisible()				{return Data->IsFullyVisible;}
u32 AuxMission::GetStageCount()						{return Data->StageCount;}
u32 AuxMission::GetStageNum()						{return Data->StageNum;}
u32 AuxMission::GetStageExpirationTime()			{return Data->StageExpirationTime;}
bool AuxMission::GetHasGivenNewMissionMessage()		{return Data->HasGivenNewMissionMessage;}

long AuxMission::GetDatabaseID()                    {return Data->DatabaseID;}
long AuxMission::GetMissionData()                   {return Data->MissionData;}

/******************************
*         SET METHODS         *
******************************/

void AuxMission::SetData(_Mission *NewData)
{
	ReplaceData(Data->ID, NewData->ID, 0);
	ReplaceString(Data->Name, NewData->Name, 1);
	ReplaceString(Data->Summary, NewData->Summary, 2);
	ReplaceString(Data->Reward, NewData->Reward, 3);
	ReplaceString(Data->FailureConsequence, NewData->FailureConsequence, 4);
	ReplaceString(Data->IssuingFaction, NewData->IssuingFaction, 5);
	ReplaceData(Data->IsTimed, NewData->IsTimed, 6);
	ReplaceData(Data->ExpirationTime, NewData->ExpirationTime, 7);
	ReplaceData(Data->StartTime, NewData->StartTime, 8);
	ReplaceData(Data->IsForfeitable, NewData->IsForfeitable, 9);
	ReplaceData(Data->IsCompleted, NewData->IsCompleted, 10);
	ReplaceData(Data->IsFailed, NewData->IsFailed, 11);
	ReplaceData(Data->IsExpired, NewData->IsExpired, 12);
	ReplaceData(Data->IsFullyVisible, NewData->IsFullyVisible, 13);
	ReplaceData(Data->StageCount, NewData->StageCount, 14);
	ReplaceData(Data->StageNum, NewData->StageNum, 15);
	Stages.SetData(&NewData->Stages);
	ReplaceData(Data->StageExpirationTime, NewData->StageExpirationTime, 17);
	ReplaceData(Data->HasGivenNewMissionMessage, NewData->HasGivenNewMissionMessage, 18);

    //No replace method as DatabaseID is not part of the packet
    Data->DatabaseID = NewData->DatabaseID;
    Data->MissionData = NewData->MissionData;
}

void AuxMission::SetID(u32 NewID)
{
	ReplaceData(Data->ID, NewID, 0);
}

void AuxMission::SetName(char * NewName)
{
	ReplaceString(Data->Name, NewName, 1);
}

void AuxMission::SetSummary(char * NewSummary)
{
	ReplaceString(Data->Summary, NewSummary, 2);
}

void AuxMission::SetReward(char * NewReward)
{
	ReplaceString(Data->Reward, NewReward, 3);
}

void AuxMission::SetFailureConsequence(char * NewFailureConsequence)
{
	ReplaceString(Data->FailureConsequence, NewFailureConsequence, 4);
}

void AuxMission::SetIssuingFaction(char * NewIssuingFaction)
{
	ReplaceString(Data->IssuingFaction, NewIssuingFaction, 5);
}

void AuxMission::SetIsTimed(bool NewIsTimed)
{
	ReplaceData(Data->IsTimed, NewIsTimed, 6);
}

void AuxMission::SetExpirationTime(u32 NewExpirationTime)
{
	ReplaceData(Data->ExpirationTime, NewExpirationTime, 7);
}

void AuxMission::SetStartTime(u32 NewStartTime)
{
	ReplaceData(Data->StartTime, NewStartTime, 8);
}

void AuxMission::SetIsForfeitable(bool NewIsForfeitable)
{
	ReplaceData(Data->IsForfeitable, NewIsForfeitable, 9);
}

void AuxMission::SetIsCompleted(bool NewIsCompleted)
{
	ReplaceData(Data->IsCompleted, NewIsCompleted, 10);
}

void AuxMission::SetIsFailed(bool NewIsFailed)
{
	ReplaceData(Data->IsFailed, NewIsFailed, 11);
}

void AuxMission::SetIsExpired(bool NewIsExpired)
{
	ReplaceData(Data->IsExpired, NewIsExpired, 12);
}

void AuxMission::SetIsFullyVisible(bool NewIsFullyVisible)
{
	ReplaceData(Data->IsFullyVisible, NewIsFullyVisible, 13);
}

void AuxMission::SetStageCount(u32 NewStageCount)
{
	ReplaceData(Data->StageCount, NewStageCount, 14);
}

void AuxMission::SetStageNum(u32 NewStageNum)
{
	ReplaceData(Data->StageNum, NewStageNum, 15);
}

void AuxMission::SetStageExpirationTime(u32 NewStageExpirationTime)
{
	ReplaceData(Data->StageExpirationTime, NewStageExpirationTime, 17);
}

void AuxMission::SetHasGivenNewMissionMessage(bool NewHasGivenNewMissionMessage)
{
	ReplaceData(Data->HasGivenNewMissionMessage, NewHasGivenNewMissionMessage, 18);
}

//No replace method as DatabaseID is not part of the packet
void AuxMission::SetDatabaseID(long NewDatabaseID)
{
    Data->DatabaseID = NewDatabaseID;
}

void AuxMission::SetMissionData(long NewMissionData)
{
    Data->MissionData = NewMissionData;
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxMission::Clear()
{
    _Mission ClearData;
    
	ClearData.ID = GetID();
	*ClearData.Name = 0;
	*ClearData.Summary = 0; 
	*ClearData.Reward = 0;
	*ClearData.FailureConsequence = 0; 
	*ClearData.IssuingFaction = 0; 
	ClearData.IsTimed = 0;
	ClearData.ExpirationTime = 0;
	ClearData.StartTime = 0;
	ClearData.IsForfeitable = 0;
	ClearData.IsCompleted = 0;
	ClearData.IsFailed = 0;
	ClearData.IsExpired = 0;
	ClearData.IsFullyVisible = 0;
	ClearData.StageCount = 0;
	ClearData.StageNum = 0;
	ClearData.Stages = Stages.GetClearStruct();
	ClearData.StageExpirationTime = 0;
	ClearData.HasGivenNewMissionMessage = 0;

    ClearData.DatabaseID = -1;
    ClearData.MissionData = 0;

    SetData(&ClearData);
}

void AuxMission::ClearFlags()
{
	memset(Flags,0,sizeof(Flags));
    Stages.ClearFlags();
}