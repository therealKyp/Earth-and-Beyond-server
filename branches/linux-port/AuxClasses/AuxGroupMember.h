#ifndef _AUXGROUPMEMBER_H_INCLUDED_
#define _AUXGROUPMEMBER_H_INCLUDED_

#include "AuxBase.h"
	
struct _GroupMember
{
	char Name[64];
	u32 GameID;
	u32 Formation;
	u32 Position;
} ATTRIB_PACKED;

class AuxGroupMember : public AuxBase
{
public:
    AuxGroupMember()
	{
	}

    ~AuxGroupMember()
	{
	}

	void Init(unsigned int MemberIndex, class AuxBase * Parent, _GroupMember * DataPointer)
	{
		Construct(Flags, ExtendedFlags, 4, Parent, MemberIndex);
        Data = DataPointer;

		memset(Flags,0, sizeof(Flags));

		ExtendedFlags[0] = char(0x06);
		ExtendedFlags[1] = char(0x0F);

		*Data->Name = 0; 
		Data->GameID = 0;
		Data->Formation = 0;
		Data->Position = 0;
	}

    void Clear();
    void ClearFlags();

	void BuildPacket(unsigned char *, long &);
	void BuildExtendedPacket(unsigned char *, long &);

	_GroupMember * GetData();

	char * GetName();
	u32 GetGameID();
	u32 GetFormation();
	u32 GetPosition();

	void SetData(_GroupMember *);

	void SetName(char *);
	void SetGameID(u32);
	void SetFormation(u32);
	void SetPosition(u32);

protected:
    void CheckData();

private:
	int HasData();

	_GroupMember * Data;

	unsigned char Flags[1];
	unsigned char ExtendedFlags[2];
};

#endif
