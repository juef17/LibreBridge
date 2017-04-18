export CC=g++
export CFLAGS=-std=c++17

all:
	+$(MAKE) -C src

clean:
	rm -f LibreBridge
	find . -name "*.o" -type f -delete