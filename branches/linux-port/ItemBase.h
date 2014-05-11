// ItemBase.h
#include "Net7.h"

#ifndef _ITEM_BASE_H_INCLUDED_
#define _ITEM_BASE_H_INCLUDED_

/* Enum defining all of the item fields */
/* The last letter corresponds to the data type */
typedef enum
{
    ITEM_FIELD_UNKNOWN0_I,                  // #0
    ITEM_FIELD_AMMO_S,                      // #1
    ITEM_FIELD_AUTOFIRE_I,                  // #2
    ITEM_FIELD_PROFESSION_RESTRICTION_I,    // #3
    ITEM_FIELD_COMBAT_LVL_REQ_I,            // #4
    ITEM_FIELD_WEAPON_DAMAGE_I,             // #5
    ITEM_FIELD_WEAPON_DAMAGE_TYPE_I,        // #6
    ITEM_FIELD_EFFECT_RANGE_I,              // #7
    ITEM_FIELD_EFFECT_RADIUS_I,             // #8
    ITEM_FIELD_ENERGY_USE_F,                // #9
    ITEM_FIELD_ENERGY_DRAIN_F,              // #10
    ITEM_FIELD_SKILL_REQ_S,                 // #11
    ITEM_FIELD_EXPLORE_LVL_REQ_I,           // #12
    ITEM_FIELD_ITEM_TYPE_S,                 // #13
    ITEM_FIELD_OVERALL_LVL_REQ_I,           // #14
    ITEM_FIELD_MISSLE_MANUV_I,              // #15
    ITEM_FIELD_REACTOR_CAP_I,               // #16
    ITEM_FIELD_LORE_RESTRICTION_I,          // #17
    ITEM_FIELD_RACE_RESTRICTION_I,          // #18
    ITEM_FIELD_WEAPON_RANGE_I,              // #19
    ITEM_FIELD_REACTOR_RECHARGE_F,          // #20
    ITEM_FIELD_WEAPON_RELOAD_F,             // #21
    ITEM_FIELD_ROUND_PER_SHOT_I,            // #22
    ITEM_FIELD_SHIELD_USE_F,                // #23
    ITEM_FIELD_SHIELD_CAP_I,                // #24
    ITEM_FIELD_SHIELD_DRAIN_F,              // #25
    ITEM_FIELD_SHIELD_RECHARGE_F,           // #26
    ITEM_FIELD_OTHER_REQ_S,                 // #27
    ITEM_FIELD_OTHER_REQ_LVL_I,             // #28
    ITEM_FIELD_ENGINE_SIGNATURE_I,          // #29
    ITEM_FIELD_UNKNOWN30_I,                 // #30
    ITEM_FIELD_ENGINE_SPEED_I,              // #31
    ITEM_FIELD_TRADE_LVL_REQ_I,             // #32
    ITEM_FIELD_ENGINE_WARP_SPEED_I,         // #33
    ITEM_FIELD_ENGINE_FREEWARP_DRAIN_F,     // #34
    ITEM_FIELD_OVERRIDE_FLAGS_I,            // #35
    ITEM_FIELD_OVERRIDE_SKILL_INCREASE_F,   // #36
    ITEM_FIELD_OVERRIDE_CRIT_INCREASE_F     // #37
} ItemBaseFields;

typedef enum
{
    ITEM_NO_PROFESSION_RESTRICTION,
    ITEM_WARRIOR_RESTRICTED,
    ITEM_TRADER_RESTRICTED,
    ITEM_EXPLORER_ONLY,
    ITEM_EXPLORER_RESTRICTED,
    ITEM_TRADER_ONLY,
    ITEM_WARRIOR_ONLY,
    ITEM_PROFESSION_RESTRICTED
} ProfessionRestriction;

typedef enum
{
    ITEM_NO_RACE_RESTRICTION,
    ITEM_TERRAN_RESTRICTED,
    ITEM_JENQUAI_RESTRICTED,
    ITEM_PROJEN_ONLY,
    ITEM_PROJEN_RESTRICTED,
    ITEM_JENQUAI_ONLY,
    ITEM_TERRAN_ONLY,
    ITEM_RACE_RESTRICTED
} RaceRestriction;

