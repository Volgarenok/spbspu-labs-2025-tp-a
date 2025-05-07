#include "data-struct.hpp"

namespace aleksandrov
{
  bool DataStruct::operator<(const DataStruct& rhs) const
  {
    if (key1 == rhs.key1)
    {
      if (key2 == rhs.key2)
      {
        return key3.size() < rhs.key3.size();
      }
      return (key2.first / key2.second) < (rhs.key2.first / rhs.key2.second);
    }
    return key1 < rhs.key1;
  }
}

