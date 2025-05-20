#include "sortDataVector.hpp"
#include <complex>
#include <cmath>
#include <algorithm>
#include "dataStruct.hpp"

namespace
{
  bool dataCMP(finaev::DataStruct& first, finaev::DataStruct& second)
  {
    if (first.key1 != second.key1)
    {
      return first.key1 < second.key1;
    }
    auto firstComplex = std::abs(first.key2);
    auto secondComplex = std::abs(second.key2);
    if (std::abs(firstComplex - secondComplex) > (1 * pow(10, -10)))
    {
      return firstComplex < secondComplex;
    }
    return first.key3.size() < second.key3.size();
  }
}

void finaev::dataSort(std::vector< DataStruct >::iterator begin, std::vector< DataStruct >::iterator end)
{
  std::sort(begin, end, dataCMP);
}
