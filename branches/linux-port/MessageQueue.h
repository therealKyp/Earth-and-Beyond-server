// MessageQueue.h

#ifndef _MESSAGE_QUEUE_H_INCLUDED_
#define _MESSAGE_QUEUE_H_INCLUDED_

#include "Mutex.h"
#ifndef WIN32
#include "Threads/Semaphore.h"
#endif

struct MessageEntry
{
    unsigned char *message;
    int length;
    struct MessageEntry *next;
	int sequence_num;
	int data;
	long player_id;
};

//================================================================================
// MessageQueue
//================================================================================

class CircularBuffer
{
public:
    CircularBuffer(long size, long message_slots);
    virtual ~CircularBuffer();

    unsigned char * Write(unsigned char *buff, long l);
    bool Read(unsigned char *dest, unsigned char *read_ptr, long l);
    bool DeCommit(unsigned char *dest, long l);
    long ToBufferEnd();
    unsigned char * CurrentWrite()       { return m_WritePtr; }
	void SetFillCount(long count)		 { m_FillCount = count; }
    unsigned long Cumulative_Written(bool clear = false);
	void RemoveAllPlayerEntries(long player_id);

	bool RetreiveMessage( unsigned char *pMessage, int * length, long size, int sequence_num, long player_id);

	MessageEntry   *GetNextEntry();

private:
    unsigned char *	m_Buffer;

    long			m_Size;
    unsigned char *	m_WritePtr;

    long			m_FillCount;
    unsigned long	m_Cumulative_Count;

    MessageEntry   *m_EntryBuffer;
    u32       m_EntryIndex;

	DWORD m_MessageSlots;
	DWORD m_EntriesInQueue;
};

class MessageQueue
{
public:
	MessageQueue(CircularBuffer *circ_buff = (0));
	virtual ~MessageQueue();

// Public methods
public:
	void Add( char *buffer );
	void Add( unsigned char *buffer, int length, long player_id );
	void AddHead( char *buffer );
	void AddHead( unsigned char *buffer, int length );
	bool CheckQueue( char *pMessage, long size );
	bool CheckQueue( unsigned char *pMessage, int * length, long size, int sequence_num = -1 );
#ifndef WIN32
	bool BlockingCheckQueue( char *pMessage, long buffer_size );
	bool BlockingCheckQueue( unsigned char *pMessage, int *length, long buffer_size, int sequence_num = -1);
#endif
	void RemoveAllPlayerEntries(long player_id);

	bool RetreiveMessage( unsigned char *pMessage, int * length, long size, int sequence_num, long player_id);

// Private member attributes
private:
    Mutex  m_GroupMutex;
    Mutex  m_Mutex;
#ifndef WIN32
    Semaphore m_Semaphore;
#endif

    CircularBuffer *m_QueueBuffer;

	MessageEntry   *m_Queue;

	DWORD m_TotalAdded;
	DWORD m_TotalAddedToHead;
	DWORD m_TotalAddedToTail;
	DWORD m_TotalRemoved;
	DWORD m_EntriesInQueue;
};

#endif // _MESSAGE_QUEUE_H_INCLUDED_

