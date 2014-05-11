// StationLoader.h

#ifdef WIN32
#pragma warning(disable:4786)
#endif

#ifndef _STATION_LOADER_H_INCLUDED_
#define _STATION_LOADER_H_INCLUDED_

#include <map>
#include <vector>
#include "XmlParser.h"
#include "PacketStructures.h"
#include "mysql/mysqlplus.h"
#include "TalkTree.h"
#include "TalkTreeParser.h"

using namespace std;

struct VendorItemList
{
	int ItemID;
	int SellPrice;
	int BuyPrice;
	int Quanity;
} ATTRIB_PACKED;

struct RoomTemplate
{
	int Index;
	int Style;
	float FogNear;
	float FogFar;
	int FogRed, FogGreen, FogBlue;
} ATTRIB_PACKED;

struct TermTemplate
{
	int Room;
	int Type;
	int Location;
	int Unknown1;
} ATTRIB_PACKED;

typedef map<int, VendorItemList *> mapItems;
typedef map<int, struct NPCTemplate *> mapNPCs;
typedef map<int, struct RoomTemplate *> mapRooms;
typedef map<int, struct TermTemplate *> mapTerminals;
typedef map<int, struct StationTemplate *> mapStations;
typedef vector<int> NPCList;
typedef vector<int> RoomList;
typedef vector<int> TermList;
typedef vector<int> vecItemList;

struct VendorGroup
{
	float SellMult;
	float BuyMult;
	bool BuyOnlyList;
	int NItems;

	vecItemList Items;			// List of Item IDs - refers to m_ItemList
} ATTRIB_PACKED;

struct NPCTemplate
{
	int Room;
	int Location;
	int Booth;
	int VGroup;
	int StarbaseID;
	int Unknown1;
	int Unknown2;
	InteractionNode NPCInteraction;
	//char * TalkTree;

	struct AvatarData Avatar;
	struct VendorGroup Vendor;

	struct StationTemplate * ParentStation;		// Points to our Parrent Station
} ATTRIB_PACKED;

struct StationTemplate
{
	int StationID;
	int Race;
	int Type;
	int NumRooms;
	int NumTerms;
	int	NumNPCs;
	int IsActive;

	char *Name;
	char WelcomeMessage[256];

	NPCList			NPCs; //list of NPC ids
	RoomList		Rooms;//list of Room ids
	TermList	    Terms;
} ATTRIB_PACKED;

// forward references
class StationLoader
{
// Constructor/Destructor
public:
    StationLoader();
	virtual ~StationLoader();

// Public Methods
public:
    bool LoadStations();
    StationTemplate * GetStation(int ID);
	NPCTemplate    *GetNPC(int ID);
    RoomTemplate   *GetRoom(int ID);
    TermTemplate   *GetTerminal(int ID);
	VendorItemList *GetVendorItem(int ID);

	char * _GetSectorName(long sector_id);  //do not use these directly
	char * _GetSystemName(long sector_id);

// Private Methods
private:
	char * GetFieldData(char Field[50][50], sql_row_c RowData, char *FindField);
    bool ParseStation(char *data);
	int  GetStationSectorID(int ObjectID);
    void LoadNPCs(sql_connection_c *connection);
    void LoadRooms(sql_connection_c *connection);
    void AddRooms(StationTemplate *current_station, long station_id, sql_connection_c *connection);
    void AddTerminals(StationTemplate *current_station, long room_id, sql_connection_c *connection);
    void AddNPCs(StationTemplate *current_station, long room_id, sql_connection_c *connection);
	void BuildMissionTree(NPCTemplate *npc);

// Private Member Attributes
private:
	mapStations m_StationList;
	mapNPCs		m_NPCList;
    mapRooms    m_RoomList;
    mapTerminals m_TermList;
	mapItems	m_ItemList;
	long		m_TalkTreeBufferLen;
	char *		m_TalkTreeBuffer;
	TalkTreeParser m_TalkTreeParser;
};


#endif // _STATION_LOADER_H_INCLUDED_

