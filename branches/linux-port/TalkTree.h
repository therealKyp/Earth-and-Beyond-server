// TalkTree.h

#ifdef WIN32
#pragma warning(disable:4786)
#endif

#include <map>
#include <vector>
#include "Net7.h"

#ifndef _TALK_TREE_H_INCLUDED_
#define _TALK_TREE_H_INCLUDED_

class Object;

using namespace std;

typedef enum
{
    NULL_NODE = 0,
    TALK_NPC = 1,
    GIVE_ITEM_NPC = 2,
    RECEIVE_ITEM_NPC = 3,
    FIGHT_MOB = 4,
	SECTOR = 5,
	NEAREST_NAV = 6,
    TAKE_ITEM_TO_LOCATION = 7,
    OBTAIN_ITEMS = 8,
    OBTAIN_ITEMS_AT_LOCATION = 9,
    USE_SKILL_ON_MOB_TYPE = 10,
    USE_SKILL_ON_OBJECT = 11,
    SCAN_OBJECT = 12,
	POSSESS_ITEM = 13,
	GIVE_CREDITS_NPC = 14,
	ARRIVE_AT = 15
} completion_node_type;

typedef enum
{
	CREDITS = 1,
	EXPLORE_XP = 2,
	COMBAT_XP = 3,
	TRADE_XP = 4,
	FACTION = 5,
	ITEM_ID = 6,
	HULL_UPGRADE = 7,
	RUN_SCRIPT = 8,
	AWARD_SKILL = 9
} reward_type;

typedef enum
{
	OVERALL_LEVEL = 1,
	COMBAT_LEVEL,
	EXPLORE_LEVEL,
	TRADE_LEVEL,
	RACE,
	PROFESSION,
	HULL_LEVEL,
	FACTION_REQUIRED,
	ITEM_REQUIRED,
	MISSION_REQUIRED
} restrict_type;

struct RewardNode
{
	reward_type type;
	int data;
	int flags;
} ATTRIB_PACKED;

typedef vector<RewardNode*> RewardList;

struct CompletionNode
{
	completion_node_type type;
	long data;
	long count;
} ATTRIB_PACKED;

typedef vector<CompletionNode*> CompletionList;

struct TalkTreeData
{
	int TextLen;
	short flags;
	short data;
	char * Text;
} ATTRIB_PACKED;

struct TalkBranch
{
	char * Text;
	//int TextLen;
	int BranchDestination;
} ATTRIB_PACKED;

typedef vector<TalkBranch*> BranchList;

typedef enum
{
	NODE_TRADE = 1,			//node gives access to trade
	NODE_POSTPONE_MISSION,	//allows you to drop the mission to continue later
	NODE_DROP_MISSION,		//means you won't be able to do the mission again
	NODE_MORE,				//'more' dialogue box
	NODE_MISSION_GOTO,		//this indicates that we need to branch to another mission node (set as Destination="x").
	NODE_MISSION_COMPLETE
} talk_type;

struct TalkNode
{
	u16 NodeNumber;
	talk_type Flags;			// Set if we are going to trade, abort mission etc
	s16 Destination;	// Set if this node is a mission stage completion
	char * Text;
	//u16 TextLen;

	u16 NumBranches;

	BranchList Branches;
} ATTRIB_PACKED;

typedef map<int, TalkNode *> mapNodes;

struct TalkTree
{
	int NumNodes;

	mapNodes Nodes;

} ATTRIB_PACKED;

struct RestrictionNode
{
	restrict_type type;
	u32 flags;
	u32 data;
} ATTRIB_PACKED;

typedef vector<RestrictionNode*> RestrictionList;

struct _MissionNode
{
	CompletionList	completion_list;
    RewardList	    rewards;
	TalkTree	    talk_tree;
	char			*description;
} ATTRIB_PACKED;

typedef map<int, _MissionNode *> mapMissionNodes;

struct MissionTree
{
	RestrictionList restriction_list; //this is a list of restrictions for this mission/talk tree
	char           *summary;
	char		   *name;
	int NumNodes;
	mapMissionNodes	Nodes;
	long MissionID;
} ATTRIB_PACKED;

struct InteractionNode
{
    TalkTree	    talk_tree; //starting talk tree
} ATTRIB_PACKED;

//typedef map<int, NPCInteractionNode *> mapInteractionNodes;

/*#define TERRAN_FLAG		0x01000000
#define JENQUAI_FLAG	0x02000000
#define PROGEN_FLAG		0x04000000

#define TRADER_FLAG		0x08000000
#define EXPLORER_FLAG	0x10000000
#define WARRIOR_FLAG	0x20000000

#define TERRAN_VALID(x) (x&TERRAN_FLAG)
#define JENQUAI_VALID(x) (x&JENQUAI_FLAG)
#define PROGEN_VALID(x) (x&PROGEN_FLAG)

#define TRADER_VALID(x) (x&TRADER_FLAG)
#define EXPLORER_VALID(x) (x&EXPLORER_FLAG)
#define WARRIOR_VALID(x) (x&WARRIOR_FLAG)*/

