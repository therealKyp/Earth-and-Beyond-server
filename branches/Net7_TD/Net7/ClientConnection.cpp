// ClientConnection.cpp
// Used for Server to Client Connections.

#include "ClientConnection.h"

#include <openssl/rand.h>

ClientConnection::ClientConnection(SOCKET sock, unsigned short port)
:   TcpConnection(sock, port)
{
}

ClientConnection::~ClientConnection()
{
}

bool ClientConnection::KeyExchange()
{
	// Generate RC4 key
	unsigned int i = 0;
	unsigned char rc4key[RC4_KEY_SIZE];
	unsigned int seed;
	seed = (unsigned int) time(NULL) / 71;
	do
	{
		if(i % 2)
		{
			seed /= i + 1;
			RAND_seed(&seed, 4);
			seed *= 20 - i;
			RAND_seed(&seed, 4);
		}
		else
		{
			seed /= 20 - i;
			RAND_seed(&seed, 4);
			seed *= i + 1;
			RAND_seed(&seed, 4);
		}
		i++;
	}
	while(!RAND_status());

	RAND_bytes(rc4key, RC4_KEY_SIZE);
	SetRC4Key(rc4key);

	// Receive the pubic key packet
    Sleep(20);
	if (recv(m_Socket, (char *) m_Buffer, 74, 0) != 74)
		return false;

	//LogMessage("Received public key packet:\n");
	//DumpBuffer(m_TcpBuffer,74);

	// Clear the buffer
	memset(m_Buffer, 0, WWRSA_BLOCK_SIZE - RC4_KEY_SIZE + sizeof(long));

	// Put the length in front of the buffer
	unsigned char *key = m_Buffer + sizeof(long);
	*((unsigned long *) m_Buffer) = ntohl(WWRSA_BLOCK_SIZE);

	// Copy the RC4 key to the bottom of the buffer
	unsigned char *dest = &key[WWRSA_BLOCK_SIZE - 1];
	unsigned char *src = rc4key;
	for (i = 0; i < RC4_KEY_SIZE; i++)
		*dest-- = *src++;

	//LogMessage("RC4 session key (prior to encryption)...\n");
	//DumpBuffer(m_TcpBuffer,WWRSA_BLOCK_SIZE + sizeof(long));

	// Encrypt the RC4 key
	m_WestwoodRSA.Encrypt(key, WWRSA_BLOCK_SIZE, key);

	//LogMessage("Sending encrypted RC4 session key...\n");
	// Send the encrypted RC4 key to the server
	Send(m_Buffer, WWRSA_BLOCK_SIZE + sizeof(long));

	return true;
}
