#include <float.h>
#include "PlayerClass.h"
#include "ServerManager.h"
#include "ObjectManager.h"
#include "MissionManager.h"
#include "Opcodes.h"

bool Player::CheckStageCompletionNodes(long mission, long stage, long npc_id, Object *obj, long param_data, completion_node_type completion_type)
{
	#define MISSION_BIT_ALREADY_DONE(count) ((completionCount = UpdateMissionData(completionIndex, count, mission_data)) == 0) //NB this changes the mission data
	#define OPTIONALLY_UPDATE_MISSION_BIT (completionCount = UpdateMissionData(completionIndex, cNode->count, mission_data))
	bool conditions_met = true;
	bool mission_stage_data_usage = false;
	long mission_data = -1; // Special flag to signify Stage 0
	long mission_id;
	long mission_slot;
	int completionIndex = 0;
	int completionCount;
	long completion_mask = 0;
	AuxMission	*am = 0;

	CompletionList::iterator itrCList;

	if (stage == 0)
	{
		mission_id = mission; //no slot yet, 'mission' is the mission database ID
		mission_slot = -1;
	}
	else
	{
		mission_slot = mission; //mission is 'slot'
		am = &m_PlayerIndex.Missions.Mission[mission_slot];
		mission_id = am->GetDatabaseID();
		mission_data = am->GetMissionData();
	}

	//get node list
	MissionTree *mTree = g_ServerMgr->m_Missions.GetMissionTree(mission_id);

	if (!mTree) return false;

	//now get completion list for the stage
	CompletionList *cList = &mTree->Nodes[stage]->completion_list;

	if (cList)
	{
		for (itrCList = cList->begin(); itrCList != cList->end(); ++itrCList)
		{
			CompletionNode *cNode = (*itrCList);

			// Check the "status" cases
			switch (cNode->type)
			{
			case GIVE_CREDITS_NPC:
				if (PlayerIndex()->GetCredits() < cNode->data)
					return false;
				OPTIONALLY_UPDATE_MISSION_BIT;
				break;
			case GIVE_ITEM_NPC:
				if (CargoItemCount(cNode->data) < cNode->count)
					return false;
				OPTIONALLY_UPDATE_MISSION_BIT;
				break;
			//case TAKE_ITEM_TO_LOCATION:
			case POSSESS_ITEM:
				if (CargoItemCount(cNode->data) < cNode->count)
					return false;
				OPTIONALLY_UPDATE_MISSION_BIT;
				break;
			case NEAREST_NAV:
				{
					Object *nearest_nav = GetNearestNav();
					if (nearest_nav && nearest_nav->GetDatabaseUID() != cNode->data)
						return false;
				}
				OPTIONALLY_UPDATE_MISSION_BIT;
				break;
			case RECEIVE_ITEM_NPC:
				if (GetCargoSlotFromItemID(0, -1) == -1)
					return false;
				OPTIONALLY_UPDATE_MISSION_BIT;
				break;
			case SECTOR:
				if (PlayerIndex()->GetSectorNum() != cNode->data)
					return false;
				OPTIONALLY_UPDATE_MISSION_BIT;
				break;
			}


			if(completion_type == cNode->type)
			{
				//CheckNodeStatus(cNode, am, param_data, completionIndex,
				// Check the "trigger" cases
				// These should be mutually exclusive within the Mission Editor
				// and appear at the end of the list; if they are valid then we
				// can send a notification to the player.  They can be used with
				// some of the "status" cases.
				switch (cNode->type)
				{
				case ARRIVE_AT:
					// Did we arrive at the correct nav?
					if (GetNearestNav() && GetNearestNav()->GetDatabaseUID() != cNode->data
						 || MISSION_BIT_ALREADY_DONE(cNode->count))
						return false;
					break;
				case FIGHT_MOB:
					// Did we kill the correct mob and did we need to?
					if(param_data != cNode->data || MISSION_BIT_ALREADY_DONE(cNode->count))
						return false;
					if(completionCount != -1)
					{
						SendVaMessage("Killed %d of %d for %s.", completionCount, (int) cNode->count, am->GetName());
						LogMessage("Required MOB killed %d of %d by %s for mission (%d) %s\n",
								   completionCount,
								   (int) cNode->count,
								   Name(),
								   am->GetDatabaseID(),
								   am->GetName());
					}
					break;
				case OBTAIN_ITEMS:
				//case OBTAIN_ITEMS_AT_LOCATION:
					// Did we loot the correct item?
					if (cNode->data != param_data || MISSION_BIT_ALREADY_DONE(cNode->count))
						return false;
					break;
				case TALK_NPC:
					// Are we talking to the right NPC?
					if (npc_id != cNode->data /*|| MISSION_BIT_ALREADY_DONE(cNode->count)*/) //don't check mission data when talking to NPC's
						return false;
					OPTIONALLY_UPDATE_MISSION_BIT;
					break;
				case USE_SKILL_ON_MOB_TYPE:
					// Did we use the correct skill on the correct mob?
					if(npc_id != cNode->data // Correct mob targetted?
						|| param_data != cNode->count // Correct skill?
						|| MISSION_BIT_ALREADY_DONE(1))
						return false;
					completionIndex -= cNode->count - 1; // This ensures that this case only increments the completion index by one
					break;
				case USE_SKILL_ON_OBJECT:
					// Did we use the correct skill on the correct object?
					if(obj && cNode->data != obj->GetDatabaseUID() // Correct object targetted?
						|| param_data != cNode->count // Correct skill?
						|| MISSION_BIT_ALREADY_DONE(1))
						return false;
					completionIndex -= cNode->count - 1; // This ensures that this case only increments the completion index by one
					break;
				}
			}

			completionIndex += cNode->count;
		} // for each CompletionNode



		// Check whether every completion has been performed i.e. is the stage completed?
		if(stage != 0)
		{
			if (completionIndex > 31)
			{
				// TODO: Should be validated when loading the mission
				LogMessage("Mission stage overflow in mission %s ID #%d, stage %d\n",
							mTree->name, mTree->MissionID, stage);
				return false;
			}

			// Update the mission data that may have been modified above
			am->SetMissionData(mission_data);

			//form a test mask
			long test_mask = 0;
			for (int i=0; i < completionIndex; i++)
			{
				test_mask |= 1 << i;
			}

			//now check if all the indicies in this range are set
			if ( (test_mask & mission_data) != test_mask )
			{
				conditions_met = false;
			}
		}
	}

	return conditions_met;
}

