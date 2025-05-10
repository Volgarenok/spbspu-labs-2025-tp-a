#include <vector>
#include <iostream>
#include <iterator>
#include <limits>
#include <algorithm>
#include "data_struct.hpp"
#include "compare.hpp"

int main()
{
  using namespace maslevtsov;
  using data_struct_in_it = std::istream_iterator< DataStruct >;
  using data_struct_out_it = std::ostream_iterator< DataStruct >;
  using streamsize_lim = std::numeric_limits< std::streamsize >;

  std::vector< DataStruct > data;

  while (!std::cin.eof()) {
    std::copy(data_struct_in_it(std::cin), data_struct_in_it(), std::back_inserter(data));
    if (!std::cin) {
      std::cin.clear();
      std::cin.ignore(streamsize_lim::max(), '\n');
    }
  }

  std::sort(data.begin(), data.end(), compare);

  std::copy(std::begin(data), std::end(data), data_struct_out_it(std::cout, "\n"));
}
