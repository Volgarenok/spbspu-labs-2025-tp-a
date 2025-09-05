#ifndef POLYGON_HPP
#define POLYGON_HPP

#include "input_struct.hpp"
#include <vector>
#include <istream>

constexpr size_t MIN_VERTEX_COUNT = 3;

namespace shiryaeva
{
  struct Point
  {
    int x, y;
  };

  struct Polygon
  {
    std::vector< Point > points;
  };

  struct VectorProduct
  {
    double operator()(const Point& p1, const Point& p2) const
    {
      return p1.x * p2.y - p2.x * p1.y;
    }
  };

  bool operator==(const Point& lhs, const Point& rhs);
  bool operator<(const Point& lhs, const Point& rhs);

  std::istream& operator>>(std::istream& in, Point& point);
  std::istream& operator>>(std::istream& in, Polygon& polygon);
  double getArea(const Polygon& polygon);
}

#endif
