// SaveManager.cpp
// This class runs a thread which handles all the players' changes
// It keeps a connection to the SQL DB open
// Eventually this could become a separate process and run on another server PC.

#include "Net7.h"
#include "SaveManager.h"
#include "PacketMethods.h"
#include <float.h>

enum experience_type { XP_COMBAT, XP_EXPLORE, XP_TRADE };

SaveManager::SaveManager()
{
	m_SaveBuffer = new CircularBuffer(0x20000, 2048);
	m_SaveQueue = new MessageQueue(m_SaveBuffer, 1000);
#ifdef WIN32
   	//UINT uiThreadId = 0;
    //m_SaveThreadHandle = (HANDLE)_beginthreadex(NULL, 0, StartSaveThread, this, CREATE_SUSPENDED, &uiThreadId);
#else
    //pthread_create(&m_Thread (??), NULL, &StartSaveThread, (void *) this);
#endif
	m_SQL_Conn.connect("net7_user", g_MySQL_Host, g_MySQL_User, g_MySQL_Pass);
}

SaveManager::~SaveManager()
{
    // TODO: ensure thread is halted and shuts down ok, having saved everyone's status
}

void SaveManager::CheckSaves()
{
	int length;
	EnbSaveHeader *header;
	unsigned char msg[SAVE_MESSAGE_MAX_LENGTH]; //message will be under 256 or the 'AddMessage' method will not process it

	while (m_SaveQueue->CheckQueue(msg, &length, SAVE_MESSAGE_MAX_LENGTH)) //check if there are any messages in the queue, if there are write them into the 'msg' buffer
	{
		//process this message
		//format is opcode/length/message
		header = (EnbSaveHeader*)msg;
		unsigned char *data = (msg + sizeof(EnbSaveHeader));

		//process opcode accordingly
		HandleSaveCode(header->save_code, header->player_id, header->size, data);
	}
}

void SaveManager::AddSaveMessage(short save_code, long player_id, short length, unsigned char *data)
{
    unsigned char pData[SAVE_MESSAGE_MAX_LENGTH];

    if (length + 4 > SAVE_MESSAGE_MAX_LENGTH)
    {
        LogMessage("Recv message overflow: length = %d\n", length);
        return;
    }

    *((short*) &pData[0]) = length;
    *((short*) &pData[2]) = save_code;
	*((long* ) &pData[4]) = player_id;

	if (data)
	{
		memcpy(pData + sizeof(short)*2 + sizeof(long), data, length);
	}

    m_SaveQueue->Add(pData, length+sizeof(EnbSaveHeader), player_id);
}

void SaveManager::HandleSaveCode(short save_code, long player_id, short bytes, unsigned char *data)
{
	switch (save_code)
	{
	case SAVE_CODE_ADVANCE_LEVEL:
		HandleAdvanceLevel(player_id, bytes, data);
		break;
	case SAVE_CODE_ADVANCE_SKILL:
		HandleAdvanceSkill(player_id, bytes, data);
		break;
	case SAVE_CODE_CHANGE_INVENTORY:
		HandleChangeInventory(player_id, bytes, data);
		break;
	case SAVE_CODE_CHANGE_EQUIPMENT:
		HandleChangeEquipment(player_id, bytes, data);
		break;
	case SAVE_CODE_AWARD_XP:
		HandleAwardXP(player_id, bytes, data);
		break;
	case SAVE_CODE_CREDIT_LEVEL:
		HandleCreditChange(player_id, bytes, data);
		break;
	case SAVE_CODE_STORE_POSITION:
		HandleStorePosition(player_id, bytes, data);
		break;
	case SAVE_CODE_ADVANCE_MISSION:
		HandleAdvanceMission(player_id, bytes, data);
		break;
	case SAVE_CODE_CHARACTER_PROGRESS_WIPE:
		HandleWipeCharacter(player_id);
		break;
	case SAVE_CODE_FULL_CHARACTER_WIPE:
		HandleFullWipeCharacter(player_id);
		break;
	case SAVE_CODE_MISSION_FLAGS:
		HandleAdvanceMissionFlags(player_id, bytes, data);
		break;
	case SAVE_CODE_CHANGE_AMMO:
		HandleChangeAmmo(player_id, bytes, data);
		break;
	case SAVE_CODE_HULL_UPGRADE:
		HandleHullUpgrade(player_id, bytes, data);
		break;
	case SAVE_CODE_HULL_LEVEL_CHANGE:
		HandleHullLevelChange(player_id, bytes, data);
		break;
	case SAVE_CODE_MISSION_COMPLETE:
		HandleMissionComplete(player_id, bytes, data);
		break;
	case SAVE_CODE_DISCOVER_NAV:
		HandleDiscoverNav(player_id, bytes, data);
		break;
	case SAVE_CODE_EXPLORE_NAV:
		HandleExploreNav(player_id, bytes, data);
		break;
	case SAVE_CODE_SET_SKILLPOINTS:
		HandleSetSkillPoints(player_id, bytes, data);
		break;
	case SAVE_CODE_SET_STARBASE:
		HandleSetRegisteredStarbase(player_id, bytes, data);
		break;
	case SAVE_CODE_SET_ENERGY_LEVELS:
		HandleSaveEnergyLevels(player_id, bytes, data);
		break;
	case SAVE_CODE_UPDATE_DATABASE:
		HandleUpdateDatabase(player_id, bytes, data);
		break;
	case SAVE_CODE_FACTION_CHANGE:
		HandleFactionUpdate(player_id, bytes, data);
		break;
	case SAVE_CODE_FULL_FACTION_WIPE:
		HandleFullFactionWipe(player_id, bytes, data);
		break;
	default:
		LogMessage( "Bad save code : %d for player %x\n", save_code, (player_id&0x00FFFFFF) );
		break;
	}
}

