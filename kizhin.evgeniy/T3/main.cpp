#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include "args-parser.hpp"
#include "command-processor.hpp"

int main(int argc, char** argv)
{
  using namespace kizhin;
  try {
    const Args args = parseArgs(argc, argv, std::cerr);
    std::ifstream in(args.filename);
    if (!in.is_open()) {
      throw std::logic_error("Failed to open file: " + args.filename);
    }
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

