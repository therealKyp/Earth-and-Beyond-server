// EffectManager.h

#ifndef _EFFECT_MANAGER_H_INCLUDED_
#define _EFFECT_MANAGER_H_INCLUDED_

#include "Connection.h"


class ServerManager;
class Connection;

class EffectManager
{
// Constructor/Destructor
public:
    EffectManager();
	virtual ~EffectManager();

public:
	void AddPlayer(Connection *C, int PlayerID);								// Add a player to list
	void RemovePlayer(int PlayerID);											// Remove Player
	int	AddObjectEffect(int PlayerID, ObjectEffect *object_effect);		// Add Effect Returns EffectID
	void RemoveObjectEffect(int EffectID);									// Remove Effect
	void RemoveEffectsByPlayer(int PlayerID);								
	void RemoveEffectToEveryone(int EffectID);


private:
	struct ObjectLists {
		int GameID;
		ObjectEffect object_effect;
		struct ObjectLists * Next;
	};

	struct PlayerLists {
		int PlayerID;
		Connection * c;
		struct PlayerLists * Next;
	};

	typedef std::vector<PlayerLists*> Players;
	typedef std::vector<ObjectLists*> EObjects;

private:
	void SendObjectEffects(PlayerLists * Player);				// Update Object Effects
	void SendEffectToEveryone(int GameID, ObjectEffect * object_effect);

private:
	Mutex			  m_Mutex;
	Players			  m_Players;
	EObjects		  m_ObjectLists;
	int				  LastEffectID;


};

#endif // _EFFECT_MANAGER_H_INCLUDED_
