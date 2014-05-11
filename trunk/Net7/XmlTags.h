// XmlTags.h
//
//    This header file defines constants for the XML parser.
//

#ifndef _XML_TAGS_H_INCLUDED_
#define _XML_TAGS_H_INCLUDED_

// Sector Content tags
#define XML_TAG_SECTOR_CONTENT              "SectorContent"
#define XML_TAG_GALAXY                      "Galaxy"
#define XML_TAG_UNIVERSE                    "Universe"
#define XML_TAG_SYSTEM                      "System"
#define XML_TAG_SECTOR                      "Sector"
#define XML_TAG_SERVER_PARAMETERS           "ServerParameters"
#define XML_TAG_GAME_OBJECT                 "GameObject"
#define XML_TAG_ACTIVATE_RENDER_STATE       "ActivateRenderState"
#define XML_TAG_OBJECT_EFFECT               "ObjectEffect"
#define XML_TAG_RELATIONSHIP                "Relationship"
#define XML_TAG_SIMPLE_POSITIONAL_UPDATE    "SimplePositionalUpdate"
#define XML_TAG_ADVANCED_POSITIONAL_UPDATE  "AdvancedPositionalUpdate"
#define XML_TAG_PLANET_POSITIONAL_UPDATE    "PlanetPositionalUpdate"
#define XML_TAG_CONSTANT_POSITIONAL_UPDATE  "ConstantPositionalUpdate"
#define XML_TAG_COMPONENT_POSITIONAL_UPDATE "ComponentPositionalUpdate"
#define XML_TAG_NAVIGATION                  "Navigation"
#define XML_TAG_DESTINATION                 "Destination"
#define XML_TAG_GREETINGS                   "Greetings"
#define XML_TAG_MOB_ID                      "MOBID"
#define XML_TAG_RESOURCE_ID                 "Resource"
#define XML_TAG_SPAWN_DATA                  "SpawnData"
#define XML_TAG_FIELD_POINT                 "FieldPoint"
#define XML_TAG_SPAWN_POINT                 "SpawnPoint"

// TODO: These are not implemented yet
#define XML_TAG_DOCKING_MSG                 "DockingMsg"
#define XML_TAG_LAUNCH_MSG                  "LaunchMsg"
#define XML_TAG_REPAIR_MSG                  "RepairMsg"
#define XML_TAG_REGISTER_MSG                "RegisterMsg"

// Item Base tags
#define XML_TAG_ITEM_TEMPLATES              "ItemTemplates"
#define XML_TAG_ITEM_BASE                   "ItemBase"
#define XML_TAG_MANUFACTURER                "Manufacturer"
#define XML_TAG_ITEM_FIELDS                 "ItemFields"
#define XML_TAG_ITEM                        "Item"
#define XML_TAG_ACTIVATABLE_EFFECTS         "ActivatableEffects"
#define XML_TAG_EQUIPABLE_EFFECTS           "EquipableEffects"
#define XML_TAG_EFFECT                      "Effect"
#define XML_TAG_COMPONENTS					"Components"
#define XML_TAG_ITEMEFFECTS					"ItemEffects"

// Multi-purpose tags
#define XML_TAG_NAME                        "Name"
#define XML_TAG_DESCRIPTION                 "Description"
#define XML_TAG_TOOLTIP                     "Tooltip"
#define XML_TAG_DESCRIPTION_VARS            "DescriptionVars"
#define XML_TAG_FLOAT                       "Float"
#define XML_TAG_NOTES                       "Notes"


