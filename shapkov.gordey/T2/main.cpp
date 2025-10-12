#include <iostream>
#include <iterator>
#include <vector>
#include <limits>
#include <algorithm>
#include "dataStruct.hpp"

int main()
{
  using shapkov::DataStruct;
  std::vector< DataStruct > data;
  using inputIt = std::istream_iterator< DataStruct >;
  using outputIt = std::ostream_iterator< DataStruct >;
  while (!std::cin.eof())
  {
    std::copy(inputIt(std::cin), inputIt(), std::back_inserter(data));
    if (!std::cin)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }
  std::sort(data.begin(), data.end());
  std::copy(std::begin(data), std::end(data), outputIt(std::cout, "\n"));
  return 0;
}
