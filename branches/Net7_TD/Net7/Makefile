TARGET = ../bin/Net7

BASE := $(wildcard *.*)
SRCS := $(filter %.cpp,$(BASE))
OBJS := $(patsubst %.cpp,%.o,$(filter %.cpp,$(SRCS)))
LIBS = -pthread -lssl -lcrypto -ldl

CPPFLAGS = -g -Wall
LDFLAGS =

CC = g++ $(CPPFLAGS)
LD = g++ $(LDFLAGS)

all: $($OBJS) $(TARGET)

.cpp.o:
	$(CC) -c $< -o $@

$(TARGET): $(OBJS)
	$(LD) $(OBJS) -o $@ $(LIBS)

clean:
	rm -f $(OBJS) $(TARGET)

