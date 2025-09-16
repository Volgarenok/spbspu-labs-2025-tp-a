#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP
#include <iosfwd>
#include <vector>

namespace kostyukov
{
  struct Point
  {
    int x;
    int y;
  };

  std::istream& operator>>(std::istream&, Point&);
  bool operator==(const Point&, const Point&);

  struct Polygon
  {
    std::vector< Point > points;
  };

  std::istream& operator>>(std::istream&, Polygon&);
  double getArea(const Polygon&);
}
#endif
