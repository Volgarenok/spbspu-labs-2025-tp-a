#include <iostream>
#include <fstream>
#include <map>
#include <iterator>
#include <functional>
#include <vector>
#include <limits>
#include <iomanip>
#include "shapes.hpp"
#include "user-commands.hpp"

int main(int argc, char** argv)
{
  using namespace alymova;
  using namespace std::placeholders;
  using CommandDataset = std::map< std::string, std::function< void(const std::vector< Polygon >&) > >;

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
  while (!file.eof())
  {
    if (file.fail())
    {
      file.clear(file.rdstate() ^ std::ios_base::failbit);
      file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
    std::copy(std::istream_iterator< Polygon >(file), std::istream_iterator< Polygon >(), std::back_inserter(polygons));
  }

  CommandDataset commands;
  commands["AREA"] = std::bind(area, std::ref(std::cin), std::ref(std::cout), _1);
  commands["MAX"] = std::bind(max, std::ref(std::cin), std::ref(std::cout), _1);
  commands["MIN"] = std::bind(min, std::ref(std::cin), std::ref(std::cout), _1);
  commands["COUNT"] = std::bind(count, std::ref(std::cin), std::ref(std::cout), _1);
  commands["INFRAME"] = std::bind(inFrame, std::ref(std::cin), std::ref(std::cout), _1);
  commands["RIGHTSHAPES"] = std::bind(rightShapes, std::ref(std::cout), _1);
  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      commands.at(command)(std::cref(polygons));
      std::cout << '\n';
    }
    catch (const std::exception& e)
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
