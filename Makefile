
.default: all

all: fifo

clean:
	find . -name '*.o' -type f -delete
	find . -name 'fifo' -type f -delete

fifo: fifo.cpp
	g++ -Wall -Werror -std=c++20 -O -o $@ $^
