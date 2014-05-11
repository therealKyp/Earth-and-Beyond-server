#include "Stats.h"
#include "PlayerClass.h"
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>


Stats::Stats()
{
    m_Player = 0;
	m_ValueID = 0;
}

Stats::~Stats()
{
	ResetStats();
}

void Stats::Init(Player * Owner)
{
    m_Player = Owner;

	// Zero out all data
	m_ValueID = 0;
	ResetStats();
}

/*
bool Stats::StatCallBack(StatCallBack func, char *Stat)
{
}
*/

void Stats::ResetStats()
{
	// Zero out all data
	m_StatsValues.clear();
	m_ValueLookup.clear();
}

void Stats::ResetStat(char * Stat)
{
	// Erase all data
	for(int x=0;x<5;x++)
	{
		m_StatsValues[Stat].Types[x].Buff.clear();
		m_StatsValues[Stat].Types[x].Total = 0;
	}
}

bool Stats::ChangeStat( int StatID, float NewValue )
{
	// Make sure this Stat ID is valid
	if (m_ValueLookup.count(StatID) == 0)
	{
		LogMessage("Warning! When trying to change StatID: %d Couldn't Find Stat in Lookup!\n", StatID);
		return false;
	}

	// Get the Stat Data
	char * Buff = m_ValueLookup[StatID].BuffName;
	char * Stat = m_ValueLookup[StatID].StatName;
	int Type = m_ValueLookup[StatID].Type;

	// Make sure we can find stat ID
	if (m_StatsValues[Stat].Types[Type].Buff[Buff].Values.count(StatID) == 0)
	{
		LogMessage("Warning! When trying to change StatID: %d Couldn't Find Stat!\n", StatID);
		return false;
	}

	// Change this value
	m_StatsValues[Stat].Types[Type].Buff[Buff].Values[StatID] = NewValue;

	// recalculate Stat
	CalculateValue(Stat, Type, Buff);
	return true;
}


// Trim Whitespace out of a string
char *str_trim( const char *s)
{
	char *news; /* result string */
    int n;     /* number of chars in new string, excluding trailing '\0' */
   
    while ( *s == ' ' )
   		++s;
		n = 0;
	while ( s[n] != '\0' )
		++n;
	while ( n > 0 && s[n-1] == ' ' )
   		--n;

	news = (char *) calloc( n+1, sizeof(char) );
	memcpy( news, s, n );
	return news;
}


int Stats::SetStat(int BaseType, char * Stats, float Data, char * BuffNames)
{
	char * Stat = g_StringMgr->GetStr((char *) Stats);
	char * BuffName = g_StringMgr->GetStr((char *) BuffNames);

	if (BaseType != STAT_BASE_VALUE && !strcmp("BASE_VALUE", BuffName))
	{
		LogMessage("Warning! Stats: BuffName = BASE_VALUE & BaseType != BASE\n");
	}

	// Keep a unique ID for each value
	m_ValueID++;

	if (m_StatsValues.count(Stat) == 0)
	{
		ResetStat(Stat);
	}

	// Save Data
	m_StatsValues[Stat].Types[BaseType].Buff[BuffName].Values[m_ValueID] = Data;

	// Save this to look up later for deleation
	m_ValueLookup[m_ValueID].BuffName = BuffName;
	m_ValueLookup[m_ValueID].StatName = Stat;
	m_ValueLookup[m_ValueID].Type = BaseType;

	// Calculate Stat
	CalculateValue(Stat, BaseType, BuffName);

	return m_ValueID;
}

float Stats::CalculateStat(char *Stat)
{
	float Value = 0;

	//For warp recovery, the buff makes the number smaller.
	//TO-DO: Implement all other buffs that make values smaller instead of larger
	float Multiplyer = (1.0f + (m_StatsValues[Stat].Types[STAT_BUFF_MULT].Total)) - (m_StatsValues[Stat].Types[STAT_DEBUFF_MULT].Total);
	Value = (m_StatsValues[Stat].Types[STAT_BASE_VALUE].Total * Multiplyer) + (m_StatsValues[Stat].Types[STAT_BUFF_VALUE].Total - m_StatsValues[Stat].Types[STAT_DEBUFF_VALUE].Total);
	return Value;
}




float Stats::CalculateStatType( char * Stat, int Type)
{
	float Total = 0;

	// Total all the values
	for(mapBuffStats::const_iterator SValue = m_StatsValues[Stat].Types[Type].Buff.begin(); SValue != m_StatsValues[Stat].Types[Type].Buff.end(); ++SValue)
	{
		Total += SValue->second.MaxValue;
	}

	return Total;
}

float Stats::FindMaxBuff( char * Stat, char *Buff, int Type)
{
	float Max = 0;

	for(mapStatValue::const_iterator SValue = m_StatsValues[Stat].Types[Type].Buff[Buff].Values.begin(); SValue != m_StatsValues[Stat].Types[Type].Buff[Buff].Values.end(); ++SValue)
	{
		if (Max < (float) SValue->second)
		{
			Max = (float) SValue->second;
		}
	}

	return Max;
}

void Stats::CalculateValue(char * Stat, int Type, char *Buff)
{
	// Find Max for Buff
	m_StatsValues[Stat].Types[Type].Buff[Buff].MaxValue = FindMaxBuff(Stat, Buff, Type);
	m_StatsValues[Stat].Types[Type].Total = CalculateStatType(Stat, Type);
	m_StatsValues[Stat].Total = CalculateStat(Stat);
}

