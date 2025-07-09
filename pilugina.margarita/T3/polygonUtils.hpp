#ifndef UTILS_HPP
#define UTILS_HPP

#include "polygon.hpp"

namespace pilugina
{
  double getPolygonArea(const Polygon &);
  bool isEvenVertexNum(const Polygon &);
  bool isOddVertexNum(const Polygon &);
  bool isEqualVertexNum(std::size_t, const Polygon &);

  double areaSumOperator(double, const Polygon &);
}

#endif
