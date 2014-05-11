// SectorContentSQL.cpp

#include "Net7.h"

#ifndef USE_MYSQL_SECTOR
#error "BUILD ERROR: USE_MYSQL_SECTOR IS NOW ESSENTIAL"
#endif

#pragma warning(disable:4786) 

#include "SectorContentParser.h"
#include "XmlParser.h"
#include "SectorData.h"
#include "mysql/mysqlplus.h"
#include "ObjectClass.h"
#include "ObjectManager.h"
#include "ServerManager.h"
#include "ResourceClass.h"

SectorContentParser::SectorContentParser()
{
    m_SectorList.clear();
	m_SystemList.clear();
	g_SectorObjects.clear();
}

SectorContentParser::~SectorContentParser()
{
    // TODO: destroy everything
}

bool SectorContentParser::LoadSectorContent()
{
	ParseSectorContent(NULL);
    return true;
}

bool SectorContentParser::ParseSectorContent(char *data)
{
    long sector_count = 0;
    SectorData * current_sector = 0;
    Object *current_object = 0;
	MOB *turret;
	char QueryString[1024];

	if(!g_MySQL_User || !g_MySQL_Pass) {
		printf("You need to set a mysql user/pass in the net7.cfg\n");
		return 0;
	}

	sql_connection_c connection( "net7", g_MySQL_Host, g_MySQL_User, g_MySQL_Pass);
	sql_query_c SectorTb( &connection );
    sql_result_c result;
    sql_result_c object_result;
	sql_result_c *Sector_result = &result;

	LoadSystems(&connection);
	LoadAsteroidContentSelection(&connection);

    sprintf(QueryString, "SELECT * FROM `sectors`");

    if ( !SectorTb.execute( QueryString ) )
    {
        printf( "MySQL Login error/Database error: (User: %s Pass: %s)\n", g_MySQL_User, g_MySQL_Pass );
        return 0;
    }
    
    SectorTb.store(Sector_result);
    
    if (!Sector_result->n_rows() || !Sector_result->n_fields()) {
        printf("Error loading rows/fields\n");
        return 0;
    }
    
    printf("Loading Sectors from SQL (%d)", (int)Sector_result->n_rows());
    
	sql_row_c SectorSQLData;
	bool new_sector;
	int i = 40;
	for(int x=0;x<Sector_result->n_rows();x++)
	{
		Sector_result->fetch_row(&SectorSQLData);	// Read in first row
		int sector_id = (int) SectorSQLData["sector_id"];
		new_sector = false;

		//printf("Loading Sector Data for (%d) %s\n", (int) SectorSQLData["sector_id"], (char*) SectorSQLData["name"] );
        printf(".");

#ifdef DEV_QUICK_START
		if (i == 0) break;
		i--;
#endif

		current_sector = m_SectorList[sector_id];

		if (current_sector == 0)
		{
			current_sector = new SectorData;
			if (current_sector)
			{
				memset(current_sector, 0, sizeof(SectorData));
				//instantiate a new ObjectManager
				current_sector->obj_manager = new ObjectManager;
				new_sector = true;
			}
			else
			{
				LogMessage("Unable to Allocate sector for %s\n", (char*) SectorSQLData["name"]);
				return false;
			}
		}

		m_SectorList[sector_id] = current_sector;
		current_sector->m_xmax = current_sector->m_xmin = 0.0f;
		current_sector->m_ymax = current_sector->m_ymin = 0.0f;

        current_sector->system_id = (int)SectorSQLData["system_id"];
		current_sector->name = g_StringMgr->GetStr((char*) SectorSQLData["name"]);

		current_sector->sector_id = sector_id;
		current_sector->num_objects = 0;

		AddSystemInfo(current_sector);

		current_sector->server_params.ZBandMin = (float)SectorSQLData["z_min"];
		current_sector->server_params.ZBandMax = (float)SectorSQLData["z_max"];
		current_sector->server_params.XMin = (float)SectorSQLData["x_min"];
        current_sector->server_params.YMin = (float)SectorSQLData["y_min"];
		current_sector->server_params.XMax = (float)SectorSQLData["x_max"];
		current_sector->server_params.YMax = (float)SectorSQLData["y_max"];

		current_sector->server_params.FogNear = (float)SectorSQLData["fog_near"];
		current_sector->server_params.FogFar = (float)SectorSQLData["fog_far"];
		current_sector->server_params.DebrisMode = (long)SectorSQLData["debris_mode"];
		current_sector->server_params.LightBackdrop = (char)SectorSQLData["light_backdrop"];
		current_sector->server_params.FogBackdrop = (int)SectorSQLData["fog_backdrop"];
		current_sector->server_params.SwapBackdrop = (int)SectorSQLData["swap_backdrop"];
		current_sector->server_params.BackdropFogNear = (float)SectorSQLData["backdrop_fog_near"];
		current_sector->server_params.BackdropFogFar = (float)SectorSQLData["backdrop_fog_far"];
		current_sector->server_params.MaxTilt = (float)SectorSQLData["max_tilt"];
		current_sector->server_params.AutoLevel = (int)SectorSQLData["auto_level"];
		current_sector->server_params.ImpulseRate = (float)SectorSQLData["impulse_rate"];
		current_sector->server_params.DecayVelocity = (float)SectorSQLData["decay_velocity"];
		current_sector->server_params.DecaySpin = (float)SectorSQLData["decay_spin"];
		current_sector->server_params.BackdropBaseAsset = (short)SectorSQLData["backdrop_asset"];
		char *greetings = (char*)SectorSQLData["greetings"];
		if (greetings)
		{
			_snprintf(current_sector->greetings, 128, "%s", greetings);
		}

		current_sector->sector_type = (int)SectorSQLData["sector_type"];

        if (current_sector->server_params.ZBandMax > 50000.0f) current_sector->server_params.ZBandMax = 50000.0f;
        if (current_sector->server_params.ZBandMin < -50000.0f) current_sector->server_params.ZBandMin = -50000.0f;

		if (new_sector)
		{
			g_ServerMgr->AddSector(current_sector->sector_id, current_sector->name, current_sector->system_name, 0);
		}

		//printf("XMax: %f YMax: %f ZMax: %f\n", current_sector->server_params.XMax, current_sector->server_params.YMax, current_sector->server_params.ZBandMax );

        // Game Objects
        sql_query_c GameObjects( &connection );
        sql_result_c *Object_result = &object_result;
        
        //TODO: just change the sectorID
        sprintf(QueryString, "SELECT * FROM `sector_objects` left join `sector_nav_points`"
            " on sector_objects.sector_object_id = sector_nav_points.sector_object_id"
            " left join `sector_objects_harvestable`"
            " on sector_objects.sector_object_id = sector_objects_harvestable.resource_id"
            " left join `sector_objects_planets`"
            " on sector_objects.sector_object_id = sector_objects_planets.planet_id"
            " left join `sector_objects_starbases`"
            " on sector_objects.sector_object_id = sector_objects_starbases.starbase_id"
            " left join `sector_objects_stargates`"
            " on sector_objects.sector_object_id = sector_objects_stargates.stargate_id"
            " left join `sector_objects_mob`"
            " on sector_objects.sector_object_id = sector_objects_mob.mob_id"
            " where sector_objects.sector_id='%d'", current_sector->sector_id);
        
		if ( !GameObjects.execute( QueryString ) )
		{
			printf( "MySQL Error (GameObjects)\n" );
			return 0;
		}

		GameObjects.store(Object_result);		

		sql_row_c ObjectData;
		for(int x=0;x<Object_result->n_rows();x++)
		{
			Object_result->fetch_row(&ObjectData);	// Read in row

			long object_uid = (long)ObjectData["sector_object_id"];

            char type = (char)ObjectData["type"];
			bool new_object = false;

			current_object = g_SectorObjects[object_uid];

			short base_asset = (short)ObjectData["base_asset_id"];

			//see if this is a turret
			AssetData *asset = g_ServerMgr->AssetList()->GetAssetData(base_asset);
			if (asset && strcmp(asset->m_CatName, "Turrets") == 0)
			{
				type = 1; //base MOB
			}

			//create new object if needed
			if (current_object == 0)
			{
				new_object = true;
				switch (type)
				{
				case 11:
					current_object = current_sector->obj_manager->AddNewObject(OT_STARGATE);
					current_object->SetPositionType(POSITION_CONSTANT);
					break;
					
				case 12:
					current_object = current_sector->obj_manager->AddNewObject(OT_STATION);
					current_object->SetPositionType(POSITION_CONSTANT);
					break;
					
				case 3:
					current_object = current_sector->obj_manager->AddNewObject(OT_PLANET);
					current_object->SetPositionType(POSITION_PLANET);
					break;
					
				case 37:
					current_object = current_sector->obj_manager->AddNewObject(OT_DECO);
					current_object->SetPositionType(POSITION_CONSTANT);
					break;
					
				case 38:
					//see if this is a field or not
					if ((float)ObjectData["max_field_radius"] > 0.0f &&
						(long) ObjectData["res_count"] > 0)
					{
						current_object = current_sector->obj_manager->AddNewObject(OT_FIELD, true);
					}
					else
					{
						current_object = current_sector->obj_manager->AddNewObject(OT_RESOURCE, true);
					}
					break;
					
				case 0:
					current_object = current_sector->obj_manager->AddNewObject(OT_MOBSPAWN);
					break;

				case 1:
					current_object = current_sector->obj_manager->AddNewObject(OT_MOB);  //used for turrets only for now, use for other stationary MOBs maybe
					break;
					
				default:
					LogMessage("WARNING: Unexpected Object type %d, in sector [%d] assuming type of Deco.\n",type, current_sector->sector_id);
					current_object = current_sector->obj_manager->AddNewObject(OT_DECO); //assume this is a deco for now
				};
            }
			else
			{
				if (current_object->CreateType() != type)
				{
					//LogMessage("Data Re-Load error - object type of %s has changed from %d to %d\n", current_object->Name(), current_object->ObjectType(), type);
					continue;
				}
			}

            if (!current_object)
            {
                printf("FATAL ERROR: ParseSectorContent unable to allocate memory for Object\n");
                m_Success = false;
                return m_Success;
            }

            //generic properties
            current_object->SetCreateType(type);
			current_object->SetPosition((float)ObjectData["position_x"], (float)ObjectData["position_y"], (float)ObjectData["position_z"]);
            current_object->SetOrientation((float)ObjectData["orientation_u"], (float)ObjectData["orientation_v"], (float)ObjectData["orientation_w"], (float)ObjectData["orientation_z"]);
            current_object->SetName((char*)ObjectData["name"]);
            current_object->SetNavType((long)ObjectData["nav_type"]);
            current_object->SetDatabaseUID(object_uid);

            g_SectorObjects[object_uid] = current_object; //NB g_SectorObjects is a std::map template
                                                          //add this object to the global sector object list
                                                          //we can reference this object by its database 'sector_id'

            //specific properties
            switch (current_object->ObjectType())
            {
            case OT_FIELD:
				current_object->SetCreateType(37); //we want to display field centres to devs
				if (new_object)
				{
					current_object->SetLevel(ObjectData["level"]);
					current_object->SetFieldCount((int)ObjectData["res_count"]);
					current_object->SetFieldRadius((float)ObjectData["max_field_radius"]);
					current_object->SetFieldType((int)ObjectData["field"]);
					AddResourceTypes(current_object, (int)ObjectData["resource_id"], &connection);
					AddFieldOreIDs(current_object, (int)ObjectData["resource_id"], &connection);
					if (current_object->GetSectorManager())
					{
						AddMOBTypes(current_object, (int)ObjectData["resource_id"], &connection);
					}
					current_object->PopulateField(false);
				}
				else if (current_object->FieldCount() == (int)ObjectData["res_count"]) //we can update the field spawn if count remains the same
				{
					current_object->SetLevel(ObjectData["level"]);
					current_object->SetFieldRadius((float)ObjectData["max_field_radius"]);
					current_object->SetFieldType((int)ObjectData["field"]);
					AddResourceTypes(current_object, (int)ObjectData["resource_id"], &connection);
					AddFieldOreIDs(current_object, (int)ObjectData["resource_id"], &connection);
					if (current_object->GetSectorManager())
					{
						AddMOBTypes(current_object, (int)ObjectData["resource_id"], &connection);
					}
					current_object->PopulateField(false, true);
				}
                break;

            case OT_MOBSPAWN:
				if (new_object)
				{
					AddMOBTypes(current_object, (int)ObjectData["mob_id"], &connection);
					current_object->SetSpawnCount(ObjectData["mob_count"]);
					current_object->SetSpawnRadius(ObjectData["mob_spawn_radius"]);
					current_object->PopulateSpawn(false);
				}
                break;

			case OT_MOB: //turret
				turret = (MOB*)current_object;
				turret->SetIsTurret();
				turret->SetBasset(base_asset);
				turret->SetMOBType(0);
				turret->SetCreateType(0);
				turret->SetName((char*)ObjectData["name"]);
				turret->SetActive(true);
				turret->SetRespawnTick(0);
				turret->SetHostileTo(OT_MOB);
				turret->SetUpdateRate(50);
				turret->SetVelocity(0);
				turret->SetDefaultStats(0, TURRET, 0, 50);
				turret->SetSpawnGroup(0);
				break;

			case OT_PLANET:
				current_object->SetTilt((float)ObjectData["tilt_angle"]);
				current_object->SetSpin((float)ObjectData["rotate_rate"]);
				ProcessDefaultObjectStats(current_object, ObjectData);
				break;

			case OT_STARGATE:
				current_object->SetFactionID(ObjectData["faction_id"]);
				if (((int)ObjectData["classSpecific"]) == 1)
				{
					current_object->SetClassSpecific();
				}
				ProcessDefaultObjectStats(current_object, ObjectData);
				break;

            case OT_DECO:
				if (current_object->NavType() > 0)
                {
                    current_object->SetObjectType(OT_NAV);
                }
				ProcessDefaultObjectStats(current_object, ObjectData);
				break;
                
            default:
				ProcessDefaultObjectStats(current_object, ObjectData);
                break;
            }

            current_sector->num_objects++;
        }

		LoadSectorOreAvailability(current_sector, &connection);
    }
    
    /////////////////////////
    // PARSING IS COMPLETE
    /////////////////////////

    printf("\n");

    return (m_Success);
}

