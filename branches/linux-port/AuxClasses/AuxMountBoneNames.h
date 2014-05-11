#ifndef _AUXMOUNTBONES_H_INCLUDED_
#define _AUXMOUNTBONES_H_INCLUDED_

#include "AuxBase.h"

struct _MountBones
{
	char MountBoneName[20][64];
} ATTRIB_PACKED;

class AuxMountBoneNames : public AuxBase
{
public:
    AuxMountBoneNames()
	{
	}

    ~AuxMountBoneNames()
	{
	}

	void Init(unsigned int MemberIndex, class AuxBase * Parent, _MountBones * DataPointer)
	{
		Construct(Flags, ExtendedFlags, 20, Parent, MemberIndex);
        Data = DataPointer;

		memset(Flags,0, sizeof(Flags));

		ExtendedFlags[0] = char(0x06);
		ExtendedFlags[1] = char(0x00);
		ExtendedFlags[2] = char(0x00);
		ExtendedFlags[3] = char(0xFF);
		ExtendedFlags[4] = char(0xFF);
		ExtendedFlags[5] = char(0x0F);

		for (int i=0;i<20;i++)
		{
			*Data->MountBoneName[i] = 0;
		}
	}

    void ClearFlags();

	void BuildPacket(unsigned char *, long &);
	void BuildExtendedPacket(unsigned char *, long &);

	_MountBones * GetData();

	char * GetMountBoneName(unsigned int);

	void SetData(_MountBones *);

	void SetMountBoneName(unsigned int, const char *);


private:
	_MountBones * Data;

	unsigned char Flags[3];
	unsigned char ExtendedFlags[6];
};

#endif
