#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include "DataStruct.hpp"
namespace
{
  using namespace nikonov;
  bool compareDataStructs(const DataStruct& lhs, const DataStruct& rhs)
  {
    if (lhs.key1 == rhs.key1)
    {
      if (lhs.key2 == rhs.key2)
      {
        return lhs.key3.length() < rhs.key3.length();
      }
      return lhs.key2 < rhs.key2;
    }
    return lhs.key1 < rhs.key1;
  }
}

int main()
{
  using nikonov::DataStruct;
  using data_istream_it = std::istream_iterator< DataStruct >;
  std::vector< DataStruct > dS;
  while (!std::cin.eof())
  {
    if (std::cin.fail())
    {
      std::cin.clear();
    }
    std::copy(data_istream_it(std::cin), data_istream_it(), std::back_inserter(dS));
  }
  std::copy(data_istream_it(std::cin), data_istream_it(), std::back_inserter(dS));
  std::sort(dS.begin(), dS.end(), compareDataStructs);
  std::copy(dS.begin(), dS.end(), std::ostream_iterator< DataStruct >(std::cout, "\n"));
}
