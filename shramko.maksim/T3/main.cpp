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
  shramko::Polygon temp;
  while (inputFile >> temp)
  {
    polygons.push_back(temp);
    inputFile.clear();
    inputFile.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
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
      auto it = commands.find(cmd);
      if (it != commands.end())
      {
        it->second();
        std::cout << '\n';
      }
      else
      {
        throw std::invalid_argument("Invalid command");
      }
    }
    catch (const std::exception& e)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  return 0;
}
