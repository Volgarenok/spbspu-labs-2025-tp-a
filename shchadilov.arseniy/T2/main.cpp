#include <iostream>
#include <iterator>
#include <vector>
#include <limits>
#include <algorithm>
#include "data.hpp"

int main()
{
  using namespace shchadilov;
  using isIterator = std::istream_iterator< DataStruct >;
  using osIterator = std::ostream_iterator< DataStruct >;
  std::vector< DataStruct > data;
  while (!std::cin.eof())
  {
    std::copy(isIterator(std::cin), isIterator(), std::back_inserter(data));
    if (!std::cin)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::sort(data.begin(), data.end(), compare);
  std::copy(data.cbegin(), data.cend(), osIterator(std::cout, "\n"));
  return 0;
}
