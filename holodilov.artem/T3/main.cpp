#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <functional>
#include <map>
#include "polygon.hpp"
#include "commands.hpp"

int main(int argc, char** argv)
{
  using namespace holodilov;

  if (argc != 2)
  {
    std::cout << "Error: enter filename as console argument:\n";
    std::cout << argv[0] << " <filename>\n";
    return 1;
  }

  std::ifstream fis(argv[1]);
  if (!fis.is_open())
  {
    std::cout << "Error: cannot open this file.\n";
    return 1;
  }

  using istreamIter = std::istream_iterator< Polygon >;
  VecPolygons vecPolygons;
  while (!fis.eof())
  {
    std::copy(istreamIter(fis), istreamIter(), std::back_inserter(vecPolygons));
    if (!fis)
    {
      fis.clear(fis.rdstate() ^ std::ios::failbit);
      fis.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  fis.close();

  std::map< std::string, std::function< void() > > cmds;
  cmds["AREA"] = std::bind(commands::area, std::ref(std::cin), std::ref(std::cout), std::cref(vecPolygons));
  cmds["MIN"] = std::bind(commands::min, std::ref(std::cin), std::ref(std::cout), std::cref(vecPolygons));
  cmds["MAX"] = std::bind(commands::max, std::ref(std::cin), std::ref(std::cout), std::cref(vecPolygons));
  cmds["COUNT"] = std::bind(commands::count, std::ref(std::cin), std::ref(std::cout), std::cref(vecPolygons));
  cmds["ECHO"] = std::bind(commands::echo, std::ref(std::cin), std::ref(std::cout), std::ref(vecPolygons));
  cmds["LESSAREA"] = std::bind(commands::lessArea, std::ref(std::cin), std::ref(std::cout), std::cref(vecPolygons));

  std::string commandName;
  while (!(std::cin >> commandName).eof())
  {
    try
    {
      cmds.at(commandName)();
      std::cout << "\n";
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
      if (std::cin.fail())
      {
        std::cin.clear(std::cin.rdstate() ^ std::ios::failbit);
      }
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  return 0;
}
