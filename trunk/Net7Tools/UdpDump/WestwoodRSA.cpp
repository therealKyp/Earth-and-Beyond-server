// WestwoodRSA.cpp

#include "stdafx.h"
#include "WestwoodRSA.h"
#include "integer.h"

using namespace std;
using namespace CryptoPP;

WestwoodRSA::WestwoodRSA()
{
}

WestwoodRSA::~WestwoodRSA()
{
}

unsigned int WestwoodRSA::GetModulus(unsigned char **p_out_buffer)
{
    unsigned char * out_buffer = *p_out_buffer;
    *out_buffer++ = 0x00;
    *out_buffer++ = 0x00;
    *out_buffer++ = 0x00;
    *out_buffer++ = 0x41;
    Integer N(WWRSA_N);
    N.Encode(out_buffer, 65);
	out_buffer += 65;
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
    Integer e(WWRSA_e);
    e.Encode(out_buffer, 1);
	out_buffer++;
    *p_out_buffer = out_buffer;
    return (5);
}

bool WestwoodRSA::Encrypt(unsigned char *in_buffer, unsigned int length, unsigned char *out_buffer)
{
    bool success = false;
    if (length > 0)
    {
        Integer e(WWRSA_e);
        Integer N(WWRSA_N);
        Integer M;
        Integer C;
        unsigned int offset = 0;
        unsigned int remaining = length;
        while (remaining >= WWRSA_BLOCK_SIZE)
        {
            M.Decode(in_buffer + offset, WWRSA_BLOCK_SIZE);
            C = a_exp_b_mod_c(M, e, N);
            C.Encode(out_buffer + offset, WWRSA_BLOCK_SIZE);
            offset += WWRSA_BLOCK_SIZE;
            remaining -= WWRSA_BLOCK_SIZE;
        }
        if (remaining > 0)
        {
            unsigned char buffer[WWRSA_BLOCK_SIZE];
            memcpy(buffer, in_buffer + offset, remaining);
            memset(buffer + remaining, 0, WWRSA_BLOCK_SIZE - remaining);
            M.Decode(buffer, WWRSA_BLOCK_SIZE);
            C = a_exp_b_mod_c(M, e, N);
            C.Encode(out_buffer + offset, WWRSA_BLOCK_SIZE);
        }
        success = true;
    }

    return success;
};

bool WestwoodRSA::Decrypt(unsigned char *in_buffer, unsigned int length, unsigned char *out_buffer)
{
    bool success = false;
    unsigned int blocks = length / WWRSA_BLOCK_SIZE;
    if (blocks * WWRSA_BLOCK_SIZE == length)
    {
        Integer d(WWRSA_d);
        Integer N(WWRSA_N);
        Integer C;
        Integer M;
        unsigned int offset = 0;
        for (unsigned int i = 0; i < blocks; i++)
        {
            C.Decode(in_buffer + offset, WWRSA_BLOCK_SIZE);
            M = a_exp_b_mod_c(C, d, N);
            M.Encode(out_buffer + offset, WWRSA_BLOCK_SIZE);
            offset += WWRSA_BLOCK_SIZE;
        }
        success = true;
    }

    return success;
};