// Sector Content Attributes
#define XML_ATTRIB_ID                       "ID"
#define XML_ATTRIB_GAME_ID                  "GameID"
#define XML_ATTRIB_SCALE                    "Scale"
#define XML_ATTRIB_BASE_ASSET               "BaseAsset"
#define XML_ATTRIB_TYPE                     "Type"
#define XML_ATTRIB_HSV                      "HSV"
#define XML_ATTRIB_RENDER_STATE_ID          "RenderStateID"
#define XML_ATTRIB_BITMASK                  "Bitmask"
#define XML_ATTRIB_EFFECT_DESC_ID           "EffectDescID"
#define XML_ATTRIB_EFFECT_ID                "EffectID"
#define XML_ATTRIB_TIME_STAMP               "TimeStamp"
#define XML_ATTRIB_DURATION                 "Duration"
#define XML_ATTRIB_SCALE                    "Scale"
#define XML_ATTRIB_HSVSHIFT                 "HSVShift"
#define XML_ATTRIB_REACTION                 "Reaction"
#define XML_ATTRIB_IS_ATTACKING             "IsAttacking"
#define XML_ATTRIB_POSITION                 "Position"
#define XML_ATTRIB_ORIENTATION              "Orientation"
#define XML_ATTRIB_VELOCITY                 "Velocity"
#define XML_ATTRIB_MOVEMENT_ID              "MovementID"
#define XML_ATTRIB_CURRENT_SPEED            "CurrentSpeed"
#define XML_ATTRIB_SET_SPEED                "SetSpeed"
#define XML_ATTRIB_ACCELERATION             "Acceleration"
#define XML_ATTRIB_ROT_Y                    "RotY"
#define XML_ATTRIB_DESIRED_Y                "DesiredY"
#define XML_ATTRIB_ROT_Z                    "RotZ"
#define XML_ATTRIB_DESIRED_Z                "DesiredZ"
#define XML_ATTRIB_IMPARTED_VELOCITY        "ImpartedVelocity"
#define XML_ATTRIB_IMPARTED_SPIN            "ImpartedSpin"
#define XML_ATTRIB_IMPARTED_ROLL            "ImpartedRoll"
#define XML_ATTRIB_IMPARTED_PITCH           "ImpartedPitch"
#define XML_ATTRIB_UPDATE_PERIOD            "UpdatePeriod"
#define XML_ATTRIB_ORBIT_ID                 "OrbitID"
#define XML_ATTRIB_ORBIT_DIST               "OrbitDist"
#define XML_ATTRIB_ORBIT_ANGLE              "OrbitAngle"
#define XML_ATTRIB_ORBIT_RATE               "OrbitRate"
#define XML_ATTRIB_ROTATE_ANGLE             "RotateAngle"
#define XML_ATTRIB_ROTATE_RATE              "RotateRate"
#define XML_ATTRIB_TILT_ANGLE               "TiltAngle"
#define XML_ATTRIB_IMPARTED_DECAY           "ImpartedDecay"
#define XML_ATTRIB_TRACTOR_SPEED            "TractorSpeed"
#define XML_ATTRIB_TRACTOR_ID               "TractorID"
#define XML_ATTRIB_TRACTOR_EFFECT_ID        "TractorEffectID"
#define XML_ATTRIB_SIGNATURE                "Signature"
#define XML_ATTRIB_PLAYER_HAS_VISITED       "PlayerHasVisited"
#define XML_ATTRIB_NAV_TYPE                 "NavType"
#define XML_ATTRIB_IS_HUGE                  "IsHuge"
#define XML_ATTRIB_IS_CAP_SHIP              "IsCapShip"
#define XML_ATTRIB_APPEARS_IN_RADAR         "AppearsInRadar"

// Item Base attributes
#define XML_ATTRIB_ITEM_TEMPLATE_ID         "ItemTemplateID"
#define XML_ATTRIB_CATEGORY                 "Category"
#define XML_ATTRIB_SUBCATEGORY              "Subcategory"
#define XML_ATTRIB_ITEM_TYPE                "ItemType"
#define XML_ATTRIB_GAME_BASSET              "GameBasset"
#define XML_ATTRIB_ICON_BASE_ASSET          "IconBaseAsset"
#define XML_ATTRIB_TECH_LEVEL               "TechLevel"
#define XML_ATTRIB_COST                     "Cost"
#define XML_ATTRIB_MAX_STACK                "MaxStack"
#define XML_ATTRIB_EFFECT_USAGE             "EffectUsage"
#define XML_ATTRIB_FLAGS                    "Flags"
#define XML_ATTRIB_FLAG_1                   "flag1"
#define XML_ATTRIB_FLAG_2                   "flag2"
#define XML_ATTRIB_ITEM_FIELD_TYPE          "type"
#define XML_ATTRIB_UNKNOWN_1                "unknown1"
#define XML_ATTRIB_UNKNOWN_2                "unknown2"
#define XML_ATTRIB_UNKNOWN_3                "unknown3"
#define XML_ATTRIB_UNKNOWN_4                "unknown4"
#define XML_ATTRIB_NUMBER					"Number"
//ItemEffect
#define XML_ATTRIB_ACTIVATE					"Activate"
#define XML_ATTRIB_EQUIP					"Equip"
#define XML_ATTRIB_TARGET					"Target"

