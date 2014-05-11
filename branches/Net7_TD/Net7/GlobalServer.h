// GlobalServer.h

#ifndef _GLOBAL_SERVER_H_INCLUDED_
#define _GLOBAL_SERVER_H_INCLUDED_

#include "Server.h"
#include "GlobalListen.h"

class GlobalServer : public Server
{
public:
    GlobalServer(unsigned int ip_address);
    ~GlobalServer();
    
    void Initialize();

private:
    GlobalListen *listen;
};

#endif // _GLOBAL_SERVER_H_INCLUDED_
