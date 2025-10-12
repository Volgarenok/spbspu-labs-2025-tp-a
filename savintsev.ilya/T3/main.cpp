#include <iostream>
#include <fstream>
#include <functional>
#include <limits>
#include <string>
#include <map>
#include "shapes.h"
#include "shapes-cmds.h"

int main(int argc, char * argv[])
{
  if (argc != 2)
  {
    std::cerr << "ERROR: Invalid number of arguments\n";
    return 1;
  }

  using namespace savintsev;

  std::ifstream file(argv[1]);
  if (!file)
  {
    std::cerr << "ERROR: Can't open file\n";
    return 1;
  }

  std::vector< Polygon > data;

  while (!file.eof())
  {
    Polygon p;
    file >> p;
    if (!file.fail())
    {
      data.push_back(p);
      continue;
    }
    file.clear(file.rdstate() ^ std::ios::failbit);
    file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }

  std::map< std::string, std::function< void() > > cmds;
  cmds["AREA"] = std::bind(area, std::ref(std::cin), std::ref(std::cout), std::cref(data));
  cmds["MAX"] = std::bind(max, std::ref(std::cin), std::ref(std::cout), std::cref(data));
  cmds["MIN"] = std::bind(min, std::ref(std::cin), std::ref(std::cout), std::cref(data));
  cmds["COUNT"] = std::bind(count, std::ref(std::cin), std::ref(std::cout), std::cref(data));
  cmds["INTERSECTIONS"] = std::bind(intersections, std::ref(std::cin), std::ref(std::cout), std::cref(data));
  cmds["SAME"] = std::bind(same, std::ref(std::cin), std::ref(std::cout), std::cref(data));
  cmds["PRINT"] = std::bind(print, std::ref(std::cout), std::cref(data));

  for (std::string command; std::cin >> command;)
  {
    try
    {
      cmds.at(command)();
    }
    catch(...)
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
