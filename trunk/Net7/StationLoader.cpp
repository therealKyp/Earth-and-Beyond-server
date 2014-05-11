// StationLoader.cpp

#include "Net7.h"
#include "StationLoader.h"
#include "mysql/mysqlplus.h"
#include "StringManager.h"
#include "TalkTreeParser.h"
#include "ServerManager.h"

StationLoader::StationLoader()
{
    m_NPCList.clear();				// clear all npc's
    m_StationList.clear();			// clear all entries
    m_RoomList.clear();
    m_TermList.clear();

	m_TalkTreeBufferLen = 16384;
	m_TalkTreeBuffer = new char[m_TalkTreeBufferLen];
}

StationLoader::~StationLoader()
{
    // TODO: destroy everything
	delete[] m_TalkTreeBuffer;
}

int StationLoader::GetStationSectorID(int ObjectID)
{
	char queryString[3048];
	sql_connection_c connection("net7", g_MySQL_Host, g_MySQL_User, g_MySQL_Pass);
	sql_query_c stnSectorID( &connection );
	sql_result_c Station_result;

	sprintf(queryString, "SELECT `gate_to` FROM `sector_objects` WHERE `sector_objects`.`sector_object_id` = '%d'", ObjectID);

	if ( !stnSectorID.execute( queryString ) )
	{
		printf("\n");
		LogMessage( "MySQL Login error/Database error: (User: %s Pass: %s)\n", g_MySQL_User, g_MySQL_Pass );
		exit(1);
		return 0;
	}

	stnSectorID.store(&Station_result);
	
	if (!Station_result.n_rows())
		return -1;

	sql_row_c StationData;
	Station_result.fetch_row(&StationData);	// Read in first row

	return (int)StationData["gate_to"];

}

bool StationLoader::LoadStations()
{
    bool success = false;
	char queryString[3048];
	StationTemplate *current_station = NULL;
	int StationCount = 0;
	int StationsinDB = 0;

	if(!g_MySQL_User || !g_MySQL_Pass) 
    {
		LogMessage("You need to set a mysql user/pass in the net7.cfg\n");
		return 0;
    }

	sql_connection_c connection("net7", g_MySQL_Host, g_MySQL_User, g_MySQL_Pass);
		sql_query_c stnStations( &connection );
	sql_result_c Station_result;

    LogMessage("Loading Stations from SQL");

#ifdef DEV_QUICK_START
	//sprintf(queryString, "SELECT * FROM `starbases` WHERE `sector_id` = "); //can use this for remote SP testing to speed up loading (load only Jove's Fury)
	int limit = 20;
#endif

	sprintf(queryString, "SELECT * FROM `starbases`");

	if ( !stnStations.execute( queryString ) )
	{
		printf("\n");
		LogMessage( "MySQL Login error/Database error: (User: %s Pass: %s)\n", g_MySQL_User, g_MySQL_Pass );
		exit(1);
		return 0;
	}

	stnStations.store(&Station_result);
	
	if (!Station_result.n_rows())
		return 0;

	StationsinDB = (int)Station_result.n_rows();

#ifdef DEV_QUICK_START
	StationsinDB = 20;
#endif

	//printf("Loading Stations from SQL (%d)...\n", (int)Station_result->n_rows());

	sql_row_c StationData;
	bool new_station;
	for(int x=0;x<StationsinDB;x++)
	{
		new_station = false;
		Station_result.fetch_row(&StationData);	// Read in first row
		//see if there's an explicit station sector id for this first
		long station_id = GetStationSectorID((int) StationData["target_sector_object"]);
		//long station_id = (int)StationData["starbase_sector_id"];
		if (station_id == 0)
		{
			station_id = (int)StationData["sector_id"] * 10 + 1; //there was no explicit sector id
		}

        //see if we already have allocated this station
		current_station = m_StationList[station_id];

        if (!current_station)
        {
            current_station = new StationTemplate;		// New Station
            StationCount++;		// Update Station Count
			new_station = true;
        }

		if (current_station)
		{
			current_station->StationID = station_id;
            printf("."); //progress

            m_StationList[current_station->StationID] = current_station;

            current_station->Name = g_StringMgr->GetStr((char*)StationData["name"]);
			current_station->Type = StationData["type"];
			current_station->IsActive = StationData["is_active"];
			current_station->Race = StationData["faction_id"];
			char * welcome = (char*)StationData["welcome_message"];
			if (welcome) strcpy(current_station->WelcomeMessage, welcome);
            current_station->NumTerms = 0;
            current_station->NumNPCs = 0;

            //clear terminals, rooms and NPC ID vectors for this station
            current_station->Terms.clear();
            current_station->Rooms.clear();
            current_station->NPCs.clear();

			int StationID = StationData["starbase_id"];

			if (new_station)
			{
				long parent_sector = (int)StationData["sector_id"];
				char *sector_name = g_ServerMgr->GetSectorName(parent_sector);
				char *system_name = g_ServerMgr->GetSystemName(parent_sector);
				if (sector_name && system_name)
				{
					g_ServerMgr->AddSector(current_station->StationID, current_station->Name, system_name, sector_name);
				}
			}

            AddRooms(current_station, StationID, &connection);
        }
        else 
        {
            LogMessage("Error allocating memory..\n");
            return (success);
        }
    }
    printf("\n");
    LogMessage("Loaded Stations\n");
    return (success);
}

