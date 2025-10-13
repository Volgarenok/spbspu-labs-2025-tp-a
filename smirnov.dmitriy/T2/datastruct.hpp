#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP

#include "delimiter.hpp"
#include <iomanip>
#include <iostream>
#include <string>

namespace smirnov
{
  struct DataStruct
  {
    double key1;
    unsigned long long key2;
    std::string key3;
  };

bool operator<(const DataStruct& lhs, const DataStruct& rhs);
std::istream& operator>>(std::istream& in, DataStruct& value);
std::ostream& operator<<(std::ostream& out, const DataStruct& value);

}

#endif

