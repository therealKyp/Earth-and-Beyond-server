#ifndef _AUXMISSIONSTAGES_H_INCLUDED_
#define _AUXMISSIONSTAGES_H_INCLUDED_

#include "AuxMissionStage.h"
	
struct _MissionStages
{
	_MissionStage Stage[20];
} ATTRIB_PACKED;

class AuxMissionStages : public AuxBase
{
public:
    AuxMissionStages()
	{
	}

    ~AuxMissionStages()
	{
	}

	void Init(unsigned int MemberIndex, class AuxBase * Parent, _MissionStages * DataPointer)
	{
		Construct(Flags, ExtendedFlags, 20, Parent, MemberIndex);
        Data = DataPointer;

		memset(Flags,0, sizeof(Flags));

		ExtendedFlags[0] = char(0x06);
		ExtendedFlags[1] = char(0x00);
		ExtendedFlags[2] = char(0x00);
		ExtendedFlags[3] = char(0xFF);
		ExtendedFlags[4] = char(0xFF);
		ExtendedFlags[5] = char(0x0F);

		for (int i=0;i<20;i++)
		{
			Stage[i].Init(i, this, &Data->Stage[i]);
		}
	}

    _MissionStages GetClearStruct();
    void Clear();
    void ClearFlags();

	void BuildPacket(unsigned char *, long &);
	void BuildExtendedPacket(unsigned char *, long &);

	_MissionStages * GetData();

	void SetData(_MissionStages *);

protected:
    void CheckData();

private:
	int HasData();

    _MissionStages * Data;

	unsigned char Flags[3];
	unsigned char ExtendedFlags[6];

public:
	class AuxMissionStage Stage[20];
};

#endif
