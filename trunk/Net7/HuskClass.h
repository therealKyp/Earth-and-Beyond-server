// HuskClass.h

#ifndef _HUSK_CLASS_H_INCLUDED_
#define _HUSK_CLASS_H_INCLUDED_

#include "ObjectClass.h"
#include "Timenode.h"

class ItemBase;

class Husk : public Object
{
public:
    Husk(long object_id);
    Husk();
    virtual ~Husk();

    unsigned long RespawnTick()                      { return m_Respawn_tick; };
    void SetLevel(short level);
    void SetType(short type);
    void SetBasset(short basset);
    void SetRespawnTick(unsigned long respawn)       { m_Respawn_tick = respawn; };
    float ResourceRemains()                 { return (m_Resource_remains); };

    void		 AddItem(ItemBase *item, long stack);
    float		 RemoveItem(long slot_id, long stack);
	void		 PopulateHusk(long mob_type);
    ContentSlot *GetContents(long index)    { return (&m_Resource_contents[index]); };
    ItemBase    *GetItem(long index);
    short        GetStack(long index);
    void         SendObjectReset();
    void         SendObjectDrain(long slot);
    void         SendToVisibilityList(bool include_player);
	void		 SetDestroyTimer(long time_delay, long respawn_delay = -1);
	void	     DestroyHusk();
	void		 RemoveDestroyTimer();
	long		 GetCreditLoot()			{ return m_HuskCredits; }
	void		 SetCreditLoot(long creds)	{ m_HuskCredits = creds; }
	void		 SetLootTimer(long time_delay);
	u32			 GetLootTime()				{ return m_LootTime; }

    void         SetHuskName(char *name);

//create methods 
    void         SendPosition(Player *player);
    void         SendAuxDataPacket(Player *player);
    void         OnCreate(Player *player);
    void         OnTargeted(Player *player);

private:
	ContentSlot		m_Resource_contents[MAX_ITEMS_PER_RESOURCE]; //giving us 8 slots by default is cheaper than handling a list vector for each resource
    char            m_Content_count;
    char            m_Resource_type;
	unsigned long	m_Respawn_tick;
    short	        m_Resource_start_value;       //this value serves as an indicator to work out how much is left of the asteroid.
    short           m_Resource_value;
    float           m_Resource_remains;
	long			m_HuskCredits;
	TimeNode		m_ObjectTimeSlot;
	u32				m_LootTime;
};

#endif // _HUSK_CLASS_H_INCLUDED_
