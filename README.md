# OSP_A2
Daniel Azoor - s3944498

Note: RR is still WIP as of submission, sorry

COMPILATION AND RUNNING INSTRUCTIONS:

"make all" will compile all three algorithms at the same time, into the executables
"fifo", "sjf" and "rr".

"make fifo", "make sjf" and "make rr" will compile each program individually, with the same executable
names as listed above.

all three programs compile on school servers with the flags -Wall -Werror -std=c++20 provided
devtoolset-11 is enabled via this command: $scl enable devtoolset-11 bash

"make clean" will clear all executables from the directory