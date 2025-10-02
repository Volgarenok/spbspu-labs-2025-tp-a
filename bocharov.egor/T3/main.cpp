#include <functional>
#include <iterator>
#include <iostream>
#include <fstream>
#include <limits>
#include <map>
#include "commands.hpp"

int main(int argc, char * argv[])
{
  using namespace bocharov;

  if (argc != 2)
  {
    std::cerr << "ERROR: wrong arguments\n";
    return 1;
  }
  std::ifstream file(argv[1]);
  if (!file.is_open())
  {
    std::cout << "ERROR: there is no such file\n";
    return 1;
  }

  using iIterator = std::istream_iterator< Polygon >;
  std::vector< Polygon > polygons;
  while (!file.eof())
  {
    if (file.fail())
    {
      file.clear();
      file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
    std::copy(iIterator(file), iIterator(), std::back_inserter(polygons));
  }

  std::map< std::string, std::function< void() > > cmds;
  cmds["AREA"] = std::bind(getArea, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["MAX"] = std::bind(getMax, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["MIN"] = std::bind(getMin, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["COUNT"] = std::bind(getCount, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["MAXSEQ"] = std::bind(getMaxSeqCommand, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["RIGHTSHAPES"] = std::bind(getRightsCnt, std::ref(std::cout), std::cref(polygons));

  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      cmds.at(command)();
      std::cout << '\n';
    }
    catch (const std::exception &)
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


