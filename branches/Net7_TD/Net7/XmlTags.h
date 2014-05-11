// XmlTags.h
//
//    This header file defines constants for the XML parser.
//

#ifndef _XML_TAGS_H_INCLUDED_
#define _XML_TAGS_H_INCLUDED_

#define XML_TAG_SECTOR_CONTENT              "SectorContent"
#define XML_TAG_GALAXY                      "Galaxy"
#define XML_TAG_UNIVERSE                    "Universe"
#define XML_TAG_SYSTEM                      "System"
#define XML_TAG_SECTOR                      "Sector"
#define XML_TAG_SERVER_PARAMETERS           "ServerParameters"
#define XML_TAG_GAME_OBJECT                 "GameObject"
#define XML_TAG_NAME                        "Name"
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
#define XML_TAG_NOTES                       "Notes"


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


// Tag ID
enum eTagId
{
    XML_TAG_ID_INVALID,
    XML_TAG_ID_SECTOR_CONTENT,
    XML_TAG_ID_GALAXY,
    XML_TAG_ID_UNIVERSE,
    XML_TAG_ID_SYSTEM,
    XML_TAG_ID_SERVER_PARAMETERS,
    XML_TAG_ID_SECTOR,
    XML_TAG_ID_GAME_OBJECT,
    XML_TAG_ID_NAME,
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
    XML_TAG_ID_NOTES
};


#endif // _XML_TAGS_H_INCLUDED_

