#include <algorithm>
#include <iterator>
#include <limits>
#include <vector>
#include "data.h"
int main()
{
  using namespace kharlamov;
  using isIterator = std::istream_iterator< DataStruct >;
  using osIterator = std::ostream_iterator< DataStruct >;
  std::vector< DataStruct > data;
  while (!std::cin.eof())
  {
    std::copy(isIterator(std::cin), isIterator(), std::back_inserter(data));
    if (!std::cin)
    {
      std::cin.clear(std::cin.rdstate() & ~std::ios::failbit);
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  if (std::cin.bad())
  {
    std::cerr << "Incorrect input\n";
    return 1;
  }
  std::sort(data.begin(), data.end(), kharlamov::compare);
  std::copy(std::begin(data), std::end(data), osIterator(std::cout, "\n"));
  return 0;
}
