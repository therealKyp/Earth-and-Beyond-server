#ifndef _AUXSHIPINDEX_H_INCLUDED_
#define _AUXSHIPINDEX_H_INCLUDED_

#include "AuxPercent.h"
#include "AuxShake.h"
#include "AuxShipInventory.h"
#include "AuxQuadrantDamage.h"
#include "AuxDamage.h"
#include "AuxLego.h"
#include "AuxBuffs.h"
#include "AuxStats.h"

struct _ShipIndex
{
	char Name[64];
	char Owner[64];
	char Title[64];
	char Rank[64];
	_Percent Energy;
	float MaxEnergy;
	_Percent Shield;
	float MaxShield;
	float HullPoints;
	float MaxHullPoints;
	float MaxTiltRate;
	float MaxTurnRate;
	float MaxTiltAngle;
	float MaxSpeed;
	float MinSpeed;
	float Acceleration;
	bool LockSpeed;
	bool LockOrient;
	bool AutoLevel;
	bool IsCloaked;
	bool IsCountermeasureActive;
	bool IsIncapacitated;
	bool IsOrganic;
	bool IsInPVP;
	bool IsAutoFollowing;
	bool IsRescueBeaconActive;
	u32 CombatLevel;
	u32 TargetGameID;
	char TargetThreat[128];
	char TargetThreatSound[128];
	u32 TargetThreatLevel;
	u32 PrivateWarpState;
	u32 GlobalWarpState;
	u32 WarpAvailable;
	u32 WarpTriggerTime;
	_Shake Shake;
	_ShipInv Inventory;
	_QuadrantDamage QuadrantDamage;
	_Damage DamageSpot;
	_Damage DamageLine;
	_Damage DamageBlotch;
	_Lego Lego;
	_Buffs Buffs;
	u64 TradeMoney;
	_Stats BaseStats;
	_Stats CurrentStats;
	u32 EngineThrustState;
	u32 EngineTrailType;
	char GuildName[64];
	u32 GuildRank;
	char GuildRankName[64];
	float SameGuildTagColor[3];
	float OtherGuildTagColor[3];
	char InterruptAbilityName[128];
	u32 InterruptState;
	u32 InterruptActivationTime;
	float InterruptProgress;
	char FactionIdentifier[64];
} ATTRIB_PACKED;

typedef enum
{
    PRIVATE_WARP_AVAILABLE,
    PRIVATE_WARP_INITIATE_WARP,
    PRIVATE_WARP_SHAKE,
    PRIVATE_WARP_END_NORMALLY,
    PRIVATE_WARP_ENGINE_LOCKED,
    PRIVATE_WARP_ENGINE_UNAVAILABLE,
    PRIVATE_WARP_GRAVITY_INTERFERENCE,
    PRIVATE_WARP_TARGET_LOST,
    PRIVATE_WARP_TARGET_REACHED,
    PRIVATE_WARP_FOLLOWERS_DISTRUPTED,
    PRIVATE_WARP_LOW_ENERGY,
    PRIVATE_WARP_OUT_OF_ENERGY
} _PrivateWarpState;

typedef enum
{
    GLOBAL_WARP_NOT_WARPING,
    GLOBAL_WARP_INITIATE_WARP,
    GLOBAL_WARP_ENGINE_TRAIL,
    GLOBAL_WARP_END_WARP
} _GlobalWarpState;

class AuxShipIndex : public AuxBase
{
public:
    AuxShipIndex()
	{
		Construct(Flags, ExtendedFlags, 58, 0, 0);
        m_CreateDiff = m_Buffer;
        m_ClickDiff = 0;
        m_Diff = 0;
        m_Packet = m_Buffer;

        m_CreateDiffLength = 0;
        m_ClickDiffLength = 0;
        m_DiffLength = 0;
        m_PacketLength = 0;

        m_Mutex = &m_WriteMutex;

        Reset();
	}

    ~AuxShipIndex()
	{
	}

    Mutex * Buffer();

    void Reset();
    void ClearFlags();

    bool HasDiff();
    void BuildDiff(); //Assembles the three diff packets

	bool BuildExtendedPacket(); //Builds the extended packet that goes to the player ONLY
	bool BuildExtendedPacket(unsigned char *, long &);

    bool BuildCreateExtendedPacket(); //Builds the extended packet for when the player is "created" for others
	bool BuildCreateExtendedPacket(unsigned char *, long &);

    bool BuildClickExtendedPacket(); //Builds the extended packet for when the player is "clicked" by others
	bool BuildClickExtendedPacket(unsigned char *, long &);

	_ShipIndex * GetData();
	u32 GetGameID();

