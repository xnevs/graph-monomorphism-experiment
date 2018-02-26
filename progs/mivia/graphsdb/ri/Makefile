CXX = g++
CXX_FLAGS = -std=c++14 -O3 -march=native

ifndef EXPERIMENT_ROOT
$(error EXPERIMENT_ROOT is not set)
endif

INC = -I$(EXPERIMENT_ROOT)/algs/RI3.6-release/include -I$(EXPERIMENT_ROOT)/algs/RI3.6-release/rilib

.PHONY: all
all: a.out

a.out: main.cpp
	$(CXX) $(CXX_FLAGS) $(INC) -o $@ main.cpp

.PHONY: clean
clean:
	rm a.out
