// MessageQueue.cpp

//////////////////////////////////////////////////////////////////////
// Include Files
//////////////////////////////////////////////////////////////////////

#include "Net7.h"
#include "MessageQueue.h"

////////////////////////////////////////////////////////////////////////////////
// Class : MessageQueue
////////////////////////////////////////////////////////////////////////////////

MessageQueue::MessageQueue(CircularBuffer *circ_buff)
{
	m_Queue = NULL;
	m_TotalAdded = 0;
	m_TotalAddedToHead = 0;
	m_TotalAddedToTail = 0;
	m_TotalRemoved = 0;
	m_EntriesInQueue = 0;

	if (circ_buff)
	{
		m_QueueBuffer = circ_buff;
	}
	else
	{
		m_QueueBuffer = new CircularBuffer(16384, 128);
	}
}

MessageQueue::~MessageQueue()
{
	//char *message;
    delete m_QueueBuffer;

	/*while (CheckQueue( message ))
	{
		delete [] message;
	}*/
}

void MessageQueue::Add( char *buffer )
{
    Add((unsigned char *) buffer, strlen(buffer) + 1, 0);
}

void MessageQueue::Add( unsigned char *buffer, int length, long player_id)
{
	MessageEntry *new_entry = m_QueueBuffer->GetNextEntry();
	new_entry->message = m_QueueBuffer->Write(buffer, length);
    new_entry->length = length;
	new_entry->next = 0;
	new_entry->sequence_num = -1;
	new_entry->data = *((int *) buffer);
	new_entry->player_id = player_id;

    // Critical Section
	m_Mutex.Lock();

	u32 queue_count = 0;

	// Is the Queue empty?
	if (m_Queue)
	{
		// No, Find the end of the linked list
		MessageEntry *entry = m_Queue;
		while (entry->next)
		{
			entry = entry->next;
			if (queue_count++ > 1000) break;
		}

		// Add this entry to the end of the linked list
		entry->next = new_entry;
	}
	else
	{
		// The linked list is empty, so this is the first entry
		m_Queue = new_entry;
	}

	m_TotalAdded++;
	m_TotalAddedToTail++;
	m_EntriesInQueue++;
#ifndef WIN32
	m_Semaphore.Post();
#endif
    m_Mutex.Unlock();
}

void MessageQueue::AddHead( char *buffer )
{
	MessageEntry *new_entry = m_QueueBuffer->GetNextEntry();
	new_entry->message = m_QueueBuffer->Write((unsigned char*)buffer, strlen(buffer) + 1);
    new_entry->length = strlen(buffer) + 1;

    // Critical Section
    m_Mutex.Lock();

	new_entry->next = m_Queue;

	m_TotalAdded++;
	m_TotalAddedToHead++;
	m_EntriesInQueue++;
#ifndef WIN32
	m_Semaphore.Post();
#endif

    m_Mutex.Unlock();
}

void MessageQueue::AddHead( unsigned char *buffer, int length )
{
	MessageEntry *new_entry = m_QueueBuffer->GetNextEntry();
	new_entry->message = m_QueueBuffer->Write(buffer, length);
    new_entry->length = length;

    // Critical Section
    m_Mutex.Lock();

	new_entry->next = m_Queue;

	m_TotalAdded++;
	m_TotalAddedToHead++;
	m_EntriesInQueue++;
#ifndef WIN32
	m_Semaphore.Post();
#endif

    m_Mutex.Unlock();
}

// returns true if message was removed from the Queue
bool MessageQueue::CheckQueue( char *pMessage, long buffer_size )
{
    int length;
    return CheckQueue( (unsigned char *) pMessage, &length, buffer_size );
}

