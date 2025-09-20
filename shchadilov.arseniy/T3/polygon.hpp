#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <iostream>
#include <vector>
#include "delimiter.hpp"

namespace shchadilov
{
  struct Point
  {
    int x, y;
    bool operator==(const Point& other) const;
  };

  struct Polygon
  {
    std::vector< Point > points;
    bool operator==(const Polygon& other) const;
  };

  std::istream& operator>>(std::istream& in, Point& dest);
  std::istream& operator>>(std::istream& in, Polygon& dest);
  double getArea(const Polygon& poly);
}

#endif