void SaveManager::HandleAdvanceLevel(long player_id, short bytes, unsigned char *data)
{
	//player just levelled up.
	sql_query_c account_query (&m_SQL_Conn);

	u8 xp_type = *((u8 *) &data[0]);
	long new_level = *((long *) &data[1]);

	switch (xp_type)
	{
    case XP_COMBAT:
		sprintf(m_QueryStr, "UPDATE net7_user.avatar_info SET combat = '%d' WHERE avatar_id = '%d'", new_level, player_id);
        break;

    case XP_EXPLORE:
		sprintf(m_QueryStr, "UPDATE net7_user.avatar_info SET explore = '%d' WHERE avatar_id = '%d'", new_level, player_id);
        break;

    case XP_TRADE:
		sprintf(m_QueryStr, "UPDATE net7_user.avatar_info SET trade = '%d' WHERE avatar_id = '%d'", new_level, player_id);
		break;
	}

	account_query.run_query(m_QueryStr);
}

void SaveManager::HandleAdvanceSkill(long player_id, short bytes, unsigned char *data)
{
	//player has just increased a skill
	sql_query_c account_query (&m_SQL_Conn);
	sql_result_c result;

	short skill_id = *((short *) &data[0]);
	short new_level= *((short *) &data[2]);

	//does this skill exist in the DB?  //TODO:: create this entry when the skill is awarded, then just run 'UPDATE' queries on it.
	sprintf(m_QueryStr, "SELECT * FROM net7_user.avatar_skill_levels WHERE avatar_id = '%d' AND skill_id = '%d'", player_id, skill_id);
	account_query.execute(m_QueryStr);
	account_query.store(&result);

	if (result.n_rows() != 0)
	{
		sprintf(m_QueryStr, "UPDATE net7_user.avatar_skill_levels SET skill_level = '%d' WHERE avatar_id = '%d' AND skill_id = '%d'", new_level, player_id, skill_id);
		account_query.run_query(m_QueryStr);
	}
	else
	{
		//we need to create a new entry
		sql_query SkillBuilder;
		SkillBuilder.Clear();
		SkillBuilder.SetTable("avatar_skill_levels");

		SkillBuilder.AddData("avatar_id", player_id);
		SkillBuilder.AddData("skill_id", skill_id);
		SkillBuilder.AddData("skill_level", new_level);

		if (!account_query.run_query(SkillBuilder.CreateQuery()))
		{
			LogMessage("Could not save Skill Info for id %d\n", player_id );
		}
	}
}

