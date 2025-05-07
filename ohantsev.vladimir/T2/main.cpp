#include <iostream>
#include <algorithm>
#include <iterator>
#include <limits>
#include <vector>
#include "data.h"

int main()
{
  using ohantsev::DataStruct;
  using inIter = std::istream_iterator< DataStruct >;
  using outIter = std::ostream_iterator< DataStruct >;

  std::vector< DataStruct > data;
  while (!std::cin.eof())
  {
    std::copy(inIter(std::cin), inIter(), std::back_inserter(data));
    if (!std::cin)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }
  std::sort(data.begin(), data.end());
  std::copy(std::begin(data), std::end(data), outIter(std::cout, "\n"));
  return 0;
}