bool Player::CheckMissionStarted(long mission_id)
{
	AuxMission * am;
    for (int i = 0; i < MAX_MISSIONS; i++)
    {
        am = &m_PlayerIndex.Missions.Mission[i];
        if (   am->GetDatabaseID() == mission_id
			&& !am->GetIsCompleted()
			&& am->GetStageNum() > 0)
        {
			return true;
		}
	}
	return false;
}
long Player::GetSlotForMission(long mission_id)
{
    long mission_slot = -1;
	int i;
    for (i = 0; i < MAX_MISSIONS; i++)
    {
        if (m_PlayerIndex.Missions.Mission[i].GetStageNum() == 0)
        {
            mission_slot = i;
            break;
        }
    }

    if (mission_slot != -1 && CheckMissionCompleted(mission_id))
    {
		mission_slot = -1;
    }
    else
    {
        //LogMessage("Too many missions already for %s\n", Name());
    }

    if (mission_slot != -1)
    {
        //do we already have this mission?
        for (i = 0; i < MAX_MISSIONS; i++)
        {
            AuxMission * m = &m_PlayerIndex.Missions.Mission[i];
            if (m->GetDatabaseID() == mission_id)
            {
                mission_slot = -1; //player already has this mission active
				LogMessage("Cannot start mission %d, already active.\n", mission_id);
				SendTalkTreeAction(-32); //close display
                break;
            }
        }
    }

    return mission_slot;
}


