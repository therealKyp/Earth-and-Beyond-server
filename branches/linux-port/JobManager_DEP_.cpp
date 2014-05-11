#include "Net7.h"
#include "XmlTags.h"
#include "JobManager_DEP_.h"
#include "JobParser_DEP_.h"
#include <string>
#include <iostream>
#include <climits>
#include "PlayerClass.h"

class Connection;

JobManager::JobManager(void) {}
JobManager::~JobManager(void) {}

bool JobManager::Initialize(void)
{
	jobId = 0; //Initialize the jobId value

	if ( //Load up the various tables needed
		m_JobParser.LoadRoutes() &&
		m_JobParser.LoadRewards() &&
		m_JobParser.LoadDetails() &&
		m_JobParser.LoadSponsors()
		)
		return true;
	else
		return false;
}

// This needs to be re-done to use the normal connection buffer system.
// We don't want an independant comms system.

// Every time a new player uses the terminal, build a new
// job list around their level to add to the global job list
void JobManager::GetJobList(Connection *c, PacketBuffer *buffer)
{
	// for now, fake the player to have 5 levels in each category
	int PlayerLevel[3];
//	PlayerLevel[COMBAT] = c->m_Player->PlayerIndex()->RPGInfo.GetCombatLevel();
//	PlayerLevel[TRADE] = c->m_Player->PlayerIndex()->RPGInfo.GetTradeLevel();
//	PlayerLevel[EXPLORE] = c->m_Player->PlayerIndex()->RPGInfo.GetExploreLevel();

	int JobsInCategory[3];
	// for now, fake 5 jobs in each category
	JobsInCategory[COMBAT] = PLAYER_JOBS_PER_CATEGORY - 0;
	JobsInCategory[TRADE] = PLAYER_JOBS_PER_CATEGORY - 0;
	JobsInCategory[EXPLORE] = PLAYER_JOBS_PER_CATEGORY - 0;

	int jobTotal = JobsInCategory[0]+JobsInCategory[1]+JobsInCategory[2];

	// Lets build a job queue!
	buffer->addLong(jobTotal);

	for (int cat = 0; cat < 3; cat++) // Loop through all 3 categories
	{
		// For each available player job slot, generate a blank job structure
		for (int i = 0; i < JobsInCategory[cat]; i++)
		{
			// Create a new job
			Job	job;

			// Randomly generate a job level based on the player's category level
			job.JobLevel = GetRandomJobLevel(PlayerLevel[cat]);

			// Now we pick a sponsor.
			JobParser::JobSponsor *sponsor = GetRandomSponsor();
			job.Sponsor = sponsor->SponsorName;

			// Get the rewards for the missions (item, cash, faction);
			job.CashReward = job.JobLevel * CASH_MULTIPLIER;
			job.ExperienceReward = job.JobLevel * XP_MULTIPLIER;
			job.FactionReward = 0; // For now, no faction reward

			// Build the rewards string that gets displayed
			JobParser::JobReward *reward = GetRandomReward(PlayerLevel[cat]);
			job.Reward = reward->ItemName + " / " + to_string(job.CashReward) + " CR";
			job.RewardItemId = reward->ItemID;

			// Now the fun part. Get a job!
			JobParser::JobDetail *details = GetRandomJobDetails(cat);
			job.Type = details->Type;
			job.Description = details->Description;
			job.Category = details->Category;

			// Generate a "unique" id for this job
			if (jobId == LONG_MAX) jobId = 0;
			jobId = jobId++;

			job.JobDescriptionId = jobId;

			m_Jobs.push_back(job);

			// add this job to the packet
			buffer->addLong(job.JobDescriptionId);
			buffer->addLong(job.Category);
			buffer->addLong(0); //unknown bits
			buffer->addLong(job.JobLevel);
			buffer->addString(job.Type);
			buffer->addString(job.Sponsor);
			buffer->addString(job.Reward);
		}
	}
}

// This function should be more complexed to make sure a range of missions around
// the player's level are created, but for now we'll do something simple.
long JobManager::GetRandomJobLevel(long playerLevel)
{
	int level = playerLevel + rand()%5 - 2;
	if (level < 1) level = -level + 1; //flip to a positive if we're dealing with n00bs
	return level;
}

