#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <iosfwd>
#include <vector>

namespace zholobov {

  struct Point {
    int x;
    int y;
  };

  struct Polygon {
    std::vector< Point > points;
  };

  std::istream& operator>>(std::istream& input, Point& point);
  std::istream& operator>>(std::istream& input, Polygon& polygon);

  using Polygons = std::vector< zholobov::Polygon >;

}

#endif
