#ifndef POLYGON_HPP
#define POLYGON_HPP
#include <iostream>
#include <vector>
#include <cstddef>

namespace smirnov
{
  struct Point
  {
    int x, y;
  };
  struct Polygon
  {
    std::vector< Point > points;
  };
  std::istream & operator>>(std::istream & in, Point & point);
  std::ostream & operator<<(std::ostream & out, const Point & point);
  bool operator==(const Point & lhs, const Point & rhs);
  std::istream & operator>>(std::istream & in, Polygon & polygon);
  bool operator==(const Polygon & lhs, const Polygon & rhs);
}
#endif
