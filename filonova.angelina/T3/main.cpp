#include "commands.hpp"

#include <fstream>
#include <iterator>
#include <limits>
#include <map>

using namespace filonova;

int main(int argc, char *argv[])
{
  using namespace filonova;

  if (argc != 2)
  {
    std::cerr << "<INVALID COMMAND>\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file.is_open())
  {
    std::cerr << "<INVALID COMMAND>\n";
    return 1;
  }

  std::vector< Polygon > polygons;
  Polygon p;
  while (file >> p)
  {
    polygons.push_back(p);
  }

  if (!file.eof())
  {
    std::cerr << "<INVALID COMMAND>\n";
    return 1;
  }

  std::map< std::string, std::function< void(std::istream &, std::ostream &) > > cmds;
  {
    using namespace std::placeholders;
    cmds["AREA"] = std::bind(filonova::area, _1, _2, std::cref(polygons));
    cmds["MAX"] = std::bind(filonova::max, _1, _2, std::cref(polygons));
    cmds["MIN"] = std::bind(filonova::min, _1, _2, std::cref(polygons));
    cmds["COUNT"] = std::bind(filonova::count, _1, _2, std::cref(polygons));
    cmds["INTERSECTIONS"] = std::bind(filonova::intersections, _1, _2, std::cref(polygons));
    cmds["RECTS"] = std::bind(filonova::rects, _2, std::cref(polygons));
  }

  std::cout << std::fixed << std::setprecision(1);

  std::string command;
  while (std::cin >> command)
  {
    try
    {
      auto it = cmds.find(command);
      if (it != cmds.end())
      {
        it->second(std::cin, std::cout);
      }
      else
      {
        std::cout << "<INVALID COMMAND>";
      }
      std::cout << '\n';
    }
    catch (const std::exception &e)
    {
      std::cout << "<INVALID COMMAND>" << '\n';
    }

    std::cin.clear();
    std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }

  return 0;
}
