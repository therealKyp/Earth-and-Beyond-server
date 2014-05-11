///////////////////////////////////////////////////////////////////
//
// PlayerSaves.cpp
//
// This file contains all the methods needed to save game status.
//
///////////////////////////////////////////////////////////////////

#include "PlayerClass.h"
#include "ServerManager.h"
#include "StaticData.h"
#include "mysql/mysqlplus.h"
#include "SaveManager.h"
#include "PacketMethods.h"

#ifndef USE_MYSQL_ACCOUNT_DATA
#error "BUILD ERROR: USE_MYSQL_ACCOUNT IS NOW ESSENTIAL"
#endif

extern sql_connection_c m_SQL_Conn;

void Player::SavePosition()
{
    if (m_Hijackee)
    {
        return;
    }

	unsigned char pos_data[32];
	float *ori = Orientation();
    *((float *) &pos_data[0]) = PosX();
    *((float *) &pos_data[4]) = PosY();
    *((float *) &pos_data[8]) = PosZ();
    *((float *) &pos_data[12]) = ori[0];
    *((float *) &pos_data[16]) = ori[1];
    *((float *) &pos_data[20]) = ori[2];
	*((float *) &pos_data[24]) = ori[3];

	*((long *) &pos_data[28]) = PlayerIndex()->GetSectorNum();

	g_SaveMgr->AddSaveMessage(SAVE_CODE_STORE_POSITION, m_CharacterID, 32, pos_data);
}

void Player::CreatePositionSave()
{
	char query_str[300];
	sql_query_c account_query (&m_SQL_Conn);

	sprintf(query_str, "DELETE FROM `avatar_position` WHERE `avatar_id` = '%ld'", m_CharacterID);
	account_query.run_query(query_str);

	sql_query PositionBuilder;
	PositionBuilder.Clear();
	PositionBuilder.SetTable("avatar_position");

	PositionBuilder.AddData("avatar_id", m_CharacterID);
	PositionBuilder.AddData("posx", 0);
	PositionBuilder.AddData("posy", 0);
	PositionBuilder.AddData("posz", 0);
	PositionBuilder.AddData("ori_w", 0);
	PositionBuilder.AddData("ori_x", 0);
	PositionBuilder.AddData("ori_y", 0);
	PositionBuilder.AddData("ori_z", 0);
	PositionBuilder.AddData("sector_id", (long)PlayerIndex()->GetSectorNum());

	if (!account_query.run_query(PositionBuilder.CreateQuery()))
	{
		LogMessage("Could not create position save for player %s [%d]\n", Name(), m_CharacterID);
	}
}

bool Player::LoadPosition()
{
    bool success = false;

    if (CharacterID() == 0)
	{
		LogMessage("Null avatar_id in LoadPosition, skipping load.");
		return false;
	}

	char query_str[256];
	sql_query_c account_query (&m_SQL_Conn);
	sql_result_c account_result;
    sql_row_c account_row;

	sprintf(query_str, "SELECT * FROM `avatar_position` WHERE `avatar_id` = '%ld'", m_CharacterID);

	if (!account_query.run_query(query_str) || account_query.n_rows() == 0)
	{
		return false;
	}

    account_query.store(&account_result);
	account_result.fetch_row(&account_row);

	if ((unsigned int)account_row["sector_id"] != PlayerIndex()->GetSectorNum()) //make sure this position applies to current sector
	{
		return false;
	}

	float posx, posy, posz, oriw, orix, oriy, oriz;

	posx = (float)account_row["posx"];
	posy = (float)account_row["posy"];
	posz = (float)account_row["posz"];

	oriw = (float)account_row["ori_w"];
	orix = (float)account_row["ori_x"];
	oriy = (float)account_row["ori_y"];
	oriz = (float)account_row["ori_z"];

	SetPosition(posx, posy, posz);
	SetOrientation(oriw, orix, oriy, oriz);

	success = true;

	return success;
}

// This may be called from the Global Server or Sector Server
bool Player::ReadSavedData()
{
	bool success = false;
	int i = 0;

	//see if we need to initialise or re-load
	//try to load in the avatar_level info, if this exists, character has been initialised
	char query_str[256];
	sql_query_c account_query (&m_SQL_Conn);
	sql_result_c result;
    sql_row_c account_row;

	sprintf(query_str, "SELECT * FROM `avatar_level_info` WHERE `avatar_id` = '%ld'", m_CharacterID);

	account_query.execute(query_str);
	account_query.store(&result);

	if (account_query.n_rows() == 0)
	{
		//we need to initialise the data
		ReInitializeSavedData();
	}
	else
	{
		//do a normal player load
		ReloadSavedData();
	}

    return (success);
}

void Player::SetHullUpgrade()
{
	long upgrade = PlayerIndex()->RPGInfo.GetHullUpgradeLevel();

	m_Database.ship_info.hull         = BaseHullAsset[m_Database.ship_data.race * 3 + m_Database.ship_data.hull];
	m_Database.ship_info.profession   = BaseProfAsset[m_Database.ship_data.race * 3 + m_Database.ship_data.profession];
	m_Database.ship_info.wing         = BaseWingAsset[m_Database.ship_data.race * 3 + m_Database.ship_data.wing];
	m_Database.ship_info.engine       = BaseEngineAsset[m_Database.ship_data.race];

	switch (upgrade)
	{
	case 3:
	case 4:
		//Take the assets from static data for consitency
		m_Database.ship_info.hull += 1;
		m_Database.ship_info.profession += 1;
		m_Database.ship_info.wing += 1;
		m_Database.ship_info.engine += 1;
		break;

	case 5:
	case 6:
		//Take the assets from static data for consitency
		m_Database.ship_info.hull += 2;
		m_Database.ship_info.profession += 2;
		m_Database.ship_info.wing += 2;
		m_Database.ship_info.engine += 2;

	default:
		break;
	}
}