// Station Tags
#define XML_TAG_STATION						"station"
#define XML_TAG_ROOM						"room"
#define XML_TAG_TERMS						"terms"
#define XML_TAG_FOGFAR						"fogfar"
#define XML_TAG_FOGNEAR						"fognear"
#define XML_TAG_FNAME						"fname"
#define XML_TAG_LNAME						"lname"
#define XML_TAG_AVATAR_TYPE					"avatar_type"
#define XML_TAG_FILTER1						"filter1"
#define XML_TAG_AVATAR_VERSION				"avatar_version"
#define XML_TAG_RACE						"race"
#define XML_TAG_PROFESSION					"profession"
#define XML_TAG_GENDER						"gender"
#define XML_TAG_MOOD_TYPE					"mood_type"
#define XML_TAG_PERSONALITY					"personality"
#define XML_TAG_NLP							"nlp"
#define XML_TAG_BODY_TYPE					"body_type"
#define XML_TAG_PANTS_TYPE					"pants_type"
#define XML_TAG_HEAD_TYPE					"head_type"
#define XML_TAG_HAIR_NUM					"hair_num"
#define XML_TAG_EAR_NUM						"ear_num"
#define XML_TAG_GOGGLE_NUM					"goggle_num"
#define XML_TAG_BEARD_NUM					"beard_num"
#define XML_TAG_WEAPON_HIP_NUM				"weapon_hip_num"
#define XML_TAG_WEAPON_UNIQUE_NUM			"weapon_unique_num"
#define XML_TAG_WEAPON_BLACK_NUM			"weapon_black_num"
#define XML_TAG_HEAD_TEXTURE_NUM			"head_texture_num"
#define XML_TAG_TATTOO_TEXTURE_NUM			"tattoo_texture_num"
#define XML_TAG_TATTOO_OFFSET				"tattoo_offest"
#define XML_TAG_HAIR_COLOR					"hair_color"
#define XML_TAG_BEARD_COLOR					"head_color"
#define XML_TAG_EYE_COLOR					"eye_color"
#define XML_TAG_SKIN_COLOR					"skin_color"
#define XML_TAG_SHIRT_PRIMARY_COLOR			"shirt_primary_color"
#define XML_TAG_SHIRT_SECONDARY_COLOR		"shirt_secondary_color"
#define XML_TAG_PANTS_PRIMARY_COLOR			"pants_primary_color"	
#define XML_TAG_PANTS_SECONDARY_COLOR		"pants_secondary_color"		
#define XML_TAG_SHIRT_PRIMARY_METAL			"shirt_primary_metal"	
#define XML_TAG_SHIRT_SECONDARY_METAL		"shirt_secondary_metal"
#define XML_TAG_PANTS_PRIMARY_METAL			"pants_primary_metal"	
#define XML_TAG_PANTS_SECONDARY_METAL		"pants_secondary_metal"
#define XML_TAG_FILTER2						"filter2"
#define XML_TAG_BODYWEIGHT					"bodyweight"	
#define XML_TAG_HEADWEIGHT					"headweight"

// Station attributes
#define XML_ATTRIB_STARBASEID				"starbaseid"
#define XML_ATTRIB_BOOTH					"booth"
#define XML_ATTRIB_LOCATION					"location"
#define XML_ATTRIB_ROOM						"room"
#define XML_ATTRIB_X						"x"
#define XML_ATTRIB_Y						"y"
#define XML_ATTRIB_ZOOM						"zoom"
#define XML_ATTRIB_R						"r"
#define XML_ATTRIB_G						"g"
#define XML_ATTRIB_B						"b"

