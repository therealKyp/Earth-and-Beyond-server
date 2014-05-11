// SectorServerManager.cpp
//

#include "Net7.h"
#include "SectorServerManager.h"
#include "Connection.h"
#include "UDPConnection.h"
#include "ServerManager.h"

SectorServerManager::SectorServerManager()
{
	// Sector servers
	m_NumSectorServers = 0;

	// Sectors
	m_NumSectors = 0;
	m_NumUnassignedSectors = 0;
	memset(m_SectorAssigned, 0, sizeof(m_SectorAssigned));
    memset(m_SectorName, 0, sizeof(m_SectorName));
    memset(m_SystemName, 0, sizeof(m_SystemName));
    memset(m_ParentSectorName, 0, sizeof(m_ParentSectorName));

	m_ServerMgr = NULL;
	m_ServerList = NULL;

	//LoadSectorList();
	LoadSectorServers();
}

SectorServerManager::~SectorServerManager()
{
	// Kill all TCP/IP connections and destroy the objects
	SectorServer * p = m_ServerList;
	SectorServer * next = NULL;
	while (p)
	{
		next = p->next;
		//if (p->username)
		//{
		//	delete p->username;
		//}
        for (short i=0; i < p->max_sectors; i++)
        {
            if (p->udp_connection[i])
            {
        		delete p->udp_connection[i];
            }
        }
		delete p;
		p = next;
	}

	for (long i=0; i < m_NumSectors; i++)
	{
		if (m_SectorName[i])
		{
			delete [] m_SectorName[i];
		}
	}
}

void SectorServerManager::LoadSectorList()
{
	// TODO: Load this data from an XML file
	// For now, let's just load the list from a flat text file
	char buffer[4096];
	char filename[MAX_PATH];
	sprintf(filename, "%ssector_list.txt", SERVER_DATABASE_PATH);
	FILE * f = fopen(filename, "r");
	if (f)
	{
		while (!feof(f))
		{
			if (fgets(buffer, sizeof(buffer), f))
			{
                // strip off any trailing cr/lf
				strtok(buffer, "\r\n");
				// ignore blank lines and records starting with a semicolon
                char c = buffer[0];
				if ((c != ';') && (c != 0))
				{
                    strcat(buffer, ",,,,");
					char *number = strtok(buffer, ",");
					char *name = strtok(NULL, ",");
                    char *system = strtok(NULL, ",");
                    char *sector_name = strtok(NULL, ",");
					if (name)
					{
						AddSector(atoi(number), name, system, sector_name);
					}
				}
			}
		}
		fclose(f);
	}
}

bool SectorServerManager::LookupSectorServer(ServerRedirect & redirect)
{
	bool success = false;

	// Scan through the linked list
	SectorServer * server = m_ServerList;
	while (server)
	{
        for (short i=0; i < server->max_sectors; i++)
        {
		    if (server->sector_assigned[i] == (long) ntohl(redirect.sector_id))
		    {
			    redirect.ip_address = server->ip_address;
			    redirect.port = server->port + i;
			    success = true;
				LogMessage("Sending to Port: %d\n",redirect.port);
			    return (success);
		    }
        }
		server = server->next;
	}

	return (success);
}

void SectorServerManager::AddSector(long sector_id, char *sector_name, char *system_name, char *parent_sector_name)
{
	m_SectorID[m_NumSectors] = sector_id;

    m_SectorName[m_NumSectors] = new char[strlen(sector_name) + 1];
	strcpy(m_SectorName[m_NumSectors], sector_name);

	m_SystemName[m_NumSectors] = new char[strlen(system_name) + 1];
	strcpy(m_SystemName[m_NumSectors], system_name);

    if (parent_sector_name)
    {
	    m_ParentSectorName[m_NumSectors] = new char[strlen(parent_sector_name) + 1];
	    strcpy(m_ParentSectorName[m_NumSectors], parent_sector_name);
    }
    else
    {
        m_ParentSectorName[m_NumSectors] = NULL;
    }

    m_SectorAssigned[m_NumSectors] = false;
	m_NumSectors++;
	m_NumUnassignedSectors++;
}

void SectorServerManager::SetServerManager(ServerManager * server_mgr)
{
	m_ServerMgr = server_mgr;
}

bool SectorServerManager::RegisterSectorServer(unsigned long ip_address, short port_number, short max_sectors, char *username)
{
	bool success = false;
	unsigned char * ip = (unsigned char *) &ip_address;
	LogMessage("RegisterSectorServer at IP address %d.%d.%d.%d, port %d\n",
		ip[0], ip[1], ip[2], ip[3], port_number);

	// To register a sector server, the IP address must be in the
	// list of authorized servers.  This list may be updated by
	// the user via a Web Browser using the same IP address as the
	// server.

	// Scan through the linked list
	SectorServer * server = m_ServerList;
	while (server)
	{
		if ((server->ip_address == ip_address) &&
			/*(server->port == port_number) &&*/
			(strcmp(server->username, username) == 0))
		{
            if (server->max_sectors != max_sectors)
            {
                server->max_sectors = max_sectors;
            }
			//LogMessage("Sector Server authenticated\n");
			//success = ConnectBackToSectorServer(server);
            success = true;
			break;
		}
		server = server->next;
	}

	return success;
}

