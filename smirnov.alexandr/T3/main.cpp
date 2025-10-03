#include <fstream>
#include <iterator>
#include <fstream>
#include <iterator>
#include <limits>
#include <map>
#include <functional>
#include "polygon.hpp"
#include "commands.hpp"

int main(int argc, char * argv[])
{
  using namespace smirnov;
  if (argc != 2)
  {
    std::cerr << "Error: filename required\n";
    return 1;
  }
  std::ifstream file(argv[1]);
  if (!file)
  {
    std::cerr << "Error: can't open file\n";
    return 1;
  }
  std::vector< Polygon > polygons;
  while (!file.eof())
  {
    using istream_it = std::istream_iterator< Polygon >;
    std::copy(istream_it{file}, istream_it{}, std::back_inserter(polygons));
    if (file.fail())
    {
      file.clear(file.rdstate() ^ std::ios::failbit);
      file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::map< std::string, std::function< void() > > cmds;
  cmds["AREA"] = std::bind(printArea, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["MAX"] = std::bind(printMax, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["MIN"] = std::bind(printMin, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["COUNT"] = std::bind(printCount, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["INFRAME"] = std::bind(printInFrame, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["MAXSEQ"] = std::bind(printMaxSeq, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  std::string cmd;
  while (!(std::cin >> cmd).eof())
  {
    try
    {
      cmds.at(cmd)();
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
