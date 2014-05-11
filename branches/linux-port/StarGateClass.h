// StarGateClass.h

#ifndef _STARGATE_CLASS_H_INCLUDED_
#define _STARGATE_CLASS_H_INCLUDED_

#include "ObjectClass.h"

class Connection;

class StarGate : public Object
{
public:
    StarGate(long object_id);
    virtual ~StarGate();

private:
	long    m_Destination;
};

#endif // _STARGATE_CLASS_H_INCLUDED_