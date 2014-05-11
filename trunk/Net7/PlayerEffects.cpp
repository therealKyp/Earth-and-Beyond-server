#include "PlayerEffects.h"
#include "PlayerClass.h"


Effects::Effects()
{
    m_Player = 0;
}

Effects::~Effects()
{
	m_EffectList.clear();
}

void Effects::Init(Player * Owner)
{
    m_Player = Owner;
}

int Effects::AddEffect(ObjectEffect *obj_effect)
{
	int EffectID = GetNet7TickCount();

	obj_effect->EffectID = EffectID;
	memcpy(&m_EffectList[EffectID].obj_effect, obj_effect, sizeof(ObjectEffect));		// Copy effect in

	// Send to all player in your range
	if(m_Player->GetStartStatus())
	{
		m_Player->SendObjectEffectRL(obj_effect);
	}
	
	return EffectID;
}

bool Effects::RemoveEffect(int EffectID)
{
	if (m_EffectList.count(EffectID) != 0)
	{
		m_EffectList.erase(m_EffectList.find(EffectID));

		// Send Remove Effect to Range List
		m_Player->RemoveEffectRL(EffectID);
		return true;
	}
	else
	{
		LogMessage("Warning: Removing an Effect ID that does not exsist!\n");
		return false;
	}
}


bool Effects::SendRemoveEffects(Player *p)
{
	for(mapEffect::const_iterator Effect = m_EffectList.begin(); Effect != m_EffectList.end(); ++Effect)
	{
		p->SendRemoveEffect(Effect->first);
	}
	return true;
}


bool Effects::SendEffects(Player *p)
{
	for(mapEffect::const_iterator Effect = m_EffectList.begin(); Effect != m_EffectList.end(); ++Effect)
	{
		p->SendObjectEffect((ObjectEffect *) &Effect->second.obj_effect);
	}
	return true;
}