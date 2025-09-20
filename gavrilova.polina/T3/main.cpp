#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <string>
#include <vector>
#include "Polygon.hpp"
#include "commands.hpp"
#include <IOStreamGuard.hpp>

namespace fileutils {
  void readPolygonsFromFile(const std::string& filename, std::vector< gavrilova::Polygon >& polygons)
  {
    std::ifstream file(filename);
    if (!file) {
      throw std::runtime_error("Error: Could not open file.");
    }
    while (file) {
      gavrilova::Polygon temp;
      if (file >> temp) {
        polygons.push_back(temp);
      } else if (file.fail() && !file.eof()) {
        file.clear();
        file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      }
    }
  }
}

int main(int argc, char* argv[])
{
  if (argc != 2) {
    std::cerr << "Error: Invalid number of arguments. Usage: ./lab <filename>\n";
    return 1;
  }

  std::vector< gavrilova::Polygon > polygons;
  try {
    fileutils::readPolygonsFromFile(argv[1], polygons);
  } catch (const std::exception& e) {
    std::cerr << e.what() << "\n";
    return 1;
  }
  using command_func_t = std::function< void(const std::vector< gavrilova::Polygon >&, std::istream&, std::ostream&) >;
  std::map< std::string, command_func_t > command_map = {
      {"AREA", gavrilova::execArea},
      {"MAX", gavrilova::execMax},
      {"MIN", gavrilova::execMin},
      {"COUNT", gavrilova::execCount},
      {"PERMS", gavrilova::execPerms},
      {"LESSAREA", gavrilova::execLessArea}};

  std::string command;
  while (!(std::cin >> command).eof()) {
    std::transform(command.begin(), command.end(), command.begin(), ::toupper);
    try {
      auto it = command_map.find(command);
      if (it == command_map.end()) {
        throw std::invalid_argument("<INVALID COMMAND>");
      }
      gavrilova::IOStreamGuard guard(std::cout);
      it->second(polygons, std::cin, std::cout);
      std::cout << "\n";
    } catch (const std::exception& e) {
      std::cout << e.what() << "\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}
