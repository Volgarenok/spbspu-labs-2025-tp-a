#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
#include "data-struct.hpp"

int main()
{
  using namespace sveshnikov;
  using in_iter = std::istream_iterator< DataStruct >;
  using out_iter = std::ostream_iterator< DataStruct >;

  std::vector< DataStruct > data;
  while (!std::cin.eof())
  {
    if (std::cin.fail())
    {
      std::cin.clear();
    }
    std::copy(in_iter(std::cin), in_iter(), std::back_inserter(data));
  }
  std::sort(data.begin(), data.end());
  std::copy(data.begin(), data.end(), out_iter(std::cout, "\n"));
  return 0;
}