void StationLoader::AddRooms(StationTemplate *current_station, long station_id, sql_connection_c *connection)
{
    char queryString[512];
    sql_query_c stnRooms( connection );
    sql_result_c Rooms_result;
    
    sprintf(queryString, "SELECT * FROM `starbase_rooms` WHERE `starbase_id` = '%d'", station_id);
    if ( !stnRooms.execute( queryString ) )
    {
        LogMessage( "Error reading with MySQL (Rooms)\n" );
        return;
    }
    
    stnRooms.store(&Rooms_result);
    sql_row_c RoomData;
    RoomTemplate *current_room = NULL;
    
    current_station->NumRooms = (int)Rooms_result.n_rows();
    
    LogDebug("Loading %d Rooms for Location: %d\n", current_station->NumRooms, current_station->StationID);

    for(int y=0;y<Rooms_result.n_rows();y++)
    {
        Rooms_result.fetch_row(&RoomData);
        int room_id = RoomData["room_id"];

        //see if we already have allocated this room
        current_room = m_RoomList[room_id];

        if (!current_room)
        {
            current_room = new RoomTemplate;
        }

        if (current_room)
        {
            current_room->Index = RoomData["type"];
            current_room->Style = RoomData["style"];
            current_room->FogNear = RoomData["fog_near"];
            current_room->FogFar = RoomData["fog_far"];
            
            current_room->FogRed = RoomData["fog_red"];
            current_room->FogGreen = RoomData["fog_green"];
            current_room->FogBlue = RoomData["fog_blue"];

            m_RoomList[room_id] = current_room;

            current_station->Rooms.push_back(room_id); 
            
            AddTerminals(current_station, room_id, connection);
            AddNPCs(current_station, room_id, connection);
        }        
    }           
}

void StationLoader::AddTerminals(StationTemplate *current_station, long room_id, sql_connection_c *connection)
{
    char queryString[512];
    //now load all terminals for this room
    sql_query_c stnTerms( connection );
    sql_result_c Term_result;
    
    sprintf(queryString, "SELECT * FROM `starbase_terminals` WHERE `room_id` = '%d'", room_id);
    
    if ( !stnTerms.execute( queryString ) )
    {
        LogMessage( "Error reading with MySQL (Terms)\n" );
        return;
    }
    
    stnTerms.store(&Term_result);
    sql_row_c TermData;
    TermTemplate *current_term = NULL;
    
    current_station->NumTerms += (int)Term_result.n_rows();
    
    LogDebug("Loading %d Terms for Location: %d\n", current_station->NumTerms, current_station->StationID);
    for(int yt=0;yt<Term_result.n_rows();yt++)
    {
        Term_result.fetch_row(&TermData);
        long term_id = TermData["terminal_id"];

        //see if we already have allocated this terminal
        current_term = m_TermList[term_id];

        if (!current_term)
        {
            current_term = new TermTemplate;
        }
        
        if(current_term)
        {
            current_station->Terms.push_back(term_id); 
            m_TermList[term_id] = current_term;

            current_term->Room = m_RoomList[room_id]->Index;
            current_term->Location = TermData["location"];
            current_term->Type = TermData["type"];                        
        }
    }
}

