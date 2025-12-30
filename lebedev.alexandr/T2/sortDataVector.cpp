#include "sortDataVector.hpp"
#include <complex>
#include <cmath>
#include <algorithm>
#include "dataStruct.hpp"

namespace
{
  bool dataCMP(lebedev::DataStruct& first, lebedev::DataStruct& second)
  {
    if (first.key1 != second.key1)
    {
      return first.key1 < second.key1;
    }
    if (first.key2 != second.key2)
    {
      return first.key2 < second.key2;
    }
    return first.key3.size() < second.key3.size();
  }
}

void lebedev::dataSort(std::vector< DataStruct >::iterator begin, std::vector< DataStruct >::iterator end)
{
  std::sort(begin, end, dataCMP);
}
