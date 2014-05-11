// ItemBaseManager.h

#ifndef _ITEM_BASE_MANAGER_H_INCLUDED_
#define _ITEM_BASE_MANAGER_H_INCLUDED_

#include "ItemBase.h"
#include "AuxClasses\AuxItem.h"

#define MAX_ITEMBASE_ITEMS  10000

#define ITEM_BASE_EMPTY 0
#define ITEM_BASE_BUSY  1

class Player;
class ItemList;
class Field;

class ItemBaseManager
{
public:
    ItemBaseManager();
    virtual ~ItemBaseManager();

public:
    void Initialize();
    bool SendItem(Player * p, ItemList * List, long ItemID);
	ItemBase * GetItem(char * ItemName);
	ItemBase * GetItem(long ItemID);

	ItemBase * GetOreTemplate(short level, short type, long sector_id, Field *f);
	ItemBase * GetHulkTemplate(short level, short type);

	bool	   QualityCalculator(_Item * myItem);

private:
    bool SendItem(Player * p, long ItemID);
    void SetRefineInfo();
    void InitializeHash();
    void InitializeList();

    void AddItemBase(ItemBase *);

    unsigned long HashItemBase(ItemBase *);
    unsigned long HashItemBase(char *);

    unsigned long HashItemBaseVal(char *);

    int m_ItemCount;
	int m_MaxItemID;

public:
    _Item EmptyItem;
	_Item InvisibleItem;

private:
    struct ItemBaseHashElement
    {
        ItemBase * Item;
        unsigned long Busy;
    } ATTRIB_PACKED;

    struct ItemBaseHashTable
    {
        unsigned long Size;
        ItemBaseHashElement * Table;
    } ATTRIB_PACKED;

    ItemBase ** m_ItemDB;   // Array lookup for items
    ItemBase ** m_ItemList; // Array of all items with no gaps

    ItemBaseHashTable m_HashTable;
};

#endif // _ITEM_BASE_MANAGER_H_INCLUDED_
