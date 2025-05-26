#include "data_struct_comparator.hpp"

bool lanovenko::compare(const DataStruct &lhs, const DataStruct &rhs)
{
  bool compareChr = lhs.key1 < rhs.key1;
  bool compareRtn = lhs.key2 < rhs.key2;
  bool compareStr = lhs.key3.size() < rhs.key3.size();
  return lhs.key1 != rhs.key1 ? compareChr : (lhs.key2 != rhs.key2 ? compareRtn : compareStr);
}
