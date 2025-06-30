#ifndef SHAPES_H
#define SHAPES_H

#include <vector>
#include <iostream>

namespace voronina
{
  struct Point
  {
    int x, y;
    bool operator==(const Point& point) const;
  };

  std::istream& operator>>(std::istream& in, Point& point);

  struct Polygon
  {
    std::vector< Point > points;
    bool operator==(const Polygon& polygon) const;
  };

  std::istream& operator>>(std::istream& in, Polygon& polygon);

}
#endif
