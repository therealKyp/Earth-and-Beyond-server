// SpaceShip.h
#ifndef _SPACE_SHIP_H_INCLUDED_
#define _SPACE_SHIP_H_INCLUDED_

#include "SpaceObject.h"

class SpaceShip : public SpaceObject
{
public:
    SpaceShip();
    virtual ~SpaceShip();

public:    
	float m_MaxSpeed; // Ship's max speed.
	float m_CurrentSpeed; // Ship's current speed.
	float m_Acceleration; // Ship's acceleration.
};

#endif // _SPACE_SHIP_H_INCLUDED_
