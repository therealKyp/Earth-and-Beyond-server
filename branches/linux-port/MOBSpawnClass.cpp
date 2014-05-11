// MOBSpawnClass.cpp

#include "Net7.h"
#include "ObjectClass.h"
#include "MOBSpawnClass.h"
#include "ObjectManager.h"
#include "MOBClass.h"

#if 0
    MOBSpawn(long object_id);
    virtual ~MOBSpawn();
    float   SpawnRadius()                   { return (m_SpawnRadius); };
    void    SetSpawnRadius(float radius)    { m_SpawnRadius = radius; };
    void    SetSpawnCount(short count)      { m_SpawnCount = count; };
    short   SpawnCount()                    { return (m_SpawnCount); };
    void    SetLevel(long mn, long mx)      { m_SpawnLevelMin = mn; m_SpawnLevelMax = mx; };
    void    SetBehaviour(short type)        { m_SpawnBehaviour = type; };
    void    SetGroupAttack(short attack)    { m_GroupAttack = (attack != 0) ? true : false; };
    void    PopulateSpawn(bool live = true);
    void    AddMOBID(long MOBID);

private:
    short   PopulateTypes(char *types, short &field_spread);

private:
	unsigned long	m_Respawn_tick;
    short	        m_Resource_value;
    float           m_SpawnRadius;
    short           m_SpawnCount;
    short           m_SpawnLevelMin; //TODO: remove once MOB table in place
    short           m_SpawnLevelMax; //      "           "
    short           m_SpawnBehaviour;//      "           "
    bool            m_GroupAttack;   //      "           "
    long            m_FirstMOBID;
    MOBIDList       m_MOBIDs;
#endif

MOBSpawn::MOBSpawn(long object_id) : Object (object_id)
{
    m_SpawnCount = 0;
    m_FirstMOBID = 0;
    m_SpawnRadius = 0;
}

MOBSpawn::~MOBSpawn()
{
    // TODO: destroy everything
}

void MOBSpawn::AddMOBID(long mob_id)
{
    m_MOBIDs.push_back(mob_id);
}

void MOBSpawn::PopulateSpawn(bool live)
{
    if (m_MOBIDs.size() == 0 || m_SpawnRadius == 0 || m_SpawnCount == 0)
    {
        //LogMessage("Unable to create MOB spawn: level %d, Radius: %.2f, count: %d\n", m_SpawnRadius, m_SpawnCount);
        m_SpawnCount = 0;
        return;
    }

    //create and populate
    float pos[3];
  
    for (short spawn = 0; spawn < m_SpawnCount; spawn++)
    {
        //choose MOB from list
        short MOB_index = rand()%(m_MOBIDs.size());
        short mob_type = (short)m_MOBIDs[MOB_index];
        
        MOB *mob = (MOB*)m_ObjectMgr->AddNewObject(OT_MOB); //MOB creation
        
        if (mob)
        {
            mob->SetMOBType(mob_type);
            mob->SetPosition(Position());
            //work out a position vector & angle
            float vector = (float)(rand()%200 - 99)*0.01f;
            float angle = (float)(rand()%360)/(360.0f / (2*PI));
       		pos[0] = (m_SpawnRadius * vector) * cosf(angle);
    		pos[1] = (m_SpawnRadius * vector) * sinf(angle);

            mob->MovePosition(pos[0], pos[1], ((float)((rand()%11) - 5) * 200.0f));
            mob->SetActive(true);
            mob->SetRespawnTick(0);
            mob->SetHostileTo(OT_PLAYER);
            mob->SetOrientation(0.0f, 0.0f, 0.0f, 1.0f);
            mob->SetUpdateRate(50);
            float velocity = (float)(rand()%100 + 130);
			float turn = (float)((rand()%11) - 5) * 0.01f;
            if (turn == 0.0f) turn = 0.02f;
            mob->SetVelocity(velocity);
            mob->AddBehaviourObject(this);
            mob->SetDefaultStats(turn, CURIOUS, velocity, 50);
			mob->Turn(turn);
        }
    }
}


