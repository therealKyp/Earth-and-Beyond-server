// EffectManager.cpp

#include "Net7.h"
#include "EffectManager.h"
#include "Opcodes.h"

EffectManager::EffectManager()
{
	LastEffectID = 0;
}

EffectManager::~EffectManager()
{

	EObjects::iterator itrObj;
	ObjectLists	* o;

	// Destroy ObjectEffects
	m_Mutex.Lock();
	for (itrObj = m_ObjectLists.begin(); itrObj != m_ObjectLists.end(); ++itrObj)
	{
		o = (*itrObj);
		if (o)
		{
			delete o;
		}
	}

    m_ObjectLists.clear();

	// Destroy Players
	Players::iterator itrPly;
	PlayerLists * p;

	m_Mutex.Lock();
	for (itrPly = m_Players.begin(); itrPly != m_Players.end(); ++itrPly)
	{
		p = (*itrPly);
		if (p)
		{
			delete p;
		}
	}
    m_Players.clear();
	m_Mutex.Unlock();
}

void EffectManager::AddPlayer(Connection *C, int PlayerID)
{
	PlayerLists	* p;

    LogDebug("EffectsManager: Add Player %d\n", PlayerID);

	p = new PlayerLists;

	// Add Data
	p->Next = 0;
	p->c = C;
	p->PlayerID = PlayerID;
	// ----------------------

	m_Players.push_back( p );		// Add to end of list!
	SendObjectEffects( p );		// Send Effects

}

void EffectManager::RemovePlayer(int PlayerID)
{
	Players::iterator itrPly;
	PlayerLists * p;


	LogMessage("EffectsManager: Removing Player %d\n", PlayerID);


	m_Mutex.Lock();
	for (itrPly = m_Players.begin(); itrPly != m_Players.end(); ++itrPly)
	{
		p = (*itrPly);

		if (p && (p->PlayerID == PlayerID))
		{
			m_Mutex.Unlock();
			RemoveEffectsByPlayer(PlayerID);	// Remove Effects by Player
			m_Mutex.Lock();
			delete p;
            m_Players.erase(itrPly);			// Remove from List
            break;
		}
	}
	m_Mutex.Unlock();
	return;
}

int EffectManager::AddObjectEffect(int PlayerID, ObjectEffect *object_effect)
{
	ObjectLists * Objects;

	Objects = new ObjectLists;

	Objects->Next = 0;
	Objects->GameID = PlayerID;
	memcpy(&Objects->object_effect, object_effect, sizeof(ObjectEffect));		// Copy in object effect
	Objects->object_effect.EffectID = LastEffectID;						    // Set EffectID
	LastEffectID++;


	m_Mutex.Lock();
	m_ObjectLists.push_back( Objects );
	m_Mutex.Unlock();

	SendEffectToEveryone(PlayerID, object_effect);
	return Objects->object_effect.EffectID;
}

void EffectManager::RemoveEffectsByPlayer(int PlayerID)
{
	EObjects::iterator itrObj;
	ObjectLists	* o;

	m_Mutex.Lock();

	for (itrObj = m_ObjectLists.begin(); itrObj != m_ObjectLists.end(); ++itrObj)
	{
		o = (*itrObj);

		if (o && (o->GameID == PlayerID))
		{
			LogDebug("Removing EffectID: %d\n", o->object_effect.EffectID);
			m_Mutex.Unlock();
			RemoveEffectToEveryone(o->object_effect.EffectID);			// Remove Effect
			m_Mutex.Lock();
			delete o;
			m_ObjectLists.erase(itrObj);
            --itrObj;
		}
	}
	m_Mutex.Unlock();
	return;
}

void EffectManager::RemoveObjectEffect(int EffectID)
{

	EObjects::iterator itrObj;
	ObjectLists	* o;

	m_Mutex.Lock();

	for (itrObj = m_ObjectLists.begin(); itrObj != m_ObjectLists.end(); ++itrObj)
	{
		o = (*itrObj);

		if (o && (o->object_effect.EffectID == EffectID))
		{
			m_Mutex.Unlock();
			RemoveEffectToEveryone(EffectID);			// Remove Effect
			m_Mutex.Lock();
			delete o;
			m_ObjectLists.erase(itrObj);
            --itrObj;
		}
	}
	m_Mutex.Unlock();
	return;
}


// Sends a Remove Effect to Everyone
void EffectManager::RemoveEffectToEveryone(int EffectID)
{
	Players::iterator itrPly;
	PlayerLists	* p;

	m_Mutex.Lock();
	for (itrPly = m_Players.begin(); itrPly != m_Players.end(); ++itrPly)
	{
		p = (*itrPly);
		if (p)
		{
			LogDebug("EffectsManager: Sending RemoveEffect (%d) to PlayerID: %d\n", EffectID, p->PlayerID);
			p->c->SendResponse(ENB_OPCODE_000F_REMOVE_EFFECT, (unsigned char *)&EffectID, sizeof(int));
		}
	}
	m_Mutex.Unlock();
	return;
}

// Sends Object Effect to Everyone
void EffectManager::SendEffectToEveryone(int GameID, ObjectEffect * object_effect)
{
	Players::iterator itrPly;
	PlayerLists	* p;

	m_Mutex.Lock();
	for (itrPly = m_Players.begin(); itrPly != m_Players.end(); ++itrPly)
	{
		p = (*itrPly);
		if (p)
		{
			LogDebug("EffectsManager: Sending Effect (%d) to PlayerID: %d\n", object_effect->EffectID, p->PlayerID);
			p->c->SendObjectEffect(object_effect);
		}
	}
	m_Mutex.Unlock();
	return;
}



void EffectManager::SendObjectEffects(PlayerLists * Player)				// Updates for a specific GameID
{
	if (!Player)			// Can't find Player
		return;

	EObjects::iterator itrObj;
	ObjectLists	* o;

	m_Mutex.Lock();

	for (itrObj = m_ObjectLists.begin(); itrObj != m_ObjectLists.end(); ++itrObj)
	{
		o = (*itrObj);

		if (o && Player && Player->c)
		{
			m_Mutex.Unlock();
			Player->c->SendObjectEffect(&o->object_effect);
			m_Mutex.Lock();
			delete o;
			m_ObjectLists.erase(itrObj);
            --itrObj;
			LogDebug("EffectsManager: Sending Effect (%d) to PlayerID: %d\n", o->object_effect.EffectID, o->GameID);
		}
	}
	m_Mutex.Unlock();
	return;
}