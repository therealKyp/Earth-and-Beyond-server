#include "net7.h"
#include "AbilityBase.h"
#include "PlayerClass.h"
#include "ServerManager.h"
#include "ObjectManager.h"

class PlayerClass;


void AbilityBase::SetTimer(long Duration)
{
	if (m_Player)
	{
		SectorManager *sm = m_Player->GetSectorManager();
		if (sm) sm->AddTimedCall(m_Player, B_ABILITY_REMOVE, Duration, NULL, m_AbilityID, 0, 0);
	}
}

void AbilityBase::SendError(char * EMsg)
{
	if (m_Player)
	{
		m_Player->SendPriorityMessageString(EMsg,"MessageLine",1000,4);
	}
}

float AbilityBase::GetInterruptThreshHold()
{
	return pow((float)m_Player->TotalLevel(), 4) + 20 + m_SkillLevel*100;
}