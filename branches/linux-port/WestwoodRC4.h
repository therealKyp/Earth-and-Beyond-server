// WestwoodRC4.h

#ifndef _WESTWOOD_RC4_H_INCLUDED_
#define _WESTWOOD_RC4_H_INCLUDED_

class WestwoodRC4
{
public:
    WestwoodRC4();
    virtual ~WestwoodRC4();

public:
   void PrepareKey(unsigned char *key_data_ptr, int key_data_len);
   void RC4(unsigned char *buffer_ptr, long buffer_len);

private:
	static void SwapByte(unsigned char *a, unsigned char *b);

private:
	unsigned char m_State[256];
	unsigned char m_x;
	unsigned char m_y;
};

#endif // _WESTWOOD_RC4_H_INCLUDED_

