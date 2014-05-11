#ifndef _AUXMISSIONSTAGE_H_INCLUDED_
#define _AUXMISSIONSTAGE_H_INCLUDED_

#include "AuxBase.h"
	
struct _MissionStage
{
	char	Text[512];
	bool	IsTimed;
} ATTRIB_PACKED;

class AuxMissionStage : public AuxBase
{
public:
    AuxMissionStage()
	{
	}

    ~AuxMissionStage()
	{
	}

	void Init(unsigned int MemberIndex, class AuxBase * Parent, _MissionStage * DataPointer)
	{
		Construct(Flags, ExtendedFlags, 2, Parent, MemberIndex);
        Data = DataPointer;

		memset(Flags,0, sizeof(Flags));

		ExtendedFlags[0] = char(0xC6);

	    *Data->Text = 0; 
		Data->IsTimed = 0;
	}

    _MissionStage GetClearStruct();
    void Clear();
    void ClearFlags();

	void BuildPacket(unsigned char *, long &);
	void BuildExtendedPacket(unsigned char *, long &);

	_MissionStage * GetData();

	char * GetText();
	bool GetIsTimed();

	void SetData(_MissionStage *);

	void SetText(char *);
	void SetIsTimed(bool);

protected:
    void CheckData();

private:
	int HasData();

	_MissionStage * Data;

	unsigned char Flags[1];
	unsigned char ExtendedFlags[1];
};

#endif
