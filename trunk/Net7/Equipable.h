#ifndef _EQUIPABLE_H_INCLUDED_
#define _EQUIPABLE_H_INCLUDED_

#include "ItemBase.h"
#include "AuxClasses\AuxEquipItem.h"
#include "TimeNode.h"

typedef enum
{
    EQUIP_SHIELD,
    EQUIP_REACTOR,
    EQUIP_ENGINE,
    EQUIP_WEAPON,
    EQUIP_DEVICE
} EquipType;

class Object;
class Player;

class Equipable
{
public:
    Equipable();
    ~Equipable();

    void Init(Player *, int);

    bool CanEquip(_Item *);
    _Item Equip(_Item *);

    void FinishInstall();
    void Hack(unsigned long);

    void PullAuxData();

    void ManualActivate();
    void AutoActivate();

	void ShootAmmo(int Target);		// Shoot ammo
	void UpdateRange();

    void CancelAutofire();

    void CoolDown();

    ItemBase * GetItemBase()            { return m_ItemBase; }
    ItemInstance * GetItemInstance()    { return &m_ItemInstance; }

	_Item * GetItem();

	float GetQuality();

private:
    void Install(unsigned long);

    void AddEffects();
    void RemoveEffects();

    void SetItemInstance();
    void SetAmmoInstance();

    void SetStats(bool Remove = false);
	void EquipEffects(int RemoveStat);

    void AddItemStateFlag(unsigned long);
    void RemoveItemStateFlag(unsigned long);

    void RemoveTimeNode();

    void Activate();
	bool Reload();
    bool UseWeapon(Object * Target);
    //Device,Shield,Reactor, Engine
	float DamageMult(float Damage);

    bool CheckRange(Object * Target);
    bool CheckOrientation(Object * Target);

	void EquipDevice(bool equip);
	bool CheckForItem(char *search, char *description);

    bool CorrectAmmo(_Item *);
    _Item EquipAmmo(_Item *);

    bool ItemReady();

private:
    EquipType m_Type;

    Player * m_Player;

    ItemBase * m_ItemBase;
	ItemBase * m_AmmoBase;
    
    ItemInstance m_ItemInstance;
    AmmoInstance m_AmmoInstance;

    TimeNode m_TimeNode;

    AuxEquipItem * m_AuxEquipItem;
    AuxItem * m_AuxAmmoItem;

	int m_StatIDs[30];		// Save Stat ID's to remove them
	int m_EEffectID;
	int m_MaxID;

    unsigned long m_ReadyTime;
    bool m_UsesAmmo;
    int m_Slot;
	float m_Range;
	int m_Target;
};

#endif