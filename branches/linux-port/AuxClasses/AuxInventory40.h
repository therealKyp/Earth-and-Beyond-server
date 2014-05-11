#ifndef _AUXINVENTORY40_H_INCLUDED_
#define _AUXINVENTORY40_H_INCLUDED_

#include "AuxItem.h"
	
struct _Inventory40
{
	_Item Item[40];
} ATTRIB_PACKED;

class AuxInventory40 : public AuxBase
{
public:
    AuxInventory40()
	{
	}

    ~AuxInventory40()
	{
	}

	void Init(unsigned int MemberIndex, class AuxBase * Parent, _Inventory40 * DataPointer)
	{
		Construct(Flags, ExtendedFlags, 40, Parent, MemberIndex);
        Data = DataPointer;

		memset(Flags,0, sizeof(Flags));

		ExtendedFlags[0] = char(0xF6);
		ExtendedFlags[1] = char(0xFF);
		ExtendedFlags[2] = char(0xFF);
		ExtendedFlags[3] = char(0xFF);
		ExtendedFlags[4] = char(0xFF);
		ExtendedFlags[5] = char(0xFF);
		ExtendedFlags[6] = char(0xFF);
		ExtendedFlags[7] = char(0xFF);
		ExtendedFlags[8] = char(0xFF);
		ExtendedFlags[9] = char(0xFF);
		ExtendedFlags[10] = char(0xFF);
		ExtendedFlags[11] = char(0x0F);

		for (int i=0;i<40;i++)
		{
			Item[i].Init(i, this, &Data->Item[i]);
		}
	}

    void Clear();
    void ClearFlags();

    void BuildPacket(unsigned char *, long &);
    void BuildExtendedPacket(unsigned char *, long &);

	_Inventory40 * GetData();

	void SetData(_Inventory40 *);

private:
    _Inventory40 * Data;

	unsigned char Flags[6];
	unsigned char ExtendedFlags[11];

public:
	class AuxItem Item[40];
};

#endif