void SectorContentParser::ProcessDefaultObjectStats(Object *current_object, sql_row_c &ObjectData)
{
	current_object->SetScale(ObjectData["scale"]);
	current_object->SetHSV((float)ObjectData["h"], (float)ObjectData["s"], (float)ObjectData["v"]);
	current_object->SetLevel(ObjectData["level"]); //set level after HSV so resources get the correct colour
	current_object->SetSignature((float)ObjectData["signature"]);
	current_object->SetRadarRange((float)ObjectData["radar_range"]);
	current_object->SetBasset((short)ObjectData["base_asset_id"]);
	current_object->SetDestination((int)ObjectData["gate_to"]);

	//has the radius been manually set?
	float radius_patch = (float)ObjectData["object_radius_patch"];
	if (radius_patch > 0.0f)
	{
		current_object->SetObjectRadius(radius_patch);
	}

	if ((int)ObjectData["appears_in_radar"] > 0) //does object appear in radar?
	{
		current_object->SetAppearsInRadar();
	}

	if (current_object->Signature() > 0.0f) //does object have navigation info (it does if there's a signature).
	{
		current_object->SetNavInfo();
	}

	if ((int)ObjectData["is_huge"] > 0)
	{
		current_object->SetHuge();
	}
}

SectorData * SectorContentParser::GetSectorData(long sector_id)
{
    return (m_SectorList[sector_id]);
}

