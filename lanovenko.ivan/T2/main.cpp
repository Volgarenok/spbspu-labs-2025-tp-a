#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <limits>
#include "data_struct.hpp"
#include "data_struct_comparator.hpp"

int main()
{
  std::vector< lanovenko::DataStruct > data;
  while (!std::cin.eof())
  {
    std::copy(std::istream_iterator< lanovenko::DataStruct >(std::cin),
    std::istream_iterator< lanovenko::DataStruct >(), std::back_inserter(data));
    if (!std::cin) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::sort(data.begin(), data.end(), lanovenko::compare);
  std::copy(std::begin(data), std::end(data), std::ostream_iterator< lanovenko::DataStruct >(std::cout, "\n"));
}
