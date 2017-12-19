CXX = g++
CXX_FLAGS = -std=c++14 -O3 -march=native -mtune=native

ifndef EXPERIMENT_ROOT
$(error EXPERIMENT_ROOT is not set)
endif

IDIR = $(EXPERIMENT_ROOT)/algs/vflib-2.0.6/include
LDIR = $(EXPERIMENT_ROOT)/algs/vflib-2.0.6/lib

libs = -lvf

.PHONY: all
all: a.out

a.out: main.cpp
	$(CXX) $(CXX_FLAGS) -I$(IDIR) -L$(LDIR) -o $@ main.cpp $(libs)

.PHONY: clean
clean:
	rm a.out
