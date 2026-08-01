CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c11
LDFLAGS = -lm

all: benchmark_nd

benchmark_nd: benchmark_nd.c cbs_nd.c cbs_nd.h
	$(CC) $(CFLAGS) -o $@ benchmark_nd.c cbs_nd.c $(LDFLAGS)

clean:
	rm -f benchmark_nd *.o

.PHONY: all clean
