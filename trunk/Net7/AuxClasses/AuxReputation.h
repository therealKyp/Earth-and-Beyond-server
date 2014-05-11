#ifndef _AUXREPUTATION_H_INCLUDED_
#define _AUXREPUTATION_H_INCLUDED_

#include "AuxFactions.h"
	
struct _Reputation
{
	_Factions Factions;
	char Affiliation[64];
} ATTRIB_PACKED;

class AuxReputation : public AuxBase
{
public:
    AuxReputation()
	{
	}

    ~AuxReputation()
	{
	}

	void Init(unsigned int MemberIndex, class AuxBase * Parent, _Reputation * DataPointer)
	{
		Construct(Flags, ExtendedFlags, 2, Parent, MemberIndex);
        Data = DataPointer;

		memset(Flags,0, sizeof(Flags));

		ExtendedFlags[0] = char(0xD6);

		Factions.Init(0, this, &Data->Factions);
		*Data->Affiliation = 0;
	}

    void ClearFlags();

	void BuildPacket(unsigned char *, long &);
	void BuildExtendedPacket(unsigned char *, long &);

	_Reputation * GetData();

	char * GetAffiliation();

	void SetData(_Reputation *);

	void SetAffilitation(char *);

private:
	_Reputation * Data;

	unsigned char Flags[1];
	unsigned char ExtendedFlags[1];

public:
	class AuxFactions Factions;
};

#endif
