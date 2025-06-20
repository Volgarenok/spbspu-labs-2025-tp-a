#include <cstddef>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <stdexcept>
#include <vector>

#include "commands.hpp"
#include "polygon.hpp"
#include "polygon_iterator.hpp"

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }

  try
  {
    std::ifstream file(argv[1]);
    if (!file) throw std::runtime_error("Failed to open file");

    std::vector<ivanova::Polygon> polygons;
    std::copy(ivanova::PolygonIterator(file),
              ivanova::PolygonIterator(),
              std::back_inserter(polygons));

    std::string command;
    while (std::cin >> command)
    {
      try
      {
        if (command == "AREA") ivanova::area(std::cin, std::cout, polygons);
        else if (command == "MAX") ivanova::max(std::cin, std::cout, polygons);
        else if (command == "MIN") ivanova::min(std::cin, std::cout, polygons);
        else if (command == "COUNT") ivanova::count(std::cin, std::cout, polygons);
        else
        {
          std::cin.ignore(100, '\n');
          throw std::invalid_argument("<INVALID COMMAND>");
        }
      }
      catch (const std::exception& e)
      {
        std::cout << e.what() << "\n";
      }
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }

  return 0;
}
