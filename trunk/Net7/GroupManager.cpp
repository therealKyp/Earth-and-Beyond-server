//GroupManager.cpp

#include "Net7.h"
#include "PlayerManager.h"
#include "SectorManager.h"
#include "PlayerClass.h"
#include "MemoryHandler.h"
#include "Opcodes.h"

// relative positions of formation members for each formation
static float formation_positions[3][6][3] = {
	// pipe
	{ {0.0,0.0,0.0}, {-300.0,-200.0,50.0}, {-300.0,200.0,50.0}, {-300.0,0.0,50.0}, {-400.0,-200.0,100.0}, {-400.0,200.0,100.0} },
	// block
	{ {0.0,0.0,0.0}, {0.0,-200.0,0.0}, {0.0,200.0,0.0}, {-300.0,0.0,100.0}, {-300.0,-200.0,100.0}, {-300.0,200.0,100.0} },
	// slot-back
	{ {0.0,0.0,0.0}, {-300.0,-200.0,0.0}, {-300.0,200.0,0.0}, {-400.0,0.0,0.0}, {-600.0,-200.0,0.0}, {-600.0,200.0, 0} }
};


//TODO: Seperate this from the player manager
Group *PlayerManager::GetGroupFromID(int GroupID)
{
	Group *g = NULL;

	if (GroupID == -1)
	{
		return (g);
	}

	m_Mutex.Lock();

	g = m_GroupList;
	while (g)
	{
		if (g->GroupID == GroupID)
		{
			m_Mutex.Unlock();
			return (g);
		}
		g = g->next;
	}

	m_Mutex.Unlock();

	return (0);
}

long PlayerManager::GetMemberID(int GroupID, int MemberIndex)
{
	Group *g = GetGroupFromID(GroupID);

	if (!g)
	{
		return 0;
	}

	return (g->Member[MemberIndex].GameID);
}

void PlayerManager::RemoveGroup(int GroupID)
{
	m_Mutex.Lock();

	Group *g = m_GroupList;
    Group *prev = NULL;
	while (g)
	{
        if (g->GroupID == GroupID)
        {
            break;
        }
        prev = g;
        g = g->next;
    }

    if (g)
    {
        if (g == m_GroupList)
        {
            m_GroupList = g->next; //if only group, grouplist should now be null
            delete g;
            LogMessage("Grouplist = %x", m_GroupList);
        }
        else if (prev)
        {
            prev->next = g->next;
            delete g;
        }
        else
        {
            //this should never happen
            LogMessage("*** Group remove error ... group %d has no previous slot and isn't the head\n", g->GroupID);
            delete g;
        }
    }
    else
    {
        LogMessage("Could not remove group id#%d\n",GroupID);
    }

	m_Mutex.Unlock();

	return;
}

void PlayerManager::GroupInvite(int GroupID, long LeaderID, long PlayerID)
{

	LogDebug("Group Invite. GroupID: %d, LeaderID %d, PlayerID: %d", GroupID, LeaderID, PlayerID);

	Player *leader = GetPlayer(LeaderID);
	Player *player = GetPlayer(PlayerID);

	if (!player)
	{
		LogDebug("GROUP INVITE - Could not find player with ID %d\n",PlayerID);
		return;
	}

	if (!leader)
	{
		LogDebug("GROUP INVITE - Could not find leader with ID %d\n",LeaderID);
		return;
	}

	if (player->GroupID() != -1)
	{
		leader->SendVaMessage("That player is already grouped!");
		return;
	}

	// See if player is busy
	if (player->ConfirmationBusy())
	{
		leader->SendVaMessage("That player is busy try again later!");
		return;
	}

	Group *g = NULL;
	
	if (GroupID != -1)
	{
		g = GetGroupFromID(GroupID);
	}

	if (!g)
	{
		//leader was not in a group, create a new group
		Group *newGroup = new Group;
		memset(newGroup,0,sizeof(Group));
		newGroup->next = NULL;

		Group * last = NULL;
		
		m_Mutex.Lock();

		if (m_GroupList)
		{
			g = m_GroupList;

			while (g)
			{
                if (g == g->next)
                {
                    LogMessage("Critical Group error, mutex lock. Group ID %d\n", g->GroupID);
                    break;
                }
				last = g;
				g = g->next;
			}

			last->next = newGroup;
		}
		else
		{
			m_GroupList = newGroup;
		}

        m_Mutex.Unlock();

		newGroup->GroupID = m_NextGroup++;
		newGroup->AutoReleaseLootingRights = false;
		newGroup->ForceAutoSplit = false;
		newGroup->RestrictedLootingRights = false;

		leader->SetGroupID(newGroup->GroupID);
		leader->SetAcceptedGroupInvite(false);
		newGroup->Member[0].GameID = LeaderID;
		newGroup->Member[0].Position = -1;
		newGroup->AcceptedInvite[0] = false;
		strcpy(newGroup->Member[0].Name, leader->Name());

		player->SetGroupID(newGroup->GroupID);
		player->SetAcceptedGroupInvite(false);
		newGroup->Member[1].GameID = PlayerID;
		newGroup->Member[1].Position = -1;
		newGroup->AcceptedInvite[1] = false;
		strcpy(newGroup->Member[1].Name, player->Name());

		leader->SendVaMessage("%s has been invited",newGroup->Member[1].Name);
		player->SendVaMessage("You have been invited by %s",newGroup->Member[0].Name);	
		
		SendGroupInvite(newGroup, player);

		for (int i=2;i<6;i++)
		{
			newGroup->Member[i].GameID = -1;
		}
	}
	else
	{
		if (g->Member[0].GameID != LeaderID)
		{
			leader->SendVaMessage("You are not the group leader!");
			return;
		}

		//invite person to existing group
		int PlayerCount = GetMemberCount(g);

		if (PlayerCount == 6)
		{
			leader->SendVaMessage("The Group is full!");
			return;
		}

		m_Mutex.Lock();

		player->SetGroupID(g->GroupID);
		player->SetAcceptedGroupInvite(false);
		strcpy(g->Member[PlayerCount].Name, player->Name());
		g->Member[PlayerCount].GameID = PlayerID;
		g->Member[PlayerCount].Position = -1;
		g->AcceptedInvite[PlayerCount] = false;
        m_Mutex.Unlock();

		leader->SendVaMessage("%s has been invited",g->Member[PlayerCount].Name);

		SendGroupInvite(g, player);		
	}
}

