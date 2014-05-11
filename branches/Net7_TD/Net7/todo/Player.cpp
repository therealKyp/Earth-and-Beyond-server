// Player.cpp
#include "Net7.h"
#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
    //if(m_Connection)
        //delete m_Connection;
    
    if(m_Ship)
        delete m_Ship;
}

/* Unknown if needed...
   This function would be used to send the client
   updates on anything that has changed. */
void Player::Update()
{

}
