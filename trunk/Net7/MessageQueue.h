// MessageQueue.h

#ifndef _MESSAGE_QUEUE_H_INCLUDED_
#define _MESSAGE_QUEUE_H_INCLUDED_

#include "Mutex.h"

struct MessageEntry
{
    unsigned char *message;
    int length;
	int sequence_num;
	int data;
	long player_id;
};

#define QUEUE_INDEX_SIZE 400

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
    UINT            m_EntryIndex;

	DWORD m_MessageSlots;
	DWORD m_EntriesInQueue;
};

class MessageQueue
{
public:
	MessageQueue(CircularBuffer *circ_buff = (0), long queue_slots = 0);
	virtual ~MessageQueue();

// Public methods
public:
	void Add( char *buffer );
	void Add( unsigned char *buffer, int length, long player_id );
	void AddHead( char *buffer );
	void AddHead( unsigned char *buffer, int length );
	bool CheckQueue( char *pMessage, long size );
	bool CheckQueue( unsigned char *pMessage, int * length, long size, int sequence_num = -1 );
	long CheckNextQueueSize();

	void RemoveAllPlayerEntries(long player_id);
	void ResetQueue();

	bool RetreiveMessage( unsigned char *pMessage, int * length, long size, int sequence_num, long player_id);

// Private member attributes
private:
    Mutex  m_GroupMutex;
    Mutex  m_Mutex;
    CircularBuffer *m_QueueBuffer;

	MessageEntry   **m_Queue;

	DWORD m_QueueIndexSize;
	DWORD m_ReadIndex;
	DWORD m_WriteIndex;

	DWORD m_TotalAdded;
	DWORD m_TotalAddedToHead;
	DWORD m_TotalAddedToTail;
	DWORD m_TotalRemoved;
	DWORD m_EntriesInQueue;
};

#endif // _MESSAGE_QUEUE_H_INCLUDED_

