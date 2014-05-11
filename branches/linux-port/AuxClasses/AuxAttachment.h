#ifndef _AUXATTACHMENT_H_INCLUDED_
#define _AUXATTACHMENT_H_INCLUDED_

#include "AuxBase.h"
	
struct _Attachment
{
	char	BoneName[64];
	u32		Type;
	u32		Asset;
	char	DataStr[64];
} ATTRIB_PACKED;

class AuxAttachment : public AuxBase
{
public:
    AuxAttachment()
	{
	}

    ~AuxAttachment()
	{
	}

	void Init(unsigned int MemberIndex, class AuxBase * Parent, _Attachment * DataPointer)
	{
		Construct(Flags, ExtendedFlags, 4, Parent, MemberIndex);
        Data = DataPointer;

		memset(Flags,0, sizeof(Flags));

		ExtendedFlags[0] = char(0x06);
		ExtendedFlags[1] = char(0x0F);

        *Data->BoneName = 0;
		Data->Type = 0;
		Data->Asset = 0;
		*Data->DataStr = 0; 
	}

    void Clear();
    void ClearFlags();

	void BuildPacket(unsigned char *, long &);
	void BuildExtendedPacket(unsigned char *, long &);
	void BuildSpecialPacket(unsigned char *, long &);

	_Attachment * GetData();

	char * GetBoneName();
	u32 GetType();
	u32 GetAsset();
	char * GetDataStr();

	void SetData(_Attachment *);

	void SetBoneName(char *);
	void SetType(u32);
	void SetAsset(u32);
	void SetDataStr(char *);

protected:
    void CheckData();

private:
	int HasData();

	_Attachment * Data;

	unsigned char Flags[1];
	unsigned char ExtendedFlags[2];
};

#endif