void PlayerManager::GroupCombatXP(Player *owner, char * msg, int MOB_level)
{
	Group *g = GetGroupFromID(owner->GroupID());
	Player *player = 0;

	float BonusXP[] = { 1.0f, 1.4f, 1.8f, 2.2f, 2.6f, 3.0f };
    float XP_Gain;
    char msg_buffer[128];

	if (g)
	{
		int Count = GetMemberCount(g);
		int inRangeCount = GetMembersInXPRange(owner, g);

		float GroupBonus;

        float XP;

		if (inRangeCount == 1)
		{
            XP_Gain = (float) owner->CalculateMOBXP(MOB_level);
			_snprintf(msg_buffer, 128, "%s %d Combat experience earned (out of range for group bonus)", msg, int(XP_Gain)); 
            owner->AwardCombatXP(msg_buffer, (long)XP_Gain, true);
            return;
		} 

		for (int i=0;i<6;i++)
		{
			if (g->Member[i].GameID == -1) continue;

			player = GetPlayer(g->Member[i].GameID);

			if (player && player->Active() && player->AcceptedGroupInvite()
				&& player->PlayerIndex()->GetSectorNum() == owner->PlayerIndex()->GetSectorNum()
				&& player->RangeFrom(owner->Position()) < 40000.0f && !player->ShipIndex()->GetIsIncapacitated())
            {
                //this player is in range for XP
                XP_Gain = (float) player->CalculateMOBXP(MOB_level);
                XP = float(XP_Gain/inRangeCount);
                GroupBonus = XP * BonusXP[inRangeCount-1];
                _snprintf(msg_buffer, 128, "%s Combat experience earned (%d + %d group bonus)", msg, int(XP), int(GroupBonus)); 
                player->AwardCombatXP(msg_buffer, (long)XP_Gain, true);
            }
		}
    }
    else
    {   //Player not in group.
        XP_Gain = (float) owner->CalculateMOBXP(MOB_level);
        _snprintf(msg_buffer, 128, "%s %d Combat experience earned", msg, int(XP_Gain)); 
        owner->AwardCombatXP(msg_buffer, (long)XP_Gain, true);
    }
}

