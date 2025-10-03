#include <list>
#include <iterator>
#include <limits>
#include "datastruct.hpp"

int main()
{
  using namespace smirnov;
  using InputIt = std::istream_iterator< DataStruct >;
  using OutputIt = std::ostream_iterator< DataStruct >;

  std::list< DataStruct > data{};

  while (!std::cin.eof())
  {
    if (std::cin.fail())
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
    std::copy(InputIt{ std::cin }, InputIt{}, std::back_inserter(data));
  }
  data.sort();
  std::copy(data.cbegin(), data.cend(), OutputIt{ std::cout, "\n" });
  return 0;
}