bool SectorServerManager::CheckConnections()
{
	bool assignments_complete = true;
    // Called by the Main thread in the Main Loop
	// Loop through the linked list of connections to Sector Servers
	// and remove those that are no longer active.

	// Loop through the linked list of connections to Sector Servers
	// and see if any have reached the ready state
	SectorServer * server = m_ServerList;
	while (server)
	{
		if (server->countdown_to_ready > 0)
		{
			server->countdown_to_ready -= 100;
			if (server->countdown_to_ready <= 0)
			{
				server->countdown_to_ready = 0;
			}
			break;
		}
		server = server->next;
	}

	//g_Sector_Start = 0;

	// Assign a sector to an available server if we have any unassigned sectors
	// Scan through the linked list
	// Do we have any available servers?
	if (m_NumUnassignedSectors > 0)
	{
		for (long i=0; i < m_NumSectors; i++)
		{
			if (!m_SectorAssigned[i])
			{
                if  (m_SectorID[i] >= 973)
                {
				    if (AssignSectorToAvailableServer(m_SectorID[i], m_SectorName[i]))
				    {
						//LogMessage("Assigned ID: %d out of %d\n", i, m_NumUnassignedSectors);
					    m_SectorAssigned[i] = true;
    					m_NumUnassignedSectors--;
                        Sleep(100); // wait 100 ms between assignments
						assignments_complete = false;
                        break;
                    } 
                    else 
                    {
						//LogMessage("Cant assign Sector: %d to server\n", m_SectorID[i]);
					}
                }
			}
		}
	}

	return assignments_complete;
}

bool SectorServerManager::AssignSectorToAvailableServer(long sector_id, char *sector_name)
{
	//LogMessage("Looking for an available server for sector %d (%s)\n", sector_id, sector_name);
	// Loop through the list of servers to find one that is available
	SectorServer * server = m_ServerList;
	while (server)
	{
		// Is this server available?
		if (server->countdown_to_ready == 0)
		{
			unsigned char * ip = (unsigned char *) &server->ip_address;
			//LogMessage("Max Sectors: %d\n", server->max_sectors);
			// Found an available server
            for (int i = 0; i < server->max_sectors; i++)
            {
                if (server->sector_assigned[i] == 0 && sector_id > 1000)
                {
			        //LogMessage("Assigning sector to server at IP address %d.%d.%d.%d, port %d\n",
				        //ip[0], ip[1], ip[2], ip[3], server->port + i);
                    short port = server->port + i;
                    server->sector_assigned[i] = sector_id;
                    g_ServerMgr->m_UDPMasterConnection->ValidateSectorServer(sector_id, port, server->ip_address);
        			//server->udp_connection[i]->SendSectorAssignment(sector_id);
                    return true;
                }
            }

		}
		server = server->next;
	}

	return false;
}

bool SectorServerManager::LoadSectorServers()
{
	bool success = false;
	// Record format example:
	//
	// 192.168.1.101,3500,18,VectoR,VectoR.360,Vector.360@gmail.com,VectoR's multi-player sector server
	// 
    // Fields:
    //	- IP Address
    //	- First Port
    //  - Max Number of Sectors
    //	- Username
    //  - Toon name to be listed in the credits
    //	- Email Address,
    //	- Server Description

	// Read the list of usernames and passwords from accounts.txt
	SectorServer * last = NULL;
	char buffer[256];
	char filename[MAX_PATH];
	sprintf(filename, "%ssector_servers.txt", SERVER_DATABASE_PATH);
	FILE *f = fopen(filename, "r");
	if (f)
	{
		while (!feof(f))
		{
			if (fgets(buffer, sizeof(buffer), f))
			{
                // strip off any trailing cr/lf
				strtok(buffer, "\r\n");
				// ignore blank lines and records starting with a semicolon
                char c = buffer[0];
				if ((c != ';') && (c != 0))
				{
                    strcat(buffer, ",,,,");
					char *ip_address = strtok(buffer, ",");
					char *port = strtok(NULL, ",");
					char *max_sectors = strtok(NULL, ",");
					char *username = strtok(NULL, ",");
					char *toon = strtok(NULL, ",");
					char *email = strtok(NULL, ",");
					char *description = strtok(NULL, ",");
					if (ip_address && port && max_sectors && username)
					{
						// Create a new entry to add to the linked list
						SectorServer * server = new SectorServer;
                        memset(server, 0, sizeof(SectorServer));
						server->ip_address = inet_addr(ip_address);
						server->port = atoi(port) + 1;
                        server->max_sectors = atoi(max_sectors);
						server->username = new char[strlen(username) + 1];
						strcpy(server->username, username);
						server->countdown_to_ready = 0;
						server->next = NULL;

						// Add this server the end of the linked list
						if (last)
						{
							last->next = server;
						}
						else
						{
							m_ServerList = server;
						}
						last = server;
						m_NumSectorServers++;
					}
				}
			}
		}
	}

	return success;
}

bool SectorServerManager::SendPacket(long sector_id, short opcode, unsigned char *data, size_t length)
{
	SectorServer * server = m_ServerList;
	while (server)
	{
        for (short i=0; i < server->max_sectors; i++)
        {
		    // Is this the correct server?
		    if ((server->sector_assigned[i] == sector_id) &&
			    (server->udp_connection[i]))
		    {
                LogMessage("SendPacket -- bad!\n");
			    //server->udp_connection[i]->SendResponse(opcode, data, length);
			    return true;
		    }
        }
		server = server->next;
	}

	return false;
}

