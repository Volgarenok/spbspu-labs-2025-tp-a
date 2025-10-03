#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <limits>
#include "DataStruct.hpp"

int main()
{
  using ageev::DataStruct;

  std::vector< DataStruct > data;

  using InputDataIt = std::istream_iterator< DataStruct >;

  while (!std::cin.eof())
  {
    std::copy(InputDataIt(std::cin), InputDataIt(), std::back_inserter(data));
    if (std::cin.bad())
    {
      std::cerr << "Fatal I/O error occurred\n";
      return 2;
    }
    if (std::cin.fail() && !std::cin.eof())
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::sort(data.begin(), data.end());
  std::copy(data.begin(), data.end(), std::ostream_iterator< DataStruct >(std::cout, "\n"));
  return 0;
}
