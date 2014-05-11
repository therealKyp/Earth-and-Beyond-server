// SkillsDatabaseSQL.h

#ifndef _SKILLSDATABASE_SQL_H_INCLUDED_
#define _SKILLSDATABASE_SQL_H_INCLUDED_

#pragma warning(disable:4786) 

#include <map>


// forward references

struct SkillConversion;

typedef std::map<unsigned long, SkillConversion*> SkillConversionList;

class SkillsContent
{
// Constructor/Destructor
public:
    SkillsContent();
	virtual ~SkillsContent();

// Public Methods
public:
    bool        LoadSkillsContent();
    long		GetSkillLevel(long skill_id);
    char	*	GetSkillDescription(long skill_id);
    long        GetBaseSkillID(long skill_id);

// Private Methods
private:

// Private Member Attributes
private:
    SkillConversionList		m_SkillConvList;
    long					m_highest_id;
	bool					m_updating;

};



//This is incomplete, just want to get some data read in for now
struct SkillConversion
{
    char   *m_Description;
	u8		m_Level;
	u8		m_BaseSkillID;
};


#endif // _SKILLSDATABASE_SQL_H_INCLUDED_

