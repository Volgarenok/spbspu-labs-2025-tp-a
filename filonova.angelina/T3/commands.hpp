#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "polygon.hpp"

#include <algorithm>
#include <functional>
#include <iomanip>
#include <numeric>
#include <string>

namespace filonova
{
  void area(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void max(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void min(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void count(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void intersections(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void rects(std::ostream &out, const std::vector< Polygon > &polygons);
}

#endif
