// WestwoodRC4.cpp

#include "stdafx.h"
#include "WestwoodRC4.h"

WestwoodRC4::WestwoodRC4()
{
	memset(m_State, 0, sizeof(m_State));
}

WestwoodRC4::~WestwoodRC4()
{
}

void WestwoodRC4::PrepareKey(unsigned char *key_data_ptr, int key_data_len)
{
    unsigned char index1;
    unsigned char index2;
    unsigned char* state;
    short counter;

    state = &m_State[0];
    for (counter = 0; counter < 256; counter++)
    {
        state[counter] = (unsigned char) counter;
    }

    m_x = 0;
    m_y = 0;
    index1 = 0;
    index2 = 0;
    for(counter = 0; counter < 256; counter++)
    {
         index2 = (key_data_ptr[index1] + state[counter] + index2) % 256;
         SwapByte(&state[counter], &state[index2]);
         index1 = (index1 + 1) % key_data_len;
    }
}

void WestwoodRC4::RC4(unsigned char *buffer_ptr, int buffer_len)
{
    unsigned char x;
    unsigned char y;
    unsigned char* state;
    unsigned char xorIndex;
    short counter;

    x = m_x;
    y = m_y;

    state = &m_State[0];
    for(counter = 0; counter < buffer_len; counter ++)
    {
         x = (x + 1) % 256;
         y = (state[x] + y) % 256;
         SwapByte(&state[x], &state[y]);
         xorIndex = (state[x] + state[y]) % 256;
         buffer_ptr[counter] ^= state[xorIndex];
     }
     m_x = x;
     m_y = y;
}

void WestwoodRC4::SwapByte(unsigned char *a, unsigned char *b)
{
    unsigned char swapByte = *a;
    *a = *b;
    *b = swapByte;
}
