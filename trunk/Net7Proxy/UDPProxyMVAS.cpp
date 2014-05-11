#include "Net7.h"
#include "UDPClient.h"
#include "ServerManager.h"
#include "Opcodes.h"
#include "Connection.h"

DWORD g_addr_off = 0;
DWORD g_thread_speed = 10;

unsigned char g_last_position[32];
bool g_ClientTerminate = false;

//this thread spins and sends position updates periodically to the server
void UDPClient::MVASThread()
{
	//float last_coords[3];
	memset(&g_last_position, 0, sizeof(g_last_position));

	WaitForLogin();

    LogMessage("MVAS thread running\n");

    DWORD last_comms_send = 0;

	while (!g_ServerShutdown)
	{
        DWORD tick = GetNet7TickCount();
        
        if (!ClientStillRunning())
        {
            if (m_PlayerID != 0 && ConnectionActive())
            {
                LogMessage("Client has terminated. Sending termination signal.\n");
                SendResponse(m_PlayerID, MVAS_LOGIN_PORT, ENB_OPCODE_1008_MVAS_LOGOFF_C_S, (unsigned char *) &m_PlayerID, sizeof(m_PlayerID));
                g_ClientTerminate = true;
                Sleep(2000);
                g_ServerShutdown = true;
            }
            else
            {
                g_ServerShutdown = true;
            }
        }
        else
        {
            //see if we have an addr for coords yet
            if (ConnectionActive() && CheckPosition() && GetSectorID() < 9999)
            {   //OK, we've got a live position and connection is active
                //TODO: design going to warp and exit warp custom packets
                SendPositionIfChanged();
            }
            if ( (m_PlayerID != 0 && ConnectionActive()) && tick > (last_comms_send + 30*1000) && g_ServerMgr->m_SectorConnection)
            {
                SendClientAlive();
                last_comms_send = tick;
            }
            else if (tick < last_comms_send)
            {
                last_comms_send = tick;
            }

            Sleep(g_thread_speed*500);
        }
	}
}

void UDPClient::TerminateClient(char *msg)
{
    long player_id = *((long *) &msg[0]);

    if (player_id == m_PlayerID)
    {
		ShutdownClient();
        //shutdown the client if it's still running and then close Net7Proxy
        g_ServerShutdown = true;
    }
}

void UDPClient::TerminateClient()
{
	ShutdownClient();
    g_ServerShutdown = true;
}

// see if we've got a live position
bool UDPClient::CheckPosition()
{
	DWORD addr_off = g_AddrStore;
	DWORD proc_read[2];
    proc_read[0] = 0;
    proc_read[1] = 0;
    bool ready_to_send = false;

	engine_read_process((void*)addr_off, (void*)proc_read, 8);

	if (proc_read[0] != 0)
	{
		if ((proc_read[0]+0x48) != g_addr_off)
		{
			g_addr_off = proc_read[0] + 0x48;
            //fprintf(stderr,"\nSector Change Detected.");
		}
		ready_to_send = true;
	}

    if (ready_to_send == true) 
    {
        return true;
    }
    else
    {
        return false;
    }
}

void UDPClient::SendPositionIfChanged()
{
	unsigned char pos_data[32];
	unsigned char buffer[64];
	float x, y, z;
    static short stationary_send_tick = 3;

	//see if we have a live address for position first
	if (g_addr_off == 0)
	{
		return;
	}

    //read position
    engine_read_process((void*)g_addr_off, (void*)buffer, 48);
    
    *((float *) &pos_data[0]) = *((float *) &buffer[12]);
    *((float *) &pos_data[4]) = *((float *) &buffer[28]);
    *((float *) &pos_data[8]) = *((float *) &buffer[44]);
    *((float *) &pos_data[12]) = *((float *) &buffer[0]);
    *((float *) &pos_data[16]) = *((float *) &buffer[16]);
    *((float *) &pos_data[20]) = *((float *) &buffer[32]);

	x = *((float *) &pos_data[0]);
	y = *((float *) &pos_data[4]);
	z = *((float *) &pos_data[8]);

	if (x == 0 && y == 0 && z == 0)
	{
		return;
	}

    if (memcmp(&pos_data[12], &g_last_position[12], 12) != 0) //has orientation changed?
    {
		SendResponse(m_PlayerID, MVAS_LOGIN_PORT, ENB_OPCODE_1004_MVAS_SEND_POSITION_C_S, pos_data, 24); //send position and orientation change
		memcpy(&g_last_position, &pos_data, 24);
        stationary_send_tick = 2;
		//LogMessage("MVAS/P: %.2f %.2f %.2f\n", x, y, z);
    }
    else if (memcmp(&pos_data[0], &g_last_position[0], 12) != 0) //has position changed?
	{
		SendResponse(m_PlayerID, MVAS_LOGIN_PORT, ENB_OPCODE_1004_MVAS_SEND_POSITION_C_S, pos_data, 12); //only send position change
		memcpy(&g_last_position, &pos_data, 24);
        stationary_send_tick = 2;
		//LogMessage("MVAS: %.2f %.2f %.2f\n", x, y, z);
	}
    else if (stationary_send_tick > 0)
    {
        stationary_send_tick--;
        SendResponse(m_PlayerID, MVAS_LOGIN_PORT, ENB_OPCODE_1004_MVAS_SEND_POSITION_C_S, pos_data, 24);
    }
}

void UDPClient::ToggleSendFrequency(char *msg)
{
	g_thread_speed = *((long *) &msg[0]);
	if (g_thread_speed < 1 || g_thread_speed > 20)
	{
		LogMessage("Warning: Bad frequency received: %d\n", g_thread_speed);
		g_thread_speed = 1;
	}
}

void UDPClient::SendClientAlive()
{
    if (m_PlayerID != 0)
    {
        if (m_AlternatePorts)
        {
            g_ServerMgr->m_UDPConnection->SendCommsAlive();
        }
        else
        {
            SendResponse(m_PlayerID, MVAS_LOGIN_PORT, ENB_OPCODE_3005_PLAYER_COMMS_ALIVE, 0, 0);
        }
        m_AlternatePorts = !m_AlternatePorts;
    }
}