void SaveManager::HandleChangeInventory(long player_id, short bytes, unsigned char *data)
{
	//player has just had an inventory change of some sort
	sql_query_c account_query (&m_SQL_Conn);
	sql_result_c result;

	u8 inventory_slot = *((u8 *) &data[0]);
	u8 inventory_type = *((u8 *) &data[1]);
	short stack_level	 = *((short *) &data[2]);
	short trade_stack	 = *((short *) &data[4]);
	float quality		 = *((float *) &data[6]);
	long  item_id		 = *((long *)  &data[10]);
	unsigned long cost	 = *((unsigned long *)   &data[14]);

	switch (inventory_type)
	{
	case PLAYER_INVENTORY:
		sprintf(m_QueryStr, "SELECT * FROM net7_user.avatar_inventory_items WHERE avatar_id = '%d' AND inventory_slot = '%d'", player_id, inventory_slot);
		break;

	case PLAYER_VAULT:
		sprintf(m_QueryStr, "SELECT * FROM net7_user.avatar_vault_items WHERE avatar_id = '%d' AND inventory_slot = '%d'", player_id, inventory_slot);
		break;
	}

	//does this item exist in the DB?
	account_query.execute(m_QueryStr);
	account_query.store(&result);

	if (result.n_rows() != 0)
	{
		//yes, just update the item
		switch (inventory_type)
		{
		case PLAYER_INVENTORY:
			sprintf(m_QueryStr, "UPDATE net7_user.avatar_inventory_items SET item_id = '%d', stack_level = '%d', trade_stack = '%d', quality = '%f', cost = '%d' WHERE avatar_id = '%d' AND inventory_slot = '%d'",
				item_id, stack_level, trade_stack, quality, cost, player_id, inventory_slot);
			break;

		case PLAYER_VAULT:
			sprintf(m_QueryStr, "UPDATE net7_user.avatar_vault_items SET item_id = '%d', stack_level = '%d', trade_stack = '%d', quality = '%f', cost = '%d' WHERE avatar_id = '%d' AND inventory_slot = '%d'",
				item_id, stack_level, trade_stack, quality, cost, player_id, inventory_slot);			break;
		}

		account_query.run_query(m_QueryStr);
	}
	else
	{
		//we need to create a new entry for this item, that's ok
		sql_query ItemBuilder;
		ItemBuilder.Clear();

		switch (inventory_type)
		{
		case PLAYER_INVENTORY:
			ItemBuilder.SetTable("avatar_inventory_items");
			break;

		case PLAYER_VAULT:
			ItemBuilder.SetTable("avatar_vault_items");
			break;
		}

		ItemBuilder.AddData("avatar_id", player_id);
		ItemBuilder.AddData("item_id", item_id);
		ItemBuilder.AddData("stack_level", stack_level);
		ItemBuilder.AddData("trade_stack", trade_stack);
		ItemBuilder.AddData("quality", quality);
		ItemBuilder.AddData("inventory_slot", inventory_slot);
		ItemBuilder.AddData("cost", cost);

		if (!account_query.run_query(ItemBuilder.CreateQuery()))
		{
			LogMessage("Could not save Inventory Info for id %d [item id %d]\n", player_id, item_id );
		}
	}
}

void SaveManager::HandleChangeEquipment(long player_id, short bytes, unsigned char *data)
{
	//player has just had an equipment change
	sql_query_c account_query (&m_SQL_Conn);
	sql_result_c result;

	u8 equipment_slot = *((u8 *) &data[0]);
	float quality		 = *((float *) &data[1]);
	long  item_id		 = *((long *)  &data[5]);

	sprintf(m_QueryStr, "SELECT * FROM net7_user.avatar_equipment WHERE avatar_id = '%d' AND equipment_slot = '%d'", player_id, equipment_slot);

	//does this item exist in the DB?
	account_query.execute(m_QueryStr);
	account_query.store(&result);

	if (result.n_rows() != 0)
	{
		//yes, just update the equipment slot
		sprintf(m_QueryStr, "UPDATE net7_user.avatar_equipment SET item_id = '%d', quality = '%f' WHERE avatar_id = '%d' AND equipment_slot = '%d'",
			item_id, quality, player_id, equipment_slot);

		account_query.run_query(m_QueryStr);
	}
	else
	{
		//we need to create a new entry for this item, that's ok
		sql_query EquipBuilder;
		EquipBuilder.Clear();
		EquipBuilder.SetTable("avatar_equipment");

		EquipBuilder.AddData("avatar_id", player_id);
		EquipBuilder.AddData("item_id", item_id);
		EquipBuilder.AddData("quality", quality);
		EquipBuilder.AddData("equipment_slot", equipment_slot);

		if (!account_query.run_query(EquipBuilder.CreateQuery()))
		{
			LogMessage("Could not save Equip Info for id %d\n", player_id );
		}
	}
}

