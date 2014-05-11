// AccountManager.cpp
//

//To make things easier, the top of this file will be SQL methods
//and the bottom of the file will be flatfile methods

//TODO: Use column indexes instead of names for SQL

#include "AccountManager.h"
#include "ServerManager.h"
#include "StringManager.h"
#include "StaticData.h"
#include <time.h>
#include "mysql/mysqlplus.h"
#include "SaveManager.h"

#ifdef USE_MYSQL_ACCOUNT_DATA
sql_connection_c m_SQL_Conn;
#endif

AccountManager::AccountManager()
{
    SetupTickets();
#ifdef USE_MYSQL_ACCOUNT_DATA
	m_SQL_Conn.connect("net7_user", g_MySQL_Host, g_MySQL_User, g_MySQL_Pass);
#else
    m_NumAccounts = 0;
	LoadAccounts();
#endif
}

AccountManager::~AccountManager()
{
#ifdef USE_MYSQL_ACCOUNT_DATA
    m_SQL_Conn.disconnect();
#endif

    AccountTicket * ticket;
    while (m_Tickets)
    {
        ticket = m_Tickets->next;
        delete m_Tickets;
        m_Tickets = ticket;
    }
}

void AccountManager::SetupTickets()
{
    //Allocate 5 structures for storing tickets
    //This is dirty, but the easiest way to do it since they are linked lists
    m_Tickets = new AccountTicket();
    memset(m_Tickets, 0, sizeof(AccountTicket));

    m_Tickets->next = new AccountTicket();
    memset(m_Tickets->next, 0, sizeof(AccountTicket));

    m_Tickets->next->next = new AccountTicket();
    memset(m_Tickets->next->next, 0, sizeof(AccountTicket));

    m_Tickets->next->next->next = new AccountTicket();
    memset(m_Tickets->next->next->next, 0, sizeof(AccountTicket));

    m_Tickets->next->next->next->next = new AccountTicket();
    memset(m_Tickets->next->next->next->next, 0, sizeof(AccountTicket));
}

