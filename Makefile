# Top-level Makefile

.PHONY: scheduling clean

scheduling:
	sh build-evolve.sh
	$(MAKE) -C scheduling

all: scheduling

default: all

clean:
	rm -rf scheduling/bin
	rm -rf scheduling/jobshop/exercises/results
	rm -rf scheduling/ci/results
