#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <vector>
#include "structures.hpp"

int main() {
  using namespace firstime;
  using DSContainer = std::vector<DataStruct>;
  using InputIt = std::istream_iterator<DataStruct>;
  using OutIt = std::ostream_iterator<DataStruct>;

  try {
    DSContainer vals;

    // Read until EOF or unrecoverable error
    while (true) {
      // Read valid DataStructs
      std::copy(InputIt(std::cin), InputIt{}, std::back_inserter(vals));

      if (std::cin.eof()) break;  // Exit on EOF

      // Clear errors and skip bad input
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // Sort and output
    std::sort(vals.begin(), vals.end());
    std::copy(vals.begin(), vals.end(), OutIt(std::cout, "\n"));
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
  }
}