typedef enum
{
    DAMAGE_IMPACT,
    DAMAGE_EXPLOSIVE,
    DAMAGE_PLASMA,
    DAMAGE_ENERGY,
    DAMAGE_EMP,
    DAMAGE_CHEMICAL
} DamageType;

struct ItemInstance
{
    int WeaponDamage;
    int WeaponDamageType;
    int WeaponRange;
    float WeaponReload;
    char * WeaponAmmo;

    int EffectRange;
    int EffectRadius;
    float EnergyUse;
    float EnergyDrain;
    float ShieldUse;
    float ShieldDrain;

    float ReactorCap;
    float ReactorRecharge;

    float ShieldCap;
    float ShieldRecharge;

    int EngineSignature;
    int EngineSpeed;
    int EngineWarpSpeed;
    float EngineFreeWarpDrain;

    //int OverrideFlags;
    //float OverrideSkillIncrease;
    //float OverrideCritIncrease;
} ATTRIB_PACKED;

struct AmmoInstance
{
    int WeaponDamage;
    int WeaponRange;
    int WeaponDamageType;
    int MissleManeuverability;
} ATTRIB_PACKED;

struct ItemRequirements
{
    int RaceRestriction;
    int ProfessionRestriction;
    int LoreRestriction;
    int CombatRequirement;
    int ExploreRequirement;
    int TradeRequirement;
    int OverallRequirement;
} ATTRIB_PACKED;


//------------------------ ItemData Structs

struct ItemField
{
    bool HasData;
    int iData;
    float fData;
    char * sData;
} ATTRIB_PACKED;

struct ItemEffect
{
	char * Name;
	char * Description;
    char * Tooltip;
    int VisualEffect;
	int Flag1;
	int Flag2;
    int DescVarCount;

	char * BuffName;

	// Stats for Varables
	char *	VarStats[3];
	int		VarType[3];
    float	DescVar[3];

	// Stats for Constants
	char *	ConstStats[2];
	int		ConstType[2];
	float	ConstValue[2];
} ATTRIB_PACKED;

struct EffectData
{
    int Count;
    ItemEffect Effects[6];
	// Container Data
	int RechargeTime;
	float Unknown2;
	int EffectRange;
	int Unknown4;
	int EnergyUse;
} ATTRIB_PACKED;

struct ItemBaseData
{
	int ItemTemplateID;
	int Category;
	int SubCategory;
    int ItemType;

    char * Name;
	char * Description;
	char * Manufacturer;

	short GameBaseAsset;
	short IconBaseAsset;
	short TechLevel;
    short MaxStack;

   	int Cost;             //Cost will never be more than 2.1 billion
	int UseEffect;        //Visual effect when the item is activated (not when effects hit)
	int EquipEffect;	  // Vistual effect when an item is equipted
	int Flags;

	int ItemFieldCount;
	ItemField ItemFields[38];

    EffectData ActivatableEffects;
    EffectData EquipableEffects;
} ATTRIB_PACKED;

class Field;

class ItemBase
{
public:
    ItemBase();
    virtual ~ItemBase();

public:
    bool BuildPacket();

public:
    int PacketLength();
    unsigned char * Packet();

    ItemBaseData * Data();
    int ItemTemplateID();
    int Category();
    int SubCategory();
    int ItemType();

    char * Name();
    char * Description();
    char * Manufacturer();

    u16 GameBaseAsset();
    u16 IconBaseAsset();
    u16 TechLevel();
    u16 MaxStack();

	int FieldType(int FieldID);

    int Cost();
    int UseEffect();
    int Flags();
	int EquipEffect();

    int NumItemFields();
    ItemField * Fields(int idx);

    int ActivatableCount();
	int EquipableCount();

    int ReginesInto();
    int Component(int idx);

    void SetData(ItemBaseData *);

    void SetRefinesInto(int val);
    void SetComponent(int idx, int val);

    ItemInstance GetItemInstance(char *);
    AmmoInstance GetAmmoInstance(char *);

    ItemRequirements GetItemRequirements();

private:
    void BuildItemBasePacket();

private:
	ItemBaseData m_Data;

    int m_RefinesInto;
	int m_Component[6];

    int m_PacketLength;
    unsigned char * m_Packet;
};

#endif // _ITEM_BASE_H_INCLUDED_
