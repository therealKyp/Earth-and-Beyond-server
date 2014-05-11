/* Where this is going: Yes I know, creating and destroying packet objects is "slow" bad for the heap. This class is 
the start of a circular buffer class that can be attached to a player object and reused for all packet calls */

//do we actually want to attach anything to player objects for sending packets?
//objects should be connection agnostic (other than the player->connection link, and that should be encapsulated), 
//all connection stuff should be handled in the connection or sector manager
//It's too confusing to have objects sending their own packets on an independant separately threaded system

#include "PacketBuffer.h"
#include <assert.h>
#include <string.h>

using namespace std;

PacketBuffer::PacketBuffer()
{
	rPos = 0;
	rErr = false;
	buf.reserve(128); // reserve some space by default
}

PacketBuffer::PacketBuffer(int reserve)
{
	rPos = 0;
	rErr = false;
	buf.reserve(reserve); // reserve a specified amount of space
}

PacketBuffer::~PacketBuffer()
{
}

void PacketBuffer::addBool(bool b) { buf.push_back(b?1:0);}
void PacketBuffer::addByte(unsigned char b) { buf.push_back(b); }

void PacketBuffer::addLong(long l) {
	buf.push_back(l & 0xff);
	buf.push_back((l >> 8) & 0xff);
	buf.push_back((l >> 16) & 0xff);
	buf.push_back((l >> 24) & 0xff);
}

void PacketBuffer::addFloat(float f) {
	union { float f; long l; } u = {f};
	addLong(u.l);
}

void PacketBuffer::addString(const string& str) {
	long len = str.size()+1;

	buf.insert(
		buf.end(),
		reinterpret_cast<const unsigned char*>(str.c_str()),
		reinterpret_cast<const unsigned char*>(str.c_str()+len)
	);
}

void PacketBuffer::addVector(const vector<unsigned char>& raw)
{
	buf.insert(buf.end(), raw.begin(), raw.end());
}

void PacketBuffer::addRawData(void *data, long len) {
	unsigned char *c = (unsigned char *)data;
	buf.insert(buf.end(),c,c+len);
}

unsigned char *PacketBuffer::getBuffer() { return &buf[rPos]; }

long PacketBuffer::size() { return buf.size(); }
long PacketBuffer::remaining() { return size()-rPos; }
bool PacketBuffer::readError() { return rErr; }

