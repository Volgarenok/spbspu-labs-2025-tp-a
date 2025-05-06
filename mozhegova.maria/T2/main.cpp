#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <limits>
#include "dataStruct.hpp"

int main()
{
  using mozhegova::DataStruct;
  using istreamIter = std::istream_iterator< DataStruct >;
  using ostreamIter = std::ostream_iterator< DataStruct >;
  std::vector< DataStruct > data;
  while (!std::cin.eof())
  {
    std::copy(istreamIter(std::cin), istreamIter(), std::back_inserter(data));
    if (std::cin.fail())
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::sort(data.begin(), data.end());
  std::copy(std::begin(data), std::end(data), ostreamIter(std::cout, "\n"));
}
