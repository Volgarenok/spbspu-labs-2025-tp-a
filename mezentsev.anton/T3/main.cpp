#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include "Commands.h"
#include "Polygon.h"

namespace
{
  void input(std::istream& in, std::vector< mezentsev::Polygon >& poly)
  {
    using istreamIt = std::istream_iterator< mezentsev::Polygon >;
    while (!in.eof())
    {
      std::copy(istreamIt(in), istreamIt(), std::back_inserter(poly));
      if (!in)
      {
        in.clear(in.rdstate() ^ std::ios::failbit);
        in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      }
    }
  }
}
int main(int argc, char** argv)
{
  using namespace mezentsev;
  if (argc != 2)
  {
    std::cerr << "Parameters are wrong\n";
    return 1;
  }
  std::vector< Polygon > polygons;
  std::ifstream file(argv[1]);
  input(file, polygons);
  std::map< std::string, std::function< void() > > commands;
  commands["AREA"] = std::bind(area, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["MAX"] = std::bind(max, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["MIN"] = std::bind(min, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["COUNT"] = std::bind(count, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["LESSAREA"] = std::bind(lessArea, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["MAXSEQ"] = std::bind(maxSeq, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      commands.at(command)();
    }
    catch (const std::exception&)
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
