#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP
#include <vector>
#include <istream>

namespace demehin
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
  std::istream& operator>>(std::istream&, Point&);
  std::istream& operator>>(std::istream&, Polygon&);

  double getPlgArea(const Polygon&);
}

#endif
