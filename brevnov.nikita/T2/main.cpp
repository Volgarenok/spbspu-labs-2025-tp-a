#include <algorithm>
#include <vector>
#include <limits>
#include <iterator>
#include "data_struct.hpp"
int main()
{
  using DaS = brevnov::DataStruct;
  using istreamIt = std::istream_iterator< DaS >;
  using ostreamIt = std::ostream_iterator< DaS >;
  std::vector< DaS > data;
  while (!std::cin.eof())
  {
    std::copy(istreamIt(std::cin), istreamIt(), std::back_inserter(data));
    if (!std::cin)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::sort(data.begin(), data.end(), brevnov::compare);
  std::copy(std::begin(data), std::end(data), ostreamIt(std::cout, "\n"));
  return 0;
}
