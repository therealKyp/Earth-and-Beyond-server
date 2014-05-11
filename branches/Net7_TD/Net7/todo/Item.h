// Item.h
#ifndef _ITEM_H_INCLUDED_
#define _ITEM_H_INCLUDED_

class Item
{
public:
    Item();
    Item(int item);
    virtual ~Item();

public:
    int m_Item;
};

#endif // _ITEM_H_INCLUDED_
