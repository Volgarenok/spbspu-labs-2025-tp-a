#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <map>

#include "polygon.hpp"

int main(int argc, char* argv[])
{
  if (argc != 2) {
    std::cerr << "Invalid parameters\n";
    return 1;
  }

  std::ifstream input(argv[1]);
  if (!input) {
    std::cerr << "Error opening input file: " << argv[1] << '\n';
    return 1;
  }

  zholobov::Polygons polygons;
  using InputStreamIterator = std::istream_iterator< zholobov::Polygon >;
  while (!input.eof()) {
    std::copy(InputStreamIterator{input}, InputStreamIterator{}, std::back_inserter(polygons));
    if (input.fail()) {
      input.clear();
      input.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  return 0;
}
