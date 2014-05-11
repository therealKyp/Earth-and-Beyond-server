#ifndef _AUXSECUREINV_H_INCLUDED_
#define _AUXSECUREINV_H_INCLUDED_

#include "AuxItem.h"
	
struct _SecureInv
{
	_Item Item[96];
} ATTRIB_PACKED;

class AuxSecureInv : public AuxBase
{
public:
    AuxSecureInv()
	{
	}

    ~AuxSecureInv()
	{
	}

	void Init(unsigned int MemberIndex, class AuxBase * Parent, _SecureInv * DataPointer)
	{
		Construct(Flags, ExtendedFlags, 96, Parent, MemberIndex);
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
		ExtendedFlags[11] = char(0xFF);
		ExtendedFlags[12] = char(0xFF);
		ExtendedFlags[13] = char(0xFF);
		ExtendedFlags[14] = char(0xFF);
		ExtendedFlags[15] = char(0xFF);
		ExtendedFlags[16] = char(0xFF);
		ExtendedFlags[17] = char(0xFF);
		ExtendedFlags[18] = char(0xFF);
		ExtendedFlags[19] = char(0xFF);
		ExtendedFlags[20] = char(0xFF);
		ExtendedFlags[21] = char(0xFF);
		ExtendedFlags[22] = char(0xFF);
		ExtendedFlags[23] = char(0xFF);
		ExtendedFlags[24] = char(0x0F);

		for (int i=0;i<96;i++)
		{
			Item[i].Init(i, this, &Data->Item[i]);
		}
	}

    void ClearFlags();

	void BuildPacket(unsigned char *, long &);
	void BuildExtendedPacket(unsigned char *, long &);

	_SecureInv * GetData();

	void SetData(_SecureInv *);

private:
    _SecureInv * Data;

	unsigned char Flags[13];
	unsigned char ExtendedFlags[25];

public:
	class AuxItem Item[96];
};

#endif
