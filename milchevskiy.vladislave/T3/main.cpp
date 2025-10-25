#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <functional>
#include <map>
#include <iterator>
#include <algorithm>
#include <limits>
#include "Shapes.hpp"
#include "Commands.hpp"

int main(int argc, char** argv)
{
  using namespace milchevskiy;
  if (argc != 2)
  {
    std::cerr << "<INVALID ARGUMENTS>\n";
    return 1;
  }
  std::ifstream file;
  file.open(argv[1]);
  if (!file.is_open())
  {
    std::cerr << "<INVALID FILE>\n";
    return 1;
  }
  std::vector< Polygon > polygons;
  using istr_iterator = std::istream_iterator< Polygon >;
  while (!file.eof())
  {
    std::copy(istr_iterator(file), istr_iterator(), std::back_inserter(polygons));
    if (!file)
    {
      file.clear();
      file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::map< std::string, std::function< void() > > cmds;
  cmds["AREA"] = std::bind(areaPrint, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["MAX"] = std::bind(maxPrint, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["MIN"] = std::bind(minPrint, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["COUNT"] = std::bind(countPrint, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["LESSAREA"] = std::bind(lessAreaPrint, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["MAXSEQ"] = std::bind(maxSeqPrint, std::ref(std::cin), std::ref(std::cout), std::ref(polygons));
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
      if (std::cin.fail())
      {
        std::cin.clear(std::cin.rdstate() ^ std::ios::failbit);
      }
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }
}
