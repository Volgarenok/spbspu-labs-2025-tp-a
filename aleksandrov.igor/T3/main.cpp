#include <functional>
#include <iostream>
#include <fstream>
#include <limits>
#include <map>
#include "geometry.hpp"
#include "polygon-commands.hpp"

int main(int argc, char* argv[])
{
  using namespace aleksandrov;

  if (argc != 2)
  {
    std::cerr << "ERROR: Incorrect arguments!\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file)
  {
    std::cerr << "ERROR: Incorrect file!\n";
    return 1;
  }

  std::vector< Polygon > polygons;
  getPolygons(file, polygons);

  std::map< std::string, std::function< void() > > commands;

  commands["AREA"] = std::bind(area, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));
  size_t max = polygons.size() - 1;
  commands["MAX"] = std::bind(ithSmallest, std::cref(polygons), max, std::ref(std::cin), std::ref(std::cout));
  commands["MIN"] = std::bind(ithSmallest, std::cref(polygons), 0, std::ref(std::cin), std::ref(std::cout));
  commands["COUNT"] = std::bind(count, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));
  commands["INFRAME"] = std::bind(inFrame, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));
  commands["RIGHTSHAPES"] = std::bind(rightShapes, std::cref(polygons), std::ref(std::cout));

  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      commands.at(command)();
      std::cout << '\n';
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