void SaveManager::HandleChangeAmmo(long player_id, short bytes, unsigned char *data)
{
	//player has just had an ammo change
	sql_query_c account_query (&m_SQL_Conn);
	sql_result_c result;

	u8 equipment_slot = *((u8 *) &data[0]);
	short ammo_stack	 = *((short *) &data[1]);
	float quality		 = *((float *) &data[3]);
	long  item_id		 = *((long *)  &data[7]);

	sprintf(m_QueryStr, "SELECT * FROM net7_user.avatar_ammo WHERE avatar_id = '%d' AND equipment_slot = '%d'", player_id, equipment_slot);

	//does this item exist in the DB?
	account_query.execute(m_QueryStr);
	account_query.store(&result);

	if (result.n_rows() != 0)
	{
		//yes, just update the equipment slot
		sprintf(m_QueryStr, "UPDATE net7_user.avatar_ammo SET item_id = '%d', quality = '%f', ammo_stack = '%d' WHERE avatar_id = '%d' AND equipment_slot = '%d'",
			item_id, quality, ammo_stack, player_id, equipment_slot);

		account_query.run_query(m_QueryStr);
	}
	else
	{
		//we need to create a new entry for this item, that's ok
		sql_query EquipBuilder;
		EquipBuilder.Clear();
		EquipBuilder.SetTable("avatar_ammo");

		EquipBuilder.AddData("avatar_id", player_id);
		EquipBuilder.AddData("item_id", item_id);
		EquipBuilder.AddData("quality", quality);
		EquipBuilder.AddData("equipment_slot", equipment_slot);
		EquipBuilder.AddData("ammo_stack", ammo_stack);

		if (!account_query.run_query(EquipBuilder.CreateQuery()))
		{
			LogMessage("Could not save Ammo Info for id %d\n", player_id );
		}
	}
}

void SaveManager::HandleAwardXP(long player_id, short bytes, unsigned char *data)
{
	//player just received XP, bar is now at a new level
	sql_query_c account_query (&m_SQL_Conn);

	u8 xp_type = *((u8 *) &data[0]);
	float new_level = *((float *) &data[1]);

	switch (xp_type)
	{
    case XP_COMBAT:
		sprintf(m_QueryStr, "UPDATE net7_user.avatar_level_info SET combat_bar_level = '%f' WHERE avatar_id = '%d'", new_level, player_id);
        break;

    case XP_EXPLORE:
		sprintf(m_QueryStr, "UPDATE net7_user.avatar_level_info SET explore_bar_level = '%f' WHERE avatar_id = '%d'", new_level, player_id);
        break;

    case XP_TRADE:
		sprintf(m_QueryStr, "UPDATE net7_user.avatar_level_info SET trade_bar_level = '%f' WHERE avatar_id = '%d'", new_level, player_id);
		break;
	}

	account_query.run_query(m_QueryStr);
}

void SaveManager::HandleUpdateDatabase(long player_id, short bytes, unsigned char *data)
{
	//player just changed sectors or logged out
	sql_query_c account_query (&m_SQL_Conn);

	u32 sector_id = *((u32 *) &data[0]);

	sprintf(m_QueryStr, "UPDATE net7_user.avatar_info SET sector = '%d' WHERE avatar_id = '%d'", sector_id, player_id);

	account_query.run_query(m_QueryStr);
}

void SaveManager::HandleCreditChange(long player_id, short bytes, unsigned char *data)
{
	//player just received XP, bar is now at a new level
	sql_query_c account_query (&m_SQL_Conn);

	u64 credits = *((u64 *) &data[0]);

	sprintf(m_QueryStr, "UPDATE net7_user.avatar_level_info SET credits = '%I64d' WHERE avatar_id = '%d'", credits, player_id);

	account_query.run_query(m_QueryStr);
}

void SaveManager::HandleStorePosition(long player_id, short bytes, unsigned char *data)
{
	//get the position stored
	//get position out of data
	float position[3];
	float orientation[4];
	long sector_id;

	position[0] = *((float *) &data[0]);
	position[1] = *((float *) &data[4]);
	position[2] = *((float *) &data[8]);

	orientation[0] = *((float *) &data[12]);
	orientation[1] = *((float *) &data[16]);
	orientation[2] = *((float *) &data[20]);
	orientation[3] = *((float *) &data[24]);

	sector_id = *((long *) &data[28]);

	//now store data into DB

	sql_query_c account_query (&m_SQL_Conn);

	sprintf(m_QueryStr, "UPDATE net7_user.avatar_position SET posx = '%.2f', posy = '%.2f', posz = '%.2f', ori_w = '%f', ori_x = '%f', ori_y = '%f', ori_z = '%f', sector_id = '%d' WHERE avatar_id = '%d'",
		position[0], position[1], position[2], orientation[0], orientation[1], orientation[2], orientation[3], sector_id, player_id);

	account_query.run_query(m_QueryStr);
}

