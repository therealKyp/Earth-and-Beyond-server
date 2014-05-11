#include "AuxPreviousAttempts.h"

void AuxPreviousAttempts::BuildPacket(unsigned char *buffer, long &index, bool TwoBitFlags)
{
    if (TwoBitFlags)
    {
        return;
    }

	AddFlags(Flags, sizeof(Flags), buffer, index);

	for (int i=0;i<16;i++)
	{
		if (CheckFlagBit(i))
		{
			Attempt[i].BuildPacket(buffer, index, TwoBitFlags);
		}
	}

	memset(Flags,0,sizeof(Flags));
}

/******************************
*         GET METHODS         *
******************************/

_PreviousAttempts * AuxPreviousAttempts::GetData()  {return Data;}

/******************************
*         SET METHODS         *
******************************/

void AuxPreviousAttempts::SetData(_PreviousAttempts *NewData)
{
	for (int i=0;i<16;i++)
	{
		Attempt[i].SetData(&NewData->Attempt[i]);
	}
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxPreviousAttempts::ResetAttempts(unsigned int EndIndex)
{
	for (unsigned int i=0;i<EndIndex;i++)
	{
		Attempt[i].SetItemName("");
        Attempt[i].SetItemID(0);
        Attempt[i].SetTechLevel(0);
	}
}

void AuxPreviousAttempts::Clear()
{
	for (int i=0;i<16;i++)
	{
		Attempt[i].SetItemName("");
        Attempt[i].SetItemID(0);
        Attempt[i].SetTechLevel(0);
        Attempt[i].ClearFlags();
	}

    memset(Flags, 0, sizeof(Flags));
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxPreviousAttempts::ClearFlags()
{
	memset(Flags,0,sizeof(Flags));

	for (int i=0;i<16;i++)
	{
        Attempt[i].ClearFlags();
	}
}