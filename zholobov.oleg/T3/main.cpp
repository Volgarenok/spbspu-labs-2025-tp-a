#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>

#include "commands.hpp"
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

  using namespace zholobov;
  Polygons polygons;
  using InputStreamIterator = std::istream_iterator< Polygon >;
  while (!input.eof()) {
    std::copy(InputStreamIterator{input}, InputStreamIterator{}, std::back_inserter(polygons));
    if (input.fail()) {
      input.clear();
      input.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  std::map< std::string, std::function< void() > > commands;
  commands["AREA"] = std::bind(cmdArea, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["MAX"] = std::bind(cmdMax, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["MIN"] = std::bind(cmdMin, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["COUNT"] = std::bind(cmdCount, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["INTERSECTIONS"] = std::bind(cmdIntersections, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["RECTS"] = std::bind(cmdRects, std::ref(std::cout), std::cref(polygons));

  std::string cmd;
  while (!(std::cin >> cmd).eof()) {
    try {
      commands.at(cmd)();
      std::cout << "\n";
    } catch (const std::exception&) {
      if (!std::cin) {
        std::cin.clear();
      }
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  return 0;
}