#ifdef USE_MYSQL_ACCOUNT_DATA

    void AccountManager::UpdateTime(long account_id)
    {
	    char queryString[256];
	    sql_query_c TimeUpdate(&m_SQL_Conn);

	    char timestr[32];
	    time_t rawtime;
	    time(&rawtime);
	    strftime(timestr, sizeof(timestr), "%d/%m/%y %H:%M:%S", localtime(&rawtime));

	    sprintf(queryString, "UPDATE `accounts` SET `last_login` = '%s' WHERE `id` = '%d'", timestr, account_id);
	    TimeUpdate.run_query(queryString);
    }

    long AccountManager::GetAccountStatus(char *username)
    {
        char query_str[256];
	    sql_query_c account_query (&m_SQL_Conn);
	    sql_result_c account_result;      
        sql_row_c account_row;
    
        sprintf(query_str, "SELECT `status` FROM `accounts` WHERE `username` = '%s'", username);

        if (!account_query.run_query(query_str) || account_query.n_rows() == 0)
        {
            return -2;
        }

	    account_query.store(&account_result);
        account_result.fetch_row(&account_row);

        return account_row[0];
    }

    long AccountManager::GetAccountID(char *username)
    {
        char query_str[256];
	    sql_query_c account_query(&m_SQL_Conn);
	    sql_result_c account_result;        
        sql_row_c account_row;
    
        sprintf(query_str, "SELECT `id` FROM `accounts` WHERE `username` = '%s'", username);

        if (!account_query.run_query(query_str) || account_query.n_rows() == 0)
        {
            return -1;
        }

	    account_query.store(&account_result);
        account_result.fetch_row(&account_row);

        return account_row[0];
    }

    //Validates the username/password set, returns the account if valid
    long AccountManager::ValidateAccount(char *username, char *password)
    {
	    char query_str[256];
	    sql_query_c account_query(&m_SQL_Conn);
	    sql_result_c account_result;      
        sql_row_c account_row;

        sprintf(query_str, "SELECT `id` FROM `accounts` WHERE `username` = '%s' AND `password` = '%s'", username, password);

        if (!account_query.run_query(query_str) || account_query.n_rows() == 0)
        {
            return -1;
        }

	    account_query.store(&account_result);
        account_result.fetch_row(&account_row);

        return account_row[0];
    }

    bool AccountManager::AddUser(char *username, char *password, char *access)
    {
        LogMessage("Adding new user `%s`\n", username);

	    sql_query_c account_query(&m_SQL_Conn);
        sql_query account_buider;

        account_buider.SetTable("accounts");
        account_buider.AddData("username", username);
        account_buider.AddData("password", password);
        account_buider.AddData("status", access);
        account_buider.AddData("last_login", "never");
    
        if (!account_query.run_query(account_buider.CreateQuery()))
        {
            return false;
        }
    
        return true;
    }

    bool AccountManager::SetAccountStatus(char *username, long status)
    {
	    LogMessage("Changing access to (%d) for `%s`\n", status, username);

        char query_str[256];
	    sql_query_c account_query(&m_SQL_Conn);

	    sprintf(query_str, "UPDATE `accounts` SET `status`='%d' WHERE `username`='%s'", status, username);
        if (!account_query.run_query(query_str))
        {
            return false;
        }

        return true;
    }

    bool AccountManager::ChangePassword(char *username, char *password)
    {
	    LogMessage("Changing password on %s\n", username);

	    char query_str[256];
	    sql_query_c account_query(&m_SQL_Conn);

	    sprintf(query_str, "UPDATE `accounts` SET `password`='%s' WHERE `username`='%s'", password, username);
        if (!account_query.run_query(query_str))
        {
            return false;
        }

        return true;
    }

    bool AccountManager::IsUsernameUnique(char *name)
    {
	    char query_str[256];
	    sql_query_c account_query (&m_SQL_Conn);

	    sprintf(query_str, "SELECT * FROM `avatar_data` WHERE `first_name` =  '%s'", name);
	    if (account_query.run_query(query_str) && account_query.n_rows() > 0)
        {
            return false;
        }

	    return true;
    }

    void AccountManager::DeleteCharacter(long avatar_id)
    {
        char query_str[256];
	    sql_query_c account_query (&m_SQL_Conn);

	    //Delete this character from the tables
        sprintf(query_str, "DELETE FROM `ship_info` WHERE `avatar_id` = '%d'", avatar_id);
	    account_query.run_query(query_str);

	    sprintf(query_str, "DELETE FROM `ship_data` WHERE `avatar_id` = '%d'", avatar_id);
	    account_query.run_query(query_str);

	    sprintf(query_str, "DELETE FROM `avatar_data` WHERE `avatar_id` = '%d'", avatar_id);
	    account_query.run_query(query_str);

	    sprintf(query_str, "DELETE FROM `avatar_info` WHERE `avatar_id` = '%d'", avatar_id);
	    account_query.run_query(query_str);

		g_SaveMgr->AddSaveMessage(SAVE_CODE_FULL_CHARACTER_WIPE, avatar_id, 0, 0);

        //Now delete the Aux file
	    sprintf(query_str, "%sCharInfo-%d.chr", SERVER_USER_PATH, avatar_id);

        LogMessage("Deleting %s\n", query_str);
        DeleteFile(query_str);
    }

    bool AccountManager::SaveDatabase(CharacterDatabase * database, long avatar_id)
    {
	    char query_str[256];
	    sql_query_c account_query (&m_SQL_Conn);

        //We need to completely remove the character we are saving (in this order due to foreign keys)

	    sprintf(query_str, "DELETE FROM `ship_info` WHERE `avatar_id` = '%d'", avatar_id);
	    if (!account_query.run_query(query_str)) return false;

	    sprintf(query_str, "DELETE FROM `ship_data` WHERE `avatar_id` = '%d'", avatar_id);
	    if (!account_query.run_query(query_str)) return false;

	    sprintf(query_str, "DELETE FROM `avatar_data` WHERE `avatar_id` = '%d'", avatar_id);
	    if (!account_query.run_query(query_str)) return false;

	    sprintf(query_str, "DELETE FROM `avatar_info` WHERE `avatar_id` = '%d'", avatar_id);
	    if (!account_query.run_query(query_str)) return false;

	    sql_query account_builder;

	    ///////////////////////////////////////////////////////////////////////////////////
	    ////////////////////////////    Save AvatarInfo    ////////////////////////////////
	    ///////////////////////////////////////////////////////////////////////////////////

        account_builder.Clear();
        account_builder.SetTable("avatar_info");

        account_builder.AddData("avatar_id", avatar_id);
        account_builder.AddData("account_id", ntohl(database->info.account_id_lsb));
        account_builder.AddData("slot", ntohl(database->info.avatar_slot));
        account_builder.AddData("sector", ntohl(database->info.sector_id));
        account_builder.AddData("galaxy", ntohl(database->info.galaxy_id));
        account_builder.AddData("count", ntohl(database->info.count));
        account_builder.AddData("admin", ntohl(database->info.admin_level));
        account_builder.AddData("combat", ntohl(database->info.combat_level));
        account_builder.AddData("explore", ntohl(database->info.explore_level));
        account_builder.AddData("trade", ntohl(database->info.trade_level));

	    if (!account_query.run_query(account_builder.CreateQuery()))
	    {
		    LogMessage("Could not save AvatarInfo for id %d\n", avatar_id);
		    return false;
	    }

	    ///////////////////////////////////////////////////////////////////////////////////
	    ////////////////////////////    Save AvatarData    ////////////////////////////////
	    ///////////////////////////////////////////////////////////////////////////////////

        account_builder.Clear();
        account_builder.SetTable("avatar_data");

        account_builder.AddData("avatar_id", avatar_id);
        account_builder.AddData("first_name", database->avatar.avatar_first_name);
        account_builder.AddData("last_name", database->avatar.avatar_last_name);
        account_builder.AddData("type", database->avatar.avatar_type);
        account_builder.AddData("version", database->avatar.avatar_version);
        account_builder.AddData("race", database->avatar.race);
        account_builder.AddData("prof", database->avatar.profession);
        account_builder.AddData("gender", database->avatar.gender);
        account_builder.AddData("mood", database->avatar.mood_type);
        account_builder.AddData("personality", database->avatar.personality);
        account_builder.AddData("nlp", database->avatar.nlp);
        account_builder.AddData("body", database->avatar.body_type);
        account_builder.AddData("pants", database->avatar.pants_type);
        account_builder.AddData("head", database->avatar.head_type);
        account_builder.AddData("hair", database->avatar.hair_num);
        account_builder.AddData("ear", database->avatar.ear_num);
        account_builder.AddData("goggle", database->avatar.goggle_num);
        account_builder.AddData("beard", database->avatar.beard_num);
        account_builder.AddData("weapon_hip", database->avatar.weapon_hip_num);
        account_builder.AddData("weapon_unique", database->avatar.weapon_unique_num);
        account_builder.AddData("weapon_back", database->avatar.weapon_back_num);
        account_builder.AddData("head_texture", database->avatar.head_texture_num);
        account_builder.AddData("tattoo_texture", database->avatar.tattoo_texture_num);
        account_builder.AddData("tattoo_X", database->avatar.tattoo_offset[0]);
        account_builder.AddData("tattoo_Y", database->avatar.tattoo_offset[1]);
        account_builder.AddData("tattoo_Z", database->avatar.tattoo_offset[2]);
        account_builder.AddData("hair_H", database->avatar.hair_color[0]);
        account_builder.AddData("hair_S", database->avatar.hair_color[1]);
        account_builder.AddData("hair_V", database->avatar.hair_color[2]);
        account_builder.AddData("beard_H", database->avatar.beard_color[0]);
        account_builder.AddData("beard_S", database->avatar.beard_color[1]);
        account_builder.AddData("beard_V", database->avatar.beard_color[2]);
        account_builder.AddData("eye_H", database->avatar.eye_color[0]);
        account_builder.AddData("eye_S", database->avatar.eye_color[1]);
        account_builder.AddData("eye_V", database->avatar.eye_color[2]);
        account_builder.AddData("skin_H", database->avatar.skin_color[0]);
        account_builder.AddData("skin_S", database->avatar.skin_color[1]);
        account_builder.AddData("skin_V", database->avatar.skin_color[2]);
        account_builder.AddData("shirt_p_H", database->avatar.shirt_primary_color[0]);
        account_builder.AddData("shirt_p_S", database->avatar.shirt_primary_color[1]);
        account_builder.AddData("shirt_p_V", database->avatar.shirt_primary_color[2]);
        account_builder.AddData("shirt_s_H", database->avatar.shirt_secondary_color[0]);
        account_builder.AddData("shirt_s_S", database->avatar.shirt_secondary_color[1]);
        account_builder.AddData("shirt_s_V", database->avatar.shirt_secondary_color[2]);
        account_builder.AddData("pants_p_H", database->avatar.pants_primary_color[0]);
        account_builder.AddData("pants_p_S", database->avatar.pants_primary_color[1]);
        account_builder.AddData("pants_p_V", database->avatar.pants_primary_color[2]);
        account_builder.AddData("pants_s_H", database->avatar.pants_secondary_color[0]);
        account_builder.AddData("pants_s_S", database->avatar.pants_secondary_color[1]);
        account_builder.AddData("pants_s_V", database->avatar.pants_secondary_color[2]);
        account_builder.AddData("shirt_p_metal", database->avatar.shirt_primary_metal);
        account_builder.AddData("shirt_s_metal", database->avatar.shirt_secondary_metal);
        account_builder.AddData("pants_p_metal", database->avatar.pants_primary_metal);
        account_builder.AddData("pants_s_metal", database->avatar.pants_secondary_metal);
        account_builder.AddData("height_weight_0", database->avatar.height_weight_1[0]);
        account_builder.AddData("height_weight_1", database->avatar.height_weight_1[1]);
        account_builder.AddData("height_weight_2", database->avatar.height_weight_1[2]);
        account_builder.AddData("height_weight_3", database->avatar.height_weight_1[3]);
        account_builder.AddData("height_weight_4", database->avatar.height_weight_1[4]);

	    if (!account_query.run_query(account_builder.CreateQuery()))
	    {
		    LogMessage("Could not save AvatarData for id %d\n", avatar_id);
		    return false;
	    }

	    ///////////////////////////////////////////////////////////////////////////////////
	    /////////////////////////////    Save ShipData    /////////////////////////////////
	    ///////////////////////////////////////////////////////////////////////////////////

        account_builder.Clear();
        account_builder.SetTable("ship_data");

        account_builder.AddData("avatar_id", avatar_id);
        account_builder.AddData("race", database->ship_data.race);
        account_builder.AddData("prof", database->ship_data.profession);
        account_builder.AddData("hull", database->ship_data.hull);
        account_builder.AddData("wing", database->ship_data.wing);
        account_builder.AddData("decal", database->ship_data.decal);
        account_builder.AddData("name", database->ship_data.ship_name);
        account_builder.AddData("name_H", database->ship_data.ship_name_color[0]);
        account_builder.AddData("name_S", database->ship_data.ship_name_color[1]);
        account_builder.AddData("name_V", database->ship_data.ship_name_color[2]);
        account_builder.AddData("hull_p_H", database->ship_data.HullPrimaryColor.HSV[0]);
        account_builder.AddData("hull_p_S", database->ship_data.HullPrimaryColor.HSV[1]);
        account_builder.AddData("hull_p_V", database->ship_data.HullPrimaryColor.HSV[2]);
        account_builder.AddData("hull_p_flat", database->ship_data.HullPrimaryColor.flat);
        account_builder.AddData("hull_p_metal", database->ship_data.HullPrimaryColor.metal);
        account_builder.AddData("hull_s_H", database->ship_data.HullSecondaryColor.HSV[0]);
        account_builder.AddData("hull_s_S", database->ship_data.HullSecondaryColor.HSV[1]);
        account_builder.AddData("hull_s_V", database->ship_data.HullSecondaryColor.HSV[2]);
        account_builder.AddData("hull_s_flat", database->ship_data.HullSecondaryColor.flat);
        account_builder.AddData("hull_s_metal", database->ship_data.HullSecondaryColor.metal);
        account_builder.AddData("prof_p_H", database->ship_data.ProfessionPrimaryColor.HSV[0]);
        account_builder.AddData("prof_p_S", database->ship_data.ProfessionPrimaryColor.HSV[1]);
        account_builder.AddData("prof_p_V", database->ship_data.ProfessionPrimaryColor.HSV[2]);
        account_builder.AddData("prof_p_flat", database->ship_data.ProfessionPrimaryColor.flat);
        account_builder.AddData("prof_p_metal", database->ship_data.ProfessionPrimaryColor.metal);
        account_builder.AddData("prof_s_H", database->ship_data.ProfessionSecondaryColor.HSV[0]);
        account_builder.AddData("prof_s_S", database->ship_data.ProfessionSecondaryColor.HSV[1]);
        account_builder.AddData("prof_s_V", database->ship_data.ProfessionSecondaryColor.HSV[2]);
        account_builder.AddData("prof_s_flat", database->ship_data.ProfessionSecondaryColor.flat);
        account_builder.AddData("prof_s_metal", database->ship_data.ProfessionSecondaryColor.metal);
        account_builder.AddData("wing_p_H", database->ship_data.WingPrimaryColor.HSV[0]);
        account_builder.AddData("wing_p_S", database->ship_data.WingPrimaryColor.HSV[1]);
        account_builder.AddData("wing_p_V", database->ship_data.WingPrimaryColor.HSV[2]);
        account_builder.AddData("wing_p_flat", database->ship_data.WingPrimaryColor.flat);
        account_builder.AddData("wing_p_metal", database->ship_data.WingPrimaryColor.metal);
        account_builder.AddData("wing_s_H", database->ship_data.WingSecondaryColor.HSV[0]);
        account_builder.AddData("wing_s_S", database->ship_data.WingSecondaryColor.HSV[1]);
        account_builder.AddData("wing_s_V", database->ship_data.WingSecondaryColor.HSV[2]);
        account_builder.AddData("wing_s_flat", database->ship_data.WingSecondaryColor.flat);
        account_builder.AddData("wing_s_metal", database->ship_data.WingSecondaryColor.metal);
        account_builder.AddData("engine_p_H", database->ship_data.EnginePrimaryColor.HSV[0]);
        account_builder.AddData("engine_p_S", database->ship_data.EnginePrimaryColor.HSV[1]);
        account_builder.AddData("engine_p_V", database->ship_data.EnginePrimaryColor.HSV[2]);
        account_builder.AddData("engine_p_flat", database->ship_data.EnginePrimaryColor.flat);
        account_builder.AddData("engine_p_metal", database->ship_data.EnginePrimaryColor.metal);
        account_builder.AddData("engine_s_H", database->ship_data.EngineSecondaryColor.HSV[0]);
        account_builder.AddData("engine_s_S", database->ship_data.EngineSecondaryColor.HSV[1]);
        account_builder.AddData("engine_s_V", database->ship_data.EngineSecondaryColor.HSV[2]);
        account_builder.AddData("engine_s_flat", database->ship_data.EngineSecondaryColor.flat);
        account_builder.AddData("engine_s_metal", database->ship_data.EngineSecondaryColor.metal);

	    if (!account_query.run_query(account_builder.CreateQuery()))
	    {
		    LogMessage("Could not save ShipData for id %d\n", avatar_id);
		    return false;
	    }

	    ///////////////////////////////////////////////////////////////////////////////////
	    /////////////////////////////    Save ShipInfo    /////////////////////////////////
	    ///////////////////////////////////////////////////////////////////////////////////

        account_builder.Clear();
        account_builder.SetTable("ship_info");

        account_builder.AddData("avatar_id", avatar_id);
        account_builder.AddData("hull", database->ship_info.hull);
        account_builder.AddData("prof", database->ship_info.profession);
        account_builder.AddData("engine", database->ship_info.engine);
        account_builder.AddData("wing", database->ship_info.wing);
        account_builder.AddData("pos_0", database->ship_info.Position[0]);
        account_builder.AddData("pos_1", database->ship_info.Position[1]);
        account_builder.AddData("pos_2", database->ship_info.Position[2]);
        account_builder.AddData("ori_0", database->ship_info.Orientation[0]);
        account_builder.AddData("ori_1", database->ship_info.Orientation[1]);
        account_builder.AddData("ori_2", database->ship_info.Orientation[2]);
        account_builder.AddData("ori_3", database->ship_info.Orientation[3]);

	    if (!account_query.run_query(account_builder.CreateQuery()))
	    {
		    LogMessage("Could not save ShipData for id %d\n", avatar_id);
		    return false;
	    }

	    return true;
    }

    //Read the avatar database for the player given an avatarID
    bool AccountManager::ReadDatabase(CharacterDatabase *database, long avatar_id)
    {
	    memset(database, 0, sizeof(CharacterDatabase));

	    char query_str[256];
		long sector_id;
	    sql_query_c account_query (&m_SQL_Conn);
	    sql_result_c account_result;
        sql_row_c account_row;

	    ///////////////////////////////////////////////////////////////////////////////////
	    ///////////////////////////    Read in AvatarInfo   ///////////////////////////////
	    ///////////////////////////////////////////////////////////////////////////////////

        sprintf(query_str, "SELECT * FROM `avatar_info` WHERE `avatar_id` = '%d'", avatar_id);

	    if (!account_query.run_query(query_str) || account_query.n_rows() == 0)
	    {
		    //LogMessage("Could not load AvatarInfo for id %d\n", avatar_id);
		    return false;
	    }

	    account_query.store(&account_result);
        account_result.fetch_row(&account_row);

        //This is the same order as the structure
        database->info.avatar_id_msb = 0;
        database->info.avatar_id_lsb = ntohl(avatar_id);
        database->info.account_id_msb = 0;
        database->info.account_id_lsb = ntohl(account_row["account_id"]);
        database->info.avatar_slot = ntohl(account_row["slot"]);   //Also (avatar_id - 1) / 5
		sector_id = account_row["sector"];
		if (sector_id == 0) sector_id = 10711;

        database->info.sector_id = ntohl(sector_id);
        database->info.galaxy_id = ntohl(account_row["galaxy"]);
        database->info.count = ntohl(account_row["count"]);
        database->info.admin_level = ntohl(account_row["admin"]);
	    database->info.gm_flag = ntohl(1);
        database->info.combat_level = ntohl(account_row["combat"]);
        database->info.explore_level = ntohl(account_row["explore"]);
        database->info.trade_level = ntohl(account_row["trade"]);

        sprintf(database->info.location, "%s", g_ServerMgr->GetSectorName(sector_id));

	    ///////////////////////////////////////////////////////////////////////////////////
	    ///////////////////////////    Read in AvatarData   ///////////////////////////////
	    ///////////////////////////////////////////////////////////////////////////////////

        sprintf(query_str, "SELECT * FROM `avatar_data` WHERE `avatar_id` = '%d'", avatar_id);

	    if (!account_query.run_query(query_str) || account_query.n_rows() == 0)
	    {
		    //LogMessage("Could not load AvatarData for id %d\n", avatar_id);
		    return false;
	    }

	    account_query.store(&account_result);
        account_result.fetch_row(&account_row);

        strcpy(database->avatar.avatar_first_name, account_row["first_name"]);
        strcpy(database->avatar.avatar_last_name, account_row["last_name"]);
	    database->avatar.avatar_type = account_row["type"];
	    database->avatar.avatar_version = account_row["version"];
	    database->avatar.race = account_row["race"];
	    database->avatar.profession = account_row["prof"];
	    database->avatar.gender = account_row["gender"];
	    database->avatar.mood_type = account_row["mood"];
	    database->avatar.personality = account_row["personality"];
	    database->avatar.nlp = account_row["nlp"];
	    database->avatar.body_type = account_row["body"];
	    database->avatar.pants_type = account_row["pants"];
	    database->avatar.head_type = account_row["head"];
	    database->avatar.hair_num = account_row["hair"];
	    database->avatar.ear_num = account_row["ear"];
	    database->avatar.goggle_num = account_row["goggle"];
	    database->avatar.beard_num = account_row["beard"];
	    database->avatar.weapon_hip_num = account_row["weapon_hip"];
	    database->avatar.weapon_unique_num = account_row["weapon_unique"];
	    database->avatar.weapon_back_num = account_row["weapon_back"];
	    database->avatar.head_texture_num = account_row["head_texture"];
	    database->avatar.tattoo_texture_num = account_row["tattoo_texture"];
	    database->avatar.tattoo_offset[0] = account_row["tattoo_X"];
	    database->avatar.tattoo_offset[1] = account_row["tattoo_Y"];
	    database->avatar.tattoo_offset[2] = account_row["tattoo_Z"];
	    database->avatar.hair_color[0] = account_row["hair_H"];
	    database->avatar.hair_color[1] = account_row["hair_S"];
	    database->avatar.hair_color[2] = account_row["hair_V"];
	    database->avatar.beard_color[0] = account_row["beard_H"];
	    database->avatar.beard_color[1] = account_row["beard_S"];
	    database->avatar.beard_color[2] = account_row["beard_V"];
	    database->avatar.eye_color[0] = account_row["eye_H"];
	    database->avatar.eye_color[1] = account_row["eye_S"];
	    database->avatar.eye_color[2] = account_row["eye_V"];
	    database->avatar.skin_color[0] = account_row["skin_H"];
	    database->avatar.skin_color[1] = account_row["skin_S"];
	    database->avatar.skin_color[2] = account_row["skin_V"];
	    database->avatar.shirt_primary_color[0] = account_row["shirt_p_H"];
	    database->avatar.shirt_primary_color[1] = account_row["shirt_p_S"];
	    database->avatar.shirt_primary_color[2] = account_row["shirt_p_V"];
	    database->avatar.shirt_secondary_color[0] = account_row["shirt_s_H"];
	    database->avatar.shirt_secondary_color[1] = account_row["shirt_s_S"];
	    database->avatar.shirt_secondary_color[2] = account_row["shirt_s_V"];
	    database->avatar.pants_primary_color[0] = account_row["pants_p_H"];
	    database->avatar.pants_primary_color[1] = account_row["pants_p_S"];
	    database->avatar.pants_primary_color[2] = account_row["pants_p_V"];
	    database->avatar.pants_secondary_color[0] = account_row["pants_s_H"];
	    database->avatar.pants_secondary_color[1] = account_row["pants_s_S"];
	    database->avatar.pants_secondary_color[2] = account_row["pants_s_V"];
	    database->avatar.shirt_primary_metal = account_row["shirt_p_metal"];
	    database->avatar.shirt_secondary_metal = account_row["shirt_s_metal"];
	    database->avatar.pants_primary_metal = account_row["pants_p_metal"];
	    database->avatar.pants_secondary_metal = account_row["pants_s_metal"];
	    database->avatar.height_weight_1[0] = account_row["height_weight_0"];
	    database->avatar.height_weight_1[1] = account_row["height_weight_1"];
	    database->avatar.height_weight_1[2] = account_row["height_weight_2"];
	    database->avatar.height_weight_1[3] = account_row["height_weight_3"];
	    database->avatar.height_weight_1[4] = account_row["height_weight_4"];

        //These two have always been the same when testing
	    database->avatar.height_weight_2[0] = database->avatar.height_weight_1[0];
	    database->avatar.height_weight_2[1] = database->avatar.height_weight_1[1];
	    database->avatar.height_weight_2[2] = database->avatar.height_weight_1[2];
	    database->avatar.height_weight_2[3] = database->avatar.height_weight_1[3];
	    database->avatar.height_weight_2[4] = database->avatar.height_weight_1[4];

	    ///////////////////////////////////////////////////////////////////////////////////
	    ///////////////////////////     Read in ShipData    ///////////////////////////////
	    ///////////////////////////////////////////////////////////////////////////////////
    
        sprintf(query_str, "SELECT * FROM `ship_data` WHERE `avatar_id` = '%d'", avatar_id);

	    if (!account_query.run_query(query_str) || account_query.n_rows() == 0)
	    {
		    //LogMessage("Could not load ShipInfo for id %d\n", avatar_id);
		    return false;
	    }

	    account_query.store(&account_result);
        account_result.fetch_row(&account_row);

	    database->ship_data.race = account_row["race"];
	    database->ship_data.profession = account_row["prof"];
	    database->ship_data.hull = account_row["hull"];
	    database->ship_data.wing = account_row["wing"];
	    database->ship_data.decal = account_row["decal"];
	    strcpy(database->ship_data.ship_name, account_row["name"]);
	    database->ship_data.ship_name_color[0] = account_row["name_H"];
	    database->ship_data.ship_name_color[1] = account_row["name_S"];
	    database->ship_data.ship_name_color[2] = account_row["name_V"];
	    database->ship_data.HullPrimaryColor.HSV[0] = account_row["hull_p_H"];
	    database->ship_data.HullPrimaryColor.HSV[1] = account_row["hull_p_S"];
	    database->ship_data.HullPrimaryColor.HSV[2] = account_row["hull_p_V"];
	    database->ship_data.HullPrimaryColor.flat = account_row["hull_p_flat"];
	    database->ship_data.HullPrimaryColor.metal = account_row["hull_p_metal"];
	    database->ship_data.HullSecondaryColor.HSV[0] = account_row["hull_s_H"];
	    database->ship_data.HullSecondaryColor.HSV[1] = account_row["hull_s_S"];
	    database->ship_data.HullSecondaryColor.HSV[2] = account_row["hull_s_V"];
	    database->ship_data.HullSecondaryColor.flat = account_row["hull_s_flat"];
	    database->ship_data.HullSecondaryColor.metal = account_row["hull_s_metal"];
	    database->ship_data.ProfessionPrimaryColor.HSV[0] = account_row["prof_p_H"];
	    database->ship_data.ProfessionPrimaryColor.HSV[1] = account_row["prof_p_S"];
	    database->ship_data.ProfessionPrimaryColor.HSV[2] = account_row["prof_p_V"];
	    database->ship_data.ProfessionPrimaryColor.flat = account_row["prof_p_flat"];
	    database->ship_data.ProfessionPrimaryColor.metal = account_row["prof_p_metal"];
	    database->ship_data.ProfessionSecondaryColor.HSV[0] = account_row["prof_s_H"];
	    database->ship_data.ProfessionSecondaryColor.HSV[1] = account_row["prof_s_S"];
	    database->ship_data.ProfessionSecondaryColor.HSV[2] = account_row["prof_s_V"];
	    database->ship_data.ProfessionSecondaryColor.flat = account_row["prof_s_flat"];
	    database->ship_data.ProfessionSecondaryColor.metal = account_row["prof_s_metal"];
	    database->ship_data.WingPrimaryColor.HSV[0] = account_row["wing_p_H"];
	    database->ship_data.WingPrimaryColor.HSV[1] = account_row["wing_p_S"];
	    database->ship_data.WingPrimaryColor.HSV[2] = account_row["wing_p_V"];
	    database->ship_data.WingPrimaryColor.flat = account_row["wing_p_flat"];
	    database->ship_data.WingPrimaryColor.metal = account_row["wing_p_metal"];
	    database->ship_data.WingSecondaryColor.HSV[0] = account_row["wing_s_H"];
	    database->ship_data.WingSecondaryColor.HSV[1] = account_row["wing_s_S"];
	    database->ship_data.WingSecondaryColor.HSV[2] = account_row["wing_s_V"];
	    database->ship_data.WingSecondaryColor.flat = account_row["wing_s_flat"];
	    database->ship_data.WingSecondaryColor.metal = account_row["wing_s_metal"];
	    database->ship_data.EnginePrimaryColor.HSV[0] = account_row["engine_p_H"];
	    database->ship_data.EnginePrimaryColor.HSV[1] = account_row["engine_p_S"];
	    database->ship_data.EnginePrimaryColor.HSV[2] = account_row["engine_p_V"];
	    database->ship_data.EnginePrimaryColor.flat = account_row["engine_p_flat"];
	    database->ship_data.EnginePrimaryColor.metal = account_row["engine_p_metal"];
	    database->ship_data.EngineSecondaryColor.HSV[0] = account_row["engine_s_H"];
	    database->ship_data.EngineSecondaryColor.HSV[1] = account_row["engine_s_S"];
	    database->ship_data.EngineSecondaryColor.HSV[2] = account_row["engine_s_V"];
	    database->ship_data.EngineSecondaryColor.flat = account_row["engine_s_flat"];
	    database->ship_data.EngineSecondaryColor.metal = account_row["engine_s_metal"];


	    ///////////////////////////////////////////////////////////////////////////////////
	    ///////////////////////////     Read in ShipInfo    ///////////////////////////////
	    ///////////////////////////////////////////////////////////////////////////////////
    
        //Load in the mounts and location of the player
        //assume that the data is intact
        sprintf(query_str, "SELECT * FROM `ship_info` WHERE `avatar_id` = '%d'", avatar_id);

	    if (!account_query.run_query(query_str) || account_query.n_rows() == 0)
	    {
		    //LogMessage("Could not load ShipInfo for id %d\n", avatar_id);
		    return false;
	    }

	    account_query.store(&account_result);
        account_result.fetch_row(&account_row);

        database->ship_info.hull = account_row["hull"];
        database->ship_info.profession = account_row["prof"];
        database->ship_info.engine = account_row["engine"];
        database->ship_info.wing = account_row["wing"];

        database->ship_info.Position[0] = account_row["pos_0"];
        database->ship_info.Position[1] = account_row["pos_1"];
        database->ship_info.Position[2] = account_row["pos_2"];
        database->ship_info.Orientation[0] = account_row["ori_0"];
        database->ship_info.Orientation[1] = account_row["ori_1"];
        database->ship_info.Orientation[2] = account_row["ori_2"];
        database->ship_info.Orientation[3] = account_row["ori_3"];

        return true;
    }

