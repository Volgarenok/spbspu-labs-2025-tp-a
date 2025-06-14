#include <iostream>
#include <limits>
#include <iterator>
#include <vector>
#include <algorithm>
#include "DataStruct.h"

int main()
{
  using fedorova::DataStruct;
  using istreamDataStruct = std::istream_iterator< DataStruct >;
  using ostreamDataStruct = std::ostream_iterator< DataStruct >;

  std::vector< DataStruct > data;

  while (!std::cin.eof())
  {
    std::copy(istreamDataStruct{ std::cin }, istreamDataStruct{}, std::back_inserter(data));
    if (std::cin.fail())
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  std::sort(data.begin(), data.end(), fedorova::compareDataStruct);

  std::copy(std::begin(data), std::end(data), ostreamDataStruct(std::cout, "\n"));

  return 0;
}
