#include "dataStruct.hpp"

bool bocharov::DataStruct::operator<(const DataStruct & rhs) const
{
  if (key1 != rhs.key1)
  {
    return key1 < rhs.key1;
  }
  else if (key2 != rhs.key2)
  {
    return key2 < rhs.key2;
  }
  else
  {
    return key3.length() < rhs.key3.length();
  }
}