void SaveManager::HandleAdvanceMission(long player_id, short bytes, unsigned char *data)
{
	//player has just either been awarded a mission or has advanced in one
	sql_query_c account_query (&m_SQL_Conn);
	sql_result_c result;

	u8		mission_slot	= *((u8 *) &data[0]);
	long	mission_id		= *((long *) &data[1]);
	short	mission_stage	= *((short *) &data[5]);

	sprintf(m_QueryStr, "SELECT * FROM net7_user.avatar_mission_progress WHERE avatar_id = '%d' AND mission_slot = '%d'", player_id, mission_slot);

	//does this item exist in the DB?
	account_query.execute(m_QueryStr);
	account_query.store(&result);

	if (result.n_rows() != 0)
	{
		//yes, just update this mission and blank the mission flags
		sprintf(m_QueryStr, "UPDATE net7_user.avatar_mission_progress SET stage_num = '%d', mission_flags = '0' WHERE avatar_id = '%d' AND mission_slot = '%d'",
			mission_stage, player_id, mission_slot);

		account_query.run_query(m_QueryStr);
	}
	else
	{
		//we need to create a new entry for this mission
		sql_query MissionBuilder;
		MissionBuilder.Clear();
		MissionBuilder.SetTable("avatar_mission_progress");

		MissionBuilder.AddData("avatar_id", player_id);
		MissionBuilder.AddData("mission_id", mission_id);
		MissionBuilder.AddData("mission_slot", mission_slot);
		MissionBuilder.AddData("mission_flags", 0);
		MissionBuilder.AddData("stage_num", mission_stage);

		if (!account_query.run_query(MissionBuilder.CreateQuery()))
		{
			LogMessage("Could not save Mission Info for id %d\n", player_id );
		}
	}
}

void SaveManager::HandleAdvanceMissionFlags(long player_id, short bytes, unsigned char *data)
{
	//player has just had mission flags changed. This is only relevant if the mission exists in the DB
	//so just do a simple DB commit.
	sql_query_c account_query (&m_SQL_Conn);
	sql_result_c result;

	u8		mission_slot	= *((u8 *) &data[0]);
	long	mission_flags	= *((long *) &data[1]);

	sprintf(m_QueryStr, "UPDATE net7_user.avatar_mission_progress SET mission_flags = '%d' WHERE avatar_id = '%d' AND mission_slot = '%d'",
		mission_flags, player_id, mission_slot);

	account_query.run_query(m_QueryStr);
}

void SaveManager::HandleHullUpgrade(long player_id, short bytes, unsigned char *data)
{
	//player has just had an hull upgrade change (or this is a new player)
	sql_query_c account_query (&m_SQL_Conn);
	sql_result_c result;

	u8 player_rank_name   = *((u8 *) &data[0]);
	u8 hull_upgrade_level = *((u8 *) &data[1]);
	float max_hull_points = *((float *) &data[2]);
	u8 cargo_space 		  = *((u8 *) &data[6]);
	u8 weapon_slots		  = *((u8 *) &data[7]);
	u8 device_slots		  = *((u8 *) &data[8]);
	u8 warp_power_level	  = *((u8 *) &data[9]);
	u8 engine_thrust_type = *((u8 *) &data[10]);


	sprintf(m_QueryStr, "SELECT * FROM net7_user.avatar_level_info WHERE avatar_id = '%d'", player_id);

	//does this item exist in the DB?
	account_query.execute(m_QueryStr);
	account_query.store(&result);

	if (result.n_rows() != 0)
	{
		//yes, just update the level info row
		sprintf(m_QueryStr, "UPDATE net7_user.avatar_level_info SET player_rank_name = '%d', hull_upgrade_level = '%d', max_hull_points = '%.2f', cargo_space = '%d', weapon_slots = '%d', device_slots = '%d' WHERE avatar_id = '%d'",
			player_rank_name, hull_upgrade_level, max_hull_points, cargo_space, weapon_slots, device_slots, player_id);

		account_query.run_query(m_QueryStr);

		sprintf(m_QueryStr, "UPDATE net7_user.avatar_level_info SET engine_thrust_type = '%d', warp_power_level = '%d' WHERE avatar_id = '%d'",
			engine_thrust_type, warp_power_level, player_id);

		account_query.run_query(m_QueryStr);
	}
	else
	{
		//we need to create a new entry for this player, that's ok
		sql_query LevelBuilder;
		LevelBuilder.Clear();
		LevelBuilder.SetTable("avatar_level_info");

		LevelBuilder.AddData("avatar_id", player_id);
		LevelBuilder.AddData("player_rank_name", player_rank_name);
		LevelBuilder.AddData("hull_upgrade_level", hull_upgrade_level);
		LevelBuilder.AddData("max_hull_points", max_hull_points);
		LevelBuilder.AddData("cargo_space", cargo_space);
		LevelBuilder.AddData("weapon_slots", weapon_slots);
		LevelBuilder.AddData("device_slots", device_slots);
		LevelBuilder.AddData("combat_bar_level", 0.0f);
		LevelBuilder.AddData("explore_bar_level", 0.0f);
		LevelBuilder.AddData("trade_bar_level", 0.0f);
		LevelBuilder.AddData("warp_power_level", warp_power_level);
		LevelBuilder.AddData("engine_thrust_type", engine_thrust_type);
		LevelBuilder.AddData("hull_points", max_hull_points);
		LevelBuilder.AddData("credits", 0);
		LevelBuilder.AddData("skill_points", 0); //skill

		if (!account_query.run_query(LevelBuilder.CreateQuery()))
		{
			LogMessage("Could not save Avatar level Info for id %d\n", player_id );
		}
	}
}

