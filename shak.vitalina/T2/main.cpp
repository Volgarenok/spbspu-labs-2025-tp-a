#include <algorithm>
#include <iterator>
#include <vector>
#include <limits>
#include <iostream>
#include "data_struct.hpp"

int main()
{
  using namespace shak;
  std::vector< DataStruct > data;
  using input_iteratot = std::istream_iterator< DataStruct >;
  using output_iteratort = std::ostream_iterator< DataStruct >;
  while (!std::cin.eof())
  {
    std::copy(input_iteratot{std::cin}, input_iteratot{}, std::back_inserter(data));
    if (std::cin.fail())
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::sort(data.begin(), data.end());
  std::copy(data.begin(), data.end(), output_iteratort(std::cout, "\n"));
  return 0;
}
