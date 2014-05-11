#ifndef __CSVPARSE_H__
#define __CSVPARSE_H__

#include <string>
using namespace std;

class CSVParser {

	private:
		string m_sData;
		string::size_type m_nPos;
	public:
		CSVParser();
		const CSVParser & operator << (const string &sIn);
		const CSVParser & operator << (const char *sIn);
		CSVParser & operator >> (long &nOut);
		CSVParser & operator >> (float &nOut);
		CSVParser & operator >> (string &sOut);

		string TrimLeft(const string& str);
		bool StartsWith(const string& str, const string& substr);
		bool LineIsBlank;

	private:
		void SkipSpaces(void);

};

#endif /* __CSVPARSE_H__ */
