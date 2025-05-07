#include <algorithm>
#include <vector>
#include <limits>
#include <iterator>
#include "data.h"
#include <iostream>

int main()
{
  std::vector< horoshilov::DataStruct > data;
  while (!std::cin.eof())
  {
    std::copy(
      std::istream_iterator< horoshilov::DataStruct >(std::cin),
      std::istream_iterator< horoshilov::DataStruct >(),
      std::back_inserter(data)
    );
    if (!std::cin)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::sort(data.begin(), data.end(), horoshilov::compareData);
  std::copy(
    std::begin(data),
    std::end(data),
    std::ostream_iterator< horoshilov::DataStruct >(std::cout, "\n")
  );
  return 0;
}

