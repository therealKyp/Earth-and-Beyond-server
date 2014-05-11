#include "AuxElements.h"

void AuxElements::BuildPacket(unsigned char *buffer, long &index)
{
	AddFlags(Flags, sizeof(Flags), buffer, index);

	for (int i=0;i<4;i++)
	{
		if (CheckFlagBit(i))
		{
			Element[i].BuildPacket(buffer, index);
		}
	}

	memset(Flags,0,sizeof(Flags));
}

void AuxElements::BuildExtendedPacket(unsigned char *buffer, long &index)
{
	AddFlags(ExtendedFlags, sizeof(ExtendedFlags), buffer, index);

    for (int i=0;i<4;i++)
    {
	    if (CheckExtendedFlagBit1(i))
	    {
		    Element[i].BuildExtendedPacket(buffer, index);
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

_Elements * AuxElements::GetData()      {return Data;}

/******************************
*         SET METHODS         *
******************************/

void AuxElements::SetData(_Elements *NewData)
{
	Element[0].SetData(&NewData->Element[0]);
	Element[1].SetData(&NewData->Element[1]);
	Element[2].SetData(&NewData->Element[2]);
	Element[3].SetData(&NewData->Element[3]);

	CheckData();
}

/******************************
*     PARENT FLAG METHODS     *
******************************/

void AuxElements::CheckData()
{
	SetParentExtendedFlag(HasData());
}

int AuxElements::HasData()
{
	return (ExtendedFlags[0] & 0xF0);
}

/******************************
*       UTILITY METHODS       *
******************************/

_Elements AuxElements::GetClearStruct()
{
    _Elements ClearData;

    ClearData.Element[0] = Element[0].GetClearStruct();
    ClearData.Element[1] = Element[1].GetClearStruct();
    ClearData.Element[2] = Element[2].GetClearStruct();
    ClearData.Element[3] = Element[3].GetClearStruct();

    return ClearData;
}

void AuxElements::Clear()
{
    SetData(&GetClearStruct());
}

void AuxElements::ClearFlags()
{
	memset(Flags,0,sizeof(Flags));

	for (int i=0;i<4;i++)
	{
        Element[i].ClearFlags();
	}
}