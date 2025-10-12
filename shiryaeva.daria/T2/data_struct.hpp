#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP

#include <string>
#include <iomanip>

namespace shiryaeva
{
  struct DataStruct
  {
    unsigned long long key1;
    char key2;
    std::string key3;
  };

  bool operator<(const DataStruct &lhs, const DataStruct &rhs);
  std::istream &operator>>(std::istream &in, DataStruct &dest);
  std::ostream &operator<<(std::ostream &out, const DataStruct &src);
}

#endif
