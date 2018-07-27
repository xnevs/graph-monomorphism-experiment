CXX = g++
CXX_FLAGS = -std=c++14 -O3 -march=native -DNDEBUG

.PHONY: all
all: a.out

a.out: main.cpp
	$(CXX) $(CXX_FLAGS) -o $@ main.cpp

.PHONY: clean
clean:
	rm a.out
