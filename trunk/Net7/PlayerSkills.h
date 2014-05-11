#ifndef _PLAYER_SKILLS_H_INCLUDED_
#define _PLAYER_SKILLS_H_INCLUDED_

struct SkillTemplate;
struct SkillClasses;

typedef enum
{
    SKILL_ERROR_NONE = 0x00, 
    SKILL_ERROR_TOTALLVL = 0x01, 
    SKILL_ERROR_SKILLPOINTS = 0x02, 
    SKILL_ERROR_NOUPDATE = 0x04,
    SKILL_ERROR_COMBATLVL = 0x08,
    SKILL_ERROR_TRADELVL = 0x10,
    SKILL_ERROR_EXPLORELVL = 0x20,
    SKILL_ERROR_MAXLVL = 0x40,
    SKILL_ERROR_ONLY = 0x80
} SKILL_ERROR;

// This is done to make setting errors easier
typedef enum
{
	SKILL_TOTAL = SKILL_ERROR_TOTALLVL,
    SKILL_COMBAT = SKILL_ERROR_COMBATLVL,
    SKILL_TRADE = SKILL_ERROR_TRADELVL,
    SKILL_EXPLORE = SKILL_ERROR_EXPLORELVL
} SKILL_REQ;

typedef enum
{
	SKILL_PRIMWEP, SKILL_SECWEP, SKILL_PRIMTECH, SKILL_SECTECH, SKILL_OTHER
} SKILL_TYPE;

struct SkillClassData
{
    int Quested;
    int LevelAquired;
	int MaxLevel;
	SKILL_TYPE LevelScheme;
} ATTRIB_PACKED;

struct SkillData
{
    SKILL_REQ RequirementScheme;
    bool Activatable;
	SkillClassData ClassType[9];   
} ATTRIB_PACKED;

#define SKILL_AFTERBURN                 0
#define SKILL_BEAM_WEAPON               1
#define SKILL_BEFRIEND                  2
#define SKILL_BIOREPRESSION             3
#define SKILL_BUILD_COMPONENTS          4
#define SKILL_BUILD_DEVICES             5
#define SKILL_BUILD_ENGINES             6
#define SKILL_BUILD_ITEMS               7
#define SKILL_BUILD_REACTORS            8
#define SKILL_BUILD_SHIELDS             9
#define SKILL_BUILD_WEAPONS             10
#define SKILL_CALL_FORWARD              11
#define SKILL_CLOAK                     12
#define SKILL_COMBAT_TRANCE             13
#define SKILL_COMPULSORY_CONTEMPLATION  14
#define SKILL_CREATE_WORMHOLE           15
#define SKILL_CRITICAL_TARGETING        16
#define SKILL_DAMAGE_CONTROL            17
#define SKILL_DEVICE_TECH               18
#define SKILL_ENERGY_LEECH              19
#define SKILL_ENGINE_TECH               20
#define SKILL_ENGINEERING               21
#define SKILL_ENRAGE                    22
#define SKILL_ENVIRONMENT_SHIELD        23
#define SKILL_FOLD_SPACE                24
#define SKILL_GRAVITY_LINK              25
#define SKILL_HACKING                   26
#define SKILL_HULL_PATCH                27
#define SKILL_ITEM_TECH                 28
#define SKILL_JENQUAI_CULTURE           29
#define SKILL_JENQUAI_LORE              30
#define SKILL_JUMPSTART                 31
#define SKILL_MAELSTROM_RESONANCE       32
#define SKILL_MENACE                    33
#define SKILL_MISSILE_WEAPON            34
#define SKILL_NAVIGATE                  35
#define SKILL_NEGOTIATE                 36
#define SKILL_POWER_DOWN                37
#define SKILL_PROGEN_CULTURE            38
#define SKILL_PROGEN_LORE               39
#define SKILL_PROJECTILE_WEAPON         40
#define SKILL_PROSPECT                  41
#define SKILL_PSIONIC_SHIELD            42
#define SKILL_QUANTUM_FLUX              43
#define SKILL_RALLY                     44
#define SKILL_REACTOR_TECH              45
#define SKILL_RECHARGE_SHIELDS          46
#define SKILL_REPAIR_EQUIPMENT          47
#define SKILL_REPULSOR_FIELD            48
#define SKILL_SCAN                      49
#define SKILL_SELF_DESTRUCT             50
#define SKILL_SHIELD_CHARGING           51
#define SKILL_SHIELD_INVERSION          52
#define SKILL_SHIELD_LEECH              53
#define SKILL_SHIELD_SAP                54
#define SKILL_SHIELD_TECH               55
#define SKILL_SUMMON                    56
#define SKILL_TERRAN_CULTURE            57


