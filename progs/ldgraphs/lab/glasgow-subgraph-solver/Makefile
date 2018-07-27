CXX = g++
CXX_FLAGS = -std=c++17 -O3 -march=native -pthread -DNDEBUG

ifndef EXPERIMENT_ROOT
$(error EXPERIMENT_ROOT is not set)
endif

INC = -I$(EXPERIMENT_ROOT)/algs/glasgow-subgraph-solver
libs = -lboost_thread -lboost_system -lboost_program_options -lpthread

objs = $(EXPERIMENT_ROOT)/algs/glasgow-subgraph-solver/intermediate/solve_subgraph_isomorphism/fixed_bit_set.o $(EXPERIMENT_ROOT)/algs/glasgow-subgraph-solver/intermediate/solve_subgraph_isomorphism/solver.o $(wildcard $(EXPERIMENT_ROOT)/algs/glasgow-subgraph-solver/intermediate/solve_subgraph_isomorphism/formats/*.o)

.PHONY: all
all: a.out

a.out: main.cpp
	$(CXX) $(CXX_FLAGS) $(INC) -o $@ main.cpp $(objs) $(libs)

.PHONY: clean
clean:
	rm a.out
