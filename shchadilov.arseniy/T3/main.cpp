#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <vector>
#include "polygon.hpp"
#include "commands.hpp"

int main(int argc, char* argv[])
{
  using namespace shchadilov;

  if (argc != 2)
  {
    std::cerr << "Error: missing filename argument\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file)
  {
    std::cerr << "Error: cannot open file\n";
    return 1;
  }

  std::vector< Polygon > polygons;
  while (!file.eof())
  {
    Polygon poly;
    if (file >> poly)
    {
      polygons.push_back(poly);
    }
    else
    {
      file.clear();
      file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  std::map< std::string, std::function< void() > > commands;

  commands["AREA"] = std::bind(printArea, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["MAX"] = std::bind(printMax, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["MIN"] = std::bind(printMin, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["COUNT"] = std::bind(printCount, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["RIGHTSHAPES"] = std::bind(printRights, std::ref(std::cout), std::cref(polygons));
  commands["RMECHO"] = std::bind(printRmEcho, std::ref(std::cin), std::ref(std::cout), std::ref(polygons));

  std::string command;
  while (std::cin >> command)
  {
    try
    {
      commands.at(command)();
      std::cout << "\n";
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
