#pragma warning (disable:4786)

#ifndef PACKET_BUFFER_H
#define PACKET_BUFFER_H

#include <string>
#include <vector>
using namespace std;

class PacketBuffer {
private:
	long rPos;
	bool rErr;
	vector<unsigned char> buf;

public:
	PacketBuffer();
	PacketBuffer(int reserve);
	~PacketBuffer();

	void addBool(bool b);
	void addByte(unsigned char b);
	void addLong(long l);
	void addFloat(float f);
	void addString(const std::string& s);
	void addVector(const std::vector<unsigned char>& raw);
	void addRawData(void *data, long l);

	unsigned char *getBuffer();
	
	long size();
	long remaining();
	bool readError();
};

#endif