// Skill Tags
#define XML_TAG_SKILL			            "Skill"
#define XML_TAG_WARRIOR	                    "Warrior"
#define XML_TAG_SENTINEL                    "Sentinel"
#define XML_TAG_PRIVATEER                   "Privateer"
#define XML_TAG_DEFNDER                     "Defender"
#define XML_TAG_EXPLORER		            "Explorer"
#define XML_TAG_SEEKER			            "Seeker"
#define XML_TAG_ENFORCER                    "Enforcer"
#define XML_TAG_SCOUT                       "Scout"
#define XML_TAG_TRADESMAN                   "Tradesman"

// Skill attributes
#define XML_ATTRIB_CATEGORY	                "Category"
#define XML_ATTRIB_TYPE						"Type"
#define XML_ATTRIB_LEARN_LVL				"LearnLvl"
#define XML_ATTRIB_LVL_TYPE	                "LvlType"
#define XML_ATTRIB_QUEST	                "Quest"
#define XML_ATTRIB_MAX		                "Max"

// Mission tags
//#define XML_ATTRIB_ID						"ID"
#define XML_TAG_MISSIONS					"Missions"
#define XML_TAG_MISSION						"Mission"
#define XML_TAG_NAME						"Name"
#define XML_TAG_SUMMARY						"Summary"
#define XML_TAG_STAGES						"Stages"
#define XML_TAG_STAGE						"Stage"
#define XML_TAG_DESCRIPTION					"Description"
#define XML_TAG_REWARDS						"Rewards"
#define XML_TAG_FACTION						"Faction"
#define XML_TAG_CREDITS						"Credits"
#define XML_TAG_EXPERIENCE					"Experience"
#define XML_TAG_FAILURE						"Failure"
#define XML_TAG_TRIGGER						"Trigger"
#define XML_TAG_CONDITION					"Condition"
#define XML_TAG_ACTION						"Action"
#define XML_TAG_BUFF_NAME					"BuffName"
#define XML_TAG_BUFF_ASSET					"BuffAsset"
#define XML_TAG_BUFF_VALUE					"BuffValue"
#define XML_TAG_TALKTREE                    "Tree"
#define XML_TAG_NEXT_STAGE                  "NextStage"
#define XML_TAG_OBJECT                      "Object"
#define XML_TAG_TEXT                        "Text"
#define XML_TAG_ITEM                        "Item"
#define XML_TAG_NPC                         "NPC"
#define XML_TAG_LOCATION                    "Location"
#define XML_TAG_HULLUPGRADE                 "HullUpgrade"
#define XML_TAG_BRANCH                      "Branch"

// Mission attributes
#define XML_ATTRIB_NAME						"Name"
#define XML_ATTRIB_FORFEITABLE				"forfeitable"
#define XML_ATTRIB_TIME						"time"
#define XML_ATTRIB_SECTORID					"SectorID"
#define XML_ATTRIB_TREENODEID               "Node"


//#define XML_ATTRIB_STARBASEID				"StarbaseID"
#define XML_ATTRIB_SELECTIONID				"SelectionID"
#define XML_ATTRIB_POSITION_X				"PositionX"
#define XML_ATTRIB_POSITION_Y				"PositionY"
#define XML_ATTRIB_POSITION_Z				"PositionZ"
#define XML_ATTRIB_RANGE					"Range"
#define XML_ATTRIB_EXPLORE					"Explore"
#define XML_ATTRIB_COMBAT					"Combat"
#define XML_ATTRIB_TRADE					"Trade"
#define XML_ATTRIB_OVERALL					"Overall"



