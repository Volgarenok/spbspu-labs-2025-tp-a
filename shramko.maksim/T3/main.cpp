#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <functional>
#include <algorithm>
#include <iterator>
#include <limits>
#include <ios>
#include "polygon.hpp"
#include "printCmd.hpp"

namespace shramko
{
  struct PrintAreaCommand
  {
    const std::vector< Polygon >& polygons;
    PrintAreaCommand(const std::vector< Polygon >& p):
      polygons(p)
    {}

    void operator()()
    {
      printArea(polygons, std::cin, std::cout);
    }
  };

  struct PrintMaxCommand
  {
    const std::vector< Polygon >& polygons;
    PrintMaxCommand(const std::vector< Polygon >& p):
      polygons(p)
    {}

    void operator()()
    {
      printMax(polygons, std::cin, std::cout);
    }
  };

  struct PrintMinCommand
  {
    const std::vector< Polygon >& polygons;
    PrintMinCommand(const std::vector< Polygon >& p):
      polygons(p)
    {}

    void operator()()
    {
      printMin(polygons, std::cin, std::cout);
    }
  };

  struct PrintCountCommand
  {
    const std::vector< Polygon >& polygons;
    PrintCountCommand(const std::vector< Polygon >& p):
      polygons(p)
    {}

    void operator()()
    {
      printCount(polygons, std::cin, std::cout);
    }
  };

  struct PrintLessAreaCommand
  {
    const std::vector< Polygon >& polygons;
    PrintLessAreaCommand(const std::vector< Polygon >& p):
      polygons(p)
    {}

    void operator()()
    {
      printLessArea(polygons, std::cin, std::cout);
    }
  };

  struct PrintRightShapesCommand
  {
    const std::vector< Polygon >& polygons;
    PrintRightShapesCommand(const std::vector< Polygon >& p):
      polygons(p)
    {}

    void operator()()
    {
      printRightShapes(polygons, std::cout);
    }
  };
}

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "Usage: program <input_file>\n";
    return 1;
  }

  std::ifstream inputFile(argv[1]);
  if (!inputFile)
  {
    std::cerr << "Cannot open input file\n";
    return 1;
  }

  std::vector< shramko::Polygon > polygons;
  while (!inputFile.eof())
  {
    std::copy(std::istream_iterator< shramko::Polygon >(inputFile),
      std::istream_iterator< shramko::Polygon >(), std::back_inserter(polygons));
    if (inputFile.fail())
    {
      inputFile.clear();
      inputFile.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  using cmd_func = std::function< void() >;
  std::map< std::string, cmd_func > commands;

  commands["AREA"] = shramko::PrintAreaCommand(polygons);
  commands["MAX"] = shramko::PrintMaxCommand(polygons);
  commands["MIN"] = shramko::PrintMinCommand(polygons);
  commands["COUNT"] = shramko::PrintCountCommand(polygons);
  commands["LESSAREA"] = shramko::PrintLessAreaCommand(polygons);
  commands["RIGHTSHAPES"] = shramko::PrintRightShapesCommand(polygons);

  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      commands.at(cmd)();
      std::cout << '\n';
    }
    catch (const std::exception&)
    {
      if (std::cin.fail())
      {
        std::cin.clear(std::cin.rdstate() ^ std::ios::failbit);
      }
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  return 0;
}