long Player::AssignMission(long mission_id)
{
    //mission valid?
    if (!g_ServerMgr->m_Missions.GetMissionCount())
    {
        LogMessage("Mission out of range: %d\n",mission_id);
        return (0);
    }

    long mission_slot = GetSlotForMission(mission_id);

    if (mission_slot != -1)
    {
        m_Mutex.Lock();
		MissionTree *mission = g_ServerMgr->m_Missions.GetMissionTree(mission_id);

        AuxMission * m = &m_PlayerIndex.Missions.Mission[mission_slot];
        m->Clear();
        m->SetDatabaseID(mission_id);
		m->SetName(mission->name);
        m->SetStageNum(1);
		m->SetStageCount(mission->NumNodes);
		m->SetSummary(mission->summary);

		m->Stages.Stage[0].SetText(mission->Nodes[1]->description); //load the first stage description

        SendAuxPlayer();

        m_Mutex.Unlock();
        SaveData();
    }

    return mission_slot;
}

bool Player::CheckForNewMissions(long obj_id, long param_1, short npc_id)
{
    long mission_slot;
    _MissionList::iterator itrMList;
    _MissionList *m_list = g_ServerMgr->m_Missions.GetMissionList();
	MissionTree *mTree;
	bool criteria_met;
	RestrictionList::iterator itrRList;
	bool race_pass;
	bool profession_pass;
	bool race_restriction;
	bool profession_restriction;

    //first check if we meet any mission criteria
    for (itrMList = m_list->begin(); itrMList != m_list->end(); ++itrMList)
	{
		mTree = (*itrMList).second;
		criteria_met = true;
		race_pass = false;
		profession_pass = false;
		race_restriction = false;
		profession_restriction = false;

		if (mTree == (0)) break;

		//first see if we're already doing this mission
		if(CheckMissionStarted(mTree->MissionID)) criteria_met = false;

		//second see if we've already done this mission
		if (CheckMissionCompleted(mTree->MissionID)) criteria_met = false;

		//third check each restriction
		for (itrRList = mTree->restriction_list.begin(); itrRList != mTree->restriction_list.end(); ++itrRList)
		{
			RestrictionNode *rNode = (*itrRList);

			switch (rNode->type)
			{
			case OVERALL_LEVEL:
				if (TotalLevel() < (long)rNode->data) criteria_met = false;
				break;
			case COMBAT_LEVEL:
				if (CombatLevel() < (long)rNode->data) criteria_met = false;
				break;
			case EXPLORE_LEVEL:
				if (ExploreLevel() < (long)rNode->data) criteria_met = false;
				break;
			case TRADE_LEVEL:
				if (TradeLevel() < (long)rNode->data) criteria_met = false;
				break;
			case RACE:
				if (Race() == rNode->data) race_pass = true;
				race_restriction = true;
				break;
			case PROFESSION:
				if (Profession() == rNode->data) profession_pass = true;
				profession_restriction = true;
				break;
			case HULL_LEVEL:
				if (PlayerIndex()->RPGInfo.GetHullUpgradeLevel() != rNode->data) criteria_met = false;
				break;
			case FACTION_REQUIRED:
				if (0) criteria_met = false; //TODO: handle faction
				break;
			case ITEM_REQUIRED:
				if (CargoItemCount(rNode->flags) < (long)rNode->data) criteria_met = false;
				break;
			case MISSION_REQUIRED:
				if (!CheckMissionCompleted(rNode->data)) criteria_met = false;
				break;

			default:
				LogMessage("Error, unsupported type in mission '%s'\n", mTree->name);
				SendVaMessage("Error in mission requirements for mission [%d] '%s': please report to devs", mTree->MissionID, mTree->name);
				break;

			}

			if (criteria_met == false) break;
		}

		if (race_restriction && !race_pass) criteria_met = false;
		if (profession_restriction && !profession_pass) criteria_met = false;

		if (criteria_met)
		{
			//check stage 0 requirements
			mission_slot = GetSlotForMission(mTree->MissionID);
			if (mission_slot != -1 && CheckStageCompletionNodes(mTree->MissionID, 0, npc_id
				, 0, 0, TALK_NPC))
			{
				//we can now launch the starter talk tree
				m_MissionAcceptance = true;
				ProposeMissionTree(mTree->MissionID, param_1);
				return true;
			}
		}
	}

    return false;
}

