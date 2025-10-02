#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "polygon.hpp"

#include <iostream>
#include <string>

constexpr size_t RECTANGLE_SIDES = 4;
constexpr double EPS = 1e-9;

namespace filonova
{
  bool hasVertexCount(const Polygon &p, size_t count);
  bool isRectangle(const filonova::Polygon &polygon);

  void area(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void max(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void min(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void count(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void intersections(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void rects(std::ostream &out, const std::vector< Polygon > &polygons);
}

#endif
