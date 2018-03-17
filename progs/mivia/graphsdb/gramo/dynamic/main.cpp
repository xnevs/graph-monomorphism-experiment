#include <cstdint>

#include <iostream>
#include <fstream>
#include <chrono>

#include <read_amalfi.h>
#include <simple_adjacency_list.h>
#include <predefined.h>

int main(int argc, char * argv[]) {
  char const * g_filename = argv[1];
  char const * h_filename = argv[2];

  std::ifstream in{g_filename,std::ios::in|std::ios::binary};
  auto g = read_amalfi<simple_adjacency_list<uint16_t>>(in);
  in.close();
  in.open(h_filename,std::ios::in|std::ios::binary);
  auto h = read_amalfi<simple_adjacency_list<uint16_t>>(in);
  in.close();

  std::chrono::time_point<std::chrono::steady_clock> start, end;
  std::chrono::milliseconds elapsed;

  start = std::chrono::steady_clock::now();

  int count = 0;
  dynamic_ind(
      g,
      h,
      [&count](auto const & S) {++count; return true;},
      [](auto x, auto y) {return true;},
      [](auto x0, auto x1, auto y0, auto y1) {return true;});

  end = std::chrono::steady_clock::now();
  elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  std::cout << count << "," << elapsed.count() << std::endl;
}
