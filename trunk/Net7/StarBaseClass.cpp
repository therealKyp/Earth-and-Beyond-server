// StarBaseClass.cpp

#include "Net7.h"
#include "ObjectClass.h"
#include "StarBaseClass.h"


StarBase::StarBase(long object_id) : Object (object_id)
{
    m_Type = OT_STATION;
}

StarBase::~StarBase()
{
    // TODO: destroy everything
}
