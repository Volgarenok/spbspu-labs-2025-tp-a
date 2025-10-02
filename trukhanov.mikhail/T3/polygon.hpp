#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <vector>
#include <iostream>
#include <data_input.hpp>

namespace trukhanov
{
  struct Point
  {
    int x, y;
  };

  std::istream& operator>>(std::istream& in, Point& point);
  std::ostream& operator<<(std::ostream& out, const Point& point);
  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
  bool operator<(const Point& lhs, const Point& rhs);
  bool operator==(const Point& lhs, const Point& rhs);

  struct Polygon
  {
    std::vector< Point > points;
  };

  std::istream& operator>>(std::istream& in, Polygon& polygon);
  std::ostream& operator<<(std::ostream& out, const Polygon& polygon);

  struct Angle
  {
    Point a;
    Point b;
    Point c;
  };

  double getArea(const Polygon& p);
  bool isEven(const Polygon& p);
  bool isOdd(const Polygon& p);
  bool compareByVertexes(const Polygon& p1, const Polygon& p2);
  bool isRight(const Polygon& p);
}

#endif
