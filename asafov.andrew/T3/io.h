#ifndef IO_H
#define IO_H
#include <string>
#include "geometry.h"
namespace asafov
{
  std::vector<Polygon> readPolygonsFromFile(const std::string& filename);
}
#endif
