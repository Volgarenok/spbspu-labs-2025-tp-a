#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <string>
#include "polygon.hpp"

namespace shiryaeva
{
  constexpr size_t RECTANGLE_SIDES = 4;
  constexpr double EPS = 1e-9;

  bool hasVertexCount(const Polygon &p, size_t count);
  bool isRectangle(const Polygon &polygon);
  bool HasDuplicates(const Polygon& p);
  void area(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void max(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void min(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void count(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void lessArea(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void intersections(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
}

#endif
