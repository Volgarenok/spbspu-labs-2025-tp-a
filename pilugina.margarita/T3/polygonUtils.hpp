#ifndef UTILS_HPP
#define UTILS_HPP

#include "polygon.hpp"

namespace pilugina
{
  struct PolygonAreaAccumulator
  {
    PolygonAreaAccumulator(const Polygon &p);
    double operator()(double acc, const Point &p1) const;

  private:
    const Polygon &poly;
    size_t n;
  };

  double getPolygonArea(const Polygon &);
  bool isEvenVertexNum(const Polygon &);
  bool isOddVertexNum(const Polygon &);
  bool isEqualVertexNum(std::size_t, const Polygon &);

  double areaSumOperator(double, const Polygon &);
}

#endif
