#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <vector>
#include "data_struct.hpp"

int main() {
  using InputStreamIterator = std::istream_iterator<sherkunov::DataStruct>;
  using OutputStreamIterator = std::ostream_iterator<sherkunov::DataStruct>;

  std::vector<sherkunov::DataStruct> data;

  InputStreamIterator it(std::cin);
  while (it != InputStreamIterator{}) {
    data.push_back(*it);
    ++it;

    if (std::cin.fail() && !std::cin.eof()) {
      std::cin.clear(std::cin.rdstate() & ~std::ios::failbit);
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      it = InputStreamIterator(std::cin);
    }
  }

  if (std::cin.bad()) {
    std::cerr << "Error: Critical input failure\n";
    return 1;
  }

  std::sort(data.begin(), data.end());
  std::copy(data.begin(), data.end(), OutputStreamIterator(std::cout, "\n"));

  return 0;
}
