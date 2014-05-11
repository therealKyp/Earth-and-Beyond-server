#ifndef _AUXGROUPMEMBERS_H_INCLUDED_
#define _AUXGROUPMEMBERS_H_INCLUDED_

#include "AuxGroupMember.h"
	
struct _GroupMembers
{
	_GroupMember Member[5];
} ATTRIB_PACKED;

class AuxGroupMembers : public AuxBase
{
public:
    AuxGroupMembers()
	{
	}

    ~AuxGroupMembers()
	{
	}

	void Init(unsigned int MemberIndex, class AuxBase * Parent, _GroupMembers * DataPointer)
	{
		Construct(Flags, ExtendedFlags, 5, Parent, MemberIndex);
        Data = DataPointer;

		memset(Flags,0, sizeof(Flags));

		ExtendedFlags[0] = char(0x06);
		ExtendedFlags[1] = char(0x3E);

		for (int i=0;i<5;i++)
		{
			Member[i].Init(i, this, &Data->Member[i]);
		}
	}

    void Clear();
    void ClearFlags();

	void BuildPacket(unsigned char *, long &);
	void BuildExtendedPacket(unsigned char *, long &);

	_GroupMembers * GetData();

	void SetData(_GroupMembers *);

protected:
    void CheckData();

private:
	int HasData();

    _GroupMembers * Data;

	unsigned char Flags[2];
	unsigned char ExtendedFlags[2];

public:
	class AuxGroupMember Member[5];
};

#endif
