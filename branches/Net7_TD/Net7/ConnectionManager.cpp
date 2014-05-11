// ConnectionManager.cpp
//
// TODO: Modify the connection manager to instead wait for
//       the connection to tell it that it has died instead
//       of continuously checking all of them.
//       If you were to do this, I don't think you'd even
//       need a connection manager.

#include "Net7.h"
#include "ConnectionManager.h"

ConnectionManager::ConnectionManager()
{
    m_TcpList = NULL;
    m_SslList = NULL;
}

ConnectionManager::~ConnectionManager()
{
    m_Mutex.Lock();

	// Kill all TCP/IP connections and destroy the objects
	TcpEntry *p = m_TcpList;
	TcpEntry *next = NULL;
	while (p)
	{
		next = p->next;
		delete p->connection;
		delete p;
		p = next;
	}

	SslEntry *p_ssl = m_SslList;
	SslEntry *next_ssl = NULL;
	while (p_ssl)
	{
		next_ssl = p_ssl->next;
		delete p_ssl->connection;
		delete p_ssl;
		p_ssl = next_ssl;
	}
	
	m_Mutex.Unlock();
}

void ConnectionManager::AddTcp(TcpConnection *connection)
{
    TcpEntry *entry = new TcpEntry;
	
    entry->connection = connection;
    entry->next = NULL;
    
    m_Mutex.Lock();    
    
    if (m_TcpList)
    {
        // Find the last entry in the linked list
        TcpEntry *p = m_TcpList;
        while (p->next)
        {
            p = p->next;
        }
        // Add the new entry to the end of the linked list
        p->next = entry;
    }
    else
    {
        m_TcpList = entry;
    }
    
    m_Mutex.Unlock();
}

void ConnectionManager::AddSsl(SslConnection *connection)
{
    SslEntry *entry = new SslEntry;
	
    entry->connection = connection;
    entry->next = NULL;
    
    m_Mutex.Lock();    
    
    if (m_SslList)
    {
        // Find the last entry in the linked list
        SslEntry *p = m_SslList;
        while (p->next)
        {
            p = p->next;
        }
        // Add the new entry to the end of the linked list
        p->next = entry;
    }
    else
    {
        m_SslList = entry;
    }
    
    m_Mutex.Unlock();
}

void ConnectionManager::CheckTcp()
{
	// Drop the dead TCP/IP connections and destroy the objects
	TcpEntry *last = NULL;
	TcpEntry *p = m_TcpList;
	TcpEntry *kill = NULL;
	while(p)
	{
		if(!p->connection->IsActive())
		{
			// Kill this connection
			kill = p;
			// This TCP connection is no longer active
			// Remove this entry from the linked list
			if(last)
				last->next = p->next;
			else
				m_TcpList = p->next;
		}
		else
			last = p;
		
		p = p->next;
		
		if(kill)
		{
			//g_LogDebug->Print("ConnectionManager deleted TCP connection\n");
			delete kill->connection;
			delete kill;
			kill = NULL;
		}
	}
}

void ConnectionManager::CheckSsl()
{
	// Drop the dead SSL connections and destroy the objects
	SslEntry * last = NULL;
	SslEntry * p = m_SslList;
	SslEntry * kill = NULL;
	while (p)
	{
		if (!p->connection->IsActive())
		{
			// Kill this connection
			kill = p;
			// This SSL connection is no longer active
			// Remove this entry from the linked list
			if (last)
				last->next = p->next;
			else
				m_SslList = p->next;
		}
		else
			last = p;

		p = p->next;
		
		if (kill)
		{
			//g_LogDebug->Print("ConnectionManager deleted SSL connection\n");
			delete kill->connection;
			delete kill;
			kill = NULL;
		}
	}
}
