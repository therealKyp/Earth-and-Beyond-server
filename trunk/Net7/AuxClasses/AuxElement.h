#ifndef _AUXELEMENT_H_INCLUDED_
#define _AUXELEMENT_H_INCLUDED_

#include "AuxBase.h"
	
struct _Element
{
	char	SourceEntity[64];
	char	SourceObject[64];
	u32		Magnitude;
	bool	IsActive;
	u32		ExpirationTime;
} ATTRIB_PACKED;

class AuxElement : public AuxBase
{
public:
    AuxElement()
	{
	}

    ~AuxElement()
	{
	}

	void Init(unsigned int MemberIndex, class AuxBase * Parent, _Element * DataPointer)
	{
		Construct(Flags, ExtendedFlags, 5, Parent, MemberIndex);
        Data = DataPointer;

		memset(Flags,0, sizeof(Flags));

		ExtendedFlags[0] = char(0x06);
		ExtendedFlags[1] = char(0x3E);

        *Data->SourceEntity = 0; 
		*Data->SourceObject = 0; 
		Data->Magnitude = 0;
		Data->IsActive = 0;
		Data->ExpirationTime = 0;
	}

    _Element GetClearStruct();
    void Clear();
    void ClearFlags();

	void BuildPacket(unsigned char *, long &);
	void BuildExtendedPacket(unsigned char *, long &);

	_Element * GetData();

	char * GetSourceEntity();
	char * GetSourceObject();
	u32 GetMagnitude();
	bool GetIsActive();
	u32 GetExpirationTime();

	void SetData(_Element *);

	void SetSourceEntity(char *);
	void SetSourceObject(char *);
	void SetMagnitude(u32);
	void SetIsActive(bool);
	void SetExpirationTime(u32);

protected:
    void CheckData();

private:
	int HasData();

	_Element * Data;

	unsigned char Flags[2];
	unsigned char ExtendedFlags[2];
};

#endif
