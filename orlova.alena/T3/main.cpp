#include <iostream>
#include <functional>
#include <algorithm>
#include <limits>
#include <vector>
#include <map>
#include <fstream>
#include <iterator>
#include "geom.h"
#include "commands.h"

int main(int argc, char** argv)
{
  using namespace orlova;

  std::vector< Polygon > polygons;

  if (argc != 2)
  {
    std::cerr << "<INCORRECT ARGUMENTS>\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file)
  {
    std::cerr << "<INCORRECT FILE>\n";
    return 1;
  }

  while (!file.eof())
  {
    if (!file)
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
  cmds["PERMS"] = std::bind(perms, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));
  cmds["MAXSEQ"] = std::bind(maxseq, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));

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
