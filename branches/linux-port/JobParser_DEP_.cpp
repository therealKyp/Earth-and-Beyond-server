/*
I could mess around with that nasty XML parser we have, or I could just use a CSV file :)

TODO: Add a jobs log file to track job happenings
*/

#include "Net7.h"
#include <iostream>
#include <fstream>
#include <ios>
#include <string>
#include "CSVParser_DEP_.h"
#include "JobParser_DEP_.h"

//#define JOB_DEBUG

using namespace std;

JobParser::JobParser()
{	int i;
	m_Routes.empty();
	for (i=0; i < 9; i++) { m_Rewards[i].empty(); }
	for (i=0; i < 3; i++) { m_Details[i].empty(); }
}
JobParser::~JobParser() {}

bool JobParser::LoadRoutes()
{
	char const filename[] = "JobRoutes.csv";

	//Switch to DB folder path (remember to switch back at the end of the function!)
	char orig_path[MAX_PATH];
    GetCurrentDirectory(sizeof(orig_path), orig_path);
    SetCurrentDirectory(SERVER_DATABASE_PATH);

	CSVParser parser;

	bool success = true;
	int lineNum = 0;

	string sLine, Comment;

	printf("Parsing %s\n", filename);
	ifstream inFile(filename);

	if (!inFile) {
		success = false;
		cerr << "Unable to open file " << filename << "\n";
	}

	while (!inFile.eof() && success)
	{
		getline(inFile, sLine); // Get a line

		// Trim left space from the line
		sLine = parser.TrimLeft(sLine);

		//cout<< sLine << "\n";

		// Skip the first line if this is row header/blank/comment
		if (parser.StartsWith(sLine, "\"<") || sLine == "" || parser.StartsWith(sLine, "\"#"))
		{
			lineNum++;
			continue;
		}

		parser << sLine; // Feed the line to the parser

		parser.LineIsBlank = true; //Assume the line is blank until proven otherwise

		JobRoute jr;

		// Now extract the columns from the line
		long int fromId = jr.FromID;
		string fromName = jr.FromName;
		long int toId = jr.ToID;
		string toName = jr.ToName;
		//parser >> jr.FromID >> jr.FromName >> jr.ToID >> jr.ToName >> Comment;
		parser >> fromId >> fromName >> toId >> toName >> Comment;

		if (Comment.length())
			cout << filename << "[" << lineNum <<"]: " << Comment << "\n";

		//skip if blank line
		if (parser.LineIsBlank)
		{
			lineNum++;
			continue;
		}

		m_Routes.push_back(jr);

		lineNum++;
	}
	inFile.close();

#ifdef JOB_DEBUG
	vector<JobRoute>::iterator i;
	for(i = m_Routes.begin(); i != m_Routes.end(); i++)
	{
		cout<<i->FromID<<", "<<i->FromName<<", "<<i->ToID<<", "<<i->ToName<<"\n";
	}
	cout << "\n";
#endif

	SetCurrentDirectory(orig_path);
	return success;
}

bool JobParser::LoadRewards()
{
	char const filename[] = "JobRewards.csv";

	//Switch to DB folder path (remember to switch back at the end of the function!)
	char orig_path[MAX_PATH];
    GetCurrentDirectory(sizeof(orig_path), orig_path);
    SetCurrentDirectory(SERVER_DATABASE_PATH);

	CSVParser parser;

	bool success = true;
	int lineNum = 0;

	string sLine, Comment;

	printf("Parsing %s\n", filename);
	ifstream inFile(filename);

	if (!inFile) {
		success = false;
		cerr << "Unable to open file " << filename << "\n";
	}

	// Do the actual parsing now
	while (!inFile.eof() && success)
	{
		getline(inFile, sLine); // Get a line

		// Trim left space from the line
		sLine = parser.TrimLeft(sLine);

		//cout<< sLine << "\n";

		// Skip the first line if this is row header/blank/comment
		if (parser.StartsWith(sLine, "\"<") || sLine == "" || parser.StartsWith(sLine, "\"#"))
		{
			lineNum++;
			continue;
		}

		parser << sLine; // Feed the line to the parser

		parser.LineIsBlank = true; //Assume the line is blank until proven otherwise

		JobReward jr;

		// Now extract the columns from the line
		long int itemId = jr.ItemID;
		string itemName = jr.ItemName;
		long int itemLevel = jr.ItemLevel;
		//parser >> itemId >> jr.ItemName >> jr.ItemLevel >> Comment;
		parser >> itemId >> itemName >> itemLevel >> Comment;

		if (Comment.length())
			cout << filename << "[" << lineNum <<"]: " << Comment << "\n";

		//skip if blank line
		if (parser.LineIsBlank)
		{
			lineNum++;
			continue;
		}

		if (jr.ItemLevel > 9)
		{
			LogMessage("Job Reward %d\'s level is too high. Setting to 9.", jr.ItemID);
			jr.ItemLevel = 9;
		}

		m_Rewards[jr.ItemLevel-1].push_back(jr);

		lineNum++;
	}
	inFile.close();

#ifdef JOB_DEBUG
	vector<JobReward>::iterator i;
	for (int level = 0; level < 9; level ++)
	{
		for(i = m_Rewards[level].begin(); i != m_Rewards[level].end(); i++)
		{
			cout<<i->ItemID<<", "<<i->ItemName<<", "<<i->ItemLevel<<"\n";
		}
	}
	cout << "\n";
#endif

	// Switching back to original path
	SetCurrentDirectory(orig_path);
	return success;
}

