#include "Stats.h"
#include "PlayerClass.h"

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

void Stats::ResetStat(const char * Stat)
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
	const char * Buff = m_ValueLookup[StatID].BuffName;
	const char * Stat = m_ValueLookup[StatID].StatName;
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


int Stats::SetStat(int BaseType, const char * Stat, float Data, const char * BuffName)
{
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

float Stats::CalculateStat(const char *Stat)
{
	float Value = 0;

	//For warp recovery, the buff makes the number smaller.
	//TO-DO: Implement all other buffs that make values smaller instead of larger
	if (!strcmp(Stat, STAT_WARP_RECOVERY))
	{
		Value = m_StatsValues[Stat].Types[STAT_BASE_VALUE].Total * (1.0f -
			m_StatsValues[Stat].Types[STAT_BUFF_MULT].Total) -
			m_StatsValues[Stat].Types[STAT_BUFF_VALUE].Total;
	}
	else
	{
		Value = m_StatsValues[Stat].Types[STAT_BASE_VALUE].Total * (1.0f +
			m_StatsValues[Stat].Types[STAT_BUFF_MULT].Total) +
			m_StatsValues[Stat].Types[STAT_BUFF_VALUE].Total;
	}

	return Value;
}




float Stats::CalculateStatType( const char * Stat, int Type)
{
	float Total = 0;

	// Total all the values
	for(mapBuffStats::const_iterator SValue = m_StatsValues[Stat].Types[Type].Buff.begin(); SValue != m_StatsValues[Stat].Types[Type].Buff.end(); ++SValue)
	{
		Total += SValue->second.MaxValue;
	}

	return Total;
}

float Stats::FindMaxBuff(const char * Stat, const char *Buff, int Type)
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

void Stats::CalculateValue(const char * Stat, int Type, const char *Buff)
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
	const char * Buff = m_ValueLookup[StatID].BuffName;
	const char * Stat = m_ValueLookup[StatID].StatName;
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

	return true;
}

// Get the stat from memory
float Stats::GetStat(const char * Stat)
{
	// See if we have it in memory
	if (m_StatsValues.count(Stat) != 0)
	{
		return m_StatsValues[Stat].Total;
	}
	else
	{
		return 0;
	}
}

// Return a specific type
float Stats::GetStatType(const char *Stat, int Type)
{
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
