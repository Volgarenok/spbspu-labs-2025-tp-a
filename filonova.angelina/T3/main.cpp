#include "commands.hpp"
#include <fstream>
#include <iterator>
#include <limits>
#include <map>

using namespace filonova;

int main(int argc, char *argv[])
{
  using istrIter = std::istream_iterator< Polygon >;

  if (argc != 2)
  {
    std::cerr << "<INVALID>\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file)
  {
    std::cerr << "<INVALID>\n";
    return 1;
  }

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

  cmds["AREA"] = std::bind(filonova::area, std::ref(std::cin), std::ref(std::cout), std::cref(plgs));
  cmds["MAX"] = std::bind(filonova::max, std::ref(std::cin), std::ref(std::cout), std::cref(plgs));
  cmds["MIN"] = std::bind(filonova::min, std::ref(std::cin), std::ref(std::cout), std::cref(plgs));
  cmds["COUNT"] = std::bind(filonova::count, std::ref(std::cin), std::ref(std::cout), std::cref(plgs));
  cmds["INTERSECTIONS"] = std::bind(filonova::intersections, std::ref(std::cin), std::ref(std::cout), std::cref(plgs));
  cmds["RECTS"] = std::bind(filonova::rects, std::ref(std::cout), std::cref(plgs));

  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      auto it = cmds.find(command);
      if (it != cmds.end())
      {
        it->second();
        std::cout << "\n";
      }
      else
      {
        std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
        std::cout << "<INVALID COMMAND>\n";
      }
    }
    catch (const std::logic_error &e)
    {
      if (std::cin.fail())
      {
        std::cin.clear(std::cin.rdstate() ^ std::ios::failbit);
      }
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  return 0;
}
