#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <vector>
#include "data-struct.hpp"

int main()
{
  using namespace kizhin;
  using DSContainer = std::vector< DataStruct >;
  using InputIt = std::istream_iterator< DataStruct >;
  using OutIt = std::ostream_iterator< DataStruct >;

  try {
    DSContainer vals(InputIt(std::cin), InputIt{});
    constexpr auto MaxSize = std::numeric_limits< std::streamsize >::max();
    while (!std::cin.eof()) {
      std::cin.clear();
      vals.insert(vals.end(), InputIt(std::cin.ignore(MaxSize, '\n')), InputIt{});
    }
    std::sort(vals.begin(), vals.end());
    std::copy(vals.begin(), vals.end(), OutIt(std::cout, "\n"));
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
  }
}