//TODO: this should all be done via the savemanager, it's here really for prototyping.
//then again, providing we keep the ptr to the database it should be ok just for reloading
void Player::ReloadSavedData()
{
	char query_str[256];
	sql_query_c account_query (&m_SQL_Conn);
	sql_result_c account_result;
    sql_row_c account_row;
	m_WeaponSlots = 0;
	m_DeviceSlots = 0;

	char blank = 0;

    static const char *faction_list[] =
    {   "TW", "TT", "TE",
        "JW", "JT", "JE",
        "PW", "PT", "PE",
        "Kokura", "Mordana", "FS",
        "V'Rix", "Anseria", "Pirate",
        "Bogeril", "JingLeung", "GC",
        "N7", "TerranPsi", "GETCo",
        "Alliance"
    };

    static const char *faction2_list[] =
    {
        "EarthCorps Warrior",   // TW
        "InfiniCorp Traders",   // TT
        "Hyperia Explorers",    // TE
        "Shinwa Warriors",      // JW
        "Sharim Traders",       // JT
        "Sha'ha'dem Explorers", // JE
        "Centuriata Warriors",  // PW
        "Collegia Traders",     // PT
        "Sabine Explorers"      // PE
    };

    long i, class_index = ClassIndex(), race = Race();

	unsigned long myTime = GetNet7TickCount();

/*---------------------------------------------------------------------*/

    PlayerIndex()->Reset();

	//get credits
	sprintf(query_str, "SELECT * FROM `avatar_level_info` WHERE `avatar_id` = '%ld'", m_CharacterID);

	account_query.run_query(query_str);
    account_query.store(&account_result);
	account_result.fetch_row(&account_row);

	PlayerIndex()->SetCredits((unsigned long)account_row["credits"]);

    ShipIndex()->Reset();

	ShipIndex()->SetName(m_Database.avatar.avatar_first_name);
	ShipIndex()->SetOwner(m_Database.avatar.avatar_first_name);

	unsigned long hull_level = (unsigned long)account_row["hull_upgrade_level"];

	PlayerIndex()->RPGInfo.SetHullUpgradeLevel(hull_level);

    ShipIndex()->SetRank(GetRank());

	float hull_points = (float)account_row["hull_points"];

	ShipIndex()->SetHullPoints(hull_points);
	ShipIndex()->SetMaxHullPoints((float)account_row["max_hull_points"]);

	ShipIndex()->SetAcceleration(117.5f);
	ShipIndex()->SetMaxTiltRate(1.158f);
	ShipIndex()->SetMaxTurnRate(1.158f);
	PlayerIndex()->RPGInfo.SetRace(m_Database.ship_data.race);
	PlayerIndex()->RPGInfo.SetProfession(m_Database.ship_data.profession);
	PlayerIndex()->RPGInfo.SetSkillPowerUpAbilityNumber(-1);
	ShipIndex()->SetWarpAvailable(2);

	ShipIndex()->Inventory.SetEquipMountModel("tvf01_1");

	//now set up slots
	//work out cargo space from scratch
	ShipIndex()->Inventory.SetCargoSpace(BaseCargo[class_index] + 2*hull_level);

	for(i=0;i<(long)ShipIndex()->Inventory.GetCargoSpace();i++)
	{
		ShipIndex()->Inventory.CargoInv.Item[i].Empty();
    }

    for(i=0;i<96;i++)
	{
		PlayerIndex()->SecureInv.Item[i].SetItemTemplateID(-1);
    }

	ShipIndex()->CurrentStats.SetScanRange(BaseScanRange[class_index]);
	ShipIndex()->CurrentStats.SetVisibility(BaseVisableRange[class_index]);

	//weapons & mounts
	//TODO: store mount positions so players can customise mounts
	long weapon_count = account_row["weapon_slots"];
	long device_count = account_row["device_slots"];

	ResetWeaponMounts();
	ResetDeviceMounts();

	ShipIndex()->Inventory.SetFutureDevices(MaxDeviceSlots[class_index]);
	ShipIndex()->Inventory.SetFutureWeapons(MaxWeaponSlots[class_index]);

	ShipIndex()->Inventory.Mounts.SetMount(0, ShieldMount);
	ShipIndex()->Inventory.Mounts.SetMount(1, ReactorMount);
	ShipIndex()->Inventory.Mounts.SetMount(2, EngineMount);

	//now set EXP bars
	float combatxp = (float)account_row["combat_bar_level"];
	float explorexp = (float)account_row["explore_bar_level"];
	float tradexp = (float)account_row["trade_bar_level"];

	PlayerIndex()->RPGInfo.SetCombatXP(combatxp);
	PlayerIndex()->RPGInfo.SetExploreXP(explorexp);
	PlayerIndex()->RPGInfo.SetTradeXP(tradexp);

	//skillpoints
	PlayerIndex()->RPGInfo.SetSkillPoints((unsigned long)account_row["skill_points"]);

	//warp power level
	ShipIndex()->CurrentStats.SetWarpPowerLevel(account_row["warp_power_level"]);
	unsigned long thrust_type = (unsigned long)account_row["engine_thrust_type"];
	ShipIndex()->SetEngineTrailType(thrust_type);

	m_RegisteredSectorID = account_row["registered_starbase"];

	float energy_bar = (float)account_row["reactor_level"];
	float shield_bar = (float)account_row["shield_level"];

	//now add equipment, new table.
	sprintf(query_str, "SELECT * FROM `avatar_equipment` WHERE `avatar_id` = '%ld'", m_CharacterID);

	account_query.run_query(query_str);
    account_query.store(&account_result);

	for(i=0;i<account_result.n_rows();i++)
	{
		account_result.fetch_row(&account_row);
		long equipment_slot = account_row["equipment_slot"];
		long item_template_id = account_row["item_id"];
		float quality = (float)account_row["quality"];
		const char *builder = account_row["builder_name"];
		if (!builder) builder = &blank;

		_Item new_equipment;
		memset(&new_equipment,0,sizeof(_Item));

		if (quality == 0.0f) quality = 1.0f; //This should not be required.

		_snprintf(new_equipment.BuilderName, 64, "%s", builder);
		new_equipment.Quality = quality;
		new_equipment.Structure = 1.0f;
		new_equipment.ItemTemplateID = item_template_id;
		new_equipment.StackCount = 1;
		memset(new_equipment.InstanceInfo, 0, 64);
		memset(new_equipment.ActivatedEffectInstanceInfo, 0, 64);
		memset(new_equipment.EquipEffectInstanceInfo, 0, 64);

		m_Equip[equipment_slot].Equip(&new_equipment);
	}

	ShipIndex()->Shield.SetStartValue(shield_bar);
	ShipIndex()->Energy.SetStartValue(energy_bar);

	if (hull_points == 0.0f)
	{
		ShipIndex()->SetIsIncapacitated(true);

		// Stop regen
		RemoveEnergy(0);
		ShipIndex()->Energy.SetStartValue(0);
		ShipIndex()->Shield.SetStartValue(0);
		ShipIndex()->Shield.SetEndTime(GetNet7TickCount());		// Set end time now!
		m_ShieldRecharge = 0;

        ImmobilisePlayer();
		ShieldUpdate(GetNet7TickCount(), 0, 0.0f);
	}
	else
	{
		ShieldUpdate(GetNet7TickCount(), m_ShieldChargeRate, ShipIndex()->Shield.GetStartValue());
	}

	//now add ammo
    for(i=0;i<20;i++)
    {
        ShipIndex()->Inventory.AmmoInv.Item[i].SetItemTemplateID(-1);
    }

	sprintf(query_str, "SELECT * FROM `avatar_ammo` WHERE `avatar_id` = '%ld'", m_CharacterID);

	account_query.run_query(query_str);
    account_query.store(&account_result);

	for(i=0;i<account_result.n_rows();i++)
	{
		account_result.fetch_row(&account_row);
		long equipment_slot = account_row["equipment_slot"];
		long item_template_id = account_row["item_id"];
		float quality = (float)account_row["quality"];
		const char *builder = account_row["builder_name"];
		if (!builder) builder = &blank;
		long stack = account_row["ammo_stack"];

		if (item_template_id > -1)
		{
			_Item ammo;
			memset(&ammo,0,sizeof(_Item));

			if (quality == 0.0f) quality = 1.0f; //This should not be required.

			_snprintf(ammo.BuilderName, 64, "%s", builder);
			ammo.Quality = quality;
			ammo.Structure = 1.0f;
			ammo.ItemTemplateID = item_template_id;
			ammo.StackCount = stack;

			m_Equip[equipment_slot].Equip(&ammo);
		}
	}

	//now add cargo
	for(u32 j=0;j<ShipIndex()->Inventory.GetCargoSpace();j++)
	{
		ShipIndex()->Inventory.CargoInv.Item[j].SetItemTemplateID(-1);
	}

	sprintf(query_str, "SELECT * FROM `avatar_inventory_items` WHERE `avatar_id` = '%ld'", m_CharacterID);

	account_query.run_query(query_str);
    account_query.store(&account_result);

	for(i=0;i<account_result.n_rows();i++)
	{
		account_result.fetch_row(&account_row);
		long inventory_slot = account_row["inventory_slot"];
		long item_template_id = account_row["item_id"];
		float quality = (float)account_row["quality"];
		const char *builder = account_row["builder_name"];
		long stack = account_row["stack_level"];
		long trade_stack = account_row["trade_stack"];
		long cost = account_row["cost"];
		if (!builder) builder = &blank;

		if (stack == 0) stack = 1; //this is to fix an old bug with equipment stack level
		if (quality == 0.0f) quality = 1.0f; //This should not be required.

		ShipIndex()->Inventory.CargoInv.Item[inventory_slot].SetItemTemplateID(item_template_id);
		ShipIndex()->Inventory.CargoInv.Item[inventory_slot].SetQuality(quality);
		ShipIndex()->Inventory.CargoInv.Item[inventory_slot].SetBuilderName(builder);
		ShipIndex()->Inventory.CargoInv.Item[inventory_slot].SetStackCount(stack);
		ShipIndex()->Inventory.CargoInv.Item[inventory_slot].SetTradeStack(trade_stack);
		ShipIndex()->Inventory.CargoInv.Item[inventory_slot].SetPrice(cost);
	}

	//now add vault

	sprintf(query_str, "SELECT * FROM `avatar_vault_items` WHERE `avatar_id` = '%ld'", m_CharacterID);

	account_query.run_query(query_str);
    account_query.store(&account_result);

	for(i=0;i<account_result.n_rows();i++)
	{
		account_result.fetch_row(&account_row);
		long inventory_slot = account_row["inventory_slot"];
		long item_template_id = account_row["item_id"];
		float quality = (float)account_row["quality"];
		const char *builder = account_row["builder_name"];
		long stack = account_row["stack_level"];
		long trade_stack = account_row["trade_stack"];
		long cost = account_row["cost"];
		if (!builder) builder = &blank;

		if (stack == 0) stack = 1; //this is to fix an old bug with equipment stack level
		if (quality == 0.0f) quality = 1.0f; //This should not be required.

		PlayerIndex()->SecureInv.Item[inventory_slot].SetItemTemplateID(item_template_id);
		PlayerIndex()->SecureInv.Item[inventory_slot].SetQuality(quality);
		PlayerIndex()->SecureInv.Item[inventory_slot].SetBuilderName(builder);
		PlayerIndex()->SecureInv.Item[inventory_slot].SetStackCount(stack);
		PlayerIndex()->SecureInv.Item[inventory_slot].SetTradeStack(trade_stack);
		PlayerIndex()->SecureInv.Item[inventory_slot].SetPrice(cost);
	}

	//now set skills

	u32 Availability[4];
    SkillData *Skills = g_ServerMgr->m_SkillList;
    SkillClassData *ClassData = 0;

	for(i=0;i<64;i++)
	{
        // Check to see if this class has this skill
		if (Skills[i].ClassType[class_index].MaxLevel > 0)
		{
            ClassData = &Skills[i].ClassType[class_index];

            if (ClassData->Quested == 0 && ClassData->LevelAquired == 0)
            {
                // This skill is available to level up
				Availability[0] = 4;
				Availability[1] = 2;
				Availability[2] = 0;
				Availability[3] = 1;
            }
            else if (ClassData->Quested == 0 && ClassData->LevelAquired > 0)
            {
                // This skill is available but at a higher level
                // Not sure if this will ever be used
				Availability[0] = 4;
				Availability[1] = 1;
				Availability[2] = ClassData->LevelAquired;
				Availability[3] = 1;
            }
            else
            {
                // This skill is aquired via a quest (Quested == 1)
				Availability[0] = 3;
				Availability[1] = 0;
				Availability[2] = 0;
				Availability[3] = 0;
            }

			PlayerIndex()->RPGInfo.Skills.Skill[i].SetAvailability(Availability);
			PlayerIndex()->RPGInfo.Skills.Skill[i].SetMaxSkillLevel(ClassData->MaxLevel);
			PlayerIndex()->RPGInfo.Skills.Skill[i].SetQuestOnlyLevel(ClassData->MaxLevel);
			PlayerIndex()->RPGInfo.Skills.Skill[i].SetLastActivationTime(myTime);
        }
    }

	PlayerIndex()->RPGInfo.Skills.Skill[SKILL_BEAM_WEAPON].SetLevel(1);
	PlayerIndex()->RPGInfo.Skills.Skill[SKILL_DEVICE_TECH].SetLevel(1);
	PlayerIndex()->RPGInfo.Skills.Skill[SKILL_ENGINE_TECH].SetLevel(1);
	PlayerIndex()->RPGInfo.Skills.Skill[SKILL_REACTOR_TECH].SetLevel(1);
	PlayerIndex()->RPGInfo.Skills.Skill[SKILL_SHIELD_TECH].SetLevel(1);

	/*if (m_Database.ship_data.profession == 2)
	{
        Availability[0] = 4;
		Availability[1] = 2;
		Availability[2] = 0;
		Availability[3] = 1;

		PlayerIndex()->RPGInfo.Skills.Skill[SKILL_PROSPECT].SetAvailability(Availability);
		PlayerIndex()->RPGInfo.Skills.Skill[SKILL_PROSPECT].SetLevel(1);
		SaveNewSkillLevel(SKILL_PROSPECT, 1);
	}*/

	sprintf(query_str, "SELECT * FROM `avatar_skill_levels` WHERE `avatar_id` = '%ld'", m_CharacterID);

	account_query.run_query(query_str);
    account_query.store(&account_result);

	for(i=0;i<account_result.n_rows();i++)
	{
		account_result.fetch_row(&account_row);
		long skill_id = account_row["skill_id"];
		long skill_level = account_row["skill_level"];

        Availability[0] = 4;
		Availability[1] = 2;
		Availability[2] = 0;
		Availability[3] = 1;

		//see if this skill has reached max level
		ClassData = &Skills[skill_id].ClassType[class_index];

		if (skill_level >= ClassData->MaxLevel)
		{
			Availability[1] = SKILL_ERROR_MAXLVL;
			Availability[2] = 0;
			Availability[3] = 0;
			skill_level = ClassData->MaxLevel;
		}

		PlayerIndex()->RPGInfo.Skills.Skill[skill_id].SetAvailability(Availability);
		PlayerIndex()->RPGInfo.Skills.Skill[skill_id].SetLevel(skill_level);
		//UpgradeSkill(skill_id);
		SkillUpdateStats(skill_id);
	}

	UpdateSkills();

	//now set levels
	PlayerIndex()->RPGInfo.SetCombatLevel(ntohl(m_Database.info.combat_level));
	PlayerIndex()->RPGInfo.SetExploreLevel(ntohl(m_Database.info.explore_level));
	PlayerIndex()->RPGInfo.SetTradeLevel(ntohl(m_Database.info.trade_level));

	//now set faction

/*---------------------------------------------------------------------*/
// Factions - leave this for now until we start doing something with them

	PlayerIndex()->Reputation.SetAffilitation(faction2_list[class_index]);

    for (i=0;i<9;i++)
    {
		PlayerIndex()->Reputation.Factions.Faction[i].SetName(faction_list[i]);
		PlayerIndex()->Reputation.Factions.Faction[i].SetReaction(1000.0f);
		PlayerIndex()->Reputation.Factions.Faction[i].SetOrder(i);
    }

	PlayerIndex()->Reputation.Factions.Faction[9].SetName("Pirate");
	PlayerIndex()->Reputation.Factions.Faction[9].SetReaction(-3000.0f);
	PlayerIndex()->Reputation.Factions.Faction[9].SetOrder(0x0E);

	PlayerIndex()->Reputation.Factions.Faction[10].SetName("N7");
	PlayerIndex()->Reputation.Factions.Faction[10].SetReaction(2000.0f);
	PlayerIndex()->Reputation.Factions.Faction[10].SetOrder(0x12);

	PlayerIndex()->SetRegistrationStarbase("Net-7 SOL");
	PlayerIndex()->SetRegistrationStarbaseSector("Saturn Sector (Sol System)");

	PlayerIndex()->SetMusicID(-1);
	PlayerIndex()->SetPIPAvatarID(-1);

/*---------------------------------------------------------------------*/

	ShipIndex()->BaseStats.SetMissleDefence(10);
	ShipIndex()->BaseStats.SetTurnRate(50);

	ShipIndex()->CurrentStats.SetMissleDefence(10);
	ShipIndex()->CurrentStats.SetTurnRate(50);



	ShipIndex()->SetFactionIdentifier(faction_list[class_index]);

	//now set correct hull types
	SetHullUpgrade();

	LoadExploredNavs(&account_query);
	LoadMissionStatus(&account_query);

	LogMessage("Loaded avatar `%s` [%d] from SQL\n", Name(), CharacterID());
}

