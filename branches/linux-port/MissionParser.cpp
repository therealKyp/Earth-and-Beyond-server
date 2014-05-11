#include "Net7.h"
#include "XmlTags.h"
#include "MissionParser.h"
#include "MissionManager.h"
#include "SectorManager.h"
#include "ServerManager.h"
#include "Connection.h"
#include "ItemList.h"
#include "Opcodes.h"
#include "SectorData.h"

static XmlParser::XmlTagLookupTable mission_tag_lookup_table[] =
{
    { XML_TAG_ID_MISSIONS,			XML_TAG_MISSIONS },
    { XML_TAG_ID_MISSION,			XML_TAG_MISSION },
    { XML_TAG_ID_NAME,				XML_TAG_NAME },
	{ XML_TAG_ID_SUMMARY,			XML_TAG_SUMMARY },
    { XML_TAG_ID_STAGES,			XML_TAG_STAGES },
    { XML_TAG_ID_STAGE,				XML_TAG_STAGE },
    { XML_TAG_ID_DESCRIPTION,		XML_TAG_DESCRIPTION },
    { XML_TAG_ID_REWARDS,			XML_TAG_REWARDS },
    { XML_TAG_ID_FACTION,			XML_TAG_FACTION },
    { XML_TAG_ID_CREDITS,			XML_TAG_CREDITS },
    { XML_TAG_ID_EXPERIENCE,		XML_TAG_EXPERIENCE },
    { XML_TAG_ID_FAILURE,			XML_TAG_FAILURE },
    { XML_TAG_ID_TRIGGER,			XML_TAG_TRIGGER },
	{ XML_TAG_ID_CONDITION,			XML_TAG_CONDITION },
	{ XML_TAG_ID_ACTION,			XML_TAG_ACTION },
    { XML_TAG_ID_TALKTREE,          XML_TAG_TALKTREE },
    { XML_TAG_ID_NEXT_STAGE,        XML_TAG_NEXT_STAGE },
    { XML_TAG_ID_OBJECT,            XML_TAG_OBJECT },
    { XML_TAG_ID_TEXT,              XML_TAG_TEXT },
    { XML_TAG_ID_ITEM,              XML_TAG_ITEM },
    { XML_TAG_ID_NPC,               XML_TAG_NPC },
    { XML_TAG_ID_LOCATION,          XML_TAG_LOCATION },
    { XML_TAG_ID_HULLUPGRADE,       XML_TAG_HULLUPGRADE },
    { XML_TAG_ID_BRANCH,            XML_TAG_BRANCH },
    { XML_TAG_ID_INVALID,			0 }
};

MissionParser::MissionParser(void)
{
	m_XmlTagLookupTable = mission_tag_lookup_table;
	m_MissionList.clear();
}

MissionParser::~MissionParser(void)
{
    m_MissionList.clear();
}

bool MissionParser::LoadMissions()
{
    bool success = false;
    char orig_path[MAX_PATH];
    GetCurrentDirectory(sizeof(orig_path), orig_path);
    SetCurrentDirectory(SERVER_DATABASE_PATH);

    FILE *f = fopen("Missions.xml", "r");

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
            success = ParseMissions(data);
			if (!success)
				exit(1);
            delete [] data;
        }
        fclose(f);
    }
    else
    {
        printf("Error opening Missions.xml\n");
    }

    SetCurrentDirectory(orig_path);
    return (success);
}

