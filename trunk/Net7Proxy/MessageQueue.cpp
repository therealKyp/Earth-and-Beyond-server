// MessageQueue.cpp

//////////////////////////////////////////////////////////////////////
// Include Files
//////////////////////////////////////////////////////////////////////

#include "Net7.h"
#include "MessageQueue.h"

////////////////////////////////////////////////////////////////////////////////
// Class : MessageQueue
////////////////////////////////////////////////////////////////////////////////

MessageQueue::MessageQueue()
{
	m_Queue = NULL;
	m_TotalAdded = 0;
	m_TotalAddedToHead = 0;
	m_TotalAddedToTail = 0;
	m_TotalRemoved = 0;
	m_EntriesInQueue = 0;
}

MessageQueue::~MessageQueue()
{
	unsigned char *message;
    int length;

	while (CheckQueue( &message, &length ))
	{
        if (message && length > 0)
        {
            delete [] message;
        }
	}
}

void MessageQueue::Add( char *buffer )
{
    Add((unsigned char *) buffer, strlen(buffer) + 1);
}

void MessageQueue::Add( unsigned char *buffer, int length)
{
	MessageEntry *new_entry = new MessageEntry;
	new_entry->message = new unsigned char[length];
	memcpy(new_entry->message, buffer, length);
    new_entry->length = length;
	new_entry->next = NULL;

    // Critical Section
    m_Mutex.Lock();

    // Is the Queue empty?
	try
    {
        if (m_Queue)
        {
            MessageEntry *entry = m_Queue;
            
            while (entry->next)
            {
                entry = entry->next;
            }
            
            // Add this entry to the end of the linked list
            entry->next = new_entry;
        }
        else
        {
            // The linked list is empty, so this is the first entry
            m_Queue = new_entry;
        }
    }
    catch (...)
	{
		m_Mutex.Unlock();
		return;
	}

	m_TotalAdded++;
	m_TotalAddedToTail++;
	m_EntriesInQueue++;

    m_Mutex.Unlock();
}


void MessageQueue::AddHead( char *buffer )
{
	MessageEntry *new_entry = new MessageEntry;
	new_entry->message = new unsigned char[strlen(buffer) + 1];
	strcpy( (char *) new_entry->message, buffer );

    // Critical Section
    m_Mutex.Lock();

	new_entry->next = m_Queue;

	// Insert this entry at the head of the Queue
	m_Queue = new_entry;

	m_TotalAdded++;
	m_TotalAddedToHead++;
	m_EntriesInQueue++;

    m_Mutex.Unlock();
}

void MessageQueue::AddHead( unsigned char *buffer, int length )
{
	MessageEntry *new_entry = new MessageEntry;
	new_entry->message = new unsigned char[length];
	memcpy(new_entry->message, buffer, length);
    new_entry->length = length;

    // Critical Section
    m_Mutex.Lock();

	new_entry->next = m_Queue;

	// Insert this entry at the head of the Queue
	m_Queue = new_entry;

	m_TotalAdded++;
	m_TotalAddedToHead++;
	m_EntriesInQueue++;

    m_Mutex.Unlock();
}

// returns true if message was removed from the Queue
bool MessageQueue::CheckQueue( char **pMessage )
{
    int length;
    return CheckQueue( (unsigned char **) pMessage, &length);
}

// returns true if message was removed from the Queue
bool MessageQueue::CheckQueue( unsigned char **pMessage, int *length)
{
    bool success = false;
    
    if (m_Queue)
    {
        // Critical section
        m_Mutex.Lock();
        
        // Pull from the head of the queue
        MessageEntry *entry = m_Queue;
        
        m_Queue = entry->next;
        
        
        m_TotalRemoved++;
        m_EntriesInQueue--;
        m_Mutex.Unlock();
        
        *pMessage = entry->message;
        *length = entry->length;
        delete entry;
        success = true;
        
    }
    
    return (success);
}
