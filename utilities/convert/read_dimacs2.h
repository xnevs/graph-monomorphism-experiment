#ifndef GMCS_READ_DIMACS2_H_
#define GMCS_READ_DIMACS2_H_

#include <istream>
#include <string>
#include <sstream>

template <typename G>
G read_dimacs2(std::istream & in) {
  int n;

  std::string line;
  std::string ignore;
  while (std::getline(in, line)) {
    std::istringstream iss{line};
    char type;
    iss >> type;
    if (type == 'p') {
      iss >> ignore >> n >> ignore;
      break;
    }
  }

  G g(n);

  while (std::getline(in, line)) {
    std::istringstream iss{line};
    char type;
    iss >> type;

    int u;
    int v;
    if (type == 'e') {
      iss >> u >> v;
      g.add_edge(u-1, v-1);
    }
  }

  return g;
}

#endif  // GMCS_READ_DIMACS2_H_
