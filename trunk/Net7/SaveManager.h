// SaveManager.h

#ifndef _SAVE_MANAGER_H_INCLUDED_
#define _SAVE_MANAGER_H_INCLUDED_

#include "Mutex.h"
#include "MessageQueue.h"
#include "mysql/mysqlplus.h"

#define SAVE_MESSAGE_MAX_LENGTH 256

class Object;
class Player;

// There is one instance of this class for all Players on this server
class SaveManager
{
public:
    SaveManager();
    virtual ~SaveManager();

	void	CheckSaves(void);
	//void	RunSaveThread(void);
	void	AddSaveMessage(short save_code, long player_id, short bytes, unsigned char *data);

private:
	void	HandleSaveCode(short save_code, long player_id, short bytes, unsigned char *data);

	void	HandleAdvanceLevel(long player_id, short bytes, unsigned char *data);
	void	HandleAdvanceSkill(long player_id, short bytes, unsigned char *data);
	void	HandleChangeInventory(long player_id, short bytes, unsigned char *data);
	void	HandleChangeEquipment(long player_id, short bytes, unsigned char *data);
	void	HandleAwardXP(long player_id, short bytes, unsigned char *data);
	void	HandleCreditChange(long player_id, short bytes, unsigned char *data);
	void	HandleStorePosition(long player_id, short bytes, unsigned char *data);
	void	HandleAdvanceMission(long player_id, short bytes, unsigned char *data);
	void	HandleAdvanceMissionFlags(long player_id, short bytes, unsigned char *data);
	void	HandleChangeAmmo(long player_id, short bytes, unsigned char *data);
	void	HandleHullUpgrade(long player_id, short bytes, unsigned char *data);
	void	HandleHullLevelChange(long player_id, short bytes, unsigned char *data);
	void	HandleMissionComplete(long player_id, short bytes, unsigned char *data);
	void	HandleWipeCharacter(long player_id);
	void	HandleFullWipeCharacter(long player_id);
	void	HandleDiscoverNav(long player_id, short bytes, unsigned char *data);
	void	HandleExploreNav(long player_id, short bytes, unsigned char *data);
	void	HandleSetSkillPoints(long player_id, short bytes, unsigned char *data);
	void	HandleSetRegisteredStarbase(long player_id, short bytes, unsigned char *data);
	void	HandleSaveEnergyLevels(long player_id, short bytes, unsigned char *data);
	void	HandleUpdateDatabase(long player_id, short bytes, unsigned char *data);
	void	HandleFactionUpdate(long player_id, short bytes, unsigned char *data);
	void	HandleFullFactionWipe(long player_id, short bytes, unsigned char *data);


	static UINT WINAPI SaveManager::StartSaveThread(void *Param);

private:
	MessageQueue    * m_SaveQueue;
	sql_connection_c  m_SQL_Conn;

	char			  m_QueryStr[512];
	CircularBuffer  * m_SaveBuffer;

};

struct EnbSaveHeader
{
    short   size;
    short   save_code;
	long	player_id;
} ATTRIB_PACKED;

#define SAVE_CODE_ADVANCE_LEVEL						0x0001
#define SAVE_CODE_ADVANCE_SKILL						0x0002
#define SAVE_CODE_CHANGE_INVENTORY					0x0003
#define SAVE_CODE_CHANGE_EQUIPMENT					0x0004
#define SAVE_CODE_AWARD_XP							0x0005
#define SAVE_CODE_CREDIT_LEVEL						0x0006
#define SAVE_CODE_STORE_POSITION					0x0007
#define SAVE_CODE_ADVANCE_MISSION					0x0008
#define SAVE_CODE_CHARACTER_PROGRESS_WIPE			0x0009
#define SAVE_CODE_MISSION_FLAGS						0x000A
#define SAVE_CODE_CHANGE_AMMO						0x000B
#define SAVE_CODE_HULL_UPGRADE						0x000C
#define SAVE_CODE_HULL_LEVEL_CHANGE					0x000D
#define SAVE_CODE_FULL_CHARACTER_WIPE				0x000E
#define SAVE_CODE_MISSION_COMPLETE					0x000F
#define SAVE_CODE_DISCOVER_NAV						0x0010
#define SAVE_CODE_EXPLORE_NAV						0x0011
#define SAVE_CODE_SET_SKILLPOINTS					0x0012
#define	SAVE_CODE_SET_STARBASE						0x0013
#define SAVE_CODE_SET_ENERGY_LEVELS					0x0014
#define SAVE_CODE_UPDATE_DATABASE					0x0015
#define SAVE_CODE_FACTION_CHANGE					0x0016
#define SAVE_CODE_FULL_FACTION_WIPE					0x0017

#define	PLAYER_VAULT								0x0001
#define PLAYER_INVENTORY							0x0002

#define DISCOVER_NAV								0x0001
#define EXPLORE_NAV									0x0002

#endif // _SAVE_MANAGER_H_INCLUDED_
