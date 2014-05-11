// TcpConnection.h
#ifndef _TCP_CONNECTION_H_INCLUDED_
#define _TCP_CONNECTION_H_INCLUDED_

#include "Mutex.h"
#include "WestwoodRSA.h"
#include "WestwoodRC4.h"
#include "PacketStructures.h"
#include "Server.h"

// Isn't this a bit... huge? 128kb..
#define TCP_BUFFER_SIZE (128 * 1024)
#define RC4_KEY_SIZE        8
#define RC4_UDP_KEY_SIZE    16

class TcpConnection
{

public:
    TcpConnection(SOCKET socket, unsigned short port);
    virtual ~TcpConnection();
    
    void Shutdown();
    bool IsActive();
    virtual bool Initialize();
    virtual void Loop();
    void RunThread();
    
    virtual void ProcessOpcode(short opcode, short bytes) = 0;
    
    void Send(unsigned char *buffer, int length);
    void SendResponse(short opcode, unsigned char *data, int length);
    
protected:
    void SetRC4Key(unsigned char *rc4_key);
    virtual bool KeyExchange() = 0;
    void DumpBuffer(unsigned char *buffer, int length);

protected:
    // Attributes required for all servers
    Mutex m_Mutex;
    WestwoodRSA m_WestwoodRSA;               // RSA-155 encryption
    SOCKET m_Socket;                         // Our TCP/IP socket
    bool m_Active;                           // true if the TCP/IP connection is active
    bool m_Running;                          // true if TCP Thread is running
    unsigned char m_Buffer[TCP_BUFFER_SIZE]; // I/O buffer
    unsigned short m_Port;                   // TCP/IP port number
    bool m_Logging;
#ifndef WIN32
    pthread_t m_Thread;
#endif

    WestwoodRC4 m_CryptIn;          // RC4 decryption for inbound data
    WestwoodRC4 m_CryptOut;         // RC4 encryption for outbound data
    
    // To be fixed later:
    //Server &m_OwningServer;
};

#endif // _TCP_CONNECTION_H_INCLUDED_