#else

    void AccountManager::LoadAccounts()
    {
        if (m_NumAccounts != 0)
        {
            LogMessage("Flat file accounts already loaded. Restart server to import changes\n");
            return;
        }

	    m_Mutex.Lock();

	    char buffer[256];
	    char filename[MAX_PATH];
	    sprintf(filename, "%saccounts.txt", SERVER_DATABASE_PATH);
	    FILE *f = fopen(filename, "r");
	    if (f)
	    {
		    while (!feof(f))
		    {
			    if (fgets(buffer, sizeof(buffer), f))
			    {
				    // ignore records starting with a semicolon
				    if (buffer[0] != ';')
				    {
					    char *username = buffer;
					    char *password = strstr(buffer, ",");
					    if (password)
					    {
						    *password++ = 0;
						    char *status = strstr(password, ",");
						    if (status)
						    {
							    *status++ = 0;
							    m_Accounts[m_NumAccounts].ID = m_NumAccounts;
							    m_Accounts[m_NumAccounts].UserName = g_StringMgr->GetStr(username);
							    m_Accounts[m_NumAccounts].Password = g_StringMgr->GetStr(password);
                                m_Accounts[m_NumAccounts].Ticket = g_StringMgr->NullStr();
							    m_Accounts[m_NumAccounts].Status = atoi(status);
							    m_Accounts[m_NumAccounts].InUse = false;
							    m_NumAccounts++;
						    }
					    }
				    }
			    }
		    }
		    fclose(f);
	    }

	    m_Mutex.Unlock();

        printf("Loaded %d accounts from file\n", m_NumAccounts);
    }

     AccountManager::_User * AccountManager::GetUserFromID(long account_id)
    {
        for (int i=0; i<m_NumAccounts; i++)
        {
            if (m_Accounts[i].ID == account_id)
            {
                return &m_Accounts[i];
            }
        }

	    return 0;
    }

     AccountManager::_User * AccountManager::GetUserFromUsername(char *username)
    {
        for (int i=0; i<m_NumAccounts; i++)
        {
            if (_strcmpi(m_Accounts[i].UserName, username) == 0)
            {
                return &m_Accounts[i];
            }
        }

	    return 0;
    }

    int AccountManager::NumAccounts()
    {
        return m_NumAccounts;
    }

    long AccountManager::GetAccountID(int index)
    {
        return index < m_NumAccounts ? m_Accounts[index].ID : -1;
    }

    //Create the path to a charachter's database given an accountID, buffer must be long enough (MAX_PATH)
    bool AccountManager::CreateCharacterDatabasePath(char *buffer, long avatar_id)
    {
        _User * u = GetUserFromID((avatar_id - 1) / 5);
        if (!u)
        {
            return false;
        }

        sprintf(buffer, "%sUser_%s_Slot_%d.dat", SERVER_USER_PATH, u->UserName, avatar_id);
        return true;
    }

    long AccountManager::GetAccountStatus(char *username)
    {
        _User * u = GetUserFromUsername(username);
        return u ? u->Status : -2;
    }

    long AccountManager::GetAccountID(char *username)
    {
        _User * u = GetUserFromUsername(username);
        return u ? u->ID : -1;
    }

    //Validates the username/password set, returns the account if valid
    long AccountManager::ValidateAccount(char *username, char *password)
    {
        for (int i=0; i<m_NumAccounts; i++)
        {
            if (strcmp(m_Accounts[i].UserName, username) == 0)
            {
                if (m_Accounts[i].InUse)
                {
                    LogMessage("ValidateUserAccount: Account `%s` already in use!\n", username);
                    return -1;
                }
                else if (strcmp(m_Accounts[i].Password, password) != 0)
                {
		            LogMessage("ValidateUserAccount: Invalid password: `%s` for user: `%s`\n", password, username);
                    return -1;
                }
                else
                {
                    return m_Accounts[i].ID;
                }
            }
        }

        LogMessage("ValidateUserAccount: Could not find user: `%s`\n", username);
        return -1;
    }

    bool AccountManager::AddUser(char *username, char *password, char *access)
    {
        LogMessage("Cannot add users when using flat file for accounts!\n");
        return false;
    }

    bool AccountManager::SetAccountStatus(char *username, long status)
    {
        LogMessage("Cannot set access level when using flat file for accounts!\n");
        return false;
    }

    bool AccountManager::ChangePassword(char *username, char *password)
    {
        LogMessage("Cannot change password when using flat file for accounts!\n");
        return false;
    }

    bool AccountManager::IsUsernameUnique(char *name)
    {
        CharacterDatabase database;
        int account_count = g_AccountMgr->NumAccounts();
        long account_id, avatar_id;

        for (int i=0; i<account_count; i++)
        {
            account_id = g_AccountMgr->GetAccountID(i);

            for (long j=0; j<5; j++)
            {
                avatar_id = account_id * 5 + j;

                if (ReadDatabase(&database, avatar_id))
                {
                    if (_strcmpi(database.avatar.avatar_first_name, name) == 0)
                    {
                        return false;
                    }
                }
            }

        }
	    
        return true;
    }

    void AccountManager::DeleteCharacter(long avatar_id)
    {
        char filename[MAX_PATH];
        if (!CreateCharacterDatabasePath(filename, avatar_id))
        {
            return;
        }

        LogDebug("Deleting %s\n", filename);	
        DeleteFile(filename);

	    sprintf(filename, "%sCharInfo-%d.chr", SERVER_USER_PATH, avatar_id);

        LogDebug("Deleting %s\n", filename);
        DeleteFile(filename);
    }

    bool AccountManager::SaveDatabase(CharacterDatabase * database, long avatar_id)
    {
        char filename[MAX_PATH];
        if (!CreateCharacterDatabasePath(filename, avatar_id))
        {
            return false;
        }

        //Write the flat file database
        FILE *f = fopen(filename, "wb");
        if (f)
        {
            fwrite(database, 1, sizeof(CharacterDatabase), f);
            fclose(f);
            return true;
        }

        return false;
    }

    //Read the avatar database for the player given an avatarID
    bool AccountManager::ReadDatabase(CharacterDatabase *database, long avatar_id)
    {
        char filename[MAX_PATH];
        if (!CreateCharacterDatabasePath(filename, avatar_id))
        {
            return false;
        }

        //Read the database from the character file
        FILE *f = fopen(filename, "rb");
        if (f)
        {
            if (fread(database, 1, sizeof(CharacterDatabase), f) == sizeof(CharacterDatabase))
            {
                fclose(f);
                return true;
            }

            fclose(f);
        }

        return false;
    }

