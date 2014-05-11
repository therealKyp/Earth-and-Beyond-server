// AssetDatabase.h

#ifndef _ASSETDATABASE_H_INCLUDED_
#define _ASSETDATABASE_H_INCLUDED_

#pragma warning(disable:4786) 

#include <map>
#include <vector>

// forward references
struct AssetData;
class sql_connection_c;

typedef std::map<unsigned long, AssetData*> AssetList;

class AssetContent
{
// Constructor/Destructor
public:
    AssetContent();
	virtual ~AssetContent();

// Public Methods
public:
    bool        LoadAssetContent();
    AssetData * GetAssetData(long basset);
    void        ReloadAssetData();
    long        GetAssetCount();

// Private Member Attributes
private:
    AssetList    m_Asset;
    long         m_highest_id;

};

struct AssetData
{
    char   *m_Name;
    char   *m_CatName;
	char   *m_SubCatName;
};


#endif // _ASSETDATABASE_H_INCLUDED_