bool PlayerManager::GroupXP(Player *owner, char * name, int XP_Gain, int GroupID, int Type)
{
	Group *g = GetGroupFromID(GroupID);
	Player *player = 0;

	float BonusXP[] = { 1.0f, 1.4f, 1.8f, 2.2f, 2.6f, 3.0f };

	if (g)
	{
		int Count = GetMemberCount(g);
		int inRangeCount = GetMembersInXPRange(owner, g);

		float XP = float(XP_Gain/inRangeCount);
		float GroupBonus = XP * BonusXP[inRangeCount-1];

		char msg_buffer[128];

		if (inRangeCount == 1)
		{
			_snprintf(msg_buffer, 128, "%s %d %s experience earned (out of range for group bonus)", name, int(XP_Gain), Type?"Explore":"Combat"); 
		} 
        else 
        {
			_snprintf(msg_buffer, 128, "%s %s experience earned (%d + %d group bonus)", name, Type?"Explore":"Combat", int(XP), int(GroupBonus)); 
		}

		for (int i=0;i<6;i++)
		{
			if (g->Member[i].GameID == -1) continue;

			player = GetPlayer(g->Member[i].GameID);

			if (player && player->Active() && player->AcceptedGroupInvite() 
				&& player->GetSectorManager()->GetSectorID() == owner->GetSectorManager()->GetSectorID()
				&& player->RangeFrom(owner->Position()) < 40000.0f && !player->ShipIndex()->GetIsIncapacitated())
			{
				if (player) 
				{
					player->SendPriorityMessageString(msg_buffer,"MessageLine",5000,4);
					if (Type == 0)
					{
						if (inRangeCount == 1)
						{
							player->AwardCombatXP(name, XP_Gain, true);
						} 
                        else
                        {
							player->AwardCombatXP(name, long(XP + GroupBonus), true);
						}
					}
					else
					{
						if (inRangeCount == 1)
						{
							player->AwardExploreXP(name, XP_Gain, true);
						}
                        else
                        {
							player->AwardExploreXP(name, long(XP + GroupBonus), true);
						}
					}
				}
			}
		}

		return true;
	}
	else
	{
		return false;
	}
}

void PlayerManager::GroupChat(int GroupID, char * SenderName, char * Message)
{
	Group *g = GetGroupFromID(GroupID);
	Player *player = 0;

	if (g)
	{	
		for (int i=0;i<6;i++)
		{
			if (g->Member[i].GameID != -1 && g->AcceptedInvite[i])
			{
				player = GetPlayer(g->Member[i].GameID);	

				if (player)
				{
					player->SendClientChatEvent(0x03, 0x00, SenderName, SenderName, "Group", Message);
				}
			}
		}
	}
}


void PlayerManager::SendGroupInvite(Group * myGroup, Player *p)
{
	char msg[60];
	unsigned char buffer[80];
	int index = 0;

	if (!p)
	{
		return;
	}

	memset(&buffer, 0, sizeof(buffer));

	if (myGroup->Member[0].Name)
	{
		_snprintf(msg, 128, "%s is requesting you to join their group", myGroup->Member[0].Name);
	}
	else
	{
		_snprintf(msg, 128, "A NULL NAME WAS SENT TO GROUPING METHOD!");
	}

	*((short *) &buffer[index]) = strlen(msg) + 1; index += 2;
	*((char *) &buffer[index]) = (char) 0x01; index++;
	memcpy(&buffer[index], msg, strlen(msg)); index += (strlen(msg) + 1);

	p->SendOpcode(ENB_OPCODE_001E_GROUP, buffer, index); //TODO: use a proper opcode description please!!
	p->SetConfirmation(1);			// Let us know we are sending a group invite
}

void PlayerManager::AcceptGroupInvite(int GroupID, long PlayerID)
{
	int MemberIndex = -1;
	Group *g = GetGroupFromID(GroupID);

	if (!g)
	{
		LogDebug("ACCEPT GROUP INVITE - No group with ID %d\n",GroupID);
		return;
	}
		
	m_Mutex.Lock();
	
	for (int i=1;i<6;i++)
	{
		if (g->Member[i].GameID == PlayerID)
		{
			MemberIndex = i;
		}
	}

    m_Mutex.Unlock();

	if (MemberIndex == -1)
	{
		LogDebug("ACCEPT GROUP INVITE - Player %d not in group %d\n",PlayerID, GroupID);
		return;
	}

	g->AcceptedInvite[MemberIndex] = true;
	g->AcceptedInvite[0] = true;

    //get group info into player
    Player *p = GetPlayer(PlayerID);
    p->SetGroup(g);
    p->SetGroupID(GroupID);
	p->SetAcceptedGroupInvite(true);

	SendAuxToGroup(g);
}

void PlayerManager::RejectGroupInvite(int GroupID, long PlayerID)
{
	int MemberIndex = -1;
	Player *p;
	Group *g = GetGroupFromID(GroupID);

	LogDebug("REJECT GROUP - Player %d declined invitation to group %d\n",PlayerID, GroupID);

	if (!g)
	{
		LogDebug("REJECT GROUP INVITE - No group with ID %d\n",GroupID);
		return;
	}

	m_Mutex.Lock();
	
	for (int i=1;i<6;i++)
	{
		if (g->Member[i].GameID == PlayerID)
		{
			MemberIndex = i;
		}
	}

    m_Mutex.Unlock();

	if (MemberIndex == -1)
	{
		LogDebug("REJECT GROUP INVITE - Player %d not in group %d\n",PlayerID, GroupID);
		return;
	}

	p = GetPlayer(PlayerID);

	if (!p)
	{	
		return;
	}

	p->SetGroupID(-1);
	p->SetAcceptedGroupInvite(false);
	
	p = GetPlayer(g->Member[0].GameID);

	if (!p)
	{
		return;
	}

	p->SendVaMessage("%s has rejected your group invitation.",g->Member[MemberIndex].Name);

	memset(&g->Member[MemberIndex],0,sizeof(_GroupMember));
	g->Member[MemberIndex].GameID = -1;

	if (!g->AcceptedInvite[0])
	{
		p->SetGroupID(-1);
		p->SetAcceptedGroupInvite(false);
		RemoveGroup(g->GroupID);
	}

}

