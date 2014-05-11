#ifndef _AUXSTATS_H_INCLUDED_
#define _AUXSTATS_H_INCLUDED_

#include "AuxBase.h"
	
struct _Stats
{
	s32	Defence;
	s32	MissleDefence;
	s32	Speed;
	s32	WarpSpeed;
	s32	WarpPowerLevel;
	s32	TurnRate;
	s32	ScanRange;
	s32	Visibility;
	s32	ResistImpact;
	s32	ResistExplosion;
	s32	ResistPlasma; 
	s32	ResistEnergy;
	s32	ResistEMP;
	s32	ResistChemical;
	s32	ResistPsionic;
} ATTRIB_PACKED;

class AuxStats : public AuxBase
{
public:
    AuxStats()
	{
	}

    ~AuxStats()
	{
	}

	void Init(unsigned int MemberIndex, class AuxBase * Parent, _Stats * DataPointer)
	{
		Construct(Flags, ExtendedFlags, 15, Parent, MemberIndex);
        Data = DataPointer;

		memset(Flags,0, sizeof(Flags));

		ExtendedFlags[0] = char(0x06);
		ExtendedFlags[1] = char(0x00);
		ExtendedFlags[2] = char(0xF8);
		ExtendedFlags[3] = char(0xFF);
		ExtendedFlags[4] = char(0x03);

		Data->Defence = 0;
		Data->MissleDefence = 0;
		Data->Speed = 0;
		Data->WarpSpeed = 0;
		Data->WarpPowerLevel = 0;
		Data->TurnRate = 0;
		Data->ScanRange = 0;
		Data->Visibility = 0;
		Data->ResistImpact = 0;
		Data->ResistExplosion = 0;
		Data->ResistPlasma = 0; 
		Data->ResistEnergy = 0;
		Data->ResistEMP = 0;
		Data->ResistChemical = 0;
		Data->ResistPsionic = 0;
	}

    void ClearFlags();

    void BuildPacket(unsigned char *, long &);
    void BuildExtendedPacket(unsigned char *, long &);

	_Stats * GetData();

	s32 GetDefence();
	s32 GetMissleDefence();
	s32 GetSpeed();
	s32 GetWarpSpeed();
	s32 GetWarpPowerLevel();
	s32 GetTurnRate();
	s32 GetScanRange();
	s32 GetVisibility();
	s32 GetResistImpact();
	s32 GetResistExplosion();
	s32 GetResistPlasma(); 
	s32 GetResistEnergy();
	s32 GetResistEMP();
	s32 GetResistChemical();
	s32 GetResistPsionic();

	void SetData(_Stats *);

	void SetDefence(s32);
	void SetMissleDefence(s32);
	void SetSpeed(s32);
	void SetWarpSpeed(s32);
	void SetWarpPowerLevel(s32);
	void SetTurnRate(s32);
	void SetScanRange(s32);
	void SetVisibility(s32);
	void SetResistImpact(s32);
	void SetResistExplosion(s32);
	void SetResistPlasma(s32); 
	void SetResistEnergy(s32);
	void SetResistEMP(s32);
	void SetResistChemical(s32);
	void SetResistPsionic(s32);

private:
	_Stats * Data;

	unsigned char Flags[3];
	unsigned char ExtendedFlags[5];
};

#endif
