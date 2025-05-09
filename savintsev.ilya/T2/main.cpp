#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include "data-struct.hpp"

int main()
{
  using namespace savintsev;

  std::vector< DataStruct > data;

  using data_iterator = std::istream_iterator< DataStruct >;
  while (!std::cin.eof())
  {
    if (std::cin.fail())
    {
      std::cin.clear();
    }
    std::copy(data_iterator(std::cin), data_iterator(), std::back_inserter(data));
  }

  std::sort(data.begin(), data.end());

  std::copy(data.begin(), data.end(), std::ostream_iterator< DataStruct >(std::cout, "\n"));
}
