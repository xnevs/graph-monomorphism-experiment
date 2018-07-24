#include <cstdint>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char * argv[]) {
  using index_type = uint16_t;
  
  char const * g_filename = argv[1];

  std::ifstream in{g_filename,std::ios::in|std::ios::binary};
  
  index_type n;
  in >> n;
  std::cout << n << std::endl;
  
  std::string ignore;
  for (index_type u=0; u<n; ++u) {
    in >> ignore >> ignore;
  }
  
  std::vector<std::vector<index_type>> adjacency_list(n);
  for (index_type i=0; i<n; ++i) {
    index_type cnt;
    in >> cnt;
    for (index_type j=0; j<cnt; ++j) {
      index_type u, v;
      in >> u >> v;
      adjacency_list[u].push_back(v);
    }
  }
  
  for (auto adj : adjacency_list) {
    std::cout << adj.size();
    for (auto v : adj) {
      std::cout << " " << v;
    }
    std::cout << std::endl;
  }
  
  in.close();
}