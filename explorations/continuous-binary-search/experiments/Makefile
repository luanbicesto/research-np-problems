CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c11
LDFLAGS = -lm

SRCS = benchmark.c cbs_algorithm.c test_functions.c
OBJS = $(SRCS:.c=.o)
TARGET = benchmark

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET) results_summary.csv results_history.csv

# Dependências
benchmark.o: benchmark.c cbs_algorithm.h test_functions.h
cbs_algorithm.o: cbs_algorithm.c cbs_algorithm.h
test_functions.o: test_functions.c test_functions.h
