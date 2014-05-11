// Player.h
#ifndef _PLAYER_H_INCLUDED_
#define _PLAYER_H_INCLUDED_

//#include "Connection.h"
#include "PlayerShip.h"

class Player
{
public:
    Player();
    virtual ~Player();
    
    void Update();

public:
    char *m_Name; // Name of the avatar.
	int m_PlayerID; // Player ID of the the player.
    int m_Location; // Sector ID the player is currently located.
    int m_Race; // The player's race.
    int m_Class; // The player's class.
    
    //Connection *m_Connection; // Pointer to the player's connection data.    
    
	PlayerShip *m_Ship; // Pointer to the Player's ship data.
};

struct PlayerList;
struct PlayerList
{
    Player player;
    PlayerList *next;
};

#endif // _PLAYER_H_INCLUDED_
