#ifndef _AUXOVERFLOWINV_H_INCLUDED_
#define _AUXOVERFLOWINV_H_INCLUDED_

#include "AuxItem.h"
	
struct _OverflowInv
{
	_Item Item[8];
} ATTRIB_PACKED;

class AuxOverflowInv : public AuxBase
{
public:
    AuxOverflowInv()
	{
	}

    ~AuxOverflowInv()
	{
	}

	void Init(unsigned int MemberIndex, class AuxBase * Parent, _OverflowInv * DataPointer)
	{
		Construct(Flags, ExtendedFlags, 8, Parent, MemberIndex);
        Data = DataPointer;

		memset(Flags,0, sizeof(Flags));

		ExtendedFlags[0] = char(0xF6);
		ExtendedFlags[1] = char(0xFF);
		ExtendedFlags[2] = char(0x0F);

		for (int i=0;i<8;i++)
		{
			Item[i].Init(i, this, &Data->Item[i]);
		}
	}

    void ClearFlags();

	void BuildPacket(unsigned char *, long &);
	void BuildExtendedPacket(unsigned char *, long &);

	_OverflowInv * GetData();

	void SetData(_OverflowInv *);

private:
    _OverflowInv * Data;

	unsigned char Flags[2];
	unsigned char ExtendedFlags[3];

public:
	class AuxItem Item[8];
};

#endif
