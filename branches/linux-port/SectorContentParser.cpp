// SectorContentParser.cpp

#include "Net7.h"

#ifndef USE_MYSQL_SECTOR

#include "SectorContentParser.h"
#include "XmlParser.h"
#include "SectorData.h"
#include "ObjectClass.h"
#include "ObjectManager.h"

static XmlParser::XmlTagLookupTable sector_content_tag_lookup_table[] =
{
    { XML_TAG_ID_SECTOR_CONTENT,                XML_TAG_SECTOR_CONTENT },
    { XML_TAG_ID_GALAXY,                        XML_TAG_GALAXY },
    { XML_TAG_ID_UNIVERSE,                      XML_TAG_UNIVERSE },
    { XML_TAG_ID_SYSTEM,                        XML_TAG_SYSTEM },
    { XML_TAG_ID_SERVER_PARAMETERS,             XML_TAG_SERVER_PARAMETERS },
    { XML_TAG_ID_SECTOR,                        XML_TAG_SECTOR },
    { XML_TAG_ID_GAME_OBJECT,                   XML_TAG_GAME_OBJECT },
    { XML_TAG_ID_NAME,                          XML_TAG_NAME },
    { XML_TAG_ID_ACTIVATE_RENDER_STATE,         XML_TAG_ACTIVATE_RENDER_STATE },
    { XML_TAG_ID_OBJECT_EFFECT,                 XML_TAG_OBJECT_EFFECT },
    { XML_TAG_ID_RELATIONSHIP,                  XML_TAG_RELATIONSHIP },
    { XML_TAG_ID_SIMPLE_POSITIONAL_UPDATE,      XML_TAG_SIMPLE_POSITIONAL_UPDATE },
    { XML_TAG_ID_ADVANCED_POSITIONAL_UPDATE,    XML_TAG_ADVANCED_POSITIONAL_UPDATE },
    { XML_TAG_ID_PLANET_POSITIONAL_UPDATE,      XML_TAG_PLANET_POSITIONAL_UPDATE },
    { XML_TAG_ID_CONSTANT_POSITIONAL_UPDATE,    XML_TAG_CONSTANT_POSITIONAL_UPDATE },
    { XML_TAG_ID_COMPONENT_POSITIONAL_UPDATE,   XML_TAG_COMPONENT_POSITIONAL_UPDATE },
    { XML_TAG_ID_NAVIGATION,                    XML_TAG_NAVIGATION },
    { XML_TAG_ID_DESTINATION,                   XML_TAG_DESTINATION },
    { XML_TAG_ID_GREETINGS,                     XML_TAG_GREETINGS },
    { XML_TAG_ID_NOTES,                         XML_TAG_NOTES },
    { XML_TAG_ID_SPAWN_DATA,                    XML_TAG_SPAWN_DATA },
    { XML_TAG_ID_FIELD,                         XML_TAG_FIELD_POINT },
    { XML_TAG_ID_SPAWN_POINT,                   XML_TAG_SPAWN_POINT },
    { XML_TAG_ID_MOB_ID,                        XML_TAG_MOB_ID },
    { XML_TAG_ID_RESOURCE_ID,                   XML_TAG_RESOURCE_ID },
    { XML_TAG_ID_INVALID,                       0 }
};

SectorContentParser::SectorContentParser()
{
    m_XmlTagLookupTable = sector_content_tag_lookup_table;
    m_SectorList = NULL;
}

SectorContentParser::~SectorContentParser()
{
    // TODO: destroy everything
}

bool SectorContentParser::LoadSectorContent()
{
    // TODO: Make HTTPS connection to the Auth Server to obtain XML sector content file
    // FOR NOW, we will simply load a local XML file with the sector contents

    bool success = false;
    char orig_path[MAX_PATH];
    GetCurrentDirectory(sizeof(orig_path), orig_path);
    SetCurrentDirectory(SERVER_DATABASE_PATH);

    FILE *f = fopen("SectorContent.xml", "r");
    if (f)
    {
        fseek(f, 0, SEEK_END);
        long file_size = ftell(f);
        fseek(f, 0, SEEK_SET);
        char *data = new char[file_size + 1];
        if (data)
        {
            // Load the entire file into memory for parsing
            long size = fread(data, 1, file_size, f);
            data[size] = 0;
            //printf("Read %d of %d bytes\n", size, file_size);
            success = ParseSectorContent(data);
            delete [] data;
        }
        fclose(f);
    }
    else
    {
        printf("Error opening SectorContent.xml\n");
    }

    SetCurrentDirectory(orig_path);
    return (success);
}

