// BuffDatabaseSQL.h

#ifndef _BUFF_DATABASE_SQL_H_INCLUDED_
#define _BUFF_DATABASE_SQL_H_INCLUDED_

#pragma warning(disable:4786) 

#include <map>


struct BuffData
{
	int		EffectID;
	int		EffectLength;
};

typedef std::map<char *, BuffData> BuffList;

class BuffContent
{
// Constructor/Destructor
public:
    BuffContent();
	virtual ~BuffContent();

// Public Methods
public:
    bool        LoadBuffContent();
	long		GetBuffEffectTime(char *buff);
	long		GetBuffEffect(char *buff);


// Private Member Attributes
private:
    BuffList				m_BuffData;
	bool					m_updating;

};


#endif // _BUFF_DATABASE_SQL_H_INCLUDED_

