CXX = g++
CXX_FLAGS = -std=c++14 -O3 -march=native

ifndef EXPERIMENT_ROOT
$(error EXPERIMENT_ROOT is not set)
endif

INC = -I$(EXPERIMENT_ROOT)/algs/vflib-3.0.1/include -I$(EXPERIMENT_ROOT)/extensions/vflib-3.0.1/include

.PHONY: all
all: a.out

a.out: main.cpp
	$(CXX) $(CXX_FLAGS) $(INC) -o $@ main.cpp

.PHONY: clean
clean:
	rm a.out
