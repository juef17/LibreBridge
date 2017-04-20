SRC_SOURCES := $(wildcard *.cpp)
SRC_OBJECTS := $(SRC_SOURCES:.cpp=.o)
AIPLAYERS_SOURCES := $(wildcard AIPlayers/*.cpp)
AIPLAYERS_OBJECTS := $(AIPLAYERS_SOURCES:.cpp=.o)

all:
	$(CC) $(CFLAGS) $(SRC_SOURCES) -c
	+$(MAKE) -C AIPlayers
	$(CC) $(CFLAGS) $(SRC_OBJECTS) $(AIPLAYERS_OBJECTS) -o ../LibreBridge

#all: LibreBridge
#
#LibreBridge: test1.o test2.o LibreBridge.o
#	g++ test1.o test2.o -o LibreBridge

#test1.o: test1.cpp
#	$(CC) $(CFLAGS) -c test1.cpp

#test2.o: test2.cpp
#	$(CC) $(CFLAGS) -c test2.cpp

#LibreBridge.o: LibreBridge.cpp
#	$(CC) $(CFLAGS) -c LibreBridge.cpp