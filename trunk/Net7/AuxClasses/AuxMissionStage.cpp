#include "AuxMissionStage.h"

void AuxMissionStage::BuildPacket(unsigned char *buffer, long &index)
{
	AddFlags(Flags, sizeof(Flags), buffer, index);

	if (Flags[0] & 0x10)	//ExtendedFlags[0] & 0x40
	{
		AddString(buffer, Data->Text, index);
	}

	if (Flags[0] & 0x20)	//ExtendedFlags[0] & 0x80
	{
		AddData(buffer, char(Data->IsTimed), index);
	}

	memset(Flags,0,sizeof(Flags));
}

void AuxMissionStage::BuildExtendedPacket(unsigned char *buffer, long &index)
{
	AddFlags(ExtendedFlags, sizeof(ExtendedFlags), buffer, index);

	if (ExtendedFlags[0] & 0x10)	//ExtendedFlags[0] & 0x40
	{
		AddString(buffer, Data->Text, index);
	}

	if (ExtendedFlags[0] & 0x20)	//ExtendedFlags[0] & 0x80
	{
		AddData(buffer, char(Data->IsTimed), index);
	}
}

/******************************
*         GET METHODS         *
******************************/

_MissionStage * AuxMissionStage::GetData()			{return Data;}

char * AuxMissionStage::GetText()					{return Data->Text;}
bool AuxMissionStage::GetIsTimed()					{return Data->IsTimed;}

/******************************
*         SET METHODS         *
******************************/

void AuxMissionStage::SetData(_MissionStage *NewData)
{
	ReplaceString(Data->Text, NewData->Text, 0);
	ReplaceData(Data->IsTimed, NewData->IsTimed, 1);

	CheckData();
}

void AuxMissionStage::SetText(char * NewText)
{
	ReplaceString(Data->Text, NewText, 0);
	CheckData();
}

void AuxMissionStage::SetIsTimed(bool NewIsTimed)
{
	ReplaceData(Data->IsTimed, NewIsTimed, 1);
	CheckData();
}

/******************************
*     PARENT FLAG METHODS     *
******************************/

void AuxMissionStage::CheckData()
{
	SetParentExtendedFlag(HasData());
}

int AuxMissionStage::HasData()
{
	return (ExtendedFlags[0] & 0x30);
}

/******************************
*       UTILITY METHODS       *
******************************/

_MissionStage AuxMissionStage::GetClearStruct()
{
    _MissionStage ClearData;
    
	*ClearData.Text = 0; 
	ClearData.IsTimed = 0;

    return ClearData;
}

void AuxMissionStage::Clear()
{
    SetData(&GetClearStruct());
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxMissionStage::ClearFlags()
{
	memset(Flags,0,sizeof(Flags));
}