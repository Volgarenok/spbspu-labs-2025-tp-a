#ifndef SHAPES_HPP
#define SHAPES_HPP
#include <vector>
#include <iostream>
#include <iterator>

namespace alymova
{
  struct Point
  {
    int x, y;
  };
  struct Polygon
  {
    std::vector< Point > points;
  };

  std::istream& operator>>(std::istream& in, Point& point);
  std::istream& operator>>(std::istream& in, Polygon& polygon);
  std::ostream& operator<<(std::ostream& out, const Point& point);
  std::ostream& operator<<(std::ostream& out, const Polygon& polygon);

  Point checkNextEnter(std::istream& in, const Point& point);
}
#endif
