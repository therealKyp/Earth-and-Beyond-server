// AccountServer.h

#ifndef _ACCOUNT_SERVER_H_INCLUDED_
#define _ACCOUNT_SERVER_H_INCLUDED_

#include "Server.h"

class AccountServer : public Server
{
public:
    AccountServer(unsigned int ip_address);
    ~AccountServer();
    
    void Initialize();
};

#endif // _ACCOUNT_SERVER_H_INCLUDED_
