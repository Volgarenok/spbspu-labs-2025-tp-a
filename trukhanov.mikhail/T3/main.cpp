#include <functional>
#include <iostream>
#include <sstream>
#include <fstream>
#include <limits>
#include <map>
#include "commands.hpp"
#include "polygon.hpp"
#include "data_input.hpp"

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "ERROR: wrong arguments\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file.is_open())
  {
    std::cerr << "ERROR: there is no such file\n";
    return 1;
  }

  using trukhanov::Polygon;
  std::vector< Polygon > polygons;
  std::string line;

  while (std::getline(file, line))
  {
    if (line.empty())
    {
      continue;
    }

    std::istringstream iss(line);
    Polygon poly;
    iss >> poly;

    if (iss && poly.points.size() >= 3)
    {
      polygons.push_back(std::move(poly));
    }
  }

  std::map< std::string, std::function< void() > > cmds;
  cmds["AREA"] = std::bind(trukhanov::area, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["MAX"] = std::bind(trukhanov::max, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["MIN"] = std::bind(trukhanov::min, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["COUNT"] = std::bind(trukhanov::count, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["LESSAREA"] = std::bind(trukhanov::lessArea, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["RIGHTSHAPES"] = std::bind(trukhanov::right, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));

  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      cmds.at(command)();
    }
    catch (const std::exception&)
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
