#include <iostream>
#include <functional>
#include <algorithm>
#include <limits>
#include <vector>
#include <map>
#include <fstream>
#include "geom.h"
#include "commands.h"

int main(int argc, char** argv)
{
  using namespace orlova;

  std::vector< Polygon > polygons;

  if (argc != 2)
  {
    std::cerr << "<INCORRECT ARGUMENTS>\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file)
  {
    std::cerr << "<INCORRECT FILE>\n";
    return 1;
  }

  getPolygons(file, polygons);

  std::map< std::string, std::function< void() > > cmds;
  cmds["area"] = std::bind(area, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));
  cmds["max"] = std::bind(max, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));
  cmds["min"] = std::bind(min, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));
  cmds["count"] = std::bind(count, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));

  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      cmds.at(command)();
    }
    catch (...)
    {
      if (std::cin.fail())
      {
        std::cin.clear(std::cin.rdstate() ^ std::ios::failbit);
      }
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }
}
