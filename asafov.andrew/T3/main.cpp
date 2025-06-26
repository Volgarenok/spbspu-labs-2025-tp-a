#include <iostream>
#include "io.h"
#include "functions.h"

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }

  try
  {
    auto polygons = asafov::readPolygonsFromFile(argv[1]);
    std::string cmd;
    while (std::getline(std::cin, cmd))
    {
      asafov::processCommand(polygons, cmd);
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
  }

  return 0;
}
