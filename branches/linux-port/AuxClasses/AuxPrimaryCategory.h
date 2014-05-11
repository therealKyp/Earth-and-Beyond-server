#ifndef _AUXPRIMARYCATEGORY_H_INCLUDED_
#define _AUXPRIMARYCATEGORY_H_INCLUDED_

#include "AuxCategories.h"
	
struct _PrimaryCategory
{
	char Name[20];
	_Categories Categories;
} ATTRIB_PACKED;

class AuxPrimaryCategory : public AuxBase
{
public:
    AuxPrimaryCategory()
	{
	}

    ~AuxPrimaryCategory()
	{
	}

	void Init(unsigned int MemberIndex, class AuxBase * Parent, _PrimaryCategory * DataPointer)
	{
		Construct(Flags, ExtendedFlags, 2, Parent, MemberIndex);
        Data = DataPointer;

		memset(Flags,0, sizeof(Flags));

		ExtendedFlags[0] = char(0xC6);

        *Data->Name = 0;
        Categories.Init(1, this, &Data->Categories);
	}

    void ClearFlags();

	void BuildPacket(unsigned char *, long &, bool);

	_PrimaryCategory * GetData();

	char * GetName();

	void SetData(_PrimaryCategory *);

	void SetName(char *);

protected:
    void CheckData();

private:
	int HasData();

    _PrimaryCategory * Data;

	unsigned char Flags[1];
	unsigned char ExtendedFlags[1];

public:
	class AuxCategories Categories;
};

#endif
