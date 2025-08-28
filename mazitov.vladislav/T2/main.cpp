#include "data_struct.hpp"
#include <iterator>
#include <vector>
#include <algorithm>
#include <limits>
#include <iostream>

int main()
{
  using mazitov::DataStruct;
  using istream_it = std::istream_iterator< DataStruct >;
  std::vector< DataStruct > data;

  while (!std::cin.eof())
  {
    std::copy(istream_it{std::cin}, istream_it{}, std::back_inserter(data));
    if (std::cin.fail())
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  std::sort(data.begin(), data.end());
  std::copy(std::begin(data), std::end(data), std::ostream_iterator< DataStruct >(std::cout, "\n"));

  return 0;
}
