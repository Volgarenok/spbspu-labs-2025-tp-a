#include <cstddef>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <string>
#include <functional>
#include "polygon.hpp"
#include "commands.hpp"
int main(int argc, char** argv)
{
  using namespace kiselev;
  using istreamIt = std::istream_iterator< Polygon >;

  if (argc != 2)
  {
    std::cerr << "Incorrect parameters\n";
    return 1;
  }

  std::vector< Polygon > polygons;
  std::ifstream file(argv[1]);
  while (!file.eof())
  {
    std::copy(istreamIt(file), istreamIt(), std::back_inserter(polygons));
    if (!file)
    {
      file.clear(file.rdstate() ^ std::ios::failbit);
      file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  std::map< std::string, std::function< void() > > commands;
  commands["AREA"] = std::bind(doAreaCommand, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["MAX"] = std::bind(doMaxCommand, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["MIN"] = std::bind(doMinCommand, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["COUNT"] = std::bind(doCountCommand, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["LESSAREA"] = std::bind(doLessAreaCommand, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["RECTS"] = std::bind(doRectsCommand, std::ref(std::cout), std::cref(polygons));

  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      commands.at(command)();
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
