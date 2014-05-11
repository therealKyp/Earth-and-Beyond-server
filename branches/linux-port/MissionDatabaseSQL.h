// MissionDatabaseSQL.h

#ifndef _MISSIONDATABASE_SQL_H_INCLUDED_
#define _MISSIONDATABASE_SQL_H_INCLUDED_

#ifdef WIN32
#pragma warning(disable:4786)
#endif

#include <map>
#include <vector>
#include "TalkTreeParser.h"

// forward references
class sql_connection_c;
struct MissionTree;

typedef std::map<unsigned long, MissionTree*> _MissionList;

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

	_MissionList*	GetMissionList()		{ return &m_Missions; }

// Private Methods
private:

// Private Member Attributes
private:
    _MissionList	m_Missions;
    long			m_highest_id;
	TalkTreeParser	m_TalkTreeParser;

};

#endif // _MISSIONDATABASE_SQL_H_INCLUDED_

