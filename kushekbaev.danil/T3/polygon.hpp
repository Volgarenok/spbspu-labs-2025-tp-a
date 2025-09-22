#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <vector>
#include <iosfwd>

namespace kushekbaev
{
  constexpr size_t MIN_NUMBER_OF_VERTICES_IN_POLYGON = 3;

  struct Point
  {
    int x, y;
  };

  struct Polygon
  {
    std::vector< Point > points;
  };

  std::istream& operator>>(std::istream& in, Point& point);
  std::istream& operator>>(std::istream& in, Polygon& polygon);
  std::ostream& operator<<(std::ostream& out, const Point& point);
  std::ostream& operator<<(std::ostream& out, const Polygon& polygon);
}

#endif
