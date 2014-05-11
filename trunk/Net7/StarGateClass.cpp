// StarGateClass.cpp

#include "Net7.h"
#include "ObjectClass.h"
#include "StarGateClass.h"


StarGate::StarGate(long object_id) : Object (object_id)
{
    m_Type = OT_STARGATE;
}

StarGate::~StarGate()
{
    // TODO: destroy everything
}
