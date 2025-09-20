#include <map>
#include <limits>
#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <functional>
#include <fstream>
#include "geom.h"
#include "commands.h"

int main(int argc, char** argv)
{
  using namespace fedorova;

  if (argc != 2)
  {
    std::cerr << "<INCORRECT ARGUMENTS>\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file)
  {
    std::cerr << "<INCORRECT FILE\n>";
    return 1;
  }

  std::vector< Polygon > polygons;

  while (!file.eof())
  {
    if (file.fail())
    {
      file.clear();
      file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
    std::copy(std::istream_iterator< Polygon >(file), std::istream_iterator< Polygon >(), std::back_inserter(polygons));
  }

  std::map< std::string, std::function< void() > > cmds;

  cmds["AREA"] = std::bind(area, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));
  cmds["MAX"] = std::bind(max, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));
  cmds["MIN"] = std::bind(min, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));
  cmds["COUNT"] = std::bind(count, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));
  cmds["RIGHTSHAPES"] = std::bind(rightShapes, std::cref(polygons), std::ref(std::cout));
  cmds["PERMS"] = std::bind(perms, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));

  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      cmds.at(command)();
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

