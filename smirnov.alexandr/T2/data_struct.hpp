#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP
#include <iostream>
#include <string>

namespace smirnov
{
  struct DataStruct
  {
    unsigned long long key1;
    long long key2;
    std::string key3;
  };
  bool compare(const DataStruct & lhs, const DataStruct & rhs);
  std::istream & operator>>(std::istream & in, DataStruct & data);
  std::ostream & operator<<(std::ostream & out, const DataStruct & data);
}
#endif
