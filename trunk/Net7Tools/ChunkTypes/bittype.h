//fake stub for bittype.h that we dont have
//going to stick mising stuff in here
#include <stdlib.h>
#define MAX(a,b)  (((a) > (b)) ? (a) : (b))
#define MIN(a,b)  (((a) < (b)) ? (a) : (b))
struct IOVector3Struct {
float X; float Y; float Z;
};

struct IOQuaternionStruct {
float X; float Y; float Z; float W;
};

struct chunk {
long type;
long size;
};

typedef unsigned long uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;
typedef signed long sint32;
typedef signed short sint16;
typedef float float32;
