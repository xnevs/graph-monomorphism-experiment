CXX = g++
CXX_FLAGS = -std=c++17 -O3 -march=native -DNDEBUG

ifndef EXPERIMENT_ROOT
$(error EXPERIMENT_ROOT is not set)
endif

INC = -I$(EXPERIMENT_ROOT)/algs/sics/include

.PHONY: all
all: a.out

a.out: main.cpp
	$(CXX) $(CXX_FLAGS) $(INC) -DSICS_ALGORITHM='backtracking_degreeprune_ind' -DSICS_DO_STATS -o $@ main.cpp
	
.PHONY: clean
clean:
	rm a.out
