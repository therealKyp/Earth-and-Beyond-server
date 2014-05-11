// MasterServer.h

#ifndef _MASTER_SERVER_H_INCLUDED_
#define _MASTER_SERVER_H_INCLUDED_

#include "Server.h"

class MasterServer : public Server
{
public:
    MasterServer(unsigned int ip_address);
    ~MasterServer();
    
    void Initialize();
};

#endif // _MASTER_SERVER_H_INCLUDED_