void PlayerManager::LeaveGroup(int GroupID, long PlayerID)
{
	Group *g = GetGroupFromID(GroupID);
	if (!g)
	{
		Player *p = GetPlayer(PlayerID);
	
		if (!p)
		{
			return;
		}

		SendEmptyGroupAux(PlayerID);
	}
	else
	{	
		m_Mutex.Lock();

		bool found = false;
		for (int i=0;i<6;i++)
		{
			if (g->Member[i].GameID == PlayerID)
			{
				RemovePlayerFromGroup(g, i);
				found = true;
				break;
			}
		}

        m_Mutex.Unlock();

		if (!found)
		{
			Player *p = GetPlayer(PlayerID);
			
			if (!p)
			{
				return;
			}
			p->SendVaMessage("You are not in the group you were supposed to be in!", GroupID);
			SendEmptyGroupAux(PlayerID);
		}
	}
}

void PlayerManager::KickFromGroup(int GroupID, long LeaderID, long PlayerID)
{
	Group *g = GetGroupFromID(GroupID);

	if (!g)
	{
		Player *p = GetPlayer(LeaderID);
		
		if (!p)
		{
			return;
		}

		p->SendVaMessage("Could not find group %d! Sending no-group packet!", GroupID);
	}
	else
	{
		if (g->Member[0].GameID != LeaderID)
		{
			Player *p = GetPlayer(LeaderID);
			
			if (!p)
			{
				return;
			}

			p->SendVaMessage("You are not the group leader!");
			return;
		}

		m_Mutex.Lock();

		//cannot kick yourself
		for (int i=1;i<6;i++)
		{
			if (g->Member[i].GameID == PlayerID)
			{
				RemovePlayerFromGroup(g, i);
				break;
			}
		}

		m_Mutex.Unlock();
	}
}

void PlayerManager::RemovePlayerFromGroup(Group * myGroup, int MemberIndex)
{
	if (!myGroup)
	{
		LogDebug("REMOVE PLAYER FROM GROUP - Null group!\n");
		return;
	}

    //update player group info
    Player *p = GetPlayer(myGroup->Member[MemberIndex].GameID);
	if (p)
	{
		p->SetGroup(0);
		p->SetGroupID(-1);
		p->SetAcceptedGroupInvite(false);
	}

	if (GetMemberCount(myGroup) == 2)
	{
		DisbanGroup(myGroup);
		return;
	}

	SendEmptyGroupAux(myGroup->Member[MemberIndex].GameID);

	// Leave formation
	if (MemberIndex == 0)
	{
		BreakFormation(myGroup->Member[MemberIndex].GameID);
	}
	else
	{
		LeaveFormation(myGroup->Member[MemberIndex].GameID);
	}

	//shift up members (assign a new lead if needed)
	for (int x=MemberIndex;x<5;x++)
	{
		myGroup->Member[x] = myGroup->Member[x+1];
	}

	//clear last member but set the flags to send an empty name
	memset(&myGroup->Member[5], 0, sizeof(_GroupMember));
	myGroup->Member[5].GameID = -1;

	SendAuxToGroup(myGroup);
}

