#ifndef _AUXCATEGORIES_H_INCLUDED_
#define _AUXCATEGORIES_H_INCLUDED_

#include "AuxCategory.h"
	
struct _Categories
{
	_Category Category[5];
} ATTRIB_PACKED;

class AuxCategories : public AuxBase
{
public:
    AuxCategories()
	{
	}

    ~AuxCategories()
	{
	}

	void Init(unsigned int MemberIndex, class AuxBase * Parent, _Categories * DataPointer)
	{
		Construct(Flags, ExtendedFlags, 5, Parent, MemberIndex);
        Data = DataPointer;

		memset(Flags,0, sizeof(Flags));

		ExtendedFlags[0] = char(0x06);
		ExtendedFlags[1] = char(0x3E);

		for (int i=0;i<5;i++)
		{
			Category[i].Init(i, this, &Data->Category[i]);
		}
	}

    void ClearFlags();

	void BuildPacket(unsigned char *, long &, bool);

	_Categories * GetData();

	void SetData(_Categories *);

protected:
    void CheckData();

private:
    int HasData();

    _Categories * Data;

	unsigned char Flags[2];
	unsigned char ExtendedFlags[2];

public:
	class AuxCategory Category[5];
};

#endif
