#include <iostream>
#include <string>
#include "io.h"
#include "functions.h"

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }

  try
  {
    std::vector<asafov::Polygon> polygons = asafov::readPolygonsFromFile(argv[1]);
    std::string command;

    while (std::getline(std::cin, command))
    {
      if (command.empty())
      {
        continue;
      }
      asafov::processCommand(polygons, command);
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
  }

  return 0;
}
