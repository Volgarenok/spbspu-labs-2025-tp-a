#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <string>

#include "commands.hpp"
#include "geometry.hpp"

int main(int argc, char** argv)
{
  using namespace kostyukov;
  using iIter = std::istream_iterator< Polygon >;
  if (argc != 2)
  {
    std::cout << "Atleast 2 optional supported commands\n";
    return 1;
  }
  std::ifstream file(argv[1]);
  std::vector< Polygon > polygons;
  while (!file.eof())
  {
    std::copy(iIter(file), iIter(), std::back_inserter(polygons));
    if (!file)
    {
      file.clear();
      file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::map< std::string, std::function< void() > > commands;
  commands["AREA"] = std::bind(area, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["MAX"] = std::bind(max, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["MIN"] = std::bind(min, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["COUNT"] = std::bind(count, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["PERMS"] = std::bind(permsCount, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["RIGHTSHAPES"] = std::bind(rightShapesCount, std::ref(std::cout), std::cref(polygons));
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
  return 0;
}
