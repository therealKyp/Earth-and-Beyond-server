#ifndef _JOBPARSER_H
#define _JOBPARSER_H

#include "Net7.h"
#include "CSVParser_DEP_.h"
#include <vector>
#include <string>
#include <sstream>
using namespace std;

//#define DEBUG

class JobParser
{
	public:
		JobParser();
		virtual ~JobParser();

		struct JobSponsor
		{
			string	SponsorName;
			long	FactionId; // Used to give faction rewards
		} ATTRIB_PACKED;

		struct JobReward
		{
			long	ItemID;
			string	ItemName;
			long	ItemLevel;
		} ATTRIB_PACKED;

		struct JobDetail
		{
			long	JobID;
			long	Category;
			string	Type;
			string	Description;
		} ATTRIB_PACKED;

		struct JobRoute
		{
			long	FromID;
			string	FromName;
			long	ToID;
			string	ToName;

		} ATTRIB_PACKED;

		typedef vector<JobRoute>	JobRoutes;
		typedef	vector<JobReward>	JobRewards;
		typedef vector<JobDetail>	JobDetails;
		typedef vector<JobSponsor>	JobSponsors;

		// Load missions from XML
		bool LoadRoutes();
		bool LoadRewards();
		bool LoadDetails();
		bool LoadSponsors();

		JobSponsors		m_Sponsors;
		JobDetails		m_Details[3];
		JobRewards		m_Rewards[9];
		JobRoutes		m_Routes;
};

#endif
