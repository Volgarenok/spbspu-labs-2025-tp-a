#include <iostream>
#include <limits>
#include <iterator>
#include <vector>
#include <algorithm>
#include "DataStruct.h"

int main()
{
  std::vector< bob::DataStruct > vector;
  using is_iter = std::istream_iterator< bob::DataStruct >;
  using os_iter = std::ostream_iterator< bob::DataStruct >;
  while (!std::cin.eof())
  {
    std::copy(is_iter(std::cin), is_iter(), std::back_inserter(vector));
    if (std::cin.fail())
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  std::sort(vector.begin(), vector.end(), bob::compareDataStruct);
  std::copy(vector.begin(), vector.end(), os_iter(std::cout, "\n"));

  return 0;
}
