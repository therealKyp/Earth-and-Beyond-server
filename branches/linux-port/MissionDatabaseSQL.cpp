// MissionDatabaseSQL.cpp

#include "Net7.h"

#ifdef USE_MYSQL_SECTOR

#include "MissionDatabaseSQL.h"
#include "mysql/mysqlplus.h"
#include "StringManager.h"
#include "TalkTree.h"


MissionHandler::MissionHandler()
{
    m_highest_id = 0;
    m_Missions.clear();
}

MissionHandler::~MissionHandler()
{
    m_Missions.clear();
}

bool MissionHandler::LoadMissionContent()
{
    long current_mission_id;
    MissionTree *current_mission;
    const char *current_mission_name;
	char QueryString[1024];

	if(!g_MySQL_User || !g_MySQL_Pass)
    {
		LogMessage("You need to set a mysql user/pass in the net7.cfg\n");
		return false;
	}

	sql_connection_c connection( "net7", g_MySQL_Host, g_MySQL_User, g_MySQL_Pass);
	sql_query_c MissionTable( &connection );
    sql_result_c result;
	sql_result_c *mission_result = &result;

    sprintf(QueryString, "SELECT * FROM `missions`");

    if ( !MissionTable.execute( QueryString ) )
    {
        LogMessage( "MySQL Login error/Database error: (User: %s Pass: %s)\n", g_MySQL_User, g_MySQL_Pass );
        return false;
    }

    MissionTable.store(mission_result);

    if (!mission_result->n_rows() || !mission_result->n_fields())
    {
        LogMessage("Error loading rows/fields\n");
        return false;
    }

    LogMessage("Loading Missions from SQL (%d)\n", (int)mission_result->n_rows());
	m_Missions.clear();

	sql_row_c MissionSQLData;
	const char *mission_xml;
	for(int x=0;x<mission_result->n_rows();x++)
	{
		mission_result->fetch_row(&MissionSQLData);
        current_mission_id = (long)MissionSQLData["mission_id"];
		mission_xml = MissionSQLData["mission_XML"];
		current_mission_name = MissionSQLData["mission_name"];

		current_mission = m_Missions[current_mission_id];

		if (!current_mission)
		{
			current_mission = new MissionTree;
		}
		else
		{
			delete[] (current_mission->name);
		}

		current_mission->MissionID = current_mission_id;

		current_mission->name = new char[strlen(current_mission_name) + 1];

		strcpy(current_mission->name, current_mission_name);

		m_TalkTreeParser.ParseMissions(current_mission, mission_xml);

        m_Missions[current_mission_id] = current_mission; //add to Missions map

        if (current_mission_id > m_highest_id)
        {
            m_highest_id = current_mission_id;
        }
    }
    return true;
}

MissionTree * MissionHandler::GetMissionTree(long mission_id)
{
    return (m_Missions[mission_id]);
}

long MissionHandler::GetMissionCount()
{
    return (m_highest_id);
}

char * MissionHandler::GetMissionName(long mission_id)
{
	MissionTree *mission = m_Missions[mission_id];
	char *name = (0);

	if (mission)
	{
		name = mission->name;
	}

	return name;
}

char * MissionHandler::GetStageDescription(long mission_id, long stage)
{
	MissionTree *mission = m_Missions[mission_id];
	char *description = (0);

	if (mission && mission->Nodes[stage])
	{
		description = mission->Nodes[stage]->description;
	}

	return description;
}

#endif
