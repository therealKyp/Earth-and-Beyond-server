#include "Net7.h"
#include "AbilityBase.h"
#include "PlayerClass.h"
#include "ServerManager.h"
#include "ObjectManager.h"

class PlayerClass;


void AbilityBase::SetTimer(long Duration)
{
	if (m_Player)
	{
		m_Player->GetSectorManager()->AddTimedCall(m_Player, B_ABILITY_REMOVE, Duration, NULL, m_AbilityID, 0, 0);
	}
}

void AbilityBase::SendError(const char * EMsg)
{
	if (m_Player)
	{
		m_Player->SendPriorityMessageString(EMsg,"MessageLine",1000,4);
	}
}
