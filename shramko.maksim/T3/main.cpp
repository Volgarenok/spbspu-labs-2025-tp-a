#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <functional>
#include <algorithm>
#include <iterator>
#include <limits>
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

  std::vector<shramko::Polygon> polygons;
  while (!inputFile.eof())
  {
    std::copy(std::istream_iterator<shramko::Polygon>(inputFile),
      std::istream_iterator<shramko::Polygon>(), std::back_inserter(polygons));
    if (inputFile.fail())
    {
      inputFile.clear();
      inputFile.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  using cmd_func = std::function< void() >;
  std::map< std::string, cmd_func > commands;

  commands["AREA"] = [&polygons]()
  {
    shramko::printArea(polygons, std::cin, std::cout);
  };

  commands["MAX"] = [&polygons]()
  {
    shramko::printMax(polygons, std::cin, std::cout);
  };

  commands["MIN"] = [&polygons]()
  {
    shramko::printMin(polygons, std::cin, std::cout);
  };

  commands["COUNT"] = [&polygons]()
  {
    shramko::printCount(polygons, std::cin, std::cout);
  };

  commands["LESSAREA"] = [&polygons]()
  {
    shramko::printLessArea(polygons, std::cin, std::cout);
  };

  commands["RIGHTSHAPES"] = [&polygons]()
  {
    shramko::printRightShapes(polygons, std::cout);
  };

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
      std::cout << "<INVALID COMMAND>\n";
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  return 0;
}
