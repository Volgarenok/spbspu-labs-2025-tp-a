#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <limits>
#include "data_struct.hpp"

int main()
{
  using lanovenko::DataStruct;
  std::vector< DataStruct > data;
  using iIterator = std::istream_iterator< DataStruct >;
  while (!std::cin.eof())
  {
    std::copy(iIterator(std::cin), iIterator(), std::back_inserter(data));
    if (!std::cin)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::sort(data.begin(), data.end(), lanovenko::compare);
  std::copy(std::begin(data), std::end(data), std::ostream_iterator< DataStruct >(std::cout, "\n"));
}
