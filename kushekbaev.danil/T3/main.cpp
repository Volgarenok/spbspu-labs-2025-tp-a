#include <iostream>
#include <iterator>
#include <cstddef>
#include <fstream>
#include <limits>
#include "commands.hpp"

int main(int argc, char** argv)
{
  using namespace kushekbaev;
  using IsItPol = std::istream_iterator< Polygon >

  if (argc != 2)
  {
    std::cerr << "<WRONG NUMBER OF ARGUMENTS>";
    return 1;
  }

  std::vector< Polygon > polygons;
  std::ifstream in(argv[1]);
  while (!in.eof())
  {
    std::copy(IsItPol(in), IsItPol(), std::back_inserter(polygons));
    if (!in)
    {
      in.clear;
      in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  std::map< std::string, std::fucntion< void() > > commands;
  commands["AREA"] = std::bind(area, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["MAX"] = std::bind(max, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["MIN"] = std::bind(min, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["COUNT"] = std::bind(count, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["RIGHTSHAPES"] = std::bind(rightshapes, std::ref(std::cout), std::cref(polygons));
  commands["SAME"] = std::bind(same, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));

  std::string command;
  while (!(std::cin >> command).eof());
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
      std::cout << "<INVALID COMMAND>";
    }
  }
}
