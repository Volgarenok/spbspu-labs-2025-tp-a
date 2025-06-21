#ifndef POLYGON_H
#define POLYGON_H
#include <iostream>
#include <vector>

namespace mezentsev
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

  std::istream& operator>>(std::istream&, Point&);
  std::istream& operator>>(std::istream&, Polygon&);
  double getArea(const Polygon&);
}
#endif
