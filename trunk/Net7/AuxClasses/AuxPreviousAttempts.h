#ifndef _AUXPREVIOUSATTEMPTS_H_INCLUDED_
#define _AUXPREVIOUSATTEMPTS_H_INCLUDED_

#include "AuxKnownFormula.h"
	
struct _PreviousAttempts
{
	_KnownFormula Attempt[16];
} ATTRIB_PACKED;

class AuxPreviousAttempts : public AuxBase
{
public:
    AuxPreviousAttempts()
	{
	}

    ~AuxPreviousAttempts()
	{
	}

	void Init(unsigned int MemberIndex, class AuxBase * Parent, _PreviousAttempts * DataPointer)
	{
		Construct(Flags, 0, 16, Parent, MemberIndex);
        Data = DataPointer;

        // We have no extended flags for this class since its NEVER sent
        // in 2-bit mode. Therefore, the flags are redundant
		memset(Flags,0, sizeof(Flags));

		for (int i=0;i<16;i++)
		{
			Attempt[i].Init(i, this, &Data->Attempt[i]);
		}
	}

    void ClearFlags();

	void BuildPacket(unsigned char *, long &, bool);

	_PreviousAttempts * GetData();

	void SetData(_PreviousAttempts *);

    void ResetAttempts(unsigned int);
    void Clear();

private:
    _PreviousAttempts * Data;

	unsigned char Flags[3];

public:
	class AuxKnownFormula Attempt[16];
};

#endif