void PlayerManager::RequestGroupAux(int GroupID, int PlayerID)
{
	Group *g;
	
	LogDebug("Requesting groupAux GroupID: %d, PlayerID: %d\n", GroupID, (PlayerID & 0x00FFFFFF));

	if (GroupID != -1)
	{
		g = GetGroupFromID(GroupID);
	}
	else
	{
		SendEmptyGroupAux(PlayerID);
		return;
	}

	if (g)
	{
		m_Mutex.Lock();

		int MemberIndex = -1;
		for (int i=0;i<6;i++)
		{
			if (g->Member[i].GameID == PlayerID)
			{
                if (!g->AcceptedInvite[i])
                {
                    g->Member[i].GameID = -1;
                }
                else
                {
				    MemberIndex = i;
                }
				break;
			}
		}

        m_Mutex.Unlock();

		if (MemberIndex != -1)
		{
			_GroupInfo myInfo;
			memset(&myInfo, 0, sizeof(_GroupInfo));

			Player *p = GetPlayer(PlayerID);
			
			if (!p)
			{
				return;
			}

			SetGroupInfoMembersFromGroup(myInfo, g, MemberIndex);

			p->PlayerIndex()->GroupInfo.SetData(&myInfo);
			p->PlayerIndex()->GroupInfo.SetIsGroupLeader(MemberIndex == 0);
			// Formation Stuff
			Player * leader = GetPlayer(g->Member[0].GameID);
			if (leader)
			{
				if (leader->PlayerIndex()->GetSectorNum() == p->PlayerIndex()->GetSectorNum())
				{
					// Show formation info
					p->PlayerIndex()->GroupInfo.SetFormation(0);
					p->PlayerIndex()->GroupInfo.SetFormationName(g->FormationName);
					p->PlayerIndex()->GroupInfo.SetPosition(g->Member[MemberIndex].Position);
				}
				else
				{
					g->Member[MemberIndex].Formation = 0;
					g->Member[MemberIndex].Position = -1;
					// Dont show if outside of sector
					p->PlayerIndex()->GroupInfo.SetFormation(0);
					p->PlayerIndex()->GroupInfo.SetPosition(-1);
					p->PlayerIndex()->GroupInfo.SetFormationName("");
					p->SendAuxPlayer();
				}
			}

			p->SetGroupID(g->GroupID);
			p->SetAcceptedGroupInvite(true);

		}
		else
		{
			SendEmptyGroupAux(PlayerID);
		}	
	}
	else
	{
		SendEmptyGroupAux(PlayerID);
	}
}

void PlayerManager::DisbanGroup(int GroupID, long PlayerID)
{
	Group *g = GetGroupFromID(GroupID);

	if (!g)
	{
		LogDebug("DISBAN GROUP - Could not find group with id %d\n",GroupID);
		return;
	}

	m_Mutex.Lock();

	if (g->Member[0].GameID != PlayerID)
	{
		Player *p = GetPlayer(PlayerID);
		
		if (!p)
		{
			m_Mutex.Unlock();
			return;
		}

		p->SendVaMessage("You are not the leader!");
	}

    m_Mutex.Unlock();

	BreakFormation(g->Member[0].GameID);
	DisbanGroup(g);
}

void PlayerManager::DisbanGroup(Group *myGroup)
{

	if (!myGroup)
	{
		LogDebug("DISBAND GROUP - Null group!\n");
		return;
	}

	for (int i=0;i<6;i++)
	{
		if (myGroup->Member[i].GameID != -1)
		{
			SendEmptyGroupAux(myGroup->Member[i].GameID);
		}
	}

	BreakFormation(myGroup->Member[0].GameID);

	RemoveGroup(myGroup->GroupID);
}

int PlayerManager::GetMemberCount(int GroupID)
{
	Group *g = GetGroupFromID(GroupID);
	return (GetMemberCount(g));
}

int PlayerManager::GetMemberCount(Group * myGroup)
{
	int count = 0;

	if (!myGroup)
	{
		LogDebug("GET MEMBER COUNT - Null group!\n");
		return 0;
	}

	m_Mutex.Lock();

	//count is amount of members with a non-zero ID
	for (int i=0;i<6;i++)
	{
		if (myGroup->Member[i].GameID != -1)
		{
			count++;
		}
	}
	
	m_Mutex.Unlock();

	return (count);
}


int PlayerManager::GetMembersInXPRange(Player *player, Group * myGroup)
{
	int count = 1; // includes the player himself
	float XPRange = 40000.0f;
	Player * otherPlayer = 0;

	if (!myGroup)
	{
		LogDebug("GET MEMBER COUNT - Null group!\n");
		return 0;
	}

	m_Mutex.Lock();

	//count is amount of members with a non-zero ID
	for (int i=0;i<6;i++)
	{
		if (myGroup->Member[i].GameID == -1 || myGroup->Member[i].GameID == player->GameID()) continue;

		otherPlayer = GetPlayer(myGroup->Member[i].GameID);

		if ((player && otherPlayer) && player->IsInSameSector(otherPlayer)
			&& player->RangeFrom(otherPlayer->Position()) < XPRange)
		{
			count++;
		}
	}
	
	m_Mutex.Unlock();

	return (count);
}

void PlayerManager::SendEmptyGroupAux(long PlayerID)
{
	Player *p = GetPlayer(PlayerID);

	if (!p)
	{
		return;
	}

	if (p->GroupID() != -1)
	{
		p->SendVaMessage("You have left the group");
	}

	p->SetGroupID(-1);
	p->SetAcceptedGroupInvite(false);

	p->PlayerIndex()->GroupInfo.Members.Clear();

	p->PlayerIndex()->GroupInfo.SetIsGroupLeader(false);
	p->PlayerIndex()->GroupInfo.SetAllowGroupInvite(true);
	p->PlayerIndex()->GroupInfo.SetShowNonCombatantActivities(true);

	p->SendAuxPlayer();
}

