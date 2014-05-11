/* File:	   : TokenParser.h
 * Author      : Zapgun
 * Version     : 1.0
 * Copyright   : copyright 2007
 * Description : A token parser class to parse token strings and replace keywords with meaningful data
 */

#include "TokenParser_DEP_.h"

TokenParser::TokenParser() {}
TokenParser::~TokenParser() {}

bool TokenParser::SplitString(const string &str,
		splitString &splitStr,
		keywords &keyWords)
{
	string open_delim = "{";
	string close_delim = "}";

	vector<string::size_type> opens, closes, split;
	string::size_type pos, openPos, closePos;

	pos = 0;
	//Find all instances of the open_delim and save to a vector
	string::size_type lastOpen = str.find_last_of(open_delim);
	while (pos <= lastOpen)
	{
		openPos = str.find_first_of(open_delim, pos);
		opens.push_back(openPos);
		pos = openPos+1;
	}

	pos = 0;
	//Find all instances of the close_delim and save to a vector
	string::size_type lastClose = str.find_last_of(close_delim);
	while (pos <= lastClose)
	{
		closePos = str.find_first_of(close_delim, pos) + 1;
		closes.push_back(closePos);
		pos = closePos;
	}

	pos = 0;
	int splitIndex = 0;
	// Now that we know all the token positions, split string nd extract tokens
	for (unsigned int i = 0; i < opens.size(); i ++)
	{
		//Check for nesting delimiters
		if (opens[i+1] < closes[i])
		{
			cout << "Parse error. Nested tokens." << endl;
			return false;
		}

		// Check for empty tokens
		string subStr = str.substr(opens[i]+1, closes[i]-1 - opens[i] -1);
		if (subStr.find_last_not_of(' ') == string::npos)
		{
			cout << "Parse error. Empty token." << endl;
			return false;
		}

		// Check to make sure that open comes before close
		if (closes[i] <= opens[i])
		{
			cout << "Token parsing error. Delimiters are out of order." << endl;
			return false;
		}

		if (pos != opens[i])
		{
			splitStr.push_back(str.substr(pos,opens[i] - pos));
			splitIndex += 1;
		}
		string key = str.substr(opens[i], closes[i] - opens[i]);
		string value = "";
		string::size_type loc = key.find( ".", 0 );
		if (loc != string::npos)
		{
			value = key.substr(loc+1, key.length() - loc - 2);
			key = key.substr(0, loc) + "}";
		}
		splitIndex += 1;
		keyword keyWord;
		keyWord.key = key;
		keyWord.value = value;
		keyWord.splitIndex = splitIndex;

		keyWords.push_back(keyWord);
		splitStr.push_back(str.substr(opens[i], closes[i] - opens[i]));

		pos = closes[i];
	}
    return true;
}