#endif

//Returns the avatar ID (base 1) for a player's character, or -1 on error
long AccountManager::GetAvatarID(char *username, int slot)
{
	if (slot >= 0 && slot <= 4)
	{
        long account_id = GetAccountID(username);
        return account_id == -1 ? -1 : AVATAR_ID(account_id,slot);
	}

    return -1;
}

char * AccountManager::IssueTicket(char *username, char *password)
{
    //Check if the account is already in use!
	if (g_PlayerMgr->CheckAccountInUse(username))
	{
		return 0;
	}

	long account = ValidateAccount(username, password);

    //If ValidateAccount returned -1, this username/password set is invalid
    if (account == -1)
    {
        return 0;
    }

    //If using SQL, update the last login time
#ifdef USE_MYSQL_ACCOUNT_DATA
    UpdateTime(account);
#endif

    return BuildTicket(username);
}

char * AccountManager::BuildTicket(char *username)
{
    unsigned long cur_time = GetNet7TickCount();

    m_Mutex.Lock();

    AccountTicket * last = 0;
    AccountTicket * time_slot = 0;

    for (time_slot = m_Tickets; time_slot; time_slot=time_slot->next)
    {
        last = time_slot;
        
        //If this ticket expired, use this slot
        if (time_slot->expire_time < cur_time)
        {
            break;
        }

        //If the slot is empty use it
        if (!time_slot->username)
        {
            break;
        }
    }

    //If we did not find an empty slot
    if (!time_slot)
    {
        last->next = new AccountTicket();
        memset(last->next, 0, sizeof(AccountTicket));
        time_slot = last->next;
    }

    //Seed the randomizer
    srand((unsigned int)time(NULL));

    //Fill the slot
    sprintf(time_slot->ticket, "%s-%d", username, rand());
    time_slot->username = g_StringMgr->GetStr(username);
    time_slot->expire_time = cur_time + TICKET_EXPIRE_TIME;

    m_Mutex.Unlock();

    return time_slot->ticket;
}

