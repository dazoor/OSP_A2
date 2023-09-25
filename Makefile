
.default: all

all: fifo sfj

clean:
	find . -name '*.o' -type f -delete
	find . -name 'fifo' -type f -delete
	find . -name 'sfj' -type f -delete

fifo: fifo.cpp
	g++ -Wall -Werror -std=c++20 -O -o $@ $^

sfj: sfj.cpp
	g++ -Wall -Werror -std=c++20 -O -o $@ $^