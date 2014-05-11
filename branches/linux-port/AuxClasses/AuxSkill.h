#ifndef _AUXSKILL_H_INCLUDED_
#define _AUXSKILL_H_INCLUDED_

#include "AuxBase.h"
	
struct _Skill
{
	u32	Level;
	u32	RecycleTime;
	u32	LastActivationTime;
	u32	Availability[4];
	u32	QuestOnlyLevel;
	u32	MaxSkillLevel;
	char Affiliation[64];
} ATTRIB_PACKED;

class AuxSkill : public AuxBase
{
public:
    AuxSkill()
	{
	}

    ~AuxSkill()
	{
	}

	void Init(unsigned int MemberIndex, class AuxBase * Parent, _Skill * DataPointer)
	{
		Construct(Flags, ExtendedFlags, 7, Parent, MemberIndex);
        Data = DataPointer;

		memset(Flags,0, sizeof(Flags));

		ExtendedFlags[0] = char(0xC6);
		ExtendedFlags[1] = char(0x19);
		ExtendedFlags[2] = char(0x03);

		Data->Level = 0;
		Data->RecycleTime = 0;
		Data->LastActivationTime = 1;	/* Always set in captured logs */
		Data->Availability[0] = 1;
		Data->Availability[1] = 0;
		Data->Availability[2] = 0;
		Data->Availability[3] = 0;
		Data->QuestOnlyLevel = 9;
		Data->MaxSkillLevel = 0;
		*Data->Affiliation = 0;
	}

    void Clear();
    void ClearFlags();

	void BuildPacket(unsigned char *, long &);
	void BuildExtendedPacket(unsigned char *, long &);

	_Skill * GetData();

	u32	GetLevel();
	u32	GetRecycleTime();
	u32	GetLastActivationTime();
	u32	* GetAvailability();
	u32	GetQuestOnlyLevel();
	u32	GetMaxSkillLevel();
	char * GetAffiliation();

	void SetData(_Skill *);

	void SetLevel(u32);
	void SetRecycleTime(u32);
	void SetLastActivationTime(u32);
	void SetAvailability(u32 *);
	void SetQuestOnlyLevel(u32);
	void SetMaxSkillLevel(u32);
	void SetAffiliation(char *);


private:
	_Skill * Data;

	unsigned char Flags[2];
	unsigned char ExtendedFlags[3];
};

#endif
