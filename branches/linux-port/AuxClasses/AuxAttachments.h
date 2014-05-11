#ifndef _AUXATTACHMENTS_H_INCLUDED_
#define _AUXATTACHMENTS_H_INCLUDED_

#include "AuxAttachment.h"
	
struct _Attachments
{
	_Attachment Attachment[18];
} ATTRIB_PACKED;

class AuxAttachments : public AuxBase
{
public:
    AuxAttachments()
	{
	}

    ~AuxAttachments()
	{
	}

	void Init(unsigned int MemberIndex, class AuxBase * Parent, _Attachments * DataPointer)
	{
		Construct(Flags, ExtendedFlags, 18, Parent, MemberIndex);
        Data = DataPointer;

		memset(Flags,0, sizeof(Flags));

		ExtendedFlags[0] = char(0x06);
		ExtendedFlags[1] = char(0x00);
		ExtendedFlags[2] = char(0xC0);
		ExtendedFlags[3] = char(0xFF);
		ExtendedFlags[4] = char(0xFF);

		for (int i=0;i<18;i++)
		{
			Attachment[i].Init(i, this, &Data->Attachment[i]);
		}
	}

    void ClearFlags();

	void BuildPacket(unsigned char *, long &);
	void BuildExtendedPacket(unsigned char *, long &);
	void BuildSpecialPacket(unsigned char *, long &);

	_Attachments * GetData();

	void SetData(_Attachments *);

private:
    _Attachments * Data;

	unsigned char Flags[3];
	unsigned char ExtendedFlags[5];

public:
	class AuxAttachment Attachment[18];
};

#endif
