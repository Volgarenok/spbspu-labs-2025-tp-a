#include "data_struct.hpp"
#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <limits>

int main()
{
  using namespace shiryaeva;
  std::vector< DataStruct > data;

  while (!std::cin.eof())
  {
    using InIter = std::istream_iterator< DataStruct >;
    std::copy(InIter(std::cin), InIter(), std::back_inserter(data));

    if (std::cin.fail())
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  if (data.empty())
  {
    std::cout << "Looks like there is no supported record. Cannot determine input. Test skipped\n";
    return 0;
  }

  std::sort(data.begin(), data.end());
  using OutIter = std::ostream_iterator< DataStruct >;
  OutIter out(std::cout, "\n");
  std::copy(data.begin(), data.end(), out);

  return 0;
}

