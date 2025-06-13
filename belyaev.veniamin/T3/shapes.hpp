#ifndef SHAPES_HPP
#define SHAPES_HPP
#include <iostream>
#include <vector>

namespace belyaev
{
  struct Point
  {
    int x, y;
  };

  struct Polygon
  {
    std::vector<Point> points;
  };

  std::istream& operator>>(std::istream& in, Point& dest);
  std::ostream& operator<<(std::ostream& out, const Point& src);
  Point checkNext(const Point& pnt, std::istream& in, bool& result);
  std::istream& operator>>(std::istream& in, Polygon& dest);
  std::ostream& operator<<(std::ostream& out, const Polygon& src);

  bool operator==(const Point& lhs, const Point& rhs);
  bool operator==(const Polygon& lhs, const Polygon& rhs);
}

#endif
