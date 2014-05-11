#include "PlayerBuffs.h"
#include "PlayerClass.h"
#include "Net7.h"


PlayerBuffs::PlayerBuffs()
{
    m_Player = 0;
	m_BuffID = 0;
}

PlayerBuffs::~PlayerBuffs()
{
	m_BuffList.clear();
}

void PlayerBuffs::Init(Player * Owner)
{
    m_Player = Owner;
}

int PlayerBuffs::AddBuff(struct Buff *AddBuff)
{
	// Create Buff data to be sent to client
	_Buff myBuff;
	memset(&myBuff,0,sizeof(_Buff));
	myBuff.BuffRemovalTime = AddBuff->ExpireTime;
	strcpy(myBuff.BuffType, AddBuff->BuffType);
	myBuff.IsPermanent = AddBuff->IsPermanent;
	// -----

	int FreeSpot;
	bool FreeFound = false;

	for(FreeSpot = 0;FreeSpot<16;FreeSpot++)
	{
		// See if it expired
		if (m_Player->ShipIndex()->Buffs.Buff[FreeSpot].GetBuffRemovalTime() < GetNet7TickCount())
		{
			// If so break
			FreeFound = true;
			break;
		}
	}

	if (FreeFound)
	{
		// Copy data in
		memcpy(&m_BuffList[m_BuffID].BuffData, &myBuff, sizeof(_Buff));
		// ---

		// Set Data in Aux
		m_Player->ShipIndex()->Buffs.Buff[FreeSpot].SetData(&myBuff);

		// Send Aux Data
		m_Player->SendAuxShip();


		if (AddBuff->EffectID != -1)
		{
			// Effect Information
			ObjectEffect obj_effect;

			// Send Effect
			obj_effect.Bitmask = 3;
			obj_effect.GameID = m_Player->GameID();
			obj_effect.TimeStamp = GetNet7TickCount();
			obj_effect.Duration = AddBuff->ExpireTime - (int) GetNet7TickCount();
			obj_effect.EffectDescID = AddBuff->EffectID;
			// ------------------

			m_BuffList[m_BuffID].RemoveEffectID = m_Player->m_Effects.AddEffect(&obj_effect);
		}
		else
		{
			m_BuffList[m_BuffID].RemoveEffectID = -1;
		}

		// Update Stats
		for(int x=0;x<5;x++)
		{
			// Add buff
			if (AddBuff->Stats[x].StatName)
			{
				m_BuffList[m_BuffID].StatID[x] = m_Player->m_Stats.SetStat(AddBuff->Stats[x].StatType, 
					AddBuff->Stats[x].StatName,	AddBuff->Stats[x].Value, AddBuff->BuffType);

				m_Player->m_Stats.UpdateAux(AddBuff->Stats[x].StatName);
			}
			else
			{
				m_BuffList[m_BuffID].StatID[x] = 0;
			}
		}

		m_Player->SendAuxShip();

		SectorManager *sm = m_Player->GetSectorManager();
		if (sm) sm->AddTimedCall(m_Player, B_REMOVE_BUFF, myBuff.BuffRemovalTime - GetNet7TickCount(), NULL, m_BuffID, 0, 0);
		
		m_BuffID++;
		return (m_BuffID-1);
	}

	return -1;
}

// Update & Remove expired buffs
void PlayerBuffs::Update(int BuffID)
{
	unsigned long TickCount = GetNet7TickCount();
	mapBuffs::iterator Buff;
	bool RemovedStat = false;


ReLoadBuffList:
	for(Buff = m_BuffList.begin(); Buff != m_BuffList.end(); ++Buff)
	{
		// Remove any stats with this buff
		if (Buff->second.BuffData.BuffRemovalTime < TickCount)
		{
			for(int x=0;x<5;x++)
			{
				if (Buff->second.StatID[x] != 0)
				{
					m_Player->m_Stats.DelStat(Buff->second.StatID[x]);
					
					RemovedStat = true;
				}
			}

			if (Buff->second.RemoveEffectID != -1)
			{
				m_Player->m_Effects.RemoveEffect(Buff->second.RemoveEffectID);
			}
			// Remove the buff
			m_BuffList.erase(Buff);
			goto ReLoadBuffList;
		}
	}

	if (RemovedStat)
	{
		m_Player->SendAuxShip();
	}
}
