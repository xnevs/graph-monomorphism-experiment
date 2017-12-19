CXX = g++
CXX_FLAGS = -std=c++14 -O3 -march=native -mtune=native

ifndef EXPERIMENT_ROOT
$(error EXPERIMENT_ROOT is not set)
endif

IDIR = $(EXPERIMENT_ROOT)/algs/bullimp/include

.PHONY: all
all: a.out

a.out: main.cpp
	$(CXX) $(CXX_FLAGS) -I$(IDIR) -o $@ main.cpp

.PHONY: clean
clean:
	rm a.out
