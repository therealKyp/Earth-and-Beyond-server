// MasterConnection.h

#ifndef _MASTER_CONNECTION_H_INCLUDED_
#define _MASTER_CONNECTION_H_INCLUDED_

#include "Net7.h"
#include "ClientConnection.h"
#include "Opcodes.h"
#include "PacketStructures.h"

class MasterConnection : public ClientConnection
{

public:
    MasterConnection(SOCKET sock, unsigned short port);
    ~MasterConnection();

protected:
    void ProcessOpcode(short opcode, short bytes);

private:
    void HandleMasterJoin();
    void SendServerRedirect(long sector_id);
};

#endif // _MASTER_CONNECTION_H_INCLUDED_
