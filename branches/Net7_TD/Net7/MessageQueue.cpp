// MessageQueue.cpp

#include "Net7.h"
#include "MessageQueue.h"

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
	char *message;

	while (CheckQueue(&message))
		delete [] message;
}

void MessageQueue::LockMessageQueue()
{
    m_GroupMutex.Lock();
}

void MessageQueue::UnlockMessageQueue()
{
    m_GroupMutex.Unlock();
}

void MessageQueue::Add(char *buffer)
{
    m_Mutex.Lock();

	MessageEntry *new_entry = new MessageEntry;
	new_entry->message = new char[strlen(buffer) + 1];
	strcpy(new_entry->message, buffer);
	new_entry->next = NULL;

	// Is the Queue empty?
	if (m_Queue)
	{
		// No, Find the end of the linked list
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

	m_TotalAdded++;
	m_TotalAddedToTail++;
	m_EntriesInQueue++;

    m_Mutex.Unlock();
}

void MessageQueue::AddHead(char *buffer)
{
    m_Mutex.Lock();

	MessageEntry *new_entry = new MessageEntry;
	new_entry->message = new char[strlen(buffer) + 1];
	strcpy(new_entry->message, buffer);
	new_entry->next = m_Queue;

	// Insert this entry at the head of the Queue
	m_Queue = new_entry;

	m_TotalAdded++;
	m_TotalAddedToHead++;
	m_EntriesInQueue++;

    m_Mutex.Unlock();
}

// returns true if message was removed from the Queue
bool MessageQueue::CheckQueue(char **pMessage)
{
	bool success = false;
    m_Mutex.Lock();

	if (m_Queue)
	{
		MessageEntry *entry = m_Queue;
		m_Queue = entry->next;
		*pMessage = entry->message;
		delete entry;

		m_TotalRemoved++;
		m_EntriesInQueue--;
		success = true;
	}

    m_Mutex.Unlock();

	return success;
}
