#ifndef SHAPES_HPP
#define SHAPES_HPP
#include <vector>
#include <iostream>

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

  bool insertIfNotNextEnter(std::istream& in, const Point& point, std::vector< Point >& dop);
}
#endif
