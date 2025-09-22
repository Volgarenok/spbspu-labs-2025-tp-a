#ifndef SHAPES_H
#define SHAPES_H

#include <iostream>
#include <vector>

namespace voronina
{
  struct Point
  {
    int x, y;
    bool operator==(const Point& point) const;
  };

  Point operator-(const Point& left, const Point& right);
  std::istream& operator>>(std::istream& in, Point& point);

  double distance(const Point& left, const Point& right);

  struct Polygon
  {
    std::vector< Point > points;
    bool operator==(const Polygon& polygon) const;
  };

  std::istream& operator>>(std::istream& in, Polygon& polygon);

}
#endif
