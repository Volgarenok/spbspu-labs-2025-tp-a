#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

#include <iosfwd>
#include <string>
#include <utility>

namespace sherkunov {

  struct DataStruct {
    long long key1;
    std::pair< long long, unsigned long long > key2;
    std::string key3;
  };

  std::istream& operator>>(std::istream& input, DataStruct& value);
  std::ostream& operator<<(std::ostream& output, const DataStruct& value);
  bool operator<(const DataStruct& lhs, const DataStruct& rhs);
}

#endif
