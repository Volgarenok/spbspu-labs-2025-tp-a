#include <list>
#include <map>
#include <functional>
#include <fstream>
#include <limits>
#include "geometric_shape.hpp"
#include "area_processing.hpp"
#include "vertex_processing.hpp"
#include "extremum_processing.hpp"
#include "utilities.hpp"

#include <iostream>
int main(int argc, char ** argv)
{
  if (argc != 2)
  {
    std::cerr << "<INVALID PARAMETER>" << '\n';
    return 1;
  }

  std::ifstream fin(argv[1]);
  if (!fin)
  {
    std::cerr << "<INVALID FILE STREAM>" << '\n';
    return 1;
  }

  std::list< zakirov::Polygon > shapes;
  zakirov::Polygon inserter;
  while (!fin.eof())
  {
    fin >> inserter;
    if (fin.fail() || fin.peek() != '\n')
    {
      fin.clear(fin.rdstate() ^ std::ios::failbit);
      fin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      continue;
    }

    shapes.push_back(inserter);
  }

  std::map< std::string, std::function< void(std::istream &, std::ostream &) > > commands;
  commands["AREA"] = std::bind(zakirov::process_area, std::cref(shapes), std::placeholders::_1, std::placeholders::_2);
  commands["LESSAREA"] = std::bind(zakirov::process_less_area, std::cref(shapes), std::placeholders::_1, std::placeholders::_2);
  commands["MAX"] = std::bind(zakirov::find_max_extremum, std::cref(shapes), std::placeholders::_1, std::placeholders::_2);
  commands["MIN"] = std::bind(zakirov::find_min_extremum, std::cref(shapes), std::placeholders::_1, std::placeholders::_2);
  commands["COUNT"] = std::bind(zakirov::count, std::cref(shapes), std::placeholders::_1, std::placeholders::_2);
  commands["ECHO"] = std::bind(zakirov::echo, std::ref(shapes), std::placeholders::_1, std::placeholders::_2);

  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      commands.at(command)(std::cin, std::cout);
      std::cout << '\n';
    }
    catch (const std::exception & e)
    {
      if (std::cin.fail())
      {
        std::cin.clear(std::cin.rdstate() ^ std::ios::failbit);
      }

      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>" << '\n';
    }
  }
}
