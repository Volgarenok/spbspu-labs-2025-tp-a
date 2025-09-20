#include <iterator>
#include <iostream>
#include <vector>
#include <fstream>
#include <limits>
#include <map>
#include <string>
#include <functional>
#include <iomanip>
#include "io_polygon.hpp"
#include "commands.hpp"

namespace
{
  int checkArguments(const int argc);

  int checkArguments(const int argc)
  {
    if (argc == 2)
    {
      return 1;
    }
    else
    {
      return 0;
    }
  }
}

int main(int argc, const char * const * argv)
{
  using namespace petrov;
  using polygon_istream_it = std::istream_iterator< Polygon >;
  if (!checkArguments(argc))
  {
    std::cerr << "ERROR: Incorrect number of arguments";
    std::cerr << "\n";
    return 1;
  }
  std::vector< Polygon > polygons;
  std::ifstream input(argv[1]);
  while (!input.eof())
  {
    if (!input)
    {
      input.clear();
      input.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
    std::copy(polygon_istream_it(input), polygon_istream_it(), std::back_inserter(polygons));
  }
  std::map< std::string, std::function< void() > > cmds;
  cmds["AREA"] = std::bind(area, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));
  cmds["MAX"] = std::bind(max, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));
  cmds["MIN"] = std::bind(min, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));
  cmds["COUNT"] = std::bind(count, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));
  cmds["RMECHO"] = std::bind(rmecho, std::ref(polygons), std::ref(std::cin), std::ref(std::cout));
  cmds["MAXSEQ"] = std::bind(maxseq, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));
  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      cmds.at(command)();
      std::cout << "\n";
    }
    catch (...)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>";
      std::cout << "\n";
    }
  }
}
