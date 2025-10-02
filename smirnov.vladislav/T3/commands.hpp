#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iostream>
#include <vector>
#include "geometry.hpp"

namespace smirnov
{
  void printAreaSum(std::istream& input, const std::vector< geom::Polygon >& polygons, std::ostream& output);
  void printMaxValueOf(std::istream& input, const std::vector< geom::Polygon >& polygons, std::ostream& output);
  void printMinValueOf(std::istream& input, const std::vector< geom::Polygon >& polygons, std::ostream& output);
  void printCountOf(std::istream& input, const std::vector< geom::Polygon >& polygons, std::ostream& output);
  void printLessAreaCnt(std::istream& input, const std::vector< geom::Polygon >& polygons, std::ostream& output);
  bool polygonsIntersect(const geom::Polygon& p1, const geom::Polygon& p2);
  void printIntersectionsCnt(std::istream& input, const std::vector< geom::Polygon >& polygons, std::ostream& output);
}

#endif
