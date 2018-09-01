#include <cstdint>
#include <iostream>
#include <fstream>

uint16_t read_word(std::istream & in) {
  uint16_t x = static_cast<unsigned char>(in.get());
  x |= static_cast<uint16_t>(in.get()) << 8;
  return x;
}

int main(int argc, char * argv[]) {
  char const * g_filename = argv[1];
  char const * h_filename = argv[2];

  std::ifstream in{g_filename,std::ios::in|std::ios::binary};
  auto m = read_word(in);
  for (decltype(m) u=0; u<m; ++u) {
    auto cnt = read_word(in);
    for (decltype(cnt) i=0; i<cnt; ++i) {
      auto v = read_word(in);
      if (v == u) {
        std::cout << "GOTCH!" << std::endl;
        return 0;
      }
    }
  }
}
