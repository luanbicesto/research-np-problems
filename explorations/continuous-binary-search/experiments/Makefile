CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c11
LDFLAGS = -lm

# Benchmark original (funções monótonas)
SRCS_BENCH = benchmark.c cbs_algorithm.c test_functions.c itp_method.c cbs_itp.c
OBJS_BENCH = $(SRCS_BENCH:.c=.o)

# Benchmark completo (funções não-monótonas)
SRCS_COMPLETE = benchmark_complete.c cbs_algorithm.c test_functions.c itp_method.c cbs_itp.c cbs_complete.c
OBJS_COMPLETE = $(SRCS_COMPLETE:.c=.o)

.PHONY: all clean run run_complete

all: benchmark benchmark_complete

benchmark: $(OBJS_BENCH)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

benchmark_complete: $(OBJS_COMPLETE)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: benchmark
	./benchmark

run_complete: benchmark_complete
	./benchmark_complete

clean:
	rm -f *.o benchmark benchmark_complete results_summary.csv results_history.csv

# Dependências
benchmark.o: benchmark.c cbs_algorithm.h test_functions.h itp_method.h cbs_itp.h
benchmark_complete.o: benchmark_complete.c cbs_complete.h cbs_algorithm.h test_functions.h
cbs_algorithm.o: cbs_algorithm.c cbs_algorithm.h
test_functions.o: test_functions.c test_functions.h
itp_method.o: itp_method.c itp_method.h cbs_algorithm.h
cbs_itp.o: cbs_itp.c cbs_itp.h cbs_algorithm.h itp_method.h
cbs_complete.o: cbs_complete.c cbs_complete.h cbs_itp.h cbs_algorithm.h