void Player::LoadExploredNavs(sql_query_c *query)
{
	char query_str[256];
	sql_result_c account_result;
    sql_row_c account_row;

	sprintf(query_str, "SELECT * FROM `avatar_exploration` WHERE `avatar_id` = '%ld'", m_CharacterID);

	query->run_query(query_str);
    query->store(&account_result);

	for(int i=0;i<account_result.n_rows();i++)
	{
		account_result.fetch_row(&account_row);
		long object_uid = account_row["object_id"];
		long flags = account_row["explore_flags"];
		long offset = (object_uid/(sizeof(long)*8));
		long *explored;
		long *exposed = (long*) (m_NavsExposed + offset);

		switch (flags)
		{
		case EXPLORE_NAV:
			explored = (long*) (m_NavsExplored + offset);
			*explored |= (1 << object_uid%32);
			//deliberate drop-through - an explored nav must have been exposed
		case DISCOVER_NAV:
			*exposed |= (1 << object_uid%32);
			break;
		}
	}
}

void Player::LoadMissionStatus(sql_query_c *query)
{
	char query_str[256];
	sql_result_c account_result;
    sql_row_c account_row;

	sprintf(query_str, "SELECT * FROM `avatar_mission_progress` WHERE `avatar_id` = '%ld'", m_CharacterID);

	query->run_query(query_str);
    query->store(&account_result);

	for(int i=0;i<account_result.n_rows();i++)
	{
		account_result.fetch_row(&account_row);
		long mission_id = account_row["mission_id"];
		long mission_slot = account_row["mission_slot"];
		long stage_num = account_row["stage_num"];
		long mission_flags = account_row["mission_flags"];

		MissionTree *mission = g_ServerMgr->m_Missions.GetMissionTree(mission_id);

		if (mission)
		{
			AuxMission * m = &m_PlayerIndex.Missions.Mission[mission_slot];
			m->Clear();
			if (stage_num > mission->NumNodes) stage_num = mission->NumNodes;
			m->SetDatabaseID(mission_id);
			m->SetName(mission->name);
			m->SetStageNum(stage_num);
			m->SetStageCount(mission->NumNodes);
			m->SetSummary(mission->summary);
			m->SetMissionData(mission_flags);

			//load the mission descriptions
			for (int i=1; i<=stage_num; i++)
			{
				if (mission->Nodes[i] && mission->Nodes[i]->description)
				{
					m->Stages.Stage[i-1].SetText(mission->Nodes[i]->description);
				}
			}
		}
		else
		{
			// This shouldnt happen unless someone deleted a mission
			LogMessage("Mission Error: Mission ID: %d is missing!", mission_id);
		}
	}
}

