#include <algorithm>
#include <iterator>
#include <limits>
#include <vector>
#include <iostream>
#include "dataStruct.hpp"

int main()
{
  std::vector< kostyukov::DataStruct > data;
  while (!std::cin.eof())
  {
    std::copy(std::istream_iterator< kostyukov::DataStruct >(std::cin),
              std::istream_iterator< kostyukov::DataStruct >(),
              std::back_inserter(data));
    if (std::cin.fail())
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::sort(data.begin(), data.end());
  std::copy(std::begin(data), std::end(data),
            std::ostream_iterator< kostyukov::DataStruct >(std::cout, "\n"));
  return 0;
}
