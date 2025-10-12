#include <vector>
#include <algorithm>
#include <iterator>
#include <limits>
#include <iostream>

#include "DataStruct.h"

int main()
{
  using voronina::DataStruct;
  std::vector< DataStruct > data;
  while (!std::cin.eof())
  {
    using InputIterator = std::istream_iterator< DataStruct >;
    std::copy(InputIterator(std::cin), InputIterator(), std::back_inserter(data));
    if (std::cin.fail())
    {
      std::cin.clear(std::cin.rdstate() & ~std::ios::failbit);
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  if (std::cin.bad())
  {
    std::cerr << "Fatal I/O error\n";
    return 1;
  }
  using OutputIterator = std::ostream_iterator< DataStruct >;
  std::sort(std::begin(data), std::end(data), voronina::compare);
  std::copy(std::begin(data), std::end(data), OutputIterator(std::cout, "\n"));
  return 0;
}
