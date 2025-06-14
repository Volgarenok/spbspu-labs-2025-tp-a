#include <iterator>
#include <vector>
#include <algorithm>
#include <limits>
#include <iostream>

#include "data_struct.hpp"

int main()
{
  using nehvedovich::DataStruct;
  using input_it_t = std::istream_iterator< DataStruct >;
  using output_it_t = std::ostream_iterator< DataStruct >;

  std::vector< DataStruct > data;

  while (!std::cin.eof())
  {
    std::copy(input_it_t{std::cin}, input_it_t{}, std::back_inserter(data));
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  std::sort(data.begin(), data.end());
  std::copy(data.begin(), data.end(), output_it_t{std::cout, "\n"});

  return 0;
}