void PlayerManager::SendAuxToGroup(Group *myGroup)
{
	int PlayerCount = GetMemberCount(myGroup);

	if (PlayerCount < 2)
	{
		DisbanGroup(myGroup);
	}
	else
	{
		_GroupInfo myInfo;
		memset(&myInfo, 0, sizeof(_GroupInfo));

		for (int i=0;i<PlayerCount;i++)
		{
			Player *p = GetPlayer(myGroup->Member[i].GameID);
			if (!p)
			{
				LogDebug("SEND AUX TO GROUP - Cannot find player with id %d",myGroup->Member[i].GameID);
			}
			else
			{
				SetGroupInfoMembersFromGroup(myInfo,myGroup,i);

				p->SetGroupID(myGroup->GroupID);
				p->SetAcceptedGroupInvite(true);

				p->PlayerIndex()->GroupInfo.SetData(&myInfo);
				p->PlayerIndex()->GroupInfo.SetIsGroupLeader(i == 0);
				
				// Formation Stuff
				p->PlayerIndex()->GroupInfo.SetFormation(myGroup->Member[i].Position != -1 ? myGroup->Member[i].Formation : 0);
				p->PlayerIndex()->GroupInfo.SetFormationName(myGroup->FormationName);
				p->PlayerIndex()->GroupInfo.SetPosition(myGroup->Member[i].Position);

				p->SendAuxPlayer();
			}
		}
	}
}

void PlayerManager::SetGroupInfoMembersFromGroup(_GroupInfo & myGroupInfo, Group * myGroup, int MemberIndex)
{
	int i,offset = 0;

	for (i=0;i<6;i++)
	{
		if (i == MemberIndex)
		{
			myGroupInfo.Formation = myGroup->Member[i].Formation;
			myGroupInfo.Position = myGroup->Member[i].Position;
			offset++;
		}
		else if (!myGroup->AcceptedInvite[i])
		{
			offset++;
		}
		else
		{
			memcpy(&myGroupInfo.Members.Member[i-offset],&myGroup->Member[i],sizeof(_GroupMember));
		}
	}
}

bool PlayerManager::CheckGrouped(Player *p1, Player *p2)
{
	if (!p1 || !p2)
	{
		return false;
	}

    if (p1->GroupID() == p2->GroupID() && p1->GroupID() != -1 && p1->AcceptedGroupInvite() && p2->AcceptedGroupInvite())
    {
        return true;
    }
    else
    {
        return false;
    }
}

float PlayerManager::GetGroupWarpSpeed(Player *p)
{
	if (!p)
	{
		return 0;
	}

	if (p->GroupID() == -1)
	{
		return (float)p->ShipIndex()->CurrentStats.GetWarpSpeed();
	}

	int GroupID = p->GroupID();
	float MinWarpSeed = 10000.0f;
	Group* g = GetGroupFromID(p->GroupID());

	// Find the lowest warp speed in the group
	if (g)
	{
		// If we are not in the formation do not drop out warp speed
		for(int x=0;x<6;x++)
		{
			if (g->Member[x].GameID == p->GameID() && g->Member[x].Position != -1)
			{
				return (float)p->ShipIndex()->CurrentStats.GetWarpSpeed();	
			}
		}

		for(int x=0;x<6;x++)
		{
			if (g->Member[x].GameID != 0 && g->Member[x].Position != -1)
			{
				Player * player = GetPlayer(g->Member[x].GameID);

				if (player)
				{
					if (player->ShipIndex()->CurrentStats.GetWarpSpeed() < MinWarpSeed)
					{
						MinWarpSeed = (float)(player->ShipIndex()->CurrentStats.GetWarpSpeed());
					}
				}
			}
		}
		return MinWarpSeed == 10000.0f ? (float)p->ShipIndex()->CurrentStats.GetWarpSpeed() : MinWarpSeed;
	}

	return (float)p->ShipIndex()->CurrentStats.GetWarpSpeed();
}

bool PlayerManager::CheckGroupFormation(Player *p)
{
	if (!p)
	{
		return false;
	}

	if (p->GroupID() == -1)
	{
		return false;
	}

	int GroupID = p->GroupID();
	Group* g = GetGroupFromID(p->GroupID());

	if (g)
	{
		for(int x=0;x<6;x++)
		{
			if (g->Member[x].GameID == p->GameID() && g->Member[x].Position != -1)
			{
				return true;
			}
		}
	}

	return false;
}

