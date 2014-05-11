// HulkClass.h

#ifndef _HULK_CLASS_H_INCLUDED_
#define _HULK_CLASS_H_INCLUDED_

#include "AuxHulk.h"
#include "ObjectClass.h"

class Hulk : public Object
{
public:
    Hulk(long object_id);
    virtual ~Hulk();

private:
	AuxHulk	m_AuxHulk;			// aux to hulk

};

#endif // _HULK_CLASS_H_INCLUDED_