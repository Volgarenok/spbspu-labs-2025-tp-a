#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <iostream>
#include <vector>

namespace fedorov
{
  struct Point
  {
    int x, y;
  };
  bool operator==(const Point &p1, const Point &p2);
  std::istream &operator>>(std::istream &in, Point &dest);
  std::ostream &operator<<(std::ostream &out, const Point &src);

  struct Polygon
  {
    std::vector< Point > points;
  };
  bool operator==(const Polygon &p1, const Polygon &p2);
  bool operator!=(const Polygon &p1, const Polygon &p2);
  std::istream &operator>>(std::istream &in, Polygon &dest);
  std::ostream &operator<<(std::ostream &out, const Polygon &src);
}

#endif
