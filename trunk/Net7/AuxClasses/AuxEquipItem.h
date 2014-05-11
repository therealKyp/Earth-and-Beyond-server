#ifndef _AUXEQUIPITEM_H_INCLUDED_
#define _AUXEQUIPITEM_H_INCLUDED_

#include "AuxItem.h"
#include "AuxEffect.h"

#define ITEM_STATE_NORMAL               0x00000000
#define ITEM_STATE_AUTO_FIRE            0x00000001
#define ITEM_STATE_UNUSABLE             0x00000002
#define ITEM_STATE_NO_TARGETING         0x00000004
#define ITEM_STATE_NO_AMMO              0x00000008

#define ITEM_STATE_DISABLED             0x00000080

#define ITEM_STATE_AUTO_FIRE_ENABLE     0x00002000

struct _EquipItem
{
	s32		ItemTemplateID;
	u32		StackCount;
	u64 	Price; 
	float	AveCost;
	float	Structure;
	float	Quality;
	char	InstanceInfo[64];
	char	ActivatedEffectInstanceInfo[64];
	char	EquipEffectInstanceInfo[64];
	char	BuilderName[64];
	u32		ReadyTime;
	float	TargetRange;
	u32		ItemState;
	_Effect Effect;
} ATTRIB_PACKED;

class AuxEquipItem : public AuxBase
{
public:
    AuxEquipItem()
	{
	}

    ~AuxEquipItem()
	{
	}

	void Init(unsigned int MemberIndex, class AuxBase * Parent, _EquipItem * DataPointer)
	{
		Construct(Flags, ExtendedFlags, 14, Parent, MemberIndex);
        Data = DataPointer;

		memset(Flags,0, sizeof(Flags));

		ExtendedFlags[0] = char(0x16);
		ExtendedFlags[1] = char(0x00);
		ExtendedFlags[2] = char(0xF8);
		ExtendedFlags[3] = char(0xFF);

        Data->ItemTemplateID = -2;	/* Invisible Item */
		Data->StackCount = 0;
		Data->Price = 0; 
		Data->AveCost = 0;
		Data->Structure = 0;
		Data->Quality = 0;
		*Data->InstanceInfo = 0; 
		*Data->ActivatedEffectInstanceInfo = 0; 
		*Data->EquipEffectInstanceInfo = 0;
		*Data->BuilderName = 0;
		Data->ReadyTime = 0;
		Data->TargetRange = 0;
		Data->ItemState = 0;

		Effect.Init(13, this, &Data->Effect);
	}

    void Clear();   /* Sets as invisible item slot */
    void Empty();   /* Sets as empty item slot */

    void ClearFlags();

	void BuildPacket(unsigned char *, long &);
	void BuildExtendedPacket(unsigned char *, long &);

	_EquipItem * GetData();

	_Item * GetItemData();

	s32 GetItemTemplateID();
	u32 GetStackCount();
	u64 GetPrice();
	float GetAveCost();
	float GetStructure();
	float GetQuality();
	char * GetInstanceInfo();
	char * GetActivatedEffectInstanceInfo();
	char * GetEquipEffectInstanceInfo();
	char * GetBuilderName();
	u32	GetReadyTime();
	float GetTargetRange();
	u32	GetItemState();

	void SetData(_EquipItem *);
	void SetItemData(_Item *);

	void SetItemTemplateID(s32);
	void SetStackCount(u32);
	void SetPrice(u64);
	void SetAveCost(float);
	void SetStructure(float);
	void SetQuality(float);
	void SetInstanceInfo(char *);
	void SetActivatedEffectInstanceInfo(char *);
	void SetEquipEffectInstanceInfo(char *);
	void SetBuilderName(char *);
	void SetReadyTime(u32);
	void SetTargetRange(float);
	void SetItemState(u32);

public:
	class AuxEffect Effect;

private:
	_EquipItem * Data;

	unsigned char Flags[3];
	unsigned char ExtendedFlags[4];
};

#endif
