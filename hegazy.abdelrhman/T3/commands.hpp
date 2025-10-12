#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "geometry.hpp"

#include <iostream>
#include <vector>

namespace bob
{
  void printAreaSum(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void printMaxValueOf(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void printMinValueOf(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void printCountOf(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void printLessArea(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void printIntersections(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
} // namespace bob
#endif
