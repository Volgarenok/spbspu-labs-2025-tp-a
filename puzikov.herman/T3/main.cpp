#include <iostream>
#include <fstream>
#include "polygon.hpp"
#include "command_handler.hpp"
#include "commands.hpp"

int main(int argc, char **argv)
{
  using namespace puzikov;

  if (argc != 2)
  {
    std::cerr << "You need to provide a filename\n";
    return 1;
  }

  std::ifstream inputFile{argv[1]};
  if (!inputFile)
  {
    std::cerr << "Couldn't open the file\n";
    return 1;
  }

  std::vector< Polygon > polygons;
  readPolygons(inputFile, polygons);
  inputFile.close();

  using std::placeholders::_1;
  using std::placeholders::_2;

  const CommandMap commands
  {
      {"AREA", std::bind(areaCommand, _1, _2, std::cref(polygons))},
      {"MAX", std::bind(maxCommand, _1, _2, std::cref(polygons))},
      {"MIN", std::bind(minCommand, _1, _2, std::cref(polygons))},
      {"COUNT", std::bind(countCommand, _1, _2, std::cref(polygons))},
      {"RMECHO", std::bind(rmEchoCommand, _1, _2, std::ref(polygons))},
      {"SAME", std::bind(sameCommand, _1, _2, std::cref(polygons))}
  };

  readCommands(std::cin, std::cout, commands);
}
