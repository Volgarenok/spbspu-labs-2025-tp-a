#include "polygon.hpp"
#include "command.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Missing filename argument\n";
    return 1;
  }

  std::ifstream fin(argv[1]);
  if (!fin) {
    std::cerr << "Error opening file\n";
    return 1;
  }

  std::vector<Polygon> polygons;
  std::string line;

  while (std::getline(fin, line)) {
    Polygon poly;
    if (parsePolygon(line, poly)) {
      polygons.push_back(std::move(poly));
    }
  }

  std::cout << std::fixed << std::setprecision(1);

  while (std::getline(std::cin, line)) {
    processCommand(line, polygons);
  }

  return 0;
}
