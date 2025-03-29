#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP
#include "types.hpp"
#include <iostream>

namespace shapkov
{
  struct DataStruct
  {
    double key1;
    std::pair< long long, unsigned long long > key2;
    std::string key3;
  };
  std::istream& operator>>(std::istream& in, DataStruct& rhs);
  std::ostream& operator<<(std::ostream& out, const DataStruct& rhs);
  bool dataComparator(const DataStruct& lhs, const DataStruct& rhs);
}
#endif
