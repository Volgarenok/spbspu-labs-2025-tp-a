#include <functional>
#include <iostream>
#include <fstream>
#include <istream>
#include <iterator>
#include <limits>
#include <map>
#include <vector>
#include "polygon.hpp"
#include "commands.hpp"

int main(int argc, char **argv)
{
  using pilugina::Polygon;
  using is_it_poly = std::istream_iterator< Polygon >;
  if (argc != 2)
  {
    std::cerr << "<INVALID PARAMETERS NUMBER>\n";
    return 1;
  }

  std::ifstream inputFile{argv[1]};
  if (!inputFile)
  {
    std::cerr << "<INVALID PARAMETERS NUMBER>\n";
    return 1;
  }

  std::vector< Polygon > polygons;
  while (!inputFile.eof())
  {
    if (inputFile.fail())
    {
      inputFile.clear(inputFile.rdstate() ^ std::ios::failbit);
      inputFile.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
    std::copy(is_it_poly{inputFile}, is_it_poly{}, std::back_inserter(polygons));
  }
  inputFile.close();

  using namespace std::placeholders;
  std::map< std::string, std::function< void(std::istream &, std::ostream &) > > commands
  {
    {"AREA", std::bind(pilugina::areaCommand, _1, _2, std::cref(polygons))},
    {"MAX", std::bind(pilugina::maxCommand, _1, _2, std::cref(polygons))},
    {"MIN", std::bind(pilugina::minCommand, _1, _2, std::cref(polygons))},
    {"COUNT", std::bind(pilugina::countCommand, _1, _2, std::cref(polygons))},
    {"ECHO", std::bind(pilugina::echoCommand, _1, _2, std::ref(polygons))},
    {"INTERSECTIONS", std::bind(pilugina::intersectsCommand, _1, _2, std::cref(polygons))},
    {"PERMS", std::bind(pilugina::permsCommand, _1, _2, std::cref(polygons))},
    {"PERIMETER", std::bind(pilugina::perimeterCommand, _1, _2, std::cref(polygons))}
  };

  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      commands.at(command)(std::cin, std::cout);
      std::cout << '\n';
    }
    catch (...)
    {
      if (std::cin.fail())
      {
        std::cin.clear(std::cin.rdstate() ^ std::ios::failbit);
      }
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }
}
