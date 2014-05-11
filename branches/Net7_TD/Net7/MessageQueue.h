// MessageQueue.h

#ifndef _MESSAGE_QUEUE_H_INCLUDED_
#define _MESSAGE_QUEUE_H_INCLUDED_

#include "Mutex.h"

class MessageQueue
{
public:
	MessageQueue();
	virtual ~MessageQueue();

public:
	void Add( char *buffer );
	void AddHead( char *buffer );
	bool CheckQueue( char **pMessage );
    void LockMessageQueue();
    void UnlockMessageQueue();

private:
    Mutex  m_GroupMutex;
    Mutex  m_Mutex;

	struct MessageEntry;
	struct MessageEntry
	{
		char *message;
		struct MessageEntry *next;
	};

	MessageEntry *m_Queue;

	u32_t m_TotalAdded;
	u32_t m_TotalAddedToHead;
	u32_t m_TotalAddedToTail;
	u32_t m_TotalRemoved;
	u32_t m_EntriesInQueue;
};

#endif // _MESSAGE_QUEUE_H_INCLUDED_