void Player::ReInitializeSavedData()
{
    long i, class_index = ClassIndex(), race = Race();

    static const char *faction_list[] =
    {   "TW", "TT", "TE",
        "JW", "JT", "JE",
        "PW", "PT", "PE",
        "Kokura", "Mordana", "FS",
        "V'Rix", "Anseria", "Pirate",
        "Bogeril", "JingLeung", "GC",
        "N7", "TerranPsi", "GETCo",
        "Alliance"
    };

    static const char *faction2_list[] =
    {
        "EarthCorps Warrior",   // TW
        "InfiniCorp Traders",   // TT
        "Hyperia Explorers",    // TE
        "Shinwa Warriors",      // JW
        "Sharim Traders",       // JT
        "Sha'ha'dem Explorers", // JE
        "Centuriata Warriors",  // PW
        "Collegia Traders",     // PT
        "Sabine Explorers"      // PE
    };

    static const char *StartingRank[] =     // starting rank, based on class
    {
        "Ensign",           // TW
        "Prentice",         // TT
        "Cadet",            // TE
        "J'nai",            // JW
        "Nan'Jeu",          // JT
        "Aspirant",         // JE
        "Legionaire",       // PW
        "Quaestor",         // PT
        "Inceptor",         // PE
    };


	unsigned long myTime = GetNet7TickCount();

	DeleteAllAvatarRecords();

/*---------------------------------------------------------------------*/

    PlayerIndex()->Reset();
	PlayerIndex()->SetCredits(500);

    for(i=0;i<96;i++)
	{
		PlayerIndex()->SecureInv.Item[i].SetItemTemplateID(-1);
    }

	PlayerIndex()->RPGInfo.SetRace(m_Database.ship_data.race);
	PlayerIndex()->RPGInfo.SetProfession(m_Database.ship_data.profession);
	PlayerIndex()->RPGInfo.SetSkillPowerUpAbilityNumber(-1);

	u32 Availability[4];
    SkillData *Skills = g_ServerMgr->m_SkillList;
    SkillClassData *ClassData = 0;

    // Do all 64 for forward compatibility
	for(i=0;i<64;i++)
	{
        // Check to see if this class has this skill
		if (Skills[i].ClassType[class_index].MaxLevel > 0)
		{
            ClassData = &Skills[i].ClassType[class_index];

            if (ClassData->Quested == 0 && ClassData->LevelAquired == 0)
            {
                // This skill is available to level up
				Availability[0] = 4;
				Availability[1] = 2;
				Availability[2] = 0;
				Availability[3] = 1;
            }
            else if (ClassData->Quested == 0 && ClassData->LevelAquired > 0)
            {
                // This skill is available but at a higher level
                // Not sure if this will ever be used
				Availability[0] = 4;
				Availability[1] = 1;
				Availability[2] = ClassData->LevelAquired;
				Availability[3] = 1;
            }
            else
            {
                // This skill is aquired via a quest (Quested == 1)
				Availability[0] = 3;
				Availability[1] = 0;
				Availability[2] = 0;
				Availability[3] = 0;
            }

			PlayerIndex()->RPGInfo.Skills.Skill[i].SetAvailability(Availability);
			PlayerIndex()->RPGInfo.Skills.Skill[i].SetMaxSkillLevel(ClassData->MaxLevel);
			PlayerIndex()->RPGInfo.Skills.Skill[i].SetQuestOnlyLevel(ClassData->MaxLevel);
			PlayerIndex()->RPGInfo.Skills.Skill[i].SetLastActivationTime(myTime);
        }
    }

    // Starting skill points
	PlayerIndex()->RPGInfo.Skills.Skill[SKILL_BEAM_WEAPON].SetLevel(1);
	PlayerIndex()->RPGInfo.Skills.Skill[SKILL_DEVICE_TECH].SetLevel(1);
	PlayerIndex()->RPGInfo.Skills.Skill[SKILL_ENGINE_TECH].SetLevel(1);
	PlayerIndex()->RPGInfo.Skills.Skill[SKILL_REACTOR_TECH].SetLevel(1);
	PlayerIndex()->RPGInfo.Skills.Skill[SKILL_SHIELD_TECH].SetLevel(1);

	SaveNewSkillLevel(SKILL_BEAM_WEAPON, 1);
	SaveNewSkillLevel(SKILL_DEVICE_TECH, 1);
	SaveNewSkillLevel(SKILL_ENGINE_TECH, 1);
	SaveNewSkillLevel(SKILL_REACTOR_TECH, 1);
	SaveNewSkillLevel(SKILL_SHIELD_TECH, 1);

    // Temporary "Hack" to allow Explorer classes to mine from the getgo
	if (m_Database.ship_data.profession == 2)
	{
        Availability[0] = 4;
		Availability[1] = 2;
		Availability[2] = 0;
		Availability[3] = 1;

		PlayerIndex()->RPGInfo.Skills.Skill[SKILL_PROSPECT].SetAvailability(Availability);
		PlayerIndex()->RPGInfo.Skills.Skill[SKILL_PROSPECT].SetLevel(1);
		SaveNewSkillLevel(SKILL_PROSPECT, 1);
	}

	PlayerIndex()->Reputation.SetAffilitation(faction2_list[class_index]);

    for (i=0;i<9;i++)
    {
		PlayerIndex()->Reputation.Factions.Faction[i].SetName(faction_list[i]);
		PlayerIndex()->Reputation.Factions.Faction[i].SetReaction(1000.0f);
		PlayerIndex()->Reputation.Factions.Faction[i].SetOrder(i);
    }

	PlayerIndex()->Reputation.Factions.Faction[9].SetName("Pirate");
	PlayerIndex()->Reputation.Factions.Faction[9].SetReaction(-3000.0f);
	PlayerIndex()->Reputation.Factions.Faction[9].SetOrder(0x0E);

	PlayerIndex()->Reputation.Factions.Faction[10].SetName("N7");
	PlayerIndex()->Reputation.Factions.Faction[10].SetReaction(2000.0f);
	PlayerIndex()->Reputation.Factions.Faction[10].SetOrder(0x12);

	PlayerIndex()->SetRegistrationStarbase("Net-7 SOL");
	PlayerIndex()->SetRegistrationStarbaseSector("Saturn Sector (Sol System)");

	PlayerIndex()->SetMusicID(-1);
	PlayerIndex()->SetPIPAvatarID(-1);

/*---------------------------------------------------------------------*/

    ItemBase * myItem = 0;

    ShipIndex()->Reset();

	ShipIndex()->SetName(m_Database.avatar.avatar_first_name);
	ShipIndex()->SetOwner(m_Database.avatar.avatar_first_name);
    ShipIndex()->SetRank(StartingRank[class_index]);

	ShipIndex()->SetHullPoints(float(HullTable[class_index * 7]));
	ShipIndex()->SetMaxHullPoints(float(HullTable[class_index * 7]));

	ShipIndex()->SetAcceleration(117.5f);
	ShipIndex()->SetMaxTiltRate(1.158f);
	ShipIndex()->SetMaxTurnRate(1.158f);

	ShipIndex()->SetWarpAvailable(2);

	ShipIndex()->Inventory.SetEquipMountModel("tvf01_1");

	ShipIndex()->Inventory.SetCargoSpace(BaseCargo[class_index]);

	for(i=0;i<BaseCargo[class_index];i++)
	{
		ShipIndex()->Inventory.CargoInv.Item[i].SetItemTemplateID(-1);
	}

    for(i=0;i<20;i++)
    {
        ShipIndex()->Inventory.AmmoInv.Item[i].SetItemTemplateID(-1);
    }

	ShipIndex()->Inventory.Mounts.SetMount(0, ShieldMount);
	ShipIndex()->Inventory.Mounts.SetMount(1, ReactorMount);
	ShipIndex()->Inventory.Mounts.SetMount(2, EngineMount);

	for(i=0;i<WeaponTable[class_index * 7];i++)
	{
        AddWeapon(i+1);
	}

	for(i=0;i<DeviceTable[class_index * 7];i++)
	{
		ShipIndex()->Inventory.Mounts.SetMount(9+i, DeviceMount);
		ShipIndex()->Inventory.EquipInv.EquipItem[9+i].SetItemTemplateID(-1);
		m_DeviceSlots++;
	}

    ItemBase * Item;

	_Item *instance;

    Item= g_ItemBaseMgr->GetItem(BaseShield[race]);
	ShipIndex()->Inventory.EquipInv.EquipItem[0].SetItemTemplateID(Item->ItemTemplateID());

    Item= g_ItemBaseMgr->GetItem(BaseReactor[race]);
	ShipIndex()->Inventory.EquipInv.EquipItem[1].SetItemTemplateID(Item->ItemTemplateID());

    Item= g_ItemBaseMgr->GetItem(BaseEngine[race]);
	ShipIndex()->Inventory.EquipInv.EquipItem[2].SetItemTemplateID(Item->ItemTemplateID());

    Item= g_ItemBaseMgr->GetItem(BaseWeapon[race]);
	ShipIndex()->Inventory.EquipInv.EquipItem[3].SetItemTemplateID(Item->ItemTemplateID());

	for(i=0;i<4;i++)
	{
		ShipIndex()->Inventory.EquipInv.EquipItem[i].SetQuality(1.0f);
		ShipIndex()->Inventory.EquipInv.EquipItem[i].SetStackCount(1);
		ShipIndex()->Inventory.EquipInv.EquipItem[i].SetStructure(1.0f);
		instance = m_Equip[i].GetItem();
		SaveEquipmentChange(i, instance);
	}

	ShipIndex()->Inventory.SetFutureDevices(MaxDeviceSlots[class_index]);
	ShipIndex()->Inventory.SetFutureWeapons(MaxWeaponSlots[class_index]);

    ShipIndex()->Lego.Attachments.Attachment[0].SetAsset(Item->GameBaseAsset());
    ShipIndex()->Lego.Attachments.Attachment[0].SetType(2);
    ShipIndex()->Lego.Attachments.Attachment[0].SetBoneName(ShipIndex()->Inventory.MountBones.GetMountBoneName(3));

	ShipIndex()->BaseStats.SetMissleDefence(10);
	ShipIndex()->BaseStats.SetSpeed(BaseSpeed[class_index]);
	ShipIndex()->BaseStats.SetWarpSpeed(2000);
	ShipIndex()->BaseStats.SetWarpPowerLevel(3);
	ShipIndex()->BaseStats.SetTurnRate(50);
	ShipIndex()->BaseStats.SetScanRange(BaseScanRange[class_index]);
	ShipIndex()->BaseStats.SetVisibility(BaseVisableRange[class_index]);

	ShipIndex()->CurrentStats.SetMissleDefence(10);
	ShipIndex()->CurrentStats.SetSpeed(BaseSpeed[class_index]);
	ShipIndex()->CurrentStats.SetWarpSpeed(2000);
	ShipIndex()->CurrentStats.SetWarpPowerLevel(3);
	ShipIndex()->CurrentStats.SetTurnRate(50);
	ShipIndex()->CurrentStats.SetScanRange(BaseScanRange[class_index]);
	ShipIndex()->CurrentStats.SetVisibility(BaseVisableRange[class_index]);

	ShipIndex()->SetEngineTrailType(10);
	ShipIndex()->SetFactionIdentifier(faction_list[class_index]);

	m_Database.ship_info.hull         = BaseHullAsset[m_Database.ship_data.race * 3 + m_Database.ship_data.hull];
	m_Database.ship_info.profession   = BaseProfAsset[m_Database.ship_data.race * 3 + m_Database.ship_data.profession];
	m_Database.ship_info.wing         = BaseWingAsset[m_Database.ship_data.race * 3 + m_Database.ship_data.wing];
	m_Database.ship_info.engine       = BaseEngineAsset[m_Database.ship_data.race];

	LogMessage("Save data reinitialized for avatar `%s` [%d]\n", Name(), CharacterID());

    SaveData();

	CreatePositionSave();
	SaveHullUpgrade();
	SaveCreditLevel();
}

