#ifndef DATASTRUCT_HPP
#define DATASTRUCT_HPP

#include <iostream>
#include <complex>
#include <string>

namespace fedorov
{
  struct DataStruct
  {
    double key1;
    std::complex< double > key2;
    std::string key3;
  };
  bool operator<(const DataStruct &lhs, const DataStruct &rhs);
  std::istream &operator>>(std::istream &in, DataStruct &value);
  std::ostream &operator<<(std::ostream &out, const DataStruct &value);
}

#endif
