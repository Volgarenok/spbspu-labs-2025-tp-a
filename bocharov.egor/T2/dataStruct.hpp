#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP

#include <string>

namespace bocharov
{
  struct DataStruct
  {
    double key1;
    char key2;
    std::string key3;
    bool operator<(const DataStruct & rhs) const;
  };
}

#endif
