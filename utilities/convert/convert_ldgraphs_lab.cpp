#include <cstdint>

#include <fstream>
#include <iostream>
#include <vector>

int main(int argc, char * argv[]) {
  using index_type = uint16_t;
  using label_type = uint8_t;
  
  char const * g_filename = argv[1];

  std::ifstream in{g_filename,std::ios::in|std::ios::binary};
  
  index_type n;
  in >> n;
  std::cout << n << std::endl;
  
  std::vector<label_type> labels(n);
  for (index_type i=0; i<n; ++i) {
    index_type u;
    label_type label;
    in >> u >> label;
    labels[u] = label;
  }
  for (auto label : labels) {
    std::cout << label << std::endl;
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