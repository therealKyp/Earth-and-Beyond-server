#ifndef _PACKET_METHODS_H_INCLUDED_
#define _PACKET_METHODS_H_INCLUDED_

template <typename T>
static void AddData(unsigned char *packet, T mydata, int &index)
{
	*((T *) &packet[index]) = mydata;
	index += sizeof(T);
}

/* Adds the string only */
static void AddDataS(unsigned char *packet, const char *mydata, int &index)
{
	memcpy(&packet[index], mydata, strlen(mydata));
	index += strlen(mydata);
}

/* Adds the string with a null terminating charachter */
static void AddDataSN(unsigned char *packet,  const char *mydata, int &index)
{
	memcpy(&packet[index], mydata, strlen(mydata) + 1);
	index += strlen(mydata) + 1;
}

/* Adds the length (short) of the string followed by the string itself */
static void AddDataLS(unsigned char *packet, const char *mydata, int &index)
{
	AddData(packet,short(strlen(mydata)),index);
    memcpy(&packet[index], mydata, strlen(mydata));
    index += strlen(mydata);
}

/* Same as above but the strings null terminated charachter is added */
static void AddDataLSN(unsigned char *packet, const char *mydata, int &index)
{
	AddData(packet,short(strlen(mydata) + 1),index);
    memcpy(&packet[index], mydata, strlen(mydata) + 1);
    index += strlen(mydata) + 1;
}

/* Flip the byte order of the data */
static void AddDataFlip4(unsigned char *packet, long mydata, int &index)
{
	*((long *) &packet[index]) = ntohl(mydata);
	index += 4;
}

/* Flip the byte order of the data */
static void AddDataFlip2(unsigned char *packet, short mydata, int &index)
{
	*((short *) &packet[index]) = ntohs(mydata);
	index += 2;
}

/* Add another buffer */
static void AddBuffer(unsigned char *packet, unsigned char *buffer, int length, int &index)
{
	memcpy(&packet[index], buffer, length);
	index += length;
}

/* Extract a string from an 'AddDataLS' encoded string */
static void ExtractDataLS(unsigned char *packet, char *buffer, int &index)
{
    short string_length = *((short *) &packet[index]);
    index += 2;
    memcpy(buffer, &packet[index], string_length);
    buffer[string_length] = 0;
    index += string_length;
}

static long ExtractLong(unsigned char *packet, int &index)
{
    index += 4;
    return ((long)&packet[index-4]);
}

static short ExtractShort(unsigned char *packet, int &index)
{
    index += 2;
    //return ((short)&packet[index-2]);
    return *((short*) packet);
}

static float ExtractFloat(unsigned char *packet, int &index)
{
    float result = *((float *) &packet[index]);
    index += 4;
    return result;
}

#endif
