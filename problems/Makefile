CC=gcc
CFLAGS=-I../ -lgsl -lgslcblas -lm
all:
	$(CC) knapsack/knapsack.c -o bin/knapsack $(CFLAGS)
	$(CC) tsp/tsp.c -o bin/tsp $(CFLAGS)
clean:
	rm -f bin/knapsack
	rm -f bin/tsp

