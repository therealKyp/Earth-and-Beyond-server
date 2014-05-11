#ifndef _AUXBUFF_H_INCLUDED_
#define _AUXBUFF_H_INCLUDED_

#include "AuxElements.h"
	
struct _Buff
{
	char BuffType[64];
	char ScrubTypeName[64];
	bool IsPermanent;
	u32 BuffRemovalTime;
	_Elements Elements;
} ATTRIB_PACKED;

class AuxBuff : public AuxBase
{
public:
    AuxBuff()
	{
	}

    ~AuxBuff()
	{
	}

	void Init(unsigned int MemberIndex, class AuxBase * Parent, _Buff * DataPointer)
	{
		Construct(Flags, ExtendedFlags, 5, Parent, MemberIndex);
        Data = DataPointer;

		memset(Flags,0, sizeof(Flags));

		ExtendedFlags[0] = char(0x06);
		ExtendedFlags[1] = char(0x3E);

		*Data->BuffType = 0; 
		*Data->ScrubTypeName = 0; 
		Data->IsPermanent = 0;
		Data->BuffRemovalTime = 0;
		Elements.Init(4, this, &Data->Elements);
	}

    void Clear();
    void ClearFlags();

	void BuildPacket(unsigned char *, long &);
	void BuildExtendedPacket(unsigned char *, long &);

	_Buff * GetData();

	char * GetBuffType();
	char * GetScrubTypeName();
	bool GetIsPermanent();
	u32 GetBuffRemovalTime();

	void SetData(_Buff *);

	void SetBuffType(char *);
	void SetScrubTypeName(char *);
	void SetIsPermanent(bool);
	void SetBuffRemovalTime(u32);

protected:
    void CheckData();

private:
	int HasData();

	_Buff * Data;

	unsigned char Flags[2];
	unsigned char ExtendedFlags[2];

public:
	class AuxElements Elements;

};

#endif
