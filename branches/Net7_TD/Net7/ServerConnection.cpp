// ServerConnection.cpp
// Used for Server to Server Connections.

#include "ServerConnection.h"

ServerConnection::ServerConnection(SOCKET sock, unsigned short port)
:   TcpConnection(sock, port)
{
}

ServerConnection::~ServerConnection()
{
}

bool ServerConnection::KeyExchange()
{
    int length = 0;
    unsigned char *p = m_Buffer;
    bool success = false;

	//g_LogDebug.Print("DoKeyExchange sending public RSA-155 key:\n");
    // Send the RSA Public Key to the client
    length += m_WestwoodRSA.GetModulus(&p);
    length += m_WestwoodRSA.GetPublicExponent(&p);
	Send(m_Buffer, length);

	// Read only 4 bytes to obtain the key length
    length = recv(m_Socket, (char *) m_Buffer, 4, 0);
	if (length <= 0)
	{
        // Lost connection, exit without displaying an error
        // This allows a server ping with no consequences
		return false;
	}

    // Get the encrypted RC4 Session Key response from the client

	//DumpBuffer(m_Buffer,length);
    if (length == 4)
    {
        long key_length = (long) ntohl((*((unsigned long *) m_Buffer)));
        if ((key_length >= 64) && (key_length <= 65))
        {
            length = recv(m_Socket, (char *) m_Buffer, key_length, 0);
	        if (length <= 0)
	        {
                // Lost connection, exit without displaying an error
                // This allows a server ping with no consequences
		        return false;
	        }

            // make sure we were able to read the entire key
            if (length == key_length)
            {
            	//g_LogDebug.Print("DoKeyExchange received encrypted RC4 session key from client");
                // disregard leading byte if it is zero
                p = m_Buffer;
                if ((key_length == WWRSA_BLOCK_SIZE + 1) && (*p == 0))
                {
                    key_length--;
                    p++;
                }

                // validate the key length against the expected value
                if (key_length == WWRSA_BLOCK_SIZE)
                {
                    // Decrypt the RC4 Session Key
                    unsigned char rc4key[WWRSA_BLOCK_SIZE];
                    if (m_WestwoodRSA.Decrypt(p, WWRSA_BLOCK_SIZE, rc4key))
                    {
				        unsigned char rc4_key_buffer[RC4_KEY_SIZE];

                        // Reverse the order of the decrypted RC4 Session Key
					    rc4_key_buffer[0] = rc4key[0x3f];
					    rc4_key_buffer[1] = rc4key[0x3e];
					    rc4_key_buffer[2] = rc4key[0x3d];
					    rc4_key_buffer[3] = rc4key[0x3c];
					    rc4_key_buffer[4] = rc4key[0x3b];
					    rc4_key_buffer[5] = rc4key[0x3a];
					    rc4_key_buffer[6] = rc4key[0x39];
					    rc4_key_buffer[7] = rc4key[0x38];

					    SetRC4Key(rc4_key_buffer);

					    //LogMessage("Decrypted RC4 session key:\n");
					    memcpy(m_Buffer, rc4_key_buffer, 8);
					    //DumpBuffer(m_TcpBuffer,8);
                        success = true;
                    }
				    else
				    {
					    g_LogError.Print("ERROR: DoKeyExchange m_WestwoodRSA.Decrypt failed\n");
				    }
                }
			    else
			    {
				    g_LogError.Print("ERROR: DoKeyExchange key_length = %d\n", key_length);
			    }
            }
		    else
		    {
			    g_LogError.Print("ERROR: DoKeyExchange key_length = %d, recv_length = %d\n", key_length, length);
		    }
        }
		else
		{
			g_LogError.Print("ERROR: DoKeyExchange key_length = %d\n", key_length);
		}
    }
    else if (length != 0)
	{
		g_LogError.Print("ERROR: DoKeyExchange expecting 4 byte key length, length = %d\n", length);
	}

    return success;
}
