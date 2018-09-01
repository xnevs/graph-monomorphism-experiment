#include <cstdint>

#include <iostream>
#include <fstream>

#include <queue>
#include <vector>
#include <algorithm>

#include <sics/read_amalfi.h>
#include <sics/adjacency_listmat.h>

template <typename G>
bool connected(G const & g) {
  std::vector<bool> visited(g.num_vertices(), false);
  std::queue<typename G::index_type> q;
  q.push(0);
  visited[0] = true;
  while(!q.empty()) {
    auto u = q.front();
    q.pop();
    for (auto he : g.out_edges(u)) {
      auto v = he.target;
      if (!visited[v]) {
        q.push(v);
        visited[v] = true;
      }
    }
    for (auto he : g.in_edges(u)) {
      auto v = he.target;
      if (!visited[v]) {
        q.push(v);
        visited[v] = true;
      }
    }
  }
  for (typename G::index_type u=0; u<g.num_vertices(); ++u) {
    if (!visited[u]) {
      return false;
    }
  }
  return true;
}

int main(int argc, char * argv[]) {
  using namespace sics;

  char const * g_filename = argv[1];
  char const * h_filename = argv[2];

  std::ifstream in{g_filename,std::ios::in|std::ios::binary};
  auto g = read_amalfi<adjacency_listmat<uint16_t, bidirectional_tag>>(in);
  in.close();

  if (connected(g)) {
    return 0;
  } else {
    return 1;
  }
}
