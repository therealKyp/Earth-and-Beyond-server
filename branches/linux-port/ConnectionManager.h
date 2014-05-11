// ConnectionManager.h

#ifndef _CONNECTION_MANAGER_H_INCLUDED_
#define _CONNECTION_MANAGER_H_INCLUDED_

#include "Mutex.h"

class SSL_Connection;
class Connection;

class ConnectionManager
{
public:
	ConnectionManager();
	virtual ~ConnectionManager();

public:
	void	AddSslConnection(SSL_Connection *ssl_connection);
	void	AddConnection(Connection *tcp_connection);
	void	CheckConnections();
	void	CheckSslConnections();
	u32		GetConnectionCount()		{ return m_ConnectionCount; }

private:
	// linked list for SSL Connection
	struct SslConnectionEntry;
	struct SslConnectionEntry
	{
		SSL_Connection * connection;
		struct SslConnectionEntry * next;
	};
	// linked list for TCP Connection
	struct ConnectionEntry;
	struct ConnectionEntry
	{
		Connection * connection;
		struct ConnectionEntry * next;
	};

private:
    Mutex   m_Mutex;
	SslConnectionEntry * m_SslConnectionList;
	ConnectionEntry * m_ConnectionList;
	u32		m_ConnectionCount;
};

#endif // _CONNECTION_MANAGER_H_INCLUDED_
