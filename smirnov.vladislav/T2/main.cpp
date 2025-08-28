#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <limits>
#include "data.hpp"

int main()
{
  using dataSt = smirnov::DataStruct;
  using istreamIt = std::istream_iterator< dataSt >;
  using ostreamIt = std::ostream_iterator< dataSt >;

  std::vector< dataSt > data;
  while (!std::cin.eof())
  {
    std::copy(istreamIt(std::cin), istreamIt(), std::back_inserter(data));

    if (!std::cin)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  std::sort(data.begin(), data.end(), smirnov::compare);
  std::copy(std::begin(data), std::end(data), ostreamIt(std::cout, "\n"));
  return 0;
}
