#ifndef _AUXSUBCATEGORY_H_INCLUDED_
#define _AUXSUBCATEGORY_H_INCLUDED_

#include "AuxBase.h"
	
struct _SubCategory
{
	char Name[32];
	u32 SubCategoryID;
	bool IsVisible;
} ATTRIB_PACKED;

class AuxSubCategory : public AuxBase
{
public:
    AuxSubCategory()
	{
	}

    ~AuxSubCategory()
	{
	}

	void Init(unsigned int MemberIndex, class AuxBase * Parent, _SubCategory * DataPointer)
	{
		Construct(Flags, ExtendedFlags, 3, Parent, MemberIndex);
        Data = DataPointer;

		memset(Flags,0, sizeof(Flags));

		ExtendedFlags[0] = char(0x86);
		ExtendedFlags[1] = char(0x03);

		*Data->Name = 0; 
		Data->SubCategoryID = 0;
		Data->IsVisible = 0;
	}

    void ClearFlags();

    void BuildPacket(unsigned char *, long &, bool);

	_SubCategory * GetData();

	char * GetName();
	u32 GetSubCategoryID();
	bool GetIsVisible();

	void SetData(_SubCategory *);

	void SetName(char *);
	void SetSubCategoryID(u32);
	void SetIsVisible(bool);

protected:
    void CheckData();

private:
	int HasData();

	_SubCategory * Data;

	unsigned char Flags[1];
	unsigned char ExtendedFlags[2];
};

#endif
