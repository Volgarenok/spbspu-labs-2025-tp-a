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
    while (true) {
      std::copy(InputIt(std::cin), InputIt{}, std::back_inserter(vals));
      if (std::cin.eof()) break;
      if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
    }
    if (vals.empty()) {
      std::cout << "Looks like there is no supported record. Cannot determine input. Test skipped\n";
      return 0;
    }
    std::sort(vals.begin(), vals.end());
    std::copy(vals.begin(), vals.end(), OutIt(std::cout, "\n"));
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
  }
  return 0;
}
