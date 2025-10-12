#include <functional>
#include <iterator>
#include <iostream>
#include <fstream>
#include <limits>
#include <map>
#include "commands.hpp"

int main(int argc, char * argv[])
{
  if (argc != 2)
  {
    std::cerr << "ERROR: wrong arguments\n";
    return 1;
  }
  std::ifstream file(argv[1]);
  if (!file.is_open())
  {
    std::cout << "ERROR: there is no such file\n";
    return 1;
  }
  using maslov::Polygon;
  using iIterator = std::istream_iterator< Polygon >;
  std::vector< Polygon > polygons;
  while (!file.eof())
  {
    if (file.fail())
    {
      file.clear();
      file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
    std::copy(iIterator(file), iIterator(), std::back_inserter(polygons));
  }

  std::map< std::string, std::function< void() > > cmds;
  cmds["AREA"] = std::bind(maslov::getArea, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["MAX"] = std::bind(maslov::getMax, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["MIN"] = std::bind(maslov::getMin, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["COUNT"] = std::bind(maslov::getCount, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["ECHO"] = std::bind(maslov::getEcho, std::ref(std::cin), std::ref(std::cout), std::ref(polygons));
  cmds["RECTS"] = std::bind(maslov::getRects, std::ref(std::cout), std::cref(polygons));

  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      cmds.at(command)();
    }
    catch (const std::exception &)
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
