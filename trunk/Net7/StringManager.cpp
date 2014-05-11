#include "StringManager.h"

// Note: The memory buffer CANNOT be resized since it's memory location can change
//       Subsequently, the pointers in the hashtable will all be invalid!

StringManager::StringManager()
{
    m_CurrentSize = STRING_MEMORY_SIZE;
    m_HashTable.Size = STRING_TABLE_SIZE;
    m_HashTable.Table = new StringHashElement[STRING_TABLE_SIZE * sizeof(StringHashElement)];
    memset(m_HashTable.Table,0,STRING_TABLE_SIZE * sizeof(StringHashElement));
    m_Memory = new char[m_CurrentSize];

    // Set the very first byte to be the null string
    *m_Memory = 0;
    m_CurLoc = m_Memory + 1;

    printf("String manager initialized\n");
}

StringManager::~StringManager()
{
    if (m_Memory != 0)
    {
        delete [] m_Memory;
    }

    if (m_HashTable.Table != 0)
    {
        delete [] m_HashTable.Table;
    }
}

char * StringManager::NullStr()
{
    // The first byte of m_Memory is 0, and is therefore a null string
    return m_Memory;
}

char * StringManager::GetStr(const char *ConstStr)
{
    return GetStr((char *) ConstStr);
}

char * StringManager::GetStr(char *Str)
{
    // Not sure if the mutex needs to be placed around this entire function or just the add function

    // If the pointer is null, or the string is null, return a null string
    if (Str == 0 || *Str == 0)
    {
        return NullStr();
    }

    // Do the initial hash on the string
    unsigned long Hash = HashStr(Str);

    // Check if this location is busy
    if (m_HashTable.Table[Hash].Busy == STRING_BUSY)
    {
        // If the location is busy, check to see if its the correct string
        if (strcmp(m_HashTable.Table[Hash].Str, Str) != 0)
        {
            // If its not the correct string, run the collision function
            if ((Hash = HashCollision(Hash,Str)) == m_HashTable.Size)
            {
                // If the collision function returns the size of the hash, we cannot find/place string
                LogMessage("StringManager - Fatal error locating/placing string: %s\n",Str);
                return NullStr();
            }
            else
            {
                // The collision function found either an empty slot or the string itself
                if (m_HashTable.Table[Hash].Busy == STRING_BUSY)
                {
                    // We found the string!
                    return m_HashTable.Table[Hash].Str;
                }
                else
                {
                    // We found an empty spot to place the string
                    return AddStr(Hash, Str);
                }
            }
        }
        else
        {
            // This is the correct string
            return m_HashTable.Table[Hash].Str;
        }
    }
    else
    {
        // This location is empty, place the string here
        return AddStr(Hash, Str);
    }

    // If for some reason we have not returned, print an error
    LogMessage("StringManager - Critical Error, GetStr did not return!\n");
    return NullStr();
}

unsigned long StringManager::HashStr(char *Str)
{
    // This is the djb2 hash function
    unsigned long Char, Hash = 5381;

    while (Char = *Str++)
        Hash = ((Hash << 5) + Hash) + Char;

    return Hash % m_HashTable.Size;
}

unsigned long StringManager::HashCollision(unsigned long Hash, char *Str)
{
    // This functions returns a location if an initial collision exists
    // For now do some simple linear probing with step size 1

    for (unsigned long i=1;i<m_HashTable.Size;i++)
    {
        Hash = (Hash + 1) % m_HashTable.Size;

        // If the location is empty or the string matches
        if (m_HashTable.Table[Hash].Busy == STRING_EMPTY || strcmp(m_HashTable.Table[Hash].Str,Str) == 0)
        {
            return Hash;
        }
    }

    // No empty location nor matching string found, return table size
    return m_HashTable.Size;
}

char * StringManager::AddStr(unsigned long Hash, char *Str)
{
    m_Mutex.Lock();

    m_HashTable.Table[Hash].Str = m_CurLoc;
    m_HashTable.Table[Hash].Busy = STRING_BUSY;
    strcpy(m_CurLoc, Str);
    m_CurLoc += strlen(Str) + 1;

    // Check if were getting close to overunning
    if (m_CurLoc - m_Memory + 1024 > STRING_MEMORY_SIZE)
    {
        LogMessage("StringManager - There is less than 1Kb available in the string manager!\n");
    }

    m_Mutex.Unlock();

    return m_HashTable.Table[Hash].Str;
}

// This is for debugging only and should NEVER be used on a live server
void StringManager::PrintOut()
{
    unsigned long i,Count = 0;

    printf("\nStringManager Dump\n");

    m_Mutex.Lock();

    for (i=0;i<m_HashTable.Size;i++)
    {
        if (m_HashTable.Table[i].Busy == STRING_BUSY)
        {
            printf("Num: %d Loc: %d String: %s\n",Count++,i,m_HashTable.Table[i].Str);
        }
    }

    m_Mutex.Unlock();
}

void StringManager::Statistics(char * CopyBuffer)
{
    char buffer[256];
    unsigned long i,Used,Left,Count = 0;

    printf("\nStringManager Statistics\n");

    m_Mutex.Lock();

    for (i=0;i<m_HashTable.Size;i++)
    {
        if (m_HashTable.Table[i].Busy == STRING_BUSY)
        {
            Count++;
        }
    }

    m_Mutex.Unlock();
    
    Left = STRING_MEMORY_SIZE - (m_CurLoc - m_Memory);
    Used = m_CurLoc - m_Memory;

    sprintf(buffer, "StringManager Statistics: %d of %d hash locations used. %d bytes used. %d bytes left.",
            Count, m_HashTable.Size, Used, Left);

    if (CopyBuffer != 0)
    {
        strcpy(CopyBuffer, buffer);
    }
    else
    {
        LogMessage("%s\n",buffer);
    }

    return ;
}
