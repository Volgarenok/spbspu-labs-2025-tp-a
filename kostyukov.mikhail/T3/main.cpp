#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <string>

#include "commandProcessor.hpp"
#include "geometry.hpp"

int main(int argc, char** argv)
{
  using namespace kostyukov;
  using iIter = std::istream_iterator< Polygon >;
  if (argc != 2)
  {
    std::cout << "Atleast 2 optional supported commands\n";
    return 1;
  }
  std::ifstream file(argv[1]);
  std::vector< Polygon > polygons;
  while (!file.eof())
  {
    std::copy(iIter(file), iIter(), std::back_inserter(polygons));
    if (file.fail())
    {
      file.clear(file.rdstate() ^ std::ios::failbit);
      file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  CommandProcessor processor(polygons, std::cin, std::cout);
  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      processor.process(command);
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
  return 0;
}
