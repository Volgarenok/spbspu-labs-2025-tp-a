#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <limits>
#include <iterator>
#include <algorithm>
#include <functional>
#include "shapes.hpp"
#include "commands.hpp"

int main(int argc, char * argv[])
{
  using sharifullina::Polygon;
  using istreamIter = std::istream_iterator< Polygon >;

  if (argc != 2)
  {
    std::cout << "wrong arguments\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file.is_open())
  {
    std::cout << "there is no such file\n";
    return 1;
  }

  std::vector< Polygon > polygons;
  while (!file.eof())
  {
    std::copy(istreamIter{file}, istreamIter{}, std::back_inserter(polygons));
    if (file.fail())
    {
      file.clear();
      file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  std::map< std::string, std::function< void() > > cmds;
  cmds["AREA"] = std::bind(sharifullina::printArea, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["MAX"] = std::bind(sharifullina::printMax, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["MIN"] = std::bind(sharifullina::printMin, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["COUNT"] = std::bind(sharifullina::printCount, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["PERMS"] = std::bind(sharifullina::printPerms, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["LESSAREA"] = std::bind(sharifullina::printLessArea, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));

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

  return 0;
}