// returns true if message was removed from the Queue
bool MessageQueue::CheckQueue( unsigned char *pMessage, int *length, long buffer_size, int sequence_num)
{
	bool success = false;
    MessageEntry *this_entry;

	if (m_Queue)
	{
        // Critical section
        m_Mutex.Lock();

        // Pull from the head of the queue
        this_entry = m_Queue;

        m_Queue = this_entry->next;
        *length = this_entry->length;
		this_entry->sequence_num = sequence_num;
        m_TotalRemoved++;
		m_EntriesInQueue--;
		if (m_EntriesInQueue < 0) m_EntriesInQueue = 0;
        m_Mutex.Unlock();

        if (this_entry->length > buffer_size)
        {
            LogMessage("Error, buffer size for Queue not large enough: is %d, message size is %d\n", buffer_size, this_entry->length);
        }
        else
        {
            if (m_QueueBuffer->Read(pMessage, this_entry->message, this_entry->length))
			{
				success = true;
			}
            m_QueueBuffer->DeCommit(this_entry->message, this_entry->length);
        }
	}

	return (success);
}

#ifndef WIN32
// returns true if message was removed from the Queue
// call to this function will block until there actually is an item to remove from the queue
bool MessageQueue::BlockingCheckQueue( char *pMessage, long buffer_size )
{
    int length;
    return BlockingCheckQueue( (unsigned char *) pMessage, &length, buffer_size );
}

// returns true if message was removed from the Queue
// call to this function will block until there actually is an item to remove from the queue
bool MessageQueue::BlockingCheckQueue( unsigned char *pMessage, int *length, long buffer_size, int sequence_num)
{
	bool success = false;
    MessageEntry *this_entry;
    m_Semaphore.Wait();
	if (m_Queue)
	{
        // Critical section
        m_Mutex.Lock();

        // Pull from the head of the queue
        this_entry = m_Queue;

        m_Queue = this_entry->next;
        *length = this_entry->length;
		this_entry->sequence_num = sequence_num;
        m_TotalRemoved++;
		m_EntriesInQueue--;
		if (m_EntriesInQueue < 0) m_EntriesInQueue = 0;
        m_Mutex.Unlock();

        if (this_entry->length > buffer_size)
        {
            LogMessage("Error, buffer size for Queue not large enough: is %d, message size is %d\n", buffer_size, this_entry->length);
        }
        else
        {
            if (m_QueueBuffer->Read(pMessage, this_entry->message, this_entry->length))
			{
				success = true;
			}
            m_QueueBuffer->DeCommit(this_entry->message, this_entry->length);
        }
	}

	return (success);
}
#endif

bool MessageQueue::RetreiveMessage( unsigned char *pMessage, int *length, long buffer_size, int sequence_num, long player_id)
{
	return m_QueueBuffer->RetreiveMessage( pMessage, length, buffer_size, sequence_num, player_id );
}

void MessageQueue::RemoveAllPlayerEntries(long player_id)
{
	m_QueueBuffer->RemoveAllPlayerEntries(player_id);
}


/*********************************************************
 *
 * Circular Buffer Methods
 *
 *********************************************************/



CircularBuffer::CircularBuffer(long size, long message_slots)
{
    m_Buffer = new unsigned char[size];

	m_MessageSlots = message_slots;

	m_EntryBuffer = new MessageEntry[m_MessageSlots];

    if (m_Buffer && m_EntryBuffer)
    {
        m_Size      = size;
        m_WritePtr  = m_Buffer;
        m_FillCount = 0;
		m_EntriesInQueue = 0;
		m_EntryIndex = 0;
		memset (m_EntryBuffer, 0, sizeof(MessageEntry)*m_MessageSlots);
	}
	else
	{
        LogMessage("Unable to allocate %d bytes for Circular Buffer\n",size);
		m_Size = 0;
		m_WritePtr  = 0;
    }
}

CircularBuffer::~CircularBuffer()
{
    delete[] m_Buffer;
	delete[] m_EntryBuffer;
    m_Size = 0;
}

MessageEntry *CircularBuffer::GetNextEntry()
{
    MessageEntry *this_entry;
    if (m_EntriesInQueue >= m_MessageSlots)
    {
		LogMessage("Warning: message slots full, we may be smacking unsued messages\n");//hopefully this never happens, but it shouldn't matter
																						//the queue is big enough that any message smacked is only
																						//there due to a player abort
		m_EntriesInQueue = 0;
    }

    //this method only gets called from one place, so no need for mutex
    this_entry = &m_EntryBuffer[m_EntryIndex];

    ++m_EntryIndex;
	++m_EntriesInQueue;

	//By the circular nature of the way messages are checked
	//we should never smack into an unread message of another player
    if (m_EntryIndex >= m_MessageSlots)
    {
        m_EntryIndex = 0;
    }

    return this_entry;
}

