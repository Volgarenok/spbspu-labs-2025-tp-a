#include <algorithm>
#include <vector>
#include <limits>
#include <iterator>
#include "data.h"

int main()
{
  using namespace mezentsev;
  using isIterator = std::istream_iterator< DataStruct >;
  using osIterator = std::ostream_iterator< DataStruct >;
  std::vector< DataStruct > data;
  while(!std::cin.eof())
  {
    std::copy(isIterator(std::cin), isIterator(), std::back_inserter(data));
    if (!std::cin)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::sort(data.begin(), data.end(), mezentsev::compare);
  std::copy(std::begin(data), std::end(data), osIterator(std::cout, "\n"));
  return 0;
}
