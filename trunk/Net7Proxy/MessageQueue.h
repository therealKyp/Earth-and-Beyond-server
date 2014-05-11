// MessageQueue.h

#ifndef _MESSAGE_QUEUE_H_INCLUDED_
#define _MESSAGE_QUEUE_H_INCLUDED_

#include "Mutex.h"

class Connection;

//================================================================================
// MessageQueue
//================================================================================

class MessageQueue
{
public:
	MessageQueue();
	virtual ~MessageQueue();

// Public methods
public:
	void Add( char *buffer );
	void Add( unsigned char *buffer, int length);
	void AddHead( char *buffer );
	void AddHead( unsigned char *buffer, int length );
	bool CheckQueue( char **pMessage );
	bool CheckQueue( unsigned char **pMessage, int * length);

// Private member attributes
private:
    Mutex  m_GroupMutex;
    Mutex  m_Mutex;
	Connection *connection;

	struct MessageEntry;
	struct MessageEntry
	{
		unsigned char *message;
        int length;
		struct MessageEntry *next;
	};

	MessageEntry *m_Queue;

	DWORD m_TotalAdded;
	DWORD m_TotalAddedToHead;
	DWORD m_TotalAddedToTail;
	DWORD m_TotalRemoved;
    DWORD m_EntriesInQueue;
};

#endif // _MESSAGE_QUEUE_H_INCLUDED_

