#include <cstdint>
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
  in.close();
  in.open(h_filename,std::ios::in|std::ios::binary);
  auto n = read_word(in);
  in.close();

  if (m <= n) {
    return 0;
  } else {
    return 1;
  }
}
