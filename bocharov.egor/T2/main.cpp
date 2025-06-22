#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <limits>
#include "ioTypes.hpp"

int main()
{
  using namespace bocharov;
  using iIterator = std::istream_iterator< DataStruct >;
  using oIterator = std::ostream_iterator< DataStruct >;
  std::vector< DataStruct > data;
  while (!std::cin.eof())
  {
    std::copy(iIterator(std::cin), iIterator(), std::back_inserter(data));
    if (!std::cin)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::sort(data.begin(), data.end());
  std::copy(std::begin(data), std::end(data), oIterator(std::cout, "\n"));
}
