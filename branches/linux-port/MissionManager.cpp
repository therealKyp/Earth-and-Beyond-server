#include "Net7.h"
#include "XmlTags.h"
#include "MissionManager.h"
#include "MissionParser.h"
#include "ItemList.h"
#include "Opcodes.h"
#include "SectorData.h"
#include "ObjectManager.h"
#include "SectorContentParser.h"
#include "ServerManager.h"

Mission::Mission()
{
    m_MissionNodes.clear();
    m_MissionName = 0;
    m_MissionSummary = 0;
    m_MissionNodeCount = 0;
    m_LevelRequirement = 0;
}

void Mission::SetMissionName(char *name)
{
    if (name)
    {
        int length = strlen(name);
        m_MissionName = new char [length + 1];

        strcpy(m_MissionName, name);
    }
}

void Mission::SetMissionSummary(char *summary)
{
    if (summary)
    {
        int length = strlen(summary);
        m_MissionSummary = new char [length + 1];

        strcpy(m_MissionSummary, summary);
    }
}

void Mission::AddNewNode(MissionNode *node)
{
    if (node)
    {
        m_MissionNodes.push_back(node);
        m_MissionNodeCount++;
    }
}

char * Mission::GetNodeDescription(u32 node)
{
    if (node < m_MissionNodes.size())
    {
        return (m_MissionNodes[node]->description);
    }
    else
    {
        return (0);
    }
}

node_type Mission::GetNodeType(u32 node)
{
    if (node < m_MissionNodes.size())
    {
        return (m_MissionNodes[node]->type);
    }
    else
    {
        return NULL_NODE;
    }
}

char * Mission::GetNPCName(u32 node)
{
    if (node < m_MissionNodes.size())
    {
        return (m_MissionNodes[node]->npc_name);
    }
    else
    {
        return (0);
    }
}

long Mission::GetNodeSectorID(u32 node)
{
    if (node < m_MissionNodes.size())
    {
        return (m_MissionNodes[node]->sector_id);
    }
    else
    {
        return (0);
    }
}

char * Mission::GetNodeLocation(u32 node)
{
    if (node < m_MissionNodes.size())
    {
        return (m_MissionNodes[node]->location);
    }
    else
    {
        return (0);
    }
}

Object * Mission::GetONodeLocation(u32 node)
{
    if (node < m_MissionNodes.size())
    {
        return (m_MissionNodes[node]->obj_location);
    }
    else
    {
        return (0);
    }
}

long Mission::GetObjectIndex(u32 node, Object *obj)
{
    long index = 0;
    ObjectList::iterator itrOList;

    if (node < m_MissionNodes.size())
    {
        for (itrOList = m_MissionNodes[node]->object_list->begin(); itrOList < m_MissionNodes[node]->object_list->end(); ++itrOList) 
        {
            if ((*itrOList) == obj)
            {
                return index;
            } 
            index++;
        }
    }
        
    return (-1);
}

long Mission::GetObjectNameIndex(u32 node, char *name, long complete)
{
    long index = 0;
    ObjNameList::iterator itrOList;

    if (node < m_MissionNodes.size())
    {
        for (itrOList = m_MissionNodes[node]->object_name_list->begin(); itrOList < m_MissionNodes[node]->object_name_list->end(); ++itrOList) 
        {
            if ((*itrOList) == name && !(complete & (1 << index)))
            {
                return index;
            } 
            index++;
        }
    }
        
    return (-1);
}

char * Mission::GetNodeObject(u32 node)
{
    if (node < m_MissionNodes.size())
    {
        return (m_MissionNodes[node]->object_name);
    }
    else
    {
        return (0);
    }
}

long Mission::GetNodeItem(u32 node)
{
    if (node < m_MissionNodes.size())
    {
        return (m_MissionNodes[node]->item_id);
    }
    else
    {
        return (0);
    }
}

short Mission::GetNodeItemQuantity(u32 node)
{
    if (node < m_MissionNodes.size())
    {
        return (m_MissionNodes[node]->item_quantity);
    }
    else
    {
        return (0);
    }
}

