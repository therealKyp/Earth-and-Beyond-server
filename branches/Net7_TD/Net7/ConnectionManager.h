// ConnectionManager.h

#ifndef _CONNECTION_MANAGER_H_INCLUDED_
#define _CONNECTION_MANAGER_H_INCLUDED_

#include "TcpConnection.h"
#include "SslConnection.h"

class ConnectionManager
{
public:
    ConnectionManager();
    ~ConnectionManager();
    
    void AddTcp(TcpConnection *connection);
    void AddSsl(SslConnection *connection);
    void CheckTcp();
    void CheckSsl();

private:
    // double linked list for TCP Connection
    struct TcpEntry;
    struct TcpEntry
    {
        TcpConnection *connection;
        TcpEntry *next;
    };

    // double linked list for SSL Connection
    struct SslEntry;
    struct SslEntry
    {
        SslConnection *connection;
        SslEntry *next;
    };

private:
    Mutex m_Mutex;
    TcpEntry *m_TcpList;
    SslEntry *m_SslList;
};

#endif // _CONNECTION_MANAGER_H_INCLUDED_
