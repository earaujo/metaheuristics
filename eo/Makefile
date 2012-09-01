CC=g++
CFLAGS=-I. -I/usr/local/include/eo -leo -leoutils
GUIFLAGS=$(pkg-config --cflas --libs gtkmm)
all: console
	@echo "Build successfully"
console:
	$(CC) knapsack/console.cpp -o bin/mknap $(CFLAGS)
