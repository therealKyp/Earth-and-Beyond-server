// MOBDatabaseSQL.cpp

#include "Net7.h"

#ifdef USE_MYSQL_SECTOR

#include "SkillsDatabase.h"
#include "mysql/mysqlplus.h"
#include "StringManager.h"


SkillsContent::SkillsContent()
{
    m_highest_id = 0;
    m_SkillConvList.clear();
	m_updating = false;
}

SkillsContent::~SkillsContent()
{
    m_SkillConvList.clear();
}

bool SkillsContent::LoadSkillsContent()
{
    long current_skill_id;
    SkillConversion *current_skill;
	char QueryString[1024];

	if (m_updating) return false;

	m_updating = true;

	if(!g_MySQL_User || !g_MySQL_Pass) 
    {
		LogMessage("You need to set a mysql user/pass in the net7.cfg\n");
		return false;
	}

	sql_connection_c connection( "net7", g_MySQL_Host, g_MySQL_User, g_MySQL_Pass);
	sql_query_c SkillsTable( &connection );
    sql_result_c result;
	sql_result_c *skill_result = &result;

    sprintf(QueryString, "SELECT * FROM `skill_levels`");

    if ( !SkillsTable.execute( QueryString ) )
    {
        LogMessage( "MySQL Login error/Database error: (User: %s Pass: %s)\n", g_MySQL_User, g_MySQL_Pass );
        return false;
    }
    
    SkillsTable.store(skill_result);
    
    if (!skill_result->n_rows() || !skill_result->n_fields()) 
    {
        LogMessage("Error loading rows/fields\n");
        return false;
    }
    
    LogMessage("Loading Skill Levels from SQL (%d)\n", (int)skill_result->n_rows());
    
	sql_row_c SkillSQLData;
	for(int x=0;x<skill_result->n_rows();x++)
	{
		skill_result->fetch_row(&SkillSQLData);
        current_skill_id = (long)SkillSQLData["skill_level_id"];
		current_skill = m_SkillConvList[current_skill_id];
		if (!current_skill)
		{
			current_skill = new SkillConversion;
		}

        current_skill->m_Description = g_StringMgr->GetStr((char*)SkillSQLData["description"]);
        current_skill->m_Level = (int)SkillSQLData["level"];
        current_skill->m_BaseSkillID = (int)SkillSQLData["skill_id"];

        m_SkillConvList[current_skill_id] = current_skill; //add to MOB map

        if (current_skill_id > m_highest_id)
        {
            m_highest_id = current_skill_id;
        }
    }

	m_updating = false;

    return true;
}

long SkillsContent::GetSkillLevel(long skill_id)
{
	if (m_SkillConvList[skill_id])
	{
		return m_SkillConvList[skill_id]->m_Level;
	}
	else
	{
		return 0;
	}
}

long SkillsContent::GetBaseSkillID(long skill_id)
{
	if (m_SkillConvList[skill_id])
	{
		return m_SkillConvList[skill_id]->m_BaseSkillID;
	}
	else
	{
		return 0;
	}
}

char* SkillsContent::GetSkillDescription(long skill_id)
{
	if (m_SkillConvList[skill_id])
	{
		return m_SkillConvList[skill_id]->m_Description;
	}
	else
	{
		return 0;
	}
}

#endif