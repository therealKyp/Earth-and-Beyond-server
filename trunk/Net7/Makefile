TARGET = Net7

BASE := $(wildcard *.*)
SRCS := $(filter %.cpp,$(BASE))
OBJS := $(patsubst %.cpp,%.o,$(filter %.cpp,$(SRCS)))
LIBS = -pthread -lssl -lcrypto -ldl -lcryptopp

CPPFLAGS = -g -I/usr/local/ssl/include -DUSE_OPENSSL -O2 -march=athlon64
LDFLAGS = -L../libs

CC = g++ $(CPPFLAGS)
LD = g++ $(LDFLAGS)

all: $($OBJS) $(TARGET)

.cpp.o:
	$(CC) -c $< -o $@

$(TARGET): $(OBJS)
	$(LD) $(OBJS) -o $@ $(LIBS)

clean:
	rm -f $(OBJS) $(TARGET)

