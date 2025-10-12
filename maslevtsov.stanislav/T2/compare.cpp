#include "compare.hpp"

bool maslevtsov::compare(const DataStruct& left, const DataStruct& right)
{
  if (left.key1 != right.key1) {
    return left.key1 < right.key1;
  }
  if (left.key2 != right.key2) {
    return left.key2 < right.key2;
  }
  return left.key3.size() < right.key3.size();
}
