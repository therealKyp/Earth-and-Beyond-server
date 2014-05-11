// ResourceClass.h

#ifndef _RESOURCE_CLASS_H_INCLUDED_
#define _RESOURCE_CLASS_H_INCLUDED_

#include "ObjectClass.h"

class ItemBase;

class Resource : public Object
{
public:
    Resource(long object_id);
    Resource();
    virtual ~Resource();

    unsigned long RespawnTick()                      { return m_Respawn_tick; };
    void SetLevel(short level);
    void SetType(short type);
    void SetLevelFromHSV(float h1);
    void SetBasset(short basset);
    void SetTypeAndName(short type);
    void SetContainerField(Object *obj)     { m_Field_Container = obj; };
    void SetRespawnTick(unsigned long respawn)       { m_Respawn_tick = respawn; };
    float ResourceRemains()                 { return (m_Resource_remains); };

    void		 AddItem(ItemBase *item, long stack);
    float		 RemoveItem(long slot_id, long stack);
    void		 ResetResource();
	void		 PopulateHusk(long mob_type);
    ContentSlot *GetContents(long index)    { return (&m_Resource_contents[index]); };
    ItemBase    *GetItem(long index);
    short        GetStack(long index);
    Object      *ContainerField()           { return (m_Field_Container); };
    void         SendObjectReset();
    void         SendObjectDrain(long slot);
    void         SendToVisibilityList(bool include_player);

    void         SetHuskName(char *name);

//create methods 
    void         SendPosition(Player *player);
    void         SendAuxDataPacket(Player *player);
    void         OnCreate(Player *player);
    void         OnTargeted(Player *player);

private:
    void         DestroyResource();

private:
	ContentSlot		m_Resource_contents[MAX_ITEMS_PER_RESOURCE]; //giving us 8 slots by default is cheaper than handling a list vector for each resource
    char            m_Content_count;
    char            m_Resource_type;
	unsigned long	m_Respawn_tick;
    short	        m_Resource_start_value;       //this value serves as an indicator to work out how much is left of the asteroid.
    short           m_Resource_value;
    Object        * m_Field_Container;
    float           m_Resource_remains;
};

typedef enum 
{
	REACTIVE_ASTEROID = 1,		// 204
	ROCKY_ASTEROID = 2,			// 201, 202, 203, 208
	HYDROCARBON = 3,			// 206, 207, 211
	CRYSTALLINE_ASTEROID = 4,	// 200, 205, 209
	CARBONACEOUS_ASTEROID = 5,	//
	DIRTY_ICE_ASTEROID = 6,		//
	GAS_CLOUD = 7,				// 210
	NICKEL_IRON = 8,			// 201, 208
	ORGANIC_HULK = 9,			//
	INORGANIC_HULK = 10			//
} ResourceType;

#define RESOURCE_TYPE_SIZE 10

#endif // _RESOURCE_CLASS_H_INCLUDED_
