#include "datastruct.h"

bool asafov::operator<(const DataStruct &a, const DataStruct &b)
{
  if (a.key1 != b.key1)
  {
    return a.key1 < b.key1;
  }
  if (std::abs(a.key2) != std::abs(b.key2))
  {
    return std::abs(a.key2) < std::abs(b.key2);
  }
  return a.key3.length() < b.key3.length();
}
