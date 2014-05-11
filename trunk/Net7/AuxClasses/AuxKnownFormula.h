#ifndef _AUXKNOWNFORMULA_H_INCLUDED_
#define _AUXKNOWNFORMULA_H_INCLUDED_

#include "AuxBase.h"
	
struct _KnownFormula
{
	char ItemName[32];
	u32 ItemID;
	u32 TechLevel;
} ATTRIB_PACKED;

class AuxKnownFormula : public AuxBase
{
public:
    AuxKnownFormula()
	{
	}

    ~AuxKnownFormula()
	{
	}

	void Init(unsigned int MemberIndex, class AuxBase * Parent, _KnownFormula * DataPointer)
	{
		Construct(Flags, ExtendedFlags, 3, Parent, MemberIndex);
        Data = DataPointer;

		memset(Flags,0, sizeof(Flags));

		ExtendedFlags[0] = char(0x86);
		ExtendedFlags[1] = char(0x03);

		*Data->ItemName = 0;
		Data->ItemID = 0;
		Data->TechLevel = 0;
	}

    void ClearFlags();

	void BuildPacket(unsigned char *, long &, bool);

	_KnownFormula * GetData();

	char * GetItemName();
	u32 GetItemID();
	u32 GetTechLevel();

	void SetData(_KnownFormula *);

	void SetItemName(char *);
	void SetItemID(u32);
	void SetTechLevel(u32);

private:
	_KnownFormula * Data;

	unsigned char Flags[1];
	unsigned char ExtendedFlags[2];
};

#endif
