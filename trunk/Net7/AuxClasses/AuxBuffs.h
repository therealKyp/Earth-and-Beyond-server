#ifndef _AUXBUFFS_H_INCLUDED_
#define _AUXBUFFS_H_INCLUDED_

#include "AuxBuff.h"
	
struct _Buffs
{
	_Buff Buff[16];
} ATTRIB_PACKED;

class AuxBuffs : public AuxBase
{
public:
    AuxBuffs()
	{
	}

    ~AuxBuffs()
	{
	}

	void Init(unsigned int MemberIndex, class AuxBase * Parent, _Buffs * DataPointer)
	{
		Construct(Flags, ExtendedFlags, 16, Parent, MemberIndex);
        Data = DataPointer;

		memset(Flags,0, sizeof(Flags));

		ExtendedFlags[0] = char(0x06);
		ExtendedFlags[1] = char(0x00);
		ExtendedFlags[2] = char(0xF0);
		ExtendedFlags[3] = char(0xFF);
		ExtendedFlags[4] = char(0x0F);

		for (int i=0;i<16;i++)
		{
			Buff[i].Init(i, this, &Data->Buff[i]);
		}
	}

    void ClearFlags();

	void BuildPacket(unsigned char *, long &);
	void BuildExtendedPacket(unsigned char *, long &);

	_Buffs * GetData();

	void SetData(_Buffs *);

private:
    _Buffs * Data;

	unsigned char Flags[3];
	unsigned char ExtendedFlags[5];

public:
	class AuxBuff Buff[16];
};

#endif
