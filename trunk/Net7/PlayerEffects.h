#pragma warning(disable:4786)

#ifndef _PLAYER_EFFECTS_H_INCLUDED_
#define _PLAYER_EFFECTS_H_INCLUDED_

#include "ItemBase.h"
#include "PacketStructures.h"
#include <map>
#include <vector>

using namespace std;

class Object;
class Player;

struct MapEffect
{
	ObjectEffect obj_effect;
	
} ATTRIB_PACKED;

typedef map<int, MapEffect> mapEffect;

class Effects
{
public:
    Effects();
    ~Effects();

    void	Init(Player *);
	int		AddEffect(ObjectEffect *obj_effect);
	bool	RemoveEffect(int EffectID);
	bool	SendEffects(Player *p);
	bool	SendRemoveEffects(Player *p);

private:

private:
    Player * m_Player;
	mapEffect m_EffectList;
};

#endif