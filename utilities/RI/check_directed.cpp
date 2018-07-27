#include <cstddef>

#include <iostream>
#include <string>

#include <sics/adjacency_listmat.h>
#include <sics/read_gf.h>

int main() {
  using namespace sics;
  
  auto g = read_gf<adjacency_listmat<uint16_t, bidirectional_tag, std::string>>(std::cin);

  bool some = false;
  bool all = true;
  for(uint16_t u=0; u!=g.num_vertices(); ++u) {
    for(auto oe : g.out_edges(u)) {
      auto v = oe.target;
      if (g.edge(v, u)) {
        some = true;
      } else {
        all = false;
      }
    }
  }

  if (some) {
    std::cout << (some ? "some" : "none") << " " << (all ? "all" : "notall") << " ";
    return 1;
  } else {
    return 0;
  }
}
