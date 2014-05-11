// Timenode.h

#ifndef _TIMENODE_H_INCLUDED_
#define _TIMENODE_H_INCLUDED_

#include "PlayerManager.h"

class Object;

typedef enum
{
	B_WARP_BROADCAST, 
    B_DESTROY_RESOURCE, B_WARP_RESET, B_WARP_TERMINATE, B_MINE_RESOURCE, B_COLLECT_RESOURCE,
    B_MOB_DAMAGE, B_RECHARGE_REACTOR, B_FORCE_LOGOUT, 
    B_BUFF_TIMEOUT, B_CAMERA_CONTROL, B_MANUFACTURE_ACTION,
    B_SHIP_DAMAGE, B_ITEM_INSTALL, B_RECHARGE_SHIELD, B_ITEM_COOLDOWN, B_TEST_MESSAGE, B_ABILITY_REMOVE,
	B_SHOOT_AMMO, B_PLAYER_BUFFS, B_SERVER_SHUTDOWN
} broadcast_function;

#define SECTOR_SERVER_CONNECTIONLESS_NODE -1
#define NODE_NO_LONGER_NEEDED -2

struct TimeNode
{
	//function type
	broadcast_function func;
	
	unsigned long event_time;
	//params
	Object *obj;
	long i1;
	long i2;
	long i3;
	long i4;
	float a;
	char *ch;
	
    long player_id;
    short EventIndex;
} ATTRIB_PACKED;

#endif