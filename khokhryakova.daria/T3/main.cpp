#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <string>
#include <functional>
#include "commands.h"
#include "poligon.h"

int main(int argc, char** argv)
{
  using namespace khokhryakova;
  using istreamIt = std::istream_iterator< Polygon >;

  if (argc != 2)
  {
    std::cerr << "Incorrect parameters\n";
    return 1;
  }

  std::vector< Polygon > polygon;
  std::ifstream file(argv[1]);
  while (!file.eof())
  {
    std::copy(istreamIt(file), istreamIt(), std::back_inserter(polygon));
    if (!file)
    {
      file.clear();
      file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  std::map< std::string, std::function< void() > > commands;
  const std::string filename = argv[1];

  commands["AREA"] = std::bind(area, std::ref(std::cin), std::ref(std::cout), std::cref(polygon));
  commands["MAX"] = std::bind(max, std::ref(std::cin), std::ref(std::cout), std::cref(polygon));
  commands["MIN"] = std::bind(min, std::ref(std::cin), std::ref(std::cout), std::cref(polygon));
  commands["COUNT"] = std::bind(count, std::ref(std::cin), std::ref(std::cout), std::cref(polygon));
  commands["ECHO"] = std::bind(echo, std::ref(std::cin), std::ref(std::cout), std::ref(polygon), argv[1]);
  commands["MAXSEQ"] = std::bind(maxSeq, std::ref(std::cin), std::ref(std::cout), std::cref(polygon));

  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      commands.at(command)();
      std::cout << '\n';
    }
    catch (...)
    {
      if (std::cin.fail())
      {
        std::cin.clear();
      }
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  return 0;
}
