// ConnectionManager.cpp


#include "Net7.h"
#include "ConnectionManager.h"
#include "SSL_Connection.h"
#include "Connection.h"
#include "ServerManager.h"
#include "PlayerClass.h"
#include "Opcodes.h"


ConnectionManager::ConnectionManager()
{
	UINT uiThreadId = 0;
	m_SslConnectionList = NULL;
	m_ConnectionList = NULL;
	m_ConnectionCount = 0;
	m_resend_thread_running = false;
	m_ResendBuffer = new CircularBuffer(0x1000, 2048);
	m_ResendQueue = new MessageQueue(m_ResendBuffer);

	//Don't use this system yet, we may not need it.
	//m_ResendThreadHandle = (HANDLE)_beginthreadex(NULL, 0, StartOpcodeResendThread, this, CREATE_SUSPENDED, &uiThreadId);
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

	SslConnectionEntry * p_ssl = m_SslConnectionList;
	SslConnectionEntry * next_ssl = NULL;
	while (p_ssl)
	{
		next_ssl = p_ssl->next;
		delete p_ssl->connection;
		delete p_ssl;
		p_ssl = next_ssl;
	}
}

void ConnectionManager::AddSslConnection(SSL_Connection *ssl_connection)
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
}

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

	m_ConnectionCount++;
}

void ConnectionManager::CheckConnections()
{
	// Drop the dead TCP/IP connections and destroy the objects
	ConnectionEntry * last = NULL;
	ConnectionEntry * p = m_ConnectionList;
	ConnectionEntry * kill = NULL;

	while (p)
	{
		if (!p->connection->IsActive())
		{
			kill = p;
			// This connection is no longer active
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
			p->connection->PulseConnectionOutput();
			last = p;
		}
		p = p->next;
		if (kill)
		{
			LogMessage("ConnectionManager closed TCP connection for [%08x]\n", kill->connection->GameID());
			//kill the node, but don't kill the connection as the connection is static
			delete kill; 
			kill = NULL;
			m_ConnectionCount--;
		}
	}
}

void ConnectionManager::CheckSslConnections()
{
	// Drop the dead SSL connections and destroy the objects
	// Perform a check of each active SSL connection and destroy if they are dead
	SslConnectionEntry * last = NULL;
	SslConnectionEntry * p = m_SslConnectionList;
	SslConnectionEntry * kill = NULL;

	u32 tick = GetNet7TickCount();

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
			//delete kill->connection;
			delete kill;
			kill = NULL;
		}
	}

}
/*
void ConnectionManager::RunOpcodeResendThread()
{
    int length;
    EnbResendHeader *header;
	unsigned char msg[128];

    
    m_resend_thread_running = true;
    
    while (!g_ServerShutdown)
    {  
        while (m_ResendQueue->CheckQueue(msg, &length, 2048)) //check if there are any messages in the queue, if there are write them into the 'msg' buffer
        {
            //process this message
            //format is opcode/length/message
            header = (EnbResendHeader*)msg;
            
            //process opcode accordingly
			HandleResend(header->player_id, header->packet_num);
        }
        SuspendThread(m_ResendThreadHandle);
        Sleep(5);
    }
}

void ConnectionManager::HandleResend(long player_id, long packet_num)
{
	//grab message
	CircularBuffer *udp_buff = g_ServerMgr->GetUDPCBuffer();
	int length;
	bool got_message = udp_buff->RetreiveMessage(m_PacketCache, &length, 2048, packet_num, player_id);

	if (got_message)
	{
		//now plop this into the player's message queue
		Player *p = g_PlayerMgr->GetPlayer(player_id);
		if (p)
		{
			p->AddMessage(ENB_OPCODE_4000_RESEND_PACKET_SEQUENCE, length, m_PacketCache);
		}
	}
}

UINT WINAPI ConnectionManager::StartOpcodeResendThread(LPVOID Param)
{
	ConnectionManager* p_this = reinterpret_cast<ConnectionManager*>( Param );

	p_this->RunOpcodeResendThread();

	return 1;
}*/
