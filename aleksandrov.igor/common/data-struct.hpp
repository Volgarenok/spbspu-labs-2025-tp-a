#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP

#include <iomanip>
#include <utility>
#include <string>

namespace aleksandrov
{
  struct DataStruct
  {
    double key1;
    std::pair< long long, unsigned long long > key2;
    std::string key3;

    bool operator<(const DataStruct&) const;
  };
}

#endif