SectorData * SectorContentParser::GetSectorData(char *sector_name)
{
	long sector_id = 0;
	
	for( SectorDataMap::iterator sector = m_SectorList.begin(); sector != m_SectorList.end(); ++sector)
	{
		SectorData *this_sector = sector->second;
		if (this_sector && 0 == _stricmp(sector_name, this_sector->name))
		{
			sector_id = this_sector->sector_id;
		}
	}

    return (m_SectorList[sector_id]);
}

char * SectorContentParser::_GetSectorName(long sector_id)
{
	SectorData * data = m_SectorList[sector_id];

	if (data)
	{
		return data->name;
	}
	else
	{
		return (0);
	}
}

char * SectorContentParser::_GetSystemName(long sector_id)
{
	SectorData * data = m_SectorList[sector_id];

	if (data)
	{
		return data->system_name;
	}
	else
	{
		return (0);
	}
}

void SectorContentParser::UpdateBoundaries(SectorData *sector, float *position) //find the boundaries for this sector
{
	if (position[0] > sector->m_xmax)
	{
		sector->m_xmax = position[0];
	}
	else if (position[0] < sector->m_xmin)
	{
		sector->m_xmin = position[0];
	}

	if (position[1] > sector->m_ymax)
	{
		sector->m_ymax = position[1];
	}
	else if (position[1] < sector->m_ymin)
	{
		sector->m_ymin = position[1];
	}
}

