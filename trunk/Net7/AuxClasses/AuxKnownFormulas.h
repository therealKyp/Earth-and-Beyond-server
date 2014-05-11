#ifndef _AUXKNOWNFORMULAS_H_INCLUDED_
#define _AUXKNOWNFORMULAS_H_INCLUDED_

#include "AuxKnownFormula.h"
	
struct _KnownFormulas
{
	_KnownFormula Formula[500];
} ATTRIB_PACKED;

class AuxKnownFormulas : public AuxBase
{
public:
    AuxKnownFormulas()
	{
	}

    ~AuxKnownFormulas()
	{
	}

	void Init(unsigned int MemberIndex, class AuxBase * Parent, _KnownFormulas * DataPointer)
	{
		Construct(Flags, 0, 500, Parent, MemberIndex);
        Data = DataPointer;

        // We have no extended flags for this class since its NEVER sent
        // in 2-bit mode. Therefore, the flags are redundant
		memset(Flags,0, sizeof(Flags));

		for (int i=0;i<500;i++)
		{
			Formula[i].Init(i, this, &Data->Formula[i]);
		}
	}

    void ClearFlags();

	void BuildPacket(unsigned char *, long &, bool);

	_KnownFormulas * GetData();

	void SetData(_KnownFormulas *);

    void ResetKnownFormulas(unsigned int);
    void Clear();

private:
    _KnownFormulas * Data;

	unsigned char Flags[63];

public:
	class AuxKnownFormula Formula[500];
};

#endif
