#ifndef _AUXMOBINDEX_H_INCLUDED_
#define _AUXMOBINDEX_H_INCLUDED_

#include "AuxPercent.h"
#include "AuxQuadrantDamage.h"
#include "AuxDamage.h"
#include "AuxLego.h"

struct _MobIndex
{
	char Name[64];                          //0
	char Owner[64];                         //1
	char Title[64];                         //2
	char Rank[64];                          //3

	_Percent Shield;                        //6
	float MaxShield;                        //7
	float HullPoints;                       //8
	float MaxHullPoints;                    //9

	bool IsCloaked;                         //19
	bool IsCountermeasureActive;            //20
	bool IsIncapacitated;                   //21
	bool IsOrganic;                         //22
	bool IsInPVP;                           //23

	bool IsRescueBeaconActive;              //25
	u32 CombatLevel;                        //26

	u32 GlobalWarpState;                    //32

	_QuadrantDamage QuadrantDamage;         //37
	_Damage DamageSpot;                     //38
	_Damage DamageLine;                     //39
	_Damage DamageBlotch;                   //40
	_Lego Lego;                             //41

	u32 EngineThrustState;                  //46
	u32 EngineTrailType;                    //47

	char InterruptibleAbilityName[128];     //53
	u32 InterruptState;                     //54
	u32 InterruptibleActivationTime;        //55
	float InterruptProgress;                //56
	char FactionIdentifier[64];             //57
} ATTRIB_PACKED;

class AuxMobIndex : public AuxBase
{
public:
    AuxMobIndex()
	{
		Construct(Flags, ExtendedFlags, 58, 0, 0);
		memset(m_PacketBuffer, 0, sizeof(m_PacketBuffer));
		
        m_CreatePacket = m_PacketBuffer;
        m_ClickPacket = m_PacketBuffer;
        m_DiffPacket = m_PacketBuffer;

        m_CreateSize = 0;
        m_ClickSize = 0;
        m_DiffSize = 0;
        m_Mutex = &m_BaseMutex;

        Reset();
	}

    ~AuxMobIndex()
	{
	}

	Mutex * Buffer();

    void Reset();
    void ClearFlags();

    void SetupPackets();

	void BuildCreatePacket();
	void BuildClickPacket();
	bool BuildDiffPacket(bool shields, bool hull);

	_MobIndex * GetData();
	u32 GetGameID();

	char *GetName();
	char *GetOwner();
	char *GetTitle();
	char *GetRank();
	float GetMaxEnergy();
	float GetMaxShield();
	float GetHullPoints();
	float GetMaxHullPoints();

    bool GetIsCloaked();
	bool GetIsCountermeasureActive();
	bool GetIsIncapacitated();
	bool GetIsOrganic();
	bool GetIsInPVP();
	bool GetIsRescueBeaconActive();
	u32 GetCombatLevel();

    u32 GetGlobalWarpState();

    u32 GetEngineThrustState();
	u32 GetEngineTrailType();

    char *GetInterruptibleAbilityName();
	u32 GetInterruptState();
	u32 GetInterruptibleActivationTime();
	float GetInterruptProgress();
	char *GetFactionIdentifier();

	void SetData(_MobIndex *);
	void SetGameID(u32);

	void SetName(char *);
	void SetOwner(char *);
	void SetTitle(char *);
	void SetRank(char *);
	void SetMaxEnergy(float);
	void SetMaxShield(float);
	void SetHullPoints(float);
	void SetMaxHullPoints(float);

    void SetIsCloaked(bool);
	void SetIsCountermeasureActive(bool);
	void SetIsIncapacitated(bool);
	void SetIsOrganic(bool);
	void SetIsInPVP(bool);
	void SetIsRescueBeaconActive(bool);
	void SetCombatLevel(u32);

    void SetGlobalWarpState(u32);

    void SetEngineThrustState(u32);
	void SetEngineTrailType(u32);

    void SetInterruptibleAbilityName(char *);
	void SetInterruptState(u32);
	void SetInterruptibleActivationTime(u32);
	void SetInterruptProgress(float);
	void SetFactionIdentifier(char *);

private:
    void BuildCreatePacket(unsigned char *, long &);
	void BuildClickPacket(unsigned char *, long &);
	bool BuildDiffPacket(unsigned char *, long &);

private:
	u32 GameID;

	_MobIndex Data;

	unsigned char Flags[8];
	unsigned char ExtendedFlags[15];

	unsigned char m_PacketBuffer[2000];

    Mutex m_BaseMutex;
	Mutex m_BufferMutex;

public:
    unsigned char * m_CreatePacket;
    unsigned char * m_ClickPacket;
    unsigned char * m_DiffPacket;

	long m_CreateSize;
	long m_ClickSize;
	long m_DiffSize;

	class AuxPercent Shield;
	class AuxQuadrantDamage QuadrantDamage;
	class AuxDamage DamageSpot;
	class AuxDamage DamageLine;
	class AuxDamage DamageBlotch;
	class AuxLego Lego;
};

#endif
