#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "geom.hpp"
#include <iosfwd>
#include <vector>

namespace ageev
{
  void area(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void max(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void min(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void count(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void intersections(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void rightshapes(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
}

#endif
