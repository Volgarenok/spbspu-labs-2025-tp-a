#ifndef SHAPES_HPP
#define SHAPES_HPP
#include <iostream>
#include <vector>
#include <iterator>

namespace cherepkov
{
  struct Point
  {
    int x, y;
    bool operator==(const Point & rhs) const;
    Point operator-(const Point & rhs) const;
  };
  struct Polygon
  {
    std::vector< Point > points;
  };

  std::istream& operator>>(std::istream& in, Point& point);
  std::istream& operator>>(std::istream& in, Polygon& polygon);
  Point handleNewLine(std::istream& in, const Point& point);
}
#endif
