#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <limits>
#include "DataStruct.hpp"

int main()
{
  using holodilov::DataStruct;
  using istreamIter = std::istream_iterator<DataStruct>;
  using ostreamIter = std::ostream_iterator<DataStruct>;

  std::vector< DataStruct > vecDataStruct;
  while (!std::cin.eof())
  {
    std::copy(istreamIter(std::cin), istreamIter(), std::back_inserter(vecDataStruct));

    if (!std::cin)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  std::sort(vecDataStruct.begin(), vecDataStruct.end());
  std::copy(std::begin(vecDataStruct), std::end(vecDataStruct), ostreamIter(std::cout, "\n"));
  return 0;
}
