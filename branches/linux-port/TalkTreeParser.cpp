// TalkTreeParser.cpp

#include "Net7.h"

#ifdef USE_MYSQL_STATIONS

#include "TalkTreeParser.h"
#include "StringManager.h"
#include "xmlParser/xmlParser.h"
#include "PlayerClass.h"

TalkTree * TalkTreeParser::GetTalkTree()
{
	return m_TalkTrees;
}

bool TalkTreeParser::ParseTalkTree(TalkTree *tree, const char *data)
{
    XMLNode TalkTreeBase;

    TalkTreeBase = XMLNode::parseString(data);

	InnerTalkTreeParser(tree, &TalkTreeBase);

    return true;
}

void TalkTreeParser::InnerTalkTreeParser(TalkTree *tree, XMLNode *TalkTreeBase)
{
	TalkNode * Current_Node = NULL;
	TalkBranch * Current_Branch = NULL;
	int NumNodes;
	int NumBranches;

    NumNodes = TalkTreeBase->nChildNode("Tree");

	XMLNode TNode;
	XMLNode TBranch;

	tree->NumNodes = NumNodes;
	tree->Nodes.clear();

	// Loop though all nodes
	for(int i=0;i<NumNodes;i++)
	{
		Current_Node = new TalkNode;

		if (Current_Node)
		{
			memset (Current_Node, 0, sizeof(TalkNode));
			TNode = TalkTreeBase->getChildNode("Tree",i);

			// Current Node Data
			Current_Node->Flags = (talk_type)intVal(TNode.getChildNode("Trade").getText());

			if (Current_Node->Flags == 0)
			{
				Current_Node->Flags = (talk_type)intVal(TNode.getChildNode("Flags").getText());
			}

			if (Current_Node->Flags > 0)
			{
				Current_Node->Destination = intVal(TNode.getChildNode("Flags").getAttribute("Data"));
			}

			Current_Node->NodeNumber = intVal(TNode.getAttribute("Node"));
			Current_Node->Text = g_StringMgr->GetStr(TNode.getChildNode("Text").getText()); //no need to reallocate

			// Save our Current Node
			tree->Nodes[Current_Node->NodeNumber] = Current_Node;

			NumBranches = TNode.nChildNode("Branch");

			Current_Node->NumBranches = NumBranches;

			// Loop though all nodes
			for(int j=0;j<NumBranches;j++)
			{
				Current_Branch = new TalkBranch;
				//memset(Current_Branch,0,sizeof(TalkBranches));

				if (Current_Branch)
				{
					Current_Node->Branches.push_back(Current_Branch);

					TBranch = TNode.getChildNode("Branch",j);
					Current_Branch->BranchDestination = intVal(TBranch.getAttribute("Node"));
					Current_Branch->Text = g_StringMgr->GetStr(TNode.getChildNode("Branch", j).getText());
				}
			}
		}
	}

}

bool TalkTreeParser::ParseMissions(MissionTree *tree, const char *data)
{
	TalkNode * Current_Node = (0);
	TalkBranch * Current_Branch = (0);
	_MissionNode *mNode = (0);

    XMLNode TalkTreeBase;
	XMLNode m_TalkNode;
	XMLNode TalkBranch;
	XMLNode MissionBranch;
	XMLNode TempBranch;

	int NumRestrictionNodes;
	int NumCompletionNodes;
	int NumRewardNodes;

	MissionBranch = XMLNode::parseString(data);

	tree->Nodes.clear();
	tree->NumNodes = 0;

	//MissionBranch = TalkTreeBase.getChildNode("Mission");

	//find name and summary of mission

	char *summary = (char*)MissionBranch.getChildNode("Summary").getText();

	LogMessage("Mission %d: %s %s\n", tree->MissionID, tree->name, summary);

	tree->summary = g_StringMgr->GetStr(summary);

	//process mission conditions (if any)
	NumRestrictionNodes = MissionBranch.nChildNode("Condition");

	for (int x=0;x<NumRestrictionNodes;x++)
	{
		RestrictionNode *rNode = new RestrictionNode;
		tree->restriction_list.push_back(rNode);
		TalkBranch = MissionBranch.getChildNode("Condition",x);
		rNode->type = (restrict_type)intVal(TalkBranch.getAttribute("ID"));
		rNode->flags = intVal(TalkBranch.getAttribute("Flags"));
		rNode->data = intVal(TalkBranch.getText());
	}

	//now process stages

	tree->NumNodes = MissionBranch.nChildNode("Stage");

	for (int x=0;x<tree->NumNodes;x++)
	{
		_MissionNode *this_node;

		if (tree->Nodes[x] == 0)
		{
			this_node = new _MissionNode;
			tree->Nodes[x] = this_node;
		}
		else
		{
			this_node = tree->Nodes[x];
		}

		//now read talktree (if any)
		TalkBranch = MissionBranch.getChildNode("Stage",x);

		// Get description
		char *description = (char*)TalkBranch.getChildNode("Description").getText();
		this_node->description = g_StringMgr->GetStr(description);

		InnerTalkTreeParser(&this_node->talk_tree, &TalkBranch);

		NumCompletionNodes = TalkBranch.nChildNode("Completion");
		this_node->completion_list.clear();
		this_node->rewards.clear();

		for (int x=0;x<NumCompletionNodes;x++)
		{
			CompletionNode *cNode = new CompletionNode;
			this_node->completion_list.push_back(cNode);
			TempBranch = TalkBranch.getChildNode("Completion",x);

			cNode->type = (completion_node_type)intVal(TempBranch.getAttribute("ID"));
			cNode->data = intVal(TempBranch.getText());
			cNode->count = intVal(TempBranch.getAttribute("Count"));
			if (cNode->count == 0) cNode->count = intVal(TempBranch.getAttribute("Data"));
			if (cNode->count == 0) cNode->count = 1;
		}

		NumRewardNodes = TalkBranch.nChildNode("Reward");

		for (int x=0;x<NumRewardNodes;x++)
		{
			RewardNode *rNode = new RewardNode;
			this_node->rewards.push_back(rNode);
			TempBranch = TalkBranch.getChildNode("Reward",x);

			rNode->type = (reward_type)intVal(TempBranch.getAttribute("ID"));
			rNode->data = intVal(TempBranch.getText());
			rNode->flags = intVal(TempBranch.getAttribute("Flags"));
		}
	}

	return true;
}

#endif
