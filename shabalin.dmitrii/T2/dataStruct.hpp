#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP

#include <complex>
#include <istream>
#include <ostream>
#include <string>

namespace shabalin
{
  struct DataStruct
  {
    std::complex< double > key1;
    double key2;
    std::string key3;
    bool operator<(const DataStruct& other) const;
  };
  std::istream& operator>>(std::istream& in, DataStruct& value);
  std::ostream& operator<<(std::ostream& out, const DataStruct& value);
}

#endif