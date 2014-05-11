// FieldClass.h

#ifndef _FIELD_CLASS_H_INCLUDED_
#define _FIELD_CLASS_H_INCLUDED_

#include "SectorData.h"
#include "ObjectClass.h"

class Player;

typedef std::vector<long> ResourceIDList;
typedef std::vector<long> MOBGameIDList;
typedef std::vector<OreNode*> ItemIDList;

class Field : public Object
{
public:
    Field(long object_id);
    virtual ~Field();
    float   FieldRadius()                   { return (m_FieldRadius); };
    void    SetFieldRadius(float radius)    { m_FieldRadius = radius; };
    void    SetFieldCount(u16 count)		{ m_FieldCount = count; };
    short   FieldCount()                    { return (m_FieldCount); };
    void    SetHSV(float h1, float h2, float h3);
    void    SetFieldType(u16 type)			{ m_FieldType = type; };
	long	GetFieldType()					{ return (long)m_FieldType; };
    void    PopulateField(bool live = true, bool repopulate = false);
    Object *SetDestination(Object *current);
    void    AddMOBID(long MOBID);
	void	AddItemID(long item_id, float frequency);
	void	BlankItemIDs();
    void    AddResource(long resource);
    void    PopulateSpawn(bool live = true) { PopulateField(live); };
	void	ResetResource();
	void	SendAuxDataPacket(Player *player);
	void	SendPosition(Player *player);
	void	SendObjectEffects(Player *player);
	void	SendToVisibilityList(bool include_player);

	ItemIDList *GetAdditionalItemIDs();

private:
    void    PopulateTypes(u16 &field_spread);
    void    AddFieldGuardian(bool repopulate);

private:
	unsigned long	m_Respawn_tick;
    u16		m_Resource_value;
    float           m_FieldRadius;
    u16		m_FieldCount;
    u16		m_FieldType;
    long            m_FirstFieldID;
    MOBIDList       m_MOBIDs;
    ResourceIDList  m_ResourceIDs;
	MOBGameIDList	m_MOBGameIDs;
	ItemIDList		m_AdditionalOreItemIDs;
};

#endif // _RESOURCE_CLASS_H_INCLUDED_
