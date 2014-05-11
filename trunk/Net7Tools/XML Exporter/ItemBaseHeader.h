// ItemBase.h

#ifndef _ITEM_BASE_H_INCLUDED_
#define _ITEM_BASE_H_INCLUDED_

#define ITEM_BASE_MAX_ACTIVATABLE_EFFECTS   6
#define ITEM_BASE_MAX_EQUIPABLE_EFFECTS     6
#define ITEM_BASE_MAX_DESCRIPTION_VARIABLES 10
#define ITEM_BASE_MAX_ITEM_FIELDS           38

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

typedef struct
{
    bool HasData;
    int iData;
    float fData;
    char sData[64];
} ItemField;

typedef struct
{
	int VisualEffect;
	char Name[128];
	char Description[128];
    char Tooltip[512];
	int Flag1;
	int Flag2;
    int DescVarCount;
    float DescVar[ITEM_BASE_MAX_DESCRIPTION_VARIABLES];
} ItemEffect;

typedef struct
{
	int         m_ItemTemplateID;

	int         m_Category;
	int         m_SubCategory;
    int         m_ItemType;

	short	    m_GameBaseAsset;
	short	    m_IconBaseAsset;
	short	    m_TechLevel;
    short       m_MaxStack;
   	int		    m_Cost;
	int		    m_UseEffect;
	int		    m_Flags;

	int         m_NumItemFields;			    // Number of Item fields
	ItemField   m_ItemFields[ITEM_BASE_MAX_ITEM_FIELDS];

    int         m_NumActivatableEffects;        // Number of Activatable Effects
    ItemEffect  m_ActivatableEffects[ITEM_BASE_MAX_ACTIVATABLE_EFFECTS];

    int         m_NumEquipableEffects;          // Number of Equipable Effects
    ItemEffect  m_EquipableEffects[ITEM_BASE_MAX_EQUIPABLE_EFFECTS];

	char        m_Name[64];
	char        m_Description[512];
	char        m_Manufacturer[64];

    int         m_ManuInt;

	int    	    m_Component[6];
    int         m_Refine;
    int         m_Launcher;
    int         m_DefaultAmmo;

    int         m_NoStore;
    int         m_NoDestroy;
    int         m_NoManu;
    int         m_NoTrade;
    int         m_Unique;

    bool        m_FromXML;

} ItemBaseStruct;

#endif
