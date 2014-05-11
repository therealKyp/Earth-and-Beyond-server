#ifndef _AUXINVENTORY20_H_INCLUDED_
#define _AUXINVENTORY20_H_INCLUDED_

#include "AuxItem.h"
	
struct _Inventory20
{
	_Item Item[20];
} ATTRIB_PACKED;

class AuxInventory20 : public AuxBase
{
public:
    AuxInventory20()
	{
	}

    ~AuxInventory20()
	{
	}

	void Init(unsigned int MemberIndex, class AuxBase * Parent, _Inventory20 * DataPointer)
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
			Item[i].Init(i, this, &Data->Item[i]);
		}
	}

    void Clear();
    void ClearFlags();

    void BuildPacket(unsigned char *, long &);
    void BuildExtendedPacket(unsigned char *, long &);

	_Inventory20 * GetData();

	void SetData(_Inventory20 *);

private:
    _Inventory20 * Data;

	unsigned char Flags[3];
	unsigned char ExtendedFlags[6];

public:
	class AuxItem Item[20];
};

#endif