void SectorContentParser::LoadSystems(sql_connection_c *connection)
{
    sql_query_c Systems( connection );
    sql_result_c result;
    char QueryString[128];

    sprintf(QueryString, "SELECT * FROM `systems`");

    if ( !Systems.execute( QueryString ) )
    {
        printf( "MySQL Error (Reading systems database)\n");
        return;
    }

    Systems.store(&result);

	SystemData *this_system = (0);

	sql_row_c system_entry;
	for(int x=0;x<result.n_rows();x++)
	{
		result.fetch_row(&system_entry);
		long system_id = (long)system_entry["system_id"];

		this_system = m_SystemList[system_id];

		if (this_system == 0)
		{
			this_system = new SystemData;
		}

		this_system->system_name = g_StringMgr->GetStr((char*)system_entry["name"]);

		m_SystemList[system_id] = this_system;
    }
}

void SectorContentParser::AddSystemInfo(SectorData *sector)
{
	SystemData *system = m_SystemList[sector->system_id];
	
	if (system)
	{
		sector->system_name = system->system_name;
	}
	else
	{
		sector->system_name = sector->name; //system name not found, use the sector name
	}
}

void SectorContentParser::AddResourceTypes(Object *obj, long resource_id, sql_connection_c *connection)
{
    sql_query_c Resource_types( connection );
    sql_result_c result;
    char QueryString[128];

    sprintf(QueryString, "SELECT * FROM `sector_objects_harvestable_restypes` WHERE `group_id` = '%d'", resource_id);

    if ( !Resource_types.execute( QueryString ) )
    {
        printf( "MySQL Error (Reading Resouce types for resource_id %d)\n", resource_id );
        return;
    }

    Resource_types.store(&result);

	obj->ResetResource();

	sql_row_c resource;
	for(int x=0;x<result.n_rows();x++)
	{
		result.fetch_row(&resource);
        long resource_basset = (long)resource["type"];

        if (resource_basset > 0)
        {
            obj->AddResource(resource_basset);
        }
    }
}

