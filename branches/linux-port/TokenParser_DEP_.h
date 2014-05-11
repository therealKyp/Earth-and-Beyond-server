/* File:	   : TokenParser.h
 * Author      : Zapgun	
 * Version     : 1.0
 * Copyright   : copyright 2007
 * Description : A token parser class to parse token strings and replace keywords with meaningful data.
 */
#pragma warning (disable:4786)

#ifndef TOKENPARSER_H_
#define TOKENPARSER_H_

#include <vector>
#include <string>
#include <iostream>
#include "Net7.h"
using namespace std;

class TokenParser
{
private:
	
	public:
		TokenParser();
		virtual ~TokenParser();

		struct keyword {
			string key;
			string value;
			unsigned int splitIndex;
		};

		typedef vector<keyword> keywords;
		typedef vector<string> splitString;
		
		bool SplitString(const string &str, splitString &splitStr, keywords &keyWords);

};

#endif /*TOKENPARSER_H_*/
