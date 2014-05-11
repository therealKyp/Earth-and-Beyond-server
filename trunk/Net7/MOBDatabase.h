// MOBDatabaseSQL.h

#ifndef _MOBDATABASE_SQL_H_INCLUDED_
#define _MOBDATABASE_SQL_H_INCLUDED_

#pragma warning(disable:4786) 

#include <map>
#include <vector>

// forward references
class ItemBase;
struct SectorData;
struct MOBData;
class sql_connection_c;
struct LootNode;

typedef std::map<unsigned long, MOBData*> MOBList;
typedef std::vector<LootNode*> MOBLoot;

class MOBContent
{
// Constructor/Destructor
public:
    MOBContent();
	virtual ~MOBContent();

// Public Methods
public:
    bool        LoadMOBContent();
    MOBData *   GetMOBData(long MOB_id);
	MOBData *	GetMOBDataFromBasset(short MOB_basset);
    void        ReloadMOBData(long MOB_id);
    long        GetMOBCount();

// Private Methods
private:
    //bool ParseSectorContent(char *data);
    //void AddMOBTypes(Object *obj, long resource_id, sql_connection_c *connection);

// Private Member Attributes
private:
    MOBList      m_MOB;
    long         m_highest_id;
	bool	     m_updating;

};

struct LootNode
{
	long item_base_id;
	float drop_chance;
	float usage_chance;
	u8 type;
	u8 quantity;
};

//This is incomplete, just want to get some data read in for now
struct MOBData
{
    char   *m_Name;
    u8      m_Level;
    u8      m_Intelligence;
    u8      m_Bravery;
    u8      m_Type;
    u16     m_FactionID;
    u16     m_Basset;
    u8      m_Altruism;
    u8      m_Agressiveness;
    float   m_Scale;
    float   m_HSV[3];

	//loot
	MOBLoot m_Loot;
};


#endif // _MOBDATABASE_SQL_H_INCLUDED_

