#ifndef _AUXEQUIPINV_H_INCLUDED_
#define _AUXEQUIPINV_H_INCLUDED_

#include "AuxEquipItem.h"
	
struct _EquipInv
{
	_EquipItem EquipItem[20];
} ATTRIB_PACKED;

class AuxEquipInv : public AuxBase
{
public:
    AuxEquipInv()
	{
	}

    ~AuxEquipInv()
	{
	}

	void Init(unsigned int MemberIndex, class AuxBase * Parent, _EquipInv * DataPointer)
	{
		Construct(Flags, ExtendedFlags, 20, Parent, MemberIndex);
        Data = DataPointer;

		memset(Flags,0, sizeof(Flags));

		ExtendedFlags[0] = char(0xF6);
		ExtendedFlags[1] = char(0xFF);
		ExtendedFlags[2] = char(0xFF);
		ExtendedFlags[3] = char(0xFF);
		ExtendedFlags[4] = char(0xFF);
		ExtendedFlags[5] = char(0x0F);

		for (int i=0;i<20;i++)
		{
			EquipItem[i].Init(i, this, &Data->EquipItem[i]);
		}
	}

    void ClearFlags();

	void BuildPacket(unsigned char *, long &);
	void BuildExtendedPacket(unsigned char *, long &);

	_EquipInv * GetData();

	void SetData(_EquipInv *);

private:
    _EquipInv * Data;

	unsigned char Flags[3];
	unsigned char ExtendedFlags[6];

public:
	class AuxEquipItem EquipItem[20];
};

#endif
