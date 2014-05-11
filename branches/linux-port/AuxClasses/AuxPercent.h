#ifndef _AUXPERCENT_H_INCLUDED_
#define _AUXPERCENT_H_INCLUDED_

#include "AuxBase.h"
	
struct _Percent
{
	u32 EndTime;
	float ChangePerTick;
	float StartValue;
} ATTRIB_PACKED;

class AuxPercent : public AuxBase
{
public:
    AuxPercent()
	{
	}

    ~AuxPercent()
	{
	}

	void Init(unsigned int MemberIndex, class AuxBase * Parent, _Percent * DataPointer)
	{
		Construct(Flags, ExtendedFlags, 3, Parent, MemberIndex);
        Data = DataPointer;

		memset(Flags,0, sizeof(Flags));

		ExtendedFlags[0] = char(0x86);
		ExtendedFlags[1] = char(0x03);

		Data->EndTime = 0;
		Data->ChangePerTick = 0;
		Data->StartValue = 0;
	}

    void ClearFlags();

	void BuildPacket(unsigned char *, long &);
	void BuildExtendedPacket(unsigned char *, long &);
	void BuildSpecialPacket(unsigned char *, long &);

	_Percent * GetData();

	u32 GetEndTime();
	float GetChangePerTick();
	float GetStartValue();

	void SetData(_Percent *);

	void SetEndTime(u32);
	void SetChangePerTick(float);
	void SetStartValue(float);


private:
	_Percent * Data;

	unsigned char Flags[1];
	unsigned char ExtendedFlags[2];
};

#endif
