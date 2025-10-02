#include <functional>
#include <limits>
#include <map>
#include <string>

#include "Commands.h"
#include "FillVectorOfShapes.h"
#include "Shapes.h"

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "<ERROR>\n";
    return 1;
  }

  std::ifstream filename;
  filename.open(argv[1]);
  if (!filename.is_open())
  {
    std::cerr << "<NO SUCH FILE>\n";
    return 1;
  }

  std::vector< voronina::Polygon > shapes = voronina::fillVectorOfShapes(filename);

  std::map< std::string, std::function< void() > > cmds;
  cmds["AREA"] = std::bind(voronina::area, std::cref(shapes), std::ref(std::cin), std::ref(std::cout));
  cmds["MAX"] = std::bind(voronina::max, std::cref(shapes), std::ref(std::cin), std::ref(std::cout));
  cmds["MIN"] = std::bind(voronina::min, std::cref(shapes), std::ref(std::cin), std::ref(std::cout));
  cmds["COUNT"] = std::bind(voronina::count, std::cref(shapes), std::ref(std::cin), std::ref(std::cout));
  cmds["MAXSEQ"] = std::bind(voronina::maxseq, std::cref(shapes), std::ref(std::cin), std::ref(std::cout));
  cmds["RIGHTSHAPES"] = std::bind(voronina::rightshapes, std::cref(shapes), std::ref(std::cout));

  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      cmds.at(command)();
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
