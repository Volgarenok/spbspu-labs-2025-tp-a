#include <functional>
#include <iostream>
#include <fstream>
#include <iterator>
#include <limits>
#include <map>
#include <vector>
#include "polygon.hpp"
#include "commands.hpp"

int main(int argc, char **argv)
{
  using namespace mazitov;
  if (argc != 2)
  {
    std::cerr << "<INVALID PARAMETERS COUNT>\n";
    return 1;
  }

  std::ifstream inputFile{argv[1]};
  if (!inputFile)
  {
    std::cerr << "<COUDNT'T READ THE FILE>\n";
    return 1;
  }

  std::vector< Polygon > polygons;
  while (!inputFile.eof())
  {
    if (inputFile.fail())
    {
      inputFile.clear(inputFile.rdstate() ^ std::ios::failbit);
      inputFile.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
    std::copy(std::istream_iterator< Polygon >(inputFile), std::istream_iterator< Polygon >(),
              std::back_inserter(polygons));
  }
  inputFile.close();

  std::map< std::string, std::function< void(std::istream &, std::ostream &) > > commands
  {
      {"AREA", std::bind(areaCommand, std::ref(std::cin), std::ref(std::cout), std::cref(polygons))},
      {"MAX", std::bind(maxCommand, std::ref(std::cin), std::ref(std::cout), std::cref(polygons))},
      {"MIN", std::bind(minCommand, std::ref(std::cin), std::ref(std::cout), std::cref(polygons))},
      {"COUNT", std::bind(countCommand, std::ref(std::cin), std::ref(std::cout), std::cref(polygons))},
      {"INTERSECTIONS", std::bind(intersectsCommand, std::ref(std::cin), std::ref(std::cout), std::cref(polygons))},
      {"RMECHO", std::bind(rmEchoCommand, std::ref(std::cin), std::ref(std::cout), std::ref(polygons))},
  };

  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      commands.at(command)(std::cin, std::cout);
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
}
