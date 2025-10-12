#include <algorithm>
#include <vector>
#include <limits>
#include <iterator>
#include <iostream>

#include "data.hpp"

int main()
{
  using namespace dribas;
  using ositer = std::ostream_iterator< DataStruct >;
  using isiter = std::istream_iterator< DataStruct >;


  std::vector< DataStruct > data;

  while (!std::cin.eof()) {
    std::copy(isiter(std::cin), isiter(), std::back_inserter(data));
    if (!std::cin) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  std::sort(data.begin(), data.end(), compare);
  std::copy(data.begin(), data.end(), ositer(std::cout, "\n"));

  return 0;
}
