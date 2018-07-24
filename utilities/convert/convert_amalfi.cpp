#include <cstdint>

#include <fstream>
#include <iostream>

uint16_t read_word(std::istream & in) {
  uint16_t x = static_cast<unsigned char>(in.get());
  x |= static_cast<uint16_t>(in.get()) << 8;
  return x;
}

int main(int argc, char * argv[]) {
  char const * g_filename = argv[1];

  std::ifstream in(g_filename,std::ios::in|std::ios::binary);
  
  auto n = read_word(in);
  
  std::cout << n << std::endl;

  for (decltype(n) u=0; u<n; ++u) {
    auto cnt = read_word(in);
    std::cout << cnt;
    for (decltype(cnt) i=0; i<cnt; ++i) {
      std::cout << " " << read_word(in);
    }
    std::cout << std::endl;
  }

  in.close();
}