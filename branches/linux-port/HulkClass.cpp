// StarBaseClass.cpp

#include "Net7.h"
#include "ObjectClass.h"
#include "HulkClass.h"


Hulk::Hulk(long object_id) : Object (object_id)
{
    m_Type = OT_HULK;

}

Hulk::~Hulk()
{
    // TODO: destroy everything
}
