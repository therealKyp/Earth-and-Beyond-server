#ifndef _AUXINVENTORY1_H_INCLUDED_
#define _AUXINVENTORY1_H_INCLUDED_

#include "AuxItem.h"
	
struct _Inventory1
{
	_Item Item[1];
} ATTRIB_PACKED;

class AuxInventory1 : public AuxBase
{
public:
    AuxInventory1()
	{
	}

    ~AuxInventory1()
	{
	}

	void Init(unsigned int MemberIndex, class AuxBase * Parent, _Inventory1 * DataPointer)
	{
		Construct(Flags, ExtendedFlags, 1, Parent, MemberIndex);
        Data = DataPointer;

		memset(Flags,0, sizeof(Flags));

		ExtendedFlags[0] = char(0x36);

		Item[0].Init(0, this, &Data->Item[0]);
	}

    void Empty();
    void ClearFlags();

	void BuildPacket(unsigned char *, long &);
	void BuildExtendedPacket(unsigned char *, long &);

	_Inventory1 * GetData();

	void SetData(_Inventory1 *);

private:
    _Inventory1 * Data;

	unsigned char Flags[1];
	unsigned char ExtendedFlags[1];

public:
	class AuxItem Item[1];
};

#endif