// Tag ID
enum eTagId
{
    XML_TAG_ID_INVALID,
    // Sector Content XML tags
    XML_TAG_ID_SECTOR_CONTENT,
    XML_TAG_ID_GALAXY,
    XML_TAG_ID_UNIVERSE,
    XML_TAG_ID_SYSTEM,
    XML_TAG_ID_SERVER_PARAMETERS,
    XML_TAG_ID_SECTOR,
    XML_TAG_ID_GAME_OBJECT,
    XML_TAG_ID_ACTIVATE_RENDER_STATE,
    XML_TAG_ID_OBJECT_EFFECT,
    XML_TAG_ID_RELATIONSHIP,
    XML_TAG_ID_SIMPLE_POSITIONAL_UPDATE,
    XML_TAG_ID_ADVANCED_POSITIONAL_UPDATE,
    XML_TAG_ID_PLANET_POSITIONAL_UPDATE,
    XML_TAG_ID_CONSTANT_POSITIONAL_UPDATE,
    XML_TAG_ID_COMPONENT_POSITIONAL_UPDATE,
    XML_TAG_ID_NAVIGATION,
    XML_TAG_ID_DESTINATION,
    XML_TAG_ID_GREETINGS,
    XML_TAG_ID_FIELD,
    XML_TAG_ID_SPAWN_POINT,
    XML_TAG_ID_SPAWN_DATA,
    XML_TAG_ID_MOB_ID,
    XML_TAG_ID_RESOURCE_ID,
    // Item Base XML tags
    XML_TAG_ID_ITEM_TEMPLATES,
    XML_TAG_ID_ITEM_BASE,
    XML_TAG_ID_MANUFACTURER,
    XML_TAG_ID_ITEM_FIELDS,
    XML_TAG_ID_ITEM,
    XML_TAG_ID_ACTIVATABLE_EFFECTS,
    XML_TAG_ID_EQUIPABLE_EFFECTS,
    XML_TAG_ID_EFFECT,
	XML_TAG_ID_COMPONENTS,
	XML_TAG_ID_ITEMEFFECTS,
    // Multi-purpose XML tags
    XML_TAG_ID_NAME,
    XML_TAG_ID_DESCRIPTION,
    XML_TAG_ID_TOOLTIP,
    XML_TAG_ID_DESCRIPTION_VARS,
    XML_TAG_ID_FLOAT,
    XML_TAG_ID_NOTES,
	// Skill XML Tags
	XML_TAG_ID_SKILL,
	XML_TAG_ID_WARRIOR,
	XML_TAG_ID_SENTINEL,
	XML_TAG_ID_PRIVATEER,
	XML_TAG_ID_DEFNDER,
	XML_TAG_ID_EXPLORER,
	XML_TAG_ID_SEEKER,
	XML_TAG_ID_ENFORCER,
	XML_TAG_ID_SCOUT,
	XML_TAG_ID_TRADESMAN,
	XML_TAG_ID_SKILL_LVL,
	XML_TAG_ID_TOTAL_LVL,
	XML_TAG_ID_COMBAT_LVL,
	XML_TAG_ID_TRADE_LVL,
	XML_TAG_ID_EXPLORE_LVL,
	XML_TAG_ID_EFFECT_ID,
	XML_TAG_ID_GROUP_EFFECT,
	XML_TAG_ID_WEAR_OFF_EFFECT,
	XML_TAG_ID_DURATION,
	XML_TAG_ID_RANAGE,
	XML_TAG_ID_ABILITY,
	XML_TAG_ID_BUFF_NAME,
	XML_TAG_ID_BUFF_ASSET,
	XML_TAG_ID_BUFF_VALUE,
	XML_TAG_ID_TARGET,
	// Mission XML tags
	XML_TAG_ID_MISSIONS,
    XML_TAG_ID_MISSION,
    //XML_TAG_ID_NAME,
	XML_TAG_ID_SUMMARY,
    XML_TAG_ID_STAGES,
    XML_TAG_ID_STAGE,
    //XML_TAG_ID_DESCRIPTION,
    XML_TAG_ID_REWARDS,
    XML_TAG_ID_FACTION,
    XML_TAG_ID_CREDITS,
    XML_TAG_ID_EXPERIENCE,
    XML_TAG_ID_FAILURE,
    XML_TAG_ID_TRIGGER,
	// Mission Trigger XML Tags
	XML_TAG_ID_CONDITION,
	XML_TAG_ID_ACTION,
    XML_TAG_ID_TALKTREE,
    XML_TAG_ID_BRANCH,
    XML_TAG_ID_NEXT_STAGE,
    XML_TAG_ID_OBJECT,
    XML_TAG_ID_TEXT,
    XML_TAG_ID_NPC,
    XML_TAG_ID_LOCATION ,
    XML_TAG_ID_HULLUPGRADE
};


#endif // _XML_TAGS_H_INCLUDED_

