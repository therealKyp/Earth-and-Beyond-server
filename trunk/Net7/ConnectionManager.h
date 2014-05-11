// ConnectionManager.h

#ifndef _CONNECTION_MANAGER_H_INCLUDED_
#define _CONNECTION_MANAGER_H_INCLUDED_

#include "Mutex.h"
#include "MessageQueue.h"

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

	static UINT WINAPI ConnectionManager::StartOpcodeResendThread(void *Param);
	void	RunOpcodeResendThread();
	void	HandleResend(long player_id, long packet_num);

private:
    Mutex   m_Mutex;
	SslConnectionEntry * m_SslConnectionList;
	ConnectionEntry * m_ConnectionList;
	u32		m_ConnectionCount;
	HANDLE				m_ResendThreadHandle;
	bool				m_resend_thread_running;
	//unsigned char		m_PacketCache[2048];
	MessageQueue	  * m_ResendQueue;
	CircularBuffer	  * m_ResendBuffer;
};

struct EnbResendHeader
{
    long	player_id;
    long	packet_num;
};

#endif // _CONNECTION_MANAGER_H_INCLUDED_
