#include <iostream>
#include <vector>
#include <map>
#include <functional>
#include <fstream>
#include <limits>
#include "shapes.hpp"
#include "commands.hpp"

int main(int argc, char* argv[])
{
  using namespace cherepkov;

  if (argc != 2)
  {
    std::cerr << "ERROR: Incorrect number of arguments\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file)
  {
    std::cerr << "ERROR: the file is unreadable\n";
    return 1;
  }

  std::vector< Polygon > polygons;

  using input_iter = std::istream_iterator< Polygon >;
  while (!file.eof())
  {
    std::copy(input_iter{file}, input_iter{}, std::back_inserter(polygons));
    if (!file)
    {
      file.clear();
      file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  std::map< std::string, std::function< void() > > cmds;
  cmds["AREA"] = std::bind(area, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["MAX"] = std::bind(max, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["MIN"] = std::bind(min, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["COUNT"] = std::bind(count, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["INFRAME"] = std::bind(inFrame, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["SAME"] = std::bind(same, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));

  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      cmds.at(command)();
    }
    catch (...)
    {
      if (std::cin.fail() || std::cin.eof())
      {
        std::cin.clear(std::cin.rdstate() ^ std::ios::failbit);
      }
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }
  return 0;
}
