#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <vector>

#include "data_struct.hpp"

int main()
{
  using InputStreamIterator = std::istream_iterator< zholobov::DataStruct >;
  using OutputStreamIterator = std::ostream_iterator< zholobov::DataStruct >;

  std::vector< zholobov::DataStruct > data;
  while (!std::cin.eof()) {
    std::copy(InputStreamIterator{std::cin}, InputStreamIterator{}, std::back_inserter(data));
    if (std::cin.fail()) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::sort(data.begin(), data.end());
  std::copy(data.begin(), data.end(), OutputStreamIterator{std::cout, "\n"});
  return 0;
}
