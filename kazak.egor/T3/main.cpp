#include <fstream>
#include <iostream>
#include <map>
#include <functional>
#include <limits>
#include <sstream>
#include <string>
#include <vector>
#include "commands.hpp"
#include "utils.hpp"

using namespace kazak;

std::vector<Polygon> polygons;

void cmdArea(std::istream & is) {
  std::string param;
  is >> param;
  commands::commandArea(polygons, param, std::cout);
}

void cmdMax(std::istream & is) {
  std::string param;
  is >> param;
  commands::commandMax(polygons, param, std::cout);
}

void cmdMin(std::istream & is) {
  std::string param;
  is >> param;
  commands::commandMin(polygons, param, std::cout);
}

void cmdCount(std::istream & is) {
  std::string param;
  is >> param;
  commands::commandCount(polygons, param, std::cout);
}

void cmdEcho(std::istream & is) {
  std::string line;
  std::getline(is, line);
  if (!line.empty() && line[0] == ' ') {
    line.erase(0, 1);
  }
  commands::commandEcho(polygons, line, std::cout);
}

void cmdInframe(std::istream & is) {
  std::string line;
  std::getline(is, line);
  if (!line.empty() && line[0] == ' ') {
    line.erase(0, 1);
  }
  commands::commandInframe(polygons, line, std::cout);
}

int main(int argc, char * argv[])
{
  if (argc != 2) {
    std::cerr << "Error: Filename not provided." << std::endl;
    return 1;
  }

  std::ifstream input(argv[1]);
  if (!input) {
    std::cerr << "Error: Cannot open file." << std::endl;
    return 1;
  }

  std::string line;
  while (std::getline(input, line)) {
    Polygon poly;
    if (utils::parsePolygon(line, poly)) {
      polygons.push_back(poly);
    }
  }

  std::map<std::string, std::function<void(std::istream &)>> commands_map;

  using namespace std::placeholders;
  commands_map["AREA"] = std::bind(cmdArea, _1);
  commands_map["MAX"] = std::bind(cmdMax, _1);
  commands_map["MIN"] = std::bind(cmdMin, _1);
  commands_map["COUNT"] = std::bind(cmdCount, _1);
  commands_map["ECHO"] = std::bind(cmdEcho, _1);
  commands_map["INFRAME"] = std::bind(cmdInframe, _1);

  while (std::cin >> line) {
    try {
      if (commands_map.count(line)) {
        commands_map.at(line)(std::cin);
      } else {
        std::cout << "<INVALID COMMAND>\n";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
    } catch (...) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  return 0;
}