JobParser::JobReward *JobManager::GetRandomReward(long playerLevel)
{
	// First determine the level of the item using fancy magic numbers
	// (sorry too lazy to const these just now)
	double table; // itemTable is use for reward[itemTable]
	double levelCalc = (double)playerLevel/(5.6 + (double)(rand()%20)/10.0);
	double faction = modf(levelCalc, &table);
	int itemTable = (int) table;
	if (itemTable >8) itemTable = 8; //As an added check, make sure level is !> 9

	//Then we look at the level reward table and get a random item from it.
	int tableSize = m_JobParser.m_Rewards[itemTable].size();
	int randItem = rand()%tableSize;

	// Return our player's uber loot!
	return &m_JobParser.m_Rewards[itemTable][randItem];
}

// All sorts of fun stuff we can do with this method and
// the sponsors system, but that's for later
JobParser::JobSponsor *JobManager::GetRandomSponsor()
{
	int tableSize = m_JobParser.m_Sponsors.size();
	int randSponsor = rand()%tableSize;
	return &m_JobParser.m_Sponsors[randSponsor];
}

// Grab a random job from the job list based on category
JobParser::JobDetail *JobManager::GetRandomJobDetails(int category)
{
	int tableSize = m_JobParser.m_Details[category].size();
	int randJob = rand()%tableSize;
	return &m_JobParser.m_Details[category][randJob];
}

void JobManager::GetJobDescription(long JobID, PacketBuffer *buffer, TokenParser *tokenParser)
{
	TokenParser::keywords keyWords;
	TokenParser::splitString splitStr;
	string parsedString;

	vector<Job>::iterator i;
	for(i = m_Jobs.begin(); i != m_Jobs.end(); i++)
	{
		if (i->JobDescriptionId == JobID)
		{
			buffer->addLong(i->JobDescriptionId);
			buffer->addByte(0x01);
			buffer->addString(i->Type);

			tokenParser->SplitString(i->Description, splitStr, keyWords);

			//Change requested by Zackmann
			parsedString = ParseTokens(&*i, splitStr, keyWords);

			buffer->addString(parsedString);
		}
	}
}

long JobManager::GetItem(long JobID)
{
	vector<Job>::iterator i;
	for(i = m_Jobs.begin(); i != m_Jobs.end(); i++)
	{
		if (i->JobDescriptionId == JobID)
		{
			return i->RewardItemId;
		}
	}
	return 0;
}

// If statements are just for basic testing, this search needs tobe replace by lookup table
string JobManager::ParseTokens(Job *job, TokenParser::splitString &splitStr, TokenParser::keywords &keyWords)
{
	TokenParser::keywords::iterator i;

	// For speed I've tried to make each keyword start with a unique character.
	for(i = keyWords.begin(); i != keyWords.end(); i++)
	{
		if (i->key.substr(1,1) == "D") // Device
		{
			if (i->key == "{Device}") // Random device item
			{}
			else if (i->key == "{DeviceMob}") // a mob that drops tech device items.
			{}
		}
		else if (i->key.substr(1,1) == "F") // From
		{
			if (i->key == "{FromSector}") // The sector the current job station is in.
			{}
			else if (i->key == "{FromStation}") // The station where the job terminal is in
			{}
			else if (i->key == "{FromNavPoint}") // A NavPoint in the same sector as the job terminals
			{}
			else if (i->key == "{FromStationPerson}") // A person in the "FromStation"
			{}
			else if (i->key == "{FromSectorPerson}") // A person in the "FromSector"
			{}
		}
		else if (i->key.substr(1,2) == "T") //To
		{
			if (i->key == "{ToSector}") // The to sector
			{}
			else if (i->key == "{ToStation}") // A station in the to sectors
			{}
			else if (i->key == "{ToNavPoint}") // A navpoint in the tos sector
			{}
		}
		else if (i->key.substr(1,1) == "S") //Some & Spawn
		{
			if (i->key == "{SpawnItem}")
			{}
		}
	}
	return "";
}