void SaveManager::HandleHullLevelChange(long player_id, short bytes, unsigned char *data)
{
	//player has just had an hull level change
	sql_query_c account_query (&m_SQL_Conn);
	sql_result_c result;

	float hull_points = *((float *) &data[0]);

	sprintf(m_QueryStr, "UPDATE net7_user.avatar_level_info SET hull_points = '%.2f' WHERE avatar_id = '%d'",
		hull_points, player_id);

	account_query.run_query(m_QueryStr);
}

void SaveManager::HandleMissionComplete(long player_id, short bytes, unsigned char *data)
{
	//player has just completed a mission
	sql_query_c account_query (&m_SQL_Conn);
	sql_result_c result;

	long mission_id =  *((long *) &data[0]);
	u8 mission_flags = *((u8 *)   &data[4]);

	//first remove mission progress
	sprintf(m_QueryStr, "DELETE FROM net7_user.avatar_mission_progress WHERE avatar_id = '%d' AND mission_id = '%d'",
		player_id, mission_id);
	account_query.run_query(m_QueryStr);

	sprintf(m_QueryStr, "SELECT * FROM net7_user.missions_completed WHERE avatar_id = '%d' AND mission_id = '%d'", player_id, mission_id);

	//does this item exist in the DB?
	account_query.execute(m_QueryStr);
	account_query.store(&result);

	if (result.n_rows() != 0)
	{
		//yes, just update the info row
		sprintf(m_QueryStr, "UPDATE net7_user.missions_completed SET mission_completion_flags = '%d' WHERE avatar_id = '%d' AND mission_id = '%d'",
			mission_flags, player_id, mission_id);

		account_query.run_query(m_QueryStr);
	}
	else
	{
		sql_query MissionBuilder;
		MissionBuilder.Clear();
		MissionBuilder.SetTable("missions_completed");

		MissionBuilder.AddData("avatar_id", player_id);
		MissionBuilder.AddData("mission_id", mission_id);
		MissionBuilder.AddData("mission_completion_flags", mission_flags);

		account_query.run_query(MissionBuilder.CreateQuery());
	}
}

