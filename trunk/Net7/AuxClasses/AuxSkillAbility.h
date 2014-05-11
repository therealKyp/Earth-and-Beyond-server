#ifndef _AUXSKILLABILITY_H_INCLUDED_
#define _AUXSKILLABILITY_H_INCLUDED_

#include "AuxBase.h"
	
struct _SkillAbility
{
	bool	HasAbility;
	u32		Usage;
	u32		Targets;
	u32		Range; 
	u32		Radius;
	u32		Validity;
	bool	CanBeUsedWhileIncapacitated;
} ATTRIB_PACKED;

class AuxSkillAbility : public AuxBase
{
public:
    AuxSkillAbility()
	{
	}

    ~AuxSkillAbility()
	{
	}

	void Init(unsigned int MemberIndex, class AuxBase * Parent, _SkillAbility * DataPointer)
	{
		Construct(Flags, ExtendedFlags, 7, Parent, MemberIndex);
        Data = DataPointer;

		memset(Flags,0, sizeof(Flags));

		ExtendedFlags[0] = char(0x06);
		ExtendedFlags[1] = char(0xF8);
		ExtendedFlags[2] = char(0x03);

		Data->HasAbility = false;
		Data->Usage = 0;
		Data->Targets = 0;
		Data->Range = 0; 
		Data->Radius = 0;
		Data->Validity = 0;
		Data->CanBeUsedWhileIncapacitated = 0;
	}

    void Clear();
    void ClearFlags();

	void BuildPacket(unsigned char *, long &);
	void BuildExtendedPacket(unsigned char *, long &);

	_SkillAbility * GetData();

	bool GetHasAbility();
	u32 GetUsage();
	u32 GetTargets();
	u32 GetRange(); 
	u32 GetRadius();
	u32 GetValidity();
	bool GetCanBeUsedWhileIncapacitated();

	void SetData(_SkillAbility *);

	void SetHasAbility(bool);
	void SetUsage(u32);
	void SetTargets(u32);
	void SetRange(u32); 
	void SetRadius(u32);
	void SetValidity(u32);
	void SetCanBeUsedWhileIncapacitated(bool);

protected:
    void CheckData();

private:
	int HasData();

	_SkillAbility * Data;

	unsigned char Flags[2];
	unsigned char ExtendedFlags[3];
};

#endif