void StationLoader::AddNPCs(StationTemplate *current_station, long room_id, sql_connection_c *connection)
{
    char queryString[512];
    
    // Add NPC id's for each room
    sql_query_c stnNPCs( connection );
    sql_result_c NPCs_result;
    
    //sprintf(queryString, "SELECT * FROM `starbase_npcs` WHERE `room_id` = '%d'", room_id);
    sprintf(queryString, "SELECT * FROM `starbase_npcs` left join `starbase_npc_avatar_templates`"
        " on starbase_npcs.npc_Id = starbase_npc_avatar_templates.avatar_template_id"
        " left join `starbase_vendors`"
        " on starbase_npcs.npc_Id = starbase_vendors.vendor_id" 
        " WHERE `room_id` = '%d'", room_id);

    if ( !stnNPCs.execute( queryString ) )
    {
        LogMessage( "Error reading with MySQL (NPCs)\n" );
        return;
    }
    
    stnNPCs.store(&NPCs_result);
    sql_row_c NPCData;
    NPCTemplate *current_npc = NULL;
    
    current_station->NumNPCs += (int)NPCs_result.n_rows();
       
    for(int y=0;y<NPCs_result.n_rows();y++)
    {
        NPCs_result.fetch_row(&NPCData);
        NPCData.allow_null(0);
        int NPCid = NPCData["npc_Id"];

        //see if we already have allocated this NPC
        current_npc = m_NPCList[NPCid];

        if (!current_npc)
        {
            current_npc = new NPCTemplate;
        }

        if(current_npc)
        {
            memset(&current_npc->Avatar,0,sizeof(AvatarData));
            m_NPCList[NPCid] = current_npc;         // Assign NPC to NPC list map
            int room_id = NPCData["room_id"];
            int Room_Type = m_RoomList[room_id]->Index;
            
            //LogMessage("Loading NPC: %s %s for Location: %d Room: %d NPCID: %d\n", (char *)NPCData["first_name"], (char *)NPCData["last_name"], (int)NPCData["location"], Room_Type, (int)NPCData["npc_Id"]);
            
            strcpy(current_npc->Avatar.avatar_first_name, NPCData["first_name"]);
            strcpy(current_npc->Avatar.avatar_last_name, NPCData["last_name"]);
            current_npc->Location = NPCData["location"];

			//read in talk tree XML text
			int talk_tree_length = strlen(NPCData["talk_tree_handle"])+1;

			if (talk_tree_length > m_TalkTreeBufferLen)
			{
				delete[] m_TalkTreeBuffer;
				m_TalkTreeBuffer = new char[talk_tree_length];
				m_TalkTreeBufferLen = talk_tree_length;
			}

			strcpy(m_TalkTreeBuffer, NPCData["talk_tree_handle"]);

			//now convert to structure format
			BuildMissionTree(current_npc);

			//TalkTreeStructure

			//now when we need to create the talk tree, it's just a case of creating the packet from the structure.

			/*if (talk_tree_length > current_npc->TalkTreeBufferLen)
			{
				if (current_npc->TalkTree)
				{
					delete (current_npc->TalkTree);
				}
				current_npc->TalkTree = new char[talk_tree_length];
				//only adjust length here, that way we track the memory available.
				current_npc->TalkTreeBufferLen = talk_tree_length;
			}
			
            strcpy(current_npc->TalkTree, NPCData["talk_tree_handle"]);*/

            current_npc->Room = Room_Type;	// Room #
            current_npc->VGroup = NPCData["groupid"];
            current_npc->Booth = NPCData["booth_type"];
            current_npc->StarbaseID = NPCid;
            current_npc->Avatar.avatar_type = NPCData["avatar_type"];
            current_npc->Avatar.avatar_version = (int)NPCData["avatar_version"];
            current_npc->Avatar.race = NPCData["race"];
            current_npc->Avatar.profession = NPCData["profession"];
            current_npc->Avatar.gender = NPCData["gender"];
            current_npc->Avatar.mood_type = NPCData["mood_type"];
            current_npc->Avatar.personality = (int) NPCData["personality"];
            current_npc->Avatar.nlp = (int) NPCData["nlp"];
            current_npc->Avatar.body_type = (int)NPCData["shirt_type"];
            current_npc->Avatar.pants_type = (int)NPCData["pants_type"];
            current_npc->Avatar.head_type = (int)NPCData["head_type"];
            current_npc->Avatar.hair_num = (int)NPCData["hair_type"];
            current_npc->Avatar.ear_num = (int)NPCData["ear_type"];
            current_npc->Avatar.goggle_num = (int)NPCData["goggle_num"];
            current_npc->Avatar.beard_num = (int)NPCData["beard_num"];
            current_npc->Avatar.weapon_hip_num = (int)NPCData["weapon_hip_num"];
            current_npc->Avatar.weapon_unique_num = (int)NPCData["weapon_unique_num"];
            current_npc->Avatar.weapon_back_num = (int)NPCData["weapon_back_num"];
            current_npc->Avatar.head_texture_num = (int)NPCData["head_texture_num"];
            current_npc->Avatar.tattoo_texture_num = (int)NPCData["tatoo_texture_num"];
            current_npc->Avatar.tattoo_offset[0] = NPCData["tatoo_x"];
            current_npc->Avatar.tattoo_offset[1] = NPCData["tatoo_y"];
            current_npc->Avatar.tattoo_offset[2] = NPCData["tatoo_z"];
            current_npc->Avatar.hair_color[0] = NPCData["hair_color_0"];
            current_npc->Avatar.hair_color[1] = NPCData["hair_color_1"];
            current_npc->Avatar.hair_color[2] = NPCData["hair_color_2"];
            current_npc->Avatar.beard_color[0] = NPCData["beard_color_0"];
            current_npc->Avatar.beard_color[1] = NPCData["beard_color_1"];
            current_npc->Avatar.beard_color[2] = NPCData["beard_color_2"];
            current_npc->Avatar.eye_color[0] = NPCData["eye_color_0"];
            current_npc->Avatar.eye_color[1] = NPCData["eye_color_1"];
            current_npc->Avatar.eye_color[2] = NPCData["eye_color_2"];
            current_npc->Avatar.skin_color[0] = NPCData["skin_color_0"];
            current_npc->Avatar.skin_color[1] = NPCData["skin_color_1"];
            current_npc->Avatar.skin_color[2] = NPCData["skin_color_2"];
            current_npc->Avatar.shirt_primary_color[0] = NPCData["shirt1_color_0"];
            current_npc->Avatar.shirt_primary_color[1] = NPCData["shirt1_color_1"];
            current_npc->Avatar.shirt_primary_color[2] = NPCData["shirt1_color_2"];
            current_npc->Avatar.shirt_secondary_color[0] = NPCData["shirt2_color_0"];
            current_npc->Avatar.shirt_secondary_color[1] = NPCData["shirt2_color_1"];
            current_npc->Avatar.shirt_secondary_color[2] = NPCData["shirt2_color_2"];
            current_npc->Avatar.pants_primary_color[0] = NPCData["pants1_color_0"];
            current_npc->Avatar.pants_primary_color[1] = NPCData["pants1_color_1"];
            current_npc->Avatar.pants_primary_color[2] = NPCData["pants1_color_2"];
            current_npc->Avatar.pants_secondary_color[0] = NPCData["pants2_color_0"];
            current_npc->Avatar.pants_secondary_color[1] = NPCData["pants2_color_1"];
            current_npc->Avatar.pants_secondary_color[2] = NPCData["pants2_color_2"];
            current_npc->Avatar.shirt_primary_metal = NPCData["shirt1_metal"];
            current_npc->Avatar.shirt_secondary_metal = NPCData["shirt2_metal"];
            current_npc->Avatar.pants_primary_metal = NPCData["pants1_metal"];
            current_npc->Avatar.pants_secondary_metal = NPCData["pants2_metal"];
            current_npc->Avatar.height_weight_1[0] = NPCData["body_weight0"];
            current_npc->Avatar.height_weight_1[1] = NPCData["body_weight1"];
            current_npc->Avatar.height_weight_1[2] = NPCData["body_weight2"];
            current_npc->Avatar.height_weight_1[3] = NPCData["body_weight3"];
            current_npc->Avatar.height_weight_1[4] = NPCData["body_weight4"];
            current_npc->Avatar.height_weight_2[0] = NPCData["head_weight0"];
            current_npc->Avatar.height_weight_2[1] = NPCData["head_weight1"];
            current_npc->Avatar.height_weight_2[2] = NPCData["head_weight2"];
            current_npc->Avatar.height_weight_2[3] = NPCData["head_weight3"];
            current_npc->Avatar.height_weight_2[4] = NPCData["head_weight4"];

            current_station->NPCs.push_back(NPCid);
            
            // Set Trade Items to 0
            current_npc->Vendor.NItems = 0;
            
            // Make sure we are a vendor
            if (current_npc->VGroup != -1)
            {
                // Load Vendor Information
                sql_query_c stnVG( connection );
                sql_result_c NPCVG_result;
                
                sprintf(queryString, "SELECT `id`,`itemid`,`sell_price`,`buy_price`,`quanity` FROM `starbase_vender_inventory` WHERE  `starbase_vender_inventory`.`groupid` = '%d'", current_npc->VGroup);
                
                if ( !stnVG.execute( queryString ) )
                {
                    LogMessage( "Error reading with MySQL (Vendors)\n" );
                    return;
                }
                
                stnVG.store(&NPCVG_result);
                sql_row_c VendorInfo;
                VendorItemList *current_item = NULL;

				current_npc->Vendor.Items.clear();

                current_npc->Vendor.NItems = (int)NPCVG_result.n_rows();
                
                // Loop through our data
                for(int yv=0;yv<NPCVG_result.n_rows();yv++)
                {
                    NPCVG_result.fetch_row(&VendorInfo);
                    VendorInfo.allow_null(0);
					int ItemID = VendorInfo["id"];
					
					current_item = m_ItemList[ItemID];
					
					if (!current_item)
					{
						current_item = new VendorItemList;
					}

                    // Add to our list
                    if(current_item)
                    {
						m_ItemList[ItemID] = current_item;
                        
                        current_npc->Vendor.Items.push_back(ItemID);
                                                
                        current_item->ItemID = VendorInfo["itemid"];
                        current_item->BuyPrice = VendorInfo["buy_price"];
                        current_item->Quanity = VendorInfo["quanity"];
                        current_item->SellPrice = VendorInfo["sell_price"];
                    }
                }
            }
        }
    }
}

