#ifndef _AUXSKILLS_H_INCLUDED_
#define _AUXSKILLS_H_INCLUDED_

#include "AuxSkill.h"
	
struct _Skills
{
	_Skill Skill[170];
} ATTRIB_PACKED;

class AuxSkills : public AuxBase
{
public:
    AuxSkills()
	{
	}

    ~AuxSkills()
	{
	}

	void Init(unsigned int MemberIndex, class AuxBase * Parent, _Skills * DataPointer)
	{
		Construct(Flags, ExtendedFlags, 64, Parent, MemberIndex);
        Data = DataPointer;

		memset(Flags,0, sizeof(Flags));

		ExtendedFlags[0] = char(0xF6);
		ExtendedFlags[1] = char(0xFF);
		ExtendedFlags[2] = char(0xFF);
		ExtendedFlags[3] = char(0xFF);
		ExtendedFlags[4] = char(0xFF);
		ExtendedFlags[5] = char(0xFF);
		ExtendedFlags[6] = char(0xFF);
		ExtendedFlags[7] = char(0x3F);
		ExtendedFlags[8] = char(0xF0);
		ExtendedFlags[9] = char(0xFF);
		ExtendedFlags[10] = char(0xFF);
		ExtendedFlags[11] = char(0xFF);
		ExtendedFlags[12] = char(0xFF);
		ExtendedFlags[13] = char(0xFF);
		ExtendedFlags[14] = char(0xFF);
		ExtendedFlags[15] = char(0xFF);
		ExtendedFlags[16] = char(0x0F);

		for (int i=0;i<64;i++)
		{
			Skill[i].Init(i, this, &Data->Skill[i]);
		}
	}

    void ClearFlags();

	void BuildPacket(unsigned char *, long &);
	void BuildExtendedPacket(unsigned char *, long &);

	_Skills * GetData();

	void SetData(_Skills *);

private:
    _Skills * Data;

	unsigned char Flags[9];
	unsigned char ExtendedFlags[17];

public:
	class AuxSkill Skill[64];
};

#endif
