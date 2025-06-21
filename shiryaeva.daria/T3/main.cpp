#include "commands.hpp"
#include "polygon.hpp"
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <vector>

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " filename\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file)
  {
    std::cerr << "Error opening file\n";
    return 1;
  }

  using Polygon = shiryaeva::Polygon;
  using PolygonIter = std::istream_iterator< Polygon >;
  std::vector< Polygon > polygons;

  while (!file.eof())
  {
    std::copy(PolygonIter(file), PolygonIter(), std::back_inserter(polygons));
    if (file.fail() && !file.eof())
    {
      file.clear();
      file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  std::map< std::string, std::function< void(std::istream&, std::ostream&) >> commands = {
        {"AREA", std::bind(shiryaeva::area, std::placeholders::_1, std::placeholders::_2, polygons)},
        {"MAX", std::bind(shiryaeva::max, std::placeholders::_1, std::placeholders::_2, polygons)},
        {"MIN", std::bind(shiryaeva::min, std::placeholders::_1, std::placeholders::_2, polygons)},
        {"COUNT", std::bind(shiryaeva::count, std::placeholders::_1, std::placeholders::_2, polygons)},
        {"LESSAREA", std::bind(shiryaeva::lessarea, std::placeholders::_1, std::placeholders::_2, polygons)},
        {"INTERSECTIONS", std::bind(shiryaeva::intersections, std::placeholders::_1, std::placeholders::_2, polygons)}
    };

  std::string command;
  while (std::cin >> command)
  {
    try
    {
      auto it = commands.find(command);
      if (it != commands.end())
      {
        it->second(std::cin, std::cout);
      }
      else
      {
        throw std::invalid_argument("<INVALID COMMAND>");
      }
    }
    catch (const std::exception& e)
    {
        std::cout << e.what();
    }
    std::cout << '\n';
    std::cin.clear();
    std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }

    return 0;
}

