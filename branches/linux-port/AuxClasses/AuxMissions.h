#ifndef _AUXMISSIONS_H_INCLUDED_
#define _AUXMISSIONS_H_INCLUDED_

#include "AuxMission.h"
	
struct _Missions
{
	_Mission Mission[12];
} ATTRIB_PACKED;

class AuxMissions : public AuxBase
{
public:
    AuxMissions()
	{
	}

    ~AuxMissions()
	{
	}

	void Init(unsigned int MemberIndex, class AuxBase * Parent, _Missions * DataPointer)
	{
		Construct(Flags, ExtendedFlags, 12, Parent, MemberIndex);
        Data = DataPointer;

		memset(Flags,0, sizeof(Flags));

		ExtendedFlags[0] = char(0xF6);
		ExtendedFlags[1] = char(0x3F);
		ExtendedFlags[2] = char(0xFF);
		ExtendedFlags[3] = char(0x0F);

		for (int i=0;i<12;i++)
		{
			Mission[i].Init(i, this, &Data->Mission[i]);
			Mission[i].SetID(i);
		}
	}

    void ClearFlags();

	void BuildPacket(unsigned char *, long &);
	void BuildExtendedPacket(unsigned char *, long &);

	_Missions * GetData();

	void SetData(_Missions *);

private:
    _Missions * Data;

	unsigned char Flags[2];
	unsigned char ExtendedFlags[4];

public:
	class AuxMission Mission[12];
};

#endif
