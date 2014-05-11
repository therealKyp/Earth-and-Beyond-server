#ifndef _AUXSHAKE_H_INCLUDED_
#define _AUXSHAKE_H_INCLUDED_

#include "AuxBase.h"
	
struct _Shake
{
	float ForceX;
	float ForceY;
	float ForceZ;
	float Damage;
} ATTRIB_PACKED;

class AuxShake : public AuxBase
{
public:
    AuxShake()
	{
	}

    ~AuxShake()
	{
	}

	void Init(unsigned int MemberIndex, class AuxBase * Parent, _Shake * DataPointer)
	{
		Construct(Flags, ExtendedFlags, 4, Parent, MemberIndex);
        Data = DataPointer;

		memset(Flags,0, sizeof(Flags));

		ExtendedFlags[0] = char(0x06);
		ExtendedFlags[1] = char(0x0F);

		Data->ForceX = 0;
		Data->ForceY = 0;
		Data->ForceZ = 0;
		Data->Damage = 0;
	}

    void Clear();
    void ClearFlags();

	void BuildPacket(unsigned char *, long &);
	void BuildExtendedPacket(unsigned char *, long &);

	_Shake * GetData();

	float GetForceX();
	float GetForceY();
	float GetForceZ();
	float GetDamage();

	void SetData(_Shake *);

	void SetForceX(float);
	void SetForceY(float);
	void SetForceZ(float);
	void SetDamage(float);

private:
	_Shake * Data;

	unsigned char Flags[1];
	unsigned char ExtendedFlags[2];
};

#endif
