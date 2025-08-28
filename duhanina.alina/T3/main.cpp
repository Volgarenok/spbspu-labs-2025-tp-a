#include <limits>
#include <map>
#include <iterator>
#include <fstream>
#include "commands.hpp"

int main(int argc, char* argv[])
{
  using namespace duhanina;
  using isIter = std::istream_iterator< Polygon >;

  if (argc != 2)
  {
    return 1;
  }

  std::ifstream file(argv[1]);
  std::vector< Polygon > polygons;
  while (!file.eof())
  {
    std::copy(isIter(file), isIter(), std::back_inserter(polygons));
    if (!file)
    {
      file.clear();
      file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  std::map< std::string, std::function< void() > > cmds;
  cmds["AREA"] = std::bind(printAreaSum, std::ref(std::cin), std::cref(polygons), std::ref(std::cout));
  cmds["MAX"] = std::bind(printMaxValue, std::ref(std::cin), std::cref(polygons), std::ref(std::cout));
  cmds["MIN"] = std::bind(printMinValue, std::ref(std::cin), std::cref(polygons), std::ref(std::cout));
  cmds["COUNT"] = std::bind(printCount, std::ref(std::cin), std::cref(polygons), std::ref(std::cout));
  cmds["RMECHO"] = std::bind(eraseRmecho, std::ref(std::cin), std::ref(polygons), std::ref(std::cout));
  cmds["LESSAREA"] = std::bind(printLessArea, std::ref(std::cin), std::cref(polygons), std::ref(std::cout));

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
