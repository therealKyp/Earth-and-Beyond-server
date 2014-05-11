// ConnectionManager.cpp


#include "Net7.h"
#include "SSL_Connection.h"
#include "ConnectionManager.h"
#include "Connection.h"


ConnectionManager::ConnectionManager()
{
	//m_SslConnectionList = NULL;
	m_ConnectionList = NULL;
}

ConnectionManager::~ConnectionManager()
{
	// Kill all TCP/IP connections and destroy the objects
	ConnectionEntry * p = m_ConnectionList;
	ConnectionEntry * next = NULL;
	while (p)
	{
		next = p->next;
		delete p->connection;
		delete p;
		p = next;
	}

	/*SslConnectionEntry * p_ssl = m_SslConnectionList;
	SslConnectionEntry * next_ssl = NULL;
	while (p_ssl)
	{
		next_ssl = p_ssl->next;
		delete p_ssl->connection;
		delete p_ssl;
		p_ssl = next_ssl;
	}*/
}

/*void ConnectionManager::AddSslConnection(SSL_Connection *ssl_connection)
{
	SslConnectionEntry *entry = new SslConnectionEntry;
	entry->connection = ssl_connection;
	entry->next = NULL;
	if (m_SslConnectionList)
	{
		// Find the last entry in the linked list
		SslConnectionEntry * p = m_SslConnectionList;
		while (p->next)
		{
			p = p->next;
		}
		// Add the new entry to the end of the linked list
		p->next = entry;
	}
	else
	{
		m_SslConnectionList = entry;
	}
}*/

void ConnectionManager::AddConnection(Connection *tcp_connection)
{
	ConnectionEntry *entry = new ConnectionEntry;
	entry->connection = tcp_connection;
	entry->next = NULL;
	if (m_ConnectionList)
	{
		ConnectionEntry * p = m_ConnectionList;
		while (p->next)
		{
			p = p->next;
		}
		p->next = entry;
	}
	else
	{
		m_ConnectionList = entry;
	}
	//LogMessage("ConnectionManager added TCP connection.\n");
}

void ConnectionManager::CheckConnections()
{
	// Drop the dead TCP/IP connections and destroy the objects
	ConnectionEntry * last = NULL;
	ConnectionEntry * p = m_ConnectionList;
	ConnectionEntry * kill = NULL;
	while (p)
	{
		if (!p->connection->IsActive() || p->connection->CheckTCPShutdownCycle())
		{
			// Kill this connection
			kill = p;
			// This TCP connection is no longer active
			// Remove this entry from the linked list
			if (last)
			{
				last->next = p->next;
			}
			else
			{
				m_ConnectionList = p->next;
			}
		}
		else
		{
			last = p;
		}
		p = p->next;
		if (kill)
		{
			//LogMessage("ConnectionManager deleted TCP connection\n");
			delete kill->connection;
			delete kill;
			kill = NULL;
		}
	}
}

/*void ConnectionManager::CheckSslConnections()
{
	// Drop the dead SSL connections and destroy the objects
	SslConnectionEntry * last = NULL;
	SslConnectionEntry * p = m_SslConnectionList;
	SslConnectionEntry * kill = NULL;
	while (p)
	{
		if (!p->connection->IsActive())
		{
			// Kill this connection
			kill = p;
			// This SSL connection is no longer active
			// Remove this entry from the linked list
			if (last)
			{
				last->next = p->next;
			}
			else
			{
				m_SslConnectionList = p->next;
			}
		}
		else
		{
			last = p;
		}
		p = p->next;
		if (kill)
		{
			//LogMessage("ConnectionManager deleted SSL connection\n");
			delete kill->connection;
			delete kill;
			kill = NULL;
		}
	}

}*/

