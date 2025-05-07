#include <iostream>
#include <limits>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <map>
#include <functional>
#include "geometry.hpp"
#include "commands.hpp"

int main(int argc, char* argv[])
{
  using demehin::Polygon;
  using istrIter = std::istream_iterator< Polygon >;

  if (argc != 2)
  {
    return 1;
  }

  std::ifstream file(argv[1]);
  std::vector< Polygon > plgs;
  while (!file.eof())
  {
    std::copy(istrIter(file), istrIter(), std::back_inserter(plgs));
    if (!file)
    {
      file.clear();
      file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  std::map< std::string, std::function< void() > > cmds;
  cmds["AREA"] = std::bind(demehin::printAreaSum, std::ref(std::cin), std::cref(plgs), std::ref(std::cout));
  cmds["MAX"] = std::bind(demehin::printMaxValueOf, std::ref(std::cin), std::cref(plgs), std::ref(std::cout));
  cmds["MIN"] = std::bind(demehin::printMinValueOf, std::ref(std::cin), std::cref(plgs), std::ref(std::cout));
  cmds["COUNT"] = std::bind(demehin::printCountOf, std::ref(std::cin), std::cref(plgs), std::ref(std::cout));
  cmds["PERMS"] = std::bind(demehin::printPermsCnt, std::ref(std::cin), std::cref(plgs), std::ref(std::cout));
  cmds["RIGHTSHAPES"] = std::bind(demehin::printRightsCnt, std::cref(plgs), std::ref(std::cout));

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
