#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <iostream>
#include <vector>

constexpr size_t MIN_VERTEX_COUNT = 3;

namespace filonova
{
  struct Point
  {
    int x, y;
  };
  std::istream &operator>>(std::istream &in, Point &point);
  bool operator<(const Point &lhs, const Point &rhs);

  struct Polygon
  {
    std::vector< Point > points;
  };
  std::istream &operator>>(std::istream &in, Polygon &polygon);

  double triangleArea(const Point &a, const Point &b, const Point &c);
  double getArea(const Polygon &polygon);
}

#endif
