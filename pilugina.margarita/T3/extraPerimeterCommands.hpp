#ifndef EXTRA_PERIMETER_COMMANDS_HPP
#define EXTRA_PERIMETER_COMMANDS_HPP

#include <iostream>
#include <vector>
#include "polygon.hpp"

namespace pilugina
{
  double getLineSize(const Point &p1, const Point &p2);
  double getPolygonPerimeter(const Polygon &poly);
  double prmtrSumOperator(double init, const Polygon &poly);
  bool isEvenVertexNumP(const Polygon &poly);
  bool isOddVertexNumP(const Polygon &poly);
  bool isEqualVertexNum(std::size_t n, const Polygon &poly);
  double calcPerimetersSum(const std::vector< Polygon > &polys);
  double getEvenPerimeter(const std::vector< Polygon > &polys);
  double getOddPerimeter(const std::vector< Polygon > &polys);
  double calcPerimetersEqVert(const std::vector< Polygon > &polys, int vertex_num);
  double calcPerimeters(const std::vector< Polygon > &polys, std::istream &in);
}

#endif
