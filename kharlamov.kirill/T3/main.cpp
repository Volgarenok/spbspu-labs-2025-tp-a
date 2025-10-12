#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <vector>
#include "commands.h"
#include "polygon.h"

int main(int argc, char** argv)
{
  using istreamIt = std::istream_iterator< kharlamov::Polygon >;
  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }
  std::ifstream file(argv[1]);
  std::vector< kharlamov::Polygon > polygons;
  while (!file.eof())
  {
    std::copy(istreamIt(file), istreamIt(), std::back_inserter(polygons));
    if (!file)
    {
      file.clear(file.rdstate() ^ std::ios::failbit);
      file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::map<std::string, std::function<void()>> commands;
  commands["AREA"] = std::bind(kharlamov::doAreaCommand, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["MAX"] = std::bind(kharlamov::doMaxCommand, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["MIN"] = std::bind(kharlamov::doMinCommand, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["COUNT"] = std::bind(kharlamov::doCountCommand, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["MAXSEQ"] = std::bind(kharlamov::doMaxSeqCommand, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["SAME"] = std::bind(kharlamov::doSameCommand, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));

  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      commands.at(cmd)();
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }
  return 0;
}
