#ifndef _AUXELEMENTS_H_INCLUDED_
#define _AUXELEMENTS_H_INCLUDED_

#include "AuxElement.h"
	
struct _Elements
{
	_Element Element[4];
} ATTRIB_PACKED;

class AuxElements : public AuxBase
{
public:
    AuxElements()
	{
	}

    ~AuxElements()
	{
	}

	void Init(unsigned int MemberIndex, class AuxBase * Parent, _Elements * DataPointer)
	{
		Construct(Flags, ExtendedFlags, 4, Parent, MemberIndex);
        Data = DataPointer;

		memset(Flags,0, sizeof(Flags));

		ExtendedFlags[0] = char(0x06);
		ExtendedFlags[1] = char(0x0F);

		Element[0].Init(0, this, &Data->Element[0]);
		Element[1].Init(1, this, &Data->Element[1]);
		Element[2].Init(2, this, &Data->Element[2]);
		Element[3].Init(3, this, &Data->Element[3]);
	}

    _Elements GetClearStruct();
    void Clear();
    void ClearFlags();

	void BuildPacket(unsigned char *, long &);
	void BuildExtendedPacket(unsigned char *, long &);

	_Elements * GetData();

	void SetData(_Elements *);

protected:
    void CheckData();

private:
	int HasData();

    _Elements * Data;

	unsigned char Flags[1];
	unsigned char ExtendedFlags[2];

public:
	class AuxElement Element[4];
};

#endif
