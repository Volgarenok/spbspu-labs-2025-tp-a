#include <map>
#include <limits>
#include <vector>
#include <fstream>
#include <functional>
#include <iostream>
#include "geometry.hpp"
#include "commands.hpp"

int main(int argc, char** argv)
{
  using namespace cherkasov;
  if (argc != 2)
  {
    std::cerr << "Wrong parameters!\n";
    return 1;
  }
  std::ifstream input(argv[1]);
  if (!input)
  {
    std::cerr << "Wrong parameters!\n";
    return 1;
  }
  std::vector< Polygon > polygons;
  while (!input.eof())
  {
    Polygon poly;
    if (input >> poly)
    {
      polygons.push_back(poly);
    }
    else
    {
      input.clear();
      input.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::map< std::string, std::function< void() > > commands;
  commands["AREA"] = std::bind(doAreaComm, std::cref(polygons), std::ref(std::cout), std::ref(std::cin));
  commands["MAX"] = std::bind(doMaxComm, std::cref(polygons), std::ref(std::cout), std::ref(std::cin));
  commands["MIN"] = std::bind(doMinComm, std::cref(polygons), std::ref(std::cout), std::ref(std::cin));
  commands["COUNT"] = std::bind(doCountComm, std::cref(polygons), std::ref(std::cout), std::ref(std::cin));
  commands["INTERSECT"] = std::bind(doIntersectComm, std::cref(polygons), std::ref(std::cout), std::ref(std::cin));
  commands["RIGHTSHAPES"] = std::bind(doRightShapesComm, std::cref(polygons), std::ref(std::cout), std::ref(std::cin));
  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      commands.at(command)();
      std::cout << "\n";
    }
    catch (const std::exception&)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
    catch (...)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }
  return 0;
}
