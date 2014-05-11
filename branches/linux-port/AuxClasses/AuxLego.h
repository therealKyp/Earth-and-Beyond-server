#ifndef _AUXLEGO_H_INCLUDED_
#define _AUXLEGO_H_INCLUDED_

#include "AuxAttachments.h"
	
struct _Lego
{
	float Scale;
	_Attachments Attachments;
} ATTRIB_PACKED;

class AuxLego : public AuxBase
{
public:
    AuxLego()
	{
	}

    ~AuxLego()
	{
	}

	void Init(unsigned int MemberIndex, class AuxBase * Parent, _Lego * DataPointer)
	{
		Construct(Flags, ExtendedFlags, 2, Parent, MemberIndex);
        Data = DataPointer;

		memset(Flags,0, sizeof(Flags));

		ExtendedFlags[0] = char(0xE6);

        Data->Scale = 0;
		Attachments.Init(1, this, &Data->Attachments);
	}

    void ClearFlags();

	void BuildPacket(unsigned char *, long &);
	void BuildExtendedPacket(unsigned char *, long &);
	void BuildSpecialPacket(unsigned char *, long &);

	_Lego * GetData();

	float GetScale();

	void SetData(_Lego *);

	void SetScale(float);

private:
	_Lego * Data;

	unsigned char Flags[1];
	unsigned char ExtendedFlags[1];

public:
	class AuxAttachments Attachments;
};

#endif
