#include <iostream>
#include "dataStruct.hpp"
#include <iterator>
#include <vector>
#include <limits>
#include <algorithm>

int main()
{
  std::vector< shapkov::DataStruct > data;
  while (!std::cin.eof())
  {
    std::copy(
      std::istream_iterator< shapkov::DataStruct >(std::cin),
      std::istream_iterator< shapkov::DataStruct >(),
      std::back_inserter(data)
    );
    if (!std::cin)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }
  std::sort(data.begin(), data.end(), shapkov::dataComparator);
  std::copy(
    std::begin(data),
    std::end(data),
    std::ostream_iterator< shapkov::DataStruct >(std::cout, "\n")
  );
  return 0;
}
