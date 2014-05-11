#ifndef _MISSIONPARSER_H
#define _MISSIONPARSER_H

#include "Net7.h"
#include "XmlParser.h"
#include <vector>

#define TRIGGER_CONDITION_TALKTREE					0x0001
#define TRIGGER_CONDITION_LOCATION					0x0002
#define TRIGGER_CONDITION_LEVEL						0x0004
#define TRIGGER_CONDITION_ITEM						0x0008
#define TRIGGER_CONDITION_STAGE_START				0x0010
#define TRIGGER_CONDITION_MISSION_START				0x0020
#define TRIGGER_CONDITION_STAGE_TIMER_EXPIRE		0x0040
#define TRIGGER_CONDITION_MISSION_TIMER_EXPIRE		0x0080

#define TRIGGERACTION_TALKTREE						0x0001
#define TRIGGERACTION_STARTTIMER					0x0002
#define TRIGGERACTION_NEXTSTAGE						0x0004
#define TRIGGERACTION_FAIL							0x0008
#define TRIGGERACTION_EXPIRE						0x0010

#define MAX_NUM_STAGES								20

#include "MissionManager.h"

struct SectorData;

class MissionParser :
	protected  XmlParser
{
public:
	MissionParser(void);
	~MissionParser(void);

public:
    /*
	struct MissionReward
	{
		int CreditReward;
		int ExploreReward;
		int CombatReward;
		int TradeReward;
		int FactionType;
		int FactionReward;
	} ATTRIB_PACKED;

	struct TriggerConditionTalkTree
	{
		int SectorID;
		int StarbaseID;
		int SelectionID;
	} ATTRIB_PACKED;

	struct TriggerActionTalkTree
	{
		int MessageLength;
		char *Message;
	} ATTRIB_PACKED;

	struct TriggerConditionLocation
	{
		int SectorID;
		float x,y,z;
		float range;
	} ATTRIB_PACKED;

	struct TriggerConditionLevel
	{
		int maxExploreLevel;
		int maxCombatLevel;
		int maxTradeLevel;
		int maxOverallLevel;

		int minExploreLevel;
		int minCombatLevel;
		int minTradeLevel;
		int minOverallLevel;
	} ATTRIB_PACKED;

	struct TriggerConditionItem
	{
		int ItemID;
		int count;
		double quality;
	} ATTRIB_PACKED;

	struct MissionConditions
	{
		int conditionType;
		TriggerConditionTalkTree talkTree;
		TriggerConditionLocation location;
		TriggerConditionLevel level;
		TriggerConditionItem item;
	} ATTRIB_PACKED;

	struct TriggerAction
	{
		int actionType;
		TriggerActionTalkTree talkTree;
	} ATTRIB_PACKED;

	struct MissionTriggerList
	{
		TriggerAction actions;
		MissionConditions conditions;
		MissionReward Rewards;
		MissionTriggerList *next;
	} ATTRIB_PACKED;

	struct MissionStageDetails
	{
		long AllowedTime;
		MissionTriggerList *triggers;
	} ATTRIB_PACKED;

	struct MissionDetails
	{
		Mission auxMissionDetails;
		long	AllowedTime;
		struct	MissionStageDetails Stages[MAX_NUM_STAGES];
		MissionReward ForfeitConsequences;
	} ATTRIB_PACKED;

	struct MissionList 
	{
		MissionDetails mission;
		MissionList *next;
	} ATTRIB_PACKED;*/

public:

	// Load missions from XML
	bool LoadMissions();
    bool Initialise();
    void SetSectorData(SectorData *data);
    Mission *GetMission(long mission_id);
    MissionList *GetMissionList()           { return &m_MissionList; }

private:
    bool ParseMissions(char *data);

    SectorData *m_SectorData;
    MissionList m_MissionList;
};

#endif
