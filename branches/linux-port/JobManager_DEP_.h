#ifndef _JOBMANAGER_H
#define _JOBMANAGER_H

#include "PacketStructures.h"
#include "JobParser_DEP_.h"
#include "Mutex.h"
#include "Connection.h"
#include "PacketBuffer_DEP_.h"
#include "TokenParser_DEP_.h"
#include <vector>

int const PLAYER_JOBS_PER_CATEGORY = 5; // How many jobs/category to generate for a player
int const PLAYER_JOB_LEVEL_SPREAD = 2; // Random +/- job level based on player level
int const CASH_MULTIPLIER = 15; //category level * multiplier = cash reward
int const XP_MULTIPLIER = 25; //category level * multiplier = xp reward

template <class T>
inline string to_string (const T& t)
{
	stringstream ss;
	ss << t;
	return ss.str();
}

class JobManager
{
	private:

		struct Job
		{
			long    JobDescriptionId;
			long    Category;
			long	Unknown;
			long	JobLevel;
			string	Type;
			string	Sponsor;
			string	Reward;
			char	IsStillAvailable;
			string	Description;		//null termiinated
			long	RewardItemId;
			long	CashReward;
			long	ExperienceReward;
			long	FactionId;
			long	FactionReward;
		} ATTRIB_PACKED;

	public:
		JobManager(void);
		~JobManager(void);

		enum {COMBAT = 0, TRADE = 1, EXPLORE = 2};

		typedef vector<Job> Jobs;

		bool	Initialize();
		void	GetJobList(Connection *c, PacketBuffer *buffer);
		void	GetJobDescription(long JobID, PacketBuffer *buffer, TokenParser *tokenParser);
		string	ParseTokens(Job *job, TokenParser::splitString &splitStr, TokenParser::keywords &keyWords);
		long	GetItem(long JobID);

	private:
		long jobId;

		JobParser m_JobParser;
		Jobs m_Jobs;

		long GetRandomJobLevel(long playerLevel);
		JobParser::JobReward	*GetRandomReward(long playerLevel);
		JobParser::JobSponsor	*GetRandomSponsor();
		JobParser::JobDetail	*GetRandomJobDetails(int category);
};

#endif
