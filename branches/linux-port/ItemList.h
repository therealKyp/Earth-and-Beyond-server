// ItemList.h

#ifndef _ITEM_LIST_H_INCLUDED_
#define _ITEM_LIST_H_INCLUDED_

#define INITIAL_ARRAY_SIZE      1024
#define ARRAY_SIZE_INCREMENT    1024

class ItemList
{
public:
    ItemList();
    virtual ~ItemList();

public:
    bool IsItemOnList(long item);
    void AddItemToList(long item);
    long GetNumItems();
    long GetItem(long index);
    void ClearList();

private:
    long m_ArraySize;
    long m_NumItems;
    long *m_ItemList;
};

#endif // _ITEM_LIST_H_INCLUDED_
