#ifndef _AUXHULKINDEX_H_INCLUDED_
#define _AUXHULKINDEX_H_INCLUDED_

#include "AuxQuadrantDamage.h"
#include "AuxDamage.h"
#include "AuxInventory20.h"
#include "AuxInventory40.h"

struct _HulkIndex
{
	char Name[64];
	char Owner[64];
	_QuadrantDamage QuadrantDamage;
	_Damage DamageSpot;
	_Damage DamageLine;
	_Damage DamageBlotch;
	_Inventory20 EquipInv;
	_Inventory40 CargoInv;
} ATTRIB_PACKED;

class AuxHulkIndex : public AuxBase
{
public:
    AuxHulkIndex()
	{
		Construct(Flags, ExtendedFlags, 8, 0, 0);
		memset(PacketBuffer, 0, sizeof(PacketBuffer));
		PacketSize = 0;

        Reset();
	}

    ~AuxHulkIndex()
	{
	}

    void Reset();
    void ClearFlags();

	bool BuildPacket(bool = false);
	bool BuildPacket(unsigned char *, long &, bool = false);

	_HulkIndex * GetData();

	u32 GetGameID();
	char * GetName();
	char * GetOwner();

	void SetData(_HulkIndex *);

	void SetGameID(u32);
	void SetName(char *);
	void SetOwner(char *);


private:
	u32 GameID;

	_HulkIndex Data;

	unsigned char Flags[2];
	unsigned char ExtendedFlags[3];

public:
	unsigned char PacketBuffer[1000];
	long PacketSize;

	class AuxQuadrantDamage QuadrantDamage;
	class AuxDamage DamageSpot;
	class AuxDamage DamageLine;
	class AuxDamage DamageBlotch;
	class AuxInventory20 EquipInv;
	class AuxInventory40 CargoInv;
};

#endif