bool Player::CheckMissions(long object_uid, long param_1, short npc_id, completion_node_type completion_type)
{
    bool mission_action = false;
    bool valid_mission = false;
	int i;
	Object *obj = g_SectorObjects[object_uid];

    //are we currently in a mission acceptance tree?
    if (m_MissionAcceptance)
    {
        return CheckForNewMissions(object_uid, param_1, npc_id);
    }

    //OK we have valid mission/s
    //check to see if we meet the criteria of the current stage
	for (i = 0; i < MAX_MISSIONS; ++i)
	{
        AuxMission * am = &m_PlayerIndex.Missions.Mission[i];
        if (!am->GetIsCompleted() && am->GetStageNum() > 0 && am->GetDatabaseID() > -1)
        {
			//check stage completion
			if (CheckStageCompletionNodes(i, am->GetStageNum(), npc_id, obj, param_1, completion_type))
			{
				mission_action = true;
				//is there a talk tree to kick off?
				if (!NPCTalkTree(i, param_1))
				{
					//ok this is a (at the moment) linear advancement mission
					//TODO: make either-or objectives, and mission stage gotos
					if(am->GetStageNum() + 1 >= am->GetStageCount())
					{
						LogMessage("Error, mission [%u] '%s' attempted to go to non-existing stage (%u)\n", am->GetDatabaseID(), am->GetName(), am->GetStageNum() + 1);
						SendVaMessage("Error in mission [%u] '%s': please report to devs", am->GetDatabaseID(), am->GetName());
					}
					else
					{
						AdvanceMission(i, am->GetStageNum()+1);
					}
				}
			}
		}

        if (mission_action == true)
        {
            break;
        }
	}

    return mission_action;
}

/* Each completion is processed serially, with the first completion starting at offset
   0 and using "length" bits.  If any bits within that range are at 0 (i.e. not yet
   done) then that index is returned.
   Return values:
     -1. This is stage 0 so it's ok to continue but do not update mission data nor notify the player
      0. There are no bits "not yet done" so this action wasn't needed
	  ?. Any other positive value means that there were bits "not yet done" and the actual value contains
	     the 1-base bit index that will be updated i.e. 1 for first, 2 for second, etc. */
int Player::UpdateMissionData(int offset, int length, long &missionData)
{
	if(missionData == -1)
	{
		// This happens during stage 0
		return -1;
	}
	for(int mission_flag = offset; mission_flag < offset + length; mission_flag++)
	{
		if ( ((1 << mission_flag) & missionData) == 0 )
		{
			// Found a bit we needed to do
			missionData = (1 << mission_flag) | missionData;
			//am->SetMissionData(missionData);
			return mission_flag - offset + 1;
		}
	}
	return 0;
}

void Player::CheckMissionMOBKill(long MOB_id)
{
	CheckMissions(0, MOB_id, 0, FIGHT_MOB);
}