void Player::SaveData(bool reset_data)
{
    bool success = false;

    if (m_Hijackee)
    {
        return;
    }

    LogDebug("Saving player %d info [%s]\n", CharacterID(), Name());

	SaveHullLevelChange(ShipIndex()->GetMaxHullPoints());
}

bool Player::SaveDatabase()
{
    if (m_Hijackee)
    {
        return true;
    }

    //first update database with relevant info
    m_Database.info.combat_level = ntohl(PlayerIndex()->RPGInfo.GetCombatLevel());
    m_Database.info.explore_level = ntohl(PlayerIndex()->RPGInfo.GetExploreLevel());
    m_Database.info.trade_level = ntohl(PlayerIndex()->RPGInfo.GetTradeLevel());
    m_Database.info.sector_id = ntohl(PlayerIndex()->GetSectorNum());

    g_AccountMgr->SaveDatabase(&m_Database, m_CharacterID);

    return true;
}

void Player::SaveNewSkillLevel(long skill_id, long skill_level)
{
	unsigned char skill_data[32];
	int index = 0;

	AddData(skill_data, (short)skill_id, index);
	AddData(skill_data, (short)skill_level, index);

	g_SaveMgr->AddSaveMessage(SAVE_CODE_ADVANCE_SKILL, m_CharacterID, index, skill_data);
}

