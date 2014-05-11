#ifndef _AUXREWARDINV_H_INCLUDED_
#define _AUXREWARDINV_H_INCLUDED_

#include "AuxItem.h"
	
struct _RewardInv
{
	_Item Item[2];
} ATTRIB_PACKED;

class AuxRewardInv : public AuxBase
{
public:
    AuxRewardInv()
	{
	}

    ~AuxRewardInv()
	{
	}

	void Init(unsigned int MemberIndex, class AuxBase * Parent, _RewardInv * DataPointer)
	{
		Construct(Flags, ExtendedFlags, 2, Parent, MemberIndex);
        Data = DataPointer;

		memset(Flags,0, sizeof(Flags));

		ExtendedFlags[0] = char(0xF6);

		Item[0].Init(0, this, &Data->Item[0]);
		Item[1].Init(1, this, &Data->Item[1]);
	}

    void ClearFlags();

	void BuildPacket(unsigned char *, long &);
	void BuildExtendedPacket(unsigned char *, long &);

	_RewardInv * GetData();

	void SetData(_RewardInv *);

private:
    _RewardInv * Data;

	unsigned char Flags[1];
	unsigned char ExtendedFlags[1];

public:
	class AuxItem Item[2];
};

#endif
