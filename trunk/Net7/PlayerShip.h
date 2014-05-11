// PlayerShip.h

#ifndef _PLAYER_SHIP_H_INCLUDED_
#define _PLAYER_SHIP_H_INCLUDED_

class PlayerShip
{
public:
    PlayerShip();
    virtual ~PlayerShip();

public:
	float	m_Position[3];
	float	m_Orientation[3];
	float	m_Speed[3];
	long	m_AvatarID;
};

#endif // _PLAYER_SHIP_H_INCLUDED_
