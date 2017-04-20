export CC=g++
export CFLAGS=-std=c++17 -Wall -Wextra -pedantic

all:
	find . -name "*.o" -type f -delete
	+$(MAKE) -C src

clean:
	rm -f LibreBridge
	find . -name "*.o" -type f -delete