bool SectorContentParser::ParseSectorContent(char *data)
{
    char * p = data;
    long sector_count = 0;
    long next_game_id = 100000;
    SectorData * current_sector = NULL;
    char attrib[64];
    char attrib2[64];
    char attrib3[64];
    char attrib4[64];
    char system_name[64];
    long system_id;

    Object *current_object = 0;

    //printf("Parsing SectorContent.xml\n");

    // Assume we will succeed
    m_Success = true;

    // Parse the data until we are done
    bool done = false;
    while (!done)
    {
        if (ParseXmlTag(&p))
        {
            switch (m_XmlTagID)
            {
            case XML_TAG_ID_SECTOR_CONTENT :
            case XML_TAG_ID_UNIVERSE :
            case XML_TAG_ID_GALAXY :
                // Ignore this tag, it is just the container for the systems and sectors
                // It is not a fatal error if this is missing
                // (although it will not be legal XML and Internet Explorer will complain)
                break;

            case XML_TAG_ID_NOTES:
                // Notes are ignored
                break;

            case XML_TAG_ID_SYSTEM :
                if ((GetXmlAttribute("ID", attrib, sizeof(attrib), true)) &&
                    (GetXmlAttribute("Name", attrib2, sizeof(attrib2), false)))
                {
                    system_id = atoi(attrib);
                    strcpy(system_name, attrib2);
                }
                break;

            case XML_TAG_ID_SECTOR :
                // Create a new sector
                // Forget about the last sector we were working on (it is in the linked list).
                current_sector = new SectorData;
				
                if (current_sector)
                {
                    current_sector->system_id = system_id;
                    strcpy(current_sector->system_name, system_name);
                    sector_count++;
                    memset(current_sector, 0, sizeof(SectorData));
                    //instantiate a new ObjectManager
                    current_sector->obj_manager = new ObjectManager;

                    current_sector->m_xmax = current_sector->m_xmin = 0.0f;
                    current_sector->m_ymax = current_sector->m_ymin = 0.0f;

                    // Add the new sector to the linked list
                    if (m_SectorList)
                    {
                        // Find the last entry in the linked list
                        SectorData *p = m_SectorList;
                        while (p->next)
                        {
                            p = p->next;
                        }
                        // Add this sector to the end of the linked list
                        p->next = current_sector;
                    }
                    else
                    {
                        // This is the first sector in the linked list
                        m_SectorList = current_sector;
                    }

                    // Parse the "ID" and "Name" attribute
                    if ((GetXmlAttribute("ID", attrib, sizeof(attrib), true)) &&
                        (GetXmlAttribute("Name", attrib2, sizeof(attrib2), false)))
                    {
                        current_sector->sector_id = atoi(attrib);
                        char *temp = DecodeXmlString(attrib2);
                        if (temp)
                        {
                            strcpy(current_sector->name, temp);
                            delete [] temp;
                        }
                    }
                    // else missing the required attribute sets m_Success to false
                }
                else
                {
                    printf("FATAL ERROR: ParseSectorContent unable to allocate memory for SectorData\n");
                    m_Success = false;
                }
                break;

            case XML_TAG_ID_SERVER_PARAMETERS :
                if (current_sector)
                {
                    if ((GetXmlAttribute("ZbandMin", attrib, sizeof(attrib), true)) &&
                        (GetXmlAttribute("ZbandMax", attrib2, sizeof(attrib2), false)) &&
                        (GetXmlAttribute("XMin", attrib3, sizeof(attrib3), false)) &&
                        (GetXmlAttribute("YMin", attrib4, sizeof(attrib4), false)))
                    {
                        current_sector->server_params.ZBandMin = (float) atof(attrib);
                        current_sector->server_params.ZBandMax = (float) atof(attrib2);
                        current_sector->server_params.XMin = (float) atof(attrib3);
                        current_sector->server_params.YMin = (float) atof(attrib4);
                        // Parse the other required attributes
                        if ((GetXmlAttribute("XMax", attrib, sizeof(attrib), true)) &&
                            (GetXmlAttribute("YMax", attrib2, sizeof(attrib2), true)) &&
                            (GetXmlAttribute("FogNear", attrib3, sizeof(attrib3), true)) &&
                            (GetXmlAttribute("FogFar", attrib4, sizeof(attrib4), true)))
                        {
                            current_sector->server_params.XMax = (float) atof(attrib);
                            current_sector->server_params.YMax = (float) atof(attrib2);
                            current_sector->server_params.FogNear = (float) atof(attrib3);
                            current_sector->server_params.FogFar = (float) atof(attrib4);
                            // Parse the other required attributes
                            if ((GetXmlAttribute("DebrisMode", attrib, sizeof(attrib), true)) &&
                                (GetXmlAttribute("LightBackdrop", attrib2, sizeof(attrib2), true)) &&
                                (GetXmlAttribute("FogBackdrop", attrib3, sizeof(attrib3), true)) &&
                                (GetXmlAttribute("SwapBackdrop", attrib4, sizeof(attrib4), true)))
                            {
                                current_sector->server_params.DebrisMode = atoi(attrib);
                                current_sector->server_params.LightBackdrop = (attrib2[0] == 't') ? 1 : 0;
                                current_sector->server_params.FogBackdrop = (attrib3[0] == 't') ? 1 : 0;
                                current_sector->server_params.SwapBackdrop = (attrib4[0] == 't') ? 1 : 0;
                                // Parse the other required attributes
                                if ((GetXmlAttribute("BackdropFogNear", attrib, sizeof(attrib), true)) &&
                                    (GetXmlAttribute("backdropFogFar", attrib2, sizeof(attrib2), true)) &&
                                    (GetXmlAttribute("MaxTilt", attrib3, sizeof(attrib3), true)) &&
                                    (GetXmlAttribute("AutoLevel", attrib4, sizeof(attrib4), true)))
                                {
                                    // ImpulseRate="0.0" DecayVelocity="0.0" DecaySpin="0.0" BackdropBaseAsset="0"
                                    current_sector->server_params.BackdropFogNear = (float) atof(attrib);
                                    current_sector->server_params.BackdropFogFar = (float) atof(attrib2);
                                    current_sector->server_params.MaxTilt = (float) atof(attrib3);
                                    current_sector->server_params.AutoLevel = (attrib4[0] == 't') ? 1 : 0;
                                    // Parse the other required attributes
                                    if ((GetXmlAttribute("ImpulseRate", attrib, sizeof(attrib), true)) &&
                                        (GetXmlAttribute("DecayVelocity", attrib2, sizeof(attrib2), true)) &&
                                        (GetXmlAttribute("DecaySpin", attrib3, sizeof(attrib3), true)) &&
                                        (GetXmlAttribute("BackdropBaseAsset", attrib4, sizeof(attrib4), true)))
                                    {
                                        current_sector->server_params.ImpulseRate = (float) atof(attrib);
                                        current_sector->server_params.DecayVelocity = (float) atof(attrib2);
                                        current_sector->server_params.DecaySpin = (float) atof(attrib3);
                                        current_sector->server_params.BackdropBaseAsset = atoi(attrib4);
                                    }
                                }
                            }
                        }
                    }
                }
                else
                {
                    printf("ParseSectorContent: <ServerParameters> must be contained within a <Sector>\n");
                    m_Success = false;
                }
                break;

            case XML_TAG_ID_GAME_OBJECT :
                if (current_sector)
                {
                    current_object = (Object*)(0);
                    // Parse the four attributes required for creating the object
                    if ((GetXmlAttribute("GameID", attrib, sizeof(attrib), true)) &&
                        (GetXmlAttribute("Scale", attrib2, sizeof(attrib2), true)) &&
                        (GetXmlAttribute("BaseAsset", attrib3, sizeof(attrib3), true)) &&
                        (GetXmlAttribute("Type", attrib4, sizeof(attrib4), true)))
                    {
                        long type = atoi(attrib4);

                        if (atoi(attrib3) != 0 && type == -1) type = 37; //bad object type, need to fix sectorcontent.xml
                        
                        switch (type)
                        {
                        case 11:
                            current_object = current_sector->obj_manager->AddNewObject(OT_STARGATE);
                            break;
                            
                        case 12:
                            current_object = current_sector->obj_manager->AddNewObject(OT_STATION);
                            break;
                            
                        case 3:
                            current_object = current_sector->obj_manager->AddNewObject(OT_PLANET);
                            break;
                            
                        case 37:
                            current_object = current_sector->obj_manager->AddNewObject(OT_DECO);
                            break;
                            
                        case 38:
                            current_object = current_sector->obj_manager->AddNewObject(OT_RESOURCE, true);
                            break;
                            
                        case -1:
                            current_object = current_sector->obj_manager->AddNewObject(OT_FIELD, true);
                            break;
                            
                        case 0:
                            current_object = current_sector->obj_manager->AddNewObject(OT_MOB, true);
                            break;
                            
                        default:
                            //Holy Diver! Type not found
                            LogMessage("WARNING: Unexpected XML type %d.\n",type);
                            current_object = current_sector->obj_manager->AddNewObject(OT_RESOURCE, true); //assume this is a resource for now
                        };
                        
                        if (!current_object)
                        {
                            printf("FATAL ERROR: ParseSectorContent unable to allocate memory for GameObject\n");
                            m_Success = false;
                        }
                        else
                        {
                            current_object->SetScale(float(atof(attrib2)));
                            current_object->SetCreateType(char(type));
                            
                            // The HSV attribute is optional
                            if (GetXmlAttribute("HSV", attrib, sizeof(attrib), false) && current_object)
                            {
                                // Parse the three color values into the array
                                float HSV[3];
                                HSV[0] = 0.0f;
                                HSV[1] = 0.0f;
                                HSV[2] = 0.0f;
                                
                                ParseFloatArray(attrib, 3, HSV);
                                current_object->SetHSV(HSV[0], HSV[1], HSV[2]);
                                current_object->SetLevelFromHSV(HSV[0]);
                            }
                            // The Signature attribute is optional
                            if (GetXmlAttribute("Signature", attrib, sizeof(attrib), false) && current_object)
                            {
                                current_object->SetSignature((float) atof(attrib));
                            }
                            // The IsCapShip attribute is optional
                            if (GetXmlAttribute("IsCapShip", attrib, sizeof(attrib), false) && current_object)
                            {
                                if (attrib[0] == '1') current_object->SetObjectType(OT_CAPSHIP);
                            }
                            // The Appears in Radar is optional, only ="0" is hidden
                            if (GetXmlAttribute("AppearsInRadar", attrib, sizeof(attrib), false))
                            {
                                if (attrib[0] == '1') current_object->SetAppearsInRadar();
                            }
                            //for now, we use RadarRange as the sig for objects that don't have nav info
                            if (GetXmlAttribute("RadarRange", attrib, sizeof(attrib), false))
                            {
                                current_object->SetRadarRange((float) atof(attrib));
                            }
                            
                            current_object->SetBasset(atoi(attrib3));
                            
                            current_sector->num_objects++;
                        }
                    }
                    else
                    {
                        printf("ParseSectorContent: <GameObject> Declaration missing params.\n");
                    }
                }
                else
                {
                    printf("ParseSectorContent: <GameObject> must be contained within a <Sector>\n");
                    m_Success = false;
                }
                break;

            case XML_TAG_ID_FIELD:
                if (current_sector)
                {
                    current_object = (Object*)(0);

                    if ((GetXmlAttribute("FieldID", attrib, sizeof(attrib), true)) &&
                        (GetXmlAttribute("Count", attrib4, sizeof(attrib4), true)) &&
                        (GetXmlAttribute("Level", attrib2, sizeof(attrib2), true)) &&
                        (GetXmlAttribute("Type", attrib3, sizeof(attrib3), true)) ) 
                    {
                        //ok we've got a Field spawn. Add it in
                        current_object = current_sector->obj_manager->AddNewObject(OT_FIELD);
                        //current_object->SetGameID(atoi(attrib));
                        current_object->SetFieldCount(atoi(attrib4));
                        current_object->SetLevel(atoi(attrib2));
                        current_object->SetFieldType(atoi(attrib3));

                        if (GetXmlAttribute("Radius", attrib, sizeof(attrib), true))
                        {
                            current_object->SetFieldRadius((float)atof(attrib));
                        }
                    }
                }
                break;

            case XML_TAG_ID_SPAWN_POINT:
                if (current_sector)
                {
                    current_object = (Object*)(0);

                    if ((GetXmlAttribute("SpawnID", attrib, sizeof(attrib), true)) &&
                        (GetXmlAttribute("Count", attrib4, sizeof(attrib4), true)))
                    {
                        //ok we've got a MOB spawn. Add it in
                        current_object = current_sector->obj_manager->AddNewObject(OT_MOBSPAWN);
                        //current_object->SetGameID(atoi(attrib));
                        current_object->SetSpawnCount(atoi(attrib4));
                        if ((GetXmlAttribute("LevelMin", attrib2, sizeof(attrib2), true)) &&
                            (GetXmlAttribute("LevelMax", attrib3, sizeof(attrib3), true)))
                        {
                            current_object->SetLevelMinMax(atoi(attrib2), atoi(attrib3));
                        }
                        if (GetXmlAttribute("SpawnRadius", attrib, sizeof(attrib), true))
                        {
                            current_object->SetSpawnRadius((float)atof(attrib));
                        }
                    } 
                }
                break;

            case XML_TAG_ID_MOB_ID:
                if (current_object)
                {
                    if (GetXmlAttribute("ID", attrib, sizeof(attrib), true))
                    {
                        current_object->AddMOBID(atoi(attrib));
                    }
                }
                else
                {
                    printf("ParseSectorContent: <MOB_ID> must be contained within a <GameObject>\n");
                    m_Success = false;
                }
                break;

            case XML_TAG_ID_RESOURCE_ID:
                if (current_object)
                {
                    if (GetXmlAttribute("ID", attrib, sizeof(attrib), true))
                    {
                        current_object->AddResource(atoi(attrib));
                    }
                }
                else
                {
                    printf("ParseSectorContent: <Resource ID=> must be contained within a <GameObject>\n");
                    m_Success = false;
                }
                break;

            case XML_TAG_ID_SPAWN_DATA:
                if (current_object)
                {
                    if (GetXmlAttribute("SpawnRadius", attrib, sizeof(attrib), true))
                    {
                        current_object->SetSpawnRadius(atoi(attrib));
                        
                        if ((GetXmlAttribute("Behaviour", attrib2, sizeof(attrib2), true)) &&
                            (GetXmlAttribute("GroupAttack", attrib4, sizeof(attrib4), true)))
                        {
                            current_object->SetBehaviour(atoi(attrib2));
                            current_object->SetGroupAttack(atoi(attrib4));
                        }                    
                    }
                    else
                    {
                        printf("ParseSectorContent: incomplete SpawnData entry: require SpawnRadius Behaviour Aggressiveness GroupAttack\n");
                    }
                }
                else
                {
                    printf("ParseSectorContent: <SpawnData> must be contained within a <GameObject>\n");
                    m_Success = false;
                }

                break;

            case XML_TAG_ID_NAME :
                // Are we parsing a game object?
                if (current_object)
                {
                    char name[60];
                    if (!GetXmlData(&p, name, sizeof(name)))
                    {
                        printf("ParseSectorContent: Unable to parse tag: %s", m_Tag);
                        m_Success = false;
                    }
                    else
                    {
                        current_object->SetName(name);
                    }
                }
                else
                {
                    printf("ParseSectorContent: <Name> must be contained within a <GameObject>\n");
                    m_Success = false;
                }
                break;

            case XML_TAG_ID_ACTIVATE_RENDER_STATE:
                if (current_object)
                {
                    if (GetXmlAttribute("RenderStateID", attrib, sizeof(attrib), true))
                    {
                        int render_state = atoi(attrib);
                        int bit = 1 << render_state;
                        current_object->SetRenderState(current_object->RenderState() | bit);
                    }
                    else
                    {
                        printf("ParseSectorContent: Unable to parse tag: %s", m_Tag);
                        m_Success = false;
                    }
                }
                else
                {
                    printf("ParseSectorContent: <ActivateRenderState> must be contained within a <GameObject>\n");
                    m_Success = false;
                }
                break;

            case XML_TAG_ID_OBJECT_EFFECT:
                if (current_object)
                {
                    ObjectEffect * object_effect = current_object->AddNewEffect();
                    
                    // Parse the two required attributes
                    if ((GetXmlAttribute("Bitmask", attrib, sizeof(attrib), true)) &&
                        (GetXmlAttribute("EffectDescID", attrib2, sizeof(attrib2), true)))
                    {
                        object_effect->Bitmask = (char) atoi(attrib);
                        object_effect->EffectDescID = atoi(attrib2);
                        if (object_effect->EffectDescID != 0)
                        {
                            current_object->SetBroadcastID(object_effect->EffectDescID);
                        }
                        if ((object_effect->Bitmask & 0x01) &&
                            (GetXmlAttribute("EffectID", attrib, sizeof(attrib), false)))
                        {
                            object_effect->EffectID = atoi(attrib);
                        }
                        // Ignore Timestamp attribute, bitmask 0x02
                        if ((object_effect->Bitmask & 0x04) &&
                            (GetXmlAttribute("Duration", attrib, sizeof(attrib), false)))
                        {
                            object_effect->Duration = atoi(attrib);
                        }
                        if ((object_effect->Bitmask & 0x08) &&
                            (GetXmlAttribute("Scale", attrib, sizeof(attrib), false)))
                        {
                            object_effect->Scale = (float) atof(attrib);
                        }
                        if ((object_effect->Bitmask & 0x70) &&
                            (GetXmlAttribute("HSVShift", attrib, sizeof(attrib), false)))
                        {
                            ParseFloatArray(attrib, 3, object_effect->HSVShift);
                        }
                    }
                }
                else
                {
                    printf("ParseSectorContent: <ObjectEffect> must be contained within a <GameObject>\n");
                    m_Success = false;
                }
                break;

            case XML_TAG_ID_RELATIONSHIP :
                if (current_object)
                {
                    // Parse the two required attributes
                    if ((GetXmlAttribute("Reaction", attrib, sizeof(attrib), true)) &&
                        (GetXmlAttribute("IsAttacking", attrib2, sizeof(attrib2), true)))
                    {
                        // this is meaningless really, MOBs should have their own behaviour
                        // TODO: check min/max on these values
                        /*current_object->reaction = atoi(attrib);
                        current_object->is_attacking = (attrib2[0] == '1');

                        // TODO: Patch for zero reaction for now
                        if (current_object->reaction == 0)
                        {
                            current_object->reaction = RELATIONSHIP_FRIENDLY;
                        }*/
                    }
                    // else missing a required attribute sets m_Success to false
                }
                else
                {
                    printf("ParseSectorContent: <Relationship> must be contained within a <GameObject>\n");
                    m_Success = false;
                }
                break;

            case XML_TAG_ID_SIMPLE_POSITIONAL_UPDATE :
                if (current_object)
                {
                    // Parse the required attributes
                    if ((GetXmlAttribute("Position", attrib, sizeof(attrib), true)) &&
                        (GetXmlAttribute("Orientation", attrib2, sizeof(attrib2), true)) &&
                        (GetXmlAttribute("Velocity", attrib3, sizeof(attrib3), true)))
                    {
                        // We ignore the Timestamp attribute
                        float pos[3], orientation[4], velocity[3];
                        ParseFloatArray(attrib, 3, pos);
                        ParseFloatArray(attrib2, 4, orientation);
                        ParseFloatArray(attrib3, 3, velocity);
                        UpdateBoundaries(current_sector, pos);

                        current_object->SetPositionType(POSITION_SIMPLE);
                        current_object->SetPosition(pos);
                        current_object->SetOrientation(orientation);
                        current_object->SetVelocityVector(velocity);
                        current_object->PopulateSpawn(false);
                    }
                    // else missing a required attribute sets m_Success to false
                }
                else
                {
                    printf("ParseSectorContent: <Relationship> must be contained within a <GameObject>\n");
                    m_Success = false;
                }
                break;

            case XML_TAG_ID_ADVANCED_POSITIONAL_UPDATE :
                if (current_object)
                {
                    // Parse the required attributes
                    if ((GetXmlAttribute("Bitmask", attrib, sizeof(attrib), true)) &&
                        (GetXmlAttribute("Position", attrib2, sizeof(attrib2), true)) &&
                        (GetXmlAttribute("Orientation", attrib3, sizeof(attrib3), true)) &&
                        (GetXmlAttribute("MovementID", attrib4, sizeof(attrib4), true)))
                    {
                        PositionInformation PosInfo;
                        memset(&PosInfo, 0, sizeof(PosInfo));
                        // We ignore the Timestamp attribute
                        PosInfo.type = POSITION_ADVANCED;
                        current_object->SetPositionType(POSITION_ADVANCED);
                        short bitmask = (short) atoi(attrib);
                        ParseFloatArray(attrib, 3, PosInfo.Position);
                        ParseFloatArray(attrib2, 4, PosInfo.Orientation);
                        PosInfo.MovementID = atoi(attrib4);
                        // Parse the optional attributes
                        // The attributes are "required" if the bitmask is set
                        if ((bitmask & 0x0001) &&
                            (GetXmlAttribute("CurrentSpeed", attrib, sizeof(attrib), true)))
                        {
                            PosInfo.CurrentSpeed = (float) atof(attrib);
                        }
                        if ((bitmask & 0x0002) &&
                            (GetXmlAttribute("SetSpeed", attrib, sizeof(attrib), true)))
                        {
                            PosInfo.SetSpeed = (float) atof(attrib);
                        }
                        if ((bitmask & 0x0004) &&
                            (GetXmlAttribute("Acceleration", attrib, sizeof(attrib), true)))
                        {
                            PosInfo.Acceleration = (float) atof(attrib);
                        }
                        if ((bitmask & 0x0008) &&
                            (GetXmlAttribute("RotY", attrib, sizeof(attrib), true)))
                        {
                            PosInfo.RotY = (float) atof(attrib);
                        }
                        if ((bitmask & 0x0010) &&
                            (GetXmlAttribute("DesiredY", attrib, sizeof(attrib), true)))
                        {
                            PosInfo.DesiredY = (float) atof(attrib);
                        }
                        if ((bitmask & 0x0020) &&
                            (GetXmlAttribute("RotZ", attrib, sizeof(attrib), true)))
                        {
                            PosInfo.RotZ = (float) atof(attrib);
                        }
                        if ((bitmask & 0x0040) &&
                            (GetXmlAttribute("DesiredZ", attrib, sizeof(attrib), true)))
                        {
                            PosInfo.DesiredZ = (float) atof(attrib);
                        }
                        if (bitmask & 0x0080)
                        {
                            if ((GetXmlAttribute("ImpartedVelocity", attrib, sizeof(attrib), true)) &&
                                (GetXmlAttribute("ImpartedSpin", attrib2, sizeof(attrib2), true)) &&
                                (GetXmlAttribute("ImpartedRoll", attrib3, sizeof(attrib3), true)) &&
                                (GetXmlAttribute("ImpartedPitch", attrib4, sizeof(attrib4), true)))
                            {
                                ParseFloatArray(attrib, 3, PosInfo.ImpartedVelocity);
                                PosInfo.ImpartedSpin = (float) atof(attrib2);
                                PosInfo.ImpartedRoll = (float) atof(attrib3);
                                PosInfo.ImpartedPitch = (float) atof(attrib4);
                            }
                        }
                        if ((bitmask & 0x0100) &&
                            (GetXmlAttribute("UpdatePeriod", attrib, sizeof(attrib), true)))
                        {
                            PosInfo.UpdatePeriod = atoi(attrib);
                        }

                        current_object->CopyPostionInfo(&PosInfo);

                        UpdateBoundaries(current_sector, PosInfo.Position);
                    }
                    // else missing a required attribute sets m_Success to false
                }
                else
                {
                    printf("ParseSectorContent: <Relationship> must be contained within a <GameObject>\n");
                    m_Success = false;
                }
                break;

            case XML_TAG_ID_PLANET_POSITIONAL_UPDATE :
                if (current_object)
                {
                    // Parse the required attributes
                    if ((GetXmlAttribute("Position", attrib, sizeof(attrib), true)) &&
                        (GetXmlAttribute("OrbitID", attrib2, sizeof(attrib2), true)) &&
                        (GetXmlAttribute("OrbitDist", attrib3, sizeof(attrib3), true)) &&
                        (GetXmlAttribute("OrbitAngle", attrib4, sizeof(attrib4), true)))
                    {
                        // We ignore the Timestamp attribute
                        PositionInformation PosInfo;
                        memset(&PosInfo, 0, sizeof(PosInfo));

                        current_object->SetPositionType(POSITION_PLANET);
                        ParseFloatArray(attrib, 3, PosInfo.Position);
                        current_object->SetPosition(PosInfo.Position);

                        PosInfo.OrbitID = atoi(attrib2);
                        PosInfo.OrbitDist = (float) atof(attrib3);
                        PosInfo.OrbitAngle = (float) atof(attrib4);
                        // Parse the other required attributes
                        if ((GetXmlAttribute("OrbitRate", attrib, sizeof(attrib), true)) &&
                            (GetXmlAttribute("RotateAngle", attrib2, sizeof(attrib2), true)) &&
                            (GetXmlAttribute("RotateRate", attrib3, sizeof(attrib3), true)) &&
                            (GetXmlAttribute("TiltAngle", attrib4, sizeof(attrib4), true)))
                        {
                            PosInfo.OrbitRate = (float) atof(attrib);
                            PosInfo.RotateAngle = (float) atof(attrib2);
                            PosInfo.RotateRate = (float) atof(attrib3);
                            PosInfo.TiltAngle = (float) atof(attrib4);
                        }

                        current_object->CopyPostionInfo(&PosInfo);
                    }
                    // else missing a required attribute sets m_Success to false
                }
                else
                {
                    printf("ParseSectorContent: <Relationship> must be contained within a <GameObject>\n");
                    m_Success = false;
                }
                break;

            case XML_TAG_ID_CONSTANT_POSITIONAL_UPDATE :
                if (current_object)
                {
                    // Parse the required attributes
                    if ((GetXmlAttribute("Position", attrib, sizeof(attrib), true)) &&
                        (GetXmlAttribute("Orientation", attrib2, sizeof(attrib2), true)))
                    {
                        float pos[3], orientation[4];
                        ParseFloatArray(attrib, 3, pos);
                        ParseFloatArray(attrib2, 4, orientation);
                        UpdateBoundaries(current_sector, pos);
                        
                        current_object->SetPositionType(POSITION_CONSTANT);
                        current_object->SetPosition(pos);
                        current_object->SetOrientation(orientation);
                        current_object->PopulateSpawn(false);
                    }
                    // else missing a required attribute sets m_Success to false
                }
                else
                {
                    printf("ParseSectorContent: <Relationship> must be contained within a <GameObject>\n");
                    m_Success = false;
                }
                break;

            case XML_TAG_ID_COMPONENT_POSITIONAL_UPDATE :
                if (current_object)
                {
                    // Parse the required attributes
                    if ((GetXmlAttribute("Position", attrib, sizeof(attrib), true)) &&
                        (GetXmlAttribute("Orientation", attrib2, sizeof(attrib2), true)) &&
                        (GetXmlAttribute("Velocity", attrib3, sizeof(attrib3), true)) &&
                        (GetXmlAttribute("ImpartedDecay", attrib4, sizeof(attrib4), true)))
                    {
                        // We ignore the Timestamp attribute
                        // We ignore the Timestamp attribute
                        PositionInformation PosInfo;
                        memset(&PosInfo, 0, sizeof(PosInfo));

                        PosInfo.type = POSITION_COMPONENT;
                        ParseFloatArray(attrib, 3, PosInfo.Position);
                        ParseFloatArray(attrib2, 4, PosInfo.Orientation);
                        ParseFloatArray(attrib3, 3, PosInfo.Velocity);
                        
                        current_object->SetPositionType(POSITION_COMPONENT);
                        current_object->SetPosition(PosInfo.Position);
                        current_object->SetOrientation(PosInfo.Orientation);
                        current_object->SetVelocityVector(PosInfo.Velocity);

                        PosInfo.ImpartedDecay = (float) atof(attrib4);
                        // Parse the other required attributes
                        if ((GetXmlAttribute("TractorSpeed", attrib, sizeof(attrib), true)) &&
                            (GetXmlAttribute("TractorID", attrib2, sizeof(attrib2), true)) &&
                            (GetXmlAttribute("TractorEffectID", attrib3, sizeof(attrib3), true)))
                        {
                            PosInfo.TractorSpeed = (float) atof(attrib);
                            PosInfo.TractorID = atoi(attrib2);
                            PosInfo.TractorEffectID = atoi(attrib3);
                        }

                        current_object->CopyPostionInfo(&PosInfo);
                        UpdateBoundaries(current_sector, PosInfo.Position);
                    }
                    // else missing a required attribute sets m_Success to false
                }
                else
                {
                    printf("ParseSectorContent: <Relationship> must be contained within a <GameObject>\n");
                    m_Success = false;
                }
                break;

            case XML_TAG_ID_NAVIGATION :
                if (current_object)
                {
                    // Parse the required attributes
                    if ((GetXmlAttribute("Signature", attrib, sizeof(attrib), true)) &&
                        (GetXmlAttribute("PlayerHasVisited", attrib2, sizeof(attrib2), true)) &&
                        (GetXmlAttribute("NavType", attrib3, sizeof(attrib3), true)) &&
                        (GetXmlAttribute("IsHuge", attrib4, sizeof(attrib4), true)))
                    {
                        long NavType = atoi(attrib3);

						//TODO: remove once DASE DB is fixed
						if (0 == strcmp(current_object->Name(), "Border Demarcation"))
						{
							NavType = 0;
						}

                        if (NavType != 0)
                        {
                            if (current_object->ObjectType() == OT_DECO)
                            {
                                current_object->SetObjectType(OT_NAV);
                            }
                        }

                        if (atoi(attrib4)) current_object->SetHuge();

                        //check for potential problems
                        if (current_object->ObjectType() == OT_STATION ||
                            current_object->ObjectType() == OT_STARGATE)
                        {
                            if (NavType == 0 && current_sector->sector_id > 1014 && current_sector->sector_id < 1100)
                            {
                                //LogMessage("Potential problem with object '%s'. Nav Type is set to %d\n", current_object->Name(), NavType);
                                NavType = 2;
                            }
                            current_object->SetRadarRange(atof(attrib));
                        }

                        if (current_object->ObjectType() == OT_PLANET)
                        {
                            if (current_object->IsHuge())
                            {
                                current_object->SetRadarRange(atof(attrib));
                                if (NavType == 0) NavType = 2;
                            }
                        }

                        current_object->SetNavInfo();
                        current_object->SetNavType(NavType);
                        current_object->SetSignature((float) atof(attrib));

                        //see if this is a 'resource' or 'mining' point
                        //first draft for random content gen
                        //remove once we get some resource stuff happening
                        Field *field;

                        if (current_sector->sector_id == 4095) //Arduinne gas planet
                        {
                            for (int i = 0; i < 3; i++)
                            {
                                field = (Field*)current_sector->obj_manager->AddNewObject(OT_FIELD);
                                field->SetFieldRadius(float(500*(rand()%4 + 4)));
                                field->SetLevel(rand()%3 + 6);
                                field->SetFieldCount(2*(rand()%5 + 4));
                                field->SetFieldType(5);
                                field->SetPosition(current_object->Position());
                                field->MovePosition((rand()%20 - 10)*100.0f, (rand()%20 - 10)*100.0f, (rand()%20 - 10)*20.0f);
                                field->PopulateField(false);
                            }
                        }
                        else if (strstr(current_object->Name(), "Gas Field") != 0 ||
                            current_sector->sector_id == 4030 )
                        {
                            field = (Field*)current_sector->obj_manager->AddNewObject(OT_FIELD);
                            field->SetFieldRadius(float(500*(rand()%4 + 4)));

                            if (strchr(current_object->Name(), '1') != 0)
                            {
                                field->SetLevel(2);
                            }
                            else if (strchr(current_object->Name(), '2') != 0)
                            {
                                field->SetLevel(3);
                            }
                            else if (strchr(current_object->Name(), '3') != 0)
                            {
                                field->SetLevel(5);
                            }
                            else
                            {
                                field->SetLevel(6);
                            }
                            field->SetFieldCount(4*(rand()%5 + 4));
                            field->SetFieldType(5);
                            field->SetPosition(current_object->Position());
                            field->MovePosition((rand()%20 - 10)*100.0f, (rand()%20 - 10)*100.0f, (rand()%20 - 10)*20.0f);
                            field->PopulateField(false);
                        }
                        else if (strstr(current_object->Name(), "Resource") != 0 ||
                            strstr(current_object->Name(), "Mining") != 0 ||
                            strstr(current_object->Name(), "Pit") != 0 ||
                            (rand()%21 > 20) )
                        {
                            field = (Field*)current_sector->obj_manager->AddNewObject(OT_FIELD);
                            field->SetFieldRadius(float(900*(rand()%4 + 9)));
                            field->SetLevel(rand()%6 + 1);
                            field->SetFieldCount(4*(rand()%5 + 4));
                            field->SetFieldType(rand()%5 + 1);
                            field->SetPosition(current_object->Position());
                            field->MovePosition((rand()%20 - 10)*100.0f, (rand()%20 - 10)*100.0f, -(rand()%20 + 30)*50.0f);
                            field->PopulateField(false);
                        }
                    }
                    // else missing a required attribute sets m_Success to false
                }
                else
                {
                    printf("ParseSectorContent: <Relationship> must be contained within a <GameObject>\n");
                    m_Success = false;
                }
                break;

            case XML_TAG_ID_DESTINATION :
                if (current_object)
                {
                    if (current_object->CreateType() == 11 || current_object->CreateType() == 3)
                    {
                        // Parse the required attributes
                        if (GetXmlAttribute("ID", attrib, sizeof(attrib), true))
                        {
                            current_object->SetDestination(atoi(attrib));
                        }
                        // else missing a required attribute sets m_Success to false
                    }
                    else
                    {
                        printf("ParseSectorContent: <Destination> must be associated with a Gate (Type=11) or Planet (Type=3)\n");
                        m_Success = false;
                    }
                }
                else
                {
                    printf("ParseSectorContent: <Destination> must be contained within a <GameObject>\n");
                    m_Success = false;
                }
                break;

            case XML_TAG_ID_GREETINGS :
                if (current_sector)
                {
                    if (!GetXmlData(&p, current_sector->greetings, sizeof(current_sector->greetings)))
                    {
                        printf("ParseSectorContent: Unable to parse tag: %s", m_Tag);
                        m_Success = false;
                    }
                }
                break;

            default :
                printf("ParseSectorContent: Unrecognized tag: %s", m_Tag);
                m_Success = false;
                break;
            }
        }
        else
        {
            done = true;
        }

        if (*p == '\0')
        {
            done = true;
        }

        // Bail out of the loop if we detected an error
        if (!m_Success)
        {
            done = true;
        }

    };

    /////////////////////////
    // PARSING IS COMPLETE
    /////////////////////////

    return (m_Success);
}

SectorData * SectorContentParser::GetSectorData()
{
    return (m_SectorList);
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

#endif
