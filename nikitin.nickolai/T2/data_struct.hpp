#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP

#include <complex>
#include <string>
#include <iomanip>

namespace nikitin
{
  struct DataStruct
  {
    long long key1;
    std::complex< double > key2;
    std::string key3;
  };

  bool operator<(const DataStruct& lhs, const DataStruct& rhs);
  std::istream& operator>>(std::istream& in, DataStruct& dest);
  std::ostream& operator<<(std::ostream& out, const DataStruct& dest);
}

#endif
