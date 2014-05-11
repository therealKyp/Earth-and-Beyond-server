// SkillParser.cpp

#include "Net7.h"
#include "SkillParser.h"
#include "XmlParser.h"

static XmlParser::XmlTagLookupTable skill_tag_lookup_table[] =
{
	{ XML_TAG_ID_NAME,				   XML_TAG_NAME },
    { XML_TAG_ID_SKILL,                XML_TAG_SKILL },
    { XML_TAG_ID_WARRIOR,              XML_TAG_WARRIOR },
    { XML_TAG_ID_SENTINEL,             XML_TAG_SENTINEL },
    { XML_TAG_ID_PRIVATEER,            XML_TAG_PRIVATEER },
    { XML_TAG_ID_DEFNDER,              XML_TAG_DEFNDER },
    { XML_TAG_ID_EXPLORER,			   XML_TAG_EXPLORER },
	{ XML_TAG_ID_ENFORCER,			   XML_TAG_ENFORCER },
    { XML_TAG_ID_SCOUT, 			   XML_TAG_SCOUT },
	{ XML_TAG_ID_SEEKER,			   XML_TAG_SEEKER },
    { XML_TAG_ID_TRADESMAN,            XML_TAG_TRADESMAN },
    { XML_TAG_ID_INVALID,              0 }
};

SkillParser::SkillParser()
{
    m_XmlTagLookupTable = skill_tag_lookup_table;
    memset(m_SkillList,0,sizeof(m_SkillList));
	m_AbilityList = NULL;
}

SkillParser::~SkillParser()
{
}

bool SkillParser::LoadSkills()
{
    bool success = false;
    char orig_path[MAX_PATH];
    GetCurrentDirectory(sizeof(orig_path), orig_path);
    SetCurrentDirectory(SERVER_DATABASE_PATH);

    FILE *f = fopen("Skills.xml", "r");
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
            success = ParseSkill(data);

			if (!success)
				exit(1);
            delete [] data;
        }
        fclose(f);
    }
    else
    {
        printf("Error opening Skills.xml\n");
    }

    SetCurrentDirectory(orig_path);
    return (success);
}

bool SkillParser::ParseSkill(char *data)
{
    char * p = data;
    int CurrentSkill = 0;

    char attrib1[64];
    char attrib2[64];
    char attrib3[64];
    char attrib4[64];
	/*
    char attrib5[64];
    char attrib6[64];
    char attrib7[64];
    char attrib8[64];
    char attrib9[64];
    char attrib10[64];
    char attrib11[64];
	*/

    printf("Parsing Skills.xml\n");

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
            case XML_TAG_ID_SKILL:
                if ((GetXmlAttribute(XML_ATTRIB_TYPE, attrib1, sizeof(attrib1), true)) &&
                    (GetXmlAttribute(XML_ATTRIB_CATEGORY, attrib2, sizeof(attrib2), true)) &&
					(GetXmlAttribute(XML_TAG_NAME, attrib3, sizeof(attrib3), true)) &&
                    (GetXmlAttribute(XML_ATTRIB_ID, attrib4, sizeof(attrib4), true))
				   )
                {
					CurrentSkill = atoi(attrib4);
                    m_SkillList[CurrentSkill].Activatable = strcmp(attrib1, "Passive") ? true : false;
                    m_SkillList[CurrentSkill].RequirementScheme = GetSkillReq(attrib2);
                }
                else
                {
                    LogMessage("Parsing error in Skills.xml!\n");
                }

                break;

            case XML_TAG_ID_ENFORCER:
                // Terran Enforcer - Index 0
                ParseClass(CurrentSkill, 0);
                break;

            case XML_TAG_ID_TRADESMAN:
                // Terran Tradesman - Index 1
                ParseClass(CurrentSkill, 1);
                break;

            case XML_TAG_ID_SCOUT:
                // Terran Scout - Index 2
                ParseClass(CurrentSkill, 2);
                break;

            case XML_TAG_ID_DEFNDER:
                // Jenquai Defender - Index 3
                ParseClass(CurrentSkill, 3);
                break;

            case XML_TAG_ID_SEEKER:
                // Jenquai Seeker - Index 4
                ParseClass(CurrentSkill, 4);
                break;

            case XML_TAG_ID_EXPLORER:
                // Jenquai Explorer - Index 5
                ParseClass(CurrentSkill, 5);
                break;

            case XML_TAG_ID_WARRIOR:
				// Projen Warrior - Index 6
                ParseClass(CurrentSkill, 6);
                break;

            case XML_TAG_ID_PRIVATEER:
                // Projen Privateer - Index 7
                ParseClass(CurrentSkill, 7);
                break;

            case XML_TAG_ID_SENTINEL:
				// Projen Sentinel - Index 8
                ParseClass(CurrentSkill, 8);
                break;

            case XML_TAG_ID_NOTES:
                // Notes are ignored
                break;

            default :
                printf("ParseItemBase: Unrecognized tag: %s", m_Tag);
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

void SkillParser::ParseClass(int SkillIndex, int ClassIndex)
{
    char attrib1[64];
    char attrib2[64];
    char attrib3[64];
    char attrib4[64];

	if ((GetXmlAttribute(XML_ATTRIB_MAX, attrib1, sizeof(attrib1), true)) &&
		(GetXmlAttribute(XML_ATTRIB_LEARN_LVL, attrib2, sizeof(attrib2), true)) &&
		(GetXmlAttribute(XML_ATTRIB_LVL_TYPE, attrib3, sizeof(attrib3), true)) &&
		(GetXmlAttribute(XML_ATTRIB_QUEST, attrib4, sizeof(attrib4), true)))
	{
        m_SkillList[SkillIndex].ClassType[ClassIndex].MaxLevel = atoi(attrib1);
        m_SkillList[SkillIndex].ClassType[ClassIndex].LevelAquired = atoi(attrib2);
        m_SkillList[SkillIndex].ClassType[ClassIndex].LevelScheme = GetSkillType(attrib3);
        m_SkillList[SkillIndex].ClassType[ClassIndex].Quested = atoi(attrib4);
	}
}

SKILL_TYPE SkillParser::GetSkillType(char *str)
{
    if (strcmp(str, "PW") == 0)
    {
        return SKILL_PRIMWEP;
    }
    else if (strcmp(str, "SW") == 0)
    {
        return SKILL_SECWEP;
    }
    else if (strcmp(str, "PT") == 0)
    {
        return SKILL_PRIMTECH;
    }
    else if (strcmp(str, "ST") == 0)
    {
        return SKILL_SECTECH;
    }
    else if (strcmp(str, "NON") == 0)
    {
        return SKILL_OTHER;
    }
    else
    {
        LogMessage("Invalid SkillType: %s\n",str);
        return SKILL_OTHER;
    }
}

SKILL_REQ SkillParser::GetSkillReq(char *str)
{
    if (strcmp(str,"Total") == 0)
    {
        return SKILL_TOTAL;
    }
    else if (strcmp(str, "Combat") == 0)
    {
        return SKILL_COMBAT;
    }
    else if (strcmp(str, "Trade") == 0)
    {
        return SKILL_TRADE;
    }
    else if (strcmp(str, "Explore") == 0)
    {
        return SKILL_EXPLORE;
    }
    else
    {
        LogMessage("Invalid SkillRequirement: %s\n",str);
        return SKILL_TOTAL;
    }
}

SkillData * SkillParser::GetSkillList()
{
    return (m_SkillList);
}

SkillLevels * SkillParser::GetAbilityList()
{
    return (m_AbilityList);
}


