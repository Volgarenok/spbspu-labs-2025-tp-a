#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <limits>
#include <map>
#include <functional>
#include "cmds.hpp"
#include "GeometricalTypes.hpp"

int main(int argc, char* argv[])
{
  using shapkov::Polygon;

  if (argc != 2)
  {
    return 1;
  }
  std::ifstream file(argv[1]);
  std::vector< Polygon > data;
  using inputIt = std::istream_iterator< Polygon >;
  while (!file.eof())
  {
    std::copy(inputIt{ file }, inputIt{}, std::back_inserter(data));
    if (!file)
    {
      file.clear();
      file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  std::map< std::string, std::function< void() > > cmds;
  cmds["AREA"] = std::bind(shapkov::area, std::ref(std::cin), std::ref(std::cout), std::cref(data));
  cmds["MAX"] = std::bind(shapkov::max, std::ref(std::cin), std::ref(std::cout), std::cref(data));
  cmds["MIN"] = std::bind(shapkov::min, std::ref(std::cin), std::ref(std::cout), std::cref(data));
  cmds["COUNT"] = std::bind(shapkov::count, std::ref(std::cin), std::ref(std::cout), std::cref(data));
  cmds["RECTS"] = std::bind(shapkov::rects, std::ref(std::cout), std::cref(data));
  cmds["SAME"] = std::bind(shapkov::same, std::ref(std::cin), std::ref(std::cout), std::cref(data));

  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      cmds.at(command)();
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
  return 0;
}
