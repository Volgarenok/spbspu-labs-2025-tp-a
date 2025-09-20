#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>
#include "polygon.hpp"

namespace guseynov
{
  namespace utils
  {
    bool isNumber(const std::string& s);
    double calculateArea(const Polygon& poly);
    Polygon parsePolygon(const std::string& line);
    std::vector< Polygon > readPolygonsFromFile(const std::string& filename);
  }
}

#endif
