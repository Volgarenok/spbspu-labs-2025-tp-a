#include <fstream>
#include <limits>
#include "commands_processing.hpp"

int main(int argc, char* argv[])
{
  using namespace lanovenko;
  using istreamIter = std::istream_iterator< Polygon >;
  if (argc != 2)
  {
    std::cout << "<INCORRENT ARGUMENTS COUNT>\n";
    return 1;
  }
  std::ifstream file(argv[1]);
  if (!file.is_open())
  {
    std::cout << "<INPUT FAIL>\n";
    return 1;
  }
  std::vector< Polygon > polygons;
  while (!file.eof())
  {
    std::copy(istreamIter{ file }, istreamIter{}, std::back_inserter(polygons));
    if (file.fail())
    {
      file.clear();
      file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  std::map< std::string, std::function< void() > > cmds;
  cmds["AREA"] = std::bind(printArea, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["MAX"] = std::bind(printMax, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["MIN"] = std::bind(printMin, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["COUNT"] = std::bind(printCount, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["RMECHO"] = std::bind(printRmecho, std::ref(std::cin), std::ref(std::cout), std::ref(polygons));
  cmds["RIGHTSHAPES"] = std::bind(printRightShapes, std::ref(std::cout), std::cref(polygons));
  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      cmds.at(command)();
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
