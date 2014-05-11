#ifndef _AUXCATEGORY_H_INCLUDED_
#define _AUXCATEGORY_H_INCLUDED_

#include "AuxSubCategories.h"
	
struct _Category
{
	char Name[32];
	_SubCategories SubCategories;
	u32 CategoryID;
} ATTRIB_PACKED;

class AuxCategory : public AuxBase
{
public:
    AuxCategory()
	{
	}

    ~AuxCategory()
	{
	}

	void Init(unsigned int MemberIndex, class AuxBase * Parent, _Category * DataPointer)
	{
		Construct(Flags, ExtendedFlags, 3, Parent, MemberIndex);
        Data = DataPointer;

		memset(Flags,0, sizeof(Flags));

		ExtendedFlags[0] = char(0x86);
		ExtendedFlags[1] = char(0x03);

        *Data->Name = 0;
        SubCategories.Init(1, this, &Data->SubCategories);
        Data->CategoryID = 0;
	}

    void ClearFlags();

	void BuildPacket(unsigned char *, long &, bool);

	_Category * GetData();

	char * GetName();
	u32 GetCategoryID();

	void SetData(_Category *);

	void SetName(char *);
	void SetCategoryID(u32);

protected:
    void CheckData();

private:
	int HasData();

    _Category * Data;

	unsigned char Flags[1];
	unsigned char ExtendedFlags[2];

public:
	class AuxSubCategories SubCategories;
};

#endif
