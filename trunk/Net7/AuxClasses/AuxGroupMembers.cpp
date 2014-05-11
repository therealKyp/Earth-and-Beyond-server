#include "AuxGroupMembers.h"

void AuxGroupMembers::BuildPacket(unsigned char *buffer, long &index)
{
	AddFlags(Flags, sizeof(Flags), buffer, index);

	for (int i=0;i<5;i++)
	{
		if (CheckFlagBit(i))
		{
			Member[i].BuildPacket(buffer, index);
		}
	}

	memset(Flags,0,sizeof(Flags));
}

void AuxGroupMembers::BuildExtendedPacket(unsigned char *buffer, long &index)
{
	AddFlags(ExtendedFlags, sizeof(ExtendedFlags), buffer, index);

	for (int i=0;i<5;i++)
	{
		if (CheckExtendedFlagBit1(i))
		{
			Member[i].BuildExtendedPacket(buffer, index);
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

_GroupMembers * AuxGroupMembers::GetData()  {return Data;}

/******************************
*         SET METHODS         *
******************************/

void AuxGroupMembers::SetData(_GroupMembers *NewData)
{
	for (int i=0;i<5;i++)
	{
		Member[i].SetData(&NewData->Member[i]);
	}

	CheckData();
}

/******************************
*     PARENT FLAG METHODS     *
******************************/

void AuxGroupMembers::CheckData()
{
	SetParentExtendedFlag(HasData());
}

int AuxGroupMembers::HasData()
{
	return (ExtendedFlags[0] & 0xF0 || ExtendedFlags[1] & 0x01);
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxGroupMembers::Clear()
{
	for (int i=0;i<5;i++)
	{
		Member[i].Clear();
	}
}

void AuxGroupMembers::ClearFlags()
{
	memset(Flags,0,sizeof(Flags));

	for (int i=0;i<5;i++)
	{
        Member[i].ClearFlags();
	}
}