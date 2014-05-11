// FactionDataSQL.h

#ifndef _FACTIONDATABASE_SQL_H_INCLUDED_
#define _FACTIONDATABASE_SQL_H_INCLUDED_

#pragma warning(disable:4786) 

#include <map>

// forward references
class sql_connection_c;
struct FactionData;

typedef std::map<unsigned long, FactionData*> FactionList;
typedef std::map<unsigned long, float> FactionValues;

class Factions
{
// Constructor/Destructor
public:
    Factions();
	virtual ~Factions();

// Public Methods
public:
    bool			LoadFactions();
    FactionData *   GetFactionData(long faction_id);
    void			ReloadFactionData();

	float			GetFactionStanding(long faction_id, long faction_ref);
	char *			GetFactionName(long faction_id);
	char *			GetFactionPDAName(long faction_id);
	long			GetFactionCount();
	bool			GetPDA(long fection_id);
	long			GetFactionOrderFromID(long faction_id);

// Private Methods
private:

// Private Member Attributes
private:
    FactionList		m_factions;
	long			m_faction_count;
};

struct FactionData
{
	char	*m_name;
	char	*m_description;
	char	*m_PDA_text;
	long	m_faction_order;
	bool	m_player_PDA;
	FactionValues m_value;
};


#endif // _FACTIONDATABASE_SQL_H_INCLUDED_

