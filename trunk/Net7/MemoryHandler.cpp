// Memory.cpp
//
// Handles allocation of Time Nodes.
//

#include "Net7.h"
#include "MemoryHandler.h"
#include "SectorData.h"
#include <vector>
#include "ObjectManager.h"
#include "PlayerClass.h"
#include "ServerManager.h"

#define PLAYER_NODE_AVAILABLE -1

// Start player cleanout thread
#ifdef WIN32
void __cdecl RunCleanoutThreadAPI(void *arg)
{
    GMemoryHandler::MParam * param = (GMemoryHandler::MParam *) arg;
    GMemoryHandler * mgr = param->ClassAdd;
    delete param;
    mgr->CheckPlayerNodeThread();
    _endthread();
}
#else // Linux
void * RunCleanoutThreadAPI(void *arg)
{
    GMemoryHandler::MParam * param = (GMemoryHandler::MParam *) arg;
    GMemoryHandler * mgr = param->ClassAdd;
    delete param;
    mgr->CheckPlayerNodeThread();
    return NULL;
}
#endif

TimeNode * MemoryHandler::GetBroadcastNodeSlot()
{
	TimeNode *NextNode;// = m_BroadcastQueueBuf;
	long i = m_CircularIndex;
    long spincount = 0;
	//search for unused node
	while (m_BroadcastQueueNodes[i]->player_id != 0)
	{
		i++;
		if (i==m_NodeSize)
		{
			i = 0;
            spincount++;
            if (spincount > 2) ExtendBroadcastSlots();//need to allocate some more nodes - add another NODE_EXTEND_SIZE nodes
		}
	}

	NextNode = m_BroadcastQueueNodes[i];

	m_CircularIndex = i + 1;

	if (m_CircularIndex == m_NodeSize)
	{
		m_CircularIndex = 0;
	}

    if (NextNode->player_id != 0) 
    {
        NextNode = 0;
    }

	return NextNode;
}

void MemoryHandler::ExtendBroadcastSlots()
{
	TimeNode * BroadcastQueueBuf = new TimeNode[NODE_EXTEND_SIZE]; //allocate another 30 nodes
	memset(BroadcastQueueBuf, 0, sizeof(TimeNode)*NODE_EXTEND_SIZE);

	LogMessage(">>>>>\n");
	LogMessage(">>>>> ----  Extending broadcast nodes!\n");
	
    TimeNode *Node;
	for (int i = 0; i < NODE_EXTEND_SIZE; i++)
	{
        Node = &BroadcastQueueBuf[i];
		m_BroadcastQueueNodes.push_back(Node); //assign new nodes to vector slots on end of list
	}

	m_NodeSize += NODE_EXTEND_SIZE;
}

MemoryHandler::MemoryHandler(long nodes)
{
	TimeNode * BroadcastQueueBuf = new TimeNode[nodes];
	memset(BroadcastQueueBuf, 0, sizeof(TimeNode)*nodes);
	m_NodeSize = nodes;

	if (!BroadcastQueueBuf)
	{
		LogMessage("FATAL ERROR: Unable to initialise memory.\n");
		exit(1);
	}

	int i;
    TimeNode *Node;
	for (i = 0; i < m_NodeSize; i++)
	{
        Node = &BroadcastQueueBuf[i];
		m_BroadcastQueueNodes.push_back(Node); //assign each node to a vector slot
	}

	m_CircularIndex = 0;
}

MemoryHandler::~MemoryHandler()
{
	//unhook any active calls still on the buffer
	long i;
	for (i = 0; i < m_NodeSize; i++)
	{
		if (m_BroadcastQueueNodes[i]->player_id != 0)
		{
			m_BroadcastQueueNodes[i]->player_id = NODE_NO_LONGER_NEEDED;
		}
	}

	Sleep(100);

	TimeNode * BroadcastBuff = m_BroadcastQueueNodes[0];

	delete[] BroadcastBuff;

	i = BROADCAST_SLOTS;

	while (i > m_NodeSize)
	{
		BroadcastBuff = m_BroadcastQueueNodes[i];
		delete[] BroadcastBuff;
		i += NODE_EXTEND_SIZE;
	}

	m_BroadcastQueueNodes.clear();
}

// Global memory handler

GMemoryHandler::GMemoryHandler(long player_nodes)
{
    long node_index;

    m_PlayerQueueBuf = new Player[player_nodes];
    //NB - no memset for derived class - you'll obliterate the vptr.

	m_PlayerBufferSize = player_nodes;
    m_PlayerCount = 0;
    m_PlayerCircularIndex = 0;

	if (!m_PlayerQueueBuf)
	{
		LogMessage("FATAL ERROR: Unable to initialise memory for player handling.\n");
	}
    else
    {
        for (node_index = 0; node_index < m_PlayerBufferSize; node_index++) //set all players to 'AVAILABLE'
        {
            m_PlayerQueueBuf[node_index].SetGameID(PLAYER_NODE_AVAILABLE);
            m_PlayerQueueBuf[node_index].SetCharacterID(-1);
            m_PlayerQueueBuf[node_index].SetAccountUsername(0);
			m_PlayerQueueBuf[node_index].SetLastAccessTime(0);
        }      
    }
}

