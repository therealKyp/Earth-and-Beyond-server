#include "mysql/mysqlplus.h"
#include "ItemBaseImporter.h"

static int myCatSize = 13;

//Name, Cat, Subcat, Type
static CatDat myCat[] = 
{
    "Beam", 10, 100, 14,
    "Beams", 10, 100, 14,
    "Projectiles", 10, 101, 16,
    "Missiles", 10, 102, 15,
    "Ammo", 10, 103, 10,

    "Devices", 11, 110, 11,

    "Reactors", 12, 120, 7,
    "Engines", 12, 121, 6,
    "Shields", 12, 122, 2,

    "Components", 50, 500, 13,

    "Ores", 80, -1, 13,
    "Resources", 80, -1, 13,

    "Loot", -1, -1, 13
};

static int myCompSize = 22;

static CompDat myComp[] =
{
    //Electronic Item
    "Software", 50, 140,//
	"Electronic Item", 50, 141,//
	"Computer", 50, 142,//

	//Reactors
	"Power Converter", 51, 150,//
	"Power Coupling", 51, 151,//
	"Power Core", 51, 152,//
	"Power Generator", 51, 153,//

	//Fabricated
	"Casing", 52, 160,//
	"Engine Frame", 52, 161,//
	"Drone Frame", 52, 162,
	"Mount", 52, 163,//

	//Weapon
	"Firing Mechanism", 53, 170,//
	"Ammunition Feeder", 53, 171,//
	"Beam Optic", 53, 172,//
	"Weapon Barrel", 53, 173,//

	//Ammo
	"Ammunition Slug", 54, 180,//
	"Warhead", 54, 181,//
	"Propellant", 54, 182,//
	"Shell Casing", 54, 183,//

    //Other invalid compoenent data
    "Power", 51, 0,
    "Forged", 52, 0,
    "Ammo", 54, 0,
};

//Don't want to import an entire header just for this function
static long FlipOrder(long Data)
{
    long RetVal = 0;
    char * D = (char *) &Data;
    char * R = (char *) &RetVal;

    R[0] = D[3];
    R[1] = D[2];
    R[2] = D[1];
    R[3] = D[0];

    return RetVal;
}

