#ifndef READ_AMALFI_H_
#define READ_AMALFI_H_

#include <cstdint>
#include <istream>

#include <argedit.hpp>
#include <argraph.hpp>

class BinaryGraphLoader : public ARGEdit<int,Empty> {
 public:
  BinaryGraphLoader(istream & in) {
    int cnt = read_word(in);
    if(cnt <= 0) {
      cnt = 0;
      return;
    }
    int i;
    for(i=0; i<cnt; ++i) {
      int nattr = 0;
      this->InsertNode(nattr);
    }
    for(i=0; i<cnt; ++i) {
      int ecount, j;
      ecount = read_word(in);
      for(j=0; j<ecount; ++j) {
        node_id v = read_word(in);
        Empty eattr;
        this->InsertEdge(i,v,eattr);
      }
    }
  }
 private:
  uint16_t read_word(std::istream & in) {
    uint16_t x = static_cast<unsigned char>(in.get());
    x |= static_cast<uint16_t>(in.get()) << 8;
    return x;
  }
};

#endif  // READ_AMALFI_H_
