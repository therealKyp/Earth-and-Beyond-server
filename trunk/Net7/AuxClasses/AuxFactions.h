#ifndef _AUXFACTIONS_H_INCLUDED_
#define _AUXFACTIONS_H_INCLUDED_

#include "AuxFaction.h"
	
struct _Factions
{
	_Faction Faction[32];
} ATTRIB_PACKED;

class AuxFactions : public AuxBase
{
public:
    AuxFactions()
	{
	}

    ~AuxFactions()
	{
	}

	void Init(unsigned int MemberIndex, class AuxBase * Parent, _Factions * DataPointer)
	{
		Construct(Flags, ExtendedFlags, 32, Parent, MemberIndex);
        Data = DataPointer;

		memset(Flags,0, sizeof(Flags));

		ExtendedFlags[0] = char(0x06);
		ExtendedFlags[1] = char(0x00);
		ExtendedFlags[2] = char(0x00);
		ExtendedFlags[3] = char(0x00);
		ExtendedFlags[4] = char(0xF0);
		ExtendedFlags[5] = char(0xFF);
		ExtendedFlags[6] = char(0xFF);
		ExtendedFlags[7] = char(0xFF);
		ExtendedFlags[8] = char(0x0F);

		for (int i=0;i<32;i++)
		{
			Faction[i].Init(i, this, &Data->Faction[i]);
		}
	}

    void ClearFlags();

	void BuildPacket(unsigned char *, long &);
	void BuildExtendedPacket(unsigned char *, long &);

	_Factions * GetData();

	void SetData(_Factions *);

private:
    _Factions * Data;

	unsigned char Flags[5];
	unsigned char ExtendedFlags[9];

public:
	class AuxFaction Faction[32];
};

#endif
