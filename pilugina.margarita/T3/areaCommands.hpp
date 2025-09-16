#ifndef AREA_COMMAND_HPP
#define AREA_COMMAND_HPP

#include <iostream>
#include <vector>
#include "polygon.hpp"

namespace pilugina
{
  double getTriangleArea(const Point &, const Point &, const Point &);
  double getPolygonArea(const Polygon &);
  bool isEvenVertexNum(const Polygon &);
  bool isOddVertexNum(const Polygon &);
  bool isEqualVertexNum(std::size_t, const Polygon &);
  double calcAreasSum(const std::vector< Polygon > &);
  double getEvenArea(const std::vector< Polygon > &);
  double getOddArea(const std::vector< Polygon > &);
  double printMeanArea(const std::vector< Polygon > &);
  double calcAreasEqVert(const std::vector< Polygon > &, int);
  double calcAreas(const std::vector< Polygon > &, std::istream &);
}

#endif