#define ADVANCED_CLOAK 0
#define ADVANCED_POWER_DOWN 1
#define ADVANCED_POWER_DOWN_2 2
#define ADVANCED_POWER_DOWN_3 3
#define ADVANCED_POWER_DOWN_4 4
#define ANGER 6
#define ANGER_GROUP 7
#define AREA_EQUIPMENT_REPAIR 8
#define AREA_HULL_REPAIR 9
#define AREA_IMMOBILIZATION_FIELD 10
#define AREA_INTIMIDATE 11
#define AREA_MASS_FIELD 12
#define AREA_MULTI_HACK 13
#define AREA_SHIELD_RECHARGE 14
#define AREA_SOOTHE 15
#define AREA_SYSTEM_HACK 16
#define AREA_TELEPORT 17
#define AREA_TERRIFY 18
#define WORMHOLE_ASTEROID_BELT_BETA 19
#define BEFRIEND 20
#define BIOCESSATION 21
#define BIOREPRESS 22
#define BIOREPRESSION_SPHERE 23
#define BIOSUPPRESS 24
#define BIOSUPPRESSION_SPHERE 25
#define WORMHOLE_CARPENTER 26
#define CAUSE_AGGRESSION 27
#define CLOAK 28
#define COMBAT_CLOAK 29
#define COMBAT_EQUIPMENT_REPAIR 30
#define COMBAT_HULL_REPAIR 31
#define COMBAT_RECHARGE_SHIELDS 32
#define COMPULSORY_CONTEMPLATION 33
#define DAMAGE_TACTICS 34
#define DEFENSE_TACTICS 35
#define DIRECTIONAL_TELEPORT 36
#define WORMHOLE_ENDRIAGO 37
#define ENERGY_DRAIN 38
#define ENERGY_LEECH 39
#define ENERGY_LEECHING_SPHERE 40
#define ENRAGE 41
#define ENRAGE_GROUP 42
#define ENTRANCE 43
#define ENVIRONMENTAL_BARRIER 44
#define ENVIRONMENTAL_SHIELD 45
#define FIRING_TACTICS 46
#define GRAVITY_FIELD 47
#define GREATER_ENVIRONMENTAL_SHIELD 48
#define GREATER_PSIONIC_SHIELD 49
#define GREATER_REPULSOR_FIELD 50
#define GROUP_CLOAK 51
#define GROUP_SAP 54
#define GROUP_SAPPING_SPHERE 55
#define GROUP_STEALTH 56
#define HACK_SYSTEMS 57
#define HACK_WEAPONS 58
#define IMMOBILIZATION_FIELD 59
#define IMPROVED_AREA_HULL_REPAIR 60
#define IMPROVED_AREA_RECHARGE 61
#define IMPROVED_AREA_REPAIR 62
#define IMPROVED_BEFRIEND 63
#define INTIMIDATE 64
#define JUMPSTART 65
#define WORMHOLE_JUPITER 66
#define WORMHOLE_KAILAASA 67
#define LESSER_ENVIRONMENTAL_SHIELD 68
#define LESSER_PSIONIC_SHIELD 69
#define LESSER_REPULSOR_FIELD 70
#define MAJOR_REPULSOR_FIELD 72
#define MASS_FIELD 73
#define MEGACHARGE_SHIELDS 74
#define MINOR_REPULSOR_FIELD 75
#define MULTI_HACK 76
#define PATCH_HULL 77
#define POWER_DOWN 78
#define PSIONIC_BARRIER 79
#define PSIONIC_INVULNERABILITY 80
#define PSIONIC_SHIELD 81
#define RECHARGE_SHIELDS 89
#define REGENERATE_EQUIPMENT 90
#define REGENERATE_SHIELDS 91
#define RENDER_ENERGY 92
#define RENDER_ENERGY_SPHERE 93
#define REPAIR_EQUIPMENT 94
#define REPAIR_HULL 95
#define REPULSOR_FIELD 96
#define RETURN_FRIEND 97
#define SAPPING_SPHERE 98
#define SCARE 99
#define SELF_DESTRUCT_1 100
#define SELF_DESTRUCT_2 101
#define SELF_DESTRUCT_3 102
#define SELF_DESTRUCT_4 103
#define SELF_DESTRUCT_5 104
#define SHIELD_BURN 105

#define SHIELD_DRAIN 106
#define SHIELD_FLARE 107
#define SHIELD_LEECH 108
#define SHIELD_LEECHING_SPHERE 109
#define GROUP_LEECH 52
#define GROUP_LEECHING_SPHERE 53

#define SHIELD_NOVA 110
#define SHIELD_RAM 111
#define SHIELD_SAP 112
#define SHIELD_SPIKE 113
#define SHIELD_TRANSFER 114
#define SOOTHE 115
#define STEALTH_TACTICS 116
#define SUMMON_ENEMY 117
#define SUMMON_ENEMY_GROUP 118
#define SUMMON_FRIEND 119
#define SUMMON_GROUP 120
#define SUPERCHARGE_SHIELDS 121
#define SUPERCHARGE_TARGET 122
#define WORMHOLE_SWOOPING_EAGLE 123
#define TELEPORT_ENEMY 124
#define TELEPORT_FRIEND 125
#define TELEPORT_SELF 126
#define TERRIFY 127
#define ULTRA_ENVIRONMENTAL_SHIELD 128
#define ULTRACHARGE_SHIELDS 129
#define ULTRACHARGE_TARGET 130
#define WORMHOLE_VALKYRIE_TWINS 131


#endif