#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <vector>

#include "DataStruct.hpp"

int main()
{
  using namespace gavrilova;
  std::vector< DataStruct > data;

  DataStruct new_struct;
  while (!std::cin.eof()) {
    std::cin >> new_struct;
    if (!std::cin) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    } else {
      data.push_back(new_struct);
    }
  }
  std::sort(data.begin(), data.end(), compare);
  std::copy(std::begin(data), std::end(data), std::ostream_iterator< DataStruct >(std::cout, "\n"));
}