void PlayerManager::FormationEngineOperation(Player *p, bool engine)
{
	if (!p) return;

	Group* g = GetGroupFromID(p->GroupID());

	if (g)
	{
		Player *gLeader = GetPlayer(g->Member[0].GameID);
		if (gLeader == p) //only process engine on request if this is from the group leader
		{
			for(int x=1;x<6;x++)
			{			
				// See if we are in the group & Formation Locked
				if (g->Member[x].Position != -1)
				{
					Player *gMember = GetPlayer(g->Member[x].GameID);
					if (gMember)
					{
						gMember->SendContrailsRL(engine);
					}
				}
			} 
		}
	}

}

bool PlayerManager::SendFormation(Player *SendP, Player *TargetP)
{
	if (!SendP || !TargetP)
	{
		return false;
	}

	// Make sure this player is not in a fomration, and if so only send the leader pos
	Group* g = GetGroupFromID(SendP->GroupID());

	if (g)
	{
		Player *gLeader = GetPlayer(g->Member[0].GameID);
		if (gLeader && (gLeader != TargetP || gLeader->WarpDrive()))
		{
			TargetP->SendAdvancedPositionalUpdate(g->Member[0].GameID, gLeader->PosInfo());
		}

		for(int x=1;x<6;x++)
		{
			// See if we are in the group & Formation Locked
			if (g->Member[x].GameID == SendP->GameID() && g->Member[x].Position != -1)
			{
				float * position;
				int formation = g->Member[x].Formation;

				if (!gLeader) return false;

				if (x!=0)
				{
					position = formation_positions[formation-4][x];
					TargetP->SendFormationPositionalUpdate(g->Member[0].GameID, SendP->GameID(), position[0], position[1], position[2]);
				}
			} 
			else if (g->Member[x].GameID == SendP->GameID() && g->Member[x].Position == -1)
			{
				// If we are in the group and not in a formation
				TargetP->SendAdvancedPositionalUpdate(SendP->GameID(), SendP->PosInfo());
			}
		}
	}
	return true;
}


bool PlayerManager::GroupAction(long SourceID, long TargetID, long Action)
{

	// formation request
	switch (Action)
	{
		case 4:
			// slot_back
			return SetFormation(SourceID, Action, "Slot Back");
			break;
		case 5:
			// block
			return SetFormation(SourceID, Action, "Block");
			break;
		case 6:
			// pipe
			return SetFormation(SourceID, Action, "Pipe");
			break;
		case 7:
			// Form up! ?
			return FormUp(SourceID);
			break;
		case 8:
			return LeaveFormation(SourceID);
			// Leave formation?
			break;
		case 9:
			// Break formation
			return BreakFormation(SourceID);
			break;
		case 12:
			// Ask member/group to acquire your target
			return RequestTargetMyTarget(SourceID, TargetID);
			break;
		default:
			break;
	}
	return false;
}

bool PlayerManager::FormUp(long PID)
{
	Player* p = GetPlayer(PID);
	// Make sure we found player
	if (!p)
	{
		return false;
	}

	// Make sure we are not dead
	if (p->ShipIndex()->GetIsIncapacitated())
	{
		return false;
	}

	long groupid = p->GroupID();
	Group* g = GetGroupFromID(groupid);
	if (!g)
	{
		return false;
	}

	Player * leader = GetPlayer(g->Member[0].GameID);

	if (!leader) return false;

	float *position;

	if (g)
	{
		int i = -1;

		// Make sure we are in the same sector as the leader
		if (leader->PlayerIndex()->GetSectorNum() != p->PlayerIndex()->GetSectorNum())
		{
			return false;
		}

		if (p->RangeFrom(leader->Position()) > 5000)
		{
			p->SendPriorityMessageString("You are not in range to form up","MessageLine",1000,4);
			return false;
		}

		

		// Find Member index in group
		for(int x=0;x<6;x++)
		{
			if (g->Member[x].GameID == PID)
			{
				i = x;
			}
		}

		// if not found leave! (should not happen)
		if (i == -1)
		{
			return false;
		}

		int Form = g->Member[0].Formation;
		// Lock formation
		g->Member[i].Formation = Form;
		g->Member[i].Position = i;		// Lock position
		p->PlayerIndex()->GroupInfo.SetFormation(Form);
		p->PlayerIndex()->GroupInfo.SetPosition(i);
		p->SendAuxPlayer();

		// set the relative position of this member in this formation
		position = formation_positions[Form-4][i];
		p->SendFormationPositionalUpdate(g->Member[0].GameID, PID, position[0], position[1], position[2]);
	}

	return true;
}

