#ifndef _AUXSKILLABILITIES_H_INCLUDED_
#define _AUXSKILLABILITIES_H_INCLUDED_

#include "AuxSkillAbility.h"
	
struct _SkillAbilities
{
	_SkillAbility Ability[170];
} ATTRIB_PACKED;

class AuxSkillAbilities : public AuxBase
{
public:
    AuxSkillAbilities()
	{
	}

    ~AuxSkillAbilities()
	{
	}

	void Init(unsigned int MemberIndex, class AuxBase * Parent, _SkillAbilities * DataPointer)
	{
		Construct(Flags, ExtendedFlags, 170, Parent, MemberIndex);
        Data = DataPointer;

		memset(Flags,0, sizeof(Flags));

		ExtendedFlags[0] = char(0x06);
		ExtendedFlags[1] = char(0x00);
		ExtendedFlags[2] = char(0x00);
		ExtendedFlags[3] = char(0x00);
		ExtendedFlags[4] = char(0x00);
		ExtendedFlags[5] = char(0x00);
		ExtendedFlags[6] = char(0x00);
		ExtendedFlags[7] = char(0x00);
		ExtendedFlags[8] = char(0x00);
		ExtendedFlags[9] = char(0x00);
		ExtendedFlags[10] = char(0x00);
		ExtendedFlags[11] = char(0x00);
		ExtendedFlags[12] = char(0x00);
		ExtendedFlags[13] = char(0x00);
		ExtendedFlags[14] = char(0x00);
		ExtendedFlags[15] = char(0x00);
		ExtendedFlags[16] = char(0x00);
		ExtendedFlags[17] = char(0x00);
		ExtendedFlags[18] = char(0x00);
		ExtendedFlags[19] = char(0x00);
		ExtendedFlags[20] = char(0x00);
		ExtendedFlags[21] = char(0xC0);
		ExtendedFlags[22] = char(0xFF);
		ExtendedFlags[23] = char(0xFF);
		ExtendedFlags[24] = char(0xFF);
		ExtendedFlags[25] = char(0xFF);
		ExtendedFlags[26] = char(0xFF);
		ExtendedFlags[27] = char(0xFF);
		ExtendedFlags[28] = char(0xFF);
		ExtendedFlags[29] = char(0xFF);
		ExtendedFlags[30] = char(0xFF);
		ExtendedFlags[31] = char(0xFF);
		ExtendedFlags[32] = char(0xFF);
		ExtendedFlags[33] = char(0xFF);
		ExtendedFlags[34] = char(0xFF);
		ExtendedFlags[35] = char(0xFF);
		ExtendedFlags[36] = char(0xFF);
		ExtendedFlags[37] = char(0xFF);
		ExtendedFlags[38] = char(0xFF);
		ExtendedFlags[39] = char(0xFF);
		ExtendedFlags[40] = char(0xFF);
		ExtendedFlags[41] = char(0xFF);
		ExtendedFlags[42] = char(0xFF);

		for (int i=0;i<170;i++)
		{
			Ability[i].Init(i, this, &Data->Ability[i]);
		}
	}

    void ClearFlags();

	void BuildPacket(unsigned char *, long &);
	void BuildExtendedPacket(unsigned char *, long &);

	_SkillAbilities * GetData();

	void SetData(_SkillAbilities *);

private:
    _SkillAbilities * Data;

	unsigned char Flags[22];
	unsigned char ExtendedFlags[43];

public:
	class AuxSkillAbility Ability[170];
};

#endif