void Player::CheckMissionSkillUse(long base_skill, long level)
{
	ObjectManager *om = GetObjectManager();
    Object *obj = (0);
	if (om) obj = om->GetObjectFromID(ShipIndex()->GetTargetGameID());

	if (obj)
	{
		switch(obj->ObjectType())
		{
		case OT_MOB:
			CheckMissions(0, base_skill, ((MOB*)obj)->GetMOBType(), USE_SKILL_ON_MOB_TYPE);
			break;
		case OT_STATION:
		case OT_STARGATE:
		case OT_RESOURCE:
		case OT_HULK:
		case OT_NAV:
		case OT_PLANET:
		case OT_HUSK:
			CheckMissions(obj->GetDatabaseUID(), base_skill, 0, USE_SKILL_ON_OBJECT);
			break;
		}
	}
}
void Player::CheckMissionArrivedAt()
{
	Object *obj = GetNearestNav();

	if (obj)
	{
		// Arriving means actually stopping and not simply going through; the ship must be stopped
		if(this->ObjectIsMoving()) // is ship moving?
			return;

		// Check whether the object is close enough to matter
		// TODO: Tweak this value to provide an optimal early-out
		float range = obj->RangeFrom(Position());
		if(range > 25000.0f)
			return;
		// A better range range would be to consiger the object's signature and the ship's scan range
		//if (RangeFrom(p->Position()) < (Signature() + p->ShipIndex()->CurrentStats.GetScanRange()))

		switch(obj->ObjectType())
		{
        case OT_DECO: // Planet
		case OT_PLANET:
        case OT_STARGATE:
        case OT_STATION: //drop through in case mission target is a station/stargate or planet
        case OT_NAV:
			if(range < 5000.0f)
			{
				CheckMissions(obj->GetDatabaseUID(), 0, 0, ARRIVE_AT);
			}
			break;
		}

#if 0
		#define LogRange(x) LogMessage("Player is %f from %s (%s)\nNearest nav: %s\n", range, x, obj->Name(), GetNearestNav() ? GetNearestNav()->Name() : "");
		switch(obj->ObjectType())
		{
		case OT_NONE:
			case  OT_STATION:
				LogRange("OT_STATION")
				break;
			case  OT_STARGATE:
				LogRange("OT_STARGATE")
				break;
			case  OT_OBJECT:
				LogRange("OT_OBJECT")
				break;
			case  OT_RESOURCE:
				LogRange("OT_RESOURCE")
				break;
			case  OT_PLAYER:
				LogRange("OT_PLAYER")
				break;
			case  OT_HULK:
				LogRange("OT_HULK")
				break;
			case  OT_ABS:
				LogRange("OT_ABS")
				break;
			case  OT_FLOATING_ORE:
				LogRange("OT_FLOATING_ORE")
				break;
			case  OT_MOB:
				LogRange("OT_MOB")
				break;
			case  OT_NAV:
				LogRange("OT_NAV")
				break;
			case  OT_PLANET:
				LogRange("OT_PLANET")
				break;
			case  OT_FIELD:
				LogRange("OT_FIELD")
				break;
			case  OT_CAPSHIP:
				LogRange("OT_CAPSHIP")
				break;
			case  OT_DECO:
				LogRange("OT_DECO")
				break;
			case  OT_HUSK:
				LogRange("OT_HUSK")
				break;
			case  OT_MOBSPAWN:
				LogRange("OT_MOBSPAWN")
				break;
		}
#endif
	}
}
void Player::MissionObjectScan(Object *obj)
{
}

bool Player::CheckMissionValidity(long target_param)
{
    Object *nearest_nav = NearestNav();
	ObjectManager *om = GetObjectManager();
	Object * target = (0);
	if (om) target = om->GetObjectFromID(ShipIndex()->GetTargetGameID());	// Get Target

    //quick check to see if device use is valid etc (will also be used for scans etc).
    bool success = false;
    for (int i = 0; i < MAX_MISSIONS; ++i)
    {
        AuxMission * am = &m_PlayerIndex.Missions.Mission[i];
        if (am->GetStageNum() > 0 && am->GetDatabaseID() > -1)
        {
			if (CheckStageCompletionNodes(i, am->GetStageNum(), 0, target, target_param))
			{
				success = true;
				break;
			}
        }
    }

    return success;
}

