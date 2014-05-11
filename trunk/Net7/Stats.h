
#pragma warning(disable:4786)

#ifndef _STATS_H_INCLUDED_
#define _STATS_H_INCLUDED_

#include "ItemBase.h"
#include <map>
#include <vector>

using namespace std;

typedef enum
{
    STAT_BASE_VALUE,
    STAT_BUFF_VALUE,
	STAT_BUFF_MULT,
    STAT_DEBUFF_VALUE,
    STAT_DEBUFF_MULT
} StatBase;

// TODO: Add more
// Shield Stuff
#define STAT_SHIELD				"STAT_SHIELD"
#define STAT_SHIELD_RECHARGE	"STAT_SHIELD_RECHARGE"
// Reactor Stuff
#define STAT_ENERGY				"STAT_ENERGY"
#define STAT_ENERGY_RECHARGE	"STAT_ENERGY_RECHARGE"
// TURBOs
#define STAT_WEAPON_TURBO		"STAT_WEAPON_TURBO"
#define STAT_BEAM_TURBO			"STAT_BEAM_TURBO"
#define STAT_PROJECTILE_TURBO	"STAT_PROJECTILE_TURBO"
#define STAT_MISSILE_TURBO		"STAT_MISSILE_TURBO"
// Engine Stuff
#define STAT_TURN				"STAT_TURN"
#define STAT_IMPULSE			"STAT_IMPULSE"
#define STAT_WARP				"STAT_WARP"
#define STAT_ACCELERATION		"STAT_ACCELERATION"
// Weapon Damages
#define STAT_BEAM_DAMAGE		"STAT_BEAM_DAMAGE"
#define STAT_PROJECTILES_DAMAGE "STAT_PROJECTILES_DAMAGE"
#define STAT_MISSILE_DAMAGE		"STAT_MISSILE_DAMAGE"
// Weapon Ranges
#define STAT_MISSILE_RANGE		"STAT_MISSILE_RANGE"
#define STAT_PROJECTILES_RANGE	"STAT_PROJECTILES_RANGE"
#define STAT_BEAM_RANGE			"STAT_BEAM_RANGE"
// Weapon Accuracies
#define STAT_MISSILE_ACCURACY		"STAT_MISSILE_ACCURACY"
#define STAT_PROJECTILES_ACCURACY	"STAT_PROJECTILES_ACCURACY"
#define STAT_BEAM_ACCURACY			"STAT_BEAM_ACCURACY"
// Other Stats
#define STAT_SIGNATURE			"STAT_SIGNATURE"
#define STAT_SCAN_RANGE			"STAT_SCAN_RANGE"
#define STAT_TRACTORBEAM		"STAT_TRACTORBEAM"
#define STAT_WARP_RECOVERY		"STAT_WARP_RECOVERY"
#define STAT_WARP_ENERGY		"STAT_WARP_ENERGY"
#define STAT_DAMAGE_CONTROL		"STAT_DAMAGE_CONTROL"
#define STAT_CRITICAL_RATE		"STAT_CRITICAL_RATE"
// Deflects	
#define STAT_CHEM_DEFLECT		"STAT_CHEM_DEFLECT"
#define STAT_IMPACT_DEFLECT		"STAT_IMPACT_DEFLECT"
#define STAT_ENERGY_DEFLECT		"STAT_ENERGY_DEFLECT"
#define STAT_PLASMA_DEFLECT		"STAT_PLASMA_DEFLECT"
#define	STAT_PSIONIC_DEFLECT	"STAT_PSIONIC_DEFLECT"
#define STAT_EXPLOSIVE_DEFLECT	"STAT_EXPLOSIVE_DEFLECT"
#define STAT_EMP_DEFLECT		"STAT_EMP_DEFLECT"
// Skills
#define STAT_SKILL_NAVIGATE				"SKILL_NAVIGATE"
#define STAT_SKILL_SCAN					"SKILL_SCAN"
#define STAT_SKILL_MISSILE_WEAPON		"SKILL_MISSILE_WEAPON"
#define STAT_SKILL_PROJECTILE_WEAPON	"SKILL_PROJECTILE_WEAPON"
#define STAT_SKILL_BEAM_WEAPON			"SKILL_BEAM_WEAPON"
#define STAT_SKILL_CLOAK				"SKILL_CLOAK"
#define STAT_SKILL_CRITICAL_TARGETING	"SKILL_CRITICAL_TARGETING"
#define STAT_SKILL_DAMAGE_CONTROL		"SKILL_DAMAGE_CONTROL"
#define STAT_SKILL_NEGOTIATE			"SKILL_NEGOTIATE"
#define STAT_SKILL_COMBAT_TRANCE		"SKILL_COMBAT_TRANCE"
#define STAT_SKILL_SHIELD_RECHARGE		"SKILL_SHIELD_RECHARGE"
#define STAT_SKILL_HULL_PATCH			"SKILL_HULL_PATCH"
#define STAT_SKILL_PROSPECT				"SKILL_PROSPECT"
//Abilities
#define STAT_CLOAK						"STAT_CLOAK"
#define STAT_SHIELD_RECHARGING			"STAT_SHIELD_RECHARGING"
#define STAT_SHIELD_RECHARGING_RANGE	"STAT_SHIELD_RECHARGING_RANGE"
#define STAT_SHIELD_RECHARGING_ECOST	"STAT_SHIELD_RECHARGING_ECOST"
#define STAT_HULL_PATCH					"STAT_HULL_PATCH"
#define STAT_HULL_PATCH_RANGE			"STAT_HULL_PATCH_RANGE"
#define STAT_HULL_PATCH_ECOST			"STAT_HULL_PATCH_ECOST"

