#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include "command-processor.hpp"

int main(int argc, char** argv)
{
  if (argc != 2 || argv[1][0] == '\0') {
    std::cerr << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }
  const char* filename = argv[1];
  try {
    std::ifstream in(filename);
    if (!in.is_open()) {
      std::cerr << "Failed to open file: " + std::string(filename) << '\n';
      return 1;
    }
    using namespace kizhin;
    using InIt = std::istream_iterator< Polygon >;
    PolygonContainer polygons(InIt{ in }, InIt{});
    constexpr auto maxSize = std::numeric_limits< std::streamsize >::max();
    while (!in.eof()) {
      in.clear();
      in.ignore(maxSize, '\n');
      polygons.insert(polygons.end(), InIt{ in }, InIt{});
    }
    processCommands(polygons, std::cin, std::cout);
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 1;
  }
}

