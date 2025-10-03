#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <functional>
#include <iterator>
#include <limits>
#include "geom.hpp"
#include "commands.hpp"

int main(int argc, char** argv)
{
  using nikonov::Polygon;
  using nikonov::Point;

  if (argc != 2)
  {
    std::cerr << "<INCORRECT ARGUMENTS>\n";
    return 1;
  }

  const char* filename = argv[1];
  std::ifstream file(filename);
  if (!file)
  {
    std::cerr << "<INCORRECT FILE>\n";
    return 1;
  }
  std::vector< Polygon > polygons;
  while (!file.eof())
  {
    if (!file)
    {
      file.clear();
      file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
    std::copy(std::istream_iterator< Polygon >(file), std::istream_iterator< Polygon >(), std::back_inserter(polygons));
  }
  std::map< std::string, std::function< void(void) > > cmds;
  cmds["AREA"] = std::bind(nikonov::getArea, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));
  cmds["MAX"] = std::bind(nikonov::getMax, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));
  cmds["MIN"] = std::bind(nikonov::getMin, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));
  cmds["COUNT"] = std::bind(nikonov::getCount, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));
  cmds["PERMS"] = std::bind(nikonov::getPerms, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));
  cmds["MAXSEQ"] = std::bind(nikonov::getMaxSeq, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));
  std::string command;
  while (std::cin >> command)
  {
    try
    {
      auto it = cmds.find(command);
      if (it == cmds.end())
      {
        throw std::logic_error("<INVALID COMMAND>");
      }
      it->second();
      std::cout << '\n';
    }
    catch (const std::exception&)
    {
      if (std::cin.fail())
      {
        std::cin.clear(std::cin.rdstate() & ~std::ios::failbit);
      }
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  return 0;
}