bool Player::NPCTalkTree(long mission_slot, long response)
{
	bool talk_tree = false;
    AuxMission * m = &PlayerIndex()->Missions.Mission[mission_slot];
	MissionTree *mTree = g_ServerMgr->m_Missions.GetMissionTree(m->GetDatabaseID());

	if (mTree->Nodes[m->GetStageNum()] == (0))
	{
		SendVaMessage("Bug in mission %s, stage %d, PLEASE REPORT TO DEVS", mTree->name, m->GetStageNum());
		return false;
	}

	TalkTree *tree = &mTree->Nodes[m->GetStageNum()]->talk_tree;

	long length = GenerateTalkTree(tree, response);

    if (length)
    {
        SendOpcode(ENB_OPCODE_0054_TALK_TREE, (unsigned char *) m_TalkTreeBuffer, length);
		//read the talk tree node flags here
		//get the node flags and destination
		talk_type flags = tree->Nodes[response]->Flags;
		long destination = tree->Nodes[response]->Destination;

		switch (flags)
		{
		case NODE_MISSION_GOTO:
			if (destination == 0)
			{
				SendVaMessage("Error in mission '%s'. Stage destination for stage %d is invalid", mTree->name, m->GetStageNum());
				LogMessage("Error in mission '%s'. Stage destination for stage %d is invalid\n", mTree->name, m->GetStageNum());
				SendTalkTreeAction(-32);
			}
			else
			{
				SendTalkTreeAction(6);
				AdvanceMission(mission_slot, destination);
			}
			break;

		case NODE_MISSION_COMPLETE:
			SendTalkTreeAction(6);
			AdvanceMission(mission_slot, -3);
			break;

		case NODE_POSTPONE_MISSION:
			SendTalkTreeAction(6);
			RemoveMission(mission_slot);
			break;

		case NODE_DROP_MISSION:
			SendTalkTreeAction(6);
			CompleteMission(m->GetDatabaseID(), 1);
            m->SetIsCompleted(true);
            SendAuxPlayer();
			break;

		default:
			break;
		}

		talk_tree = true;
        /*if (next_stage != 0)
        {
            SendTalkTreeAction(6);
            AdvanceMission(mission_slot, next_stage);
        }*/
    }

	return talk_tree;
}

void Player::ProposeMissionTree(long mission_id, long response)
{
 	MissionTree *mTree = g_ServerMgr->m_Missions.GetMissionTree(mission_id);
	TalkTree *tree = &mTree->Nodes[0]->talk_tree;
	long length = GenerateTalkTree(tree, response);

    if (length)
    {
		SendOpcode(ENB_OPCODE_0054_TALK_TREE, (unsigned char *) m_TalkTreeBuffer, length);

		talk_type flags = tree->Nodes[response]->Flags;
		long destination = tree->Nodes[response]->Destination;

        if (destination != 0 && flags == NODE_MISSION_GOTO)
        {
            SendTalkTreeAction(6);
			EndStageReward(mission_id, 0);
			AssignMission(mission_id);
			m_MissionAcceptance = false;
        }
    }
}

void Player::AdvanceMission(long mission_slot, long stage)
{
    AuxMission * m = &m_PlayerIndex.Missions.Mission[mission_slot];
    //see if there's any reward for completing the current stage
    if (CheckEndStageConditions(m))
    {
        EndStageReward(m->GetDatabaseID(), m->GetStageNum());
        m->SetMissionData(0);

        if (stage > 0)
        {
            m->SetStageNum(stage);
            m->Stages.Stage[stage-1].SetText(g_ServerMgr->m_Missions.GetStageDescription(m->GetDatabaseID(), stage));
            SendAuxPlayer();
			SaveAdvanceMission(mission_slot);
        }
        else if (stage == -2) //repeatable mission
        {
            RemoveMission(mission_slot);
			CompleteMission(m->GetDatabaseID(), 0);
        }
        else if (stage == -3) //non-repeatable mission
        {
            m->SetIsCompleted(true);
            SendAuxPlayer();
			CompleteMission(m->GetDatabaseID(), 1);
		}
    }
}