//this method leaves the base character design, name and ship but resets them back to a starting condition.
void SaveManager::HandleWipeCharacter(long player_id)
{
	//ok remove all records of inventory, equipment, ammo, skills, levels & rank info
	sql_query_c account_query (&m_SQL_Conn);

	//remove position info
	sprintf(m_QueryStr, "DELETE FROM `avatar_position` WHERE `avatar_id` = '%d'", player_id);
	account_query.run_query(m_QueryStr);

	sprintf(m_QueryStr, "DELETE FROM `avatar_level_info` WHERE `avatar_id` = '%d'", player_id);
	account_query.run_query(m_QueryStr);

	sprintf(m_QueryStr, "DELETE FROM `avatar_ammo` WHERE `avatar_id` = '%d'", player_id);
	account_query.run_query(m_QueryStr);

	sprintf(m_QueryStr, "DELETE FROM `avatar_inventory_items` WHERE `avatar_id` = '%d'", player_id);
	account_query.run_query(m_QueryStr);

	sprintf(m_QueryStr, "DELETE FROM `avatar_vault_items` WHERE `avatar_id` = '%d'", player_id);
	account_query.run_query(m_QueryStr);

	sprintf(m_QueryStr, "DELETE FROM `avatar_equipment` WHERE `avatar_id` = '%d'", player_id);
	account_query.run_query(m_QueryStr);

	sprintf(m_QueryStr, "DELETE FROM `avatar_mission_progress` WHERE `avatar_id` = '%d'", player_id);
	account_query.run_query(m_QueryStr);

	sprintf(m_QueryStr, "DELETE FROM `missions_completed` WHERE `avatar_id` = '%d'", player_id);
	account_query.run_query(m_QueryStr);
	
	sprintf(m_QueryStr, "DELETE FROM `faction_data` WHERE `avatar_id` = '%d'", player_id);
	account_query.run_query(m_QueryStr);
	
	sprintf(m_QueryStr, "DELETE FROM `avatar_exploration` WHERE `avatar_id` = '%d'", player_id);
	account_query.run_query(m_QueryStr);

	sprintf(m_QueryStr, "DELETE FROM `avatar_skill_levels` WHERE `avatar_id` = '%d'", player_id);
	account_query.run_query(m_QueryStr);

	sprintf(m_QueryStr, "UPDATE net7_user.avatar_info SET combat = '0', explore = '0', trade = '0' WHERE avatar_id = '%d'", player_id);
	account_query.run_query(m_QueryStr);
}

//this method removes all trace of the avatar in the database, must be used when the avatar is deleted via the client avatar character selector.
void SaveManager::HandleFullWipeCharacter(long player_id)
{
	sql_query_c account_query (&m_SQL_Conn);

	//remove position info
	sprintf(m_QueryStr, "DELETE FROM `avatar_position` WHERE `avatar_id` = '%d'", player_id);
	account_query.run_query(m_QueryStr);

	sprintf(m_QueryStr, "DELETE FROM `avatar_level_info` WHERE `avatar_id` = '%d'", player_id);
	account_query.run_query(m_QueryStr);

	sprintf(m_QueryStr, "DELETE FROM `avatar_ammo` WHERE `avatar_id` = '%d'", player_id);
	account_query.run_query(m_QueryStr);

	sprintf(m_QueryStr, "DELETE FROM `avatar_inventory_items` WHERE `avatar_id` = '%d'", player_id);
	account_query.run_query(m_QueryStr);

	sprintf(m_QueryStr, "DELETE FROM `avatar_vault_items` WHERE `avatar_id` = '%d'", player_id);
	account_query.run_query(m_QueryStr);

	sprintf(m_QueryStr, "DELETE FROM `avatar_equipment` WHERE `avatar_id` = '%d'", player_id);
	account_query.run_query(m_QueryStr);

	sprintf(m_QueryStr, "DELETE FROM `avatar_mission_progress` WHERE `avatar_id` = '%d'", player_id);
	account_query.run_query(m_QueryStr);

	sprintf(m_QueryStr, "DELETE FROM `missions_completed` WHERE `avatar_id` = '%d'", player_id);
	account_query.run_query(m_QueryStr);
	
	sprintf(m_QueryStr, "DELETE FROM `faction_data` WHERE `avatar_id` = '%d'", player_id);
	account_query.run_query(m_QueryStr);
	
	sprintf(m_QueryStr, "DELETE FROM `avatar_exploration` WHERE `avatar_id` = '%d'", player_id);
	account_query.run_query(m_QueryStr);

	sprintf(m_QueryStr, "DELETE FROM `avatar_skill_levels` WHERE `avatar_id` = '%d'", player_id);
	account_query.run_query(m_QueryStr);
}

void SaveManager::HandleFullFactionWipe(long player_id, short bytes, unsigned char *data)
{
	sql_query_c account_query (&m_SQL_Conn);

	//remove all faction data

	sprintf(m_QueryStr, "DELETE FROM faction_data");
	account_query.run_query(m_QueryStr);
}