	char *GetName();
	char *GetOwner();
	char *GetTitle();
	char *GetRank();
	float GetMaxEnergy();
	float GetMaxShield();
	float GetHullPoints();
	float GetMaxHullPoints();
	float GetMaxTiltRate();
	float GetMaxTurnRate();
	float GetMaxTiltAngle();
	float GetMaxSpeed();
	float GetMinSpeed();
	float GetAcceleration();
	bool GetLockSpeed();
	bool GetLockOrient();
	bool GetAutoLevel();
	bool GetIsCloaked();
	bool GetIsCountermeasureActive();
	bool GetIsIncapacitated();
	bool GetIsOrganic();
	bool GetIsInPVP();
	bool GetIsAutoFollowing();
	bool GetIsRescueBeaconActive();
	u32 GetCombatLevel();
	u32 GetTargetGameID();
	char *GetTargetThreat();
	char *GetTargetThreatSound();
	u32 GetTargetThreatLevel();
	u32 GetPrivateWarpState();
	u32 GetGlobalWarpState();
	u32 GetWarpAvailable();
	u32 GetWarpTriggerTime();
	u64 GetTradeMoney();
	u32 GetEngineThrustState();
	u32 GetEngineTrailType();
	char *GetGuildName();
	u32 GetGuildRank();
	char *GetGuildRankName();
	float *GetSameGuildTagColor();
	float *GetOtherGuildTagColor();
	char *GetInterruptAbilityName();
	u32 GetInterruptState();
	u32 GetInterruptActivationTime();
	float GetInterruptProgress();
	char *GetFactionIdentifier();

	void SetData(_ShipIndex *);
	void SetGameID(u32);

	void SetName(char *);
	void SetOwner(char *);
	void SetTitle(char *);
	void SetRank(char *);
	void SetMaxEnergy(float);
	void SetMaxShield(float);
	void SetHullPoints(float);
	void SetMaxHullPoints(float);
	void SetMaxTiltRate(float);
	void SetMaxTurnRate(float);
	void SetMaxTiltAngle(float);
	void SetMaxSpeed(float);
	void SetMinSpeed(float);
	void SetAcceleration(float);
	void SetLockSpeed(bool);
	void SetLockOrient(bool);
	void SetAutoLevel(bool);
	void SetIsCloaked(bool);
	void SetIsCountermeasureActive(bool);
	void SetIsIncapacitated(bool);
	void SetIsOrganic(bool);
	void SetIsInPVP(bool);
	void SetIsAutoFollowing(bool);
	void SetIsRescueBeaconActive(bool);
	void SetCombatLevel(u32);
	void SetTargetGameID(u32);
	void SetTargetThreat(char *);
	void SetTargetThreatSound(char *);
	void SetTargetThreatLevel(u32);
	void SetPrivateWarpState(u32);
	void SetGlobalWarpState(u32);
	void SetWarpAvailable(u32);
	void SetWarpTriggerTime(u32);
	void SetTradeMoney(u64);
	void SetEngineThrustState(u32);
	void SetEngineTrailType(u32);
	void SetGuildName(char *);
	void SetGuildRank(u32);
	void SetGuildRankName(char *);
	void SetSameGuildTagColor(float *);
	void SetOtherGuildTagColor(float *);
	void SetInterruptAbilityName(char *);
	void SetInterruptState(u32);
	void SetInterruptActivationTime(u32);
	void SetInterruptProgress(float);
	void SetFactionIdentifier(char *);

private:

	bool BuildPacket(unsigned char *, long &);
	bool BuildCreatePacket(unsigned char *, long &);
	bool BuildClickPacket(unsigned char *, long &);

private:
	u32 GameID;

	_ShipIndex Data;

	unsigned char Flags[8];
	unsigned char ExtendedFlags[15];

    Mutex m_WriteMutex;
    Mutex m_BufferMutex;

	unsigned char m_Buffer[10000];

public:
    unsigned char * m_CreateDiff;
    unsigned char * m_ClickDiff;
    unsigned char * m_Diff;
    unsigned char * m_Packet;

    long m_CreateDiffLength;
    long m_ClickDiffLength;
    long m_DiffLength;
    long m_PacketLength;

	class AuxPercent Energy;
	class AuxPercent Shield;
	class AuxShake Shake;
	class AuxShipInv Inventory;
	class AuxQuadrantDamage QuadrantDamage;
	class AuxDamage DamageSpot;
	class AuxDamage DamageLine;
	class AuxDamage DamageBlotch;
	class AuxLego Lego;
	class AuxBuffs Buffs;
	class AuxStats BaseStats;
	class AuxStats CurrentStats;
};

#endif
