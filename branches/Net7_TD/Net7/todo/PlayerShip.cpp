// PlayerShip.cpp

#include "Net7.h"
#include "PlayerShip.h"

PlayerShip::PlayerShip()
{
    m_GameID = 0;
    m_Hull = -1;
    m_Wing = -1;
}

PlayerShip::PlayerShip(int gameID, int hull, int wing)
:   m_Hull(hull),
    m_Wing(wing)
{
    m_GameID = gameID;
}

PlayerShip::~PlayerShip()
{

}
