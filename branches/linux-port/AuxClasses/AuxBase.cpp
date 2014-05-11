#include "AuxBase.h"

AuxBase::AuxBase()
{
	m_Flags = 0;
	m_ExtendedFlags = 0;
	m_FlagCount = 0;
	m_Parent = 0;
	m_MemberIndex = 0;
    m_Mutex = 0;
}

AuxBase::~AuxBase()
{
}

void AuxBase::Construct(unsigned char *Flags, unsigned char *ExtendedFlags, unsigned int FlagCount,
                class AuxBase *Parent, unsigned int MemberIndex)
{
	m_Flags = Flags;
	m_ExtendedFlags = ExtendedFlags;
	m_FlagCount = FlagCount;
	m_Parent = Parent;
	m_MemberIndex = MemberIndex;

    if (m_Parent)
    {
        m_Mutex = m_Parent->m_Mutex;
    }
}

/******************************
*         ADD METHODS         *
******************************/

void AuxBase::AddString(unsigned char *buffer, char *str, long &index)
{
	AddData(buffer,short(strlen(str)),index);
	memcpy(&buffer[index], str, strlen(str));
	index += strlen(str);
}

void AuxBase::AddFlags(unsigned char *flags, unsigned int size, unsigned char *buffer, long &index)
{
	for (unsigned int i=0;i<size;i++)
	{
		AddData(buffer,flags[i],index);
	}
}

/******************************
*       REPLACE METHODS       *
******************************/

void AuxBase::ReplaceString(char *orig, const char *src, unsigned int flagNum)
{
    m_Mutex->Lock();

	if (strcmp(orig,src))
	{
		/* The string is different, set the flags */
		SetAuxBit(m_Flags, flagNum);

        /* This eliminates useless recursion */
        if ((m_Flags[0] & 0x02) == 0)
        {
			m_Flags[0] |= 0x02;
			SetParentFlag();
        }

		/* Change the extended flags for this bit if needed */
		if (!strlen(orig) && strlen(src))
		{
			UnsetAuxBit(m_ExtendedFlags, flagNum + m_FlagCount);
			SetAuxBit(m_ExtendedFlags, flagNum);
		}
		else if (!strlen(src) && strlen(orig))
		{
			SetAuxBit(m_ExtendedFlags, flagNum + m_FlagCount);
			UnsetAuxBit(m_ExtendedFlags, flagNum);
		}

		/* Copy the string */
		strcpy(orig,src);
	}

    m_Mutex->Unlock();
}

void AuxBase::ReplaceAvail(u32 *orig, u32 *src, unsigned int flagNum)
{
    m_Mutex->Lock();

	if (memcmp(orig,src,sizeof(u32)*4))
	{
		/* The ints are differnt */
		SetAuxBit(m_Flags, flagNum);

        /* This eliminates useless recursion */
        if ((m_Flags[0] & 0x02) == 0)
        {
			m_Flags[0] |= 0x02;
			SetParentFlag();
        }

		/* Since the default is non-empty, availability cannot be empty */

		/* Copy the ints */
		memcpy(orig,src,sizeof(u32)*4);
	}

    m_Mutex->Unlock();
}

void AuxBase::ReplaceColor(float *orig, float *src, unsigned int flagNum)
{
	if (memcmp(orig,src,sizeof(float)*3))
	{
		/* The floats are differnt */
		SetAuxBit(m_Flags, flagNum);

        /* This eliminates useless recursion */
        if ((m_Flags[0] & 0x02) == 0)
        {
			m_Flags[0] |= 0x02;
			SetParentFlag();
        }

		/* Change the extended flags for this bit if needed */
		if (src[0] || src[1] || src[2])
		{
			UnsetAuxBit(m_ExtendedFlags, flagNum + m_FlagCount);
			SetAuxBit(m_ExtendedFlags, flagNum);
		}
		else
		{
			SetAuxBit(m_ExtendedFlags, flagNum + m_FlagCount);
			UnsetAuxBit(m_ExtendedFlags, flagNum);
		}

		/* Copy the floats */
		memcpy(orig,src,sizeof(float)*3);
	}
}

/******************************
*      FLAG CHECK METHODS     *
******************************/

unsigned int AuxBase::CheckFlagBit(unsigned int flagNum)
{
	return (CheckAuxBit(m_Flags, flagNum));
}

unsigned int AuxBase::CheckExtendedFlagBit1(unsigned int flagNum)
{
	return (CheckAuxBit(m_ExtendedFlags, flagNum));
}

unsigned int AuxBase::CheckExtendedFlagBit2(unsigned int flagNum)
{
	return (CheckAuxBit(m_ExtendedFlags, flagNum + m_FlagCount));
}

unsigned int AuxBase::CheckAuxBit(unsigned char *flagBuffer, unsigned int flagNum)
{
	return (flagBuffer[(flagNum + 4) / 8] & (1 << ((flagNum + 4) % 8)));
}

/******************************
*  FLAG MANIPULATION METHODS  *
******************************/

void AuxBase::SetAuxBit(unsigned char *flagBuffer, unsigned int flagNum)
{
	flagBuffer[(flagNum + 4) / 8] |= (1 << ((flagNum + 4) % 8));
}

void AuxBase::UnsetAuxBit(unsigned char *flagBuffer, unsigned int flagNum)
{
	flagBuffer[(flagNum + 4) / 8] &= ~(1 << ((flagNum + 4) % 8));
}

/******************************
*   RECURSIVE START METHODS   *
******************************/

void AuxBase::SetParentFlag()
{
	if (m_Parent)
	{
		m_Parent->ChildFlagChanged(m_MemberIndex);
	}
}

void AuxBase::SetParentExtendedFlag(unsigned int Set)
{
	if (m_Parent)
	{
		m_Parent->ChildExtendedFlagChanged(m_MemberIndex, Set);
	}
}

/******************************
*      RECURSIVE METHODS      *
******************************/

void AuxBase::ChildFlagChanged(unsigned int ChildIndex)
{
	SetAuxBit(m_Flags, ChildIndex); /* Set the flag associated with the index */

    if ((m_Flags[0] & 0x02) == 0)
    {
	    m_Flags[0] |= 0x02;         /* Set the main flag */
        SetParentFlag();	        /* Continue upward */
    }
}

void AuxBase::ChildExtendedFlagChanged(unsigned int ChildIndex, unsigned int Set)
{
	if (Set)
	{
		SetAuxBit(m_ExtendedFlags, ChildIndex);
	}
	else
	{
		UnsetAuxBit(m_ExtendedFlags, ChildIndex);
	}

    CheckData();    /* Continue recursion on a case-by-case basis */
}
