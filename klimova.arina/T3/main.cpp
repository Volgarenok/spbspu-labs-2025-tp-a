#include <functional>
#include <iostream>
#include <iterator>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
#include "polygon.hpp"
#include "commands.hpp"

int main(int argc, char* argv[])
{
  using klimova::Polygon;
  using inputIterator = std::istream_iterator< Polygon >;

  if (argc != 2) {
    return 1;
  }
  std::ifstream file(argv[1]);
  std::vector< Polygon > polygons;
  while (!file.eof()) {
    if (!file) {
      file.clear();
      file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
    std::copy(inputIterator(file), inputIterator(), std::back_inserter(polygons));
  }
  auto cmds = createCommandHandler(polygons);
  std::string command;
  while (!(std::cin >> command).eof()) {
    try {
      cmds.at(command)();
    }
    catch (const std::exception&) {
      if (std::cin.fail()) {
        std::cin.clear(std::cin.rdstate() ^ std::ios::failbit);
      }
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }
  return 0;
}
