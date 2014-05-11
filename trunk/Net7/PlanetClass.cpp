// PlanetClass.cpp

#include "Net7.h"
#include "ObjectClass.h"
#include "PlanetClass.h"


Planet::Planet(long object_id) : Object (object_id)
{
    m_Type = OT_PLANET;
}

Planet::~Planet()
{
    // TODO: destroy everything
}
