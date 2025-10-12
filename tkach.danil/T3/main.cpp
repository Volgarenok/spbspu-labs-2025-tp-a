#include <iostream>
#include <fstream>
#include <limits>
#include <iterator>
#include <algorithm>
#include <functional>
#include <string>
#include <map>
#include "shapes.hpp"
#include "commands.hpp"

int main(const int argc, const char* const* const argv)
{
  using namespace tkach;
  using istreamIT = std::istream_iterator< Polygon >;
  if (argc != 2)
  {
    std::cerr << "Error: incorrect input\n";
    return 1;
  }
  std::fstream in(argv[1]);
  if (!in.is_open())
  {
    std::cerr << "File is not open\n";
    return 1;
  }
  std::vector< Polygon > data;
  while (!in.eof())
  {
    std::copy(istreamIT{in}, istreamIT{}, std::back_inserter(data));
    if (in.fail())
    {
      in.clear(in.rdstate() ^ std::ios::failbit);
      in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::map< std::string, std::function< void() > > cmds;
  cmds["AREA"] = std::bind(printArea, std::ref(std::cin), std::ref(std::cout), std::cref(data));
  cmds["MAX"] = std::bind(printMax, std::ref(std::cin), std::ref(std::cout), std::cref(data));
  cmds["MIN"] = std::bind(printMin, std::ref(std::cin), std::ref(std::cout), std::cref(data));
  cmds["COUNT"] = std::bind(printCount, std::ref(std::cin), std::ref(std::cout), std::cref(data));
  cmds["SAME"] = std::bind(printSame, std::ref(std::cin), std::ref(std::cout), std::cref(data));
  cmds["RECTS"] = std::bind(printRects, std::ref(std::cout), std::cref(data));
  std::string command;
  while (!(std::cin >> command).eof()) {
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
