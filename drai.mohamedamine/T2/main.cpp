#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <vector>
#include "data-struct.hpp"

<<<<<<< HEAD
int main()
{
  using namespace firstry;
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
=======
int main() {
  using namespace firstime;
  using DSContainer = std::vector<DataStruct>;
  using InputIt = std::istream_iterator<DataStruct>;
  using OutIt = std::ostream_iterator<DataStruct>;
  try {
    DSContainer vals;
    while (true) {
      DataStruct temp;
      if (std::cin >> temp) {
        vals.push_back(temp);
      }
      else if (std::cin.eof()) {
        break;
      }
      else {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
    }
    if (vals.empty()) {
      std::cout << "Looks like there is no supported record. Cannot determine input. Test skipped\n";
      return 0;
    }
    std::sort(vals.begin(), vals.end());
    if (!vals.empty()) {
      std::cout << "Atleast one supported record type\n";
      std::copy(vals.begin(), vals.end(), OutIt(std::cout, "\n"));
    }
>>>>>>> 0646c30bb99ff9c1a94d10cb10cba2f8e23c37f3
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
  }
  return 0;
}
