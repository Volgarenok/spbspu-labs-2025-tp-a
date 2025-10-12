#ifndef POLYGON_HPP
#define POLYGON_HPP
#include <istream>
#include <ostream>
#include <vector>

namespace kiselev
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

  std::istream& operator>>(std::istream&, Point&);
  std::istream& operator>>(std::istream&, Polygon&);
  double getArea(const Polygon&);
  bool isRect(const Polygon&);

}
#endif
