// NavTypeClass.h

#ifndef _NAVTYPE_CLASS_H_INCLUDED_
#define _NAVTYPE_CLASS_H_INCLUDED_

#include "ObjectClass.h"

class StaticMap : public Object
{
public:
    StaticMap(long object_id);
    virtual ~StaticMap();

    void SetDestination(long destination_sector)    { m_Destination = destination_sector; };
    void SetAppearsInRadar()                        { m_AppearsInRadar = true; };
    void SetHuge()                                  { m_IsHuge = 1; };
    void SetNavType(long nav_type)                  { m_NavType = nav_type; };

    long NavType()                                  { return (m_NavType); };
    char IsHuge()                                   { return (m_IsHuge); };
    long Destination()                              { return (m_Destination); };
    long GetBroadcastID();
    void SendSoundBroadcast(long rcount);

    char IsNav()                                    { return (m_NavInfo && m_NavType ? 1 : 0); };
    bool AppearsInRadar()                           { return (m_AppearsInRadar); };
    void SetEIndex(long *index_array);
    bool GetEIndex(long *index_array);
    void BlipGate(long player_id);
    void SendToVisibilityList(bool include_player);
	void OutOfRangeTrigger(Player *p, float range);
	void InRangeTrigger(Player *p, float range);

//create methods
    void SendObjectEffects(Player *player);
    void SendPosition(Player *player);
    void SendAuxDataPacket(Player *player);
    void SendNavigation(Player *player);
    void OnCreate(Player *player);
    void OnTargeted(Player *player);

    void SendObjectEffectsTCP(Player *player);
    void SendPositionTCP(Player *player);
    void SendAuxDataPacketTCP(Player *player);
    void SendNavigationTCP(Player *player);


private:
	char	m_IsHuge;
	bool	m_AppearsInRadar;
    long    m_NavType;
    bool    m_HasNavInfo;

	long    m_Destination;
    long    m_SignalType;
};

#endif // _NAVTYPE_CLASS_H_INCLUDED_