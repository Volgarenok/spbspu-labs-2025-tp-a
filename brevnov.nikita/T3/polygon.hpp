#ifndef POLYGON_HPP
#define POLYGON_HPP
#include <istream>
#include <ostream>
#include <vector>
namespace brevnov
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
  double get_area(const Polygon&);
  bool has_right_angle(const Polygon&);
}
#endif