bool MissionParser::ParseMissions(char *data)
{
	char * p = data;
    long mission_count = 0;
	char attrib[1024];		// If the mission / stage descriptions are longer than 1024 characters, then this can be increased to accomodate
	char attrib1[32];
	char attrib2[32];
	int numStages = 0;
    MissionNode     *current_node = (0);
    Mission         *current_mission = (0);
    TalkTreeNode    *current_talktree_node = (0);

	printf("Parsing Missions.xml\n");

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
				case XML_TAG_ID_MISSIONS :
					// Just a container tag
					break;
				case XML_TAG_ID_MISSION :
					m_CurrentID = m_XmlTagID;

					current_mission = new Mission;
                    current_node = (0);
                    current_talktree_node = (0);

					numStages = 0;

					if (current_mission)
					{
                        current_mission->SetMissionID(mission_count);
                        mission_count++;

                        //add this mission to the mission list
                        m_MissionList.push_back(current_mission);
						
						if (GetXmlAttribute(XML_ATTRIB_ID, attrib, sizeof(attrib), true) &&
							GetXmlAttribute(XML_ATTRIB_FORFEITABLE, attrib1, sizeof(attrib1), true) &&
							GetXmlAttribute(XML_ATTRIB_TIME, attrib2, sizeof(attrib2), true))
						{
							//current_mission->mission.auxMissionDetails.IsForfeitable = (atoi(attrib1) == 1);
							//current_mission->AllowedTime = atol(attrib2);
							/*if (current_mission->mission.AllowedTime > 0)
							{
								current_mission->mission.auxMissionDetails.Flags[1] |= (char)0x18;
								current_mission->mission.auxMissionDetails.IsTimed = true;

							}
							else
							{
								current_mission->mission.auxMissionDetails.IsTimed = false;
							}*/
                            if (GetXmlAttribute(XML_ATTRIB_OVERALL, attrib, sizeof(attrib), false))
                            {
                                current_mission->SetLevelRequirement(atol(attrib));
                            }
						}
						else
						{
							printf("ParseMissions: Mission does not contain correct attributes\n");
							m_Success = false;
							break;
						}
					}
					break;
				case XML_TAG_ID_NAME :
					if (current_mission)
					{
						if (m_CurrentID == XML_TAG_ID_MISSION)
						{
							if (!GetXmlData(&p, attrib, sizeof(attrib)))
							{
								printf("ParseMissions: Unable to parse tag: %s", m_Tag);
								m_Success = false;
							}
							else
							{
								// Send the name of the mission with the packet
								//current_mission->mission.auxMissionDetails.Flags[0] |= (char)0x20;
                                current_mission->SetMissionName(attrib);
							}
						}
						else
						{
							printf("ParseMissions: <%s> must be contained within a <Mission> tag\n", m_Tag);
							m_Success = false;
						}
					}
					else
					{
						printf("ParseMissions: <%s> must be contained within a <Mission> tag\n", m_Tag);
						m_Success = false;
					}
					break;
				case XML_TAG_ID_SUMMARY :
					if (current_mission)
					{
						if (m_CurrentID == XML_TAG_ID_MISSION)
						{
							if (!GetXmlData(&p, attrib, sizeof(attrib)))
							{
								printf("ParseMissions: Unable to parse tag: %s", m_Tag);
								m_Success = false;
							}
							else
							{
								// Send the summary of the mission with the packet
								//current_mission->mission.auxMissionDetails.Flags[0] |= (char)0x40;
                                current_mission->SetMissionSummary(attrib);
							}
						}
						else
						{
							printf("ParseMissions: <%s> must be contained within a <Mission> tag\n", m_Tag);
							m_Success = false;
						}
					}
					break;
				case XML_TAG_ID_STAGES :
					// Ignore, container
					break;
				case XML_TAG_ID_STAGE :
					if (current_mission)
					{
						m_CurrentID = m_XmlTagID;
						numStages++;
                        current_node = new MissionNode;
                        memset(current_node, 0, sizeof(MissionNode));

                        current_mission->AddNewNode(current_node);

						//current_mission->mission.auxMissionDetails.Stage = 1;
						//current_mission->mission.auxMissionDetails.StageCount = numStages;

						// Send some stages
						//current_mission->mission.auxMissionDetails.Flags[2] |= 0x1C;
						// Send this stage in particular
						//current_mission->mission.auxMissionDetails.Stages[numStages-1].Flags[0] |= (char)0x22;
						//current_mission->mission.auxMissionDetails.StageFlags[0] |= 0x12;
						//current_mission->mission.auxMissionDetails.Stages[numStages-1].IsTimed = false; // Default

						if (numStages >= MAX_NUM_STAGES)
						{
							printf("ParseMissions: Too many stages\n");
							m_Success = false;
						}
						if (GetXmlAttribute(XML_ATTRIB_TIME, attrib2, sizeof(attrib2), false))
						{
							/*current_mission->mission.Stages[numStages-1].AllowedTime = atol(attrib2);
							if (current_mission->mission.Stages[numStages-1].AllowedTime > 0)
							{
								current_mission->mission.auxMissionDetails.Stages[numStages-1].IsTimed = true;
							}*/
						}
                        if (GetXmlAttribute(XML_ATTRIB_TYPE, attrib1, sizeof(attrib1), true))
                        {
                            current_node->type = (node_type)atoi(attrib1);
                        }
                        if (GetXmlAttribute(XML_ATTRIB_SECTORID, attrib1, sizeof(attrib1), false))
                        {
                            current_node->sector_id = atoi(attrib1);
                        }
					}
					else
					{
						printf("ParseMissions: <%s> must be contained within a <Mission> tag\n", m_Tag);
						m_Success = false;
					}
					break;
				case XML_TAG_ID_DESCRIPTION :
					if (current_node)
					{
						if (m_CurrentID == XML_TAG_ID_STAGE)
						{
                            if (!GetXmlData(&p, attrib, sizeof(attrib)))
                            {
								printf("ParseMissions: Unable to parse tag: %s", m_Tag);
								m_Success = false;
							}
							else
							{
                                int length = strlen(attrib);
                                current_node->description = new char [length+1];
                                strcpy(current_node->description, attrib);
							}
						}
						else if (m_CurrentID == XML_TAG_ID_REWARDS)
						{
							if (!GetXmlData(&p, attrib, sizeof(attrib)))
							{
								printf("ParseMissions: Unable to parse tag: %s", m_Tag);
								m_Success = false;
							}
							else if (current_node->reward)
							{
                                int length = strlen(attrib);
                                current_node->reward->description = new char [length+1];
                                strcpy(current_node->reward->description, attrib);
							}
                            else
                            {
                                printf("ParseMissions: <%s> must be contained within a <Rewards> tag\n", m_Tag);
                                m_Success = false;
                            }
						}
						/*else if (m_CurrentID == XML_TAG_ID_FAILURE)
						{
							if (!GetXmlData(&p, current_mission->mission.auxMissionDetails.FailureConsequence, sizeof(current_mission->mission.auxMissionDetails.FailureConsequence)))
							{
								printf("ParseMissions: Unable to parse tag: %s", m_Tag);
								m_Success = false;
							}
							else
							{
								current_mission->mission.auxMissionDetails.Flags[1] |= 0x01;
							}
						}
						else if (m_CurrentID == XML_TAG_ID_FACTION)
						{
							if (!GetXmlData(&p, current_mission->mission.auxMissionDetails.IssuingFaction, sizeof(current_mission->mission.auxMissionDetails.IssuingFaction)))
							{
								printf("ParseMissions: Unable to parse tag: %s", m_Tag);
								m_Success = false;
							}
							else
							{
								current_mission->mission.auxMissionDetails.Flags[1] |= 0x02;
							}
						}
						else
						{
							printf("ParseMissions: <%s> must be within a <Rewards>, <Failure>, or <Faction> tag\n", m_Tag);
							m_Success = false;
						}*/
					}
					else
					{
						printf("ParseMissions: <%s> must be contained within a <Stage> tag\n", m_Tag);
						m_Success = false;
					}
					break;
				case XML_TAG_ID_REWARDS :
					if (current_node)
					{
                        m_CurrentID = m_XmlTagID;
                        current_node->reward = new MissionReward;
                        memset(current_node->reward, 0, sizeof(MissionReward));
					}
					else
					{
						printf("ParseMissions: <%s> must be contained within a <Stage> tag\n", m_Tag);
						m_Success = false;
					}
					break;
				case XML_TAG_ID_FACTION :
					if (current_mission)
					{
						if (m_CurrentID == XML_TAG_ID_REWARDS ||
							m_CurrentID == XML_TAG_ID_FAILURE)
						{
							if (!GetXmlAttribute(XML_ATTRIB_NAME, attrib1, sizeof(attrib1), true) ||
								!GetXmlData(&p, attrib, sizeof(attrib)))
							{
								printf("ParseMissions: Unable to parse tag: %s", m_Tag);
								m_Success = false;
							}
							else
							{
								if (m_CurrentID == XML_TAG_ID_REWARDS)
								{
                                    if (current_node->reward)
                                    {
                                        current_node->reward->FactionType = atoi(attrib1);
                                        current_node->reward->FactionReward = atoi(attrib);
                                    }
								}
								else
								{
                                    //TODO:Forfeit
									//current_mission->mission.ForfeitConsequences.FactionType = atoi(attrib1);
									//current_mission->mission.ForfeitConsequences.FactionReward = atoi(attrib);
								}
							}
						}
						else
						{
							printf("ParseMissions: <%s> must be contained within a <Rewards> or <Failure> tag\n", m_Tag);
							m_Success = false;
						}
					}
					else
					{
						printf("ParseMissions: <%s> must be contained within a <Mission> tag\n", m_Tag);
						m_Success = false;
					}
					break;
				case XML_TAG_ID_CREDITS :
					if (current_mission)
					{
						if (m_CurrentID == XML_TAG_ID_REWARDS)
						{
							if (!GetXmlData(&p, attrib, sizeof(attrib)))
							{
								printf("ParseMissions: Unable to parse tag: %s", m_Tag);
								m_Success = false;
							}
							else
							{
                                if (current_node->reward)
                                {
                                    current_node->reward->CreditReward = atoi(attrib);
                                }
							}
						}
						/*else if (m_CurrentID == XML_TAG_ID_FAILURE)
						{
							if (!GetXmlData(&p, attrib, sizeof(attrib)))
							{
								printf("ParseMissions: Unable to parse tag: %s", m_Tag);
								m_Success = false;
							}
							else
							{
								current_mission->mission.ForfeitConsequences.CreditReward = atoi(attrib);
							}
						}*/
						else
						{
							printf("ParseMissions: <%s> must be contained within a <Rewards> or <Failure> tag\n", m_Tag);
							m_Success = false;
						}
					}
					else
					{
						printf("ParseMissions: <%s> must be contained within a <Mission> tag\n", m_Tag);
						m_Success = false;
					}
					break;
				case XML_TAG_ID_HULLUPGRADE :
					if (current_mission)
					{
						if (m_CurrentID == XML_TAG_ID_REWARDS)
						{
							if (!GetXmlData(&p, attrib, sizeof(attrib)))
							{
								printf("ParseMissions: Unable to parse tag: %s", m_Tag);
								m_Success = false;
							}
							else
							{
                                if (current_node->reward)
                                {
                                    current_node->reward->HullUpgrade = atoi(attrib);
                                }
							}
						}
						else
						{
							printf("ParseMissions: <%s> must be contained within a <Rewards> or <Failure> tag\n", m_Tag);
							m_Success = false;
						}
					}
					else
					{
						printf("ParseMissions: <%s> must be contained within a <Mission> tag\n", m_Tag);
						m_Success = false;
					}
					break;
				case XML_TAG_ID_EXPERIENCE :
					if (current_mission)
					{
						if (m_CurrentID == XML_TAG_ID_REWARDS)
						{
							if (!GetXmlAttribute(XML_ATTRIB_TYPE, attrib1, sizeof(attrib1), true) ||
								!GetXmlData(&p, attrib, sizeof(attrib)))
							{
								printf("ParseMissions: Unable to parse tag: %s", m_Tag);
								m_Success = false;
							}
							else if (current_node->reward)
							{
								if (strcmp(attrib1, "Explore") == 0)
								{
                                    current_node->reward->ExploreReward = atoi(attrib);
								}
								else if (strcmp(attrib1, "Combat") == 0 )
								{
                                    current_node->reward->CombatReward = atoi(attrib);
								}
								else if (strcmp(attrib1, "Trade") == 0)
								{
                                    current_node->reward->TradeReward = atoi(attrib);
								}
								else
								{
									printf("ParseMissions: Unable to parse tag: %s: Type must be either Explore, Combat, or Trade", m_Tag);
									m_Success = false;
								}
							}
						}
						/*else if (m_CurrentID == XML_TAG_ID_FAILURE)
						{
							if (!GetXmlAttribute(XML_ATTRIB_TYPE, attrib1, sizeof(attrib1), true) ||
								!GetXmlData(&p, attrib, sizeof(attrib)))
							{
								printf("ParseMissions: Unable to parse tag: %s", m_Tag);
								m_Success = false;
							}
							else
							{
								if (strcmp(attrib1, "Explore") == 0)
								{
									current_mission->mission.ForfeitConsequences.ExploreReward = atoi(attrib);
								}
								else if (strcmp(attrib1, "Combat") == 0 )
								{
									current_mission->mission.ForfeitConsequences.CombatReward = atoi(attrib);
								}
								else if (strcmp(attrib1, "Trade") == 0)
								{
									current_mission->mission.ForfeitConsequences.TradeReward = atoi(attrib);
								}
								else
								{
									printf("ParseMissions: Unable to parse tag: %s: Type must be either Explore, Combat, or Trade", m_Tag);
									m_Success = false;
								}
							}
						}*/
						else
						{
							printf("ParseMissions: <%s> must be contained within a <Rewards> or <Failure> tag\n", m_Tag);
							m_Success = false;
						}
					}
					else
					{
						printf("ParseMissions: <%s> must be contained within a <Mission> tag\n", m_Tag);
						m_Success = false;
					}
					break;
				case XML_TAG_ID_FAILURE :
					if (current_mission)
					{
						m_CurrentID = m_XmlTagID;
					}
					else
					{
						printf("ParseMissions: <%s> must be contained within a <Mission> tag\n", m_Tag);
						m_Success = false;
					}
					break;
                case XML_TAG_ID_TALKTREE:
                    if (current_node)
                    {
                        if (!GetXmlAttribute(XML_ATTRIB_TREENODEID, attrib1, sizeof(attrib1), true))
                        {
                            printf("ParseMissions: Unable to parse tag: %s", m_Tag);
                            m_Success = false;
                        }
                        else
                        {
                            m_CurrentID = m_XmlTagID;
                            if (current_node->talk_tree_list == 0)
                            {
                                current_node->talk_tree_list = new TalkTreeList;
                                current_node->talk_tree_list->push_back(current_talktree_node); //create a 'zero' dummy entry
                            }

                            current_talktree_node = new TalkTreeNode;
                            memset(current_talktree_node, 0, sizeof(TalkTreeNode));
                            current_talktree_node->built_text = false;
                            current_talktree_node->node_id = atoi(attrib1);
                            current_node->talk_tree_list->push_back(current_talktree_node);
                        }
                    }
                    else
                    {
						printf("ParseMissions: <%s> must be contained within a <Stage> tag\n", m_Tag);
						m_Success = false;
                    }
                    break;
                case XML_TAG_ID_TEXT:
                    if (current_talktree_node)
                    {
						if (m_CurrentID == XML_TAG_ID_TALKTREE)
						{
							if (!GetXmlData(&p, attrib, sizeof(attrib)))
							{
								printf("ParseMissions: Unable to parse tag: %s", m_Tag);
								m_Success = false;
							}
                            else if (current_talktree_node->talk_text == 0)
                            {
                                int sizeAttrib = strlen(attrib);
                                int read=0, write=0;
                                char chr;
                                while (attrib[read] != 0)
                                {
                                    if (attrib[read] == '\\' &&
                                        read+1 < sizeAttrib &&
                                        attrib[read+1] != '\\')
                                    {
                                        read++;
                                        chr = atoi(&attrib[read]);
                                        if (chr > 10)
                                            read++;
                                        if (chr>100)
                                            read++;
                                        if (chr>255)
                                        {
                                            printf("MissionParser: \\\\ Commands can not be larger than 255\n");
                                            m_Success = false;
                                            break;
                                        }
                                        attrib[write] = chr;
                                    }
                                    else
                                    {
                                        attrib[write] = attrib[read];
                                    }
                                    write++;
                                    read++;
                                }
                                attrib[write] = 0;

                                current_talktree_node->talk_text = new char [write+3];
                                memset(current_talktree_node->talk_text, 0, write+3);
                                memcpy(current_talktree_node->talk_text, attrib, (write));
                                current_talktree_node->talk_text_len = (write+3);
							}
						}
                    }
                    break;
                case XML_TAG_ID_BRANCH:
                    if (current_talktree_node)
                    {
						if (m_CurrentID == XML_TAG_ID_TALKTREE)
						{
                            if (!GetXmlAttribute(XML_ATTRIB_TREENODEID, attrib1, sizeof(attrib1), true))
                            {
                                printf("ParseMissions: Unable to parse branch node tag: %s", m_Tag);
                                m_Success = false;
                            }
                            else
                            { //got branch destination, now read text
                                if (!GetXmlData(&p, attrib, sizeof(attrib)))
                                {
                                    printf("ParseMissions: Unable to parse tag: %s", m_Tag);
                                    m_Success = false;
                                }
                                else
                                {
                                    if (current_talktree_node->branch_desc == 0)
                                    {
                                        current_talktree_node->branch_desc = new BranchTextList;
                                    }

                                    int length = strlen(attrib);
                                    BranchDesc *this_desc = new BranchDesc;
                                    this_desc->branch_text = new char [length + 1];
                                    this_desc->branch_dest = atoi(attrib1);
                                    strcpy(this_desc->branch_text, attrib);
                                    current_talktree_node->branch_desc->push_back(this_desc);
                                }
                            }
                        }
                    }
                    break;
                case XML_TAG_ID_NEXT_STAGE:
                    if (current_talktree_node)
                    {
                        if (m_CurrentID == XML_TAG_ID_TALKTREE)
                        {
                            if (!GetXmlData(&p, attrib, sizeof(attrib)))
                            {
                                printf("ParseMissions: Unable to parse tag: %s", m_Tag);
                                m_Success = false;
                            }
                            else
                            {
                                int dest = atoi(attrib);
                                current_talktree_node->tree_dest = dest;
                            }
                        }
                    }
                    break;
                case XML_TAG_ID_LOCATION:
                    if (current_node)
                    {
                        if (!GetXmlData(&p, attrib, sizeof(attrib)))
                        {
                            printf("ParseMissions: Unable to parse tag: %s", m_Tag);
                            m_Success = false;
                        }
                        else
                        {
                            current_node->location = g_StringMgr->GetStr(attrib);
                        }
                    }
                    else
                    {
						printf("ParseMissions: <%s> must be contained within a <Stage> tag\n", m_Tag);
						m_Success = false;
                    }
                    break;
                case XML_TAG_ID_OBJECT:
                    if (current_node)
                    {
                        if (!GetXmlData(&p, attrib, sizeof(attrib)))
                        {
                            printf("ParseMissions: Unable to parse tag: %s", m_Tag);
                            m_Success = false;
                        }
                        else
                        {
                            //push back text into vector manager
                            if (!current_node->object_name_list)
                            {
                                current_node->object_name_list = new ObjNameList;
                            }
                            current_node->object_name = g_StringMgr->GetStr(attrib); //the string is going to be loaded into the string manager anyway, so no waste!
                            current_node->object_name_list->push_back(g_StringMgr->GetStr(attrib));
                        }
                    }
                    else
                    {
						printf("ParseMissions: <%s> must be contained within a <Stage> tag\n", m_Tag);
						m_Success = false;
                    }
                    break;
                case XML_TAG_ID_NPC:
                    if (current_node)
                    {
                        if (!GetXmlAttribute(XML_ATTRIB_ID, attrib1, sizeof(attrib1), true))
                        {
                            printf("ParseMissions: Unable to parse tag: %s", m_Tag);
                            m_Success = false;
                        }
                        else
                        {                            
                            if (!GetXmlData(&p, attrib, sizeof(attrib)))
                            {
                                printf("ParseMissions: Unable to parse tag: %s", m_Tag);
                                m_Success = false;
                            }
                            else
                            {
                                current_node->npc_name = g_StringMgr->GetStr(attrib);
                                current_node->npc_id = atoi(attrib1);
                            }
                        }
                    }
                    else
                    {
						printf("ParseMissions: <%s> must be contained within a <Stage> tag\n", m_Tag);
						m_Success = false;
                    }
                    break;
                case XML_TAG_ID_ITEM:
                    if (current_node)
                    {
                        attrib1[0] = 0;
                        if (!GetXmlAttribute("Stack", attrib1, sizeof(attrib1), false))
                        {
                            printf("ParseMissions: Unable to parse tag: %s", m_Tag);
                            m_Success = false;
                        }

                        if (m_CurrentID == XML_TAG_ID_REWARDS)
                        {
                            if (!GetXmlData(&p, attrib, sizeof(attrib)))
                            {
                                printf("ParseMissions: Unable to parse tag: %s", m_Tag);
                                m_Success = false;
                            }
                            else if (current_node->reward)
                            {
                                current_node->reward->ItemRewardID = atoi(attrib);
                            }
                            else
                            {
                                printf("ParseMissions: <%s> must be contained within a <Rewards> tag\n", m_Tag);
                                m_Success = false;
                            }
                        }
                        else
                        {
                            if (!GetXmlData(&p, attrib, sizeof(attrib)))
                            {
                                printf("ParseMissions: Unable to parse tag: %s", m_Tag);
                                m_Success = false;
                            }
                            else
                            {
                                current_node->item_id = atoi(attrib);
                                if (atoi(attrib1) > 0)
                                {
                                    current_node->item_quantity = atoi(attrib1);
                                }
                                else
                                {
                                    current_node->item_quantity = 1;
                                }
                            }
                        }
                    }
                    else
                    {
                        printf("ParseMissions: <%s> must be contained within a <Stage> tag\n", m_Tag);
                        m_Success = false;
                    }
                    break;

				default :
					printf("ParseMissions: Unrecognized tag: %s", m_Tag);
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
	}

	return m_Success;
}

Mission *MissionParser::GetMission(long mission_id)
{
    return (m_MissionList[mission_id]);
}

bool MissionParser::Initialise()
{
    MissionList::iterator itrMList;

    //first spin through each mission
    for (itrMList = m_MissionList.begin(); itrMList < m_MissionList.end(); ++itrMList) 
    {
        (*itrMList)->InitialiseNodes();
    }   

    return true;
}
