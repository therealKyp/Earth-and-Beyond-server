#include "AuxDamage.h"

void AuxDamage::BuildPacket(unsigned char *buffer, long &index)
{
	/*
	AddFlags(Flags, sizeof(Flags), buffer, index);

	memset(Flags,0,sizeof(Flags));
	*/
}

void AuxDamage::BuildExtendedPacket(unsigned char *buffer, long &index)
{
    /*
    AddFlags(ExtendedFlags, sizeof(ExtendedFlags), buffer, index);
    */
}

void AuxDamage::BuildSpecialPacket(unsigned char *buffer, long &index)
{
	/*
	AddFlags(Flags, sizeof(Flags), buffer, index);
    */
}

/******************************
*         GET METHODS         *
******************************/

_Damage * AuxDamage::GetData()			{return Data;}

/******************************
*         SET METHODS         *
******************************/

void AuxDamage::SetData(_Damage * NewData)
{
	/*
	ReplaceData(Data.Slot1, NewData->Slot1, 0);
	ReplaceData(Data.Slot2, NewData->Slot2, 1);
	ReplaceData(Data.Slot3, NewData->Slot3, 2);
	ReplaceData(Data.Slot4, NewData->Slot4, 3);
	*/
}

/******************************
*       UTILITY METHODS       *
******************************/

void AuxDamage::Clear()
{
    _Damage ClearData;
    
    ClearData.NoClueWhatSoEver = 0;

    SetData(&ClearData);
}

void AuxDamage::ClearFlags()
{
	memset(Flags,0,sizeof(Flags));
}