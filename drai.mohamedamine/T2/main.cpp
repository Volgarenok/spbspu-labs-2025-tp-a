#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <vector>
#include "data.hpp"

int main()
{
  using amine::DataStruct;
  using Vec = std::vector< DataStruct >;
  using In = std::istream_iterator< DataStruct >;
  using Out = std::ostream_iterator< DataStruct >;

  constexpr auto max_ignore = std::numeric_limits< std::streamsize >::max();
  Vec data;

  while (!std::cin.eof())
  {
    In in{ std::cin };
    In in_end{};
    std::copy(in, in_end, std::back_inserter(data));
    if (std::cin.fail())
    {
      std::cin.clear();
      std::cin.ignore(max_ignore, '\n');
    }
  }

  std::sort(data.begin(), data.end());

  Out out{ std::cout, "\n" };
  std::copy(data.begin(), data.end(), out);

  return 0;
}
