#include "Commands.h"
#include "Polygon.h"
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <vector>

int main(int argc, char** argv)
{
  using istreamIt = std::istream_iterator< horoshilov::Polygon >;
  if (argc != 2)
  {
      std::cerr << "Usage: " << argv[0] << " <filename>\n";
      return 1;
  }
  std::ifstream file(argv[1]);
  std::vector<horoshilov::Polygon> polygons;
  while (!file.eof())
  {
    std::copy(istreamIt(file), istreamIt(), std::back_inserter(polygons));
    if (!file)
    {
     file.clear(file.rdstate() ^ std::ios::failbit);
     file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
     }
  }
  std::map<std::string, std::function<void()>> commands;
  commands["AREA"] = std::bind(horoshilov::printArea, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["MAX"] = std::bind(horoshilov::printMax, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["MIN"] = std::bind(horoshilov::printMin, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["COUNT"] = std::bind(horoshilov::printCount, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["PERMS"] = std::bind(horoshilov::printPermsCount, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["INTERSECTIONS"] = std::bind(horoshilov::printIntersections, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));

  std::string cmd;
  while (std::cin >> cmd)
  {
      try
      {
          commands.at(cmd)();
      }
      catch (...)
      {
          std::cout << "<INVALID COMMAND>\n";
          std::cin.clear();
          std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
  }
  return 0;
}
