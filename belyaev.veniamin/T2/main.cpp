#include <iostream>
#include <vector>
#include "data-struct.hpp"
#include <iterator>
#include <algorithm>
#include <limits>

int main()
{
  using namespace belyaev;

  std::vector< DataStruct > data;
  while (!std::cin.eof() && !std::cin.bad())
  {
    if (std::cin.fail())
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
    std::copy(
      std::istream_iterator< DataStruct >{std::cin},
      std::istream_iterator< DataStruct >{},
      std::back_inserter(data)
    );
  }

  if (std::cin.bad())
  {
    std::cerr << "Input error.\n";
    return 2;
  }

  std::sort(data.begin(), data.end());
  std::copy(data.begin(), data.end(), std::ostream_iterator< DataStruct >{std::cout, "\n"});

  return 0;
}
