#ifndef _ITEM_BASE_IMPORTER_H_INCLUDED_
#define _ITEM_BASE_IMPORTER_H_INCLUDED_

#include "ItemBaseHeader.h"
#include <stdio.h>
#include <string>

typedef struct
{
    char * Str;
    int Cat;
    int SubCat;
    int Type;
} CatDat;

typedef struct
{
    char * Str;
    int Cat;
    int SubCat;
} CompDat;

class ItemBaseImporter
{
public:
    ItemBaseImporter(long MaxItems);
    ~ItemBaseImporter();

    void ImportXML(char * FileName);
    bool ConvertDase(char * Dase_DB, char * Dase_Host, char * Dase_User, char * Dase_Pass,
                     char * Net7_DB, char * Net7_Host, char * Net7_User, char * Net7_Pass, int ClearTables);

    long ItemCount()        { return m_ItemCount; }

    void Normalize();

    bool WriteFlatFile(char *);
    bool ReadFlatFile(char *);

    void ExportXML(char *);

private:
    bool ParseXML(char *);

    void ConflictCheck();

    int GetFirstID(long);
    int GetFirstName(char *);

private:
    ItemBaseStruct * m_Data;
    long m_ItemCount;
    long m_MaxItems;
};

#endif