bool PlayerManager::LeaveFormation(long PID)
{
	Player* p = GetPlayer(PID);

	// Make sure we found player
	if (!p)
	{
		return false;
	}

	long groupid = p->GroupID();
	Group* g = GetGroupFromID(groupid);

	if (g)
	{
		int i = -1;

		// Find Member index in group
		for(int x=0;x<6;x++)
		{
			if (g->Member[x].GameID == PID)
			{
				i = x;
			}
		}

		// if not found leave! (should not happen)
		if (i == -1)
		{
			return false;
		}

		if (g->Member[i].Position == -1)
		{
			return false;
		}

		int Form = g->Member[i].Formation;
		// Lock formation
		g->Member[i].Position = -1;		// UnLock position
		g->Member[i].Formation = 0;		// UnLock formation
		p->PlayerIndex()->GroupInfo.SetFormation(0);
		p->PlayerIndex()->GroupInfo.SetPosition(-1);
		p->PlayerIndex()->GroupInfo.SetFormationName(g->FormationName);
		p->SendAuxPlayer();

		// update this members position
		if (i)
		{
			p->SendLocationAndSpeed(true);
		}
	}

	return true;
}

bool PlayerManager::SetFormation(long leaderID, long formation, char* formation_name)
{
	Player * leader = GetPlayer(leaderID);
	Player* p = NULL;
	long memberID;

	if (!leader) return false;

	long groupid = leader->GroupID();

	if (!leader) return false;

	if (leader != GetPlayer(GetMemberID(groupid, 0)))
	{
		// This is not the leader, probaly a hack
		return false;
	}

	if (!leader)
	{
		// Can't Find leader?
		return false;
	}

	if (leader->ShipIndex()->GetIsIncapacitated())
	{
		// Cant form when you are dead
		return false;
	}

	Group* g = GetGroupFromID(groupid);

	if (g)
	{
		strcpy(g->FormationName, formation_name);
	}
	else
	{
		return false;
	}

	for (int i = 0; i < 6; ++i)
	{
		memberID = GetMemberID(groupid, i);
		p = GetPlayer(memberID);
		if (p)
		{
			// only lock fomration with leader
			g->Member[i].Formation = i == 0 ? formation : 0;
			g->Member[i].Position = i == 0 ? 0 : -1;

			if (!i)
			{
				p->SendVaMessage("You have initiated a %s formation.", formation_name);
			}

			if (p->PlayerIndex()->GetSectorNum() == leader->PlayerIndex()->GetSectorNum())
			{
				p->PlayerIndex()->GroupInfo.SetFormation(g->Member[i].Formation);
				p->PlayerIndex()->GroupInfo.SetFormationName(formation_name);
				p->PlayerIndex()->GroupInfo.SetPosition(g->Member[i].Position);
				p->SendVaMessage("%s formation initiated.", formation_name);
				p->SendAuxPlayer();
			}
		}
	}

	return true;
}

bool PlayerManager::BreakFormation(long leaderID)
{
	Player* p = GetPlayer(leaderID);

	if (!p)
	{
		return false;
	}
	long groupid = p->GroupID();
	p = NULL;

	Group* g = GetGroupFromID(groupid);
	Player* leader = GetPlayer(leaderID);

	if (!g || !leader)
	{
		return false;
	}

	long memberID;
	long formation = g->Member[0].Formation;
	float* leaderPosition = leader->Position();
	// leaderOrientation = ?	// TODO - set the group member orientations

	if (g->Member[0].GameID != leaderID)
	{
		// Leader can only break formation
		return false;
	}

	sprintf(g->FormationName, "");

	for (int i = 0; i < 6; ++i)
	{
		memberID = GetMemberID(groupid, i);
		p = GetPlayer(memberID);
		if (p)
		{
			g->Member[i].Formation = 0;
			g->Member[i].Position = -1;

			p->SendVaMessage("Break formation initiated.");
			p->PlayerIndex()->GroupInfo.SetFormation(0);
			p->PlayerIndex()->GroupInfo.SetFormationName("");
			p->PlayerIndex()->GroupInfo.SetPosition(-1);
			p->SendAuxPlayer();

			// update this members position
			if (i)
			{
				p->SendLocationAndSpeed(true);
			}
		}
	}

	return true;
}

bool PlayerManager::RequestTargetMyTarget(long sourceID, long targetID)
{
	long groupid = GetPlayer(sourceID)->GroupID();
	Group* g = GetGroupFromID(groupid);
	Player* p = NULL;
	long memberID;

	for (int i = 0; i < 6; ++i)
	{
		memberID = GetMemberID(groupid, i);
		if (targetID == -1 || targetID == memberID)
		{
			p = GetPlayer(memberID);
			if (p)
			{
				p->SendVaMessage("Target my target.");
				//p->SendAuxPlayer();
			}
		}
	}

	return true;
}

