#ifndef SHAPES_HPP
#define SHAPES_HPP

#include <istream>
#include <vector>

namespace maslevtsov {
  struct Point
  {
    int x, y;
  };

  struct Polygon
  {
    std::vector< Point > points;
  };

  std::istream& operator>>(std::istream& in, Point& rhs);
  std::istream& operator>>(std::istream& in, Polygon& rhs);

  bool operator==(const Point& lhs, const Point& rhs);
}

#endif
