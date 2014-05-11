// Server.h

#ifndef _SERVER_H_INCLUDED_
#define _SERVER_H_INCLUDED_

class Server
{
public:
    Server(unsigned int ip_address, const char *name);
    virtual ~Server();
    
    virtual void Error(const char *error);
    virtual void Initialize() = 0;
    //virtual void Check();
    //virtual void Shutdown() = 0;
    
protected:
    unsigned int m_IpAddress;
    char m_ServerName[16]; // Name of the Server ("Account", "Master", "1060" (Sector))
};

#endif // _SERVER_H_INCLUDED_