bool Stats::DelStat(int StatID)
{
	// Make sure this Stat ID is valid
	if (m_ValueLookup.count(StatID) == 0)
	{
		LogMessage("Warning! When trying to change StatID: %d Couldn't Find Stat in Lookup!\n", StatID);
		return false;
	}	
	
	// Clear the Data
	char * Buff = m_ValueLookup[StatID].BuffName;
	char * Stat = m_ValueLookup[StatID].StatName;
	int Type = m_ValueLookup[StatID].Type;

	// Make sure we can find stat ID
	if (m_StatsValues[Stat].Types[Type].Buff[Buff].Values.count(StatID) == 0)
	{
		LogMessage("Warning! When trying to remove StatID: %d Couldn't Find Stat!\n", StatID);
		return false;
	}

	// Erase this value
	m_StatsValues[Stat].Types[Type].Buff[Buff].Values.erase(m_StatsValues[Stat].Types[Type].Buff[Buff].Values.find(StatID));

	// recalculate Stat
	CalculateValue(Stat, Type, Buff);

	// Update aux
	UpdateAux(Stat);

	return true;
}

// Get the stat from memory
float Stats::GetStat(char * Stats)
{
	// See if we have it in memory
	char * Stat = g_StringMgr->GetStr((char *) Stats);

	if (m_StatsValues.count(Stat) != 0)
	{
		float Total = m_StatsValues[Stat].Total;
		return Total;
	}
	else
	{
		return 0;
	}
}

// Return a specific type
float Stats::GetStatType(char *Stats, int Type)
{
	// See if we have it in memory
	char * Stat = g_StringMgr->GetStr((char *) Stats);

	// See if we have it in memory
	if (m_StatsValues.count(Stat) != 0)
	{
		return m_StatsValues[Stat].Types[Type].Total;
	}
	else
	{
		return 0;
	}
}

void Stats::UpdateAux(char *Stat)
{
	if (!strcmp(Stat, STAT_WARP))
	{
		s32 WarpSpeed = (s32)this->GetStat(Stat);
		m_Player->ShipIndex()->CurrentStats.SetWarpSpeed(WarpSpeed);
	}
	else if (!strcmp(Stat, STAT_IMPULSE))
	{
		m_Player->ShipIndex()->SetMaxSpeed(this->GetStat(STAT_IMPULSE));
		m_Player->ShipIndex()->SetMinSpeed(-this->GetStat(STAT_IMPULSE)/2);
		m_Player->ShipIndex()->SetAcceleration(this->GetStat(STAT_ACCELERATION));
	}
	else if (!strcmp(Stat, STAT_TURN))
	{
		m_Player->ShipIndex()->CurrentStats.SetTurnRate((s32)this->GetStat(Stat));
	}
	else if (!strcmp(Stat, STAT_SCAN_RANGE))
	{
		m_Player->ShipIndex()->CurrentStats.SetScanRange((s32)this->GetStat(Stat));
	}
	else if (!strcmp(Stat, STAT_CHEM_DEFLECT))
	{
		m_Player->ShipIndex()->CurrentStats.SetResistChemical((s32)this->GetStat(Stat));
	}
	else if (!strcmp(Stat, STAT_PSIONIC_DEFLECT))
	{
		m_Player->ShipIndex()->CurrentStats.SetResistPsionic((s32)this->GetStat(Stat));
	}
	else if (!strcmp(Stat, STAT_EMP_DEFLECT))
	{
		m_Player->ShipIndex()->CurrentStats.SetResistEMP((s32)this->GetStat(Stat));
	}
	else if (!strcmp(Stat, STAT_ENERGY_DEFLECT))
	{
		m_Player->ShipIndex()->CurrentStats.SetResistEnergy((s32)this->GetStat(Stat));
	}
	else if (!strcmp(Stat, STAT_EXPLOSIVE_DEFLECT)) 
	{
		m_Player->ShipIndex()->CurrentStats.SetResistExplosion((s32)this->GetStat(Stat));
	}
	else if (!strcmp(Stat, STAT_IMPACT_DEFLECT))
	{
		m_Player->ShipIndex()->CurrentStats.SetResistImpact((s32)this->GetStat(Stat));
	}
	else if (!strcmp(Stat, STAT_PLASMA_DEFLECT))
	{
		m_Player->ShipIndex()->CurrentStats.SetResistPlasma((s32)this->GetStat(Stat));
	}
	else if (!strcmp(Stat, STAT_SHIELD))
	{
		m_Player->ShipIndex()->SetMaxShield((float)this->GetStat(Stat));
	}
	else if (!strcmp(Stat, STAT_SIGNATURE))
	{
		m_Player->ShipIndex()->CurrentStats.SetVisibility((s32)this->GetStat(Stat));
	}
	else if (!strcmp(Stat, STAT_ENERGY))
	{
		m_Player->ShipIndex()->SetMaxEnergy((float)this->GetStat(Stat));
	}
	else if (!strcmp(Stat, STAT_BEAM_RANGE) || !strcmp(Stat, STAT_MISSILE_RANGE) || !strcmp(Stat, STAT_PROJECTILES_RANGE))
	{
		for (int i=0; i<20; i++)
		{
			// Update all ranges for weapons
			m_Player->m_Equip[i].UpdateRange();
		}
	}
}