void SectorContentParser::AddMOBTypes(Object *obj, long object_id, sql_connection_c *connection)
{
    sql_query_c Mob_types( connection );
    sql_result_c result;
    char QueryString[128];

    sprintf(QueryString, "SELECT * FROM `mob_spawn_group` WHERE `spawn_group_id` = '%d'", object_id);

    if ( !Mob_types.execute( QueryString ) )
    {
        printf( "MySQL Error (Reading mob_spawn_group types for spawn_group_id %d)\n", object_id );
        return;
    }

    Mob_types.store(&result);

	sql_row_c mob_entry;
	for(int x=0;x<result.n_rows();x++)
	{
		result.fetch_row(&mob_entry);
        long mob_id = (long)mob_entry["mob_id"];

        if (mob_id > 0)
        {
            obj->AddMOBID(mob_id);
        }
    }
}

void SectorContentParser::AddFieldOreIDs(Object *obj, long object_id, sql_connection_c *connection)
{
    sql_query_c Ore_types( connection );
    sql_result_c result;
    char QueryString[128];

	//first blank out all existing entries in list
	obj->BlankItemIDs();

	sprintf(QueryString, "SELECT * FROM `sector_objects_harvestable_oretypes` WHERE `resource_id` = '%d'", object_id);

    if ( !Ore_types.execute( QueryString ) )
    {
		printf( "MySQL Error (Reading sector_objects_harvestable_oretypes types for resource_id %d)\n", object_id );
        return;
    }

    Ore_types.store(&result);

	sql_row_c ore_entry;
	for(int x=0;x<result.n_rows();x++)
	{
		OreNode *ore_node = 0;
		result.fetch_row(&ore_entry);
        long item_id = (long)ore_entry["additional_ore_item_id"];
		float frequency = (float)ore_entry["frequency"];

		obj->AddItemID(item_id, frequency);
    }
}

