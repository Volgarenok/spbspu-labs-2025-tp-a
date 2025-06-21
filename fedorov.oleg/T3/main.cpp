#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <limits>

#include "polygon.hpp"
#include "commands.hpp"
#include "functional.hpp"

using std::placeholders::_1;
using std::placeholders::_2;

int main(int argc, char *argv[])
{
  using namespace fedorov;

  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }

  std::vector< Polygon > polygons;
  try
  {
    std::ifstream file(argv[1]);
    if (!file)
    {
      throw std::runtime_error("Failed to open file");
    }
    readPolygons(file, polygons);
  }
  catch (const std::exception &e)
  {
    std::cerr << "Error: " << e.what() << '\n';
    return 2;
  }

  std::map< std::string, std::function< void(std::istream &, std::ostream &) > > commands{
      {"AREA", std::bind(areaCommand, _1, _2, std::cref(polygons))},
      {"LESSAREA", std::bind(LessAreaCommand, _1, _2, std::ref(polygons))},
      {"MAX", std::bind(maxCommand, _1, _2, std::cref(polygons))},
      {"MIN", std::bind(minCommand, _1, _2, std::cref(polygons))},
      {"ECHO", std::bind(EchoCommand, _1, _2, std::ref(polygons))},
      {"COUNT", std::bind(countCommand, _1, _2, std::cref(polygons))}};

  std::string cmd;
  while (std::getline(std::cin, cmd))
  {
    if (cmd.empty())
    {
      continue;
    }

    std::istringstream iss(cmd);
    std::string command;
    iss >> command;

    try
    {
      auto it = commands.find(command);
      if (it != commands.end())
      {
        it->second(iss, std::cout);
      }
      else
      {
        throw std::invalid_argument("Unknown command");
      }
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
      iss.clear();
      iss.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}
