// TalkTreeParser.h

#ifndef _TALK_TREE_PARSER_H_INCLUDED_
#define _TALK_TREE_PARSER_H_INCLUDED_

#include "TalkTree.h"
#include "StringManager.h"

struct XMLNode;

class TalkTreeParser
{
private:
	TalkTree * m_TalkTrees;

public:
    TalkTreeParser()    {}
    ~TalkTreeParser()   {}

public:
	TalkTree * GetTalkTree();
    bool ParseTalkTree(TalkTree *tree, char *data);
	bool ParseMissions(MissionTree *tree, char *data);

private:
	void InnerTalkTreeParser(TalkTree *tree, XMLNode *TalkNode);
};


#endif
