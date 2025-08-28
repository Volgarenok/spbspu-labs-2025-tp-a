#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <vector>

#include "dataStruct.hpp"

int main()
{
  using kostyukov::DataStruct;
  using DataStructInputIt = std::istream_iterator< DataStruct >;
  using DataStructOutputIt = std::ostream_iterator< DataStruct >;
  std::vector< DataStruct > data;
  while (!std::cin.eof())
  {
    std::copy(DataStructInputIt(std::cin), DataStructInputIt(), std::back_inserter(data));
    if (std::cin.fail())
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::sort(data.begin(), data.end());
  std::copy(std::begin(data), std::end(data), DataStructOutputIt(std::cout, "\n"));
  return 0;
}
