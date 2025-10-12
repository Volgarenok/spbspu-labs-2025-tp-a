#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <vector>
#include <istream>

namespace holodilov
{
  struct Point
  {
    int x;
    int y;

    bool operator==(const Point& rhs) const;
  };

  struct Polygon
  {
    std::vector< Point > points;

    bool operator==(const Polygon& rhs) const;

    double getArea() const;
  };

  std::istream& operator>>(std::istream& is, Point& point);

  std::istream& operator>>(std::istream& is, Polygon& polygon);

  using VecPolygons = std::vector< Polygon >;
}

#endif
