#ifdef WIN32
#pragma warning(disable:4786)
#endif

#ifndef _PLAYER_BUFFS_H_INCLUDED_
#define _PLAYER_BUFFS_H_INCLUDED_

#include "ItemBase.h"
#include "PacketStructures.h"
#include "AuxClasses/AuxShipIndex.h"
#include <map>
#include <vector>

using namespace std;

class Object;
class Player;

struct StatList
{
	char	StatName[128];
	int		StatType;
	float	Value;
} ATTRIB_PACKED;

struct MapBuffs
{
	_Buff BuffData;
	int StatID[5];			// Stat ID's to remove effects
} ATTRIB_PACKED;

struct Buff
{
	char BuffType[128];
	int ExpireTime;
	bool IsPermanent;
	struct StatList Stats[5];
} ATTRIB_PACKED;

typedef map<const int, MapBuffs> mapBuffs;

class PlayerBuffs
{
public:
    PlayerBuffs();
    ~PlayerBuffs();

    void	Init(Player *);
	int		AddBuff(struct Buff *AddBuff);
	void	Update(int BuffID);				// Update expire buffs

private:

private:
    Player * m_Player;
	mapBuffs m_BuffList;
	int		 m_BuffID;		// Incomented to never repeat
};

#endif
