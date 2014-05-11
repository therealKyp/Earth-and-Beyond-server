// FactionDataSQL.cpp

#include "Net7.h"

#ifdef USE_MYSQL_SECTOR

#include "FactionDataSQL.h"
#include "mysql/mysqlplus.h"
#include "StringManager.h"


Factions::Factions()
{
    m_factions.clear();
}

Factions::~Factions()
{
    m_factions.clear();
}

bool Factions::LoadFactions()
{
    long current_faction_id;
    FactionData *current_faction;
	char QueryString[1024];
	long faction_order = 0;

	m_faction_count = 0;

	if(!g_MySQL_User || !g_MySQL_Pass) 
    {
		LogMessage("You need to set a mysql user/pass in the net7.cfg\n");
		return false;
	}

	sql_connection_c connection( "net7", g_MySQL_Host, g_MySQL_User, g_MySQL_Pass);
	sql_query_c FactionTable( &connection );
    sql_result_c result;
	sql_result_c *faction_result = &result;
	sql_result_c matrix;

    sprintf(QueryString, "SELECT * FROM `factions`");

    if ( !FactionTable.execute( QueryString ) )
    {
        LogMessage( "MySQL Login error/Database error: (User: %s Pass: %s)\n", g_MySQL_User, g_MySQL_Pass );
        return false;
    }
    
    FactionTable.store(faction_result);
    
    if (!faction_result->n_rows() || !faction_result->n_fields()) 
    {
        LogMessage("Error loading rows/fields\n");
        return false;
    }
    
    LogMessage("Loading Factions from SQL (%d)\n", (int)faction_result->n_rows());
    
	sql_row_c FactionSQLData;
	for(int x=0;x<faction_result->n_rows();x++)
	{
		faction_result->fetch_row(&FactionSQLData);
        current_faction_id = (long)FactionSQLData["faction_id"];
		current_faction = m_factions[current_faction_id];
		if (!current_faction)
		{
			current_faction = new FactionData;
			m_factions[current_faction_id] = current_faction;
		}

		m_faction_count++;

		current_faction->m_name = g_StringMgr->GetStr((char*)FactionSQLData["name"]);
		current_faction->m_description = 0; //Not sure we need to load this. If we do: g_StringMgr->GetStr((char*)FactionSQLData["description"]);
		current_faction->m_player_PDA = (int)FactionSQLData["player_PDA"] == 1 ? true : false;
		if (current_faction->m_player_PDA)
		{
			current_faction->m_PDA_text = g_StringMgr->GetStr((char*)FactionSQLData["PDA_text"]);
			current_faction->m_faction_order = faction_order;
			faction_order++;
		}
		else
		{
			current_faction->m_PDA_text = 0;
			current_faction->m_faction_order = -1;
		}

		//OK now load in the faction relationships for this faction
		sprintf(QueryString, "SELECT * FROM `faction_matrix` WHERE faction_id = '%d'", current_faction_id);

		sql_query_c Faction_matrix( &connection );
        sql_result_c *matrix_result = &matrix;
        
		if ( !Faction_matrix.execute( QueryString ) )
		{
			printf( "MySQL Error (Faction Matrix)\n" );
			return 0;
		}

		Faction_matrix.store(matrix_result);		

		sql_row_c MatrixSQLData;
		for(int y=0;y<matrix_result->n_rows();y++)
		{
			matrix_result->fetch_row(&MatrixSQLData);	// Read in row
			float faction_value = (float)MatrixSQLData["base_value"];
			long faction_entry_id = (int)MatrixSQLData["faction_entry_id"];

			current_faction->m_value[faction_entry_id] = faction_value;
		}
	}

    return true;
}

float Factions::GetFactionStanding(long faction_id, long faction_ref)
{
	if (m_factions[faction_id])
	{
		return m_factions[faction_id]->m_value[faction_ref];
	}
	else
	{
		return 0.0f;
	}
}

char* Factions::GetFactionName(long faction_id)
{
	if (m_factions[faction_id])
	{
		return m_factions[faction_id]->m_name;
	}
	else
	{
		return 0;
	}
}

char* Factions::GetFactionPDAName(long faction_id)
{
	if (m_factions[faction_id])
	{
		return m_factions[faction_id]->m_PDA_text;
	}
	else
	{
		return 0;
	}
}

bool Factions::GetPDA(long faction_id)
{
	if (m_factions[faction_id])
	{
		return m_factions[faction_id]->m_player_PDA;
	}
	else
	{
		return false;
	}
}

long Factions::GetFactionOrderFromID(long faction_id)
{
	if (m_factions[faction_id])
	{
		return m_factions[faction_id]->m_faction_order;
	}
	else
	{
		return -1;
	}
}

FactionData * Factions::GetFactionData(long faction_id)
{
    return (m_factions[faction_id]);
}

long Factions::GetFactionCount()
{
    return (m_faction_count);
}

#endif