#ifndef _AUXSUBCATEGORIES_H_INCLUDED_
#define _AUXSUBCATEGORIES_H_INCLUDED_

#include "AuxSubCategory.h"
	
struct _SubCategories
{
	_SubCategory SubCategory[5];
} ATTRIB_PACKED;

class AuxSubCategories : public AuxBase
{
public:
    AuxSubCategories()
	{
	}

    ~AuxSubCategories()
	{
	}

	void Init(unsigned int MemberIndex, class AuxBase * Parent, _SubCategories * DataPointer)
	{
		Construct(Flags, ExtendedFlags, 5, Parent, MemberIndex);
        Data = DataPointer;

		memset(Flags,0, sizeof(Flags));

		ExtendedFlags[0] = char(0x06);
		ExtendedFlags[1] = char(0x3E);

		for (int i=0;i<5;i++)
		{
			SubCategory[i].Init(i, this, &Data->SubCategory[i]);
		}
	}

    void ClearFlags();

	void BuildPacket(unsigned char *, long &, bool);

	_SubCategories * GetData();

	void SetData(_SubCategories *);

protected:
    void CheckData();

private:
    int HasData();

    _SubCategories * Data;

	unsigned char Flags[2];
	unsigned char ExtendedFlags[2];

public:
	class AuxSubCategory SubCategory[5];
};

#endif