bool JobParser::LoadDetails()
{
	char const filename[] = "JobDetails.csv";

	//Switch to DB folder path (remember to switch back at the end of the function!)
	char orig_path[MAX_PATH];
    GetCurrentDirectory(sizeof(orig_path), orig_path);
    SetCurrentDirectory(SERVER_DATABASE_PATH);

	CSVParser parser;

	bool success = true;
	int lineNum = 0;

	string sLine, Comment;

	printf("Parsing %s\n", filename);
	ifstream inFile(filename);

	if (!inFile) {
		success = false;
		cerr << "Unable to open file " << filename << "\n";
	}

	// Do the actual parsing now
	while (!inFile.eof() && success)
	{
		getline(inFile, sLine); // Get a line

		// Trim left space from the line
		sLine = parser.TrimLeft(sLine);

		//cout<< sLine << "\n";

		// Skip the first line if this is row header/blank/comment
		if (parser.StartsWith(sLine, "\"<") || sLine == "" || parser.StartsWith(sLine, "\"#"))
		{
			lineNum++;
			continue;
		}

		parser << sLine; // Feed the line to the parser

		parser.LineIsBlank = true; //Assume the line is blank until proven otherwise

		JobDetail jd;

		// Now extract the columns from the line
		long int jobId = jd.JobID;
		long int category = jd.Category;
		string type = jd.Type;
		string description = jd.Description;
		//parser >> (jd.JobID + 0) >> (jd.Category + 0) >> (jd.Type) >> (jd.Description) >> Comment;
		parser >> jobId >> category >> type >> description >> Comment;

		if (Comment.length())
			cout << filename << "[" << lineNum <<"]: " << Comment << "\n";

		//skip if blank line
		if (parser.LineIsBlank)
		{
			lineNum++;
			continue;
		}

		if (jd.Category > 2)
		{
			LogMessage("Job %d\'s category is too high. Setting to 2.", jd.JobID);
			jd.Category = 2;
		}

		m_Details[jd.Category].push_back(jd);

		lineNum++;
	}
	inFile.close();


#ifdef JOB_DEBUG
	vector<JobDetail>::iterator i;
	for(i = m_Details.begin(); i != m_Details.end(); i++)
	{
		cout<<i->JobID<<", "<<i->Category<<", "<<i->.Type<<i->Description<<"\n";
	}
	cout << "\n";
#endif

	// Switching back to original path
	SetCurrentDirectory(orig_path);
	return success;
}

bool JobParser::LoadSponsors()
{
	char const filename[] = "JobSponsors.csv";

	//Switch to DB folder path (remember to switch back at the end of the function!)
	char orig_path[MAX_PATH];
    GetCurrentDirectory(sizeof(orig_path), orig_path);
    SetCurrentDirectory(SERVER_DATABASE_PATH);

	CSVParser parser;

	bool success = true;
	int lineNum = 0;

	string sLine, Comment;

	printf("Parsing %s\n", filename);
	ifstream inFile(filename);

	if (!inFile) {
		success = false;
		cerr << "Unable to open file " << filename << "\n";
	}

	// Do the actual parsing now
	while (!inFile.eof() && success)
	{
		getline(inFile, sLine); // Get a line

		// Trim left space from the line
		sLine = parser.TrimLeft(sLine);

		//cout<< sLine << "\n";

		// Skip the first line if this is row header/blank/comment
		if (parser.StartsWith(sLine, "\"<") || sLine == "" || parser.StartsWith(sLine, "\"#"))
		{
			lineNum++;
			continue;
		}

		parser << sLine; // Feed the line to the parser

		parser.LineIsBlank = true; //Assume the line is blank until proven otherwise

		JobSponsor js;

		// Now extract the columns from the line
		parser >> js.SponsorName >> Comment;

		if (Comment.length())
			cout << filename << "[" << lineNum <<"]: " << Comment << "\n";

		//skip if blank line
		if (parser.LineIsBlank)
		{
			lineNum++;
			continue;
		}

		m_Sponsors.push_back(js);

		lineNum++;
	}
	inFile.close();

#ifdef JOB_DEBUG
	vector<JobSponsor>::iterator i;
	for(i = m_Sponsors.begin(); i != m_Sponsors.end(); i++)
	{
		cout<<i->SponsorName<<"\n";
	}
	cout << "\n";
#endif

	// Switching back to original path
	SetCurrentDirectory(orig_path);
	return success;
}
