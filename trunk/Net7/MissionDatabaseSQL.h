// MissionDatabaseSQL.h

#ifndef _MISSIONDATABASE_SQL_H_INCLUDED_
#define _MISSIONDATABASE_SQL_H_INCLUDED_

#pragma warning(disable:4786) 

#include <map>
#include <vector>
#include "TalkTreeParser.h"

// forward references
class sql_connection_c;
struct MissionTree;

typedef std::map<unsigned long, MissionTree*> _MissionList;
typedef std::map<unsigned long, bool> _StartNPCs;

class MissionHandler
{
// Constructor/Destructor
public:
    MissionHandler();
	virtual ~MissionHandler();

// Public Methods
public:
    bool        LoadMissionContent();
    MissionTree *   GetMissionTree(long mission_id);
    void        ReloadMissionData(long mission_id);
	long		GetMissionCount();
	char		*	GetMissionName(long mission_id);
	long		GetNextMissionID(long mission_id);
	char		*	GetStageDescription(long mission_id, long stage);
	long		GetHighestID()				{ return m_highest_id; }
	bool		GetMissionStartNPC(long NPC_id);
	void		SetMissionStartNPC(long NPC_id);

	_MissionList*	GetMissionList()		{ return &m_Missions; }

// Private Methods
private:

// Private Member Attributes
private:
    _MissionList	m_Missions;
    long			m_highest_id;
	TalkTreeParser	m_TalkTreeParser;
	_StartNPCs		m_StarterNPCs;

};

#endif // _MISSIONDATABASE_SQL_H_INCLUDED_