void Player::SaveInventoryChange(long slot)
{
	_Item *item = ShipIndex()->Inventory.CargoInv.Item[slot].GetData();

	long item_id = item->ItemTemplateID;
	short stack_level = (short)item->StackCount;
	short trade_stack = (short)item->TradeStack;
	float quality = item->Quality;
	u32 price = (u32)item->Price;

	unsigned char data[32];
	int index = 0;

	AddData(data, (u8)slot, index);
	AddData(data, (u8)PLAYER_INVENTORY, index);
	AddData(data, stack_level, index);
	AddData(data, trade_stack, index);
	AddData(data, quality, index);
	AddData(data, item_id, index);
	AddData(data, price, index);

	g_SaveMgr->AddSaveMessage(SAVE_CODE_CHANGE_INVENTORY, m_CharacterID, index, data);
}

void Player::SaveVaultChange(long slot)
{
	_Item *item = PlayerIndex()->SecureInv.Item[slot].GetData();
	long item_id = item->ItemTemplateID;
	short stack_level = (short)item->StackCount;
	short trade_stack = (short)item->TradeStack;
	float quality = item->Quality;
	u32 price = (u32)item->Price;

	unsigned char data[32];
	int index = 0;

	AddData(data, (u8)slot, index);
	AddData(data, (u8)PLAYER_VAULT, index);
	AddData(data, stack_level, index);
	AddData(data, trade_stack, index);
	AddData(data, quality, index);
	AddData(data, item_id, index);
	AddData(data, price, index);

	g_SaveMgr->AddSaveMessage(SAVE_CODE_CHANGE_INVENTORY, m_CharacterID, index, data);
}

