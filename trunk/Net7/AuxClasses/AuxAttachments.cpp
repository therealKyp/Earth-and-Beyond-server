#include "AuxAttachments.h"

void AuxAttachments::BuildPacket(unsigned char *buffer, long &index)
{
	AddFlags(Flags, sizeof(Flags), buffer, index);

	for (int i=0;i<18;i++)
	{
		if (CheckFlagBit(i))
		{
			Attachment[i].BuildPacket(buffer, index);
		}
	}

	memset(Flags,0,sizeof(Flags));
}

void AuxAttachments::BuildExtendedPacket(unsigned char *buffer, long &index)
{
	AddFlags(ExtendedFlags, sizeof(ExtendedFlags), buffer, index);

    for (int i=0;i<18;i++)
    {
	    if (CheckExtendedFlagBit1(i))
	    {
		    Attachment[i].BuildExtendedPacket(buffer, index);
	    }
	    else if (CheckExtendedFlagBit2(i))
	    {
		    AddData(buffer, char(0x05), index);
	    }
    }
}

void AuxAttachments::BuildSpecialPacket(unsigned char *buffer, long &index)
{
	AddFlags(Flags, sizeof(Flags), buffer, index);

	for (int i=0;i<18;i++)
	{
		if (CheckFlagBit(i))
		{
			Attachment[i].BuildSpecialPacket(buffer, index);
		}
	}
}

/******************************
*         GET METHODS         *
******************************/

_Attachments * AuxAttachments::GetData()    {return Data;}

/******************************
*         SET METHODS         *
******************************/

void AuxAttachments::SetData(_Attachments *NewData)
{
	for (int i=0;i<18;i++)
	{
		Attachment[i].SetData(&NewData->Attachment[i]);
	}
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxAttachments::ClearFlags()
{
	memset(Flags,0,sizeof(Flags));

	for (int i=0;i<18;i++)
	{
        Attachment[i].ClearFlags();
	}
}