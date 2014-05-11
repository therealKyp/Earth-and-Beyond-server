// SkillParser.h

#ifndef _SKILL_PARSER_H_INCLUDED_
#define _SKILL_PARSER_H_INCLUDED_

#include "XmlParser.h"
#include "PlayerSkills.h"

struct SkillLevels;

class SkillParser : protected XmlParser
{
// Constructor/Destructor
public:
    SkillParser();
	virtual ~SkillParser();

// Public Methods
public:
    bool LoadSkills();
    SkillData * GetSkillList();
	SkillLevels	* GetAbilityList();

// Private Methods
private:
    bool ParseSkill(char *);
    void ParseClass(int, int);
    SKILL_TYPE GetSkillType(char *);
    SKILL_REQ GetSkillReq(char *);

// Private Member Attributes
private:
    SkillData m_SkillList[64];
	SkillLevels * m_AbilityList;
};


#endif // _SKILL_PARSER_H_INCLUDED_