void Player::SaveXPBarLevel(long xp_type, float xp_bar)
{
	unsigned char data[32];
	int index = 0;

	AddData(data, (u8)xp_type, index);
	AddData(data, xp_bar, index);

	g_SaveMgr->AddSaveMessage(SAVE_CODE_AWARD_XP, m_CharacterID, index, data);
}

void Player::SaveAdvanceLevel(long xp_type, long level)
{
	unsigned char data[32];
	int index = 0;

	AddData(data, (u8)xp_type, index);
	AddData(data, level, index);

	g_SaveMgr->AddSaveMessage(SAVE_CODE_ADVANCE_LEVEL, m_CharacterID, index, data);
}

void Player::SaveAdvanceMission(long slot)
{
	unsigned char data[32];
	int index = 0;

	AuxMission *m = &m_PlayerIndex.Missions.Mission[slot];
	long mission_id = m->GetDatabaseID();
	long mission_stage = m->GetStageNum();

	AddData(data, (u8)slot, index);
	AddData(data, mission_id, index);
	AddData(data, (short)mission_stage, index);

	g_SaveMgr->AddSaveMessage(SAVE_CODE_ADVANCE_MISSION, m_CharacterID, index, data);
}

void Player::SaveMissionFlags(long slot)
{
	unsigned char data[32];
	int index = 0;

	AuxMission *m = &m_PlayerIndex.Missions.Mission[slot];
	long mission_id = m->GetDatabaseID();
	long mission_flags = m->GetMissionData();

	AddData(data, (u8)slot, index);
	AddData(data, mission_flags, index);

	g_SaveMgr->AddSaveMessage(SAVE_CODE_MISSION_FLAGS, m_CharacterID, index, data);
}

void Player::CompleteMission(long mission_id, long completion_flags)
{
	unsigned char data[32];
	int index = 0;

	AddData(data, mission_id, index);
	AddData(data, (u8)completion_flags, index);

	g_SaveMgr->AddSaveMessage(SAVE_CODE_MISSION_COMPLETE, m_CharacterID, index, data);
}

bool Player::CheckMissionCompleted(long mission_id)
{
	bool already_done = false;
	//have we already done this mission, and aren't allowed to do it again
	char query_str[256];

	if (m_LastMissionCompletion == mission_id)
	{
		return true; //mission completed
	}

	sql_query_c account_query (&m_SQL_Conn);
	sql_result_c account_result;
    sql_row_c account_row;

	sprintf(query_str, "SELECT * FROM net7_user.missions_completed WHERE avatar_id = '%ld' AND mission_id = '%ld'", m_CharacterID, mission_id);

	account_query.run_query(query_str);
    account_query.store(&account_result);

	if (account_result.n_rows() > 0)
	{
		account_result.fetch_row(&account_row);
		long completion = account_row["mission_completion_flags"];

		if (completion != 0)
		{
			already_done = true;
			m_LastMissionCompletion = mission_id;
		}
	}

	return already_done;
}

void Player::SaveCreditLevel()
{
	u64 credits = PlayerIndex()->GetCredits();

	unsigned char data[32];
	int index = 0;

	AddData(data, credits, index);

	g_SaveMgr->AddSaveMessage(SAVE_CODE_CREDIT_LEVEL, m_CharacterID, index, data);
}

void Player::SaveEquipmentChange(long slot, _Item *item)
{
	long item_id = item->ItemTemplateID;
	float quality = item->Quality;

	unsigned char data[32];
	int index = 0;

	AddData(data, (u8)slot, index);
	AddData(data, quality, index);
	AddData(data, item_id, index);

	g_SaveMgr->AddSaveMessage(SAVE_CODE_CHANGE_EQUIPMENT, m_CharacterID, index, data);
}

