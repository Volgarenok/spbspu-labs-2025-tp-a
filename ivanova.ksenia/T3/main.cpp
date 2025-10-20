#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <iterator>
#include <limits>
#include "polygon.hpp"
#include "polygon_iterator.hpp"
#include "commands.hpp"

int main()
{
  using namespace ivanova;

  std::vector< Polygon > polygons;
  std::map< std::string, std::function< void(std::istream&, std::ostream&, std::vector< Polygon >&) > > commands;

  commands["AREA"] = area;
  commands["MAX"] = max;
  commands["MIN"] = min;
  commands["COUNT"] = count;
  commands["ECHO"] = echo;
  commands["SAME"] = same;

  std::copy(
    PolygonIterator(std::cin),
    PolygonIterator(),
    std::back_inserter(polygons)
  );

  std::cin.clear();
  std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');

  std::string command;
  while (std::cin >> command)
  {
    try
    {
      auto it = commands.find(command);
      if (it != commands.end())
      {
        it->second(std::cin, std::cout, polygons);
      }
      else
      {
        throw std::invalid_argument("<INVALID COMMAND>");
      }
    }
    catch (const std::exception& e)
    {
      std::cout << e.what() << '\n';
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  return 0;
}
