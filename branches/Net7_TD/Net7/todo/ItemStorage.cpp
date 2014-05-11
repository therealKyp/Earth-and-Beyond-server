// ItemStorage.cpp
#include "Net7.h"
#include "ItemStorage.h"

ItemStorage::ItemStorage(int size)
:   m_Size(size)
{
    m_Item = new Item[size];
}

ItemStorage::~ItemStorage()
{
    delete [] m_Item;
}

int ItemStorage::GetSize()
{
    return m_Size;
}

void ItemStorage::SetSize(int size)
{

}
