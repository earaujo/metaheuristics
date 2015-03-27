CC=gcc
BUILDPATH=$(realpath ..)/evolve/build
CC_INCLUDES=-I$(BUILDPATH)/deps/include/ -I$(BUILDPATH)/include/
CC_LIBRARIES=-L$(BUILDPATH)/deps/lib/ -L$(BUILDPATH)/lib/ -lgsl -lgslcblas -lm -levolve
CC_OPTIONS=

all:
	$(CC) jobshop/jobshop.c -o bin/jobshop $(CC_INCLUDES) $(CC_LIBRARIES) $(CC_OPTIONS)
clean:
	rm -f bin/jobshop
