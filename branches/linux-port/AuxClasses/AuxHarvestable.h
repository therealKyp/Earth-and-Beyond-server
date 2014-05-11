#ifndef _AUXHARVESTABLE_H_INCLUDED_
#define _AUXHARVESTABLE_H_INCLUDED_

#include "AuxInventory40.h"

struct _Harvestable
{
	char Name[64];
	_Inventory40 CargoInv;
	float PercentFull;
	u32 TechLevel;
} ATTRIB_PACKED;

class AuxHarvestable : public AuxBase
{
public:
    AuxHarvestable()
	{
		Construct(Flags, ExtendedFlags, 4, 0, 0);
		memset(PacketBuffer, 0, sizeof(PacketBuffer));
		PacketSize = 0;

        Reset();
	}

    ~AuxHarvestable()
	{
	}

    void Reset();
    void ClearFlags();

	bool BuildPacket(bool = false);
	bool BuildPacket(unsigned char *, long &, bool = false);

	bool BuildNamePacket();
	bool BuildNamePacket(unsigned char *, long &);

	_Harvestable * GetData();

	u32 GetGameID();
	char * GetName();
	float GetPercentFull();
	u32 GetTechLevel();

	void SetData(_Harvestable *);

	void SetGameID(u32);
	void SetName(char *);
	void SetPercentFull(float);
	void SetTechLevel(u32);


private:
	u32 GameID;

	_Harvestable Data;

	unsigned char Flags[1];
	unsigned char ExtendedFlags[2];

public:
	unsigned char PacketBuffer[2000];
	long PacketSize;

	class AuxInventory40 CargoInv;
};

#endif
