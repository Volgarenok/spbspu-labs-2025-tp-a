#ifndef GEOM_H
#define GEOM_H

#include <vector>
#include <iostream>

namespace fedorova
{
  struct Point
  {
    int x;
    int y;
  };

  struct Polygon
  {
    std::vector< Point > points;
  };

  bool operator==(const Point&, const Point&);
  bool operator==(const Polygon&, const Polygon&);
  Point operator-(const Point&, const Point&);
  std::istream& operator>>(std::istream&, Point&);
  std::istream& operator>>(std::istream&, Polygon&);
  std::ostream& operator<<(std::ostream&, const Point&);
  std::ostream& operator<<(std::ostream&, const Polygon&);
}

#endif

