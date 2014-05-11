#include "AuxKnownFormulas.h"

void AuxKnownFormulas::BuildPacket(unsigned char *buffer, long &index, bool TwoBitFlags)
{
    if (TwoBitFlags)
    {
        return;
    }

	AddFlags(Flags, sizeof(Flags), buffer, index);

	for (int i=0;i<500;i++)
	{
		if (CheckFlagBit(i))
		{
			Formula[i].BuildPacket(buffer, index, TwoBitFlags);
		}
	}

	memset(Flags,0,sizeof(Flags));
}

/******************************
*         GET METHODS         *
******************************/

_KnownFormulas * AuxKnownFormulas::GetData()    {return Data;}

/******************************
*         SET METHODS         *
******************************/

void AuxKnownFormulas::SetData(_KnownFormulas *NewData)
{
	for (int i=0;i<500;i++)
	{
		Formula[i].SetData(&NewData->Formula[i]);
	}
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxKnownFormulas::ResetKnownFormulas(unsigned int EndIndex)
{
	for (unsigned int i=0;i<EndIndex;i++)
	{
		Formula[i].SetItemName("");
        Formula[i].SetItemID(0);
        Formula[i].SetTechLevel(0);
	}
}

void AuxKnownFormulas::Clear()
{
	for (int i=0;i<500;i++)
	{
		Formula[i].SetItemName("");
        Formula[i].SetItemID(0);
        Formula[i].SetTechLevel(0);
        Formula[i].ClearFlags();
	}

    memset(Flags, 0, sizeof(Flags));
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxKnownFormulas::ClearFlags()
{
	memset(Flags,0,sizeof(Flags));

	for (int i=0;i<500;i++)
	{
        Formula[i].ClearFlags();
	}
}