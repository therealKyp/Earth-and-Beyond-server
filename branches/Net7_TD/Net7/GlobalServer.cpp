// GlobalServer.cpp

#include "Net7.h"
#include "GlobalServer.h"

GlobalServer::GlobalServer(unsigned int ip_address)
: Server(ip_address, "Global")
{
    //listen = new GlobalListen(m_IpAddress, DEFAULT_GLOBAL_SERVER_PORT);
}

GlobalServer::~GlobalServer()
{
    //if(listen)    
        //delete listen;
}

void GlobalServer::Initialize()
{

}