void SaveManager::HandleDiscoverNav(long player_id, short bytes, unsigned char *data)
{
	//player has just discovered a nav, make a record
	sql_query_c account_query (&m_SQL_Conn);

	long object_uid =  *((long *) &data[0]);

	sql_query ExploreBuilder;
	ExploreBuilder.Clear();
	ExploreBuilder.SetTable("avatar_exploration");

	ExploreBuilder.AddData("avatar_id", player_id);
	ExploreBuilder.AddData("object_id", object_uid);
	ExploreBuilder.AddData("explore_flags", DISCOVER_NAV);

	account_query.run_query(ExploreBuilder.CreateQuery());
}

void SaveManager::HandleExploreNav(long player_id, short bytes, unsigned char *data)
{
	//player has just explored a nav, update the record
	sql_query_c account_query (&m_SQL_Conn);

	long object_uid =  *((long *) &data[0]);

	sprintf(m_QueryStr, "UPDATE net7_user.avatar_exploration SET explore_flags = '%d' WHERE avatar_id = '%d' AND object_id = '%d'",
		EXPLORE_NAV, player_id, object_uid);

	account_query.run_query(m_QueryStr);
}

void SaveManager::HandleSetSkillPoints(long player_id, short bytes, unsigned char *data)
{
	//set skill points
	sql_query_c account_query (&m_SQL_Conn);

	long skill_points =  *((long *) &data[0]);

	sprintf(m_QueryStr, "UPDATE net7_user.avatar_level_info SET skill_points = '%d' WHERE avatar_id = '%d'",
		skill_points, player_id);

	account_query.run_query(m_QueryStr);
}

void SaveManager::HandleSetRegisteredStarbase(long player_id, short bytes, unsigned char *data)
{
	//set skill points
	sql_query_c account_query (&m_SQL_Conn);

	long registered_starbase =  *((long *) &data[0]);

	sprintf(m_QueryStr, "UPDATE net7_user.avatar_level_info SET registered_starbase = '%d' WHERE avatar_id = '%d'",
		registered_starbase, player_id);

	account_query.run_query(m_QueryStr);
}

void SaveManager::HandleSaveEnergyLevels(long player_id, short bytes, unsigned char *data)
{
	//set skill points
	sql_query_c account_query (&m_SQL_Conn);

	float energy =  *((float *) &data[0]);
	float shield =  *((float *) &data[4]);

	if (_isnan(energy)) energy = 0.1f; //last ditch attempt to stop a crash.
	if (_isnan(shield)) shield = 0.1f;

	sprintf(m_QueryStr, "UPDATE net7_user.avatar_level_info SET reactor_level = '%f', shield_level = '%f' WHERE avatar_id = '%d'",
		energy, shield, player_id);

	account_query.run_query(m_QueryStr);
}

void SaveManager::HandleFactionUpdate(long player_id, short bytes, unsigned char *data)
{
	//player has just completed a mission
	sql_query_c account_query (&m_SQL_Conn);
	sql_result_c result;

	long faction_id =  *((short *) &data[0]);
	float faction_value = *((float *) &data[2]);
	long faction_order = -1;

	if (bytes > 6)
	{
		faction_order = *((short *) &data[6]);
	}

	sprintf(m_QueryStr, "SELECT * FROM net7_user.faction_data WHERE avatar_id = '%d' AND faction_id = '%d'", player_id, faction_id);

	//does this item exist in the DB?
	account_query.execute(m_QueryStr);
	account_query.store(&result);

	if (result.n_rows() != 0)
	{
		//yes, just update the info row
		if (faction_order != -1)
		{
			sprintf(m_QueryStr, "UPDATE net7_user.faction_data SET faction_order = '%d' AND faction_value = %.2f WHERE avatar_id = '%d' AND faction_id = '%d'",
				faction_order, faction_value, player_id, faction_id);
		}
		else
		{
			sprintf(m_QueryStr, "UPDATE net7_user.faction_data SET faction_value = %.2f WHERE avatar_id = '%d' AND faction_id = '%d'",
				faction_value, player_id, faction_id);
		}

		account_query.run_query(m_QueryStr);
	}
	else
	{
		if (faction_order == -1)
		{
			LogMessage("FACTION SETUP ERROR: Adding New faction, order = -1... Faction ID = %d, value = %.2f\n", faction_id, faction_value);
		}

		sql_query FactionBuilder;
		FactionBuilder.Clear();
		FactionBuilder.SetTable("faction_data");

		FactionBuilder.AddData("avatar_id", player_id);
		FactionBuilder.AddData("faction_id", faction_id);
		FactionBuilder.AddData("faction_value", faction_value);
		FactionBuilder.AddData("faction_order", faction_order);

		account_query.run_query(FactionBuilder.CreateQuery());
	}
}