char * Mission::GetNodeTalkTree(u32 node, u32 tree_id, short *length, short *next_stage)
{
    if (node < m_MissionNodes.size() && 
        m_MissionNodes[node]->talk_tree_list && 
        tree_id < m_MissionNodes[node]->talk_tree_list->size() &&
        tree_id != 0)
    {
        TalkTreeList talk_list = *m_MissionNodes[node]->talk_tree_list;
        TalkTreeNode *tree_node = talk_list[tree_id];
        //if the text hasn't been built yet, build it now.
        BuildTalkTreeNode(tree_node);

        *length = tree_node->talk_text_len;
        *next_stage = tree_node->tree_dest;
        return (tree_node->talk_text);
    }
    else
    {
        return (0);
    }
}

void Mission::BuildTalkTreeNode(TalkTreeNode *node)
{
    BranchTextList::iterator itrBranchList;
    BranchDesc *desc;
    u16 length = node->talk_text_len;

    if (node->built_text)
    {
        return;
    }

    //here we construct the string to drive the E&B client's conversation tree system
    //it's only done the first time a particular tree is requested

    //find total length
    if (node->branch_desc != (0))
    {
        short branch_count = 0;
        long string_index;
        for (itrBranchList = node->branch_desc->begin(); itrBranchList < node->branch_desc->end(); ++itrBranchList) 
        {
            u16 len;
            desc = (*itrBranchList);
            len = strlen(desc->branch_text);
            len += 5;
            branch_count++;
            length += len;
        }

        //now we have the new string length.
        char *pString = new char [length];
        memset(pString, 0, length);
        memcpy(pString, node->talk_text, node->talk_text_len);
        pString[node->talk_text_len-1] = (char)branch_count; //set branch count
        string_index = node->talk_text_len;

        //now add in each branch
        for (itrBranchList = node->branch_desc->begin(); itrBranchList < node->branch_desc->end(); ++itrBranchList) 
        {
            long len;
            desc = (*itrBranchList);
            len = strlen(desc->branch_text);
            pString[string_index] = (char)desc->branch_dest; //set branch destination
            string_index += 4;
            strcpy( (pString + string_index), desc->branch_text);
            string_index = string_index + len + 1;
        }

        delete node->talk_text;
        node->talk_text = pString;
    }

    node->built_text = true;
    node->talk_text_len = length;
}

short Mission::GetNPCID(u32 node)
{
    if (node < m_MissionNodes.size())
    {
        return (m_MissionNodes[node]->npc_id);
    }
    else
    {
        return (0);
    }
}

