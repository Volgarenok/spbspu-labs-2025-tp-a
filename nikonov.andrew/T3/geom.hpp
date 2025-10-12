#ifndef GEOM_H
#define GEOM_H
#include <vector>
#include <istream>
#include <ostream>
namespace nikonov
{
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

  bool operator==(const Point& a, const Point& b);
  bool operator==(const Polygon& a, const Polygon& b);
}
#endif
