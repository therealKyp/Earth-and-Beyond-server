#ifndef _AUXITEM_H_INCLUDED_
#define _AUXITEM_H_INCLUDED_

#include "AuxBase.h"

struct _Item
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

    int     TradeStack;
} ATTRIB_PACKED;

class AuxItem : public AuxBase
{
public:
    AuxItem()
	{
	}

    ~AuxItem()
	{
	}

	void Init(unsigned int MemberIndex, class AuxBase * Parent, _Item * DataPointer)
	{
        Construct(Flags, ExtendedFlags, 10, Parent, MemberIndex);

        Data = DataPointer;

    	memset(Flags,0, sizeof(Flags));

		ExtendedFlags[0] = char(0x16);
		ExtendedFlags[1] = char(0x80);
		ExtendedFlags[2] = char(0xFF);

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

        Data->TradeStack = 0;
	}

    void Clear();   /* Sets as invisible item slot */
    void Empty();   /* Sets as empty item slot */
    void ClearFlags();

    void BuildPacket(unsigned char *, long &);
    void BuildExtendedPacket(unsigned char *, long &);

	_Item * GetData();

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
    int GetTradeStack();

	void SetData(_Item *);

	void SetItemTemplateID(s32);
	void SetStackCount(u32);
	void SetPrice(u64);
	void SetAveCost(float);
	void SetStructure(float);
	void SetQuality(float);
	void SetInstanceInfo(char *);
	void SetActivatedEffectInstanceInfo(char *);
	void SetEquipEffectInstanceInfo(char *);
	void SetBuilderName(const char *);
    void SetTradeStack(int);

    void AddTradeStack(int);

private:
	_Item * Data;

	unsigned char Flags[2];
	unsigned char ExtendedFlags[3];
};

#endif