void Player::SaveAmmoChange(long slot, _Item *ammo)
{
	long item_id = ammo->ItemTemplateID;
	float quality = ammo->Quality;
	long stack = ammo->StackCount;

	unsigned char data[32];
	int index = 0;

	AddData(data, (u8)slot, index);
	AddData(data, (short)stack, index);
	AddData(data, quality, index);
	AddData(data, item_id, index);

	g_SaveMgr->AddSaveMessage(SAVE_CODE_CHANGE_AMMO, m_CharacterID, index, data);
}

void Player::SaveHullLevelChange(float new_hull_level)
{
	unsigned char data[32];
	int index = 0;

	float hull_level = ShipIndex()->GetHullPoints();

	AddData(data, hull_level, index);

	g_SaveMgr->AddSaveMessage(SAVE_CODE_HULL_LEVEL_CHANGE, m_CharacterID, index, data);
}

void Player::SaveHullUpgrade()
{
	ShipIndex()->SetRank(GetRank());
	u8 player_rank_name   = (u8)PlayerIndex()->RPGInfo.GetHullUpgradeLevel();
	u8 hull_upgrade_level = (u8)PlayerIndex()->RPGInfo.GetHullUpgradeLevel();
	float max_hull_points = ShipIndex()->GetMaxHullPoints();
	u8 cargo_space 		  = (u8)ShipIndex()->Inventory.GetCargoSpace();
	u8 weapon_slots		  = m_WeaponSlots;
	u8 device_slots		  = m_DeviceSlots;
	u8 warp_power_level	  = (u8)ShipIndex()->CurrentStats.GetWarpPowerLevel();
	u8 engine_thrust_type = (u8)ShipIndex()->GetEngineTrailType();

	unsigned char data[32];
	int index = 0;

	AddData(data, player_rank_name, index);
	AddData(data, hull_upgrade_level, index);
	AddData(data, max_hull_points, index);
	AddData(data, cargo_space, index);
	AddData(data, weapon_slots, index);
	AddData(data, device_slots, index);
	AddData(data, warp_power_level, index);
	AddData(data, engine_thrust_type, index);

	g_SaveMgr->AddSaveMessage(SAVE_CODE_HULL_UPGRADE, m_CharacterID, index, data);
}

void Player::WipeCharacter()
{
	g_SaveMgr->AddSaveMessage(SAVE_CODE_CHARACTER_PROGRESS_WIPE, m_CharacterID, 0, 0);
}

void Player::SaveDiscoverNav(long object_uid)
{
	unsigned char data[32];
	int index = 0;

	AddData(data, object_uid, index);

	g_SaveMgr->AddSaveMessage(SAVE_CODE_DISCOVER_NAV, m_CharacterID, index, data);
}

void Player::SaveExploreNav(long object_uid)
{
	unsigned char data[32];
	int index = 0;

	AddData(data, object_uid, index);

	g_SaveMgr->AddSaveMessage(SAVE_CODE_EXPLORE_NAV, m_CharacterID, index, data);
}

void Player::SaveSkillPoints()
{
	unsigned char data[32];
	int index = 0;
	long points = PlayerIndex()->RPGInfo.GetSkillPoints();

	AddData(data, points, index);

	g_SaveMgr->AddSaveMessage(SAVE_CODE_SET_SKILLPOINTS, m_CharacterID, index, data);
}

void Player::SaveAmmoLevels()
{
	_Item * ammo;
	long item_id;
	float quality;
	long stack;

	unsigned char data[32];

	for (int i = 0; i < m_WeaponSlots; i++)
	{
		ammo = ShipIndex()->Inventory.AmmoInv.Item[i+3].GetData();
		if (ammo && ammo->StackCount > 0)
		{
			item_id = ammo->ItemTemplateID;
			quality = ammo->Quality;
			stack = ammo->StackCount;

			int index = 0;

			AddData(data, (u8)(i+3), index);
			AddData(data, (short)stack, index);
			AddData(data, quality, index);
			AddData(data, item_id, index);

			g_SaveMgr->AddSaveMessage(SAVE_CODE_CHANGE_AMMO, m_CharacterID, index, data);
		}
	}
}

void Player::SaveRegisteredStarbase()
{
	unsigned char data[32];
	int index = 0;

	AddData(data, m_RegisteredSectorID, index);

	g_SaveMgr->AddSaveMessage(SAVE_CODE_SET_STARBASE, m_CharacterID, index, data);
}

void Player::SaveEnergyLevels()
{
	unsigned char data[32];
	float shield_bar = GetShield();
	float energy_bar = GetEnergy();

	int index = 0;

	AddData(data, shield_bar, index);
	AddData(data, energy_bar, index);

	g_SaveMgr->AddSaveMessage(SAVE_CODE_SET_ENERGY_LEVELS, m_CharacterID, index, data);
}

void Player::DeleteAllAvatarRecords()
{
	char query_str[300];
	sql_query_c account_query (&m_SQL_Conn);

	sprintf(query_str, "DELETE FROM `avatar_ammo` WHERE `avatar_id` = '%ld'", m_CharacterID);
	account_query.run_query(query_str);

	sprintf(query_str, "DELETE FROM `avatar_equipment` WHERE `avatar_id` = '%ld'", m_CharacterID);
	account_query.run_query(query_str);

	sprintf(query_str, "DELETE FROM `avatar_faction_level` WHERE `avatar_id` = '%ld'", m_CharacterID);
	account_query.run_query(query_str);

	sprintf(query_str, "DELETE FROM `avatar_inventory_items` WHERE `avatar_id` = '%ld'", m_CharacterID);
	account_query.run_query(query_str);

	sprintf(query_str, "DELETE FROM `avatar_mission_progress` WHERE `avatar_id` = '%ld'", m_CharacterID);
	account_query.run_query(query_str);

	sprintf(query_str, "DELETE FROM `avatar_skill_levels` WHERE `avatar_id` = '%ld'", m_CharacterID);
	account_query.run_query(query_str);

	sprintf(query_str, "DELETE FROM `avatar_vault_items` WHERE `avatar_id` = '%ld'", m_CharacterID);
	account_query.run_query(query_str);

	sprintf(query_str, "DELETE FROM `missions_completed` WHERE `avatar_id` = '%ld'", m_CharacterID);
	account_query.run_query(query_str);

	sprintf(query_str, "DELETE FROM `avatar_exploration` WHERE `avatar_id` = '%ld'", m_CharacterID);
	account_query.run_query(query_str);
}
