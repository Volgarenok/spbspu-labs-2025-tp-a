#include "commands.hpp"

#include <algorithm>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <vector>

int main(int argc, char *argv[])
{
  using namespace filonova;
  if (argc != 2)
  {
    std::cerr << "<INVALID COMMAND>\n";
    return 1;
  }
  else
  {
    std::ifstream in(argv[1]);
    if (!in.is_open())
    {
      std::cerr << "<INVALID COMMAND>\n";
      return 1;
    }
    std::vector< Polygon > polygons;
    using input_it_t = std::istream_iterator< Polygon >;
    while (!in.eof())
    {
      std::copy(input_it_t{in}, input_it_t{}, std::back_inserter(polygons));
      if (!in)
      {
        in.clear();
        in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      }
    }

    std::map< std::string, std::function< void(std::istream &, std::ostream &) > > cmds;
    {
      using namespace std::placeholders;
      cmds["AREA"] = std::bind(area, _1, _2, std::cref(polygons));
      cmds["MAX"] = std::bind(max, _1, _2, std::cref(polygons));
      cmds["MIN"] = std::bind(min, _1, _2, std::cref(polygons));
      cmds["COUNT"] = std::bind(count, _1, _2, std::cref(polygons));
      cmds["INTERSECTIONS"] = std::bind(intersections, _1, _2, std::cref(polygons));
      cmds["RECTS"] = std::bind(rects, _2, std::cref(polygons));

      std::string command;
      while (std::cin >> command)
      {
        try
        {
          cmds.at(command)(std::cin, std::cout);
          std::cout << '\n';
        }
        catch (...)
        {
          std::cout << "<INVALID COMMAND>" << '\n';
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      }
    }
  }
}
