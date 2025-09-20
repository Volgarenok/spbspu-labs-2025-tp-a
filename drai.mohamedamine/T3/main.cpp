#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "commands.hpp"
#include "polygon.hpp"

namespace amine
{
  void readAndProcess(std::istream& in, CommandProcessor& processor)
  {
    std::string line;
    if (!std::getline(in, line))
      return;
    processor(line);
    readAndProcess(in, processor);
  }

  void readPolygons(std::istream& in, std::vector< Polygon >& polygons)
  {
    std::string line;
    if (!std::getline(in, line))
      return;

    Polygon poly;
    if (!line.empty() && parse_polygon(line, poly) && poly.points.size() >= 3)
    {
      polygons.push_back(poly);
    }
    readPolygons(in, polygons);
  }
}

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    std::cerr << "Error: missing filename argument\n";
    return 1;
  }

  std::ifstream infile(argv[1]);
  if (!infile)
  {
    std::cerr << "Error: could not open file\n";
    return 1;
  }

  std::vector< Polygon > polygons;
  amine::readPolygons(infile, polygons);

  amine::CommandProcessor processor(polygons);
  amine::readAndProcess(std::cin, processor);
  return 0;
}
