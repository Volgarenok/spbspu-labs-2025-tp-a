#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <vector>
#include "structures.hpp"

int main() {
  using namespace firstime;  // Updated namespace
  using DSContainer = std::vector<DataStruct>;
  using InputIt = std::istream_iterator<DataStruct>;
  using OutIt = std::ostream_iterator<DataStruct>;

  try {
    DSContainer vals(InputIt(std::cin), InputIt{});

    // Handle remaining input (e.g., newlines after errors)
    constexpr auto MaxSize = std::numeric_limits<std::streamsize>::max();
    while (!std::cin.eof()) {
      std::cin.clear();
      std::cin.ignore(MaxSize, '\n');
      vals.insert(vals.end(), InputIt(std::cin), InputIt{});
    }

    // Sort using the custom comparator
    std::sort(vals.begin(), vals.end());

    // Output sorted data
    std::copy(vals.begin(), vals.end(), OutIt(std::cout, "\n"));
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
  }
}