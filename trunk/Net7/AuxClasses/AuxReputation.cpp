#include "AuxReputation.h"

void AuxReputation::BuildPacket(unsigned char *buffer, long &index)
{
	AddFlags(Flags, sizeof(Flags), buffer, index);

	if (Flags[0] & 0x10)	//ExtendedFlags[0] & 0x40
	{
		Factions.BuildPacket(buffer, index);
	}

	if (Flags[0] & 0x20)	//ExtendedFlags[0] & 0x80
	{
		AddString(buffer, Data->Affiliation, index);
	}

	memset(Flags,0,sizeof(Flags));
}

void AuxReputation::BuildExtendedPacket(unsigned char *buffer, long &index)
{
	AddFlags(ExtendedFlags, sizeof(ExtendedFlags), buffer, index);

	if (ExtendedFlags[0] & 0x10)	//ExtendedFlags[0] & 0x40
	{
		Factions.BuildExtendedPacket(buffer, index);
	}
	else if (ExtendedFlags[0] & 0x40)
	{
		AddData(buffer, char(0x05), index);
	}

	if (ExtendedFlags[0] & 0x20)	//ExtendedFlags[0] & 0x80
	{
		AddString(buffer, Data->Affiliation, index);
	}
}

/******************************
*         GET METHODS         *
******************************/

_Reputation * AuxReputation::GetData()      {return Data;}

char * AuxReputation::GetAffiliation()		{return Data->Affiliation;}

/******************************
*         SET METHODS         *
******************************/

void AuxReputation::SetData(_Reputation *NewData)
{
	Factions.SetData(&NewData->Factions);
	ReplaceString(Data->Affiliation, NewData->Affiliation, 1);
}

void AuxReputation::SetAffilitation(char * NewAffiliation)
{
	ReplaceString(Data->Affiliation, NewAffiliation, 1);
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxReputation::ClearFlags()
{
	memset(Flags,0,sizeof(Flags));
    Factions.ClearFlags();
}