bool Mission::GetNodeReward(u32 node)
{
    if (node < m_MissionNodes.size() &&
        m_MissionNodes[node]->reward != 0)
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

void Mission::GetNodeXPReward(u32 node, short *combat, short *explore, short *trade)
{
    if (node < m_MissionNodes.size() &&
        m_MissionNodes[node]->reward != 0)
    {
        *combat = m_MissionNodes[node]->reward->CombatReward;
        *explore = m_MissionNodes[node]->reward->ExploreReward;
        *trade = m_MissionNodes[node]->reward->TradeReward;
    }
    return;
}

long Mission::GetNodeCreditReward(u32 node)
{
    if (node < m_MissionNodes.size() &&
        m_MissionNodes[node]->reward != 0)
    {
        return m_MissionNodes[node]->reward->CreditReward;
    }
    else
    {
        return 0;
    }
}

void Mission::GetNodeFactionReward(u32 node, short *faction, short *faction_reward)
{
    if (node < m_MissionNodes.size() &&
        m_MissionNodes[node]->reward != 0)
    {
        *faction = m_MissionNodes[node]->reward->FactionType;
        *faction_reward = m_MissionNodes[node]->reward->FactionReward;
    }
    return;
}

long Mission::GetNodeItemReward(u32 node)
{
    if (node < m_MissionNodes.size() &&
        m_MissionNodes[node]->reward != 0)
    {
        return m_MissionNodes[node]->reward->ItemRewardID;
    }
    else
    {
        return 0;
    }
}

long Mission::GetNodeHullUpgradeReward(u32 node)
{
    if (node < m_MissionNodes.size() &&
        m_MissionNodes[node]->reward != 0)
    {
        return m_MissionNodes[node]->reward->HullUpgrade;
    }
    else
    {
        return 0;
    }
}

char * Mission::GetNodeRewardDescription(u32 node)
{
    if (node < m_MissionNodes.size() &&
        m_MissionNodes[node]->reward != 0)
    {
        return m_MissionNodes[node]->reward->description;
    }
    else
    {
        return 0;
    }
}

long Mission::GetNodeObjectMask(u32 node)
{
    if (node < m_MissionNodes.size())
    {
        long mask = (1 << m_MissionNodes[node]->object_name_list->size()) - 1;
        return mask;
    }
    else
    {
        return 0;
    }
}

bool Mission::InitialiseNodes()
{
    ObjNameList::iterator itrOList;
    MissionNodeList::iterator itrNodeList;
    MissionNode *node;
    Object *obj;

    //spin through each mission element
    for (itrNodeList = m_MissionNodes.begin(); itrNodeList < m_MissionNodes.end(); ++itrNodeList) 
    {
        node = (*itrNodeList);
        
        if (node)
        {
            //match location to object.
            ObjectManager *node_manager = GetObjectManager(node->sector_id);
            
            //match object text to objects.
            if (node_manager)
            {                
                if (node->location)
                {
                    obj = node_manager->GetObjectFromName(node->location);
                    if (obj)
                    {
                        node->obj_location = obj;
                    }
                    else
                    {
                        //report error in mission
                        LogMessage("No Object in sector %d found to match '%s'\n", node->sector_id, node->location);
                    }
                }
                
                if (node->object_name_list && node->type != FIGHT_MOB)
                {
                    node->object_list = new ObjectList;
                    for (itrOList = node->object_name_list->begin(); itrOList < node->object_name_list->end(); ++itrOList) 
                    {
                        obj = node_manager->GetObjectFromName(*itrOList);
                        
                        if (obj)
                        {
                            node->object_list->push_back(obj);
                        }
                        else
                        {
                            //report error in mission
                            LogMessage("No Object in sector %d found to match '%s'\n", node->sector_id, (*itrOList));
                        }                        
                    }
                }
            }
        }
    }

    return true;
}

ObjectManager *Mission::GetObjectManager(long sector_id)
{
    //SectorData * p = data;
    ObjectManager *manager = (0);
	SectorData *p = g_ServerMgr->m_SectorContent.GetSectorData(sector_id);
	if (p)
	{
		manager = p->obj_manager;
	}

    return manager;
}

#if 0
MissionManager::MissionManager(void)
{
	m_missionList = NULL;
}

MissionManager::~MissionManager(void)
{
}

bool MissionManager::Initialize(void)
{
	if (m_missionList != NULL)
	{
		m_MissionParser.refreshMissionList();
	}
	else
	{
		if (!m_MissionParser.LoadMissions())
		{
			return false;
		}
	}
	m_missionList = m_MissionParser.GetMissionList();
	return true;
}

MissionParser::MissionDetails *MissionManager::getMission(int id)
{
	MissionParser::MissionList *p = m_missionList;
	while (p != NULL)
	{
		if (p->mission.auxMissionDetails.ID == id)
		{
			return &p->mission;
		}
		p = p->next;
	}
	return NULL;
}

bool MissionManager::givePlayerMission(Connection *c, int missionID)
{
	int slot;

	MissionParser::MissionDetails *missionTemplate = NULL;

	for (slot=0; slot<12; slot++)
	{
		if (c->m_AuxPlayer.m_Missions.Missions[slot].ID == 0)
		{
			break;
		}
	}
	if (slot == 12)
	{
		LogMessage("No more mission slots available");
		return false;
	}
	
	missionTemplate = getMission(missionID);
	if (missionTemplate != NULL)
	{
		printf("Creating mission id %d in mission slot %d\n", missionID, slot);

		//memcpy(&c->m_AuxPlayer.m_Missions.Missions[slot], &missionTemplate->auxMissionDetails, sizeof(Mission));

		// TODO: When timing is in place, this will need to be updated so the current time / expiration time are set
		c->m_AuxPlayer.m_Missions.Flags[(slot + 4) / 8] |= 1<<((slot + 4) % 8);
		c->m_AuxPlayer.SetMission(&missionTemplate->auxMissionDetails, slot);
		c->m_SectorMgr->AddTimedCall(c->m_Player, B_MISSION_CHECKLOCATION, 5000, NULL, c->m_AuxShip.m_GameID);
		return true;
	}
	LogMessage("AddMission: Could not find mission ID %d\n", missionID);
	return false;
}
bool MissionManager::clearMission(Connection *c, int missionID)
{
	bool foundMission = false;
	int index, i;
	for (i=0; i < 12 && c->m_AuxPlayer.m_Missions.Missions[i].ID > 0; i++)
	{
		if (c->m_AuxPlayer.m_Missions.Missions[i].ID == missionID &&
				(c->m_AuxPlayer.m_Missions.Missions[i].IsCompleted ||
				 c->m_AuxPlayer.m_Missions.Missions[i].IsFailed ||
				 c->m_AuxPlayer.m_Missions.Missions[i].IsExpired) &&
				 !foundMission)
		{
			foundMission = true;
			index = i;

			printf("Clearing mission id %d in mission i %d\n", missionID, i);
		}
		else if (foundMission)
		{
			// TODO : Send this in one packet, instead of multiple packets.
			// Also figure out how to prevent the client from announcing
			// new missions for each moved mission
			c->m_AuxPlayer.m_Missions.Missions[i].HasGivenNewMissionMessage = true;
			c->m_AuxPlayer.SetMission(&c->m_AuxPlayer.m_Missions.Missions[i], i-1);
		}
	}
	if (foundMission)
	{
		c->m_AuxPlayer.m_Missions.Missions[i-1].Flags[0] = (char)0x22;
		c->m_AuxPlayer.m_Missions.Missions[i-1].Flags[1] = (char)0x00;	// Send failed
		c->m_AuxPlayer.m_Missions.Missions[i-1].Flags[2] = (char)0x00;
		c->m_AuxPlayer.m_Missions.Missions[i-1].Name[0] = '\0';			// 0-length name = clear
		c->m_AuxPlayer.SetMission(&c->m_AuxPlayer.m_Missions.Missions[i-1], i-1);

		char mask = ~(1<<((i + 3) % 8));
		c->m_AuxPlayer.m_Missions.Flags[(i + 3) / 8] &= mask;
		c->m_AuxPlayer.m_Missions.Missions[i-1].ID = 0;
	}
	return foundMission;
}
bool MissionManager::forfeitMission(Connection *c, int missionID)
{
	char savedFlags[3];
	MissionParser::MissionDetails *details = getMission(missionID);

	if (details == NULL)
	{
		return false;
	}

	for (int i=0; i < 12; i++)
	{
		if (c->m_AuxPlayer.m_Missions.Missions[i].ID == missionID)
		{
			if (!details->auxMissionDetails.IsForfeitable)
			{
				return false;
			}

			c->m_AuxPlayer.m_Missions.Missions[i].IsFailed = true;
			
			memcpy(savedFlags, c->m_AuxPlayer.m_Missions.Missions[i].Flags, sizeof(savedFlags));
			c->m_AuxPlayer.m_Missions.Missions[i].Flags[0] = (char)0x02;
			c->m_AuxPlayer.m_Missions.Missions[i].Flags[1] = (char)0x80;	// Send failed
			c->m_AuxPlayer.m_Missions.Missions[i].Flags[2] = (char)0x00;
			c->m_AuxPlayer.SetMission(&c->m_AuxPlayer.m_Missions.Missions[i], i);
			memcpy(c->m_AuxPlayer.m_Missions.Missions[i].Flags, savedFlags, sizeof(c->m_AuxPlayer.m_Missions.Missions[i].Flags));
			printf("Forfeiting mission id %d in mission slot %d\n", missionID, i);
			
			c->m_AuxPlayer.AddCredits(details->ForfeitConsequences.CreditReward);
			c->AddCombatXP("Mission:", details->ForfeitConsequences.CombatReward);
			c->AddExploreXP("Mission:", details->ForfeitConsequences.ExploreReward);
			c->AddTradeXP("Mission:", details->ForfeitConsequences.TradeReward);


			return true;
		}
	}
	return false;
}

bool MissionManager::EvaluateConditions(Connection *c, MissionParser::MissionTriggerList *trigger)
{
	int overallLevel = c->m_AuxPlayer.m_RPGInfo.ExploreLevel + c->m_AuxPlayer.m_RPGInfo.CombatLevel + c->m_AuxPlayer.m_RPGInfo.TradeLevel;
	if ((trigger->conditions.conditionType & TRIGGER_CONDITION_LEVEL) != 0 )
	{
		// Make sure their level is in range for this trigger
		if (c->m_AuxPlayer.m_RPGInfo.ExploreLevel < trigger->conditions.level.minExploreLevel ||
			c->m_AuxPlayer.m_RPGInfo.CombatLevel < trigger->conditions.level.minCombatLevel ||
			c->m_AuxPlayer.m_RPGInfo.TradeLevel < trigger->conditions.level.minTradeLevel ||
			overallLevel < trigger->conditions.level.minOverallLevel ||
			c->m_AuxPlayer.m_RPGInfo.ExploreLevel > trigger->conditions.level.maxExploreLevel ||
			c->m_AuxPlayer.m_RPGInfo.CombatLevel > trigger->conditions.level.maxCombatLevel ||
			c->m_AuxPlayer.m_RPGInfo.TradeLevel > trigger->conditions.level.maxTradeLevel ||
			overallLevel > trigger->conditions.level.maxOverallLevel
			)
		{
			return false;
		}
	}
	return true;
}

bool MissionManager::EvaluateActions(Connection *c, MissionParser::MissionTriggerList *trigger, int missionNumber, Mission &playerMission, MissionParser::MissionDetails *details)
{
	char savedFlags[3];
	char savedFlags2[3];

	// This method is ugly and needs to be split up.  I'll do that later.  -Unimatrix

	doMissionRewards(c, trigger);

	if ((trigger->actions.actionType & TRIGGERACTION_TALKTREE) != 0)
	{
		c->m_IsInMissionConversation = true;
        c->SendResponse(ENB_OPCODE_0054_TALK_TREE, (unsigned char *) trigger->actions.talkTree.Message, trigger->actions.talkTree.MessageLength-1);
	}
	if ((trigger->actions.actionType & TRIGGERACTION_NEXTSTAGE) != 0)
	{
		if (playerMission.Stage == playerMission.StageCount)
		{
			playerMission.IsCompleted = true;

			memcpy(savedFlags, playerMission.Flags, sizeof(savedFlags));
			playerMission.Flags[0] = (char)0x02;
			playerMission.Flags[1] = (char)0x40;	// Send completed
			playerMission.Flags[2] = (char)0x00;
			c->m_AuxPlayer.SetMission(&playerMission, missionNumber);
			memcpy(playerMission.Flags, savedFlags, sizeof(playerMission.Flags));
		}
		else
		{
			playerMission.Stage++;

			memcpy(savedFlags, playerMission.Flags, sizeof(savedFlags));
			playerMission.Flags[0] = (char)0x02;
			playerMission.Flags[1] = (char)0x00;	// Send completed
			playerMission.Flags[2] = (char)0x18;

			int flagIndex = (playerMission.Stage + 3) / 8;
			int shift = (playerMission.Stage + 3) % 8;

			memcpy(savedFlags2, playerMission.StageFlags, sizeof(savedFlags2));
			playerMission.StageFlags[0] = 0x02;
			playerMission.StageFlags[flagIndex] |= (1<<shift);

			c->m_AuxPlayer.SetMission(&playerMission, missionNumber);
			memcpy(playerMission.StageFlags, savedFlags2, sizeof(playerMission.StageFlags));
			memcpy(playerMission.Flags, savedFlags, sizeof(playerMission.Flags));
			// Keep the stage for when they logout / login
			playerMission.StageFlags[flagIndex] |= (1<<shift);
		}
    }
	if ((trigger->actions.actionType & TRIGGERACTION_FAIL) != 0)
	{
		playerMission.IsFailed = true;
			
		memcpy(savedFlags, playerMission.Flags, sizeof(savedFlags));
		playerMission.Flags[0] = (char)0x02;
		playerMission.Flags[1] = (char)0x80;	// Send failed
		playerMission.Flags[2] = (char)0x00;
		c->m_AuxPlayer.SetMission(&playerMission, missionNumber);
		memcpy(playerMission.Flags, savedFlags, sizeof(playerMission.Flags));
    }
	if ((trigger->actions.actionType & TRIGGERACTION_EXPIRE) != 0)
	{
		playerMission.IsExpired = true;
			
		memcpy(savedFlags, playerMission.Flags, sizeof(savedFlags));
		playerMission.Flags[0] = (char)0x02;
		playerMission.Flags[1] = (char)0x00;	// Send expired
		playerMission.Flags[2] = (char)0x01;
		c->m_AuxPlayer.SetMission(&playerMission, missionNumber);
		memcpy(playerMission.Flags, savedFlags, sizeof(playerMission.Flags));
    }
	return true;
}

bool MissionManager::interceptTalkTree(Connection *c)
{
	int x;
	MissionParser::MissionDetails *details;

	for (x=0; x<12; x++)
	{
		if (c->m_AuxPlayer.m_Missions.Missions[x].ID > 0 &&
			!c->m_AuxPlayer.m_Missions.Missions[x].IsCompleted &&
			!c->m_AuxPlayer.m_Missions.Missions[x].IsFailed &&
			!c->m_AuxPlayer.m_Missions.Missions[x].IsExpired)
		{
			details = getMission(c->m_AuxPlayer.m_Missions.Missions[x].ID);
			if (details == NULL)
			{
				LogMessage("InterceptTalkTree: Could not find MissionID %d\n", c->m_AuxPlayer.m_Missions.Missions[x].ID);
				return false;
			}
			
			MissionParser::MissionTriggerList *trigger = details->Stages[c->m_AuxPlayer.m_Missions.Missions[x].Stage-1].triggers;
			while (trigger != NULL)
			{
				if ((trigger->conditions.conditionType & TRIGGER_CONDITION_TALKTREE) != 0 &&
					(trigger->actions.actionType & TRIGGERACTION_TALKTREE) != 0 &&
					trigger->conditions.talkTree.SelectionID == -1 &&
					c->m_SectorID == trigger->conditions.talkTree.SectorID &&
					c->m_StarbaseTargetID == trigger->conditions.talkTree.StarbaseID)
				{
					if (EvaluateConditions(c, trigger))
					{
						EvaluateActions(c, trigger, x, c->m_AuxPlayer.m_Missions.Missions[x], details);
						return true;
					}
				}
				trigger = trigger->next;
			}
		}
	}
	return false;
}

bool MissionManager::interceptTalkTreeAction(Connection *c, int Selection)
{
	int x;
	MissionParser::MissionDetails *details;

	if (c->m_IsInMissionConversation &&
		(Selection == 0 || Selection == 230))
	{
		c->m_IsInMissionConversation = false;
		c->SendTalkTreeAction(-32);
		return true;
	}
	else if (!c->m_IsInMissionConversation)
	{
		return false;
	}

	for (x=0; x<12; x++)
	{
		// Check for active mission slot
		if (c->m_AuxPlayer.m_Missions.Missions[x].ID > 0 &&
			!c->m_AuxPlayer.m_Missions.Missions[x].IsCompleted &&
			!c->m_AuxPlayer.m_Missions.Missions[x].IsFailed &&
			!c->m_AuxPlayer.m_Missions.Missions[x].IsExpired )
		{
			details = getMission(c->m_AuxPlayer.m_Missions.Missions[x].ID);
			if (details == NULL)
			{
				LogMessage("InterceptTalkTree: Could not find MissionID %d\n", c->m_AuxPlayer.m_Missions.Missions[x].ID);
				return false;
			}
			MissionParser::MissionTriggerList *trigger = details->Stages[c->m_AuxPlayer.m_Missions.Missions[x].Stage-1].triggers;
			while (trigger != NULL)
			{
				if ((trigger->conditions.conditionType & TRIGGER_CONDITION_TALKTREE) != 0 &&
					(trigger->actions.actionType & TRIGGERACTION_TALKTREE) != 0 &&
					c->m_SectorID == trigger->conditions.talkTree.SectorID)
				{
					if (EvaluateConditions(c, trigger))
					{
						if (trigger->conditions.talkTree.SelectionID != -1 &&
							trigger->conditions.talkTree.SelectionID == Selection)
						{
							EvaluateActions(c, trigger, x, c->m_AuxPlayer.m_Missions.Missions[x], details);
							return true;
						}
					}
				}
				trigger = trigger->next;
			}
		}
	}
	return false;
}

bool MissionManager::checkPlayerLocation(Connection *c, float position[3])
{
	bool playerHasLocationCondition = false;
	for (int x=0; x<12 && c->m_AuxPlayer.m_Missions.Missions[x].ID > 0; x++)
	{
		if (!c->m_AuxPlayer.m_Missions.Missions[x].IsCompleted &&
			!c->m_AuxPlayer.m_Missions.Missions[x].IsFailed &&
			!c->m_AuxPlayer.m_Missions.Missions[x].IsExpired &&
			!c->m_IsInMissionConversation)
		{
			MissionParser::MissionDetails *details = getMission(c->m_AuxPlayer.m_Missions.Missions[x].ID);
			if (details != NULL)
			{
				MissionParser::MissionTriggerList *trigger = details->Stages[c->m_AuxPlayer.m_Missions.Missions[x].Stage-1].triggers;
				while (trigger != NULL)
				{
					if ((trigger->conditions.conditionType & TRIGGER_CONDITION_LOCATION) != 0 &&
						trigger->conditions.location.SectorID == c->m_SectorID)
					{
						playerHasLocationCondition = true;

						// x^2 + y^2 + z^2 = r^2
						float range = trigger->conditions.location.range * trigger->conditions.location.range;
						float pos = (trigger->conditions.location.x - position[0]) * (trigger->conditions.location.x - position[0]) +
										(trigger->conditions.location.y - position[1]) * (trigger->conditions.location.y - position[1]) +
										(trigger->conditions.location.z - position[2]) * (trigger->conditions.location.z - position[2]);
						if (pos < range &&	EvaluateConditions(c, trigger))
						{
							EvaluateActions(c, trigger, x, c->m_AuxPlayer.m_Missions.Missions[x], details);
						}
					}
					trigger = trigger->next;
				}
			}
		}
	}
	return playerHasLocationCondition;
}

bool MissionManager::doMissionRewards(Connection *c, MissionParser::MissionTriggerList *trigger)
{
	if ( trigger->Rewards.CreditReward != 0 )
	{
		c->m_AuxPlayer.AddCredits(trigger->Rewards.CreditReward);
	}

	if ( trigger->Rewards.CombatReward != 0 )
	{
		c->AddCombatXP("Mission:", trigger->Rewards.CombatReward);
	}

	if ( trigger->Rewards.ExploreReward != 0 )
	{
		c->AddExploreXP("Mission:", trigger->Rewards.ExploreReward);
	}

	if ( trigger->Rewards.TradeReward != 0 )
	{
		c->AddTradeXP("Mission:", trigger->Rewards.TradeReward);
	}
	// TODO: Faction, items
	return true;
}
#endif