GMemoryHandler::~GMemoryHandler()
{
	delete[] m_PlayerQueueBuf;
}


long GMemoryHandler::GetPlayerCount()
{
    return (m_PlayerCount);
}

//this is no longer needed, done in PlayerManager::RunMovementThread
void GMemoryHandler::CheckPlayerNodeThread()
{
	Player *node = (0);
	long node_index;

	unsigned long current_time = GetNet7TickCount();

	m_PlayerMutex.Lock();

	for (node_index = 0; node_index < m_PlayerBufferSize; node_index++)
	{
		node = &m_PlayerQueueBuf[node_index];
		//See if the player has had any involvement with anything for the past half hour 
		//    (if the connection is invalid and we haven't handled them for 
		//     more than 2 mins then it is a dead connection - Net7Proxy will issue a keepalive every 30 secs)
		if (node->GameID() != PLAYER_NODE_AVAILABLE && node->LastAccessTime() != 0 && (node->LastAccessTime() + 60000*2) < current_time )
		{
			//release the player node.
			/*LogMessage("*** Cleanup thread is removing dead player: %s [0x%08x]\n*** last received time: %d, time now: %d diff: %d\n", node->Name(), node->GameID(),
			node->LastAccessTime(), current_time, current_time - node->LastAccessTime());*/
			g_ServerMgr->m_PlayerMgr.DropPlayerFromGalaxy(node);
			ReleasePlayerNode(node);              
		}
	}

	m_PlayerMutex.Unlock();
}


// for New 'Player' class

Player * GMemoryHandler::GetPlayerNode(char *account_name)
{
    Player *node = (0);
    Player *NodeAllocated = (0);
    long node_index; 

    if (m_PlayerCount >= MAX_ONLINE_PLAYERS)
    {
        return (0);
    }

    m_PlayerMutex.Lock();

    node_index = m_PlayerCircularIndex;
    
    //Get first available PlayerNode after the current circular index
	while (m_PlayerQueueBuf[node_index].GameID() != PLAYER_NODE_AVAILABLE)
	{
		node_index++;
		if (node_index==m_PlayerBufferSize)
		{
			node_index = 0;
		}
	}
    
    NodeAllocated = &m_PlayerQueueBuf[node_index];
    
    m_PlayerCircularIndex = node_index + 1;

	if (m_PlayerCircularIndex == m_PlayerBufferSize)
	{
		m_PlayerCircularIndex = 0;
	}
    
    NodeAllocated->SetGameID(node_index | PLAYER_TAG);
    NodeAllocated->SetGroupID(-1);
    NodeAllocated->SetMVASIndex(-1);
    NodeAllocated->SetLastAccessTime(GetNet7TickCount());

	NodeAllocated->SetGameIndex(node_index);
    
    m_PlayerCount++;

    m_PlayerMutex.Unlock();

	return NodeAllocated;
}

void GMemoryHandler::ReleasePlayerNode(Player *player)
{
    m_PlayerMutex.Lock();
    if (player->GameID() != PLAYER_NODE_AVAILABLE && m_PlayerCount > 0)
    {
		//LogMessage("Player %s [%08x] released\n", player->Name(), player->GameID());
        player->SetGameID(PLAYER_NODE_AVAILABLE);
        m_PlayerCount--;
    }

    player->SetLastAccessTime(0);
    player->SetAccountUsername(0);
    player->SetCharacterID(-1);
    m_PlayerMutex.Unlock();
}

Player * GMemoryHandler::GetPlayerA(long avatar_id, bool sector_login)
{
    Player *player = 0;

    long avatar_index = avatar_id & 0x00FFFFFF;

	if (!IS_PLAYER(avatar_id)) return (0);

    if (avatar_index > m_PlayerBufferSize) //see if this is in valid player range
    {
        //LogMessage(">> Player Not valid. id: %x index = %x\n", avatar_id, avatar_index);
        return (0);
    }

    m_PlayerMutex.Lock();
    player = &m_PlayerQueueBuf[avatar_index];
    m_PlayerMutex.Unlock();

    if (player->GameID() == PLAYER_NODE_AVAILABLE) //make sure there's something there
    {
        //LogMessage("GameID for avatarid %x = %x\n",avatar_id,player->GameID());
        if (sector_login)
        {
            LogMessage("Override invalid player ID for %s\n", player->Name());
            player->SetGameID(avatar_id);
            return player;
        }
        return (0);
    }

    //player->SetLastAccessTime(GetNet7TickCount());

    return player;
}