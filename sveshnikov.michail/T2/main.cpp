#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
#include "data-struct.hpp"

int main()
{
  using namespace sveshnikov;
  std::vector< DataStruct > data;

  while (!std::cin.eof())
  {
    if (std::cin.fail())
    {
      std::cin.clear();
    }
    std::copy(
      std::istream_iterator< DataStruct >(std::cin),
      std::istream_iterator< DataStruct >(),
      std::back_inserter(data)
    );
  }

  std::sort(data.begin(), data.end());

  std::copy(
    data.begin(),
    data.end(),
    std::ostream_iterator< DataStruct >(std::cout, "\n")
  );
  return 0;
}
