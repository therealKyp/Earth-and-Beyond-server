#ifndef _AUXDAMAGE_H_INCLUDED_
#define _AUXDAMAGE_H_INCLUDED_

#include "AuxBase.h"
	
struct _Damage
{
	u32	NoClueWhatSoEver;
} ATTRIB_PACKED;

class AuxDamage : public AuxBase
{
public:
    AuxDamage()
	{
	}

    ~AuxDamage()
	{
	}

	void Init(unsigned int MemberIndex, class AuxBase * Parent, _Damage * DataPointer)
	{
		Construct(Flags, ExtendedFlags, 5, Parent, MemberIndex);
        Data = DataPointer;

		memset(Flags,0, sizeof(Flags));

		ExtendedFlags[0] = char(0x06);
		ExtendedFlags[1] = char(0x3E);

		Data->NoClueWhatSoEver = 0;
	}

    void Clear();
    void ClearFlags();

	void BuildPacket(unsigned char *, long &);
	void BuildExtendedPacket(unsigned char *, long &);
	void BuildSpecialPacket(unsigned char *, long &);

	_Damage * GetData();

	u32 GetNoClueWhatSoEver();

	void SetData(_Damage *);

	void SetNoClueWhatSoEver(u32);


private:
	_Damage * Data;

	unsigned char Flags[2];
	unsigned char ExtendedFlags[2];
};

#endif
