// ItemStorage.h
#ifndef _ITEM_STORAGE_H_INCLUDED_
#define _ITEM_STORAGE_H_INCLUDED_

#include "Item.h"

class ItemStorage
{
public:
    ItemStorage(int size);
    virtual ~ItemStorage();
    
    int GetSize();
    void SetSize(int size);

private:
    int m_Size;
    Item *m_Item;
};

#endif // _ITEM_STORAGE_H_INCLUDED_