bool ItemBaseImporter::ConvertDase(char * Dase_DB, char * Dase_Host, char * Dase_User, char * Dase_Pass,
                                 char * Net7_DB, char * Net7_Host, char * Net7_User, char * Net7_Pass, int ClearTables)
{
    long m_ItemStart = m_ItemCount;
    long m_CurrentItem = m_ItemCount;
    ItemBaseStruct * m_ItemData = 0;

    int m_NumItems = 0;

    char StrBuff[1024];
	char QueryString[1024];

    //Initialize SQL Connection
	sql_connection_c Dase_Conn(Dase_DB, Dase_Host, Dase_User, Dase_Pass);
	sql_query_c Dase_Query(&Dase_Conn);
	sql_result_c Dase_Result;
	sql_row_c Dase_Row;

	sql_connection_c Net7_Conn(Net7_DB, Net7_Host, Net7_User, Net7_Pass);
	sql_query_c Net7_Query(&Net7_Conn);
    sql_query Net7_Q;

    // If the clear flag is present, drop all of the data from the Net7 database
    if (ClearTables == 1)
    {
        static char * ClearAmmo = "DELETE FROM `item_ammo`";
        static char * ClearBeam = "DELETE FROM `item_beam`";
        static char * ClearProjectile = "DELETE FROM `item_projectile`";
        static char * ClearMissle = "DELETE FROM `item_missle`";
        static char * ClearDevice = "DELETE FROM `item_device`";
        static char * ClearReactor = "DELETE FROM `item_reactor`";
        static char * ClearEngine = "DELETE FROM `item_engine`";
        static char * ClearShield = "DELETE FROM `item_shield`";
        static char * ClearManu = "DELETE FROM `item_manufacture`";
        static char * ClearBase = "DELETE FROM `item_base`";
        static char * ClearManuBase = "DELETE FROM `item_manufacturer_base`";

        if (!Net7_Query.run_query(ClearAmmo)) return false;
        if (!Net7_Query.run_query(ClearBeam)) return false;
        if (!Net7_Query.run_query(ClearProjectile)) return false;
        if (!Net7_Query.run_query(ClearMissle)) return false;
        if (!Net7_Query.run_query(ClearDevice)) return false;
        if (!Net7_Query.run_query(ClearReactor)) return false;
        if (!Net7_Query.run_query(ClearEngine)) return false;
        if (!Net7_Query.run_query(ClearShield)) return false;
        if (!Net7_Query.run_query(ClearManu)) return false;
        if (!Net7_Query.run_query(ClearBase)) return false;
        if (!Net7_Query.run_query(ClearManuBase)) return false;

        printf("Database emptied!\n\n");
    }

    // Obtain all of the manufacturer strings
    static char * DaseManufacturerQuery = "SELECT DISTINCT `manufacturer` FROM `items`";
	
    if (!Dase_Query.run_query(DaseManufacturerQuery)) return false;

	Dase_Query.store(&Dase_Result);

    int ManufCount = (int)Dase_Result.n_rows();
    char ** Manuf = new char * [ManufCount];
    int * ManufIdx = new int [ManufCount];

    // Insert a blank record into the table to denote no manufacturer
    static char * NoManuf = "INSERT INTO `item_manufacturer_base` (`id`, `name`) VALUES ('0', '')";

    if (!Net7_Query.run_query(NoManuf))
    {
        return false;
    }

    //We inserted #0 already, so the next one is #1
    int CurM = 1;

    // Place the manufacturers in the manufacturer table
    for (int m=0; m<ManufCount; m++)
    {
	    Dase_Result.fetch_row(&Dase_Row);
        Dase_Row.allow_null(0);

        //Faster than accessing the row
        Manuf[m] = new char[64];
        strcpy(Manuf[m], Dase_Row[0]);

        //Compare the string and see if it is a default manufacturer (or invalid)
        if (strlen(Manuf[m]) == 0 || strcmpi(Manuf[m], "null") == 0 ||
            strcmpi(Manuf[m], "Unknown") == 0 || strcmpi(Manuf[m], "Unkown") == 0)
        {
            ManufIdx[m] = 0;
        }
        else
        {
            ManufIdx[m] = CurM++;
            Net7_Q.Clear();
            Net7_Q.SetTable("item_manufacturer_base");
            Net7_Q.AddData("id", ManufIdx[m]);
            Net7_Q.AddData("name", Manuf[m]);
            strcpy(QueryString, Net7_Q.CreateQuery());

            if (!Net7_Query.run_query(QueryString))
            {
                return false;
            }
        }
    }

    // We now have all of the manufacturer strings stored in Manuf and stored in the DB

    static char * DaseItemQuery = "SELECT * FROM items";
    if (!Dase_Query.run_query(DaseItemQuery))
	{
		return false;
	}

    Dase_Query.store(&Dase_Result);	
	m_NumItems = (long)Dase_Result.n_rows();

    printf("%d items found in DASE Database\n", m_NumItems);

    // Iterate for each of the rows in DASE
    for (int i=0; i<m_NumItems; i++)
    {
        if (i % 20 == 0)
        {
            printf("First pass %d / %d\n", i + 1, m_NumItems);
        }

	    Dase_Result.fetch_row(&Dase_Row);
        Dase_Row.allow_null(0);

        m_ItemData = &m_Data[m_ItemCount++];
        memset(m_ItemData, 0, sizeof(ItemBaseStruct));

        strcpy(StrBuff, Dase_Row["Type"]);

        for (int cat=0; cat<myCatSize; cat++)
        {
            if (strcmpi(StrBuff, myCat[cat].Str) == 0)
            {
                m_ItemData->m_Category = myCat[cat].Cat;
                m_ItemData->m_SubCategory = myCat[cat].SubCat;
                m_ItemData->m_ItemType = myCat[cat].Type;
                break;
            }
        }

        // If this is a component there is extra data
        if (m_ItemData->m_Category == 50)
        {
            strcpy(StrBuff, Dase_Row["Category"]);

            for (int comp=0; comp<myCompSize; comp++)
            {
                if (strcmpi(StrBuff, myComp[comp].Str) == 0)
                {
                    m_ItemData->m_Category = myComp[comp].Cat;
                    m_ItemData->m_SubCategory = myComp[comp].SubCat;
                    break;
                }
            }

            //check to see if we found something, if invalidate cat / subcat
            if (m_ItemData->m_SubCategory == 500)
            {
                m_ItemData->m_Category = 0;
                m_ItemData->m_SubCategory = 0;
            }
        }

        m_ItemData->m_ItemTemplateID = Dase_Row["ID"];
        m_ItemData->m_TechLevel = Dase_Row["TechLvl"];
        m_ItemData->m_GameBaseAsset = Dase_Row["3d"];
        m_ItemData->m_IconBaseAsset = Dase_Row["2d"];

	    m_ItemData->m_Cost = 0;
        m_ItemData->m_MaxStack = Dase_Row["Stack"];

        if (m_ItemData->m_MaxStack == 0)
        {
            m_ItemData->m_MaxStack = 1;
        }

        m_ItemData->m_NoDestroy = Dase_Row["NoDestroy"];
        m_ItemData->m_NoManu = Dase_Row["NoManu"];
        m_ItemData->m_NoStore = Dase_Row["NoStore"];
        m_ItemData->m_NoTrade = Dase_Row["NoTrade"];
        m_ItemData->m_Unique = Dase_Row["IsUnique"];

        m_ItemData->m_UseEffect = 0;
        m_ItemData->m_Flags = 0;

        strcpy(m_ItemData->m_Name, Dase_Row["RealName"]);
        strcpy(m_ItemData->m_Manufacturer, Dase_Row["Manufacturer"]);
        m_ItemData->m_Description[0] = '\0';
        strcpy(StrBuff, Dase_Row["Description"]);

        //Throw away beta items
        if (strstr(StrBuff, "beta laser") != 0 || strstr(StrBuff, "beta projectile") != 0)
        {
            m_ItemCount--;
            continue;
        }

        //Remove all special charachters
        char * tok = strtok(StrBuff, "\r\t\n");

        if (tok)
        {
            while (true)
            {
                strcat(m_ItemData->m_Description, tok); //Add in the piece of the string
                tok = strtok(NULL, "\r\t\n");
                if (tok)
                {
                    strcat(m_ItemData->m_Description, "\\n"); //Add in return string
                }
                else
                {
                    break;
                }
            }
        }

        //Import all of the ItemFields
        m_ItemData->m_DefaultAmmo = Dase_Row["DefaultAmmo"];

        m_ItemData->m_ItemFields[ITEM_FIELD_ENERGY_USE_F].fData = Dase_Row["ActivationCost"];

        m_ItemData->m_ItemFields[ITEM_FIELD_WEAPON_RELOAD_F].fData = Dase_Row["Delay"];
        m_ItemData->m_ItemFields[ITEM_FIELD_WEAPON_RANGE_I].fData = Dase_Row["BaseRange"];
        m_ItemData->m_ItemFields[ITEM_FIELD_WEAPON_RANGE_I].fData *= 1000.0f;
        m_ItemData->m_ItemFields[ITEM_FIELD_WEAPON_RANGE_I].iData = (int)m_ItemData->m_ItemFields[ITEM_FIELD_WEAPON_RANGE_I].fData;
        m_ItemData->m_ItemFields[ITEM_FIELD_EFFECT_RANGE_I].iData = m_ItemData->m_ItemFields[ITEM_FIELD_WEAPON_RANGE_I].iData;
        m_ItemData->m_ItemFields[ITEM_FIELD_WEAPON_DAMAGE_I].iData = Dase_Row["Damage"];

        m_ItemData->m_ItemFields[ITEM_FIELD_ENGINE_SPEED_I].iData = Dase_Row["BaseThrust"];
        m_ItemData->m_ItemFields[ITEM_FIELD_ENGINE_WARP_SPEED_I].iData = Dase_Row["BaseWarp"];
        m_ItemData->m_ItemFields[ITEM_FIELD_ENGINE_FREEWARP_DRAIN_F].fData = Dase_Row["Drain"];
        m_ItemData->m_ItemFields[ITEM_FIELD_ENGINE_SIGNATURE_I].fData = Dase_Row["Signature"];
        m_ItemData->m_ItemFields[ITEM_FIELD_ENGINE_SIGNATURE_I].fData *= 1000.0f;
        m_ItemData->m_ItemFields[ITEM_FIELD_ENGINE_SIGNATURE_I].iData = (int)m_ItemData->m_ItemFields[ITEM_FIELD_ENGINE_SIGNATURE_I].fData;

        m_ItemData->m_ItemFields[ITEM_FIELD_REACTOR_CAP_I].iData = Dase_Row["BaseCapacity"];
        m_ItemData->m_ItemFields[ITEM_FIELD_REACTOR_RECHARGE_F].fData = Dase_Row["BaseRecharge"];

        m_ItemData->m_ItemFields[ITEM_FIELD_SHIELD_CAP_I].iData = m_ItemData->m_ItemFields[ITEM_FIELD_REACTOR_CAP_I].iData;
        m_ItemData->m_ItemFields[ITEM_FIELD_SHIELD_RECHARGE_F].fData = m_ItemData->m_ItemFields[ITEM_FIELD_REACTOR_RECHARGE_F].fData;

        m_ItemData->m_ItemFields[ITEM_FIELD_MISSLE_MANUV_I].iData = Dase_Row["Maneuverability"];
        m_ItemData->m_ItemFields[ITEM_FIELD_ROUND_PER_SHOT_I].iData = Dase_Row["Rounds"];

        if (m_ItemData->m_ItemFields[ITEM_FIELD_ROUND_PER_SHOT_I].iData == 0)
            m_ItemData->m_ItemFields[ITEM_FIELD_ROUND_PER_SHOT_I].iData = 1;

        /*DAMAGE TYPE*/

        for (int n=0; n<ManufCount; n++)
        {
            if (strcmpi(m_ItemData->m_Manufacturer, Manuf[n]) == 0)
            {
                m_ItemData->m_ManuInt = ManufIdx[n];
                break;
            }
        }

        m_ItemData->m_Component[0] = Dase_Row["Comp1_Id"];
        m_ItemData->m_Component[1] = Dase_Row["Comp2_Id"];
        m_ItemData->m_Component[2] = Dase_Row["Comp3_Id"];
        m_ItemData->m_Component[3] = Dase_Row["Comp4_Id"];
        m_ItemData->m_Component[4] = Dase_Row["Comp5_Id"];
        m_ItemData->m_Component[5] = Dase_Row["Comp6_Id"];

        for (int com=0; com<6; com++)
        {
            if (m_ItemData->m_Component[com] == 0)
            {
                m_ItemData->m_Component[com] = -1;
            }
        }

        m_ItemData->m_Refine = 0;
        m_ItemData->m_Launcher = 0;

        Net7_Q.Clear();
        Net7_Q.SetTable("item_base");
        Net7_Q.AddData("id", m_ItemData->m_ItemTemplateID);
        Net7_Q.AddData("level", m_ItemData->m_TechLevel);
        Net7_Q.AddData("category", m_ItemData->m_Category);
        Net7_Q.AddData("sub_category", m_ItemData->m_SubCategory);
        Net7_Q.AddData("price", m_ItemData->m_Cost);
        Net7_Q.AddData("max_stack", m_ItemData->m_MaxStack);

        Net7_Q.AddData("name", m_ItemData->m_Name);
        Net7_Q.AddData("description", m_ItemData->m_Description);
        Net7_Q.AddData("manufacturer", m_ItemData->m_ManuInt);

        Net7_Q.AddData("2d_asset", m_ItemData->m_IconBaseAsset);
        Net7_Q.AddData("3d_asset", m_ItemData->m_GameBaseAsset);

        Net7_Q.AddData("no_trade", m_ItemData->m_NoTrade);
        Net7_Q.AddData("no_store", m_ItemData->m_NoStore);
        Net7_Q.AddData("no_destroy", m_ItemData->m_NoDestroy);
        Net7_Q.AddData("no_manu", m_ItemData->m_NoManu);
        Net7_Q.AddData("unique", m_ItemData->m_Unique);

        Net7_Q.AddData("rest_race", 0);
        Net7_Q.AddData("rest_prof", 0);

        strcpy(QueryString, Net7_Q.CreateQuery());

        if (!Net7_Query.run_query(QueryString)) return false;
    }

    printf("First pass successful!\nStarting second pass\n");

    // Iterate for each of the rows in DASE
    for (int j=m_ItemStart; j<m_ItemCount; j++)
    {
        if (j % 20 == 0)
        {
            printf("Second pass %d / %d\n", j + 1, m_ItemCount);
        }

        m_ItemData = &m_Data[m_ItemStart + j];

        //If its a special item, data goes into another table
        switch (m_ItemData->m_SubCategory)
        {
        case 100:   //Beams
            Net7_Q.Clear();
            Net7_Q.SetTable("item_beam");
            Net7_Q.AddData("item_id", m_ItemData->m_ItemTemplateID);
            Net7_Q.AddData("damage_type", m_ItemData->m_ItemFields[ITEM_FIELD_WEAPON_DAMAGE_TYPE_I].iData);

            Net7_Q.AddData("range", m_ItemData->m_ItemFields[ITEM_FIELD_WEAPON_RANGE_I].iData);
            Net7_Q.AddData("energy", m_ItemData->m_ItemFields[ITEM_FIELD_ENERGY_USE_F].fData);
            Net7_Q.AddData("reload", m_ItemData->m_ItemFields[ITEM_FIELD_WEAPON_RELOAD_F].fData);

            //Beams only changed in damage
            Net7_Q.AddData("damage_100", m_ItemData->m_ItemFields[ITEM_FIELD_WEAPON_DAMAGE_I].iData);
            Net7_Q.AddData("damage_200", m_ItemData->m_ItemFields[ITEM_FIELD_WEAPON_DAMAGE_I].iData);

            strcpy(QueryString, Net7_Q.CreateQuery());
            if (!Net7_Query.run_query(QueryString)) return false;
            break;

        case 101:   //Projectiles
            if (m_ItemData->m_DefaultAmmo > 0)
            {
                sprintf(QueryString, "SELECT `RealName` FROM `items` WHERE `id` = %d", m_ItemData->m_DefaultAmmo);
                if (Dase_Query.run_query(QueryString))
                {
	                Dase_Query.store(&Dase_Result);
	                Dase_Result.fetch_row(&Dase_Row);
                    Dase_Row.allow_null(0);
                    strcpy(StrBuff, Dase_Row["RealName"]);
                }
                else
                {
                    StrBuff[0] = '\0';
                }
            }
            else
            {
                StrBuff[0] = '\0';
            }

            Net7_Q.Clear();
            Net7_Q.SetTable("item_projectile");
            Net7_Q.AddData("item_id", m_ItemData->m_ItemTemplateID);
            Net7_Q.AddData("ammo_name", StrBuff);
            Net7_Q.AddData("rounds_fired", m_ItemData->m_ItemFields[ITEM_FIELD_ROUND_PER_SHOT_I].iData);

            Net7_Q.AddData("range", m_ItemData->m_ItemFields[ITEM_FIELD_WEAPON_RANGE_I].iData);
            Net7_Q.AddData("energy", m_ItemData->m_ItemFields[ITEM_FIELD_ENERGY_USE_F].fData);

            //Projectile launchers changed only in reload
            Net7_Q.AddData("reload_100", m_ItemData->m_ItemFields[ITEM_FIELD_WEAPON_RELOAD_F].fData);
            Net7_Q.AddData("reload_200", m_ItemData->m_ItemFields[ITEM_FIELD_WEAPON_RELOAD_F].fData);

            strcpy(QueryString, Net7_Q.CreateQuery());
            if (!Net7_Query.run_query(QueryString)) return false;
            break;

        case 102:   //Missles
            if (m_ItemData->m_DefaultAmmo > 0)
            {
                sprintf(QueryString, "SELECT `RealName` FROM `items` WHERE `id` = %d", m_ItemData->m_DefaultAmmo);
                if (Dase_Query.run_query(QueryString))
                {
	                Dase_Query.store(&Dase_Result);
	                Dase_Result.fetch_row(&Dase_Row);
                    Dase_Row.allow_null(0);
                    strcpy(StrBuff, Dase_Row["RealName"]);
                }
                else
                {
                    StrBuff[0] = '\0';
                }
            }
            else
            {
                StrBuff[0] = '\0';
            }

            Net7_Q.Clear();
            Net7_Q.SetTable("item_missle");
            Net7_Q.AddData("item_id", m_ItemData->m_ItemTemplateID);
            Net7_Q.AddData("ammo_name", StrBuff);
            Net7_Q.AddData("rounds_fired", m_ItemData->m_ItemFields[ITEM_FIELD_ROUND_PER_SHOT_I].iData);

            Net7_Q.AddData("energy", m_ItemData->m_ItemFields[ITEM_FIELD_ENERGY_USE_F].fData);

            //Missles changed only in reload
            Net7_Q.AddData("reload_100", m_ItemData->m_ItemFields[ITEM_FIELD_WEAPON_RELOAD_F].fData);
            Net7_Q.AddData("reload_200", m_ItemData->m_ItemFields[ITEM_FIELD_WEAPON_RELOAD_F].fData);

            strcpy(QueryString, Net7_Q.CreateQuery());
            if (!Net7_Query.run_query(QueryString)) return false;
            break;

        case 103:   //Ammo
            Net7_Q.Clear();
            Net7_Q.SetTable("item_ammo");
            Net7_Q.AddData("item_id", m_ItemData->m_ItemTemplateID);
            Net7_Q.AddData("launcher_id", 0);

            Net7_Q.AddData("range", m_ItemData->m_ItemFields[ITEM_FIELD_WEAPON_RANGE_I].iData);
            Net7_Q.AddData("maneuv", m_ItemData->m_ItemFields[ITEM_FIELD_MISSLE_MANUV_I].iData);

            //Ammo only changed in damage
            Net7_Q.AddData("damage_100", m_ItemData->m_ItemFields[ITEM_FIELD_WEAPON_DAMAGE_I].iData);
            Net7_Q.AddData("damage_200", m_ItemData->m_ItemFields[ITEM_FIELD_WEAPON_DAMAGE_I].iData);

            strcpy(QueryString, Net7_Q.CreateQuery());
            if (!Net7_Query.run_query(QueryString)) return false;
            break;

        case 110:   //Devices
            Net7_Q.Clear();
            Net7_Q.SetTable("item_device");
            Net7_Q.AddData("item_id", m_ItemData->m_ItemTemplateID);

            Net7_Q.AddData("range", m_ItemData->m_ItemFields[ITEM_FIELD_WEAPON_RANGE_I].iData);
            Net7_Q.AddData("cooldown", 0);

            //Devices only changed in energy used
            Net7_Q.AddData("energy_100", m_ItemData->m_ItemFields[ITEM_FIELD_ENERGY_USE_F].fData);
            Net7_Q.AddData("energy_200", m_ItemData->m_ItemFields[ITEM_FIELD_ENERGY_USE_F].fData);

            strcpy(QueryString, Net7_Q.CreateQuery());
            if (!Net7_Query.run_query(QueryString)) return false;
            break;

        case 120:   //Reactors
            Net7_Q.Clear();
            Net7_Q.SetTable("item_reactor");
            Net7_Q.AddData("item_id", m_ItemData->m_ItemTemplateID);

            Net7_Q.AddData("cap", m_ItemData->m_ItemFields[ITEM_FIELD_REACTOR_CAP_I].iData);
            
            //Reactors only changed in recharge
            Net7_Q.AddData("recharge_100", m_ItemData->m_ItemFields[ITEM_FIELD_REACTOR_RECHARGE_F].fData);
            Net7_Q.AddData("recharge_200", m_ItemData->m_ItemFields[ITEM_FIELD_REACTOR_RECHARGE_F].fData);

            strcpy(QueryString, Net7_Q.CreateQuery());
            if (!Net7_Query.run_query(QueryString)) return false;
            break;

        case 121:   //Engines
            Net7_Q.Clear();
            Net7_Q.SetTable("item_engine");
            Net7_Q.AddData("item_id", m_ItemData->m_ItemTemplateID);

            Net7_Q.AddData("warp", m_ItemData->m_ItemFields[ITEM_FIELD_ENGINE_WARP_SPEED_I].iData);
            Net7_Q.AddData("warp_charge", 0);

            //Engines changed in thrust, signature, and freewarpdrain
            Net7_Q.AddData("warp_drain_100", m_ItemData->m_ItemFields[ITEM_FIELD_ENGINE_FREEWARP_DRAIN_F].fData);
            Net7_Q.AddData("warp_drain_200", m_ItemData->m_ItemFields[ITEM_FIELD_ENGINE_FREEWARP_DRAIN_F].fData);
            Net7_Q.AddData("thrust_100", m_ItemData->m_ItemFields[ITEM_FIELD_ENGINE_SPEED_I].iData);
            Net7_Q.AddData("thrust_200", m_ItemData->m_ItemFields[ITEM_FIELD_ENGINE_SPEED_I].iData);
            Net7_Q.AddData("signature_100", m_ItemData->m_ItemFields[ITEM_FIELD_ENGINE_SIGNATURE_I].iData);
            Net7_Q.AddData("signature_200", m_ItemData->m_ItemFields[ITEM_FIELD_ENGINE_SIGNATURE_I].iData);

            strcpy(QueryString, Net7_Q.CreateQuery());
            if (!Net7_Query.run_query(QueryString)) return false;
            break;

        case 122:   //Shields
            Net7_Q.Clear();
            Net7_Q.SetTable("item_shield");
            Net7_Q.AddData("item_id", m_ItemData->m_ItemTemplateID);

            Net7_Q.AddData("cap", m_ItemData->m_ItemFields[ITEM_FIELD_SHIELD_CAP_I].iData);

            //Shields only changed in recharge
            Net7_Q.AddData("recharge_100", m_ItemData->m_ItemFields[ITEM_FIELD_SHIELD_RECHARGE_F].fData);
            Net7_Q.AddData("recharge_200", m_ItemData->m_ItemFields[ITEM_FIELD_SHIELD_RECHARGE_F].fData);

            strcpy(QueryString, Net7_Q.CreateQuery());
            if (!Net7_Query.run_query(QueryString)) return false;
            break;
        }

        if (m_ItemData->m_Component[0] != -1)
        {
            //Insert the manufacture table
            Net7_Q.Clear();
            Net7_Q.SetTable("item_manufacture");
            Net7_Q.AddData("item_id", m_ItemData->m_ItemTemplateID);

            Net7_Q.AddData("comp_1", m_ItemData->m_Component[0]);
            Net7_Q.AddData("comp_2", m_ItemData->m_Component[1]);
            Net7_Q.AddData("comp_3", m_ItemData->m_Component[2]);
            Net7_Q.AddData("comp_4", m_ItemData->m_Component[3]);
            Net7_Q.AddData("comp_5", m_ItemData->m_Component[4]);
            Net7_Q.AddData("comp_6", m_ItemData->m_Component[5]);

            strcpy(QueryString, Net7_Q.CreateQuery());
            if (!Net7_Query.run_query(QueryString)) return false;
        }
    }

    for (int d=0; d<ManufCount; d++)
    {
        if (Manuf[d])
            delete [] Manuf[d];
    }

    delete [] Manuf;
    delete [] ManufIdx;

    printf("DASE Import finished\n");

    return true;

}

