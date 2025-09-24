#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <functional>
#include <algorithm>
#include <iterator>
#include <limits>
#include <ios>
#include "polygon.hpp"
#include "printCmd.hpp"

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "Usage: program <input_file>\n";
    return 1;
  }

  std::ifstream inputFile(argv[1]);
  if (!inputFile)
  {
    std::cerr << "Cannot open input file\n";
    return 1;
  }

  std::vector< shramko::Polygon > polygons;
  while (!inputFile.eof())
  {
    std::copy(std::istream_iterator< shramko::Polygon >(inputFile), std::istream_iterator< shramko::Polygon >(), std::back_inserter(polygons));
    if (inputFile.fail())
    {
      inputFile.clear();
      inputFile.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  using cmd_func = std::function< void() >;
  std::map< std::string, cmd_func > commands;

  commands["AREA"] = std::bind(&shramko::printArea, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));
  commands["MAX"] = std::bind(&shramko::printMax, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));
  commands["MIN"] = std::bind(&shramko::printMin, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));
  commands["COUNT"] = std::bind(&shramko::printCount, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));
  commands["LESSAREA"] = std::bind(&shramko::printLessArea, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));
  commands["RIGHTSHAPES"] = std::bind(&shramko::printRightShapes, std::cref(polygons), std::ref(std::cout));

  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      commands.at(cmd)();
      std::cout << '\n';
    }
    catch (const std::exception&)
    {
      if (std::cin.fail())
      {
        std::cin.clear(std::cin.rdstate() ^ std::ios::failbit);
      }
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  return 0;
}