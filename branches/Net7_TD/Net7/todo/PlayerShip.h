// PlayerShip.h

#ifndef _PLAYER_SHIP_H_INCLUDED_
#define _PLAYER_SHIP_H_INCLUDED_

#include "SpaceShip.h"

class PlayerShip : public SpaceShip
{
public:
    PlayerShip();
    PlayerShip(int GameID, int Hull, int Wing);
    virtual ~PlayerShip();

public:
    int m_Hull; // The ship's hull type.
    int m_Wing; // The ship's wing type.

};

#endif // _PLAYER_SHIP_H_INCLUDED_
