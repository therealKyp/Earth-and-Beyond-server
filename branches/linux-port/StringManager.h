// StringManager.h

#ifndef _STRING_MANAGER_H_INCLUDED_
#define _STRING_MANAGER_H_INCLUDED_

#include "Net7.h"
#include "Mutex.h"

#define STRING_TABLE_SIZE 32000
#define STRING_MEMORY_SIZE 10485760 //10MB: 1024 * 1024 * 10

#define STRING_EMPTY 0
#define STRING_BUSY 1

class StringManager
{
public:
    StringManager();
    ~StringManager();

public:
    char * GetStr(char *);
    char * GetStr(const char *);
    char * NullStr();
    void PrintOut();
    void Statistics(char * = 0);

private:
    struct StringHashElement
    {
        char * Str;
        unsigned long Busy;
    } ATTRIB_PACKED;

    struct StringHashTable
    {
        unsigned long Size;
        StringHashElement * Table;
    } ATTRIB_PACKED;;

    unsigned long HashStr(char *);
    unsigned long HashCollision(unsigned long, char *);
    char * AddStr(unsigned long, char *);

private:
    StringHashTable m_HashTable;
    char * m_Memory;
    char * m_CurLoc;
    long m_CurrentSize;
    Mutex m_Mutex;
};

#endif