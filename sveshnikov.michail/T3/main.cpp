#include <map>
#include <limits>
#include <fstream>
#include <functional>
#include "commands.hpp"

int main(int argc, char **argv)
{
  using namespace sveshnikov;
  if (argc != 2)
  {
    std::cerr << "Error: file is not specified!\n";
    return 1;
  }
  std::ifstream in(argv[1]);
  if (!in.is_open())
  {
    std::cerr << "Error: cannot open file!\n";
    return 1;
  }
  std::vector< Polygon > shapes;
  loadPolygons(in, shapes);

  using is_t = std::istream;
  using os_t = std::ostream;
  using polygon_set_t = std::vector< Polygon >;
  std::map< std::string, std::function< void(is_t &, os_t &, polygon_set_t &) > > cmds;
  cmds["AREA"] = area;
  cmds["MAX"] = max;
  cmds["MIN"] = min;
  cmds["COUNT"] = count;
  cmds["MAXSEQ"] = maxseq;
  cmds["RMECHO"] = rmecho;

  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      cmds.at(command)(std::cin, std::cout, shapes);
    }
    catch (...)
    {
      if (std::cin.fail())
      {
        std::cin.clear(std::cin.rdstate() ^ std::ios::failbit);
      }
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>" << '\n';
    }
  }
  return 0;
}
