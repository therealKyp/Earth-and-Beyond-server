#ifndef _AUXQUADRANTDAMAGE_H_INCLUDED_
#define _AUXQUADRANTDAMAGE_H_INCLUDED_

#include "AuxBase.h"
	
struct _QuadrantDamage
{
	float Slot1;
	float Slot2;
	float Slot3;
	float Slot4;
} ATTRIB_PACKED;

class AuxQuadrantDamage : public AuxBase
{
public:
    AuxQuadrantDamage()
	{
	}

    ~AuxQuadrantDamage()
	{
	}

	void Init(unsigned int MemberIndex, class AuxBase * Parent, _QuadrantDamage * DataPointer)
	{
		Construct(Flags, ExtendedFlags, 4, Parent, MemberIndex);
        Data = DataPointer;

		memset(Flags,0, sizeof(Flags));

		ExtendedFlags[0] = char(0x06);
		ExtendedFlags[1] = char(0x0F);

		Data->Slot1 = 0;
		Data->Slot2 = 0;
		Data->Slot3 = 0;
		Data->Slot4 = 0;
	}

    void Clear();
    void ClearFlags();

	void BuildPacket(unsigned char *, long &);
	void BuildExtendedPacket(unsigned char *, long &);
	void BuildSpecialPacket(unsigned char *, long &);

	_QuadrantDamage * GetData();

	float GetSlot1();
	float GetSlot2();
	float GetSlot3();
	float GetSlot4();

	void SetData(_QuadrantDamage *);

	void SetSlot1(float);
	void SetSlot2(float);
	void SetSlot3(float);
	void SetSlot4(float);

private:
	_QuadrantDamage * Data;

	unsigned char Flags[1];
	unsigned char ExtendedFlags[2];
};

#endif
