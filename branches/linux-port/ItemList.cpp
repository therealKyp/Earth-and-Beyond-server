// ItemList.cpp
//
//
#include "Net7.h"
#include "ItemList.h"

ItemList::ItemList()
{
    m_ItemList = new long[INITIAL_ARRAY_SIZE];
    if (m_ArraySize)
    {
        m_ArraySize = INITIAL_ARRAY_SIZE;
    }
    else
    {
        m_ArraySize = 0;
    }
    m_NumItems = 0;
}

ItemList::~ItemList()
{
    if (m_ItemList)
    {
        delete m_ItemList;
    }
}

bool ItemList::IsItemOnList(long item)
{
    bool found = false;

    for (long i=0; i<m_NumItems; i++)
    {
        if (m_ItemList[i] == item)
        {
            found = true;
            break;
        }
    }

    return found;
}

void ItemList::AddItemToList(long item)
{
    if (m_ItemList)
    {
        // Is the array full?
        if (m_NumItems == m_ArraySize)
        {
            LogMessage("ItemList - Array of size [%d] full. Adding [%d] slots!\n",m_ArraySize,ARRAY_SIZE_INCREMENT);

            // The array is full, we need to increase the size of the list
            m_ArraySize += ARRAY_SIZE_INCREMENT;
            long * array = new long[m_ArraySize];
            memcpy(array, m_ItemList, sizeof(long) * m_NumItems);
            delete [] m_ItemList;
            m_ItemList = array;
        }

        if (m_NumItems < m_ArraySize)
        {
            // We have room in the array for another item
            m_ItemList[m_NumItems++] = item;
        }
        else
        {
            // Fatal error -- num items is greater than the array size.
        }
    }
}

long ItemList::GetNumItems()
{
    return m_NumItems;
}

long ItemList::GetItem(long index)
{
    long item = -1;
    if (m_ItemList && (index >= 0) && (index < m_NumItems))
    {
        item = m_ItemList[index];
    }
    return (item);
}

void ItemList::ClearList()
{
    for (long i=0; i<m_NumItems; i++)
    {
        m_ItemList[i] = 0;
    }

    m_NumItems = 0;
}

