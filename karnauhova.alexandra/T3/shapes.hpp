#ifndef SHAPES_HPP
#define SHAPES_HPP
#include <iostream>
#include <vector>

namespace karnauhova
{
  struct Point
  {
    int x, y;
    bool operator==(const Point& rhs) const;
  };

  struct CalcAreaPoint
  {
    double operator()(const Point& p1, const Point& p2);
  };

  struct Polygon
  {
    std::vector< Point > points;
    bool operator==(const Polygon& rhs) const;
  };

  std::istream& operator>>(std::istream& in, Point& point);
  std::istream& operator>>(std::istream& in, Polygon& pol);
  double getArea(const Polygon& polygon);
}
#endif
