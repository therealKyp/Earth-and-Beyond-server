// StarBaseClass.h

#ifndef _STARBASE_CLASS_H_INCLUDED_
#define _STARBASE_CLASS_H_INCLUDED_

#include "ObjectClass.h"

class Connection;

class StarBase : public Object
{
public:
    StarBase(long object_id);
    virtual ~StarBase();

private:
	long    m_Destination;
};

#endif // _STARBASE_CLASS_H_INCLUDED_