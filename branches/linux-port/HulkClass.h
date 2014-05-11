// HulkClass.h

#ifndef _HULK_CLASS_H_INCLUDED_
#define _HULK_CLASS_H_INCLUDED_

#include "AuxClasses/AuxHulkIndex.h"
#include "ObjectClass.h"

class Hulk : public Object
{
public:
    Hulk(long object_id);
    virtual ~Hulk();

private:
	AuxHulkIndex	m_AuxHulk;			// aux to hulk

};

#endif // _HULK_CLASS_H_INCLUDED_