//This returns a string from the string manager
char * AccountManager::GetUsernameFromTicket(char *ticket)
{
    unsigned long cur_time = GetNet7TickCount();

    m_Mutex.Lock();

    for (AccountTicket * time_slot = m_Tickets; time_slot; time_slot=time_slot->next)
    {
        //We need to first compare the ticket, then see if it expired
        if (strcmp(time_slot->ticket, ticket) == 0)
        {
            if (time_slot->expire_time > cur_time)
            {
                //Ticket is valid, expire it
                time_slot->expire_time = cur_time;

                m_Mutex.Unlock();
                return time_slot->username;
            }
            else
            {
                //The ticket expired, free this slot
                time_slot->expire_time = 0;
                time_slot->ticket[0] = 0;
                time_slot->username = 0;

                m_Mutex.Unlock();
                return 0;
            }
        }
    }

    //The ticket was not found
    m_Mutex.Unlock();
    return 0;
}

//Returns -1 if successful, and an error otherwise
long AccountManager::CreateCharacter(GlobalCreateCharacter * create)
{
    //The new character needs to be checked for errors before it is allowed to be added
	char * name = create->avatar.avatar_first_name;
	int name_len = strlen(create->avatar.avatar_first_name);

	//Name must be atleast 3 characters long
	if (name_len < 3)
    {
		return G_ERROR_TOO_SHORT;
	}

	//Name cannot have more than 3 repeating characters
    //Name must also have atleast one vowel
    char chr, last = 0, count = 0;
    bool has_vowel = false;

    for(int i=0; i<name_len; i++)
	{
		chr = name[i];

        //Check for vowel in the same loop
        if (!has_vowel)
        {
		    if (name[i] == 'a' || name[i] == 'e' || name[i] == 'i' || name[i] == 'o' || name[i] == 'u' || name[i] == 'y')
		    {
			    has_vowel = true;
		    }
        }

		if (isupper(chr))
        {
			chr = tolower(chr);
        }

		if (last == chr)
		{
            count++;

            //When count is 2, there were 3 repeating characters
			if (count == 2)
            {
				return G_ERROR_REPEATING_CHAR;
			}
		}
        else
        {
			count = 0;
        }

		last = chr;
	}

    //If no vowel was found above, send error
	if (!has_vowel) 
    {
		return G_ERROR_ONE_VOWEL;
	}

	//Check to see if character name or ship name is not forbidden
    char ForName[MAX_PATH];
	sprintf(ForName, "%sforbidden.txt", SERVER_DATABASE_PATH);

	FILE *f = fopen(ForName, "r");
    if (f)
    {
        char buffer[128];
        char * restricted_name;

		while (!feof(f))
		{
			if (fgets(buffer, sizeof(buffer), f))
			{
		        if (restricted_name = strtok(buffer, "\n"))
                {
                    //Check character name
                    if (!_strcmpi(restricted_name, create->avatar.avatar_first_name))
                    {
                        fclose(f);
					    return G_ERROR_RESTRICTED_LIST;
				    }

                    //Check ship name
                    if (!_strcmpi(restricted_name, create->ship_data.ship_name))
                    {
                        fclose(f);
					    return G_ERROR_RESTRICTED_SHIP;
				    }
                }
            }
        }

        fclose(f);
	}

	//Lastly, check to see if that name was used
	if (!IsUsernameUnique(name)) 
    {
		return G_ERROR_NICKNAME_USED;
	}

    //Now the character has been validated and needs to be created
    CharacterDatabase database;
    memset(&database, 0, sizeof(CharacterDatabase));

    long start_sector = StartSector[create->avatar.race * 3 + create->avatar.profession];
    long avatar_id = GetAvatarID(create->account_username, htonl(create->character_slot));

    if (avatar_id == -1)
    {
        //Something went wrong
        return G_ERROR_NET7_INTERNAL;
    }

    //Set data to AvatarInfo
    database.info.avatar_id_lsb   = ntohl(avatar_id);
    database.info.account_id_lsb  = ntohl(ACCOUNT_ID(avatar_id));
    database.info.avatar_slot     = create->character_slot;
    database.info.sector_id       = ntohl(start_sector);
    database.info.galaxy_id       = create->galaxy_id;
    database.info.count           = ntohl(5);
    database.info.admin_level     = ntohl(GetAccountStatus(create->account_username));
    database.info.gm_flag         = ntohl(1);

    //Copy in the sector location (even if its null)
    sprintf(database.info.location, "%s", g_ServerMgr->GetSectorName(start_sector));

    //Copy in their avatar and ship
    database.avatar = create->avatar;
    database.ship_data = create->ship_data;

    //Now setup the ship base assets
	database.ship_info.hull         = BaseHullAsset[create->ship_data.race * 3 + create->ship_data.hull];
	database.ship_info.profession   = BaseProfAsset[create->ship_data.race * 3 + create->ship_data.profession];
	database.ship_info.wing         = BaseWingAsset[create->ship_data.race * 3 + create->ship_data.wing];
	database.ship_info.engine       = BaseEngineAsset[create->ship_data.race];

    //Finally save this character in out database
	SaveDatabase(&database, avatar_id);

    return -1;
}

