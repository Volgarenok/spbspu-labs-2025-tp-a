#include <fstream>
#include "commands.hpp"

int main(int argc, char* argv[])
{
  using namespace nikitin;
  using isIt = std::istream_iterator< Polygon >;

  if (argc != 2)
  {
    std::cerr << "Error: invalid arguments.\n";
    return 1;
  }

  std::fstream file(argv[1]);
  if (!file.is_open())
  {
    std::cerr << "Error: cannot open the file.\n";
    return 1;
  }

  std::vector< Polygon > polygons;
  while (!file.eof())
  {
    std::copy(isIt{file}, isIt{}, std::back_inserter(polygons));
    if (file)
    {
      file.clear(file.rdstate() ^ std::ios::failbit);
      file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  file.close();

  std::map< std::string, std::function< void() > > commands;
  commands["AREA"] = std::bind(areaPrint, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["MAX"] = std::bind(maxPrint, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["MIN"] = std::bind(minPrint, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["COUNT"] = std::bind(countPrint, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["INFRAME"] = std::bind(inframePrint, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["RIGHTSHAPES"] = std::bind(rightshapesPrint, std::ref(std::cout), std::cref(polygons));
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
        std::cin.clear(std::cin.rdstate() ^ std::ios::failbit);
      }
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }
}