#endif

/*
Mission format:

Start the mission with a <Mission> label

then then mission name, and summary.

The name is used in the client for the overall mission name and the summary is printed underneath the mission name. So both appear in the client PDA.

Then we have mission conditions. These are a set of nodes which define who can accept the mission.
The numerical values are:

1.	OVERALL_LEVEL
2.	COMBAT_LEVEL
3.	EXPLORE_LEVEL
4.	TRADE_LEVEL
5. 	RACE
6.	PROFESSION
7.	HULL_LEVEL
8.	FACTION_REQUIRED
9.	ITEM_REQUIRED

Explanation:

1.	OVERALL_LEVEL
2.	COMBAT_LEVEL
3.	EXPLORE_LEVEL
4.	TRADE_LEVEL

These are self explanatory really. You need to have at least the level listed to be able to be given the mission.
You can use as many as you need. EG

<Condition ID="1">50</Condition>
<Condition ID="2">10</Condition>
<Condition ID="3">25</Condition>
<Condition ID="4">10</Condition>

To get the above mission, you need to be overall level 50 or above, and also have at least level 10 in combat,
level 25 explore and level 10 trade.

5. 	RACE
6.	PROFESSION

These two are also pretty simple. The race and profession values are:

Terran: 0
Jenquai:1
Progen: 2

and

Warrior :0
Trader	:1
Explorer:2

The race and profession conditions can be used together, so for example we can have a mission for terran and progen only,
or a mission for terran explorer and terran trader only.

EG

<Condition ID="5">0</Condition>
<Condition ID="5">1</Condition>

This mission would be for terran and jenquai only.

<Condition ID="5">0</Condition>
<Condition ID="6">1</Condition>

This mission would be terran trader only.

<Condition ID="6">0</Condition>
<Condition ID="6">2</Condition>

This mission is warriors and explorers only.


7.	HULL_LEVEL

To get this mission you have to have a specific hull upgrade level.

8.	FACTION_REQUIRED

To get this mission you need to have a minimum of a certain faction eg:

<Condition ID="8" Flags="1">-500</Condition>

For this mission you'd need to have at least -500 faction with whatever faction 1 is in the faction DB.

9.	ITEM_REQUIRED

<Condition ID="9" Flags="3009">2</Condition>

This means before you start the mission, you need to posess quantity 2 of item #3009.

That concludes the current entry conditions (we can easily add more).


Now we begin describing the mission stages. Stage 0 is special, in that it never appears in the mission log, it's the acceptance stage.
So currently this stage always has a talk tree, as at the moment that's the only way of accepting a mission.

add a stage tag:

	<Stage ID="0">

and next the stage description (don't need this for stage 0, but it won't hurt the server to add one):
	<Description>Find a cider addled belter</Description>

This stage description appears on the PDA, under the mission name/description. It changes each time you progress through the mission.

now each stage has completion requirements. This is where we specify which NPC you need to talk to, what MOB you need to kill,
what items to obtain/possess and so on.

The stage completion requirement nodes are:

1.	TALK_NPC		- Takes the NPC ID that you want the talk tree to apply to
					<Completion ID="1">341</Completion> : triggers if you talk to NPC 341

2.	GIVE_ITEM_NPC	- Checks you have an item, and removes it when the stage completes
					<Completion ID="2" Count="2">3009</Completion>: check you have 2 of item 3009, and gives them to NPC

3.	RECEIVE_ITEM_NPC- Checks you have enough cargo space to receive an item from NPC
					<Completion ID="3">1</Completion>: need to have one slot free to progress

4.	FIGHT_MOB		- Triggers if you have killed a certain MOB type:
					<Completion ID="4" Count="2">20</Completion>: need to kill 2 of MOB type 20

5.	SECTOR			- Triggers if you're in the specified sector:
					<Completion ID="5">1015</Completion>: need to be in sector 1015 (Luna)

6.	NEAREST_NAV		- Triggers if the nearest nav is the one specified:
					<Completion ID="6">250</Completion>: the nearest nav should be sector object database item 250

7.	TAKE_ITEM_TO_LOCATION - Need to have a certain object in a certain location:
					<Completion ID="7" Data="3009">250</Completion>
					Need to have item 3009 when you're near to sector object 250 to trigger

8.	OBTAIN_ITEMS	- Need to mine/loot an item to trigger
					<Completion ID="8">3009</Completion>
					Need to mine or loot item 3009 for this condition to trigger

9.	OBTAIN_ITEMS_AT_LOCATION - same as above but with a location
					<Completion ID="9" Data="3009">250</Completion>
					Need to mine/loot item 3009 near to sector object #250

10.	PERFORM_ACTION_AT_LOCATION - not sure how to use this/or if it would be useful?
11.	PERFORM_ACTION_ON_OBJECT   - not sure how to use this/or if it would be useful?

12.	SCAN_OBJECT		- Scan object database ID
					<Completion ID="12">250</Completion>
					Scan object database id 250.

13.	POSSESS_ITEM	- Need to have an item in your possession for this to trigger
					<Completion ID="13" Count="2">3009</Completion>
					Need to have two of item #3009 in your hold to advance

14. POSSESS_CREDITS - Need to have a minimum of credit specified. This credit will be removed on stage completion.
					<Completion ID="14">5000</Completion>
					Need to have 5000 credits. This is taken on stage completion


You can use multiple requirements together eg:

<Completion ID="1">341</Completion>
<Completion ID="13" Count="5">3020</Completion>
<Completion ID="13" Count="4">3009</Completion>

I need to talk to NPC ID 341, having at least 5 of item 3020 and 4 of 3009 in my hold.

Then comes the talk tree. This is identical to the standard talk tree, but with a couple of different flags:

1.	NODE_TRADE = 1			//node gives access to trade
2.	NODE_POSTPONE_MISSION	//allows you to drop the mission to continue later (or repeat it)
3.	NODE_DROP_MISSION		//means you won't be able to do the mission again
4.	NODE_MORE				//'more' dialogue box
5.	NODE_MISSION_GOTO		//this indicates that we need to branch to another mission node (set as Destination="x").
6.	NODE_MISSION_COMPLETE	//mission is now complete, you can't do it again


eg

    <Tree Node="3">
      <Text>/noway Bah! You had your chance and you blew it, damn @race.</Text>
      <Flags>3</Flags>   - indicate that the mission is dropped and you can't do it again
    </Tree>
    <Tree Node="4">
      <Text>/point Thankyou! The next waters are in Io Sector, near the Tempest Fringe and in Europa, near The Hunting Grounds.</Text>
      <Flags Data="3">5</Flags>	- indicate stage is complete, and next stage is stage '3'.
    </Tree>

<Flags>1</Flags> is the trade node indicator.

if you use 'MORE' this is a branch goto, and you need to specify the desired talk tree branch (same as you do with the <Branch ...> tags:

    <Tree Node="1">
      <Text>/noway Hello, it's @aprofession. How nice</Text>
      <Flags Data="2">4</Flags> - display 'more' and branch to node '2' when clicked
    </Tree>
    <Tree Node="2">
      <Text>/shrug Yeah I was wondering, can you do something for me?</Text>
      ...
    </Tree>

Finally we have reward nodes. These are a list of everything you get when you complete the stage.
You can have rewards at every stage if you want, or just rewards at the final stage.

1.	CREDITS
2.	EXPLORE_XP
3.	COMBAT_XP
4.	TRADE_XP
5.	FACTION
6.	ITEM_ID
7.	HULL_UPGRADE
8.	RUN_SCRIPT

example:

    <Reward ID="1">1000</Reward>
    <Reward ID="2">500</Reward>
    <Reward ID="4">500</Reward>
    <Reward ID="5" Flags="2">1000</Reward>

This will give the player 1000 credits, 500 explore XP, 500 trade XP and 1000 faction points for faction 2.

	<Reward ID="6">3009</Reward>

This would give the player item 3009.

RUN_SCRIPT doesn't do anything yet - I'm not sure how it will work yet - although it would cause a script to be executed.
This could be used for example to re-create the wormhole mission where the "lunchbox" explodes in your ship.

Hopefully this should explain how the current mission system works.
There could be a few changes; in writing this up I noticed that the talk tree flags are handled in a different way to the other flags.

Also, please feel free to suggest any other flag types which I've almost certainly missed.



ADVANCED:

I'm currently doing 'parallel' stages, so a player can complete a mission in different ways.

eg:

  <Stage ID="5" TwinStageID="6" GotoStageID="7">
    <Description>Obtain some hydrogen gas in Io, Sol</Description>
    <Completion ID="5">1052</Completion>
    <Completion ID="8">xxxx</Completion>
  </Stage>
  <Stage ID="6" TwinStageID="5" GotoStageID="7">
    <Description>Obtain a Porata scale in Europa, Sol</Description>
    <Completion ID="5">1055</Completion>
    <Completion ID="8">yyyy</Completion>
  </Stage>

Then a player can either hunt the porata or mine the gas. Or you can use the 'GotoStageID' to create different mission paths based on the talk tree responses.*/

