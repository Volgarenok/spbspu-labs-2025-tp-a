#include <vector>
#include <fstream>
#include <iostream>
#include "cmds_run.h"

int main(int argc, char* argv[])
{
  using ohantsev::Polygon;
  using ohantsev::fillPolygons;
  using ohantsev::cmdsHandle;
  if (argc < 2)
  {
    std::cerr << "Empty filename\n";
    return 1;
  }
  std::ifstream in(argv[1]);
  if (!in.is_open())
  {
    std::cout << "File not found\n";
    return 1;
  }
  std::vector< Polygon > polygons;
  fillPolygons(polygons, in);
  cmdsHandle(polygons, std::cin, std::cout);
}
