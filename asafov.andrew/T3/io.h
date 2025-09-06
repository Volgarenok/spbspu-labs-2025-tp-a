#ifndef IO_H
#define IO_H

#include <string>
#include <vector>
#include "geometry.h"

namespace asafov
{
  std::vector<Polygon> readPolygonsFromFile(const std::string& filename);
  Polygon parsePolygonFromString(const std::string& str);

  bool parsePoint(const std::string& str, size_t& pos, Point& point);
  bool parseNumber(const std::string& str, size_t& pos, int& number);
  void skipWhitespace(const std::string& str, size_t& pos);
}

#endif
