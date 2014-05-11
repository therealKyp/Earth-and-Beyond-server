// GlobalConnection.h

#ifndef _GLOBAL_CONNECTION_H_INCLUDED_
#define _GLOBAL_CONNECTION_H_INCLUDED_

#include "ClientConnection.h"

class GlobalConnection : public ClientConnection
{
public:
    GlobalConnection(SOCKET sock, unsigned short port);
    ~GlobalConnection();

protected:
    void ProcessOpcode(short opcode, short bytes);

private:
    void HandleVersionRequest();
    
    void SendVersionResponse(int status);
    
    void HandleGlobalConnect();
    void HandleGlobalTicketRequest();
    void HandleDeleteCharacter();
    void HandleCreateCharacter();
};

#endif // _GLOBAL_CONNECTION_H_INCLUDED_
