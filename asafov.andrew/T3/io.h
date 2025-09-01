#ifndef IO_H
#define IO_H

#include <string>
#include <vector>
#include "geometry.h"

namespace asafov
{
  std::vector<Polygon> readPolygonsFromFile(const std::string& filename);
  Polygon parsePolygonFromString(const std::string& str);
}

#endif