#if 0
    m_Signature = m_FieldRadius;
    if (m_Level == 0 || m_FieldType == 0 || m_FieldCount == 0)
    {
        LogMessage("Unable to create asteroid field '%s'. level:%d,Type:%d,count:%d\n",Name(), m_Level, m_FieldType, m_FieldCount);
        m_FieldCount = 0;
        return;
    }

    //create and populate asteroid field
	float pos[3];
	float inc;
	unsigned int count;
	float angle = 0.0f;
	inc = (2*3.14159f)/m_FieldCount;
	pos[2] = m_Position_info.Position[2];

	short types_size;
	char types[16];
	short type_index;
	int this_level;
	int level_seed;
	short field_spread;
	float factor1, factor2, factor3 = 0.0f;
    Object *obj;

    types_size = PopulateTypes(&types[0], field_spread);

	for (count = 0; count < m_FieldCount; count++)
	{
		switch (field_spread)
		{
		case 1: //ring shape
			factor1 = factor2 = 1;
			break;
		case 2: //donut shape
			factor1 = factor2 = 1.0f - (float)(rand()%20)/100.0f;
			break;
		case 4: //sphere
			inc = inc * 3;
			//drop through
		case 3: //cylinder shape
			factor1 = factor2 = 1.0f - (float)(rand()%90)/100.0f;
			factor3 = (float)(rand()%70)/100.0f;
			break;

		default:
			factor1 = factor2 = 1;
			break;
		}

		//position is x=radius*cosf(angle) y=radius*sinf(angle)
		pos[0] = m_Position_info.Position[0] + (m_FieldRadius * factor1) * cosf(inc * count);
		pos[1] = m_Position_info.Position[1] + (m_FieldRadius * factor2) * sinf(inc * count);
		if (field_spread == 4)
		{
			pos[2] = m_Position_info.Position[2] + (m_FieldRadius * factor3) * sinf(inc * count);
		}
		else
		{
			pos[2] = m_Position_info.Position[2] + (rand()%2000 - 1000);
		}

		type_index = rand()%types_size;
		level_seed = rand()%20;
		this_level = m_Level;
		if (level_seed > 17)
		{
			this_level++;
		}
		else if (level_seed < 4)
		{
			this_level--;
		}

        obj = m_ObjectMgr->AddNewObject(OT_RESOURCE, true);
        obj->SetTypeAndName(types[type_index]);
        obj->SetPosition(pos);
        obj->SetLevel(this_level);
        obj->RandomiseOrientation();
        obj->SetContainerField(this);

        if (m_FirstFieldID == 0)
        {
            m_FirstFieldID = obj->GameID();
        }

        if (live)
        {
            obj->ResetResource();
        }
    }
#endif


long MOBSpawn::UpdateSpawn(u32 current_tick, bool handle_attacks)
{
    return 0;
}

#if 0
void Field::AddFieldGuardian()
{
    short mob_type = 0;

    if (m_FieldRadius < 3000.0f || m_FieldType == 5) return;

    switch (rand()%5)
    {
    case 0:
        mob_type = MOB_EnergyPhoenix;
        break;
    case 1:
        mob_type = MOB_NebulaKraken;
        break;
    case 2:
        mob_type = MOB_Crystalloid1;
        break;
    case 3:
        mob_type = MOB_Manes;
        break;
    case 4:
        mob_type = MOB_MalefariFighter;
        break;
    }

    MOB *mob = (MOB*)m_ObjectMgr->AddNewObject(OT_MOB); //MOB creation
    
    if (mob)
    {
        mob->SetMOBType(mob_type);
        mob->SetPosition(Position());
        mob->MovePosition(-200.0f, 0.0f, -200.0f);
        mob->SetLevel((m_Level) * 3 + rand()%10);
        mob->SetActive(true);
        mob->SetRespawnTick(0);
        mob->SetHostileTo(OT_PLAYER);
        mob->SetOrientation(0.0f, 0.0f, 0.0f, 1.0f);
        mob->SetUpdateRate(50);
        float velocity = (float)(rand()%100 + 130);
        float turn = (float)(rand()%3 + 2)*0.02f;
        if (rand()%10 > 7) turn = -(float)(rand()%3 + 2)*0.02f;
        if (turn == 0.0f) turn = 0.02f;
        mob->SetVelocity(velocity);
        mob->AddBehaviourObject(this);
        mob->SetBehaviour(PATROL_FIELD);
        mob->SetDefaultStats(turn, PATROL_FIELD, velocity, 50);
    }
}
#endif