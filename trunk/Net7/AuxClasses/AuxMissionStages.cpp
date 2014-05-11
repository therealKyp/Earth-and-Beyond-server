#include "AuxMissionStages.h"

void AuxMissionStages::BuildPacket(unsigned char *buffer, long &index)
{
	AddFlags(Flags, sizeof(Flags), buffer, index);

	for (int i=0;i<20;i++)
	{
		if (CheckFlagBit(i))
		{
			Stage[i].BuildPacket(buffer, index);
		}
	}

	memset(Flags,0,sizeof(Flags));
}

void AuxMissionStages::BuildExtendedPacket(unsigned char *buffer, long &index)
{
	AddFlags(ExtendedFlags, sizeof(ExtendedFlags), buffer, index);

	for (int i=0;i<20;i++)
	{
		if (CheckExtendedFlagBit1(i))
		{
			Stage[i].BuildExtendedPacket(buffer, index);
		}
		else if (CheckExtendedFlagBit2(i))
		{
			AddData(buffer, char(0x05), index);
		}
	}
}

/******************************
*         GET METHODS         *
******************************/

_MissionStages * AuxMissionStages::GetData()    {return Data;}

/******************************
*         SET METHODS         *
******************************/

void AuxMissionStages::SetData(_MissionStages *NewData)
{
	for (int i=0;i<20;i++)
	{
		Stage[i].SetData(&NewData->Stage[i]);
	}
}

/******************************
*     PARENT FLAG METHODS     *
******************************/

void AuxMissionStages::CheckData()
{
	SetParentExtendedFlag(HasData());
}

int AuxMissionStages::HasData()
{
	return (ExtendedFlags[0] & 0xF0 || ExtendedFlags[1] & 0xFF || ExtendedFlags[2] & 0xFF);
}

/******************************
*       UTILITY METHODS       *
******************************/

_MissionStages AuxMissionStages::GetClearStruct()
{
    _MissionStages ClearData;
    
	for (int i=0;i<20;i++)
	{
		ClearData.Stage[i] = Stage[i].GetClearStruct();
	}

    return ClearData;
}

void AuxMissionStages::Clear()
{
    SetData(&GetClearStruct());
}

void AuxMissionStages::ClearFlags()
{
	memset(Flags,0,sizeof(Flags));

	for (int i=0;i<20;i++)
	{
        Stage[i].ClearFlags();
	}
}