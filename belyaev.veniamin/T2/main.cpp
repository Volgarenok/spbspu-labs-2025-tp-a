#include <vector>
#include <iterator>
#include <algorithm>
#include <limits>
#include "data-struct.hpp"

int main()
{
  using namespace belyaev;
  using istreamDS = std::istream_iterator< DataStruct >;
  using ostreamDS = std::ostream_iterator< DataStruct >;

  std::vector< DataStruct > data;
  while (!std::cin.eof() && !std::cin.bad())
  {
    if (std::cin.fail())
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
    std::copy(istreamDS{std::cin}, istreamDS{}, std::back_inserter(data));
  }

  if (std::cin.bad())
  {
    std::cerr << "Input error.\n";
    return 2;
  }

  std::sort(data.begin(), data.end());
  std::copy(data.begin(), data.end(), ostreamDS{std::cout, "\n"});

  return 0;
}
