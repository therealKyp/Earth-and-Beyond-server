// AssetDatabaseSQL.cpp

#include "Net7.h"

#ifdef USE_MYSQL_SECTOR

#include "AssetDatabase.h"
#include "mysql/mysqlplus.h"
#include "StringManager.h"


AssetContent::AssetContent()
{
    m_highest_id = 0;
    m_Asset.clear();
}

AssetContent::~AssetContent()
{
    m_Asset.clear();
}

bool AssetContent::LoadAssetContent()
{
    long current_asset_id;
    AssetData *current_asset;
	char QueryString[1024];

	if(!g_MySQL_User || !g_MySQL_Pass) 
    {
		printf("You need to set a mysql user/pass in the net7.cfg\n");
		return false;
	}

	sql_connection_c connection( "net7", g_MySQL_Host, g_MySQL_User, g_MySQL_Pass);
	sql_query_c AssetTable( &connection );
    sql_result_c result;
	sql_result_c *asset_result = &result;

    sprintf(QueryString, "SELECT * FROM `assets`");

    if ( !AssetTable.execute( QueryString ) )
    {
        LogMessage( "MySQL Login error/Database error: (User: %s Pass: %s)\n", g_MySQL_User, g_MySQL_Pass );
        return false;
    }
    
    AssetTable.store(asset_result);
    
    if (!asset_result->n_rows() || !asset_result->n_fields()) 
    {
        LogMessage("Error loading rows/fields\n");
        return false;
    }
    
    LogMessage("Loading Assets from SQL (%d)\n", (int)asset_result->n_rows());
    
	sql_row_c AssetSQLData;
	for(int x=0;x<asset_result->n_rows();x++)
	{
		asset_result->fetch_row(&AssetSQLData);
        current_asset_id = (long)AssetSQLData["base_id"];
		char *name = (char*)AssetSQLData["descr"];

		if (stricmp(name, "NULL") != 0)
		{
			current_asset = new AssetData;

			current_asset->m_Name = g_StringMgr->GetStr(name);
			current_asset->m_CatName = g_StringMgr->GetStr((char*)AssetSQLData["main_cat"]);
			current_asset->m_SubCatName = g_StringMgr->GetStr((char*)AssetSQLData["sub_cat"]);

			m_Asset[current_asset_id] = current_asset; //add to asset map

			if (current_asset_id > m_highest_id)
			{
				m_highest_id = current_asset_id;
			}
		}
    }
    return true;
}

AssetData * AssetContent::GetAssetData(long asset_id)
{
    return (m_Asset[asset_id]);
}

long AssetContent::GetAssetCount()
{
    return (m_highest_id);
}

#endif