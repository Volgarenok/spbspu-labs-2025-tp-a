#include <iostream>
#include <fstream>
#include "polygon.hpp"
#include "command_handler.hpp"

int main(int argc, char **argv)
{
  using puzikov::Polygon;

  if (argc != 2)
  {
    std::cerr << "You need to provide a filename 👺\n";
    return 1;
  }

  std::ifstream inputFile {argv[1]};
  if (!inputFile)
  {
    std::cerr << "Couldn't open the file 😢\n";
    return 1;
  }

  std::vector< puzikov::Polygon > polygons;
  puzikov::readPolygons(inputFile, polygons);
  inputFile.close();

  puzikov::CommandHandler cmdHandler(polygons);
  cmdHandler.readCommands(std::cin, std::cout);
}
