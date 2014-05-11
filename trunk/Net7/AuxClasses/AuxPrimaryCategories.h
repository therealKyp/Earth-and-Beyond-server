#ifndef _AUXPRIMARYCATEGORIES_H_INCLUDED_
#define _AUXPRIMARYCATEGORIES_H_INCLUDED_

#include "AuxPrimaryCategory.h"
	
struct _PrimaryCategories
{
	_PrimaryCategory PrimaryCategory[2];
} ATTRIB_PACKED;

class AuxPrimaryCategories : public AuxBase
{
public:
    AuxPrimaryCategories()
	{
	}

    ~AuxPrimaryCategories()
	{
	}

	void Init(unsigned int MemberIndex, class AuxBase * Parent, _PrimaryCategories * DataPointer)
	{
		Construct(Flags, ExtendedFlags, 2, Parent, MemberIndex);
        Data = DataPointer;

		memset(Flags,0, sizeof(Flags));

		ExtendedFlags[0] = char(0xC6);

		PrimaryCategory[0].Init(0, this, &Data->PrimaryCategory[0]);
		PrimaryCategory[1].Init(1, this, &Data->PrimaryCategory[1]);
	}

    void ClearFlags();

	void BuildPacket(unsigned char *, long &, bool);

	_PrimaryCategories * GetData();

	void SetData(_PrimaryCategories *);

protected:
    void CheckData();

private:
    int HasData();

    _PrimaryCategories * Data;

	unsigned char Flags[1];
	unsigned char ExtendedFlags[1];

public:
	class AuxPrimaryCategory PrimaryCategory[2];
};

#endif
