
.default: all

all: fifo sjf rr

clean:
	find . -name '*.o' -type f -delete
	find . -name 'fifo' -type f -delete
	find . -name 'sjf' -type f -delete
	find . -name 'rr' -type f -delete

fifo: fifo.cpp
	g++ -Wall -Werror -std=c++20 -O -o $@ $^

sjf: sjf.cpp
	g++ -Wall -Werror -std=c++20 -O -o $@ $^

rr: rr.cpp
	g++ -Wall -Werror -std=c++20 -O -o $@ $^