bool Player::CheckEndStageConditions(AuxMission *m)
{
    bool success = true;
	CompletionList::iterator itrCList;
	//get node list
	MissionTree *mTree = g_ServerMgr->m_Missions.GetMissionTree(m->GetDatabaseID());

	//now get completion list for the stage
	CompletionList *cList = &mTree->Nodes[m->GetStageNum()]->completion_list;

	Object *nearest_nav = GetNearestNav();

	if (cList)
	{
		for (itrCList = cList->begin(); itrCList != cList->end(); ++itrCList)
		{
			CompletionNode *cNode = (*itrCList);

			switch (cNode->type)
			{
			case GIVE_ITEM_NPC:
				if (CargoItemCount(cNode->data) >= cNode->count)
				{
					CargoRemoveItem(cNode->data, cNode->count);
					SendAuxShip();
					success = true;
				}
				break;

			case GIVE_CREDITS_NPC:
				if (PlayerIndex()->GetCredits() >= cNode->data)
				{
					PlayerIndex()->SetCredits(PlayerIndex()->GetCredits() - cNode->data);
				}
				else
				{
					PlayerIndex()->SetCredits(0);
				}
				SaveCreditLevel();
				break;

			default:
				break;
			}
		}
	}
    return success;
}

void Player::EndStageReward(long mission_id, long stage)
{
	char msg_buffer[64];
	RewardList::iterator itrRList;
	//get node list
	MissionTree *mTree = g_ServerMgr->m_Missions.GetMissionTree(mission_id);

	//now get completion list for the stage
	RewardList *rList = &mTree->Nodes[stage]->rewards;

	if (rList)
	{
		for (itrRList = rList->begin(); itrRList != rList->end(); ++itrRList)
		{
			RewardNode *rNode = (*itrRList);

			switch (rNode->type)
			{
			case CREDITS:
				PlayerIndex()->SetCredits(PlayerIndex()->GetCredits() + rNode->data);
				SaveCreditLevel();
				SendAuxPlayer();
				sprintf(msg_buffer, "You have gained %d credits!", rNode->data);
				SendMessageString(msg_buffer, 3);
				SendClientSound("coin.wav");
				break;

			case EXPLORE_XP:
				AwardExploreXP("Mission:", rNode->data);
				break;

			case COMBAT_XP:
				AwardCombatXP("Mission:", rNode->data);
				break;

			case TRADE_XP:
				AwardTradeXP("Mission:", rNode->data);
				break;

			case FACTION: //TODO
				break;

			case ITEM_ID:
				{
					_Item myItem = g_ItemBaseMgr->EmptyItem;

					if (rNode->flags == 0) rNode->flags = 1;

					myItem.ItemTemplateID = rNode->data;
					myItem.StackCount = rNode->flags;
					myItem.Price = 0;
					myItem.Quality = 1;
					myItem.Structure = 1;

					LogMessage("Given item %d to %s\n", myItem.ItemTemplateID, Name());

                    CargoAddItem(&myItem);
					SendAuxShip();
				}
				break;

			case HULL_UPGRADE:
				ShipUpgrade(rNode->data);
				break;

			case RUN_SCRIPT:  //TODO
				break;

			case AWARD_SKILL:
				if (PlayerIndex()->RPGInfo.Skills.Skill[rNode->data].GetAvailability()[0] == 3)
				{
					u32 Availability[4] = {4,0,0,1};
					PlayerIndex()->RPGInfo.Skills.Skill[rNode->data].SetAvailability(Availability);
				}
				break;

			default:
				break;
			}
		}
	}
}

void Player::RemoveMission(long mission_slot)
{
    //do we have this mission?
    AuxMission * m = &m_PlayerIndex.Missions.Mission[mission_slot];

    if (m->GetStageNum() > 0)
    {
        m->Clear();
        m->SetStageNum(0);
        SendAuxPlayer();
    }
}

void Player::MissionDismiss(long mission_id)
{
    RemoveMission(mission_id);
}
