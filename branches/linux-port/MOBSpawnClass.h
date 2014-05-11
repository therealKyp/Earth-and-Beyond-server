// MOBSpawnClass.h

#ifndef _MOBSPAWN_CLASS_H_INCLUDED_
#define _MOBSPAWN_CLASS_H_INCLUDED_

#include "SectorData.h"
#include "ObjectClass.h"
#include <vector>

class MOBSpawn : public Object
{
public:
    MOBSpawn(long object_id);
    virtual ~MOBSpawn();
    float   SpawnRadius()                   { return (m_SpawnRadius); };
    void    SetSpawnRadius(float radius)    { m_SpawnRadius = radius; };
    void    SetSpawnCount(short count)      { m_SpawnCount = count; };
    short   SpawnCount()                    { return (m_SpawnCount); };
    void    SetBehaviour(short type)        { m_SpawnBehaviour = type; };
    void    SetGroupAttack(short attack)    { m_GroupAttack = (attack != 0) ? true : false; };
    void    PopulateSpawn(bool live = true);
    void    AddMOBID(long MOBID);

    long    UpdateSpawn(u32 current_tick, bool handle_attacks);

private:
	unsigned long	m_Respawn_tick;
    float           m_SpawnRadius;
    short           m_SpawnCount;
    short           m_SpawnBehaviour;//      Remove once AI scripts are working
    bool            m_GroupAttack;   //      "           "
    long            m_FirstMOBID;
    MOBIDList       m_MOBIDs;
};

#endif // _MOBSPAWN_CLASS_H_INCLUDED_
