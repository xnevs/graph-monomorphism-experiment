CXX = g++
CXX_FLAGS = -std=c++14 -O3 -march=native

ifndef EXPERIMENT_ROOT
$(error EXPERIMENT_ROOT is not set)
endif

INC = -I$(EXPERIMENT_ROOT)/algs/vflib-2.0.6/include
LIB = -L$(EXPERIMENT_ROOT)/algs/vflib-2.0.6/lib

libs = -lvf

.PHONY: all
all: a.out

a.out: main.cpp
	$(CXX) $(CXX_FLAGS) $(INC) $(LIB) -o $@ main.cpp $(libs)

.PHONY: clean
clean:
	rm a.out
