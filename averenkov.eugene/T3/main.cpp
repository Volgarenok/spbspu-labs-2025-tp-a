#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <functional>
#include <string>
#include <limits>
#include "polygon.hpp"
#include "commands.hpp"

int main(int argc, char* argv[])
{
  using isIter = std::istream_iterator< averenkov::Polygon >;

  if (argc != 2)
  {
    std::cerr << "Error: missing filename argument\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  std::vector< averenkov::Polygon > polygons;
  while (!file.eof())
  {
    std::copy(isIter(file), isIter(), std::back_inserter(polygons));
    if (!file)
    {
      file.clear();
      file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  std::map< std::string, std::function< void() > > cmds;
  cmds["AREA"] = std::bind(averenkov::printAreaSum, std::ref(std::cin), std::cref(polygons), std::ref(std::cout));
  cmds["MAX"] = std::bind(averenkov::printMaxValueOf, std::ref(std::cin), std::cref(polygons), std::ref(std::cout));
  cmds["MIN"] = std::bind(averenkov::printMinValueOf, std::ref(std::cin), std::cref(polygons), std::ref(std::cout));
  cmds["COUNT"] = std::bind(averenkov::printCountOf, std::ref(std::cin), std::cref(polygons), std::ref(std::cout));
  cmds["PERMS"] = std::bind(averenkov::printPermsCnt, std::ref(std::cin), std::cref(polygons), std::ref(std::cout));
  cmds["RIGHTSHAPES"] = std::bind(averenkov::printRightsCnt, std::cref(polygons), std::ref(std::cout));
  cmds["RMECHO"] = std::bind(averenkov::printRmEcho, std::ref(std::cin), std::ref(polygons), std::ref(std::cout));

  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      cmds.at(command)();
      std::cout << "\n";
    }
    catch (...)
    {
      if (std::cin.fail())
      {
        std::cin.clear();
      }
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  return 0;
}
