#include <iostream>
#include <cstdlib>
#include "CSVParser_DEP_.h"
using namespace std;


CSVParser::CSVParser()
{
  m_sData = "";
  m_nPos = 0;
}

const CSVParser & CSVParser::operator <<(const string & sIn)
{
  this->m_sData = sIn;
  this->m_nPos = 0;
  return *this;
}

const CSVParser & CSVParser::operator <<(const char *sIn)
{
  this->m_sData = sIn;
  this->m_nPos = 0;
  return *this;
}

CSVParser & CSVParser::operator >>(long & nOut)
{
  string sTmp = "";
  SkipSpaces();
  while (m_nPos < m_sData.length() && m_sData[m_nPos] != ',')
    sTmp += m_sData[m_nPos++];

  m_nPos++; // skip past comma
  nOut = atoi(sTmp.c_str());
  if (nOut) LineIsBlank = false;
  return *this;
}

CSVParser & CSVParser::operator >>(float & nOut)
{
  string sTmp = "";
  SkipSpaces();
  while (m_nPos < m_sData.length() && m_sData[m_nPos] != ',')
    sTmp += m_sData[m_nPos++];

  m_nPos++; // skip past comma
  nOut = atof(sTmp.c_str());
  if (nOut) LineIsBlank = false;
  return *this;
}

CSVParser & CSVParser::operator >>(string & sOut)
{
  bool hasQuotes = false;
  sOut = "";
  SkipSpaces();

  // Jump past first " if necessary
  if (m_nPos < m_sData.length() && m_sData[m_nPos] == '"') {
    hasQuotes = true;
    m_nPos++;
  }

  while (m_nPos < m_sData.length()) {
    if (!hasQuotes && m_sData[m_nPos] == ',')
      break;
    if (hasQuotes && m_sData[m_nPos] == '"') {
      if (m_nPos + 1 >= m_sData.length() - 1)
        break;
      if (m_sData[m_nPos+1] == ',')
        break;
    }
    sOut += m_sData[m_nPos++];
  }

  // Jump past last " if necessary
  if (hasQuotes && m_nPos < m_sData.length() && m_sData[m_nPos] == '"')
    m_nPos++;

  // Jump past , if necessary
  if (m_nPos < m_sData.length() && m_sData[m_nPos] == ',')
    m_nPos++;
  if (sOut.size()) LineIsBlank = false;
  return *this;
}

void CSVParser::SkipSpaces(void)
{
  while (m_nPos < m_sData.length() && m_sData[m_nPos] == ' ')
    m_nPos++;
}

string CSVParser::TrimLeft(const string& str)
{
    string t = str;
	// Trim left space from line
	string::iterator i;
	for (i = t.begin(); i != t.end(); i++)
	{
		if (!isspace(*i)) { break;}
	}
	if (i == t.end()) { t.empty(); }
	else { t.erase(t.begin(), i); }
	return t;
}

bool CSVParser::StartsWith(const string& str, const string& substr) {
    return str.find(substr) == 0;
}
