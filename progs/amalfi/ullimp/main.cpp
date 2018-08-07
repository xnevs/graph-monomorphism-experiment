#include <cstring>

#include <iostream>
#include <fstream>
#include <chrono>

#include <digraph.hpp>
#include <map.hpp>
#include <order.hpp>
#include <ullimp.hpp>

int main(int argc, char * argv[]) {
  Digraph g(argv[1]);
  Digraph h(argv[2]);

  std::chrono::time_point<std::chrono::steady_clock> start, end;
  std::chrono::milliseconds elapsed;

  start = std::chrono::steady_clock::now();

  UllImp alg(g, h);

  alg.find(true);

  end = std::chrono::steady_clock::now();
  elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  std::cout << alg.isoCount << "," << elapsed.count() << std::endl;

  return 0;
}