bool CircularBuffer::RetreiveMessage( unsigned char *pMessage, int *length, long buffer_size, int sequence_num, long player_id)
{
	//search through all messages for specific entry
	for (u32 i = 0; i < m_MessageSlots; i++)
	{
		//do any sequence nums match?
		if (m_EntryBuffer[i].sequence_num == sequence_num && m_EntryBuffer[i].player_id == player_id)
		{
			Read(pMessage, m_EntryBuffer[i].message, m_EntryBuffer[i].length);
			int buffdata = *((int *) pMessage);
			if (m_EntryBuffer[i].data == buffdata) //data still preserved?
			{
				*length = m_EntryBuffer[i].length;
				return true;
			}
		}
	}

	return false;
}

unsigned char * CircularBuffer::Write(unsigned char *buff, long size)
{
    //can we grab space from the buffer?
	if (size > (m_Size - m_FillCount))
	{
		LogMessage("WARNING:: We may be smacking the back of the queue buffer.\n");
		m_FillCount = 0;
    }

    m_Cumulative_Count += size;
    m_FillCount += size;

#ifdef _DEBUG_CHECKS
	if (m_WritePtr > (m_Buffer + m_Size))
	{
		LogMessage("Error with write ptr, greater than buffer.\n");
		m_WritePtr = m_Buffer;
	}
#endif

	unsigned char *start_write = m_WritePtr;

	if ((m_WritePtr + size) > (m_Buffer + m_Size))
	{
		long write1 = m_Size - (m_WritePtr - m_Buffer);
		memcpy(m_WritePtr, buff, write1);
		memcpy(m_Buffer, buff+write1, size - write1);
		m_WritePtr = m_Buffer + (size - write1);
	}
	else
	{
		memcpy(m_WritePtr, buff, size);
		m_WritePtr += size;
	}

	//now check the buffer guard bands

    return start_write;
}

bool CircularBuffer::Read(unsigned char *buff, unsigned char *read_ptr, long size)
{
	//check for errors
	if (read_ptr > (m_Buffer + m_Size))
	{
		return false;
	}
    //see if buffer goes off the end
    if ((read_ptr + size) > (m_Buffer + m_Size))
    {
        long read1 = m_Size - (read_ptr - m_Buffer);
		memcpy(buff, read_ptr, read1);
		memcpy(buff + read1, m_Buffer, size - read1);
    }
    else
    {
        memcpy(buff, read_ptr, size);
    }

    return true;
}

void CircularBuffer::RemoveAllPlayerEntries(long player_id)
{
	for (u32 i = 0; i < m_MessageSlots; i++)
	{
		//are there any messages from this player still on the queue?
		if (m_EntryBuffer[i].player_id == player_id)
		{
			//Decommit the entry
			DeCommit(m_EntryBuffer[i].message, m_EntryBuffer[i].length);
		}
	}
}

bool CircularBuffer::DeCommit(unsigned char *dest, long size)
{
    m_FillCount -= size;
	if (m_FillCount < 0) m_FillCount = 0;
	m_EntriesInQueue--;
	if (m_EntriesInQueue < 0) m_EntriesInQueue = 0;
    return true;
}

unsigned long CircularBuffer::Cumulative_Written(bool clear)
{
    unsigned long count = m_Cumulative_Count;
    if (clear) m_Cumulative_Count = 0;
    return count;
}

long CircularBuffer::ToBufferEnd()
{
	long to_end = m_Size - ((m_WritePtr - m_Buffer) + 1);

#ifdef _DEBUG_CHECKS
	if (to_end < 0)
	{
		LogMessage("Negative buffer spill value.\n");
		to_end = 0;
	}

#else
	if (to_end < 0) to_end = 0;
#endif

	return to_end;
}