void SectorContentParser::LoadSectorOreAvailability(SectorData *sector, sql_connection_c *connection)
{
    sql_query_c Ore_types( connection );
    sql_result_c result;
    char QueryString[128];
	u32 ore_index = 0;

	//first blank out all existing entries in list
	for( SectorOreList::iterator ore_itr = sector->OreList.begin(); ore_itr != sector->OreList.end(); ++ore_itr)
	{
		(*ore_itr)->frequency = 0.0f;
		(*ore_itr)->item_id = 0;
	}
	sector->ore_list_size = 0;

	sprintf(QueryString, "SELECT * FROM `base_ore_list` WHERE `sector_id` = '%d'", sector->sector_id);

    if ( !Ore_types.execute( QueryString ) )
    {
		printf( "MySQL Error (Reading base_ore_list types for sector %d)\n", sector->sector_id );
        return;
    }

    Ore_types.store(&result);

	sql_row_c ore_entry;
	for(int x=0;x<result.n_rows();x++)
	{
		OreNode *ore_node = 0;
		result.fetch_row(&ore_entry);
        long item_id = (long)ore_entry["item_id"];
		float frequency = (float)ore_entry["frequency"];

		if (ore_index < sector->OreList.size())
		{
			ore_node = sector->OreList[ore_index];
		}
		else
		{
			ore_node = new OreNode;
			sector->OreList.push_back(ore_node);
		}

		ore_node->frequency = frequency;
		ore_node->item_id = item_id;
		sector->ore_list_size++;
		ore_index++;
    }
}

long SectorContentParser::GetNextSectorID(long sector_id)
{
	long next_sector_id = 0;
	bool found_next = false;

	if (sector_id == 0)
	{
		next_sector_id = (m_SectorList.begin())->second->sector_id;
	}
	else
	{
		for( SectorDataMap::iterator sector = m_SectorList.begin(); sector != m_SectorList.end(); ++sector)
		{
			SectorData *this_sector = sector->second;
			if (this_sector)
			{
				if (found_next == true)
				{
					next_sector_id = this_sector->sector_id;
					break;
				}
				if (this_sector->sector_id == sector_id) 
				{
					found_next = true;
				}
			}
		}
	}

    return (next_sector_id);
}

void SectorContentParser::LoadAsteroidContentSelection(sql_connection_c *connection)
{
    sql_query_c Ore_types( connection );
    sql_result_c result;
    char QueryString[128];
	u32 ore_index = 0;
	AsteroidSubcatVec *sub_cat_vec;

	for (int i = 1; i <= RESOURCE_TYPE_SIZE; i++)
	{
		sprintf(QueryString, "SELECT * FROM `asteroid_content_selection` WHERE `asteroid_type` = '%d'", i);
		if ( !Ore_types.execute( QueryString ) )
		{
			printf( "MySQL Error (Reading asteroid_content_selection types for asteroid_type %d)\n", i );
			return;
		}

		Ore_types.store(&result);

		//blank any current data here
		if (m_AsteroidContentList[i])
		{
			m_AsteroidContentList[i]->clear();
			sub_cat_vec = m_AsteroidContentList[i];
		}
		else
		{
			sub_cat_vec = new AsteroidSubcatVec;
			m_AsteroidContentList[i] = sub_cat_vec;
		}
		
		sql_row_c resource;
		for(int x=0;x<result.n_rows();x++)
		{
			result.fetch_row(&resource);
			long subcat = (long)resource["item_subcat_id"];
			sub_cat_vec->push_back(subcat); //let STL handle allocation, this data should be fairly static anyway
		}
	}
}

AsteroidSubcatVec* SectorContentParser::GetAsteroidContentSelection(long asteroid_type)
{
	AsteroidSubcatVec *sub_cat_vec = m_AsteroidContentList[asteroid_type];
	return sub_cat_vec;
}