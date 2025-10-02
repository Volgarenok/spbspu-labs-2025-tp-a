#include <iterator>
#include <iostream>
#include <vector>
#include <fstream>
#include <limits>
#include <map>
#include <string>
#include <functional>
#include <data_input.hpp>
#include "commands.hpp"
#include "polygon.hpp"

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "ERROR: wrong arguments\n";
    return 1;
  }

  std::ifstream input(argv[1]);
  if (!input.is_open())
  {
    std::cerr << "ERROR: there is no such file\n";
    return 1;
  }

  using trukhanov::Polygon;
  using polygon_it = std::istream_iterator< Polygon >;

  std::vector< Polygon > polygons;
  while (!input.eof() && !input.bad())
  {
    std::copy(polygon_it(input), polygon_it(), std::back_inserter(polygons));
    if (!input)
    {
      input.clear(input.rdstate() ^ std::ios_base::failbit);
      input.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  std::map< std::string, std::function< void() > > cmds;
  cmds["AREA"] = std::bind(trukhanov::area, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["MAX"] = std::bind(trukhanov::max, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["MIN"] = std::bind(trukhanov::min, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["COUNT"] = std::bind(trukhanov::count, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["LESSAREA"] = std::bind(trukhanov::lessArea, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["RIGHTSHAPES"] = std::bind(trukhanov::right, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["COMMONSIDE"] = std::bind(trukhanov::commonSide, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));

  std::string command;
  while (!(std::cin >> command).eof() && !std::cin.bad())
  {
    try
    {
      cmds.at(command)();
    }
    catch (...)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }
  return 0;
}
