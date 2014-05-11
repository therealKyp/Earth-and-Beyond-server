// PlanetClass.h

#ifndef _PLANET_CLASS_H_INCLUDED_
#define _PLANET_CLASS_H_INCLUDED_

#include "ObjectClass.h"

class Connection;

class Planet : public Object
{
public:
    Planet(long object_id);
    virtual ~Planet();

private:

};

#endif // _PLANET_CLASS_H_INCLUDED_