// Stores the values for each added Stat
typedef map<int, float> mapStatValue;

// Lists all the values for each Stat
struct MapValue
{
	mapStatValue Values;
	float MaxValue;
} ATTRIB_PACKED;

typedef map<char *, MapValue> mapBuffStats;
// Stores each buff with its max value for the stat
struct MapBuff
{
	mapBuffStats Buff;
	float Total;
} ATTRIB_PACKED;

struct MapType
{
	MapBuff Types[5];
	float Total;
} ATTRIB_PACKED;

typedef map<char *, MapType> mapStatValues;

typedef void(__stdcall *StatCallBack)(char *);


struct StatLookup
{
	char * StatName;
	int Type;
	char * BuffName;
} ATTRIB_PACKED;

// used in loops
typedef map<int, StatLookup> mapValueIdLookup;
typedef map<char *, StatCallBack> mapStatFuncLookup;



class Object;
class Player;

class Stats
{
public:
    Stats();
    ~Stats();

    void Init(Player *);

	// CallBacks
	//bool	StatCallBack(StatCallBack func, char *Stat);

	// Stats
	bool	ChangeStat( int StatID, float NewValue );
	int		SetStat( int BaseType, char * Stat, float Data, char * BuffName = "BASE_VALUE");
	bool	DelStat( int BuffID );
	float	GetStat( char * Stat );
	float	GetStatType( char *Stat, int Type );
	void	UpdateAux(char *StatName);
	void	ResetStats();				// Reset all stats
	void	ResetStat( char * Stat );

private:
	float	CalculateStat( char * Stat );
	float	FindMaxBuff( char * Stat, char *Buff, int Type);
	float	CalculateStatType( char * Stat, int Type );
	void	CalculateValue(char * Stat, int Type, char *Buff);

private:
    Player * m_Player;
	StatBase m_StatType;

	int m_ValueID;
	mapValueIdLookup	m_ValueLookup;
	mapStatValues		m_StatsValues;
	mapStatFuncLookup	m_CallBackFunc;	// Save Functions to call when a stat is updated

	// Holds the stats for each player
	float m_Value[200];					// Saves the full calculation
	float m_BaseValue[200];				// Saves base stats (from items)
	float m_BuffValue[200];				// Saves any buffs (this includes actives and equips)
	float m_BuffMult[200];
	float m_DeBuffValue[200];			// Any debuffs to a player is stored here
	float m_DeBuffMult[200];
};

#endif