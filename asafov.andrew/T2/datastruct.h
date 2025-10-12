#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H
#include <complex>
#include <string>
#include <iostream>
namespace asafov
{
  struct DataStruct
  {
    unsigned long long key1 = 0;            //[ULL BIN]
    std::complex< double > key2 = {0.0, 0.0}; //[CMP LSP]
    std::string key3;
  };

  bool operator<(const DataStruct &a, const DataStruct &b);
  std::istream& operator>>(std::istream& is, DataStruct& data);
  std::ostream& operator<<(std::ostream& os, const DataStruct& data);
}
#endif
