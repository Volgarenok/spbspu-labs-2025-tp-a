#include <iostream>
#include <fstream>
#include "geometry.hpp"
#include "polygon-commands.hpp"

int main(int argc, char* argv[])
{
  using namespace aleksandrov;

  if (argc != 2)
  {
    std::cerr << "ERROR: Incorrect arguments!\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file)
  {
    std::cerr << "ERROR: Incorrect file!\n";
    return 1;
  }

  std::vector< Polygon > polygons;
  getPolygons(file, polygons);

  try
  {
    processCommands(std::cin, std::cout, polygons);
  }
  catch (const std::exception& e)
  {
    std::cout << e.what() << '\n';
    return 1;
  }
}

