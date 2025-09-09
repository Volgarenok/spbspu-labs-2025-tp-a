#ifndef DATA_HPP
#define DATA_HPP

#include <string>
#include <complex>
#include <utility>
#include <iostream>

namespace beshimow {
  struct DataStruct {
    std::complex<double> key1;
    std::pair<long long, unsigned long long> key2;
    std::string key3;
  };

  std::istream &operator>>(std::istream &in, DataStruct &dest);
  std::ostream &operator<<(std::ostream &out, const DataStruct &dest);
  bool operator<(const DataStruct &lhs, const DataStruct &rhs);
}

#endif
