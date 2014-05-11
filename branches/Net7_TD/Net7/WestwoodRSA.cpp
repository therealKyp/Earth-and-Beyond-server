// WestwoodRSA.cpp

#include "Net7.h"
#include "WestwoodRSA.h"

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rsa.h>
#include <openssl/engine.h>


WestwoodRSA::WestwoodRSA()
{
	p = BN_new();
	q = BN_new();
	N = BN_new();
	d = BN_new();
	e = BN_new();
	temp = BN_CTX_new();
	if(!p || !q || !N || !d || !e || !temp)
		g_LogError.Print("ABORT: Failed to allocate BIGNUMs in WestwoodRSA.");
 
	BN_dec2bn(&p, WWRSA_p);
	BN_dec2bn(&q, WWRSA_q);
	BN_dec2bn(&N, WWRSA_N);
	BN_dec2bn(&d, WWRSA_d);
	BN_dec2bn(&e, WWRSA_e);
}

WestwoodRSA::~WestwoodRSA()
{
	BN_free(p);
	BN_free(q);
	BN_free(N);
	BN_free(d);
	BN_free(e);
	BN_CTX_free(temp);
}

unsigned int WestwoodRSA::GetModulus(unsigned char **p_out_buffer)
{
    unsigned char * out_buffer = *p_out_buffer;
    *out_buffer++ = 0x00;
    *out_buffer++ = 0x00;
    *out_buffer++ = 0x00;
    *out_buffer++ = 0x41;

	*out_buffer++ = 0x00;
	BN_bn2bin(N, out_buffer);
	out_buffer += 64;

    *p_out_buffer = out_buffer;
    return (69);
}

unsigned int WestwoodRSA::GetPublicExponent(unsigned char **p_out_buffer)
{
    unsigned char * out_buffer = *p_out_buffer;
    *out_buffer++ = 0x00;
    *out_buffer++ = 0x00;
    *out_buffer++ = 0x00;
    *out_buffer++ = 0x01;

	BN_bn2bin(e, out_buffer);

	out_buffer++;
    *p_out_buffer = out_buffer;
    return (5);
}

bool WestwoodRSA::Encrypt(unsigned char *in_buffer, unsigned int length, unsigned char *out_buffer)
{
    bool success = false;
    if (length > 0)
    {
        BIGNUM M;
        BIGNUM C;
        BN_init(&M);
        BN_init(&C);
        unsigned int offset = 0;
        unsigned int remaining = length;
        while (remaining >= WWRSA_BLOCK_SIZE)
        {
            BN_bin2bn(in_buffer + offset, WWRSA_BLOCK_SIZE, &M);
            BN_mod_exp(&C, &M, e, N, temp);
            BN_bn2bin(&C, out_buffer + offset);
            offset += WWRSA_BLOCK_SIZE;
            remaining -= WWRSA_BLOCK_SIZE;
        }
        if (remaining > 0)
        {
            unsigned char buffer[WWRSA_BLOCK_SIZE];
            memcpy(buffer, in_buffer + offset, remaining);
            memset(buffer + remaining, 0, WWRSA_BLOCK_SIZE - remaining);
            BN_bin2bn(buffer, WWRSA_BLOCK_SIZE, &M);
            BN_mod_exp(&C, &M, e, N, temp);
            BN_bn2bin(&C, out_buffer + offset);
        }
        success = true;
    }

    return success;
}

bool WestwoodRSA::Decrypt(unsigned char *in_buffer, unsigned int length, unsigned char *out_buffer)
{
    bool success = false;
    unsigned int blocks = length / WWRSA_BLOCK_SIZE;
    if (blocks * WWRSA_BLOCK_SIZE == length)
    {
        BIGNUM C;
        BIGNUM M;
        BN_init(&C);
        BN_init(&M);
        unsigned int offset = 0;
        for (unsigned int i = 0; i < blocks; i++)
        {
            BN_bin2bn(in_buffer + offset, WWRSA_BLOCK_SIZE, &C);
            BN_mod_exp(&M, &C, d, N, temp);
            
            int size = BN_bn2bin(&M, out_buffer + offset);
			
			/* Need this cause BN_bn2bin will yield at the beginning
			   of the buffer, unlike Integer::Encode which, because
			   it is passed size as a parameter (WWRSA_BLOCK_SIZE), 
			   will place the data at the end of the buffer.   
			 */			
            if(size < WWRSA_BLOCK_SIZE && size > 0)
            {
            	unsigned char temp[WWRSA_BLOCK_SIZE];
            	int diff = WWRSA_BLOCK_SIZE - size;
            	
            	memcpy(temp, (out_buffer + offset), size);
            	memset((out_buffer + offset), 0, WWRSA_BLOCK_SIZE - size);
            	memcpy((out_buffer + offset + diff), temp, size);
            }
            
            offset += WWRSA_BLOCK_SIZE;
        }
        success = true;
    }

    return success;
}

