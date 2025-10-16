#include <iterator>
#include <vector>
#include <algorithm>
#include <limits>
#include <iostream>
#include "data_struct.hpp"

int main()
{
  using nikitin::DataStruct;
  using istream_it = std::istream_iterator< DataStruct >;
  using ostream_it = std::ostream_iterator< DataStruct >;
  std::vector< DataStruct > data;

  while (!std::cin.eof())
  {
    std::copy(istream_it{std::cin}, istream_it{}, std::back_inserter(data));
    if (std::cin.fail())
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  std::sort(data.begin(), data.end());
  std::copy(data.begin(), data.end(), ostream_it{std::cout, "\n"});
}