void StationLoader::BuildMissionTree(NPCTemplate *npc)
{
	//TODO: delete existing node

	//first lets process normal talk trees
	//InteractionNode *node = new NPCInteractionNode;

	//npc->NPCInteraction[0] = node;

	m_TalkTreeParser.ParseTalkTree(&npc->NPCInteraction.talk_tree, m_TalkTreeBuffer);

	//m_TalkTreeParser.ParseMissions(&npc->NPCInteraction.mission_tree, m_TalkTreeBuffer);
}

NPCTemplate * StationLoader::GetNPC(int ID)
{
	return (m_NPCList[ID]);
}

mapNPCs * StationLoader::GetNPCList()
{
	return &m_NPCList;
}

long StationLoader::GetNPCCount()
{
	return m_NPCList.size();
}

// Send the Station Data from ID
StationTemplate * StationLoader::GetStation(int ID)
{
	return (m_StationList[ID]);
}

RoomTemplate * StationLoader::GetRoom(int ID)
{
    return (m_RoomList[ID]);
}

TermTemplate * StationLoader::GetTerminal(int ID)
{
    return (m_TermList[ID]);
}

VendorItemList * StationLoader::GetVendorItem(int ID)
{
    return (m_ItemList[ID]);
}

char * StationLoader::_GetSectorName(long sector_id)
{
	StationTemplate * station = m_StationList[sector_id];

	if (station)
	{
		return station->Name;
	}
	else
	{
		return (0);
	}
}