void AccountManager::BuildAvatarList(GlobalAvatarList * list, long account_id)
{
    char *LOCAL_IP = "127.0.0.1"; //local loopback
    CharacterDatabase database;
    memset(list, 0, sizeof(GlobalAvatarList));

    //Insert the five characters into the avatar list
    for (int i=0; i<5; i++)
    {
        if (ReadDatabase(&database, AVATAR_ID(account_id, i)))
        {
            sprintf(database.info.location, "%s", 
                g_ServerMgr->GetSectorName(ntohl(database.info.sector_id)));

            list->avatar[i].data = database.avatar;
            list->avatar[i].info = database.info;
        }
    }

    list->num_galaxies = ntohl(1);
    strcpy(list->galaxy[0].Name, g_Galaxy_Name);
    list->galaxy[0].GalaxyID = 1;
    list->galaxy[0].IP_Address = ntohl(inet_addr(LOCAL_IP));//ntohl(g_ServerMgr->m_IpAddress);
    list->galaxy[0].port = ntohs(MASTER_SERVER_PORT);
    list->galaxy[0].NumPlayers = g_ServerMgr->m_GlobMemMgr->GetPlayerCount();
    list->galaxy[0].MaxPlayers = MAX_ONLINE_PLAYERS;
    list->galaxy[0].unknown2 = 1;
}

