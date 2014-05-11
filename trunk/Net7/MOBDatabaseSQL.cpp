// MOBDatabaseSQL.cpp

#include "Net7.h"

#ifdef USE_MYSQL_SECTOR

#include "MOBDatabase.h"
#include "mysql/mysqlplus.h"
#include "StringManager.h"


MOBContent::MOBContent()
{
    m_highest_id = 0;
    m_MOB.clear();
	m_updating = false;
}

MOBContent::~MOBContent()
{
    m_MOB.clear();
}

bool MOBContent::LoadMOBContent()
{
    long current_mob_id;
    MOBData *current_mob;
	LootNode *current_loot;
	char QueryString[1024];

	if (m_updating) return false;

	m_updating = true;

	if(!g_MySQL_User || !g_MySQL_Pass) 
    {
		LogMessage("You need to set a mysql user/pass in the net7.cfg\n");
		return false;
	}

	sql_connection_c connection( "net7", g_MySQL_Host, g_MySQL_User, g_MySQL_Pass);
	sql_query_c MOBTable( &connection );
    sql_result_c result;
	sql_result_c *mob_result = &result;

    sprintf(QueryString, "SELECT * FROM `mob_base`");

    if ( !MOBTable.execute( QueryString ) )
    {
        LogMessage( "MySQL Login error/Database error: (User: %s Pass: %s)\n", g_MySQL_User, g_MySQL_Pass );
        return false;
    }
    
    MOBTable.store(mob_result);
    
    if (!mob_result->n_rows() || !mob_result->n_fields()) 
    {
        LogMessage("Error loading rows/fields\n");
        return false;
    }
    
    LogMessage("Loading MOBs from SQL (%d)\n", (int)mob_result->n_rows());
    
	sql_row_c MOBSQLData;
	for(int x=0;x<mob_result->n_rows();x++)
	{
		mob_result->fetch_row(&MOBSQLData);
        current_mob_id = (long)MOBSQLData["mob_id"];
		current_mob = m_MOB[current_mob_id];
		if (!current_mob)
		{
			current_mob = new MOBData;
		}

        current_mob->m_Name = g_StringMgr->GetStr((char*)MOBSQLData["name"]);
        current_mob->m_Level = (int)MOBSQLData["level"];
        current_mob->m_Type = (int)MOBSQLData["type"];
        current_mob->m_Agressiveness = (int)MOBSQLData["aggressiveness"];
        current_mob->m_Basset = (int)MOBSQLData["base_asset_id"];
        current_mob->m_Altruism = (int)MOBSQLData["altruism"];
        current_mob->m_FactionID = (int)MOBSQLData["faction_id"];
        current_mob->m_Bravery = (int)MOBSQLData["bravery"];
        current_mob->m_Intelligence = (int)MOBSQLData["intelligence"];
        current_mob->m_Scale = (float)MOBSQLData["scale"];
        current_mob->m_HSV[0]= (float)MOBSQLData["h"];
        current_mob->m_HSV[1]= (float)0.0f;//MOBSQLData["s"]; //don't use these for now - cause bad effects.
        current_mob->m_HSV[2]= (float)0.0f;//MOBSQLData["v"];

        //patch MOB basset if not yet set
        if (current_mob->m_Basset == 65535)
        {
            current_mob->m_Basset = 1134; //set as evil mushroom
        }

        if (current_mob->m_Scale == 0.0f)
        {
            current_mob->m_Scale = 1.0f;
        }

        m_MOB[current_mob_id] = current_mob; //add to MOB map

        if (current_mob_id > m_highest_id)
        {
            m_highest_id = current_mob_id;
        }
		
		if (current_mob->m_Loot.size() != 0) //invalidate any loot
		{
			for (u32 i = 0; i < current_mob->m_Loot.size(); i++)
			{
				current_mob->m_Loot[i]->item_base_id = -1;
			}
		}
    }

	//now read in any Loot
    sprintf(QueryString, "SELECT * FROM `mob_items`");

    if ( !MOBTable.execute( QueryString ) )
    {
        LogMessage( "MySQL Login error/Database error: (User: %s Pass: %s)\n", g_MySQL_User, g_MySQL_Pass );
        return false;
    }
    
    MOBTable.store(mob_result);
    
    if (!mob_result->n_rows() || !mob_result->n_fields()) 
    {
        LogMessage("Error loading rows/fields for MOB items\n");
        return false;
    }

	u32 current_mob_loot_index = 0;
	long previous_mob_id = -1;
        
	for(u32 x=0;x<mob_result->n_rows();x++)
	{
		mob_result->fetch_row(&MOBSQLData);
		current_mob_id = (long)MOBSQLData["mob_id"];
		current_mob = m_MOB[current_mob_id];

		if (current_mob)
		{
			current_loot = 0;
			//find first available entry on list or create new entry
			if (current_mob->m_Loot.size() != 0)
			{
				for (u32 i = 0; i < current_mob->m_Loot.size(); i++)
				{
					if (current_mob->m_Loot[i]->item_base_id == -1)
					{
						current_loot = current_mob->m_Loot[i];
						break;
					}
				}
			}

			if (current_loot == 0)
			{
				//need to add new loot node
				current_loot = new LootNode;
				current_mob->m_Loot.push_back(current_loot);
			}

			current_loot->item_base_id= (long)MOBSQLData["item_base_id"];
			float drop_chance = (float)MOBSQLData["drop_chance"];
			float usage_chance= (float)MOBSQLData["usage_chance"];
			long quantity	 = (long)MOBSQLData["qty"];
			long type		 = (long)MOBSQLData["type"];

			current_loot->drop_chance = (unsigned char)drop_chance;
			current_loot->usage_chance= (unsigned char)usage_chance;
			current_loot->quantity	  = (unsigned char)quantity;
			current_loot->type		  = (unsigned char)type;
		}
	}

	m_updating = false;

    return true;
}

MOBData * MOBContent::GetMOBData(long MOB_id)
{
    return (m_MOB[MOB_id]);
}

MOBData * MOBContent::GetMOBDataFromBasset(short MOB_basset)
{
	MOBList::const_iterator itrMOB;
	MOBData *mob_data = 0;

	for(itrMOB = m_MOB.begin(); itrMOB != m_MOB.end(); ++itrMOB)
	{
		if ((itrMOB->second) && itrMOB->second->m_Basset == MOB_basset)
		{
			mob_data = itrMOB->second;
			break;
		}
	}

	return mob_data;
}

long MOBContent::GetMOBCount()
{
    return (m_highest_id);
}

#endif